/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: Implementation of CGparameter class for JetStream Cg Runtime

#define CG_LIB
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <Cg/CgCommon.h>
#include <Cg/cgBinary.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// structures -------------------------------------------

// globals ----------------------------------------------

static const CGenum var_table[] = {CG_VARYING, CG_UNIFORM, CG_CONSTANT, CG_MIXED};
static const CGenum dir_table[] = {CG_IN, CG_OUT, CG_INOUT, CG_ERROR};

PSGL_EXPORT CgparameterHookFunction _cgParameterCreateHook = NULL;
PSGL_EXPORT CgparameterHookFunction _cgParameterDestroyHook = NULL;

// helper functions -------------------------------------

static void _jsCgGetParameterResourceInfo( CgRuntimeParameter *rtParameter, int arrayIndex, CGresource *resource, unsigned long *resourceIndex, unsigned short* numEmbeddedConstants)
{
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry;
    const CgParameterResource *paramResource = _jsGetParameterResource( rtParameter->program, paramEntry );

    const unsigned short *paramResourceTable = rtParameter->program->resources;

    if (( paramEntry->flags & CGPV_MASK ) == CGPV_VARYING )
    {
        *resource = ( CGresource )paramResource->resource;
        *resourceIndex = -1U;
    }
    else if ((( paramEntry->flags & CGPV_MASK ) & CGPV_CONSTANT ) || (( paramEntry->flags & CGPV_MASK ) & CGPV_UNIFORM ) )
    {
        CGtype parameterResourceType = _jsGetParameterCGtype( rtParameter->program, paramEntry );
        // sampler types
        if ( parameterResourceType >= CG_SAMPLER1D && parameterResourceType <= CG_SAMPLERCUBE )
        {
            *resource = ( CGresource )paramResource->resource;
            *resourceIndex = 0;
        }
        // nonsampler types
        else
        {
            if ( parameterResourceType == CGP_SCF_BOOL )
                *resource = CG_B;
            else
                *resource = CG_C;

            bool bIsVertexProgram;
            switch ( rtParameter->program->header.profile )
            {
                case CG_PROFILE_SCE_VP_TYPEB:
                case ( CGprofile )7005:
                case CG_PROFILE_SCE_VP_RSX:
                    bIsVertexProgram = true;
                    break;
                default:
                    bIsVertexProgram = false;
                    break;
            }


            //get rowCount
            unsigned int rows = _jsGetTypeRowCount( parameterResourceType );
            if ( rows == 0 )
                rows = 1;

	    // no embedded constants for vertex, init to 0 for fragment
	    *numEmbeddedConstants = 0;

            // vertex programs
            if ( bIsVertexProgram )
            {
                // element of an array
                if ( arrayIndex >= 0 )
                {
                    if ( paramEntry->flags & CGP_CONTIGUOUS )
                    {
                        *resourceIndex = paramResource->resource + arrayIndex * rows;
                    }
                    else
                    {
                        //each matrix should be stored as just one element, so no reason to do a special case
                        //.. hum that's probably a false statement. in that case, it's all unfolded I think...
                        //so a matrix should take n prameters...
                        //I add the *row, I am almost sure it should be there, the runtime doesn't work for matrix of less
                        //than 4 row... I bet...
                        unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + arrayIndex * rows );
                        *resourceIndex = ( tempResIndex == 0xFFFF ) ? -1U : ( int )tempResIndex;
                    }
                }
                // not array
                else
                {
                    unsigned short tempResIndex = paramResource->resource;
                    *resourceIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                }
            }
            // fragment programs
            else
            {
                const unsigned short* resPtr = paramResourceTable + paramResource->resource;
                if ( arrayIndex < 0 )
                    arrayIndex = 0;

                int skipCount = arrayIndex * rows;
                while ( skipCount )
                {
                    resPtr++;
                    int embeddedCount = *( resPtr++ );
                    resPtr += embeddedCount;
                    skipCount--;
                }
                //resPtr points to the register resource of the arrayIndex
                unsigned short tempResIndex = *( resPtr );

		// accumulate total numEmbeddedConstants for all rows of a matrix
		while (rows)
		    {
			resPtr++; // skip register
			unsigned short ecCount = *resPtr; 
			resPtr++;
			*numEmbeddedConstants += ecCount;
			resPtr += ecCount;
			rows--;
		    }

                *resourceIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
            }
        }

    }
}

//this function advance of a certain number of parameters at the same level, so it skips whatever number
//of children parameters is needed.
//It assumes distance is smaller than the number of item left at the level or that we are at level 0
//which means that there is not check that we are not jumping one level higher.
//cgGetNextParameter will check that
static CGparameter _jsAdvanceParameter( CGparameter param, int distance )
{
    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return NULL;

    if ( distance == 0 )
        return param;

    //are we in a compact array
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if (( previousParameter->parameterEntry->flags & CGP_ARRAY ) &&
                !( previousParameter->parameterEntry->flags & CGP_UNROLLED ) )
        {
            //we have an array item
            int arrayIndex = CG_GETINDEX( param );
            arrayIndex += distance;
            const CgParameterArray *parameterArray =  _jsGetParameterArray( previousParameter->program, previousParameter->parameterEntry );
            int arraySize = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
            if ( arrayIndex < arraySize )
            {
                JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
                int ret = ( int )rtParameter->id;
                ret |= ( arrayIndex << CG_PARAMETERSIZE );
                return ( CGparameter )ret;
            }
            else
            {
                JS_ASSERT( 0 );
                return ( CGparameter )NULL;
            }
        }
    }

    //stop when the distance is 0
    const CgParameterEntry *endEntry = rtParameter->program->parametersEntries + rtParameter->program->rtParametersCount;
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry;

    while ( distance && paramEntry < endEntry )
    {
        switch ( paramEntry->flags & CGP_TYPE_MASK )
        {
            case CGP_ARRAY:
            {
                if ( paramEntry->flags & CGP_UNROLLED )
                {
                    const CgParameterArray *parameterArray = _jsGetParameterArray( rtParameter->program, paramEntry );
                    int arraySize = ( int )_jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );

                    //we are done with this item, advance
                    paramEntry++;
                    distance--;

                    paramEntry++; //skip the array type... do we have the array type when it's unrolled ???
                    distance += arraySize;
                }
                else
                {
                    //we are done with this item, advance
                    paramEntry++;
                    distance--;
                    paramEntry++; //skip the array type
                }
                break;
            }
            case CGP_STRUCTURE:
            {
                const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, paramEntry );
                //we are done with this item, advance
                paramEntry++;
                distance--;
                //add the member count
                distance += parameterStructure->memberCount;
                break;
            }
            case CGP_INTRINSIC:
                //we are done with this item, advance
                paramEntry++;
                distance--;
                break;
        }
    }

    if ( paramEntry < endEntry )
    {
        //get the index:
        size_t offset = ( paramEntry - rtParameter->parameterEntry );
        CgRuntimeParameter *nextParameter = rtParameter + offset;
        return nextParameter->id;
    }
    else
    {
        //case where we were at the root and there is no more items
        return ( CGparameter )NULL;
    }
}


#ifdef JS_DEBUG

void _jsCgParameterDump( CGparameter param, unsigned int indent )
{
    _jsCgPrintSpaces( indent ); printf( "Parameter: %d\n", cgGetParameterOrdinalNumber( param ) );
    _jsCgPrintSpaces( indent + 3 ); printf( "name = %s\n", cgGetParameterName( param ) );
    _jsCgPrintSpaces( indent + 3 ); printf( "type = %s\n", cgGetTypeString( cgGetParameterType( param ) ) );
    _jsCgPrintSpaces( indent + 3 ); printf( "resource = %s\n", cgGetResourceString( cgGetParameterResource( param ) ) );
    _jsCgPrintSpaces( indent + 3 ); printf( "resourceIndex = %ld\n", cgGetParameterResourceIndex( param ) );
    _jsCgPrintSpaces( indent + 3 ); printf( "variability = %s\n", cgGetEnumString( cgGetParameterVariability( param ) ) );

    CgRuntimeParameter* paramPtr = _jsCgGLTestParameter( param );
    JS_ASSERT( paramPtr != NULL );
}

#endif

unsigned int _cgHashString( const char *str )
{
    // this is just the elf hashing function
    unsigned int h = 0;
    unsigned int g;
    while ( *str )
    {
        h = ( h << 4 ) + *str++;
        if (( g = h & 0xf0000000 ) )	h ^= g >> 24;
        h &= ~g;
    }
    return h;
}

static int connection_creates_cycle( CgRuntimeParameter* paramFrom, CgRuntimeParameter* paramTo )
{
    // from is parent, to is child
    // start with child, walk up the tree, it must end and not reach where it started
    CgRuntimeParameter* paramHierarchyIter = paramFrom;
    while ( paramHierarchyIter != NULL )
    {
        if ( paramHierarchyIter == paramTo )
        {
            return 1;
        }

        // search the program's connection table to find the connection info for this parameter
        // each parameter only ever has one parent
        STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = paramHierarchyIter->program->connectionTable.begin();
        while ( paramConnectionTableIter != paramHierarchyIter->program->connectionTable.end() )
        {
            if ( paramConnectionTableIter->child == paramHierarchyIter )
            {
                paramHierarchyIter = paramConnectionTableIter->parent;
                break;
            }
            paramConnectionTableIter++;
        }
        // if the parameter is not found in the connection table, we are done walking the hierarchy and there is no cycle
        if ( paramConnectionTableIter == paramHierarchyIter->program->connectionTable.end() )
        {
            paramHierarchyIter = NULL;
        }
    }
    return 0;
}

void _jsCgDestroyContextParam( CgRuntimeParameter* ptr )
{
    // the parameter to be destroyed must not be connected to anything at this point

    // assert it is a runtime created param
    JS_ASSERT( is_created_param( ptr ) );

    // also clean up any semantics that were created for this parameter
    STL_NAMESPACE vector<CgRuntimeSemantic>::iterator semanticIter = ptr->program->parameterSemantics.begin();
    while ( semanticIter != ptr->program->parameterSemantics.end() )
    {
        if ( semanticIter->param == ptr )
        {
            ptr->program->parameterSemantics.erase( semanticIter );
            break;
        }
        semanticIter++;
    }

    // remove this parameter from connections in case it was a child, pulling values from a parent
    STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = ptr->program->connectionTable.begin();
    while ( paramConnectionTableIter != ptr->program->connectionTable.end() )
    {
        if ( paramConnectionTableIter->child == ptr )
        {
            ptr->program->connectionTable.erase( paramConnectionTableIter );
            break;
        }
        paramConnectionTableIter++;
    }

    // this will clean up fx stuff if needed
    if ( _cgParameterDestroyHook ) _cgParameterDestroyHook( ptr );

    // lastly, remove it from the list of runtime created parameters
    STL_NAMESPACE vector<CgRuntimeParameter*>::iterator rtCreatedIter = ptr->program->runtimeCreatedParameters.begin();
    while ( rtCreatedIter != ptr->program->runtimeCreatedParameters.end() )
    {
        if ( *rtCreatedIter == ptr )
        {
            ptr->program->runtimeCreatedParameters.erase( rtCreatedIter );
            break;
        }
        rtCreatedIter++;
    }

    // erase the actual param name id
    _jsEraseName( &_CurrentContext->cgParameterNameSpace, ( jsName )( ptr->id ) );

    // and free the alloced memory
    jsFree( ptr );
}

static void _setSamplerRuntimeParameter( CgRuntimeParameter*ptr, const void*v, int /*index*/ )
{
    JS_ASSERT( ptr );
    JS_ASSERT( is_created_param( ptr ) );
    if ( v )
    {
        *( GLuint* )ptr->pushBufferPointer = *( GLuint* )v;
    }
}

CgRuntimeParameter* _jsCgCreateParameterInternal( _CGprogram *program, const char* name, CGtype type )
{
    CgRuntimeParameter* param = NULL;
    int isSampler = 0;
    switch ( type )
    {
        case CG_SAMPLER2D:
        case CG_SAMPLER3D:
        case CG_SAMPLERCUBE:
            // if creating a sampler, we need to reserve room for the name
            // binding. This is useful for CgFX.
            isSampler = 1;
        case CG_FLOAT: // fall through
        case CG_FLOAT1:
        case CG_FLOAT2:
        case CG_FLOAT3:
        case CG_FLOAT4:
        case CG_FLOAT1x1:
        case CG_FLOAT2x1:
        case CG_FLOAT1x2:
        case CG_FLOAT3x1:
        case CG_FLOAT1x3:
        case CG_FLOAT4x1:
        case CG_FLOAT1x4:
        case CG_FLOAT2x2:
        case CG_FLOAT2x3:
        case CG_FLOAT3x2:
        case CG_FLOAT2x4:
        case CG_FLOAT4x2:
        case CG_FLOAT3x3:
        case CG_FLOAT3x4:
        case CG_FLOAT4x3:
        case CG_FLOAT4x4:
        {
            // allocate everything in one go
            // space for the runtime param and its entry
            // space for the data which will be stored in the push buffer pointer
            // space for the name which will be pointed to by the entry
            unsigned int size = sizeof( CgRuntimeParameter ) + sizeof( CgParameterEntry ) +
                                ( _jsCountFloatsInCgType( type ) * sizeof( float ) ) + ( name ? strlen( name ) + 1 : 1 );

            // lets create a param. for this basic type.
            param = ( CgRuntimeParameter* ) jsMalloc( size );

            if ( !param ) { _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR ); return NULL; }
            memset( param, 0, size );

            CGparameter id = ( CGparameter )_jsCreateName( &_CurrentContext->cgParameterNameSpace, param );
            if ( !id ) { jsFree( param ); return NULL;}

            param->id = id;

            // identify this param as a context or effect param by using the program passed in
            param->program = program;

            // did one large allocation, first part of the block holds the rtparam, second holds the entry, then the values, then the name
            param->parameterEntry = ( CgParameterEntry * )(( char* )param + sizeof( CgRuntimeParameter ) );
            param->pushBufferPointer = ( void * )(( char* )param->parameterEntry + sizeof( CgParameterEntry ) );

            CgParameterEntry* paramEntryInitializer = const_cast<CgParameterEntry*>( param->parameterEntry );

            paramEntryInitializer->nameOffset = ( unsigned int )(( char* )param->pushBufferPointer + _jsCountFloatsInCgType( type ) * sizeof( float ) );

            program->runtimeCreatedParameters.push_back( param );

            // set the name if it was specified, else it is set to null string from memset above
            if ( name )
            {
                strcpy( reinterpret_cast<char*>( paramEntryInitializer->nameOffset ), name );
            }

            paramEntryInitializer->typeIndex = ( unsigned short )type;

            paramEntryInitializer->flags = 0;
            paramEntryInitializer->flags |= CGP_RTCREATED;
            paramEntryInitializer->flags |= CGPD_IN | CGPV_UNIFORM;
            paramEntryInitializer->flags |= CGPF_GLOBAL;

            // i'm not sure i understand the significance the special handling of sampler
            // but there was some skeletal code lying around that seemed relevant so
            // i'm leaving it in case it was important.
            if ( isSampler )
            {
                param->glType = _jsCgGetSamplerGLTypeFromCgType( type );
                paramEntryInitializer->flags |= CGPF_REFERENCED;
            }

            // the setters for runtime created parameters just need to copy the values into the
            // block of memory allocated above pointed to by the pushBufferPointer.
            // they have no true hardware resources and the actual program parameters
            // will pull any values needed due to connections out of the pushBufferPointer
            // memory when their program is bound (or during that obnoxious call to cgGetParameterValues).
            // we are going to reuse the unshared vertex program setters because they do this operation
            // this is not because this is an unshared vertex program parameter.

            param->setterIndex = _cgRaiseInvalidParamIndex;
            param->settercIndex = _cgRaiseNotMatrixParamIndex;
            param->setterrIndex = _cgRaiseNotMatrixParamIndex;
            param->samplerSetter = _cgRaiseInvalidParamIndex;

            switch ( type )
            {
                case CG_FLOAT:
    case CG_FLOAT1: case CG_FLOAT2: case CG_FLOAT3: case CG_FLOAT4:
                {
                    unsigned int floatCount = _jsCountFloatsInCgType( type );
                    param->setterIndex = getVectorTypeIndexSetterFunction( 0, 0, VERTEX_PROFILE_INDEX, floatCount - 1 );
                }
                break;
    case CG_FLOAT1x1: case CG_FLOAT1x2: case CG_FLOAT1x3: case CG_FLOAT1x4:
    case CG_FLOAT2x1: case CG_FLOAT2x2: case CG_FLOAT2x3: case CG_FLOAT2x4:
    case CG_FLOAT3x1: case CG_FLOAT3x2: case CG_FLOAT3x3: case CG_FLOAT3x4:
    case CG_FLOAT4x1: case CG_FLOAT4x2: case CG_FLOAT4x3: case CG_FLOAT4x4:
                    // WE ARE ASSUMING THAT ROW_MAJOR IS AT 0 and COL_MAJOR IS AT 1
                    param->setterrIndex = getMatrixTypeIndexSetterFunction( 0, 0, VERTEX_PROFILE_INDEX, _jsGetTypeRowCount( type ) - 1, _jsGetTypeColCount( type ) - 1, 0 );
                    param->settercIndex = getMatrixTypeIndexSetterFunction( 0, 0, VERTEX_PROFILE_INDEX, _jsGetTypeRowCount( type ) - 1, _jsGetTypeColCount( type ) - 1, 1 );
                    break;
case CG_SAMPLER1D: case CG_SAMPLER2D: case CG_SAMPLER3D: case CG_SAMPLERRECT: case CG_SAMPLERCUBE:
                    param->samplerSetter = _setSamplerRuntimeParameter;
                    break;
                case CGP_SCF_BOOL:
                    break;
                default:
                    JS_ASSERT( 0 );
                    break;
            }

            if ( _cgParameterCreateHook ) _cgParameterCreateHook( param );
            // done creating the param
            break;
        }
        default:
            _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
            break;
    }
    return param;
}

//TODO: share
static int _jsGetSizeofSubArray( const short *dimensions, int count )
{
    int res = 1;
    for ( int i = 0;i < count;i++ )
        res *= ( int )( *( dimensions++ ) );
    return res;
}


// special value 0 for namespace means don't care about namespace. Legacy API
static _CGparameter *_cgGetNamedParameter( _CGprogram* progPtr, const char* name, CGenum name_space, int *arrayIndex, const CgParameterEntry *_startEntry = NULL, int _entryCount = -1 )
{
    if ( name == NULL )
        return NULL;

    /*_CGparameter* param;// = progPtr->paramArray;
    unsigned int hash = _cgHashString(name);

       // slow linear search
       // TODO ************ replace this with a hash lookup.

       for (unsigned int i = 0; i<progPtr->paramCount; ++i)
       {
    param = &progPtr->paramArray[i];
    while (param)
        {
    	// compare against hash value first
    	if (hash == param->hashVal)
    	    {
    		CGenum paramNamespace = (param->ordinal == -1) ? CG_GLOBAL:CG_PROGRAM;
    		// only do a string compare if the hash values match.
    		// check that the namespace matches too.
    		if (0 == strcmp(name, param->name) && (name_space ==0 || (name_space == paramNamespace)))
    		    {
    			return param;
    		    }
    	    }

    	param = param->nextLeaf;
        }
       }*/

    // slow linear search
    // TODO ************ replace this with a hash lookup.
    //TODO: too slow ? would be faster if the structure items would be ordered
    //JS_ASSERT(name_space == 0 && "namespace not implemented yet");
    *arrayIndex = -1;
    int done = 0;
    const char *structureEnd;
    const char *structureStart = name;
    int itemIndex = -1;
    _CGprogram *program = progPtr;

    const CgParameterEntry *currentEntry;
    const CgParameterEntry *lastEntry;

    //containerCount limit the number of item that we look for, once we have started recursing in a structure or in an unrolled array
    int containerCount = -2;
    if ( _startEntry && _entryCount != -1 )
    {
        currentEntry = _startEntry;
        containerCount = _entryCount;
    }
    else
    {
        currentEntry = program->parametersEntries;
    }
    lastEntry = program->parametersEntries + program->rtParametersCount;

    int isInArray = 0;
    bool bWasUnrolled = false;
    const char *prevStructureStart = structureStart; //for unrolled

    //look recursively in the parameter structure of the program
    while (( !done ) && ( *structureStart ) && ( containerCount != -1 ) )
    {
        // process the next structural element
        structureEnd = strpbrk( structureStart, ".[" );
        // test if we are done finding structural information
        if ( structureEnd == NULL )
        {
            //set structureEnd correctly so that the rest of the function performs correctly
            structureEnd = structureStart + strlen( structureStart );
            //set the done flag to exit the loop
            done = 1;
        }

        //extract the current structural element name
        if ( bWasUnrolled )
        {
            bWasUnrolled = false;
            structureStart = prevStructureStart;
        }
        char structName[256];
        int length = ( int )( structureEnd - structureStart );
        strncpy( structName, structureStart, length );
        structName[length] = '\0';
        prevStructureStart = structureStart; //for unrolled
        structureStart = structureEnd + 1;

        //advance the current entry
        bool found = false;
        while ( !found && currentEntry < lastEntry && ( containerCount == -2 || containerCount > 0 ) )
        {
            if ( !strncmp( structName, program->stringTable + currentEntry->nameOffset, length )
                    && ( name_space == 0 || ( name_space == CG_GLOBAL && ( currentEntry->flags & CGPF_GLOBAL ) )
                         || ( name_space == CG_PROGRAM && !( currentEntry->flags & CGPF_GLOBAL ) ) ) )
            {
                if (( int )strlen( program->stringTable + currentEntry->nameOffset ) != length )
                {
                    //special case for array of samplers, which are always unrolled
                    //try full name match up:
                    if ( !strcmp( name, program->stringTable + currentEntry->nameOffset ) )
                    {
                        found = true;
                        done = 1;
                    }

                    // SPECIAL CASE FOR ARRAY_OF_SAMPLERS
                    if ( !strncmp( name, program->stringTable + currentEntry->nameOffset, length ) &&
                            !strcmp( "[0]", program->stringTable + currentEntry->nameOffset + length ) )
                    {
                        found = true;
                        done = 1;
                    }
                    // END SPECIAL CASE FOR ARRAY_OF_SAMPLERS
                }
                else
                    found = true;
            }

            if ( !found )
            {
                //we are skipping entries here, the search stays at the same level
                int skipCount = 1;
                while ( skipCount && currentEntry < lastEntry )
                {
                    //skip as many entry as necessary
                    if ( currentEntry->flags & CGP_STRUCTURE )
                    {
                        const CgParameterStructure *parameterStructure = _jsGetParameterStructure( program, currentEntry );
                        skipCount += parameterStructure->memberCount;
                    }
                    else if ( currentEntry->flags & CGP_ARRAY )
                    {
                        if ( currentEntry->flags & CGP_UNROLLED )
                        {
                            const CgParameterArray *parameterArray =  _jsGetParameterArray( program, currentEntry );
                            skipCount += _jsGetSizeofSubArray(( short* )parameterArray->dimensions, parameterArray->dimensionCount );
                        }
                        else
                            skipCount++; //the following item will be the type ( can't be a structure as they are always enrolled at the moment )
                    }
                    currentEntry++;
                    skipCount--;
                }
            }
            if ( containerCount != -2 ) //denotes that we are in a structure or in an array and that we shouldn't look beyond
                containerCount--;
        }
        //at that point we failed or we succeeded to find the entry at that level.
        //if we have succeed we continue in the lower level if needed. if we have failed, we return not found.

        if ( found )
        {
            switch ( currentEntry->flags & CGP_TYPE_MASK )
            {
                case 0:
                    itemIndex = ( int )( currentEntry - program->parametersEntries );
                    break;
                case CGP_ARRAY:
                {

                    const CgParameterEntry *arrayEntry = currentEntry;
                    const CgParameterArray *parameterArray =  _jsGetParameterArray( program, arrayEntry );

                    if ( *structureEnd == '\0' )  //if we are asked for the array parameter itself and not an array item:
                    {
                        itemIndex = ( int )( currentEntry - program->parametersEntries );
                        break;
                    }

                    //go inside:
                    currentEntry++;
                    JS_ASSERT(( currentEntry->flags & CGP_ARRAY ) == 0 );
                    if ( currentEntry->flags &CGP_STRUCTURE )
                    {
                        JS_ASSERT( arrayEntry->flags & CGP_UNROLLED );
                        //continue to look into the list, but we should stop before the end of the array
                        bWasUnrolled = true;
                        containerCount = _jsGetSizeofSubArray(( short* )parameterArray->dimensions, parameterArray->dimensionCount );
                        break;
                    }
                    else
                    {
                        //the array contains base types
                        isInArray = 1;
                        const char *arrayStart = structureEnd;
                        const char *arrayEnd = structureEnd;

                        int dimensionCount = 0;
                        int arrayCellIndex = 0;
                        while ( *arrayStart == '[' && dimensionCount < parameterArray->dimensionCount )
                        {
                            arrayEnd = strchr( arrayStart + 1, ']' );
                            int length = ( int )( arrayEnd - arrayStart - 1 );
                            char indexString[16];
                            strncpy( indexString, arrayStart + 1, length );
                            indexString[length] = '\0';
                            int index = atoi( indexString );
                            int rowSize = parameterArray->dimensions[dimensionCount];
                            if ( index >= rowSize )
                            {
                                //index out of range: not found
				return NULL;
                            }
                            arrayCellIndex += index * _jsGetSizeofSubArray(( short* )parameterArray->dimensions + dimensionCount, parameterArray->dimensionCount - dimensionCount - 1 );

                            arrayStart = arrayEnd + 1;
                            dimensionCount++;
                        }
                        structureEnd = arrayStart;
                        if ( *structureEnd == '\0' )
                            done = 1;

                        if ( done )
                        {
                            //found the item in the array
                            ( *arrayIndex ) = arrayCellIndex;
                            itemIndex = ( int )( currentEntry - program->parametersEntries );
                        }
                        //else... don't know what to do, it's a array of structure not unrolled
                    }
                }
                break;
                case CGP_STRUCTURE:
                    if ( done )
                    {
                        //the user is getting back a strut... not sure that all the checks are in place for such a return value
                        itemIndex = ( int )( currentEntry - program->parametersEntries );
                    }
                    else
                    {
                        const CgParameterStructure *parameterStructure = _jsGetParameterStructure( program, currentEntry );
                        containerCount = parameterStructure->memberCount;
                    }
                    break;
                default:
                    JS_ASSERT( 0 );
                    break;
            }
        }
        if ( found )
        {
            if ( !bWasUnrolled )
                currentEntry++;
        }
        else
            break;//not found
    }

    //have we found it ?
    if ( itemIndex != -1 )
        return ( _CGparameter* )( program->runtimeParameters + itemIndex );
    else
        return NULL;
}
// API functions ----------------------------------------

// not attempting to implement parameter creation at this time. Not until
// we get more information on the "private interface" to the CGC compiler.

/** @addtogroup Cg
 *@{
 */

/**
@short Creates a parameter

cgCreateParameter() creates context level shared parameters.  These
parameters are primarily used by connecting them to one or more program
parameters with cgConnectParameter().

cgCreateParameter() can create the following types of parameters:

@code
CG_FLOAT
CG_FLOAT1
CG_FLOAT2
CG_FLOAT3
CG_FLOAT4
CG_FLOAT1x1
CG_FLOAT2x1
CG_FLOAT1x2
CG_FLOAT3x1
CG_FLOAT1x3
CG_FLOAT4x1
CG_FLOAT1x4
CG_FLOAT2x2
CG_FLOAT2x3
CG_FLOAT3x2
CG_FLOAT2x4
CG_FLOAT4x2
CG_FLOAT3x3
CG_FLOAT3x4
CG_FLOAT4x3
CG_FLOAT4x4
CG_SAMPLER2D
CG_SAMPLER3D
CG_SAMPLERCUBE
@endcode

@param ctx     Specifies the context that the new parameter will be added to.
@param type    The type of the new parameter.

@return Returns the handle to the new parameter.

@par Examples
@code
CGcontext Context = cgCreateContext();
CGparameter MySharedFloatParam = cgCreateParameter(Context, CG_FLOAT);
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameterArray, cgCreateParameterMultiDimArray, cgDestroyParameter
 */
CG_API CGparameter cgCreateParameter( CGcontext ctx, CGtype type )
{
    JS_APIENTRY_TIMER( cgCreateParameter );
    // This routine is currently (release 0.2.1) a nop.  When it is changed to generate new parameters this will impact the
    // debugger runtime because all cg routines that take parameters as inputs must be matched to their production.
    // See the discussion of producer-consumer deficiencies in the design document for the debugger runtime (JSPerfTool).
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgCreateParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check context handle
    if ( !CG_IS_CONTEXT( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }
    _CGcontext * ctxPtr = _cgGetContextPtr( ctx );
    _CGprogram * program = &ctxPtr->defaultProgram;
    CgRuntimeParameter *param = _jsCgCreateParameterInternal( program, NULL, type );
    return param ? param->id : NULL;
}

/**
@short Creates a parameter array

cgCreateParameterArray() is currently not implemented.

cgCreateParameterArray() creates context level shared parameter arrays.
These parameters are primarily used by connecting them to one or more
program parameter arrays with cgConnectParameter().

cgCreateParameterArray() works similarly to cgCreateParameter().  Instead
of creating a single parameter, it creates an array of them.

@param ctx     Specifies the context that the new parameter will be added to.
@param type    The type of the new parameter.
@param length  The length of the array being created.

@return       Returns the handle to the new parameter array.

@par Examples
@code
CGcontext Context = cgCreateContext();
CGparameter MyFloatArray = cgCreateParameterArray(Context, CG_FLOAT, 5);
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameter, cgCreateParameterMultiDimArray, cgDestroyParameter
 */
CG_API CGparameter cgCreateParameterArray( CGcontext ctx, CGtype type, int length )
{
    JS_APIENTRY_TIMER( cgCreateParameterArray );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgCreateParameterArray, JSDPM_CALL_TYPE_STATE_CG, NULL, 3,
                             ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             length, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check context handle
    if ( !CG_IS_CONTEXT( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }
    // check type
    // _jsCgRaiseError(CG_INVALID_VALUE_TYPE_ERROR);

    // TODO **********
    return ( CGparameter )NULL;
}

/**
@short Creates a multi-dimensional parameter array

cgCreateParameterMultiDimArray() is currently not implemented.

cgCreateParameterMultiDimArray() creates context level shared multi-dimensional
parameter arrays.  These parameters are primarily used by connecting
them to one or more program parameter arrays with cgConnectParameter().

cgCreateParameterMultiDimArray() works similarly to cgCreateParameterArray().
Instead of taking a single length parameter it takes an
array of lengths, one per dimension.  The dimension of the array is
defined by the <I><c>dim</c></I> parameter.

@param       ctx     Specifies the context that the new parameter will be added to.
@param       type    The type of the new parameter.
@param       dim     The dimension of the multi-dimensional array.
@param       lengths An array of length values, one length per dimension.

@return Returns the handle to the new parameter array.

@par Examples
@code
// Creates a three dimensional float array similar to the C declaration:
// float MyFloatArray[5][3][4];
int Lengths[] = { 5, 3, 4 };
CGcontext Context = cgCreateContext();
CGparameter MyFloatArray = cgCreateParameterArray(Context, CG_FLOAT, 3, Lengths);
@endcode
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameter, cgCreateParameterArray, cgDestroyParameter
 */
CG_API CGparameter cgCreateParameterMultiDimArray( CGcontext ctx, CGtype type, int dim, const int* lengths )
{
    JS_APIENTRY_TIMER( cgCreateParameterMultiDimArray );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgCreateParameterMultiDimArray, JSDPM_CALL_TYPE_STATE_CG, NULL, 4,
                             ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             dim, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             lengths, dim*sizeof( int ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif // JS_DPM

    // check context handle
    if ( !CG_IS_CONTEXT( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    // check type
    // _jsCgRaiseError(CG_INVALID_VALUE_TYPE_ERROR);

    // TODO **********
    return ( CGparameter )NULL;
}


/**
@short Destroys a parameter

cgDestroyParameter() destroys parameters created with cgCreateParameter(),
cgCreateParameterArray(), or cgCreateParameterMultiDimArray().

Upon destruction, the <c>CGparameter</c> will become invalid.  Any connections
(see cgConnectParameter()) in which <I><c>param</c></I> is the destination parameter
will be disconnected.  An error will be thrown if <I><c>param</c></I> is a source
parameter in any connections.

The parameter being destroyed may not be one of the children parameters
of a struct or array parameter.  In other words it must be a <c>CGparameter</c>
returned by one of the cgCreateParameter() family of entry points.

@param       param   The parameter to destroy.

@par Examples
@code
CGcontext Context = cgCreateContext();
CGparameter MyFloatParam = cgCreateParameter(Context, CG_FLOAT);
CGparameter MyFloatParamArray = cgCreateParameterArray(Context, CG_FLOAT, 5);
// ...
cgDestroyParameter(MyFloatParam);
cgDestroyParameter(MyFloatParamArray);
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_PARAMETER_IS_NOT_SHARED_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a parameter created by one of the cgCreateParameter() family of entry points.</TD>
</TR>
<TR>
<TD><CODE>CG_CANNOT_DESTROY_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is a source parameter in a connection made by cgConnectParameter().  cgDisconnectParameter() should be used before calling cgDestroyParameter() in such a case. </TD>
</TR>
</TABLE>

@sa      cgCreateParameter, cgCreateParameterMultiDimArray, cgCreateParameterArray
*/
CG_API void cgDestroyParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgDestroyParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgDestroyParameter, JSDPM_CALL_TYPE_STATE_CG, , 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    CgRuntimeParameter *rtParam = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    // check that the param was created via cgCreateParameter
    if ( !is_created_param( rtParam ) )
    {
        _jsCgRaiseError( CG_PARAMETER_IS_NOT_SHARED_ERROR );
        return;
    }

    // check to see if parameter is not a source before deletion
    bool isConnectionParent = false;
    struct _CGprogram* programIter = rtParam->program->parentContext->programList;
    while ( programIter != NULL )
    {
        // search the program's connection table to find if this is a parent param to anybody
        STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = programIter->connectionTable.begin();
        while ( paramConnectionTableIter != programIter->connectionTable.end() )
        {
            if ( paramConnectionTableIter->parent == rtParam )
            {
                isConnectionParent = true;
                break;
            }
            paramConnectionTableIter++;
        }
        programIter = programIter->next;
    }
    if ( isConnectionParent )
    {
        _jsCgRaiseError( CG_CANNOT_DESTROY_PARAMETER_ERROR );
        return;
    }

    // this call will call a hook function to clean up the fx if needed
    _jsCgDestroyContextParam( rtParam );

}

/**
@short Connect two parameters

cgConnectParameter() connects a source (<I><c>from</c></I>) parameter to a destination
(<I><c>to</c></I>) parameter.  The resulting connection forces the value and variability
of the destination parameter to be identical to the source
parameter.  A source parameter may be connected to multiple destination
parameters but there may only be one source parameter per destination
parameter.

cgConnectParameter() may be used to create an arbitrarily deep tree.  A
runtime error will be thrown if a cycle is inadvertently  created.  For
example, the following code snippet would generate a <CODE>CG_BIND_CREATES_CYCLE_ERROR</CODE>:

@code
CGcontext Context = cgCreateContext();
CGparameter Param1 = cgCreateParameter(Context, CG_FLOAT);
CGparameter Param2 = cgCreateParameter(Context, CG_FLOAT);
CGparameter Param3 = cgCreateParameter(Context, CG_FLOAT);
cgConnectParameter(Param1, Param2);
cgConnectParameter(Param2, Param3);
cgConnectParameter(Param3, Param1); // This will throw the error
@endcode

If the source type is a complex type (e.g., struct, or array) the
topology and member types of both parameters must be identical.  Each
correlating member parameter will be connected.

Both parameters must be of the same type unless the source parameter is
a struct type, the destination parameter is an interface type, and the
struct type implements the interface type.   In such a case, a copy of
the parameter tree under the source parameter will be duplicated,
linked to the original tree, and placed under the destination parameter.

If an array parameter is connected to a resizable array parameter, the
destination parameter array will automatically be resized to match the
source array.

The source parameter may not be a program parameter.  Also the variability
of the parameters may not be <CODE>CG_VARYING</CODE>.

@param       from    The source parameter.
@param       to      The destination parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>Either the <I><c>from</c></I> or <I><c>to</c></I> parameters are invalid handles.</TD>
</TR>
<TR>
<TD><CODE>CG_PARAMETER_IS_NOT_SHARED</CODE></TD>
<TD>The source parameter is a program parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_BIND_CREATES_CYCLE_ERROR</CODE></TD>
<TD>The connection will result in a cycle.</TD>
</TR>
<TR>
<TD><CODE>CG_PARAMETERS_DO_NOT_MATCH_ERROR</CODE></TD>
<TD>The parameters do not have the same type or the topologies do not match.</TD>
</TR>
@if future
<TR>
<TD><CODE>CG_ARRAY_TYPES_DO_NOT_MATCH_ERROR</CODE></TD>
<TD>The type of two arrays being connected do not match.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR</CODE></TD>
<TD>The dimensions of two arrays being connected do not match.</TD>
</TR>
@endif
</TABLE>

@sa cgGetConnectedParameter, cgGetConnectedToParameter, cgDisconnectParameter
*/
CG_API void cgConnectParameter( CGparameter from, CGparameter to )
{
    JS_APIENTRY_TIMER( cgConnectParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgConnectParameter, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             from, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             to, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check parameter handles
    if ( !CG_IS_PARAMETER( from ) || !CG_IS_PARAMETER( to ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // check and get parameter handles
    CgRuntimeParameter *rtParamFrom = ( CgRuntimeParameter* )_jsCgGLTestParameter( from );
    CgRuntimeParameter *rtParamTo = ( CgRuntimeParameter* )_jsCgGLTestParameter( to );

    // check that the param was created via cgCreateParameter
    if ( !is_created_param( rtParamFrom ) )
    {
        _jsCgRaiseError( CG_PARAMETER_IS_NOT_SHARED_ERROR );
        return;
    }

    if ( connection_creates_cycle( rtParamFrom, rtParamTo ) )
    {
        _jsCgRaiseError( CG_BIND_CREATES_CYCLE_ERROR );
        return;
    }

    JS_ASSERT( rtParamFrom->parameterEntry != NULL );
    JS_ASSERT( rtParamTo->parameterEntry != NULL );

    // type index is actually the offset into the block of memory following the entries
    CGtype typeFrom = _jsGetParameterCGtype( rtParamFrom->program, rtParamFrom->parameterEntry );
    CGtype typeTo = _jsGetParameterCGtype( rtParamTo->program, rtParamTo->parameterEntry );
    if ( typeFrom != typeTo )
    {
        // simple for now.
        _jsCgRaiseError( CG_PARAMETERS_DO_NOT_MATCH_ERROR );
        return;
    }

    // (for array types) check that types match
    // _jsCgRaiseError(CG_ARRAY_TYPES_DO_NOT_MATCH_ERROR);

    // (for array types) check that all dimensions match
    // _jsCgRaiseError(CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR);

    // we will need the top param to all for quick pull from arbitrarily deep hierarchies
    // it gets updated if top gets disconnected from parent
    // all the checks will have already happened when the initial connection was made
    CgRuntimeParameter *rtParamTop = rtParamFrom;
    // search parent connections in parent program and walk up to top
    STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = rtParamTop->program->connectionTable.begin();
    while ( paramConnectionTableIter != rtParamTop->program->connectionTable.end() )
    {
        // if this parameter is the child in a table, return its parent
        if ( paramConnectionTableIter->child == rtParamTop )
        {
            rtParamTop = paramConnectionTableIter->parent;
            paramConnectionTableIter = rtParamTop->program->connectionTable.begin();
        }
        else
        {
            paramConnectionTableIter++;
        }
    }

    // add new entry to the destination param's program's connection table
    CgParameterConnection newConnection;
    newConnection.child = rtParamTo;
    newConnection.parent = rtParamFrom;
    newConnection.top = rtParamTop;
    newConnection.childOnBindSetter = _cgRaiseInvalidParamIndex;
    switch ( typeTo )
    {
        case CG_FLOAT:
case CG_FLOAT1: case CG_FLOAT2: case CG_FLOAT3: case CG_FLOAT4:
            newConnection.childOnBindSetter = rtParamTo->setterIndex;
            break;
case CG_FLOAT1x1: case CG_FLOAT1x2: case CG_FLOAT1x3: case CG_FLOAT1x4:
case CG_FLOAT2x1: case CG_FLOAT2x2: case CG_FLOAT2x3: case CG_FLOAT2x4:
case CG_FLOAT3x1: case CG_FLOAT3x2: case CG_FLOAT3x3: case CG_FLOAT3x4:
case CG_FLOAT4x1: case CG_FLOAT4x2: case CG_FLOAT4x3: case CG_FLOAT4x4:
            newConnection.childOnBindSetter = rtParamTo->setterrIndex;
            break;
case CG_SAMPLER1D: case CG_SAMPLER2D: case CG_SAMPLER3D: case CG_SAMPLERRECT: case CG_SAMPLERCUBE:
            newConnection.childOnBindSetter = rtParamTo->samplerSetter;
            break;
        case CGP_SCF_BOOL:
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    JS_ASSERT( rtParamTo->program != NULL );
    rtParamTo->program->connectionTable.push_back( newConnection );
}

/**
@short Disconnects two parameters

cgDisconnectParameter() disconnects an existing connection made with
cgConnectParameter() between two parameters.  Since a given parameter can
only be connected to one source parameter, only the destination parameter
is required as an argument to cgDisconnectParameter().

If the type of <I><c>param</c></I> is an interface and the struct connected to it is
a struct that implements it, any sub-parameters created by the connection will also be destroyed.

@param       param   The destination parameter in the connection that will be disconnected.

@par Examples
@code
CGparameter TimeParam1 = cgGetNamedParameter(program1, "time");
CGparameter SharedTime = cgCreateParameter(Context, cgGetParameterType(TimeParam1));
cgConnectParameter(SharedTime, TimeParam1);
// ...
cgDisconnectParameter(TimeParam1);
@endcode

@par Errors
`Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetConnectedParameter, cgGetConnectedToParameter, cgConnectParameter
*/
CG_API void cgDisconnectParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgDisconnectParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgDisconnectParameter, JSDPM_CALL_TYPE_STATE_CG, , 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // check and get parameter handles
    CgRuntimeParameter *rtParamTo = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    // check parameter handles
    if ( !CG_IS_PARAMETER( rtParamTo ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // search the program's connection table to find the connection info for this parameter
    // each parameter only ever has one parent
    STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = rtParamTo->program->connectionTable.begin();
    while ( paramConnectionTableIter != rtParamTo->program->connectionTable.end() )
    {
        if ( paramConnectionTableIter->child == rtParamTo )
        {
            break;
        }
        paramConnectionTableIter++;
    }

    // if the parameter is not found in the connection table, it was never connected in the first place
    if ( paramConnectionTableIter == rtParamTo->program->connectionTable.end() )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // parameter was found to be connected, delete its entry from the table to disconnect
    rtParamTo->program->connectionTable.erase( paramConnectionTableIter );

    // check to see if parameter is a parent to for anyone in an arbitrarily deep hierarchy
    // if it is, we must update all the children's top pointers to point to this param
    struct _CGprogram* programIter = rtParamTo->program->parentContext->programList;
    while ( programIter != NULL )
    {
        // search the connection table of all the programs in this context to find if this is a parent param to anybody
        paramConnectionTableIter = programIter->connectionTable.begin();
        while ( paramConnectionTableIter != programIter->connectionTable.end() )
        {
            if ( paramConnectionTableIter->parent == rtParamTo )
            {
                // this is the case where rt was the parent, and now must be the top because
                // it was just disconnected from its own parent
                paramConnectionTableIter->top = rtParamTo;
            }
            paramConnectionTableIter++;
        }
        programIter = programIter->next;
    }
}

/**
@short Gets the connected source parameter

Returns the source parameter of a connection to <I><c>param</c></I>.

@param       param   Specifies the destination parameter.

@return Returns the connected source parameter if <I><c>param</c></I> is connected to one.
Otherwise <I>(CGparameter)0</I> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa       cgConnectParameter, cgGetConnectedToParameter
*/
CG_API CGparameter cgGetConnectedParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetConnectedParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetConnectedParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    CgRuntimeParameter* ptr = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = ptr->program->connectionTable.begin();
    while ( paramConnectionTableIter != ptr->program->connectionTable.end() )
    {
        // if this parameter is the child in a table, return its parent
        if ( paramConnectionTableIter->child == ptr )
        {
            return paramConnectionTableIter->parent->id;
        }
        paramConnectionTableIter++;
    }

    // not found in connection table, has no parent
    return NULL;
}

/**
@short Gets the number of connected destination parameters

cgGetNumConnectedToParameters() returns the number of destination parameters
connected to the source parameter <I><c>param</c></I>.  It&rsquo;s primarily used with
cgGetConnectedToParameter().

@param       param   Specifies the source parameter.

@return Returns the number of connected destination parameters to <I><c>param</c></I>.
0 is returned if an error is thrown.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>


@sa        cgConnectParameter, cgGetConnectedParameter
*/
CG_API int cgGetNumConnectedToParameters( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetNumConnectedToParameters );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNumConnectedToParameters, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return 0;
    }

    int ret = 0;
    CgRuntimeParameter *ptr = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    _CGprogram* prog = ptr->program;
    _CGcontext* context = prog->parentContext;

    // must iterate over all the programs in this context to see which of them
    // have parameters which are connected to this parent
    _CGprogram* programIter = context->programList;
    while ( programIter != NULL )
    {
        STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = programIter->connectionTable.begin();
        while ( paramConnectionTableIter != programIter->connectionTable.end() )
        {
            // if this connection uses the param ptr as the parent, increment the count
            if ( paramConnectionTableIter->parent == ptr )
            {
                ret++;
            }
            paramConnectionTableIter++;
        }

        programIter = programIter->next;
    }

    return ret;
}

/**
@short Gets a connected destination parameter

Returns one of the destination parameters connected to <I><c>param</c></I>.
cgGetNumConnectedToParameters() should be used to determine the number of
destination parameters connected to <I><c>param</c></I>.

@param       param   Specifies the source parameter.

@param       index   Since there may be multiple destination (to) parameters
connected to <I><c>param</c></I>, <I><c>index</c></I> is need to specify which one is
returned.  <I>index</I> must be within the range of 0 to N - 1 where N
is the number of connected destination parameters.

@return Returns one of the connected destination parameters to <I><c>param</c></I>.
<I>(CGparameter)0</I> is returned if an error is thrown.

@par Examples
@code
int i;
int NParams = cgGetNumConnectedToParameters(SourceParam);
for (int i=0; i < NParams; ++i)
   {
      CGparameter ToParam = cgGetConnectedToParameter(SourceParam, i);
      // ... Do stuff to ToParam ...
   }
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not greater than equal to 0 or less than what cgGetNumConnectedToParameters() returns.</TD>
</TR>
</TABLE>

@sa       cgConnectParameter, cgGetNumConnectedToParameters
*/
CG_API CGparameter cgGetConnectedToParameter( CGparameter param, int index )
{
    JS_APIENTRY_TIMER( cgGetConnectedToParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetConnectedToParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             index, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    if ( index < 0 )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return ( CGparameter )NULL;
    }


    int ret = 0;
    CgRuntimeParameter *ptr = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    _CGprogram* prog = ptr->program;
    _CGcontext* context = prog->parentContext;

    // must iterate over all the programs in this context to see which of them
    // have parameters which are connected to this parent
    _CGprogram* programIter = context->programList;
    while ( programIter != NULL )
    {
        STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = programIter->connectionTable.begin();
        while ( paramConnectionTableIter != programIter->connectionTable.end() )
        {
            // if this is the n'th child, where n == index, return its id
            if ( paramConnectionTableIter->parent == ptr )
            {
                if ( ret == index )
                {
                    return paramConnectionTableIter->child->id;
                }
                ret++;
            }
            paramConnectionTableIter++;
        }

        programIter = programIter->next;
    }

    // else the index is greater than the number of children of this parameter
    _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );

    return ( CGparameter )NULL;
}

/**
@short Get a program parameter by name

The parameters of a program can be retrieved directly by name using the
cgGetNamedParameter() function.  The names of the parameters in a program
can be discovered by iterating through the program&rsquo;s parameters (see
cgGetNextParameter()), calling cgGetParameterName() for each one in turn.

The parameter name does not have to be a complete name for a leaf node
parameter.  For example, if you have a Cg program with the following
parameters:
@code
struct FooStruct
   {
      float4 A;
      float4 B;
   };
struct BarStruct
   {
      FooStruct Foo[2];
   };
void main(BarStruct Bar[3])
   {
      // ...
   }
@endcode
The following leaf-node parameters will be generated:
@code
Bar[0].Foo[0].A
Bar[0].Foo[0].B
Bar[0].Foo[1].A
Bar[0].Foo[1].B
Bar[1].Foo[0].A
Bar[1].Foo[0].B
Bar[1].Foo[1].A
Bar[1].Foo[1].B
Bar[2].Foo[0].A
Bar[2].Foo[0].B
Bar[2].Foo[1].A
Bar[2].Foo[1].B
@endcode
A handle to any of the non-leaf arrays or structs can be directly
obtained by using the appropriate name.  The following are a few examples
of valid names that may be used with cgGetNamedParameter()
given the above example Cg program:
@code
"Bar"
"Bar[1]"
"Bar[1].Foo"
"Bar[1].Foo[0]"
"Bar[1].Foo[0].B"
     ...
@endcode

@param       prog    Specifies the program to retrieve the parameter from.
@param       name    Specifies the name of the parameter to retrieve.

@return Returns the named parameter from the program.  If the program has no
parameter corresponding to name, a <CODE>NULL</CODE> is returned (cgIsParameter()
returns <CODE>CG_FALSE</CODE> for invalid parameters).

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgIsParameter, cgGetFirstParameter, cgGetNextParameter, cgGetFirstStructParameter, cgGetArrayParameter, cgGetParameterName
*/

CG_API CGparameter cgGetNamedParameter( CGprogram prog, const char* name )
{
    JS_APIENTRY_TIMER( cgGetNamedParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetNamedParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             name, strlen( name ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif // JS_DPM
    // check program handle
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    _CGprogram* progPtr = _cgGetProgPtr( prog );
    // 0 means don't care
    int arrayIndex = -1;
    CgRuntimeParameter *param = ( CgRuntimeParameter * )_cgGetNamedParameter( progPtr, name, ( CGenum )0, &arrayIndex );
    if ( param )
    {
        JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
        int ret = ( int )param->id;
        if ( arrayIndex != -1 )
            ret |= ( arrayIndex << CG_PARAMETERSIZE );
        return ( CGparameter )ret;
    }
    else
        return ( CGparameter )NULL;
}

/**
@short Get a program parameter by name

cgGetNamedProgramParameter() is essentially identical to cgGetNamedParameter()
except it limits the search of the parameter to the name space
specified by <I><c>name_space</c></I>.

@param       prog    Specifies the program to retrieve the parameter from.
@param name_space    Specifies the namespace of the parameter to iterate through.
Currently <CODE>CG_PROGRAM</CODE> and <CODE>CG_GLOBAL</CODE> are supported.
@param       name    Specifies the name of the parameter to retrieve.

@return Returns the named parameter from the program.  If the program has no
parameter corresponding to <I><c>name</c></I>, a <CODE>NULL</CODE> is returned (cgIsParameter()
returns <CODE>CG_FALSE</CODE> for invalid parameters).

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa       cgGetNamedParameter
*/
CG_API CGparameter cgGetNamedProgramParameter( CGprogram prog, CGenum name_space, const char* name )
{
    JS_APIENTRY_TIMER( cgGetNamedProgramParameter );
    // Same as cgGetNamedParameter, but restricts search to a specific namespace
    // i.e. only return parameters of this CGprogram that are global.
    //
    // Valid namespaces are CG_GLOBAL and CG_PROGRAM,

    // check program handle
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    // check namespace
    switch ( name_space )
    {
        case CG_GLOBAL :
            break;
        case CG_PROGRAM :
            break;
        default :
            // ********* NOTE: docs say nothing about this check.
            return NULL;
            break;
    }
    _CGprogram* progPtr = _cgGetProgPtr( prog );
    int arrayIndex = -1;
    CgRuntimeParameter *param = ( CgRuntimeParameter * )_cgGetNamedParameter( progPtr, name, name_space, &arrayIndex );
    if ( param )
    {
        JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
        int ret = ( int )param->id;
        if ( arrayIndex != -1 )
            ret |= ( arrayIndex << CG_PARAMETERSIZE );
        return ( CGparameter )ret;
    }
    else
        return ( CGparameter )NULL;
}

/**
@short Get the first parameter in a program

cgGetFirstParameter() returns the first top-level parameter in a program.
This function is used for recursing through all parameters in a program.
See the cgGetNextParameter() manpage for more information on parameter traversal.

@param       prog    Specifies the program to retrieve the first parameter from.

@param       name_space  Specifies the namespace of the parameter to iterate through.
Currently only <CODE>CG_PROGRAM</CODE> is supported.

@return Returns the first <c>CGparameter</c> object in <I><c>prog</c></I>.
<CODE>NULL</CODE> is returned if <I>prog</I> is invalid or if <I><c>prog</c></I> does not have any parameters.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa       cgGetNextParameter
*/
CG_API CGparameter cgGetFirstParameter( CGprogram prog, CGenum name_space )
{
    JS_APIENTRY_TIMER( cgGetFirstParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetFirstParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             name_space, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif // JS_DPM

    // check program handle
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    _CGprogram* progPtr = _cgGetProgPtr( prog );

    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )progPtr->runtimeParameters;

    // find the first param of the requested namespace
    for ( int i = 0; i < progPtr->rtParametersCount; ++i )
    {
        // check parameter handle
        bool isGlobal = ( rtParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;
        if ( isGlobal && name_space == CG_GLOBAL )
        {
            return ( CGparameter )rtParameter->id;
        }
        if ( !isGlobal && name_space == CG_PROGRAM )
        {
            return ( CGparameter )rtParameter->id;
        }
        rtParameter++;
    }
    return ( CGparameter )NULL;
}

/**
 * @}
 */


//this function returns the parent array if the parameter passed is member of an array as well as the position in the array
//returns null other wise
static CgRuntimeParameter *_jsGetArrayItemParent( CGparameter param, int *arrayIndex )
{
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return NULL;

    // the case of the array element is easy to solve:
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if ( previousParameter->parameterEntry->flags & CGP_ARRAY )
        {
            if ( previousParameter->parameterEntry->flags & CGP_UNROLLED )
            {
                //that's easy, we are on the first item of the array
                //the other positions will be discovered by the main loop later
                if ( arrayIndex )
                    *arrayIndex = 0;
                return previousParameter;
            }
            else
            {
                //we have an array item
                if ( arrayIndex )
                    *arrayIndex = CG_GETINDEX( param );
                return previousParameter;
            }
        }
    }

    //we are in the case where we don't have the first item of an unrolled array and where we don't have an item of a compact array
    //the algorithm it to walk back the parameter list until we found the parent.
    //if the parent is a structure or if there is no parent, returns NULL, else return the parent and the index,

    //get the parent of the current as well as its location
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry;
    const CgParameterEntry *firstEntry = rtParameter->program->parametersEntries;

    //we have already treated the current entry at that point, the loop starts on the previous one, the distance is 1
    int distance = 1;
    paramEntry--;

    *arrayIndex = 0;
    bool haveResult = false;
    while ( paramEntry >= firstEntry && !haveResult )
    {
        switch ( paramEntry->flags & CGP_TYPE_MASK )
        {
            case CGP_ARRAY:
                //we found it
                haveResult = true;
                break;
            case CGP_STRUCTURE:
            {
                const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, paramEntry );
                if ( distance >= parameterStructure->memberCount )
                {
                    //the parameter is not in this structure, so I need to remove from the distance all the structure item
                    distance -= parameterStructure->memberCount;
                    //I need to patch the array Index, we have counted some items which were not in the array
                    //this struct should count just for one
                    //so basically it means we are in an array of struct if this struct is one item.
                    //if we only have homogeneous array, we can speed up that function, I don't do it,
                    //in case we extend the behaviour in the future
                    ( *arrayIndex ) -= parameterStructure->memberCount;
                    //increment the arrayIndex
                    ( *arrayIndex )++;
                }
                else
                {
                    //so we are in the structure, not in an array
                    ( *arrayIndex ) = -1;
                    return NULL;
                }
                break;
            }
            case CGP_INTRINSIC:
                //increment the arrayIndex
                ( *arrayIndex )++;
                break;
        }
        distance++;
        paramEntry--;
    }

    if ( haveResult )
    {
        size_t offset = ( rtParameter->parameterEntry - paramEntry );
        CgRuntimeParameter *parentParameter = rtParameter - offset;
        return parentParameter;
    }
    else
    {
        ( *arrayIndex ) = -1;
        return NULL;
    }
}

/** @addtogroup Cg
 *@{
 */

/**
@short Iterate through program&rsquo;s parameters

The parameters of a program can be iterated over recursively by using
the cgGetNextParameter(), cgGetFirstParameter(), cgGetFirstStructParameter(),
and cgGetArrayParameter() functions.

The following example code illustrates one way to do this.

@code
void RecurseParams( CGparameter param ) {
   if(!param)
      return;
   do {
      switch(cgGetParameterType(param))
         {
            case CG_STRUCT :
               RecurseParams(cgGetFirstStructParameter(param));
               break;
            case CG_ARRAY :
               {
                 int ArraySize = cgGetArraySize(param, 0);
                 int i;
                 for (i=0; i < ArraySize; ++i)
                    RecurseParams(cgGetArrayParameter(param, i));
               }
               break;
            default :
                 // Do stuff to param
         }
   } while((param = cgGetNextParameter(param)) != 0);
}

void RecurseParamsInProgram( CGprogram prog ) {
   RecurseParams( cgGetFirstParameter( prog, CG_PROGRAM ) );
}
@endcode

Note that no specific order of traversal is defined by this mechanism.
The only guarantee is that each parameter will be visited exactly once.

@param       param Specifies the "current" parameter.

@return Returns the next parameter in the program&rsquo;s internal sequence of parameters.
Returns <CODE>NULL</CODE> when current is the last parameter in the program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetFirstParameter, cgGetFirstStructParameter, cgGetArrayParameter, cgGetParameterType
*/
CG_API CGparameter cgGetNextParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetNextParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNextParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return ( CGparameter )NULL;

    // runtime created parameters are treated separately because they are in a different namespace
    // as such, you never need to traverse from a program param to a runtime param, or visa-versa
    if ( rtParameter->parameterEntry->flags & CGP_RTCREATED )
    {
        // we have a runtime created parameter
        STL_NAMESPACE vector<CgRuntimeParameter*>::iterator rtCreatedIter = rtParameter->program->runtimeCreatedParameters.begin();
        while ( rtCreatedIter != rtParameter->program->runtimeCreatedParameters.end() )
        {
            if ( *rtCreatedIter == rtParameter )
            {
                rtCreatedIter++;
                if ( rtCreatedIter == rtParameter->program->runtimeCreatedParameters.end() )
                {
                    break;
                }
                return ( *rtCreatedIter )->id;
            }
            rtCreatedIter++;
        }
        // no next parameter for this one
        return ( CGparameter )NULL;
    }

    // the case of the array element of a compact array is easy to solve
    int arrayIndex = -1;
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if (( previousParameter->parameterEntry->flags & CGP_ARRAY ) &&
                !( previousParameter->parameterEntry->flags & CGP_UNROLLED ) )
        {
            //we have an array item
            arrayIndex = CG_GETINDEX( param );
            const CgParameterArray *parameterArray =  _jsGetParameterArray( previousParameter->program, previousParameter->parameterEntry );
            int arraySize = _jsGetSizeofSubArray(( short* )parameterArray->dimensions, parameterArray->dimensionCount );
            arrayIndex++;
            if ( arrayIndex < arraySize )
            {
                JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
                int ret = ( int )rtParameter->id;
                ret |= ( arrayIndex << CG_PARAMETERSIZE );
                return ( CGparameter )ret;
            }
            else
                return ( CGparameter )NULL;
        }
    }

    //get the parent of the current as well as its location
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry;
    const CgParameterEntry *firstEntry = rtParameter->program->parametersEntries;

    int distance = 1;
    paramEntry--;

    bool bNextExists = false;
    while ( paramEntry >= firstEntry && !bNextExists )
    {
        switch ( paramEntry->flags & CGP_TYPE_MASK )
        {
            case CGP_ARRAY:
                distance--; // the array has one extra item, whether it's a structure or if it's an intrinsic type
                //Ced: not it's not true, if it's unrolled there is no extra item
                break;
            case CGP_STRUCTURE:
            {
                const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, paramEntry );
                //+1  because of the structure element, there's structure
                if ( distance >= ( parameterStructure->memberCount + 1 ) )
                {
                    //the parameter is not in this structure, so I need to remove from the distance all the structure item
                    //so this structure will count for a distance of just one ( the distance is the level distance )
                    distance -= parameterStructure->memberCount;
                }
                else
                {
                    //We are going to exit here
                    //ok so we were in this structure, so check if we have more item in the structure after the current one
                    if ( distance < parameterStructure->memberCount )
                    {
                        //we still have some items in the structure, take the next item
                        bNextExists = true;
                        break;
                    }
                    else
                    {
                        //no more items at this level return null
                        return ( CGparameter )NULL;
                    }
                }
                break;
            }
            case CGP_INTRINSIC:
                break;
        }
        distance++;
        paramEntry--;
    }
    //we have now elimated the case where we were in a struct and there were no more items, now if the item has a successor,
    //this is what we are looking for

    //we have already treated the current entry at that point, the loop starts on the previous one, the distance is 1
    CGparameter nextParam = _jsAdvanceParameter( rtParameter->id, 1 );

    if ( nextParam )
    {
        CgRuntimeParameter *nextParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( nextParam );
        JS_ASSERT( nextParameter );

        //check that next param is still in the same namespace as param
        bool isCurrentGlobal = ( rtParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;
        bool isNextGlobal = ( nextParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;
        if ( isNextGlobal != isCurrentGlobal )
        {
            //the next item doesn't have the same global flag... since they are grouped, it means we have quitted the current struct
            return ( CGparameter )NULL;
        }
        else
            return nextParam;
    }
    else
    {
        //make sure we were not in a struct, else we should have found a next item
        JS_ASSERT( !bNextExists );

        //case where we were at the root and there is no more items
        return ( CGparameter )NULL;
    }
}

/**
@short Get the first child parameter from a struct parameter

cgGetFirstStructParameter() returns the first member parameter of a
struct parameter.  The rest of the members may be retrieved from the
first member by iterating with the cgGetNextParameter() function.

@param       param   Specifies the struct parameter.
This parameter must be of type <CODE>CG_STRUCT</CODE> (returned by cgGetParameterType()).

@return Returns a handle to the first member parameter.
<CODE>NULL</CODE> is returned if <I><c>param</c></I> is not a struct or if some other error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetNextParameter, cgGetFirstParameter
*/
CG_API CGparameter cgGetFirstStructParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetFirstStructParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetFirstStructParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return ( CGparameter )NULL;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_STRUCTURE ) != CGP_STRUCTURE )
    {
        // failure, there was no child.
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( parameterEntry->flags & CGP_RTCREATED )
    {
        JS_ASSERT( 0 && "Currently can not create struct parameters at runtime.  This should not happen." );
    }
#endif

    const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, parameterEntry );
    if ( parameterStructure->memberCount > 0 ) //is is needed ?
    {
        CgRuntimeParameter *firstStructureItem = rtParameter + 1;
        return firstStructureItem->id;
    }
    else
        return ( CGparameter )NULL; //we have a struct with 0 items ?
}

/**
@short Get a struct parameter by name

The member parameters of a struct parameter may be retrieved directly by
name using the cgGetNamedStructParameter() function.

The names of the parameters in a struct may be discovered by iterating
through the struct&rsquo;s member parameters (see cgGetFirstStructParameter()),
and calling cgGetParameterName() for each one in turn.

@param       param    Specifies the struct parameter to retrieve the member parameter from.

@param       name    Specifies the name of the member parameter to retrieve.

@return Returns the member parameter from the given struct.  If the struct
has no member parameter corresponding to <I><c>name</c></I>, a <CODE>NULL</CODE> is returned
(cgIsParameter() returns <CODE>CG_FALSE</CODE> for invalid parameters).

@par Errors
Use cgGetError() to retrieve the value of the error flag.
@par
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>name</c></I> is <CODE>NULL</CODE>.</TD>
</TR>
</TABLE>

@sa       cgGetFirstStructParameter, cgGetNextParameter, cgIsParameter, cgGetParameterName
*/
CG_API CGparameter cgGetNamedStructParameter( CGparameter param, const char* name )
{
    JS_APIENTRY_TIMER( cgGetNamedStructParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetNamedStructParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             name, strlen( name ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif // JS_DPM

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return ( CGparameter )NULL;

    // check input string
    if ( NULL == name )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return ( CGparameter )NULL;
    }

    if ( !( rtParameter->parameterEntry->flags & CGP_STRUCTURE ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return ( CGparameter )NULL;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( rtParameter->parameterEntry->flags & CGP_RTCREATED ) )
    {
        JS_ASSERT( 0 && "Currently can not create struct parameters at runtime.  This should not happen." );
    }
#endif

    const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, rtParameter->parameterEntry );

    int arrayIndex = -1;
    CgRuntimeParameter *structParam = ( CgRuntimeParameter * )_cgGetNamedParameter( rtParameter->program, name, ( CGenum )0, &arrayIndex, rtParameter->parameterEntry + 1, parameterStructure->memberCount );
    if ( structParam )
    {
        JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
        int ret = ( int )structParam->id;
        if ( arrayIndex != -1 )
            ret |= ( arrayIndex << CG_PARAMETERSIZE );
        return ( CGparameter )ret;
    }
    else
        return ( CGparameter )NULL;
}

/**
 *
@short Get the first dependent parameter from a parameter

cgGetFirstDependentParameter() returns the first member dependent parameter
associated with a given parameter.
The rest of the members may be retrieved from the first member by iterating with the cgGetNextParameter() function.

Dependent parameters are parameters that have the same name as a given parameter
but different resources.
They only exist in profiles that have multiple resources associated with one parameter.

@note
cgGetFirstDependentParameter() is obsolete for parameter traversal and
is <I>not</I> relevant for the PSGL implementation.

@param       param    Specifies the parameter.

@return cgGetFirstDependentParameter() returns a handle to the first member parameter.
@par
<CODE>NULL</CODE> is returned if <I><c>param</c></I> is not a struct or if some other error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetFirstParameter, cgGetNextParameter

*/
CG_API CGparameter cgGetFirstDependentParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetFirstDependentParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetFirstDependentParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }
    return ( CGparameter )NULL;
}

/**
@short Get a parameter from an array

cgGetArrayParameter() returns the parameter of array <I><c>param</c></I> specified by <i><c>arrayIndex</c></i>.
This function is used when inspecting elements of an array
parameter in a program.

@param       param   Specifies the array parameter handle.
@param       arrayIndex   Specifies the index into the array.

@return Returns the parameter at the specified index of <I><c>param</c></I> if <I><c>param</c></I> references an array and the index is valid.
@par
Returns <CODE>NULL</CODE> otherwise.

@par Examples
@code
CGparameter array = ...; // some array parameter
int array_size = cgGetArraySize( array );
for(i=0; i < array_size; ++i)
   {
      CGparameter element = cgGetArrayParameter(array, i);
      // Do stuff to element
   }
@endcode
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>arrayIndex</c></I> is outside the bounds of the array.</TD>
</TR>
</TABLE>

@sa  cgGetArrayDimension, cgGetArraySize, cgGetParameterType
*/
CG_API CGparameter cgGetArrayParameter( CGparameter param, int arrayIndex )
{
    JS_APIENTRY_TIMER( cgGetArrayParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetArrayParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             arrayIndex, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_cgGLTestArrayParameter( param, arrayIndex, 1 );
    if ( !rtParameter )
        return ( CGparameter )NULL;

    CGparameter arrayFirstItemID;
    if ( rtParameter->parameterEntry->flags & CGP_UNROLLED )
    {
        //move over the first item of the array and starts from here
        rtParameter++;
        arrayFirstItemID = rtParameter->id;
    }
    else
    {
        // SPECIAL CASE FOR ARRAY_OF_SAMPLERS
        if ( JS_UNLIKELY( !( rtParameter->parameterEntry->flags & ( CGP_STRUCTURE | CGP_ARRAY ) ) ) &&
                isSampler( _jsGetParameterCGtype( rtParameter->program, rtParameter->parameterEntry ) ) )
        {
            for ( int i = 0; i < arrayIndex; ++i )
            {
                rtParameter++;
            }
            return rtParameter->id;
        }
        // END SPECIAL CASE FOR ARRAY_OF_SAMPLERS

        //move to the type item
        rtParameter++;
        //and create the ID for the item 0
        //I know this is stupid, and that this really the same as the previous case, but that's to make the code understandable
        arrayFirstItemID = ( CGparameter )((( unsigned int )rtParameter->id ) | ( 0 << CG_PARAMETERSIZE ) );
    }
    CGparameter arrayItemID = _jsAdvanceParameter( arrayFirstItemID, arrayIndex );
    JS_ASSERT( arrayItemID );
    return arrayItemID;
}

/**
@short Get the dimension of an array parameter

cgGetArrayDimension() returns the dimension of the array specified by
<I><c>param</c></I>.  This function is used when inspecting an array parameter in a program.

@param       param   Specifies the array parameter handle.

@return Returns the dimension of <I><c>param</c></I> if <I><c>param</c></I> references an array.
@par
Returns 0 otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is invalid or not an array.</TD>
</TR>
</TABLE>

@sa cgGetNextParameter, cgGetArraySize, cgGetArrayParameter
*/
CG_API int cgGetArrayDimension( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetArrayDimension );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetArrayDimension, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // return how many dimensions this array has.
    // TODO: API should be an unsigned int.

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return -1;

    // ***** NOT IN CG DOCUMENTATION, but should be ****

    // this is a rarely queried value, so we do not store it.
    // instead we calculate it every time it is requested.
    // recurse down the array tree until the parameter type is not CG_ARRAY
    // counting the depth as we go.

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_ARRAY ) == 0 )
    {
        // ***** NOT IN CG DOCUMENTATION, but should be ****
        _jsCgRaiseError( CG_ARRAY_PARAM_ERROR );
        return 0;
    }
#ifndef JS_NO_ERROR_CHECK
    else if ( JS_UNLIKELY( parameterEntry->flags & CGP_RTCREATED ) )
    {
        JS_ASSERT( 0 && "Runtime array parameters can not be created.  This should not happen." );
    }
#endif
    else
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( rtParameter->program, parameterEntry );
        return ( int )parameterArray->dimensionCount;
    }
    return 0;
}

/**
@short Get the type of an array parameter

cgGetArrayType() returns the type of the members of an array.  If the
given array is multi-dimensional, it will return the type of the members of the innermost array.

@param       param   Specifies the array parameter handle.

@return Returns the type of the inner most array.
Returns <CODE>CG_UNKNOWN_TYPE</CODE> if an error is thrown.

@par Examples
@code
CGcontext Context = cgCreateContext();
CGparameter MyArray = cgCreateParameterArray(Context, CG_FLOAT, 5);
CGtype MyArrayType = cgGetArrayType(MyArray); // This will return CG_FLOAT
@endcode
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetArraySize, cgGetArrayDimension
*/
CG_API CGtype cgGetArrayType( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetArrayType );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetArrayType, JSDPM_CALL_TYPE_STATE_CG, CG_UNKNOWN_TYPE, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // return the type of the leaf node of this CG_ARRAY type

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_UNKNOWN_TYPE;

    // ***** NOT IN CG DOCUMENTATION, but should be ****

    // this is a rarely queried value, so we do not store it.
    // instead we calculate it every time it is requested.
    // recurse down the array tree until the parameter type is not CG_ARRAY
    // and return the first child that is not a CG_ARRAY type.

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_ARRAY ) == 0 )
    {
        // ***** NOT IN CG DOCUMENTATION, but should be ****
        _jsCgRaiseError( CG_ARRAY_PARAM_ERROR );
        return CG_UNKNOWN_TYPE;
    }
#ifndef JS_NO_ERROR_CHECK
    else if ( JS_UNLIKELY( parameterEntry->flags & CGP_RTCREATED ) )
    {
        JS_ASSERT( 0 && "Runtime array parameters can not be created.  This should not happen." );
    }
#endif
    else
    {
        //get the parameter type inside the array
        rtParameter++;
        parameterEntry = rtParameter->parameterEntry;
        if ( parameterEntry->flags & CGP_STRUCTURE )
        {
            return CG_STRUCT;
        }
#ifndef JS_NO_ERROR_CHECK
        else if ( JS_UNLIKELY( parameterEntry->flags & CGP_ARRAY ) ) //unlikely...since it's not supported
        {
            JS_ASSERT( 0 && "Arrays of arrays can not be created or exist in any way.  This should not happen." );
            //return CG_ARRAY;
        }
#endif
        else
        {
// when we are able to support runtime created arrays of params we must handle that here
            const CgParameterResource *parameterResource = _jsGetParameterResource( rtParameter->program, parameterEntry );
            return ( CGtype )parameterResource->type;
        }
    }
    return CG_UNKNOWN_TYPE;
}

/**
@short Get the size of one dimension of an array parameter

cgGetArraySize() returns the size one the specified dimension of the
array specified by <I><c>param</c></I>.  This function is used when inspecting an
array parameter in a program.

@param       param   Specifies the array parameter handle.

@param       dimension  Specifies the dimension whose size will be returned.

@return Returns the size of <I><c>param</c></I> if <I><c>param</c></I> references an array.
@par
Returns -1 on error.

@par Examples
@code
// compute the number of elements in an array
if( cgIsArray( param ) ) {
   int dim = cgGetArrayDimension( param );
   int elements = cgGetArraySize( param, 0);
   for( int i=1; i < dim; i++ ) {
      elements *= cgGetArraySize( param, i );
   }
   ...
}
@endcode
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_DIMENSION_ERROR</CODE></TD>
<TD>Dimension is less than 0.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is an invalid
parameter handle or not an array.</TD>
</TR>
</TABLE>

@sa cgGetNextParameter, cgGetArrayDimension, cgGetArrayParameter
*/
CG_API int cgGetArraySize( CGparameter param, int dimension )
{
    JS_APIENTRY_TIMER( cgGetArraySize );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetArraySize, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             dimension, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return -1;

    // this is another rarely queried value (see also cgGetArrayDimension), so we
    // do not store it. Instead we calculate it every time it is requested.
    // recurse down the array tree decrementing "dimension" until either it reached zero
    // and we return the arraySize or we fail to find a child in which case the
    // dimension was invalid.

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_ARRAY ) == 0 )
    {
        // ***** NOT IN CG DOCUMENTATION, but should be ****
        _jsCgRaiseError( CG_ARRAY_PARAM_ERROR );
        return CG_UNKNOWN_TYPE;
    }
#ifndef JS_NO_ERROR_CHECK
    else if ( JS_UNLIKELY( parameterEntry->flags & CGP_RTCREATED ) )
    {
        JS_ASSERT( 0 && "Runtime array parameters can not be created.  This should not happen." );
    }
#endif
    else
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( rtParameter->program, parameterEntry );
        if ( dimension < 0 || dimension >= parameterArray->dimensionCount )
        {
            _jsCgRaiseError( CG_INVALID_DIMENSION_ERROR );
            return -1;
        }
        else
        {
            return ( int )parameterArray->dimensions[dimension];
        }
    }
    return -1;
}

/**
@short Sets the size of a resizable array parameter

cgSetArraySize() sets the size of a resizable array parameter <I><c>param</c></I> to <I><c>size</c></I>.

@param       param   Specifies the array parameter handle.
@param       size    Specifies the new size of the array.

@par Examples
@code
// If you have Cg program with a parameter like this:
// ...
float4 main(float4 myarray[])
{
   // ...
}

// You can set the size of an array parameter called "myarray" to 5 like this:
  CGparameter MyArrayParam =
              cgGetNamedProgramParameter(Program, CG_PROGRAM, "myarray");
  cgSetArraySize(MyArrayParam, 5);
@endcode
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is an invalid
parameter handle or not an array.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array param.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_DIMENSION_ERROR</CODE></TD>
<TD>The dimension of the array
parameter <I><c>param</c></I> is not 1.</TD>
</TR>
<TR>
<TD><CODE>CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not
a resizable array.</TD>
</TR>
</TABLE>

@sa       cgGetArraySize, cgGetArrayDimension, cgSetMultiDimArraySize
*/
CG_API void cgSetArraySize( CGparameter param, int size )
{
    JS_APIENTRY_TIMER( cgSetArraySize );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgSetArraySize, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             size, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // check that param is an array type
    // _jsCgRaiseError(CG_ARRAY_PARAM_ERROR);

    // check if the array is resizable
    // _jsCgRaiseError(CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR);

    // check array dimensionality
    //if (array.dimensions != 1)
    //{
    //  _jsCgRaiseError(CG_INVALID_DIMENSION_ERROR);
    //  return;
    //}

    return;
}

/**
@short    Sets the size of a resizable multi-dimensional array parameter

cgSetMultiDimArraySize() sets the size of each dimension of resiable
multi-dimensional array parameter <I><c>param</c></I>.  <I><c>sizes</c></I> must be an array that
has N number of elements where N is equal to the result of cgGetArrayDimension().

@param       param   Specifies the array parameter handle.
@param       sizes   An array of sizes for each dimension of the array.

@par Examples
@code
// If you have Cg program with a parameter like this :
// ...
float4 main(float4 myarray[][][])
{
// ...
}
//
// you can set the sizes of each dimension of the array parameter called "myarray" like this:
//
const int Sizes[] = { 3, 2, 4 };
CGparameter MyArrayParam =
            cgGetNamedProgramParameter(Program, CG_PROGRAM, "myarray");
cgSetMultiDimArraySize(MyArrayParam, Sizes);
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is an invalid parameter handle or not an array.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array param.</TD>
</TR>
<TR>
<TD><CODE>CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a resizable array.</TD>
</TR>
</TABLE>

@sa       cgGetArraySize, cgGetArrayDimension, cgSetArraySize
*/
CG_API void cgSetMultiDimArraySize( CGparameter param, const int* sizes )
{
    JS_APIENTRY_TIMER( cgSetMultiDimArraySize );
    // XXX cg api called
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgSetArraySize, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             sizes, cgGetArrayDimension( param )*sizeof( int ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif // JS_DPM
    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // check that param is an array type
    // _jsCgRaiseError(CG_ARRAY_PARAM_ERROR);

    // check if the array is resizable
    // _jsCgRaiseError(CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR);

    return;
}

/**
@short Get a parameter&rsquo;s parent program

cgGetParameterProgram() allows the application to retrieve a handle to
the program a given parameter belongs to.

@param       param   Specifies the parameter.

@return Returns a <c>CGprogram</c> handle to the parent program.  In the event of an error <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a parameter.</TD>
</TR>
</TABLE>

@sa       cgCreateProgram, cgCreateContext
*/
CG_API CGprogram cgGetParameterProgram( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
    {
        return ( CGprogram )NULL;
    }
    else if ( JS_UNLIKELY( rtParameter->parameterEntry->flags & CGP_RTCREATED ) )
    {
        // I don't think we want to expose the fact that we internally store runtime created effect and context parameters in a program
        return ( CGprogram )NULL;
    }
    else
    {
        return rtParameter->program->id;
    }
    return ( CGprogram )NULL;
}

/**
@short Get a parameter&rsquo;s parent context

cgGetParameterContext() allows the application to retrieve a handle to
the context a given parameter belongs to.

@param       param   Specifies the parameter.

@return Returns a <I>CGcontext</I> handle to the parent context.
In the event of an error <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid context.</TD>
</TR>
</TABLE>

@sa       cgGetParameterProgram, cgGetParameterEffect
*/
CG_API CGcontext cgGetParameterContext( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterContext );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterContext, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return ( CGcontext )NULL;
    }

    CGcontext result = cgGetProgramContext( cgGetParameterProgram( param ) );

    return result;
}



/**
@short Determine if a CGparameter references a valid Cg parameter object

cgIsParameter() returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> references a valid parameter
object.  This function is typically used for iterating through the
parameters of an object.  It can also be used as a consistency check
when the application caches <c>CGparameter</c> handles.  Certain program operations
like deleting the program or context object that the parameter
is contained in will cause a parameter object to become invalid.

@param       param   Specifies the parameter handle to check.

@return Returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> references a valid parameter object.
@par
Returns <CODE>CG_FALSE</CODE> otherwise.

@sa cgGetNextParameter
*/
CG_API CGbool cgIsParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgIsParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsParameter, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    if ( JS_LIKELY( CG_IS_PARAMETER( param ) ) )
    {
        return CG_TRUE;
    }
    return CG_FALSE;
}

/**
@short Get a program parameter&rsquo;s type

cgGetParameterType() allows the application to retrieve the type of a
parameter in a Cg program.  This type is necessary for the application
to be able to supply the program&rsquo;s inputs and use the program&rsquo;s outputs.

cgGetParameterType() will return <CODE>CG_STRUCT</CODE> if the parameter is a struct
and <CODE>CG_ARRAY</CODE> if the parameter is an array.  Otherwise it will return
the data type associated with the parameter.

@param       param   Specifies the program parameter.

@return Returns the type enumerant of <I><c>param</c></I>.  If an error occurs, <CODE>CG_UNKNOWN_TYPE</CODE> will be returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetType, cgGetTypeString
*/
CG_API CGtype cgGetParameterType( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterType );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterType, JSDPM_CALL_TYPE_STATE_CG, CG_UNKNOWN_TYPE, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    /*// check input for validity
    if (JS_UNLIKELY(!CG_IS_PARAMETER(param)))
    {
        _jsCgRaiseError(CG_INVALID_PARAM_HANDLE_ERROR);
        return CG_UNKNOWN_TYPE;
    }
    return _cgGetParamPtr(param)->type;*/
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_UNKNOWN_TYPE;
    else
    {
        if ( rtParameter->parameterEntry->flags & CGP_ARRAY )
            return CG_ARRAY;
        else if ( rtParameter->parameterEntry->flags & CGP_STRUCTURE )
            return CG_STRUCT;
        else
        {
            return _jsGetParameterCGtype( rtParameter->program, rtParameter->parameterEntry );
        }
    }
}

/**
@short Get a program parameter&rsquo;s type

cgGetParameterNamedType() returns the type of <I><c>param</c></I> similarly to cgGetParameterType().
However, if the type is a user defined struct it will
return the unique enumerant associated with the user defined type
instead of <CODE>CG_STRUCT</CODE>.

@param       param   Specifies the parameter.

@return Returns the type of <I><c>param</c></I>.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa       cgGetParameterType
*/
CG_API CGtype cgGetParameterNamedType( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterNamedType );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterNamedType, JSDPM_CALL_TYPE_STATE_CG, CG_UNKNOWN_TYPE, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    return cgGetParameterType( param );
}

/**
@short Get a program parameter&rsquo;s semantic

cgGetParameterSemantic() allows the application to retrieve the semantic
of a parameter in a Cg program.  If the parameter does not have a
semantic assigned to it, an empty string will be returned.

@param       param   Specifies the program parameter.

@return Returns the null-terminated semantic string for the parameter.
@par
Returns null if an error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter is not a leaf node.</TD>
</TR>
</TABLE>

@sa cgGetParameterResource, cgGetParameterResourceIndex, cgGetParameterName, cgGetParameterType
*/
CG_API const char* cgGetParameterSemantic( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterSemantic );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterSemantic, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return NULL;
    else
    {
        const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;

        unsigned short type = parameterEntry->flags & CGP_TYPE_MASK;
        if ( type == CGP_STRUCTURE || CGP_STRUCTURE == CGP_ARRAY )
        {
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            return NULL;
        }

        // this table holds semantics that were created at runtime for either program or context or effect scope parameters
        // this search happens first because you can overwrite what was set in the binary at runtime
        // and we won't remove the entry from the binary representation of semantics
        // but we will add an entry into this table which should be returned here.
        STL_NAMESPACE vector<CgRuntimeSemantic>::iterator semanticIter = rtParameter->program->parameterSemantics.begin();
        while ( semanticIter != rtParameter->program->parameterSemantics.end() )
        {
            if ( semanticIter->param == rtParameter )
            {
                return &semanticIter->semantic[0];
            }
            semanticIter++;
        }

        size_t entryIndex = ( parameterEntry - rtParameter->program->parametersEntries );

        //look for the parameter semantic in the semantic table for semantics set in the compiled source
        int count = rtParameter->program->semanticCount;
        int i;
        for ( i = 0;i < count;i++ )
        {
            const CgParameterSemantic *semantic = rtParameter->program->semanticIndices + i;
            if ( semantic->entryIndex == ( unsigned short )entryIndex )
            {
                //found
                return rtParameter->program->stringTable + semantic->semanticOffset;
            }
        }

        //not found, we don't have the semantic for this parameter, returns empty strings
        return "";
    }
}

/**
@short Get a program parameter&rsquo;s base resource

cgGetParameterBaseResource() allows the application to retrieve the base
resource for a parameter in a Cg program.  The base resource is the
first resource in a set of sequential resources.  For example, if a
given parameter has a resource of <CODE>CG_ATTR7</CODE>, its base resource would be
<CODE>CG_ATTR0</CODE>.  Only parameters with resources whose name ends with a number
will have a base resource.  All other parameters will return the undefined
resource <CODE>CG_UNDEFINED</CODE> when calling cgGetParameterBaseResource().

The numerical portion of the resource may be retrieved with the
cgGetParameterResourceIndex() function.  For example, if the resource for a
given parameter is <CODE>CG_ATTR7</CODE>, cgGetParameterResourceIndex() will return 7.

@param       param   Specifies the program parameter.

@return Returns the base resource of <I><c>param</c></I>.  If no base resource exists for the
given parameter, <CODE>CG_UNDEFINED</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter is not a leaf node.</TD>
</TR>
</TABLE>

@sa cgGetParameterResource, cgGetParameterResourceIndex, cgGetResourceString
*/
CG_API CGresource cgGetParameterBaseResource( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterBaseResource );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterBaseResource, JSDPM_CALL_TYPE_STATE_CG, CG_UNDEFINED, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_UNDEFINED;
    else
    {
        const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;

        unsigned short type = parameterEntry->flags & CGP_TYPE_MASK;
        if ( type == CGP_STRUCTURE || type == CGP_ARRAY )
        {
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            return CG_UNDEFINED;
        }

        CGresource resource = cgGetParameterResource( param );
        return _jsGetBaseResource( resource );
    }
}

/**
@short Get the first leaf parameter in a program

cgGetFirstLeafParameter() returns the first leaf parameter in a program.
The combination of cgGetFirstLeafParameter() and cgGetNextLeafParameter()
allow the iteration through all of the parameters of basic data types
(not structs or arrays) without recursion.  See the cgGetNextLeafParameter()
manpage for more information.

@param       prog    Specifies the program to retrieve the first leaf parameter from.
@param       name_space Specifies the namespace of the parameter to iterate through.

@return Returns the first leaf <c>CGparameter</c> object in <I><c>prog</c></I>.
<CODE>NULL</CODE> is returned if <I><c>prog</c></I> is
invalid or if <I>prog</I> does not have any parameters.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa       cgGetNextLeafParameter
*/
CG_API CGparameter cgGetFirstLeafParameter( CGprogram prog, CGenum name_space )
{
    JS_APIENTRY_TIMER( cgGetFirstLeafParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetFirstLeafParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             name_space, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

    // check program handle
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return ( CGparameter )NULL;
    }

    _CGprogram* progPtr = _cgGetProgPtr( prog );
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )progPtr->runtimeParameters;

    //find the first leaf param of the requested namespace
    for ( int i = 0; i < progPtr->rtParametersCount; ++i )
    {
        // check parameter handle
        bool isGlobal = ( rtParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;
        if (( isGlobal && name_space == CG_GLOBAL ) || ( !isGlobal && name_space == CG_PROGRAM ) )
        {
            int type = ( rtParameter->parameterEntry->flags &  CGP_TYPE_MASK );
            if ( type == CGP_INTRINSIC )
                return ( CGparameter )rtParameter->id;
            else if ( type == CGP_ARRAY )
            {
                JS_ASSERT( type == CGP_ARRAY );
                if ( !( rtParameter->parameterEntry->flags & CGP_UNROLLED ) )
                {
                    //can we have a parameter of size 0 ?
                    const CgParameterArray *parameterArray = _jsGetParameterArray( rtParameter->program, rtParameter->parameterEntry );
                    int arraySize = ( int )_jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                    rtParameter++; //we move on the type parameter
                    if ( arraySize )
                    {
                        //sending back ARRAY ITEM 0 here, should really use a macro to create the CGparameter handle
                        return rtParameter->id;
                    }
                    //else we just continue
                }
                //else unrolled, just continue
            }
            //else if (type == CGP_STRUCTURE)  //just continue
        }
        rtParameter++;
    }
    return ( CGparameter )NULL;
}

/**
@short Get the next leaf parameter in a program

cgGetNextLeafParameter() returns the leaf parameter (not struct or array
parameters) following a given leaf parameter.
The following is an example of how to iterate through all the leaf
parameters in a program :
@code
CGparameter leaf = cgGetFirstLeafParameter( prog );
while(leaf)
   {
      // do stuff with leaf
      leaf = cgGetNextLeafParameter( leaf );
   }
@endcode

@param       param    Specifies a current leaf parameter.

@return Returns the next leaf <c>CGparameter</c> object.
<CODE>NULL</CODE> is returned if <I><c>prog</c></I> is invalid
or if <I><c>prog</c></I> does not have any more leaf parameters.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgGetFirstLeafParameter
*/
CG_API CGparameter cgGetNextLeafParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetNextLeafParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNextLeafParameter, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return ( CGparameter )NULL;

    //this parameter should be an intrinsic
    if (( rtParameter->parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return ( CGparameter )NULL;
    }

    // runtime created parameters are treated separately because they are in a different namespace
    // as such, you never need to traverse from a program param to a runtime param, or visa-versa
    // additionally, in the current implementation, we only support creation of leaf types.
    if ( rtParameter->parameterEntry->flags & CGP_RTCREATED )
    {
        // we have a runtime created parameter
        STL_NAMESPACE vector<CgRuntimeParameter*>::iterator rtCreatedIter = rtParameter->program->runtimeCreatedParameters.begin();
        while ( rtCreatedIter != rtParameter->program->runtimeCreatedParameters.end() )
        {
            if ( *rtCreatedIter == rtParameter )
            {
                rtCreatedIter++;
                if ( rtCreatedIter == rtParameter->program->runtimeCreatedParameters.end() )
                {
                    break;
                }
                return ( *rtCreatedIter )->id;
            }
            rtCreatedIter++;
        }
        // no next parameter for this one
        return ( CGparameter )NULL;
    }

    // the case of the array element is easy to solve:
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if (( previousParameter->parameterEntry->flags & CGP_ARRAY ) && !( previousParameter->parameterEntry->flags & CGP_UNROLLED ) )
        {
            //we have an array item
            int arrayIndex = CG_GETINDEX( param );
            arrayIndex++;
            const CgParameterArray *parameterArray =  _jsGetParameterArray( previousParameter->program, previousParameter->parameterEntry );
            int arraySize = _jsGetSizeofSubArray(( short* )parameterArray->dimensions, parameterArray->dimensionCount );
            if ( arrayIndex < arraySize )
            {
                JS_ASSERT( arrayIndex < ( 1 << ( 32 - CG_PARAMETERSIZE ) ) && "the max array size in registers is 1024" );
                int ret = ( int )rtParameter->id;
                ret |= ( arrayIndex << CG_PARAMETERSIZE );
                return ( CGparameter )ret;
            }
            //else
            //we need to leave this array and continue the traversal
        }
    }

    bool isCurrentGlobal = ( rtParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;

    const CgParameterEntry *endEntry = rtParameter->program->parametersEntries + rtParameter->program->rtParametersCount;
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry + 1;
    while ( paramEntry < endEntry )
    {
        size_t offset = ( paramEntry - rtParameter->parameterEntry );
        CgRuntimeParameter *nextParameter = rtParameter + offset;
        bool isNextGlobal = ( nextParameter->parameterEntry->flags & CGPF_GLOBAL ) == CGPF_GLOBAL;
        if ( isNextGlobal != isCurrentGlobal )
		{
			paramEntry++; // let's not quit, but continue to look for a matching namespace 
			continue;
            //return ( CGparameter )NULL; //the items are gathered by namespace, so if different namespace then we quit
		}
        switch ( paramEntry->flags & CGP_TYPE_MASK )
        {
            case CGP_ARRAY:
            {
                if ( paramEntry->flags & CGP_UNROLLED )
                {
                    //we skip unrolled array
                    break;
                }
                else
                {
                    //we need to go inside and send back the first array item
                    //check how many item do we have in the array
                    const CgParameterArray *parameterArray = _jsGetParameterArray( rtParameter->program, paramEntry );
                    int arraySize = ( int )_jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                    if ( arraySize )
                    {
                        nextParameter++; //go on the array time
                        //sending back ARRAY ITEM 0 here, should really use a macro to create the CGparameter handle
                        return nextParameter->id;
                    }
                    else
                    {
                        //no item in the array skip the item time continue
                        nextParameter++; //go on the array time, will be skipped
                    }
                }
                break;
            }
            case CGP_STRUCTURE:
            {
                //we just skip structures
                break;
            }
            case CGP_INTRINSIC:
                //we are done we found an intrinsic
                return nextParameter->id;
                break;
        }
        paramEntry++;
    }
    return ( CGparameter )NULL;
}

/**
 * @}
 */

static bool _jsPrependString( char *dst, const char *src, size_t size )
{
    int len = strlen( src );
    int previousLen = strlen( dst );
    int spaceLeft = size - ( previousLen + 1 ); //+1 for white space
    if ( spaceLeft < len )
    {
        JS_ASSERT( spaceLeft < len && "parameter name too long ( > 128 )" );
        return false;
    }
    memmove( dst + len, dst, previousLen + 1 );
    strncpy( dst, src, len );
    return true;
}

/** @addtogroup Cg
 *@{
 */

/**
@short Get a program parameter&rsquo;s name

cgGetParameterName() allows the application to retrieve the name of a
parameter in a Cg program.  This name can be used later to retrieve the
parameter from the program using cgGetNamedParameter().

@param       param   Specifies the program parameter.

@return Returns the null-terminated name string for the parameter.
Returns NULL if <I><c>param</c></I> is invalid.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a
valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetNamedParameter, cgGetNextParameter, cgGetParameterType, cgGetParameterVariability,
cgGetParameterDirection, cgSetParameterVariability,
*/
CG_API const char* cgGetParameterName( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterName );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterName, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return NULL;

    // runtime created parameters have their names stored in the entry differently than compile created params
    if ( rtParameter->parameterEntry->flags & CGP_RTCREATED )
    {
        return reinterpret_cast<const char*>( rtParameter->parameterEntry->nameOffset );
    }

    char *currentParameterName = rtParameter->program->parentContext->currentParameterName;
    currentParameterName[0] = '\0';
    size_t stringSize = sizeof( rtParameter->program->parentContext->currentParameterName );

    //I walk down the parameterEntry list until I find the root
    const CgParameterEntry *paramEntry = rtParameter->parameterEntry;
    const CgParameterEntry *firstEntry = rtParameter->program->parametersEntries;

    //start by the current name
    bool res = _jsPrependString( currentParameterName, rtParameter->program->stringTable + paramEntry->nameOffset, stringSize );
    if ( !res )
        return NULL;
    //are we starting from an array ?
    if ( paramEntry > firstEntry )
    {
        const CgParameterEntry *previousEntry = paramEntry - 1;
        if (( previousEntry->flags & CGP_ARRAY ) && !( previousEntry->flags & CGP_UNROLLED ) )
        {
            //ok we are in an non unrolled array
            //I need to append the array index, I should use the dimensions , no time for now
            int index = CG_GETINDEX( param );
            //should divide the index on the dimensions... later...
            char buffer[256];
            sprintf( buffer, "[%i]", index );
            if ( strlen( currentParameterName ) + strlen( buffer ) + 1 < stringSize )
                strcat( currentParameterName, buffer );
            else
            {
                JS_ASSERT( 0 && "parameter name too long ( > 128 )" );
                return NULL;
            }
            //prepend array name
            res = _jsPrependString( currentParameterName, rtParameter->program->stringTable + previousEntry->nameOffset, stringSize );
            if ( !res )
                return NULL;
            paramEntry--;
        }
    }

    //we have already treated the current entry at that point, the loop starts on the previous one, the distance is 1
    int distance = 1;
    paramEntry--;

    while ( paramEntry >= firstEntry )
    {
        switch ( paramEntry->flags & CGP_TYPE_MASK )
        {
            case CGP_ARRAY:
                distance--; // the array has one extra item, whether it's a structure or if it's an intrinsic type
                break;
            case CGP_STRUCTURE:
            {
                const CgParameterStructure *parameterStructure = _jsGetParameterStructure( rtParameter->program, paramEntry );
                if ( distance > parameterStructure->memberCount )
                {
                    //the parameter is not in this structure, so I need to remove from the distance all the structure item
                    distance -= parameterStructure->memberCount;
                }
                else
                {
                    //the parameter is in this structure, prepend the name
                    res = _jsPrependString( currentParameterName, ".", stringSize );
                    if ( !res ) return NULL;
                    res = _jsPrependString( currentParameterName, rtParameter->program->stringTable + paramEntry->nameOffset, stringSize );
                    if ( !res ) return NULL;
                    distance = 0;
                }
                break;
            }
            case CGP_INTRINSIC:
                break;
        }
        distance++;
        paramEntry--;
    }
    return currentParameterName;

}


/**
@short Get a program parameter&rsquo;s resource

cgGetParameterResource() allows the application to retrieve the resource
for a parameter in a Cg program.  This resource is necessary for the
application to be able to supply the program&rsquo;s inputs and use the program&rsquo;s outputs.

The resource enumerant is a profile-specific hardware resource.

@param       param   Specifies the program parameter.

@return Returns the resource of <I><c>param</c></I>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter is not a leaf node.</TD>
</TR>
</TABLE>

@sa cgGetParameterResourceIndex, cgGetParameterBaseResource, cgGetResourceString
*/
CG_API CGresource cgGetParameterResource( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterResource );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterResource, JSDPM_CALL_TYPE_STATE_CG, CG_UNDEFINED, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
    {
        return CG_UNDEFINED;
    }

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;

    if ( parameterEntry->flags & CGP_RTCREATED )
    {
        // runtime created parameters do not have resources
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return CG_UNDEFINED;
    }

    unsigned short type = parameterEntry->flags & CGP_TYPE_MASK;
    if ( type == CGP_STRUCTURE || type == CGP_ARRAY )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return CG_UNDEFINED;
    }

    // TODO *************  docs say something incoherent about returning
    // CG_INVALID_VALUE if cgIsParameterReferenced() == CG_FALSE?
    if ( !( parameterEntry->flags & CGPF_REFERENCED ) )
    {
        return CG_UNDEFINED;
    }

    int arrayIndex = -1;
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if ( previousParameter->parameterEntry->flags & CGP_ARRAY )
        {
            //we have an array item
            arrayIndex = CG_GETINDEX( param );
        }
    }

    unsigned long resourceIndex = 0;
    CGresource resource = ( CGresource )0;
    unsigned short numEmbeddedConstants;
    _jsCgGetParameterResourceInfo( rtParameter, arrayIndex, &resource, &resourceIndex, &numEmbeddedConstants );

    // this returns undefined for array items that are not allocated
    if (arrayIndex >= 0 && resourceIndex == -1U && numEmbeddedConstants == 0)
	return CG_UNDEFINED;

    return resource;
}


/**
@short Get a program parameter&rsquo;s resource index

cgGetParameterResourceIndex() allows the application to retrieve the
resource index for a parameter in a Cg program.  This index value is
only used with resources that are linearly addressable.

@param       param   Specifies the program parameter.

@return Returns the resource index of <I><c>param</c></I>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter is not a leaf node.</TD>
</TR>
</TABLE>

@sa cgGetParameterResource, cgGetResource, cgGetResourceString
*/
CG_API unsigned long cgGetParameterResourceIndex( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterResourceIndex );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterResourceIndex, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
    {
        return CG_UNDEFINED;
    }

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;

    if ( parameterEntry->flags & CGP_RTCREATED )
    {
        // runtime created parameters do not have resources
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return CG_UNDEFINED;
    }

    unsigned short type = parameterEntry->flags & CGP_TYPE_MASK;
    if ( type == CGP_STRUCTURE || CGP_STRUCTURE == CGP_ARRAY )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return -1U;
    }

    // TODO *************  docs say something incoherent about returning
    // CG_INVALID_VALUE if cgIsParameterReferenced() == CG_FALSE?
    if ( !( parameterEntry->flags & CGPF_REFERENCED ) )
    {
        return -1U;
    }

    int arrayIndex = -1;
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *previousParameter = rtParameter - 1;
        if ( previousParameter->parameterEntry->flags & CGP_ARRAY )
        {
            //we have an array item
            arrayIndex = CG_GETINDEX( param );
        }
    }

    unsigned long resourceIndex = 0;
    CGresource resource = ( CGresource )0;
    unsigned short numEmbeddedConstants;
    _jsCgGetParameterResourceInfo( rtParameter, arrayIndex, &resource, &resourceIndex, &numEmbeddedConstants );
    if ( resource == CG_B || resource == CG_C )
        return ( unsigned long )resourceIndex;
    else
        return ( unsigned long )( resource - _jsGetBaseResource( resource ) );

}


/**
@short Get a program parameter&rsquo;s variability

cgGetParameterVariability() allows the application to retrieve the variability
of a parameter in a Cg program.  This variability is necessary
for the application to be able to supply the program&rsquo;s inputs and use
the program&rsquo;s outputs.

cgGetParameterVariability() returns one of the following values:

<TABLE rules=all>
<TR>
<TD><CODE>CG_VARYING</CODE></TD>
<TD>
A varying parameter is one whose value changes with each invocation of the program.
</TD>
</TR>
<TR>
<TD><CODE>CG_UNIFORM</CODE></TD>
<TD>
A uniform parameter is one whose value does not change with
each invocation of a program, but whose value can change
between groups of program invocations.
</TD>
</TR>
<TR>
<TD><CODE>CG_CONSTANT</CODE></TD>
<TD>
A constant parameter never changes for the life of a compiled
program.  Modifying a constant parameter requires program recompilation.
</TD>
</TR>
<TR>
<TD><CODE>CG_MIXED</CODE></TD>
<TD>
A structure parameter that contains parameters that differ in variability.
</TD>
</TR>
</TABLE>

@param       param   Specifies the program parameter.

@return Returns the variability of <I><c>param</c></I>.  Returns CG_ERROR if an error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa cgGetNamedParameter, cgGetNextParameter, cgGetParameterName, cgGetParameterType, cgGetParameterDirection,
cgSetParameterVariability
*/
CG_API CGenum cgGetParameterVariability( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterVariability );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterVariability, JSDPM_CALL_TYPE_STATE_CG, ( CGenum )0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return ( CGenum )0;
    else
    {
        unsigned int variability = rtParameter->parameterEntry->flags & CGPV_MASK;
        if ( variability == CGPV_UNIFORM )
            return CG_UNIFORM;
        else if ( variability == CGPV_VARYING )
            return CG_VARYING;
        else if ( variability == CGPV_CONSTANT )
            return CG_CONSTANT;
        else if ( variability == CGPV_MIXED )
            return CG_MIXED;
        else
            return ( CGenum )0;
    }
}


/**
@short Get a program parameter&rsquo;s direction

cgGetParameterDirection() allows the application to distinguish program
input parameters from program output parameters.  This information is
necessary for the application to properly supply the program inputs and
use the program outputs.

cgGetParameterDirection() will return one of the following enumerants:

<TABLE rules=all>
<TR>
<TD><CODE>CG_IN</CODE></TD>
<TD>
Specifies an input parameter.
</TD>
</TR>
<TR>
<TD><CODE>CG_OUT</CODE></TD>
<TD>
Specifies an output parameter.
</TD>
</TR>
<TR>
<TD><CODE>CG_INOUT</CODE></TD>
<TD>
Specifies a parameter that is both input and output.
</TD>
</TR>
<TR>
<TD><CODE>CG_ERROR</CODE></TD>
<TD>
If an error occurs.
</TD>
</TR>
</TABLE>

@param       param   Specifies the program parameter.

@return Returns the direction of <I><c>param</c></I>.  Returns <CODE>CG_ERROR</CODE> if an error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetNamedParameter, cgGetNextParameter, cgGetParameterName, cgGetParameterType, cgGetParameterVariability,
cgSetParameterVariability
*/
CG_API CGenum cgGetParameterDirection( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterDirection );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterDirection, JSDPM_CALL_TYPE_STATE_CG, ( CGenum )0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_ERROR;
    else
    {
        unsigned int direction = rtParameter->parameterEntry->flags & CGPD_MASK;
        if ( direction == CGPD_IN )
            return CG_IN;
        else if ( direction == CGPD_OUT )
            return CG_OUT;
        else if ( direction == CGPD_INOUT )
            return CG_INOUT;
        else
            return CG_ERROR;
    }
}

/**
@short Determine if a CGparameter is referenced within the program

cgIsParameterReferenced() returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> is referenced (used)
within the program.  It otherwise returns <CODE>CG_FALSE</CODE>.

The value returned from this routine may depend on various factors,
including the level of compiler optimization that was used when compiling the program.

If the return value is <CODE>CG_FALSE</CODE>, that indicates that cgGetParameterResource()
may return <CODE>CG_INVALID_VALUE</CODE> for this parameter.

@param param   Specifies the parameter handle to check.

@return Returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> is referenced.
@par
Returns <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa  cgIsParameterUsed, cgGetNextParameter, cgGetParameterResource
*/
CG_API CGbool cgIsParameterReferenced( CGparameter param )
{
    JS_APIENTRY_TIMER( cgIsParameterReferenced );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsParameterReferenced, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_FALSE;
    else
    {
        return (( rtParameter->parameterEntry->flags & CGPF_REFERENCED ) != 0 );
    }
}




/**
@short Test whether a parameter is used by an effect or program.

Test whether a parameter is used by an effect or program.

@param	param	The parameter handle to check.
@param	handle	The <code>CGeffect</code> or <code>CGprogram</code> to search for
                this parameter in.

@returns Returns <CODE>CG_TRUE</CODE> if the parameter is present and used by the
specified effect or program, or <CODE>CG_FALSE</CODE> otherwise.

@sa cgIsParameterReferenced, cgGetNextParameter, cgGetParameterResource

*/
CG_API CGbool cgIsParameterUsed( CGparameter param, CGhandle handle )
{
    JS_APIENTRY_TIMER( cgIsParameterUsed );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgIsParameterUsed, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             handle, sizeof( CGhandle ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    // TODO: make a more accurate and general definition of "used", including runtime created parameters.
    if ( !rtParameter )
    {
        return CG_FALSE;
    }
    
    if ( !CG_IS_PROGRAM( (CGprogram)handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_FALSE;
    }

    // adding support for effect handles would require moving this to PSGLFX or something that would break the abstraction of that library
    _CGprogram* progPtr = _cgGetProgPtr( (CGprogram)handle ); 
    
    if ( !progPtr )
    {
        return CG_FALSE;
    }

    if (rtParameter->program != progPtr)
	{
	    return CG_FALSE;
	}
    
    return (( rtParameter->parameterEntry->flags & CGPF_REFERENCED ) != 0 );
}




/**
@short Get a program parameter&rsquo;s values

cgGetParameterValues() allows the application to retrieve default, constant, or current values from
uniform parameters.

@param paramIn Specifies the program parameter.

@param value_type
Determines what type of value to return.  Valid enumerants are:
\arg <CODE>CG_CONSTANT:</CODE> Returns the constant values for
parameters that have constant variability.  See cgGetParameterVariability() for more information.
This enumerant is currently not supported.
\arg <CODE>CG_DEFAULT:</CODE> Returns the default values for a uniform parameter.
\arg <CODE>CG_CURRENT:</CODE> Returns the current values for a uniform parameter.

@param nvalues Points to memory where the number of values will be stored and returned.

@returns A pointer to an array of double values.  The number of values
in the array is returned via the <I><c>nvalues</c></I> parameter.
If no values are available, <CODE>NULL</CODE> will be returned and <I><c>nvalues</c></I> will be 0.

@note
<CODE>CG_CONSTANT</CODE> is not supported because of compiler optimizations.
cgGetParameterValues() only returns valid values for <CODE>CG_CURRENT</code> and <code>CG_DEFAULT</CODE>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>paramIn</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>nvalues</c></I> is <CODE>NULL</CODE>.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD>The <I><c>value_type</c></I> parameter is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetParameterVariability
*/
CG_API const double* cgGetParameterValues( CGparameter paramIn, CGenum value_type, int *nvalues )
{
    JS_APIENTRY_TIMER( cgGetParameterValues );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGetParameterValues, JSDPM_CALL_TYPE_STATE_CG, NULL, 3,
                             paramIn, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             value_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             nvalues, sizeof( int ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    // check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( paramIn );
    if ( !rtParameter )
        return NULL;

    // check nvalues pointer
    if ( NULL == nvalues )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return NULL;
    }

    // check if we are trying to get the current value of a uniform that is
    // unreferenced and unbound (happens when compiler folds global constants),
    // we can only return the default value in this case.
    if ( value_type == CG_CURRENT )
    {
        int var = ( rtParameter->parameterEntry->flags & CGPV_MASK );
        if ( var == CGPV_UNIFORM &&
                !( rtParameter->parameterEntry->flags & CGPF_REFERENCED ) &&
                !( rtParameter->parameterEntry->flags & CGP_RTCREATED ) &&
                !( rtParameter->parameterEntry->flags & CGP_ARRAY ) )
        {
            const CgParameterResource* parameterResource = _jsGetParameterResource( rtParameter->program, rtParameter->parameterEntry );
            if ( parameterResource )
            {
                if ( parameterResource->resource == 0xffff )
                {
                    value_type = CG_DEFAULT;
                }
            }
        }

        // this is the case for const parameters that never have a push buffer allocated
        // because they can not be set since they are const in the shader, so the value
        // will always be the defaults.
        if ( rtParameter->pushBufferPointer == NULL )
        {
            value_type = CG_DEFAULT;
        }
    }

    // as this function returns a pointer to an array of doubles we have
    // to allocate space for this array, make sure it's never re-allocated for
    // this parameter, plus make sure it is deleted when the CGcontext is
    // destroyed.
    //
    // All in all a nasty API function.

    double values[16];
    *nvalues = 0;
    _CGprogram *program = rtParameter->program;

    // check value type then copy the approriate values into the local values buffer
    switch ( value_type )
    {
        case CG_CONSTANT:
#if 0 // do nothing for constants right now until the meaning is clarified
            // return contents of constant field.
            {
                // if there are constants, they will only be in the platformParam
                CgBinaryParameter* binparam = ( CgBinaryParameter* )( param->platformParam );
                if ( binparam )
                {
                    int floatcount = _jsCountFloatsInCgType( param->type );
                    JS_ASSERT( floatcount <= 16 );
                    CgBinaryProgram* binprog = ( CgBinaryProgram* )param->parentProgram->platformProgramBinary;
                    if ( param->rows > 0 ) // first handle matrices
                    {
                        // first check to see if the matrix has leaves with constants by checking the next param in the array
                        CgBinaryParameter* childBinparam = binparam + 1;
                        if ( childBinparam->embeddedConst )
                        {
                            // the constants must be copied out for each row, from each successive child
                            for ( int r = 0; r < param->rows; r++ )
                            {
                                // each row has its own leaf parameter that contains the constant values
                                // we copy the values from the constant values of the leaf parameter into the local buffer
                                CgBinaryEmbeddedConstant* constinfo = ( CgBinaryEmbeddedConstant* )(( const char * )binprog + childBinparam->embeddedConst );
                                // if we have an embeddedConst field, it must have at least one occurance
                                JS_ASSERT( constinfo->ucodeCount > 0 );
                                // get the values out of the first embedded offset
                                float* src = ( float* )(( const char * )binprog + constinfo->ucodeOffset[0] );
                                // copy the values into the local buffer, just the right number of values for the column
                                for ( int floatIndex = 0; floatIndex < param->cols; ++floatIndex )
                                {
                                    values[floatIndex + r * param->cols] = src[floatIndex];
                                }
                            }
                            *nvalues = floatcount;
                        }
                    }
                    else // then handle flat types
                    {
                        if ( binparam->embeddedConst )
                        {
                            // we copy the values from the constant values of the parameter (a leaf)
                            CgBinaryEmbeddedConstant* constinfo = ( CgBinaryEmbeddedConstant* )(( const char * )binprog + binparam->embeddedConst );
                            // if we have an embeddedConst field, it must have at least one occurance
                            JS_ASSERT( constinfo->ucodeCount > 0 );
                            // get the values out of the first embedded offset
                            float* src = ( float* )(( const char * )binprog + constinfo->ucodeOffset[0] );
                            // see how many floats we need to set and copy them into the local buffer
                            for ( int floatIndex = 0; floatIndex < floatcount; ++floatIndex )
                            {
                                values[floatIndex] = src[floatIndex];
                            }
                            *nvalues = floatcount;
                        }
                    }
                }
            }
#endif
            break;

        case CG_DEFAULT:
            // return contents of default value field.
        {
            if ( rtParameter->parameterEntry->flags & CGP_RTCREATED )
            {
                // runtime created parameters do not have default values
                break;
            }
            // if there are defaults, they will only be in the platformParam
            //do a slow linear search in the constants we have
            int count = program->defaultValuesIndexCount;
            for ( int i = 0;i < count;i++ )
            {
                int index = ( int )program->defaultValuesIndices[i].entryIndex;
                CgRuntimeParameter *tmp = program->runtimeParameters + index;
                if ( tmp == rtParameter )
                {
                    //found
                    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
                    if ( parameterEntry->flags & CGP_ARRAY )
                    {
                        JS_ASSERT( 0 && "this function cannot be called on arrays" );
                        return NULL;
                    }
                    const float *itemDefaultValues = program->defaultValues + program->defaultValuesIndices[i].defaultValueIndex;
                    CGtype parameterResourceType = _jsGetParameterCGtype( program, parameterEntry );
                    unsigned int cols = _jsGetTypeColCount( parameterResourceType );
                    unsigned int rows = _jsGetTypeRowCount( parameterResourceType );
                    if ( rows == 0 )
                        rows = 1; //flat types versus matrices

                    // the defaults must be copied out for each row, from each successive child
                    for ( GLuint r = 0; r < rows; r++ )
                    {
                        const float* src = itemDefaultValues + 4 * r;//right now default values always come by 4
                        // copy the values into the local buffer, just the right number of values for the column
                        for ( GLuint floatIndex = 0; floatIndex < cols; ++floatIndex )
                        {
                            values[floatIndex + r * cols] = src[floatIndex];
                        }
                    }
                    *nvalues = rows * cols;
                    break; //loop to find the defaults of the parameter
                }
            }
            break;
        }
        case CG_CURRENT:
            // return contents of current value field.
            // **** this is documented in cgSetParameter docs, but not the cgGetParameterValues docs.
        {
            if ( rtParameter->pushBufferPointer )
            {
                const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
                if ( parameterEntry->flags & CGP_ARRAY )
                {
                    JS_ASSERT( 0 && "this function cannot be called on arrays" );
                    return NULL;
                }

                CGtype parameterResourceType = _jsGetParameterCGtype( program, parameterEntry );

                unsigned int cols = _jsGetTypeColCount( parameterResourceType );
                unsigned int rows = _jsGetTypeRowCount( parameterResourceType );
                if ( rows == 0 )
                    rows = 1; //flat types versus matrices

                int floatcount = cols * rows;
                // take the padding in the matrices into account
                int	colCount = 4;
                int paddingSize = 0;
                if ( cols )
                {
                    colCount = cols;
                    paddingSize = 4 - cols;
                }
                JS_ASSERT( floatcount <= 16 );

                bool isInArray = false;
                unsigned int testParam = ((( unsigned int )paramIn ) & CG_PARAMETERMASK );
                // array parameter handle = ( base array handle + 1 ) + ((parameter index)<< 22)
                if (( testParam != ( unsigned int )paramIn ) || // masking it like this will chop the index off the array
                        (( rtParameter > rtParameter->program->runtimeParameters ) && ( rtParameter - 1 )->parameterEntry->flags & CGP_ARRAY ) ) // this handles the case of the first array element at index 0 (and hopefully compact arrays i think)
                {
                    isInArray = true;
                }

                // if this parameter is connected as the child to a parent parameter,
                // it is possible that the values will not have been pulled into the pushbuffer yet
                // if the program that owns this parameter has not been bound since the parent was set.
                // pull the values, just to be sure, even though this will pull values for
                // every parameter in this program.  it's a slow call anyway.
                _pullConnectedParameterValues( rtParameter->program );

                float *source;

                if ( isInArray )
                {
                    source = ( float* )( *(( unsigned int ** )rtParameter->pushBufferPointer + CG_GETINDEX( paramIn ) ) );
                }
                else
                {
                    source = ( float* )rtParameter->pushBufferPointer;
                }

                for ( int i = 0; i < floatcount; ++i )
                {
                    values[i] = source[i + ( i / colCount ) * paddingSize];
                }

                *nvalues = floatcount;
            }
            break;
        }
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return NULL;
            break;
    }

    if ( *nvalues > 0 )
    {
        double *parameterValues = rtParameter->program->parentContext->currentParameterValue;
        for ( int i = 0; i < *nvalues; i++ )
        {
            parameterValues[i] = values[i];
        }
        return parameterValues;
    }
    return NULL;
}

/**
@short Get a program parameter&rsquo;s ordinal number

cgGetParameterOrdinalNumber() returns an integer that represents the
position of a parameter when it was declared within the Cg program.
For example the following Cg program:
@code
float4 globalVector;
MYOUT main(float2 position : POSITION,
           float4 color    : COLOR,
           uniform float4x4 matrix,
           uniform float4 vectors[2],
           float2 texCoord : TEXCOORD0)
{
     // etc ...
}
@endcode
Would generate the following parameters and associated ordinal numbers:
@code
          position     -> 0
          color        -> 1
          matrix       -> 2
          vectors      -> 3
          vectors[0]   -> 3
          vectors[1]   -> 3
          texCoord     -> 4
          globalVector ->-1
@endcode

@param param   Specifies the program parameter.

@return Returns the ordinal number associated with a parameter.  The parameter
must not be a constant.  If it is a constant (cgGetParameterVariability()
returns <CODE>CG_CONSTANT</CODE>) then 0 is returned and no error is generated.
@par
Returns -1 for parameters that are in the global namespace.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa cgGetParameterVariability
*/
CG_API int cgGetParameterOrdinalNumber( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterOrdinalNumber );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterOrdinalNumber, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return CG_FALSE;
    }

    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return CG_FALSE;
    }

    // not implemented yet on the new runtime
    // AND the spec on what this function does has actually been changed
    // for globals and constants according to more recent nv documentation.
    // yuck.
    return -123;

    // if param of a CG_CONSTANT, return 0. We do not currently expose
    // constants, and I am not sure why we would raise any error
    // anyways.

}

/**
@short Determine if a parameter is global

cgIsParameterGlobal() returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> is a global parameter and
<CODE>CG_FALSE</CODE> otherwise.

@param       param   Specifies the parameter handle to check.

@return Returns <CODE>CG_TRUE</CODE> if <I><c>param</c></I> is global.
@par
Returns <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is
invalid.</TD>
</TR>
</TABLE>

*/
CG_API CGbool cgIsParameterGlobal( CGparameter param )
{
    JS_APIENTRY_TIMER( cgIsParameterGlobal );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsParameterGlobal, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return CG_FALSE;
    else
    {
        return (( rtParameter->parameterEntry->flags & CGPF_GLOBAL ) != 0 );
    }
}

/**
@short Get an array member parameter&rsquo;s index

cgGetParameterIndex() returns an integer that represents the index of an array parameter.

@param param   Specifies the parameter.

@return Returns the index associated with an array member parameter.
If the parameter is not in an array, -1 will be returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is
invalid.</TD>
</TR>
</TABLE>

@sa       cgGetArrayParameter
*/
CG_API int cgGetParameterIndex( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGetParameterIndex );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetParameterIndex, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return -1;

    // currently we don't support the runtime creation of array parameters so runtime created params never have an array index
    if ( rtParameter->parameterEntry->flags & CGP_RTCREATED )
        return -1;

    int arrayIndex = -1;
    CgRuntimeParameter *parent = _jsGetArrayItemParent( param, &arrayIndex );
    if ( parent )
        return arrayIndex;
    else
        return -1;
}

/**
@short Set a parameter&rsquo;s variability

cgSetParameterVariability() allows the application to change the variability of a parameter.

Currently parameters may not be changed to or from <CODE>CG_VARYING</CODE> variability.
However parameters of <CODE>CG_UNIFORM</CODE> and <CODE>CG_LITERAL</CODE> variability may be changed.

Valid values for <I><c>vary</c></I> include:

<TABLE rules=all>
<TR>
<TD><CODE>CG_UNIFORM</CODE></TD>
<TD>
A uniform parameter is one whose value does not change with each
invocation of a program, but whose value can change between groups
of program invocations.
</TD>
</TR>
<TR>
<TD><CODE>CG_LITERAL</CODE></TD>
<TD>
A literal parameter is folded out at compile time.  Making a uniform
parameter literal will often make a program more efficient at
the expense of requiring a compile every time the value is set.
</TD>
</TR>
<TR>
<TD><CODE>CG_DEFAULT</CODE></TD>
<TD>
By default, the variability of a parameter will be overridden by
the source parameter connected to it unless it is changed with
cgSetParameterVariability().  If it is set to <CODE>CG_DEFAULT</CODE> it will
restore the default state of assuming the source parameters variability.
</TD>
</TR>
</TABLE>

@param       param   Specifies the parameter.

@param       vary    The variability the <I><c>param</c></I> will be set to.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD>The handle <I><c>param</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>vary</c></I> is not a valid enumerant.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_VARIABILITY_ERROR</CODE></TD>
<TD>The parameter
could not be changed to the variability indicated by <I><c>vary</c></I>.</TD>
</TR>
</TABLE>

@sa       cgGetParameterVariability
*/
CG_API void cgSetParameterVariability( CGparameter param, CGenum vary )
{
    JS_APIENTRY_TIMER( cgSetParameterVariability );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgSetParameterVariability, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             vary, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    // check vary enum for validity.
    switch ( vary )
    {
        case CG_UNIFORM:
        case CG_LITERAL:
        case CG_DEFAULT:
            break;
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return;
    }

    // check that requested variability can be applied to this param
    CGenum var = cgGetParameterVariability( param );
    if ( CG_VARYING == var )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_VARIABILITY_ERROR );
        return;
    }
    else
    {
        switch ( vary )
        {
            case CG_UNIFORM:
            case CG_LITERAL:
                // **** record the change of variability, forcing a recompile.
                // **** but we're pre-compiled so it does nothing to us.
                break;
            case CG_DEFAULT:
                // **** if the param is connected, we can change the target
                // **** param back to its default variability.
                // **** but we got no connected params yet. So give up.
                break;
            default:
                _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
                break;
        }
    }
    return;
}

/**
 * @}
 */
// **** NOTE: undocumented in online documentation!
CG_API void cgSetParameterSemantic( CGparameter param, const char* semantic )
{
    JS_APIENTRY_TIMER( cgSetParameterSemantic );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgSetParameterSemantic, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             semantic, strlen( semantic ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    // check parameter handle
    if ( JS_UNLIKELY( !CG_IS_PARAMETER( param ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
        return;
    }

    CgRuntimeParameter* rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );

    unsigned short type = rtParameter->parameterEntry->flags & CGP_TYPE_MASK;
    if ( type == CGP_STRUCTURE || CGP_STRUCTURE == CGP_ARRAY )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // first see if the parameter already has this semantic set from compile time, in which case, just return, it is already set
    size_t entryIndex = ( rtParameter->parameterEntry - rtParameter->program->parametersEntries ) / sizeof( CgParameterEntry );
    //look for the parameter semantic in the semantic table for semantics set in the compiled source
    int count = rtParameter->program->semanticCount;
    int i;
    for ( i = 0;i < count;i++ )
    {
        const CgParameterSemantic *semanticEntry = rtParameter->program->semanticIndices + i;
        if ( semanticEntry->entryIndex == ( unsigned short )entryIndex )
        {
            //found the semantic for this parameter
            if ( strcmp( semantic, rtParameter->program->stringTable + semanticEntry->semanticOffset ) == 0 )
            {
                // if it already has the value we want, just return
                return;
            }
            else
            {
                // if it has a different value, break out of this loop and give it a new one
                break;
            }
        }
    }

    // this table holds semantics that were created at runtime for either program or context or effect scope parameters
    // must check this table to see if the param already has one set, in which case the semantic should just be updated
    STL_NAMESPACE vector<CgRuntimeSemantic>::iterator semanticIter = rtParameter->program->parameterSemantics.begin();
    while ( semanticIter != rtParameter->program->parameterSemantics.end() )
    {
        if ( semanticIter->param == rtParameter )
        {
            // we found this parameter already has a runtime set semantic, reassign it.
            // semanticIter->semantic is a std::string so this assignment properly copies the string
            //semanticIter->semantic = semantic;
            semanticIter->semantic.clear();
            semanticIter->semantic.insert( semanticIter->semantic.end(), semantic, semantic + strlen( semantic ) + 1 );
            return;
        }
        semanticIter++;
    }

    // finally, if this parameter has no semantic yet, create an entry in the semantics table to store this semantic
    CgRuntimeSemantic newSemanticEntry;
    newSemanticEntry.param = rtParameter;
    //newSemanticEntry.semantic = semantic;
    newSemanticEntry.semantic.clear();
    newSemanticEntry.semantic.insert( newSemanticEntry.semantic.end(), semantic, semantic + strlen( semantic ) + 1 );
    rtParameter->program->parameterSemantics.push_back( newSemanticEntry );
}

