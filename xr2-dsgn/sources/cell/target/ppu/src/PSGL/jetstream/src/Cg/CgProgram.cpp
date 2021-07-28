/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: Implementation of CGprogram class for Jetstream Cg Runtime

#define CG_LIB
#include <Cg/cg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>
//TODO: readelf and cgbdefs: remove later, used for tmp first pass of elf format
#include <PSGL/readelf.h>
#include "../../../cgc/cgbio/cgbtypes.h"
#include "../../tools/src/cgnv2elf/cgnv2rt.h"
#include "../../tools/src/cgnv2elf/cgnv2elfversion.h"

#include <Cg/CgProgramGroup.h>

#include <Cg/cgBinary.h>
#include <Cg/CgCommon.h>

#include <PSGL/readelf.h>


PSGL_EXPORT CgprogramHookFunction _cgProgramCreateHook = NULL;
PSGL_EXPORT CgprogramHookFunction _cgProgramDestroyHook = NULL;
PSGL_EXPORT CgprogramCopyHookFunction _cgProgramCopyHook = NULL;

PSGL_EXPORT cgRTCgcCompileHookFunction _cgRTCgcCompileProgramHook = NULL;
PSGL_EXPORT cgRTCgcFreeHookFunction _cgRTCgcFreeCompiledProgramHook;

// structures -------------------------------------------


// externs ----------------------------------------------

#ifdef JS_DEBUG
extern void _jsCgParameterDump( CGparameter param, unsigned int indent );
#endif


// helper functions -------------------------------------

#ifdef JS_DEBUG

void _jsCgProgramDump( CGprogram prog, unsigned int indent )
{
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
    _CGprogram* ptr = _cgGetProgPtr( prog );
    _jsCgPrintSpaces( indent );    printf( "   shader parameterCount = %d\n", ptr->rtParametersCount );

    // loop over all parameters and dump them
    for ( int i = 0; i < ptr->rtParametersCount; ++i )
    {
        CGparameter q = ptr->runtimeParameters[i].id;
        _jsCgPrintSpaces( indent );    printf( "   parameterId = %d\n", ( int )q );
        _jsCgParameterDump( q, indent + 3 );
    }

    _jsCgPrintSpaces( indent );    printf( "   runtime created parameterCount = %d\n", ptr->runtimeCreatedParameters.size() );

    STL_NAMESPACE vector<CgRuntimeParameter*>::iterator rtCreatedParamIter = ptr->runtimeCreatedParameters.begin();
    while ( rtCreatedParamIter != ptr->runtimeCreatedParameters.end() )
    {
        CGparameter q = ( *rtCreatedParamIter )->id;
        _jsCgPrintSpaces( indent );    printf( "   parameterId = %d\n", ( int )q );
        _jsCgParameterDump( q, indent + 3 );

        rtCreatedParamIter++;
    }

    return;
}

#endif


// Program list functions
// The context can contain many programs, but each one is a variable sized
// chunk of data produced by the CGC compiler and loaded at runtime. We cannot
// include intrusive "next" pointers in this data block so instead we'll use
// a non-intrusive list. The list is walked using cgGetFirstProgram() and
// cgGetNextProgram()

void _jsCgProgramZero( _CGprogram* p )
{
    // zero all pointers in the node and enclosed binary program
    // this makes sure cgIsProgram calls on invalid pointers always fail
    memset( p, 0, sizeof( _CGprogram ) );
    return;
}

void _jsCgProgramPushFront( _CGcontext* ctx, _CGprogram* prog )
{
    // push the program to the context.
    // updates the linked list and program count

    // link the node into the head of the list
    prog->next = ctx->programList;
    ctx->programList = prog;

    // update the parent context
    prog->parentContext = ctx;

    // update the program count
    ctx->programCount++;
}

_CGprogram* _jsCgProgramFindPrev( _CGcontext* ctx, _CGprogram* prog )
{
    // get a pointer the the list element *before* prog in the list
    // this allows us to unlink it safely.

    _CGprogram* ptr = ctx->programList;

    while ( NULL != ptr && prog != ptr->next )
    {
        ptr = ptr->next;
    }

    return ptr;
}

inline void _jsCgProgramEraseAndFree( _CGprogram* prog )
{
    _jsCgProgramErase( prog );
    jsFree( prog );
}

void _jsCgProgramErase( _CGprogram* prog )
{
    // remove the program from the linked list and deallocate it's storage.

    if ( _cgProgramDestroyHook ) _cgProgramDestroyHook( prog );

    switch ( prog->header.profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            _jsPlatformProgramErase( prog );
            break;
        default:
            // default program
            break;
    }

    // check to see if each effect parameter is a parent in a connection and remove that reference before deletion
    //
    // for every effect param, check every program in its context and every connection in each program,
    // then remove any where the effect param is a parent
    STL_NAMESPACE vector<CgRuntimeParameter*>::iterator effectParamIter = prog->runtimeCreatedParameters.begin();
    while ( effectParamIter != prog->runtimeCreatedParameters.end() )
    {
        struct _CGprogram* programIter = prog->parentContext->programList;
        while ( programIter != NULL )
        {
            // search the program's connection table to find if this is a parent param to anybody
            STL_NAMESPACE vector<CgParameterConnection>::iterator paramConnectionTableIter = programIter->connectionTable.begin();
            while ( paramConnectionTableIter != programIter->connectionTable.end() )
            {
                if ( paramConnectionTableIter->parent == *effectParamIter )
                {
                    // Use iterator returned by erase() function as "nextParam" iterator
					paramConnectionTableIter = programIter->connectionTable.erase( paramConnectionTableIter );
                }
                else
                {
                    paramConnectionTableIter++;
                }
            }
            programIter = programIter->next;
        }
        //_jsCgDestroyContextParam(*effectParamIter);
        effectParamIter++;
    }

    while ( prog->runtimeCreatedParameters.size() > 0 )
    {
        // this routine removes the parameter from the array
        _jsCgDestroyContextParam( prog->runtimeCreatedParameters[0] );
    }

    // after the above loop and call to destroy the param, these parameters should have been destroyed and this table should be empty.
    JS_ASSERT( prog->runtimeCreatedParameters.size() == 0 );

    // i thought we could assert this but this table may not be empty if there was a connection made to a context parameter and it was never disconnected
    //JS_ASSERT(prog->connectionTable.size() == 0);

    // first i thought this couldn't be asserted.  now i think it can.  fx parameters should all be cleaned up at this point by destroying each of the runtime created list
    JS_ASSERT( prog->fxParameterExtensions.size() == 0 );

    // i'd like to assert that additional semantics should have been cleaned up already, but i think if one is set on a program parameter that is not runtime created, it might still be hanging around.  Do we need to free that???
    //JS_ASSERT(prog->parameterSemantics.size() == 0);

    // free allocated memory in these stl containers. 
    // Current clear() implementation deallocates the memory.
    // Is it better to explicitly call a "destructor" of this prog before freeing its memory?
	prog->runtimeCreatedParameters.clear();
	prog->connectionTable.clear();
	prog->fxParameterExtensions.clear();
	prog->parameterSemantics.clear();

    // return program and node to free store
    if ( prog->id ) _jsEraseName( &_CurrentContext->cgProgramNameSpace, ( jsName )prog->id );

    //free ELF data obtained from the runtime converter
    if ( prog->runtimeElf )
        jsFree( prog->runtimeElf );

    // zero out all pointers
    _jsCgProgramZero( prog );
}

void _jsCgProgramEraseAfter( _CGprogram* prog )
{
    // delete the list entry after the current pointer.
    // (use in conjunction with find_prev for full effect!)

    _CGprogram* next = prog->next;
    if ( next )
    {
        prog->next = next->next;
        _jsCgProgramEraseAndFree( next );
    }
}

void _jsCgProgramDestroyAll( _CGcontext* ctx )
{
    // destroy all programs in this context
    while ( ctx->programList )
    {
        _CGprogram * p = ctx->programList;
        ctx->programList = p->next;
        // erase the program
        _jsCgProgramEraseAndFree( p );
    }
}


static inline unsigned int getMatrixRowCount( CGtype type )
{
    unsigned int rc = 0;
    switch ( type )
    {
        case CG_FLOAT1x1:
        case CG_FLOAT1x2:
        case CG_FLOAT1x3:
        case CG_FLOAT1x4:
            rc = 1;
            break;
        case CG_FLOAT2x1:
        case CG_FLOAT2x2:
        case CG_FLOAT2x3:
        case CG_FLOAT2x4:
            rc = 2;
            break;
        case CG_FLOAT3x1:
        case CG_FLOAT3x2:
        case CG_FLOAT3x3:
        case CG_FLOAT3x4:
            rc = 3;
            break;
        case CG_FLOAT4x1:
        case CG_FLOAT4x2:
        case CG_FLOAT4x3:
        case CG_FLOAT4x4:
            rc = 4;
            break;
        case CG_HALF1x1:
        case CG_HALF1x2:
        case CG_HALF1x3:
        case CG_HALF1x4:
        case CG_INT1x1:
        case CG_INT1x2:
        case CG_INT1x3:
        case CG_INT1x4:
        case CG_BOOL1x1:
        case CG_BOOL1x2:
        case CG_BOOL1x3:
        case CG_BOOL1x4:
        case CG_FIXED1x1:
        case CG_FIXED1x2:
        case CG_FIXED1x3:
        case CG_FIXED1x4:
            rc = 1;
            break;
        case CG_HALF2x1:
        case CG_HALF2x2:
        case CG_HALF2x3:
        case CG_HALF2x4:
        case CG_INT2x1:
        case CG_INT2x2:
        case CG_INT2x3:
        case CG_INT2x4:
        case CG_BOOL2x1:
        case CG_BOOL2x2:
        case CG_BOOL2x3:
        case CG_BOOL2x4:
        case CG_FIXED2x1:
        case CG_FIXED2x2:
        case CG_FIXED2x3:
        case CG_FIXED2x4:
            rc = 2;
            break;
        case CG_HALF3x1:
        case CG_HALF3x2:
        case CG_HALF3x3:
        case CG_HALF3x4:
        case CG_INT3x1:
        case CG_INT3x2:
        case CG_INT3x3:
        case CG_INT3x4:
        case CG_BOOL3x1:
        case CG_BOOL3x2:
        case CG_BOOL3x3:
        case CG_BOOL3x4:
        case CG_FIXED3x1:
        case CG_FIXED3x2:
        case CG_FIXED3x3:
        case CG_FIXED3x4:
            rc = 3;
            break;
        case CG_HALF4x1:
        case CG_HALF4x2:
        case CG_HALF4x3:
        case CG_HALF4x4:
        case CG_INT4x1:
        case CG_INT4x2:
        case CG_INT4x3:
        case CG_INT4x4:
        case CG_BOOL4x1:
        case CG_BOOL4x2:
        case CG_BOOL4x3:
        case CG_BOOL4x4:
        case CG_FIXED4x1:
        case CG_FIXED4x2:
        case CG_FIXED4x3:
        case CG_FIXED4x4:
            rc = 4;
            break;
        default:
            break;
    }
    return rc;
}

CGtype getMatrixRowType( CGtype type )
{
    CGtype rt = ( CGtype )0;
    switch ( type )
    {
        case CG_FLOAT1x1:
        case CG_FLOAT2x1:
        case CG_FLOAT3x1:
        case CG_FLOAT4x1:
            rt = CG_FLOAT1;
            break;
        case CG_FLOAT1x2:
        case CG_FLOAT2x2:
        case CG_FLOAT3x2:
        case CG_FLOAT4x2:
            rt = CG_FLOAT2;
            break;
        case CG_FLOAT1x3:
        case CG_FLOAT2x3:
        case CG_FLOAT3x3:
        case CG_FLOAT4x3:
            rt = CG_FLOAT3;
            break;
        case CG_FLOAT1x4:
        case CG_FLOAT2x4:
        case CG_FLOAT3x4:
        case CG_FLOAT4x4:
            rt = CG_FLOAT4;
            break;
        case CG_HALF1x1:
        case CG_HALF2x1:
        case CG_HALF3x1:
        case CG_HALF4x1:
            rt = CG_HALF1;
            break;
        case CG_HALF1x2:
        case CG_HALF2x2:
        case CG_HALF3x2:
        case CG_HALF4x2:
            rt = CG_HALF2;
            break;
        case CG_HALF1x3:
        case CG_HALF2x3:
        case CG_HALF3x3:
        case CG_HALF4x3:
            rt = CG_HALF3;
            break;
        case CG_HALF1x4:
        case CG_HALF2x4:
        case CG_HALF3x4:
        case CG_HALF4x4:
            rt = CG_HALF4;
            break;
        case CG_INT1x1:
        case CG_INT2x1:
        case CG_INT3x1:
        case CG_INT4x1:
            rt = CG_INT1;
            break;
        case CG_INT1x2:
        case CG_INT2x2:
        case CG_INT3x2:
        case CG_INT4x2:
            rt = CG_INT2;
            break;
        case CG_INT1x3:
        case CG_INT2x3:
        case CG_INT3x3:
        case CG_INT4x3:
            rt = CG_INT3;
            break;
        case CG_INT1x4:
        case CG_INT2x4:
        case CG_INT3x4:
        case CG_INT4x4:
            rt = CG_INT4;
            break;
        case CG_BOOL1x1:
        case CG_BOOL2x1:
        case CG_BOOL3x1:
        case CG_BOOL4x1:
            rt = CG_BOOL1;
            break;
        case CG_BOOL1x2:
        case CG_BOOL2x2:
        case CG_BOOL3x2:
        case CG_BOOL4x2:
            rt = CG_BOOL2;
            break;
        case CG_BOOL1x3:
        case CG_BOOL2x3:
        case CG_BOOL3x3:
        case CG_BOOL4x3:
            rt = CG_BOOL3;
            break;
        case CG_BOOL1x4:
        case CG_BOOL2x4:
        case CG_BOOL3x4:
        case CG_BOOL4x4:
            rt = CG_BOOL4;
            break;
        case CG_FIXED1x1:
        case CG_FIXED2x1:
        case CG_FIXED3x1:
        case CG_FIXED4x1:
            rt = CG_FIXED1;
            break;
        case CG_FIXED1x2:
        case CG_FIXED2x2:
        case CG_FIXED3x2:
        case CG_FIXED4x2:
            rt = CG_FIXED2;
            break;
        case CG_FIXED1x3:
        case CG_FIXED2x3:
        case CG_FIXED3x3:
        case CG_FIXED4x3:
            rt = CG_FIXED3;
            break;
        case CG_FIXED1x4:
        case CG_FIXED2x4:
        case CG_FIXED3x4:
        case CG_FIXED4x4:
            rt = CG_FIXED4;
            break;
        default:
            break;
    }
    return rt;
}


// forward declaration for recursion
void AccumulateSizeForParams( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                              unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                              unsigned int prefixLength, int elementResourceIndex,
                              size_t* nvParamSize, size_t* nvParamStringsSize, size_t* nvParamOffsetsSize,
                              CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                              CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount, size_t* nvParamDefaultsSize,
                              ELF_section_t* consttab );

void AccumulateSizeForParamResource( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                     unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                     unsigned int prefixLength, int elementResourceIndex,
                                     size_t* nvParamSize, size_t* nvParamStringsSize, size_t* nvParamOffsetsSize,
                                     CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                     CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount, size_t* nvParamDefaultsSize,
                                     ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterResource* paramResource = ( CgParameterResource* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    unsigned int rowCount = getMatrixRowCount(( CGtype )paramResource->type );

    // param
    *nvParamSize += sizeof( CgBinaryParameter );
    // and for the row params of a matrix
    *nvParamSize += rowCount * sizeof( CgBinaryParameter );

    // name
    *nvParamStringsSize += ( prefixLength + strlen( strtab->data + paramEntry->nameOffset ) + 1 );
    // and for each row of a matrix which assumes just an extra 3 characters for [i]
    *nvParamStringsSize += ( rowCount * ( prefixLength + strlen( strtab->data + paramEntry->nameOffset ) + 3 + 1 ) );


    // semantic

    // totally inefficient, but easiest implementation for now is to do the On^2 search
    // in the semantic and default tables for the data associated with this parameter
    unsigned int entryIndex = paramEntry - paramEntries;

    // semantic
    char* semantic = NULL;
    for ( unsigned int sem = 0; sem < semanticIndexCount; sem++ )
    {
        CgParameterSemantic* semanticIndex = semanticIndexTable + sem;
        if ( semanticIndex->entryIndex == entryIndex )
        {
            // found the semantic for this parameter
            semantic = strtab->data + semanticIndex->semanticOffset;
            break;
        }
    }
    if ( semantic )
    {
        // increment the names size buffer
        *nvParamStringsSize += strlen( semantic ) + 1;
    }

    // default
    float* defaults = NULL;
    for ( unsigned int def = 0; def < defaultIndexCount; def++ )
    {
        CgParameterDefaultValue* defaultIndex = defaultIndexTable + def;
        if ( defaultIndex->entryIndex == entryIndex )
        {
            // found the default for this parameter
            defaults = ( float* )consttab->data + defaultIndex->defaultValueIndex;
        }
    }
    if ( defaults )
    {
        // defaults are always padded to 4 floats per param
        unsigned int defaultsCount = ( rowCount ? rowCount : 1 ) * 4;
        *nvParamDefaultsSize += sizeof( float ) * defaultsCount;
    }

    // offsets
    // RESOURCE DECODING ALGORTITHM BEGIN
    // fragment programs
    // do we want this referenced param test???
    if ( nvParamOffsetsSize != NULL && paramEntry->flags & CGPF_REFERENCED )
    {
        // non varying params
        if ( paramEntry->flags & CGPV_CONSTANT || paramEntry->flags & CGPV_UNIFORM )
        {
            // non sampler types
            if ( !isSampler(( CGtype )paramResource->type ) )
            {
                unsigned short* resPtr = paramResourceTable + paramResource->resource;

                if ( elementResourceIndex >= 0 )
                {
                    //advance resPtr to the arrayIndex
                    int skipCount = elementResourceIndex;
                    while ( skipCount )
                    {
                        resPtr++;
                        int embeddedCount = *( resPtr++ );
                        resPtr += embeddedCount;
                        // if an array of matrices, must skip row parameter resources too
                        for ( int row = 1; row <= ( int )rowCount; row++ )
                        {
                            resPtr++;
                            int embeddedCount = *( resPtr++ );
                            resPtr += embeddedCount;
                        }
                        skipCount--;
                    }
                    //resPtr points to the resource of the arrayIndex
                }

                if ( !isMatrix(( CGtype )( paramResource->type ) ) )
                {
                    // advance past register to second field that contains embedded count
                    resPtr++;
                    unsigned int embeddedConstantCount = *( resPtr++ );
                    *nvParamOffsetsSize += sizeof( CgBinaryEmbeddedConstant ) + sizeof( unsigned int ) * ( embeddedConstantCount - 1 );
                }
                else
                {
                    for ( int row = 1; row <= ( int )rowCount; row++ )
                    {
                        // advance past register to second field that contains embedded count
                        resPtr++;
                        unsigned int embeddedConstantCount = *( resPtr++ );
                        *nvParamOffsetsSize += sizeof( CgBinaryEmbeddedConstant ) + sizeof( unsigned int ) * ( embeddedConstantCount - 1 );
                    }
                }

            }
        }

    }
    // RESOURCE DECODING ALGORTITHM END


    // entry cursor
    *nextEntry = paramEntry + 1;
}

void AccumulateSizeForParamArray( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                  unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                  unsigned int prefixLength, int elementResourceIndex,
                                  size_t* nvParamSize, size_t* nvParamStringsSize, size_t* nvParamOffsetsSize,
                                  CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                  CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount, size_t* nvParamDefaultsSize,
                                  ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterArray* paramArray = ( CgParameterArray* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    // this parameter's entry says it's an array
    // this parameter's entry says its packing in the flags
    // the next entry specifies its type...
    // ... either a list of every element in the array
    // ... or a single element in the array to represent all because they are the same
    // ... or a single element to specify the layout of the parameter, but not every element has a resource

    // this would give the number of intrinsics OR structs, but if base type is struct
    // to get the number of parameters, must recurse over struct fields
    unsigned int numElements = 1;
    for ( unsigned int dim = 0; dim < paramArray->dimensionCount; dim++ )
    {
        numElements *= paramArray->dimensions[dim];
    };

    unsigned int prefixLengthIn = prefixLength;

    // if simple array of intrinsics
    JS_ASSERT( paramEntriesCount > ( unsigned int )(( paramEntry + 1 ) - paramEntries ) );
    // next entry will always be either an intrinsic or a struct

    if ( !( paramEntry->flags & CGP_UNROLLED ) ) // one bit is not enough cause there are 3 representations!
    {
        // all elements are the same so have one entry to represent all of them
        CgParameterEntry* elementEntry = paramEntry + 1;

        // entries are same type, and packed, so just repeat same param
        for ( unsigned int element = 0; element < numElements; element++ )
        {
            JS_ASSERT(( elementEntry->flags & CGP_TYPE_MASK ) != CGP_ARRAY );

            // name length for prefixIn +  rest of array name for array + 16 bytes generously for [index]
            prefixLength = prefixLengthIn + strlen( strtab->data + paramEntry->nameOffset ) + 16;

            // right now i think this is always intrinsics for the not unrolled case, current limitation
            AccumulateSizeForParamResource( elementEntry, paramEntries, paramEntriesCount,
                                            paramResourceTable, strtab, nextEntry,
                                            prefixLength, element,
                                            nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                            semanticIndexTable, semanticIndexCount,
                                            defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                            consttab );

            *nextEntry = *nextEntry - 1; // undo the increment on the next entry for this special case of shared entries
        }
        *nextEntry = *nextEntry + 1; // redo the increment upon exiting the loop, could be smarter, whatever
    }
    else if ( paramEntry->flags & CGP_UNROLLED )
    {
        // here everything is unrolled

        CgParameterEntry* elementEntry = paramEntry + 1;

        for ( unsigned int element = 0; element < numElements; element++ )
        {
            JS_ASSERT(( elementEntry->flags & CGP_TYPE_MASK ) != CGP_ARRAY );

            // name length for prefixIn +  rest of array name for array + 16 bytes generously for [index]
            prefixLength = prefixLengthIn + strlen( strtab->data + paramEntry->nameOffset ) + 16;

            AccumulateSizeForParams( elementEntry, paramEntries, paramEntriesCount,
                                     paramResourceTable, strtab, nextEntry,
                                     prefixLength, -1,
                                     nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                     semanticIndexTable, semanticIndexCount,
                                     defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                     consttab );

            elementEntry = *nextEntry;
        }
    }
    else
    {
        // there should be another case here for sparse array representation but
        // we don't seem to have enough bits at the moment to actually generate
        // a test that will get us here.
        // besides, we haven't completely designed it yet.
        JS_ASSERT( 0 && "Unsupported array packing designation." );
    }
}

void AccumulateSizeForParamStruct( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                   unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                   unsigned int prefixLength, int elementResourceIndex,
                                   size_t* nvParamSize, size_t* nvParamStringsSize, size_t* nvParamOffsetsSize,
                                   CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                   CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount, size_t* nvParamDefaultsSize,
                                   ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterStructure* paramStruct = ( CgParameterStructure* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    // add prefix of struct name and .
    // !!! but what about if we are inside an array???  then no struct name
    if ( paramEntry->nameOffset > 0 ) // is this the right test???
    {
        prefixLength += strlen( strtab->data + paramEntry->nameOffset );
    }
    prefixLength += 1; // allow for the dot

    unsigned short memberCount = paramStruct->memberCount;
    CgParameterEntry* memberEntry = paramEntry + 1;

    for ( unsigned int member = 0; member < memberCount; member++ )
    {
        AccumulateSizeForParams( memberEntry, paramEntries, paramEntriesCount,
                                 paramResourceTable, strtab, nextEntry,
                                 prefixLength, -1,
                                 nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                 semanticIndexTable, semanticIndexCount,
                                 defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                 consttab );

        memberEntry = *nextEntry;
    }
}

void AccumulateSizeForParams( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                              unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                              unsigned int prefixLength, int elementResourceIndex,
                              size_t* nvParamSize, size_t* nvParamStringsSize, size_t* nvParamOffsetsSize,
                              CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                              CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount, size_t* nvParamDefaultsSize,
                              ELF_section_t* consttab )
{
    // expand for intrinsics, arrays, and structs
    // flags hold which type of parameter it is (along with other information)
    switch ( paramEntry->flags & CGP_TYPE_MASK )
    {
        case CGP_INTRINSIC:
        {
            //printf("resource --- %p\n", paramEntry);
            AccumulateSizeForParamResource( paramEntry, paramEntries, paramEntriesCount,
                                            paramResourceTable, strtab, nextEntry,
                                            prefixLength, elementResourceIndex,
                                            nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                            semanticIndexTable, semanticIndexCount,
                                            defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                            consttab );
            break;
        }
        case CGP_ARRAY:
        {
            //printf("array --- %p\n", paramEntry);
            AccumulateSizeForParamArray( paramEntry, paramEntries, paramEntriesCount,
                                         paramResourceTable, strtab, nextEntry,
                                         prefixLength, elementResourceIndex,
                                         nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                         semanticIndexTable, semanticIndexCount,
                                         defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                         consttab );
            break;
        }
        case CGP_STRUCTURE:
        {
            //printf("structure --- %p\n", paramEntry);
            AccumulateSizeForParamStruct( paramEntry, paramEntries, paramEntriesCount,
                                          paramResourceTable, strtab, nextEntry,
                                          prefixLength, elementResourceIndex,
                                          nvParamSize, nvParamStringsSize, nvParamOffsetsSize,
                                          semanticIndexTable, semanticIndexCount,
                                          defaultIndexTable, defaultIndexCount, nvParamDefaultsSize,
                                          consttab );
            break;
        }
        default:
        {
            JS_ASSERT( 0 && "unsupported type" );
            break;
        }

    }

}

// forward declaration for recursion
void PopulateDataForParams( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                            unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                            int elementResourceIndex, unsigned int* localParamNo, char* nvBinary,
                            CgBinaryParameter** nvParams, char** nvParamStrings,
                            CgBinaryEmbeddedConstant** nvParamOffsets, float** nvParamDefaults,
                            char* prefix, unsigned int prefixLength,
                            CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                            CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount,
                            ELF_section_t* consttab );

void PopulateDataForParamResource( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                   unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                   int elementResourceIndex, unsigned int* localParamNo, char* nvBinary,
                                   CgBinaryParameter** nvParams, char** nvParamStrings,
                                   CgBinaryEmbeddedConstant** nvParamOffsets, float** nvParamDefaults,
                                   char* prefix, unsigned int prefixLength,
                                   CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                   CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount,
                                   ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterResource* paramResource = ( CgParameterResource* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    unsigned int rowCount = getMatrixRowCount(( CGtype )paramResource->type );

    // type
    ( *nvParams )->type = ( CGtype )( paramResource->type );
    // sub type for row params of matrix
    for ( int row = 1; row <= ( int )rowCount; row++ )
    {
        ( *nvParams + row )->type = getMatrixRowType(( CGtype )paramResource->type );
    }

#if 0
    // forward pointer to index if array
    if ( elementResourceIndex >= 0 )
        paramResource += elementResourceIndex;
#endif

    // res and resindex
    // RESOURCE DECODING ALGORTITHM BEGIN
    // varying parameters
    if (( paramEntry->flags & CGPV_MASK ) == CGPV_VARYING )
    {
        JS_ASSERT( rowCount == 0 );
        ( *nvParams )->res = ( CGresource )paramResource->resource;
        ( *nvParams )->resIndex = -1;
    }
    else if ((( paramEntry->flags & CGPV_MASK ) & CGPV_CONSTANT ) || (( paramEntry->flags & CGPV_MASK ) & CGPV_UNIFORM ) )
    {
        // sampler types
        if ( isSampler(( CGtype )paramResource->type ) )
        {
            JS_ASSERT(( elementResourceIndex == -1 ) && "arrays of samplers currently unsuported" );
            ( *nvParams )->res = ( CGresource )paramResource->resource; //should be CG_TEXUNIT0 .. 15
            ( *nvParams )->resIndex = 0;
        }
        // nonsampler types
        else
        {
            // CG_B requires support too
            for ( int row = 0; row <= ( int )rowCount; row++ )
            {
                if ( paramResource->type == CGP_SCF_BOOL ) //(paramEntry->flags & CGP_SCFBOOL)
                {
                    ( *nvParams + row )->res = CG_B;
                    ( *nvParams )->type = CG_BOOL;
                }
                else
                {
                    ( *nvParams + row )->res = CG_C;
                }
            }

            // vertex programs
            if ( nvParamOffsets == NULL )
            {
                // element of an array
                if ( elementResourceIndex >= 0 )
                {
                    if ( !isMatrix(( CGtype )( paramResource->type ) ) )
                    {
                        unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + elementResourceIndex );
                        ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                    }
                    else if ( paramEntry->flags & CGP_CONTIGUOUS )
                    {
                        unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + elementResourceIndex );
                        ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                        for ( int row = 1; row <= ( int )rowCount; row++ )
                        {
                            unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + elementResourceIndex ) + row - 1;
                            ( *nvParams + row )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                        }
                    }
                    else
                    {
                        unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + elementResourceIndex * rowCount );
                        ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                        for ( int row = 1; row <= ( int )rowCount; row++ )
                        {
                            unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + elementResourceIndex * rowCount + row - 1 );
                            ( *nvParams + row )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                        }
                    }
                }
                // not array
                else
                {
                    if ( !isMatrix(( CGtype )( paramResource->type ) ) )
                    {
                        unsigned short tempResIndex = paramResource->resource;
                        ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                    }
                    else
                    {
                        if ( paramEntry->flags & CGP_CONTIGUOUS )
                        {
                            unsigned short tempResIndex = paramResource->resource;
                            ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                            for ( int row = 1; row <= ( int )rowCount; row++ )
                            {
                                unsigned short tempResIndex = paramResource->resource + row - 1;
                                ( *nvParams + row )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                            }
                        }
                        else
                        {
                            unsigned short tempResIndex = *( paramResourceTable + paramResource->resource );
                            ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                            for ( int row = 1; row <= ( int )rowCount; row++ )
                            {
                                unsigned short tempResIndex = *( paramResourceTable + paramResource->resource + row - 1 );
                                ( *nvParams + row )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                            }
                        }
                    }
                }
            }
            // fragment programs
            else
            {
                unsigned short* resPtr = paramResourceTable + paramResource->resource;

                if ( elementResourceIndex >= 0 )
                {
                    // !!! matrices should be handled here too, i think
                    //advance resPtr to the arrayIndex
                    int skipCount = elementResourceIndex;
                    while ( skipCount )
                    {
                        resPtr++;
                        int embeddedCount = *( resPtr++ );
                        resPtr += embeddedCount;
                        // if an array of matrices, must skip row parameter resources too
                        for ( int row = 1; row <= ( int )rowCount; row++ )
                        {
                            resPtr++;
                            int embeddedCount = *( resPtr++ );
                            resPtr += embeddedCount;
                        }
                        skipCount--;
                    }
                    //resPtr points to the resource of the arrayIndex
                }

                if ( !isMatrix(( CGtype )( paramResource->type ) ) )
                {
                    unsigned short tempResIndex = *( resPtr++ );
                    ( *nvParams )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                    unsigned int embeddedConstantCount = *( resPtr++ );
                    ( *nvParamOffsets )->ucodeCount = embeddedConstantCount;
                    unsigned int ec = 0;
                    for ( ec = 0; ec < embeddedConstantCount; ec++ )
                    {
                        ( *nvParamOffsets )->ucodeOffset[ec] = *( resPtr++ );
                    }
                    // set the field in the nv param
                    ( *nvParams )->embeddedConst = ( char* )( *nvParamOffsets ) - nvBinary;
                    // forward pointer to nvParamOffsets
                    *nvParamOffsets = ( CgBinaryEmbeddedConstant* )( &(( *nvParamOffsets )->ucodeOffset[ec] ) );
                }
                else
                {
                    for ( int row = 1; row <= ( int )rowCount; row++ )
                    {
                        unsigned short tempResIndex = *( resPtr++ );
                        ( *nvParams + row )->resIndex = ( tempResIndex == 0xFFFF ) ? -1 : ( int )tempResIndex;
                        unsigned int embeddedConstantCount = *( resPtr++ );
                        ( *nvParamOffsets )->ucodeCount = embeddedConstantCount;
                        unsigned int ec = 0;
                        for ( ec = 0; ec < embeddedConstantCount; ec++ )
                        {
                            ( *nvParamOffsets )->ucodeOffset[ec] = *( resPtr++ );
                        }
                        // set the field in the nv param
                        ( *nvParams + row )->embeddedConst = ( char* )( *nvParamOffsets ) - nvBinary;
                        // forward pointer to nvParamOffsets
                        *nvParamOffsets = ( CgBinaryEmbeddedConstant* )( &(( *nvParamOffsets )->ucodeOffset[ec] ) );
                    }
                }
            }
        }

    }
    else
    {
        JS_ASSERT( 0 && "unhandled kind of param" );
    }
    // RESOURCE DECODING ALGORTITHM END


    // name
    if ( paramEntry->nameOffset )
    {
        // copy the name and store its offset in the param
        // i think prefix is already handled for this base case
        strcpy( *nvParamStrings, strtab->data + paramEntry->nameOffset );
    }
    // set the name offset in the param, starting from the prefix
    char* namebase = NULL;
    if ( prefix )
    {
        ( *nvParams )->name = prefix - nvBinary;
        namebase = prefix;
    }
    else
    {
        ( *nvParams )->name = *nvParamStrings - nvBinary;
        namebase = *nvParamStrings;
    }


    // increment the names cursor
    *nvParamStrings += strlen( *nvParamStrings ) + 1;

    // and now handle the matrix rows
    for ( int row = 1; row <= ( int )rowCount; row++ )
    {
        // copy the name and store its offset in the param
        sprintf( *nvParamStrings, "%s[%d]", namebase, row - 1 );
        // set the name offset in the param
        ( *nvParams + row )->name = *nvParamStrings - nvBinary;
        // increment the names cursor
        *nvParamStrings += strlen( *nvParamStrings ) + 1;
    }

    // totally inefficient, but easiest implementation for now is to do the On^2 search
    // in the semantic and default tables for the data associated with this parameter
    unsigned int entryIndex = paramEntry - paramEntries;

    // semantic
    char* semantic = NULL;
    for ( unsigned int sem = 0; sem < semanticIndexCount; sem++ )
    {
        CgParameterSemantic* semanticIndex = semanticIndexTable + sem;
        if ( semanticIndex->entryIndex == entryIndex )
        {
            // found the semantic for this parameter
            semantic = strtab->data + semanticIndex->semanticOffset;
            break;
        }
    }
    if ( semantic )
    {
        // copy the name and store its offset in the param
        strcpy( *nvParamStrings, semantic );
        // set the name offset in the param (or params if a matrix)
        for ( int row = 0; row <= ( int )rowCount; row++ )
        {
            ( *nvParams )->semantic = *nvParamStrings - nvBinary;
        }
        // increment the names cursor
        *nvParamStrings += strlen( semantic ) + 1;
    }

    // default
    float* defaults = NULL;
    for ( unsigned int def = 0; def < defaultIndexCount; def++ )
    {
        CgParameterDefaultValue* defaultIndex = defaultIndexTable + def;
        if ( defaultIndex->entryIndex == entryIndex )
        {
            // found the default for this parameter
            defaults = ( float* )consttab->data + defaultIndex->defaultValueIndex;
        }
    }
    if ( defaults )
    {
        // defaults are always padded to 4 floats per param
        unsigned int defaultsCount = ( rowCount ? rowCount : 1 ) * 4;
        // copy the values into the right place in the buffer
        memcpy( *nvParamDefaults, defaults, defaultsCount * sizeof( float ) );
        // set the field in the param to point to them
        if ( rowCount == 0 )
        {
            ( *nvParams )->defaultValue = ( char* )( *nvParamDefaults ) - nvBinary;
        }
        else
        {
            for ( int row = 1; row <= ( int )rowCount; row++ )
            {
                ( *nvParams + row )->defaultValue = ( char* )( *nvParamDefaults + 4 * ( row - 1 ) ) - nvBinary;
            }
        }
#if 0
        printf( "DEFAULTS OUT OF BUFFER:\n" );
        for ( int di = 0; di < defaultsCount; di++ )
        {
            printf( "%f ", defaults[di] );
        }
        printf( "\n" );
#endif

        // forward the buffer pointer
        *nvParamDefaults += defaultsCount;
    }


    // fill other fields, once for the matrix and once for every row
    for ( int row = 0; row <= ( int )rowCount; row++ )
    {

        // var
        unsigned int variability = paramEntry->flags & CGPV_MASK;
        if ( variability == CGPV_VARYING )
            ( *nvParams )->var = CG_VARYING;
        else if ( variability == CGPV_UNIFORM )
            ( *nvParams )->var = CG_UNIFORM;
        else if ( variability == CGPV_CONSTANT )
            ( *nvParams )->var = CG_CONSTANT;
        else if ( variability == CGPV_MIXED )
            ( *nvParams )->var = CG_MIXED;
        else
        {
            ( *nvParams )->var = ( CGenum )0;
            JS_ASSERT( "unrecognized variability enum" );
        }

        //direction
        unsigned int direction = paramEntry->flags & CGPD_MASK;
        if ( direction == CGPD_IN )
            ( *nvParams )->direction = CG_IN;
        else if ( direction == CGPD_OUT )
            ( *nvParams )->direction = CG_OUT;
        else if ( direction == CGPD_INOUT )
            ( *nvParams )->direction = CG_INOUT;
        else
        {
            ( *nvParams )->direction = ( CGenum )0;
            JS_ASSERT( "unrecognized direction enum" );
        }

        // paramno
        if ( paramEntry->flags & CGPF_GLOBAL )
            ( *nvParams )->paramno = -1;
        else if ( paramEntry->flags & CGP_INTERNAL )
            ( *nvParams )->paramno = -2;
        else
        {
            ( *nvParams )->paramno = *localParamNo;
            if ( row == 0 )
            {
                *localParamNo += 1;
            }
        }

        // isReferenced
        ( *nvParams )->isReferenced = (( paramEntry->flags & CGPF_REFERENCED ) != 0 );

        // isShared
        ( *nvParams )->isShared = (( paramEntry->flags & CGPF_SHARED ) != 0 );


        // increment param cursor
        *nvParams += 1;


    }

    // entry cursor
    *nextEntry = paramEntry + 1;

}

void PopulateDataForParamArray( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                int elementResourceIndex, unsigned int* localParamNo, char* nvBinary,
                                CgBinaryParameter** nvParams, char** nvParamStrings,
                                CgBinaryEmbeddedConstant** nvParamOffsets, float** nvParamDefaults,
                                char* prefix, unsigned int prefixLength,
                                CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount,
                                ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterArray* paramArray = ( CgParameterArray* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    // this would give the number of intrinsics OR structs, but if base type is struct
    // to get the number of parameters, must recurse over struct fields
    unsigned int numElements = 1;
    for ( unsigned int dim = 0; dim < paramArray->dimensionCount; dim++ )
    {
        numElements *= paramArray->dimensions[dim];
    };

    // for name handling
    char* prefixIn = prefix;
    unsigned int prefixLengthIn = prefixLength;

    // if simple array of intrinsics
    JS_ASSERT( paramEntriesCount > ( unsigned int )(( paramEntry + 1 ) - paramEntries ) );
    // next entry will always be either an intrinsic or a struct

    if ( !( paramEntry->flags & CGP_UNROLLED ) ) // one bit is not enough cause there are 3 representations!
    {
        // all elements are the same so have one entry to represent all of them
        CgParameterEntry* elementEntry = paramEntry + 1;

        // entries are same type, and packed, so just repeat same param
        for ( unsigned int element = 0; element < numElements; element++ )
        {
            JS_ASSERT(( elementEntry->flags & CGP_TYPE_MASK ) != CGP_ARRAY );

            // where is the array name???  in paramEntry or elementEntry???

            // name
            // copy prefix if it's not the first element whose prefix is already in place
            if ( element > 0 )
            {
                strncpy( *nvParamStrings, prefixIn, prefixLengthIn );
                prefix = *nvParamStrings;
                *nvParamStrings += prefixLengthIn;
            }
            // rest of array name for array and index
            sprintf( *nvParamStrings, "%s[%d]", ( strtab->data + paramEntry->nameOffset ), element );

            if ( prefix == NULL )
            {
                prefix = *nvParamStrings;
            }

            *nvParamStrings += strlen( *nvParamStrings );

            prefixLength = strlen( prefix );

            PopulateDataForParamResource( elementEntry, paramEntries, paramEntriesCount,
                                          paramResourceTable, strtab, nextEntry,
                                          element, localParamNo, nvBinary,
                                          nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                                          prefix, prefixLength,
                                          semanticIndexTable, semanticIndexCount,
                                          defaultIndexTable, defaultIndexCount,
                                          consttab );

            *nextEntry = *nextEntry - 1; // undo the increment on the next entry for this special case of shared entries
        }
        *nextEntry = *nextEntry + 1; // redo the increment on the next entry upon leaving the loop
    }
    else if ( paramEntry->flags & CGP_UNROLLED )
    {
        // here everything is unrolled

        CgParameterEntry* elementEntry = paramEntry + 1;

        for ( unsigned int element = 0; element < numElements; element++ )
        {
            JS_ASSERT(( elementEntry->flags & CGP_TYPE_MASK ) != CGP_ARRAY );

            // where is the array name???  in paramEntry or elementEntry???

            // name
            // copy prefix if it's not the first element whose prefix is already in place
            if ( element > 0 )
            {
                strncpy( *nvParamStrings, prefixIn, prefixLengthIn );
                prefix = *nvParamStrings;
                *nvParamStrings += prefixLengthIn;
            }
            // rest of array name for array and index
            sprintf( *nvParamStrings, "%s[%d]", ( strtab->data + paramEntry->nameOffset ), element );
            *nvParamStrings += strlen( *nvParamStrings );
            prefixLength = strlen( prefix );

            PopulateDataForParams( elementEntry, paramEntries, paramEntriesCount,
                                   paramResourceTable, strtab, nextEntry,
                                   -1, localParamNo, nvBinary,
                                   nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                                   prefix, prefixLength,
                                   semanticIndexTable, semanticIndexCount,
                                   defaultIndexTable, defaultIndexCount,
                                   consttab );

            elementEntry = *nextEntry;
        }
    }
    else
    {
        // there should be another case here for sparse array representation but
        // we don't seem to have enough bits at the moment to actually generate
        // a test that will get us here.
        // besides, we haven't completely designed it yet.
        JS_ASSERT( 0 && "Unsupported array packing designation." );
    }
}

void PopulateDataForParamStruct( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                                 unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                                 int elementResourceIndex, unsigned int* localParamNo, char* nvBinary,
                                 CgBinaryParameter** nvParams, char** nvParamStrings,
                                 CgBinaryEmbeddedConstant** nvParamOffsets, float** nvParamDefaults,
                                 char* prefix, unsigned int prefixLength,
                                 CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                                 CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount,
                                 ELF_section_t* consttab )
{
    // type index is actually the offset into the block of memory following the entries
    CgParameterStructure* paramStruct = ( CgParameterStructure* )(( char* )( paramEntries + paramEntriesCount ) + paramEntry->typeIndex );

    unsigned short memberCount = paramStruct->memberCount;
    CgParameterEntry* memberEntry = paramEntry + 1;

    // set the prefix pointer if it is not already set
    if ( prefix == NULL )
    {
        prefix = *nvParamStrings;
    }
    // add prefix of struct name and .
    // !!! but what about if we are inside an array???  then no struct name?  or is . built in?
    if ( paramEntry->nameOffset > 0 ) // is this the right test???
    {
        // name
        unsigned int nameLength = strlen( strtab->data + paramEntry->nameOffset );
        // copy the name and store its offset in the param
        strcpy( *nvParamStrings, strtab->data + paramEntry->nameOffset );

        // increment the names cursor but don't include null term
        *nvParamStrings += nameLength;
        prefixLength += nameLength;
    }

    // add the dot
    strcpy( *nvParamStrings, "." );
    *nvParamStrings += 1;
    prefixLength += 1;

    for ( unsigned int member = 0; member < memberCount; member++ )
    {
        // put the prefix in place for each subsequent member
        if ( member > 0 )
        {
            strncpy( *nvParamStrings, prefix, prefixLength );
            prefix = *nvParamStrings;
            *nvParamStrings += prefixLength;
        }

        PopulateDataForParams( memberEntry, paramEntries, paramEntriesCount,
                               paramResourceTable, strtab, nextEntry,
                               -1, localParamNo, nvBinary,
                               nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                               prefix, prefixLength,
                               semanticIndexTable, semanticIndexCount,
                               defaultIndexTable, defaultIndexCount,
                               consttab );

        memberEntry = *nextEntry;
    }
}

void PopulateDataForParams( CgParameterEntry* paramEntry, CgParameterEntry* paramEntries, unsigned int paramEntriesCount,
                            unsigned short* paramResourceTable, ELF_section_t* strtab, CgParameterEntry** nextEntry,
                            int elementResourceIndex, unsigned int* localParamNo, char* nvBinary,
                            CgBinaryParameter** nvParams, char** nvParamStrings,
                            CgBinaryEmbeddedConstant** nvParamOffsets, float** nvParamDefaults,
                            char* prefix, unsigned int prefixLength,
                            CgParameterSemantic* semanticIndexTable, unsigned int semanticIndexCount,
                            CgParameterDefaultValue* defaultIndexTable, unsigned int defaultIndexCount,
                            ELF_section_t* consttab )
{

    // expand for intrinsics, arrays, and structs
    // flags hold which type of parameter it is (along with other information)
    switch ( paramEntry->flags & CGP_TYPE_MASK )
    {
        case CGP_INTRINSIC:
        {
            //printf("resource --- %p\n", paramEntry);
            PopulateDataForParamResource( paramEntry, paramEntries, paramEntriesCount,
                                          paramResourceTable, strtab, nextEntry,
                                          elementResourceIndex, localParamNo, nvBinary,
                                          nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                                          prefix, prefixLength,
                                          semanticIndexTable, semanticIndexCount,
                                          defaultIndexTable, defaultIndexCount,
                                          consttab );
            break;
        }
        case CGP_ARRAY:
        {
            //printf("array --- %p\n", paramEntry);
            PopulateDataForParamArray( paramEntry, paramEntries, paramEntriesCount,
                                       paramResourceTable, strtab, nextEntry,
                                       elementResourceIndex, localParamNo, nvBinary,
                                       nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                                       prefix, prefixLength,
                                       semanticIndexTable, semanticIndexCount,
                                       defaultIndexTable, defaultIndexCount,
                                       consttab );
            break;
        }
        case CGP_STRUCTURE:
        {
            //printf("structure --- %p\n", paramEntry);
            PopulateDataForParamStruct( paramEntry, paramEntries, paramEntriesCount,
                                        paramResourceTable, strtab, nextEntry,
                                        elementResourceIndex, localParamNo, nvBinary,
                                        nvParams, nvParamStrings, nvParamOffsets, nvParamDefaults,
                                        prefix, prefixLength,
                                        semanticIndexTable, semanticIndexCount,
                                        defaultIndexTable, defaultIndexCount,
                                        consttab );
            break;
        }
        default:
        {
            JS_ASSERT( 0 && "unsupported type" );
            break;
        }

    }
}

// temp routine for conversion of elf to NV Binary
char* _jsConvertElfToNVBinary( const char* ptr, const char* entry )
{
    //first pass elf format: recreate the NV shader here, make it transparent for the rest of the loading
    ELF_t* elfFile = NULL;
    char* nvBinary = NULL;

    //printf("Begin binary conversion %p %s\n", ptr, entry);

    while ( 1 ) // this loop allows us to break if there is a load error without returning so we can free the elf
    {
        //load the elf and retrieve the sections
        elfFile = readElfFromMemory( ptr, 0 );
        if ( !elfFile )
            break;

        ELF_section_t* strtab = findSection( elfFile, ".strtab" );
        if ( !strtab )
            break;

        ELF_section_t* consttab = findSection( elfFile, ".const" );
        if ( !consttab )
            break;

        ELF_section_t* symtab = findSection( elfFile, ".symtab" );
        if ( !symtab )
            break;

        ELF_section_t* shadertab = findSection( elfFile, ".shadertab" );
        if ( !shadertab )
            break;

        // find the entry for program to load in the symbol table (no hash support)
        int symbolIndex = lookupSymbol( elfFile, entry );

        if ( symbolIndex == -1 )
        {
            //printf("find empty-string symbol for an effort at backwards compatibility\n");
            // find the entry for program to load in the symbol table (no hash support)
            symbolIndex = lookupSymbol( elfFile, "" );
            //printf("found empty-string symbol %d\n", symbolIndex);
        }

        if ( symbolIndex == -1 )
            break;

        Elf32_Sym* programSymbol = ( Elf32_Sym* )symtab->data + symbolIndex;
        //ELF_symbol_t* programSymbol = elfFile->symbols + symbolIndex;

        //printf("got program symbol %p %p %p\n", programSymbol, symtab, &symtab->data);

        // then inspect the program
        unsigned int st_value = programSymbol->st_value; // get index of header in shadertab
        //unsigned int st_value = programSymbol->vma; // get index of header in shadertab

        // get program out of shader table
        CgProgramHeader* shader = ( CgProgramHeader* )shadertab->data + st_value;

#if 1 // can get the sections by name but this is weaker than getting them by index

        //get the shader specifics sections
        char sectionName[64];

        sprintf( sectionName, ".text%04i", st_value );
        ELF_section_t* texttab = findSection( elfFile, sectionName );
        if ( !texttab )
            break;

        sprintf( sectionName, ".paramtab%04i", st_value );
        ELF_section_t* paramtab = findSection( elfFile, sectionName );
        if ( !paramtab )
            break;

#else // this is how we'd get them from index if our elf reader was compliant

        unsigned int st_shndx = programSymbol->section;
        // JS_ASSERT(elfFile->e_shnum > programSymbol->section);

        ELF_section_t* texttab = elfFile->sections + st_shndx;

        // it is suggested that the section headers for parameters, relocations, etc. follow this
#endif

        //////////////////////////
        // first pass to compute total size needed and allocate memory
        //////////////////////////

        size_t nvTotalBinarySize = 0;

        size_t nvParamSize = 0;
        size_t nvParamStringsSize = 0;
        size_t nvParamOffsetsSize = 0;
        size_t nvParamDefaultsSize = 0;

        const size_t nvProgramSize = _jsPad( sizeof( CgBinaryProgram ), 16 );
        nvTotalBinarySize += nvProgramSize;

        // inspect its ucode
        JS_ASSERT( shader->instructionCount*16 == texttab->header.sh_size ); // these two should be identical
        size_t ucodeSize = ( size_t )texttab->header.sh_size;

        // vertex or fragment
        bool isVertex = false;
        size_t nvProfileProgramSize = 0;

        // JS_ASSERT(programSymbol->st_other == STO_RSX_SHADER); // this symbol field is currently repackaged and unavailable

        //if (shader->profile == CG_PROFILE_SCE_VP_TYPEB || shader->profile == CG_PROFILE_SCE_VP_TYPEC || shader->profile == CG_PROFILE_SCE_VP_RSX)
        if ( shader->profile == CG_PROFILE_SCE_VP_TYPEB || shader->profile == CG_PROFILE_SCE_VP_RSX )
        {
            isVertex = true;
            const size_t nvVertexProgramSize = _jsPad( sizeof( CgBinaryVertexProgram ), 16 );
            nvTotalBinarySize += nvVertexProgramSize;
            nvProfileProgramSize = nvVertexProgramSize;
        }
        //else if (shader->profile == CG_PROFILE_SCE_FP_TYPEB || shader->profile == CG_PROFILE_SCE_FP_TYPEC || shader->profile == CG_PROFILE_SCE_FP_RSX)
        else if ( shader->profile == CG_PROFILE_SCE_FP_TYPEB || shader->profile == CG_PROFILE_SCE_FP_RSX )
        {
            const size_t nvFragmentProgramSize = _jsPad( sizeof( CgBinaryFragmentProgram ), 16 );
            nvTotalBinarySize += nvFragmentProgramSize;
            nvProfileProgramSize = nvFragmentProgramSize;
        }
        else
        {
            JS_ASSERT( 0 && "unrecognized profile" );
        }

        // then inspect its parameters
        CgParameterTableHeader* paramHeader = ( CgParameterTableHeader* )paramtab->data;

        unsigned int paramEntriesCount = paramHeader->entryCount;
        CgParameterEntry* paramEntries = ( CgParameterEntry* )(( char* )paramHeader + sizeof( CgParameterTableHeader ) );

        unsigned short* paramResourceTable = ( unsigned short* )(( char* )paramHeader + paramHeader->resourceTableOffset );

        unsigned int defaultIndexCount = paramHeader->defaultValueIndexCount;
        CgParameterDefaultValue* defaultIndexTable = ( CgParameterDefaultValue* )(( char* )paramHeader + paramHeader->defaultValueIndexTableOffset );

        unsigned int semanticIndexCount = paramHeader->semanticIndexCount;
        CgParameterSemantic* semanticIndexTable = ( CgParameterSemantic* )(( char* )paramHeader + paramHeader->semanticIndexTableOffset );

        CgParameterEntry* paramEntry = paramEntries;
        CgParameterEntry* nextEntry = NULL;

        // recurse through parameters
        for ( unsigned int paramIndex = 0; paramIndex < paramEntriesCount && ( unsigned int )( paramEntry - paramEntries ) < paramEntriesCount; paramIndex++ )
        {
            //printf("ACCUMULATE PARAM SIZE %d %p\n", paramIndex, paramEntry);

            AccumulateSizeForParams( paramEntry, paramEntries, paramEntriesCount,
                                     paramResourceTable, strtab, &nextEntry,
                                     0, -1,
                                     &nvParamSize, &nvParamStringsSize, isVertex ? NULL : &nvParamOffsetsSize,
                                     semanticIndexTable, semanticIndexCount,
                                     defaultIndexTable, defaultIndexCount, &nvParamDefaultsSize,
                                     consttab );

            paramEntry = nextEntry;
        }


        // pad each chunk of the file so the next part can at least start out aligned
        ucodeSize = _jsPad( ucodeSize, 16 );
        nvParamSize = _jsPad( nvParamSize, 16 );
        nvParamStringsSize = _jsPad( nvParamStringsSize, 16 );
        nvParamOffsetsSize = _jsPad( nvParamOffsetsSize, 16 );
        nvParamDefaultsSize = _jsPad( nvParamDefaultsSize, 16 );

        // total all the memory from params, strings (both names and semantics), offsets
        nvTotalBinarySize += ucodeSize + nvParamSize + nvParamStringsSize + nvParamOffsetsSize + nvParamDefaultsSize;

        nvBinary = ( char* )jsMalloc( nvTotalBinarySize );
        memset(( void* )nvBinary, 0, nvTotalBinarySize );

        //////////////////////////
        // second pass will go through and populate the structure
        //////////////////////////

        CgBinaryProgram* nvProgram = ( CgBinaryProgram* )nvBinary;

        // for now, nvlayout will be saved as:
        // program, profprogram, param array, ucode, offsets, strings, defaults

        nvProgram->profile = ( CGprofile )shader->profile;
        nvProgram->binaryFormatRevision = 6;
        nvProgram->totalSize = nvTotalBinarySize;
        nvProgram->parameterCount = nvParamSize / sizeof( CgBinaryParameter );
        nvProgram->parameterArray = nvProgramSize + nvProfileProgramSize;
        nvProgram->program = nvProgramSize;
        nvProgram->ucodeSize = ucodeSize;
        nvProgram->ucode = nvProgramSize + nvProfileProgramSize + nvParamSize;

        if ( isVertex )
        {
            CgBinaryVertexProgram* nvVertexProgram = ( CgBinaryVertexProgram* )( nvBinary + nvProgramSize );
            nvVertexProgram->instructionCount = shader->instructionCount;
            nvVertexProgram->instructionSlot = shader->vertexProgram.instructionSlot;
            nvVertexProgram->registerCount = shader->vertexProgram.registerCount;
            nvVertexProgram->attributeInputMask = shader->attributeInputMask;
            nvVertexProgram->attributeOutputMask = shader->vertexProgram.attributeOutputMask;
            nvVertexProgram->userClipMask = shader->vertexProgram.userClipMask;
        }
        else
        {
            CgBinaryFragmentProgram* nvFragmentProgram = ( CgBinaryFragmentProgram* )( nvBinary + nvProgramSize );
            nvFragmentProgram->instructionCount = shader->instructionCount;
            nvFragmentProgram->attributeInputMask = shader->attributeInputMask;
            nvFragmentProgram->partialTexType = shader->fragmentProgram.partialTexType;
            nvFragmentProgram->texCoordsInputMask = shader->fragmentProgram.texcoordInputMask;
            nvFragmentProgram->texCoords2D = shader->fragmentProgram.texcoord2d;
            nvFragmentProgram->texCoordsCentroid = shader->fragmentProgram.texcoordCentroid;
            nvFragmentProgram->registerCount = shader->fragmentProgram.registerCount;
            nvFragmentProgram->outputFromH0 = shader->fragmentProgram.flags & CGF_OUTPUTFROMH0;
            nvFragmentProgram->depthReplace = shader->fragmentProgram.flags & CGF_DEPTHREPLACE;
            nvFragmentProgram->pixelKill = shader->fragmentProgram.flags & CGF_PIXELKILL;
        }

        // copy the ucode into place (copy actual ucode size without padding...  although should always be aligned implicitly)
        memcpy( nvBinary + nvProgram->ucode, texttab->data, texttab->header.sh_size );

        // parameters
        CgBinaryParameter* nvParams = ( CgBinaryParameter* )( nvBinary + nvProgram->parameterArray );
        // offsets come immediately after ucode
        CgBinaryEmbeddedConstant* nvParamOffsets = ( CgBinaryEmbeddedConstant* )( nvBinary + nvProgram->ucode + ucodeSize );
        // strings come after offsets
        char* nvParamStrings = nvBinary + nvProgram->ucode + ucodeSize + nvParamOffsetsSize;
        // defaults come last
        float* nvParamDefaults = ( float* )( nvBinary + nvProgram->ucode + ucodeSize + nvParamOffsetsSize + nvParamStringsSize );

#if 0
        printf( "SIZE %d, OFFSETS %d %d %d %d START %p END %p\n", nvTotalBinarySize,
                nvProgram->parameterArray,
                nvProgram->ucode,
                nvProgram->ucode + ucodeSize + nvParamOffsetsSize,
                nvProgram->ucode + ucodeSize + nvParamOffsetsSize + nvParamStringsSize,
                nvProgram, nvProgram + nvTotalBinarySize );
#endif

        unsigned int localParamNo = 0;

        paramEntry = paramEntries;
        nextEntry = NULL;

        // recurse through parameters
        for ( unsigned int paramIndex = 0; paramIndex < paramEntriesCount && ( unsigned int )( paramEntry - paramEntries ) < paramEntriesCount; paramIndex++ )
        {
            //printf("POPULATE PARAM INDEX %d %p\n", paramIndex, paramEntry);

            PopulateDataForParams( paramEntry, paramEntries, paramEntriesCount,
                                   paramResourceTable, strtab, &nextEntry,
                                   -1, &localParamNo, nvBinary,
                                   &nvParams, &nvParamStrings, isVertex ? NULL : &nvParamOffsets, &nvParamDefaults,
                                   NULL, 0,
                                   semanticIndexTable, semanticIndexCount,
                                   defaultIndexTable, defaultIndexCount,
                                   consttab );

            paramEntry = nextEntry;
        }

        break;  // simply exit the loop since we reached the end
    }

    if ( elfFile )
    {
        freeElf( elfFile );
    }


#if 0
    CgBinaryProgram* nvProgram = ( CgBinaryProgram* )nvBinary;
    printf( "FINAL parameter count %d\n", nvProgram->parameterCount );
    for ( int printi = 0; printi < nvProgram->parameterCount; printi++ )
    {
        printf( "param %d %p name %d %s %d %d\n", printi, nvBinary + nvProgram->parameterArray,
                (( CgBinaryParameter* )( nvBinary + nvProgram->parameterArray ) + printi )->name,
                nvBinary + (( CgBinaryParameter* )( nvBinary + nvProgram->parameterArray ) + printi )->name,
                (( CgBinaryParameter* )( nvBinary + nvProgram->parameterArray ) + printi )->res,
                (( CgBinaryParameter* )( nvBinary + nvProgram->parameterArray ) + printi )->resIndex );
        /* 		    printf("param %d name %d %s\n", printi, */
        /* 			   ((CgBinaryParameter*)(nvBinary + nvProgram->parameterArray) + printi)->name,  */
        /* 			   nvBinary + ((CgBinaryParameter*)(nvBinary + nvProgram->parameterArray) + printi)->name); */
    }
#endif

    printf( "--- COMPLETED ELF TO NV BINARY CONVERSION ---\n" );

    return nvBinary;
}

bool _jsCgCreateProgramChecks( CGcontext ctx, CGprofile profile, CGenum program_type )
{
    // check context
    if ( !CG_IS_CONTEXT( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return false;
    }

    // check the profile.
    switch ( profile )
    {
            /*case CG_PROFILE_SCE_VP_TYPEC:
            case CG_PROFILE_SCE_FP_TYPEC:
            	_JS_REPORT_EXTRA( PSGL_REPORT_CG_PROFILE_DEPRECATED, "Profile %s has been deprecated. Use sce_fp_rsx or sec_vp_rsx instead.", cgGetProfileString( profile ) );*/
        case CG_PROFILE_SCE_VP_TYPEB:
        case CG_PROFILE_SCE_FP_TYPEB:
        case CG_PROFILE_SCE_VP_RSX:
        case CG_PROFILE_SCE_FP_RSX:
            break;
        default:
            _jsCgRaiseError( CG_UNKNOWN_PROFILE_ERROR );
            return false;
    }

    // check the program_type
    switch ( program_type )
    {
        case CG_BINARY:
        case CG_SOURCE:
            break;
        default: // currently reject CG_OBJECT (assembly code in ASCII) and CG_UNDEFINED
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return false;
    }

    return true;
}

typedef struct
{
    const char* elfFile;
    size_t elfFileSize;

    const char *symtab;
    size_t symbolSize;
    size_t symbolCount;
    const char *symbolstrtab;

    const char* shadertab;
    size_t shadertabSize;
    const char* strtab;
    size_t strtabSize;
    const char* consttab;
    size_t consttabSize;
}
CGELFBinary;

typedef struct
{
    const char *texttab;
    size_t texttabSize;
    const char *paramtab;
    size_t paramtabSize;
    int index;
}
CGELFProgram;

static bool cgOpenElf( const void *ptr, size_t size, CGELFBinary *elfBinary )
{
    while ( 1 )
    {
        size_t symbolSize;
        size_t symbolCount;
        const char *symbolstrtab;
        const char *symtab = findSymbolSectionInPlace(( const char * )ptr, size, &symbolSize, &symbolCount, &symbolstrtab );
        if ( !symtab )
            break;

        size_t shadertabSize;
        const char *shadertab = findSectionInPlace(( const char* )ptr, size, ".shadertab", &shadertabSize );
        if ( !shadertab )
            break;
        size_t strtabSize;
        const char *strtab = findSectionInPlace(( const char* )ptr, size, ".strtab", &strtabSize );
        if ( !strtab )
            break;
        size_t consttabSize;
        const char *consttab = findSectionInPlace(( const char* )ptr, size, ".const", &consttabSize );
        if ( !consttab )
            break;

        //check the ELF version
        size_t noteSize;
        const char *notetab = findSectionInPlace(( const char* )ptr, size, ".note", &noteSize );
        unsigned int archiverVersion = 0;
        if ( notetab )
        {
            //traverse the notes until finding the CGNV2ELF_PRODUCT_STRING
            const char *noteEnd = notetab + noteSize;
            const size_t productIDLength = sizeof( CGNV2ELF_PRODUCT_STRING ) - 1;
            while ( notetab < noteEnd )
            {
                unsigned int n_namesz = *( const unsigned int* )notetab;
                notetab += 4;
                unsigned int n_descsz = *( const unsigned int* )notetab;
                notetab += 4;
                unsigned int n_type = *( const unsigned int* )notetab;
                notetab += 4;

                if ( n_namesz != productIDLength || n_type != CGNV2ELF_VERSION_NOTE_TYPE )
                {
                    notetab += ( _jsPad( n_namesz, 4 ) + _jsPad( n_descsz, 4 ) );
                    continue;
                }

                bool match = strncmp( notetab, CGNV2ELF_PRODUCT_STRING, productIDLength ) == 0;
                notetab += _jsPad( n_namesz, 4 );

                if ( !match )
                {
                    notetab += _jsPad( n_descsz, 4 );
                    continue;
                }

                //we found it
                JS_ASSERT( n_descsz == 4 );
                archiverVersion = *( const unsigned int* )notetab;
                break;
            }
        }
        if ( archiverVersion < CGNV2ELF_VERSION )
        {
            //output version mismatched
            _JS_REPORT_EXTRA( PSGL_REPORT_ASSERT, "CG elf binary version r%i not supported by this SDK. The version needs to be r%i or higher. The binary needs to be generated with a more recent archiver tool. Please use cgnv2elf -v to check the version.", archiverVersion, CGNV2ELF_VERSION );
            break;
        }

        elfBinary->elfFile = ( const char* )ptr;
        elfBinary->elfFileSize = size;
        elfBinary->symtab = symtab;
        elfBinary->symbolSize = symbolSize;
        elfBinary->symbolCount = symbolCount;
        elfBinary->symbolstrtab = symbolstrtab;

        elfBinary->shadertab = shadertab;
        elfBinary->shadertabSize = shadertabSize;
        elfBinary->strtab = strtab;
        elfBinary->strtabSize = strtabSize;

        elfBinary->consttab = consttab;
        elfBinary->consttabSize = consttabSize;

        return true;
    }

    return false;
}

static void cgCloseElf( CGELFBinary *elfBinary )
{}

static bool cgGetElfProgramByIndex( CGELFBinary *elfBinary, int index, CGELFProgram *elfProgram )
{
    while ( true )
    {
        char sectionName[64];
        sprintf( sectionName, ".text%04i", index );
        size_t texttabSize;
        const char *texttab = findSectionInPlace( elfBinary->elfFile, elfBinary->elfFileSize, sectionName, &texttabSize );
        if ( !texttab )
            break;
        sprintf( sectionName, ".paramtab%04i", index );
        size_t paramtabSize;
        const char *paramtab = findSectionInPlace( elfBinary->elfFile, elfBinary->elfFileSize, sectionName, &paramtabSize );
        if ( !paramtab )
            break;

        elfProgram->texttab = texttab;
        elfProgram->texttabSize = texttabSize;
        elfProgram->paramtab = paramtab;
        elfProgram->paramtabSize = paramtabSize;
        elfProgram->index = index;
        return true;
    }
    return false;
}

static bool cgGetElfProgramByName( CGELFBinary *elfBinary, const char *name, CGELFProgram *elfProgram )
{
    //if no name try to return the first program
    int res;
    if ( name == NULL || name[0] == '\0' )
        res = 0;
    else
        res = lookupSymbolValueInPlace( elfBinary->symtab, elfBinary->symbolSize, elfBinary->symbolCount, elfBinary->symbolstrtab, name );

    if ( res != -1 )
        return cgGetElfProgramByIndex( elfBinary, res, elfProgram );
    else
        return false;
}

static CGprogram _jsCgCreateProgram( CGcontext ctx, CGprofile profile, const CgProgramHeader *programHeader, const void *ucode, const CgParameterTableHeader *parameterHeader, const char *stringTable, const float *defaultValues )
{
    // Create the program structure.
    // all the structural data is filled in here,
    // as well as the profile.
    // The parameters and the actual program are generated from the ABI specific calls.

    _CGprogram* prog = ( _CGprogram* )jsMalloc( sizeof( _CGprogram ) );
    if ( NULL == prog )
    {
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        return NULL;
    }

    // zero out the fields
    _jsCgProgramZero( prog );

    // fill in the fields we know
    prog->parentContext = _cgGetContextPtr( ctx );
    prog->header.profile = profile;

    int success = 0;

    // create a name for the program and record it in the object
    CGprogram id = ( CGprogram )_jsCreateName( &_CurrentContext->cgProgramNameSpace, prog );
    if ( !id )
    {
        jsFree( prog );
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        return NULL;
    }
    prog->id = id;

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

    // load the binary into the program object
    switch ( profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            // TODO ************** need to include the entry symbol too
            success = _jsPlatformGenerateVertexProgram( prog, programHeader, ucode, parameterHeader, stringTable, defaultValues );
            break;
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            success = _jsPlatformGenerateFragmentProgram( prog, programHeader, ucode, parameterHeader, stringTable, defaultValues );
            break;
        default:
            // should never reach here
            JS_ASSERT( 0 );
    }

    // if the creation failed, free all resources.
    // the error was raised when the error was encoutered.
    if ( 0 == success )
    {
        // free the program object
        jsFree( prog );
        // release the id too
        _jsEraseName( &_CurrentContext->cgProgramNameSpace, ( jsName )id );
        return NULL;
    }

    // success! add the program to the program list in the context.
    _jsCgProgramPushFront( prog->parentContext, prog );
    if ( _cgProgramCreateHook ) _cgProgramCreateHook( prog );

    // everything worked.
    return id;
}

static CGprogram _jsCgUpdateProgramAtIndex( CGprogramGroup group, int index, int refcount );

// API functions ----------------------------------------

/** @addtogroup Cg
 *@{
 */
/**
@short Generate a new program object

cgCreateProgram() generates a new <CODE>CGprogram</CODE> object and
adds it to the specified Cg context.

To use cgCreateProgram() to create a new <CODE>CGProgram</CODE> object from Cg source code, you must link
with the files <CODE>libcgc.a</CODE> and <CODE>libPSGLcgc.a</CODE>, and call the cgRTCgcInit() initialization function.

The following is a typical sequence of commands for initializing a new program:

@code
CGcontext ctx = cgCreateContext();
CGprogram prog = cgCreateProgram(ctx,
                                 CG_BINARY,
                                 program,
                                 CG_PROFILE_SCE_FP_RSX,
                                 "main",
                                 NULL);
@endcode

@param       ctx     Specifies the context that the new program will be added to.

@param       program_type
The <I><c>program_type</c></I> parameter is an enumerant that describes what
the <I><c>program</c></I> parameter string contains.  The following is a list
of valid enumerants that may be passed in:
\arg <CODE>CG_BINARY: </CODE> If <I><c>program_type</c></I> is <CODE>CG_BINARY</CODE>, <I><c>program</c></I> must be a string that contains binary
code that resulted from the compilation of Cg source code.
\arg <CODE>CG_SOURCE: </CODE> If <I><c>program_type</c></I> is <CODE>CG_SOURCE</CODE>, <I><c>program</c></I> must be a string that contains the
Cg source code to be compiled.
\arg <CODE>CG_OBJECT:</CODE> Not implemented

@param       program A string containing the compiled binary code or Cg source code of a shader program.
               See the <I><c>program_type</c></I> parameter above for more information.

@param       profile The enumerant for the profile the program.
<CODE>CG_PROFILE_SCE_VP_RSX</CODE> and <CODE>CG_PROFILE_SCE_FP_RSX</CODE> are valid enumerants for
             vertex and fragment shader programs.

@param       entry   If <i><c>program_type</c></i> is <c>CG_SOURCE</c>, <i><c>entry</c></i> specifies the entry point to the shader program.  If set to
               <CODE>NULL</CODE>, the default value is <CODE>"main"</CODE>. If <i><c>program_type</c></i> is <c>CG_BINARY</c>, then <i><c>entry</c></i> is the
	       name (without extension) of the Cg program to create within the binary.  If set to <c>NULL</c>, the name of the first program in the binary is used.

@param       args    If <i><c>program_type</c></i> is <c>CG_SOURCE</c> and <I><c>args</c></I> is not <CODE>NULL</CODE> it is assumed to be an array of
null-terminated strings that will be passed as directly to the compiler
as arguments.  The last value of the array must be a <CODE>NULL</CODE>.  For other values of <i><c>program_type</c></i>, <i>args</i> is ignored.

@return Returns a <CODE>CGprogram</CODE> handle on success.
@par
Returns <CODE>NULL</CODE> if any error occurs.

@note
As of SDK 0.9.0, cgCreateProgram() does not copy the buffer passed for the <i><c>program</c></i> parameter.  Therefore, do not
delete the buffer until you have called cgDestroyProgram() for the program.

@note
Programs that use the old Cg binary format will be converted to the new binary format when the program object is created.  Because the
name of the converted program is undefined, <i><c>entry</c></i> must be <c>NULL</c>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>program_type</c></I> is an invalid enumerant.</TD>
</TR>
<TR>
<TD><CODE>CG_UNKNOWN_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is not a supported profile.</TD>
</TR>
<TR>
<TD><CODE>CG_COMPILER_ERROR</CODE></TD>
<TD>The compile failed.</TD>
</TR>
</TABLE>

@sa       cgCreateContext, cgGetProgramString, cgCreateProgramFromFile, cgRTCgcInit
 */
CG_API CGprogram cgCreateProgram( CGcontext ctx,
                                  CGenum program_type,
                                  const char* program,
                                  CGprofile profile,
                                  const char* entry,
                                  const char** args )
{
    JS_APIENTRY_TIMER( cgCreateProgram );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        int argCount = 0;
        if ( _JS_DPM_STATE( CAPTURE ) )
        {
            const char **a = args;
            if ( a != NULL ) while ( *( a++ ) != NULL )   argCount++;
        }
#ifdef JS_DPM_V2
        _JS_DPM_STANDARD_API_PREAMBLE( cgCreateProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, 6, 0 );
        _JS_DPM_API_PROCESS_ARGUMENT( 1, ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
        _JS_DPM_API_PROCESS_ARGUMENT( 2, program_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
        _JS_DPM_API_PROCESS_ARGUMENT( 3, program, sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// treat the binary program as non-volatile data (a risky assumption)
        _JS_DPM_API_PROCESS_ARGUMENT( 4, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
        _JS_DPM_API_PROCESS_ARGUMENT( 5, entry, sizeof( entry ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        _JS_DPM_API_PROCESS_ARGUMENT( 6, args, sizeof( args ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        _JS_DPM_STANDARD_API_LEADOUT( cgCreateProgram, JSDPM_CALL_TYPE_STATE_CG, NULL );
#else
        const int parameterCount = 6;
        if ( entry == NULL && _JS_DPM_STATE( CAPTURE ) )
        {
            _JS_DPM_API_ENTRY_6( cgCreateProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, parameterCount + argCount + 1,
                                 ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 program_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 program, sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,	// treat the binary program as non-volatile data (a risky assumption)
                                 profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 entry, sizeof( entry ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,
                                 args, argCount*sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_6( cgCreateProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, parameterCount + argCount + 1,
                                 ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 program_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 program, sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,	// treat the binary program as non-volatile data (a risky assumption)
                                 profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 entry, strlen( entry ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                                 args, argCount*sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }


        int i;
        if ( _JS_DPM_STATE( CAPTURE ) )
        {
            for ( i = 0; i < argCount; ++i )
            {
                _JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(( parameterCount + 1 + i ), args[i], strlen( args[i] ) + 1, _NOBRACKET, INPUT_BY_REFERENCE, 0 );
            }	// for
            _JS_DPM_CURRENT_ARGUMENT_PTR( parameterCount + argCount + 1 ) = NULL;
        }
        else if ( _JS_DPM_STATE( REPLAY ) )
        {

            args = ( const char ** )_JS_DPM_CURRENT_ARGUMENT_PTR( parameterCount + 1 );
        }
#endif // JS_DPM_V1 half of JS_DM_V2
    }
#endif // JS_DPM

    // Load a program from a memory pointer.
    // NOTE: in our API all programs are pre-compiled binaries
    // so entry point and compiler arguments are ignored.

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

    //hack to counter change of defines for program_type at r5294
    // previously CG_BINARY was defined the same as CG_ROW_MAJOR
    // if those values are passed in here, move them to the new values and remove this hack after we have
    // an sdk that incorporates these changes so that prebuild libs (aka debugfont) can be used meanwhile
    if ( program_type == CG_ROW_MAJOR )
        program_type = CG_BINARY;

    if ( !_jsCgCreateProgramChecks( ctx, profile, program_type ) )
        return NULL;

    //data to extract from the buffer passed:
    CgProgramHeader *programHeader = NULL;
    const void *ucode = NULL;
    CgParameterTableHeader *parameterHeader = NULL;
    const char *stringTable = NULL;
    const float *defaultValues = NULL;

    //first step, compile any source file
    const char *binaryBuffer = NULL;
    char* compiled_program = NULL;
    if ( program_type == CG_SOURCE )
    {
        if ( _cgRTCgcCompileProgramHook )
        {
            _cgRTCgcCompileProgramHook( program, cgGetProfileString( profile ), entry, args, &compiled_program );
            if ( !compiled_program )
            {
                _jsCgRaiseError( CG_COMPILER_ERROR );
                return NULL;
            }
            binaryBuffer = compiled_program;
        }
        else
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "The CG runtime compiler hasn't been setup. cgRTCgcInit() should be called prior to this function." );
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return NULL;
        }
    }
    else
    {
        binaryBuffer = program;
    }

    bool bConvertedToElf = false;

    //At that point we have a binary file which is either any ELF or an NV format file
    const unsigned int ElfTag = 0x7F454C46; // == MAKEFOURCC(0x7F,'E','L','F');
    if ( !( *( unsigned int* )binaryBuffer == ElfTag ) )
    {
        //we have an NV file, convert it to the runtime format

        // if it was initially binary, throw warning about old format and recommend conversion to new with cgnv2elf
        // don't throw the warning if it was source, cause clearly that would have been on purpose.
        if ( program_type == CG_BINARY )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "A binary shader is being loaded using a deprecated binary format.  Please use the cgnv2elf tool to convert to the new, memory-saving, faster-loading format." );
        }

        //convert from NV format to the runtime format
        int compiled_program_size = 0;
        STL_NAMESPACE vector<char> stringTableArray;
        STL_NAMESPACE vector<float> defaultValuesArray;
        CgBinaryProgram* nvProgram = ( CgBinaryProgram* )binaryBuffer;
        char *runtimeElfShader = NULL;
        //check the endianness
        int totalSize;
        if (( nvProgram->profile != CG_PROFILE_SCE_FP_TYPEB ) && ( nvProgram->profile != CG_PROFILE_SCE_VP_TYPEB ) &&
                ( nvProgram->profile != ( CGprofile )7006 ) && ( nvProgram->profile != ( CGprofile )7005 ) &&
                ( nvProgram->profile != CG_PROFILE_SCE_FP_RSX ) && ( nvProgram->profile != CG_PROFILE_SCE_VP_RSX ) )
        {
            totalSize = endianSwapWord( nvProgram->totalSize );
        }
        else
            totalSize = nvProgram->totalSize;
        int res = convertNvToElfFromMemory( binaryBuffer, totalSize, 2, 0, ( void** ) & runtimeElfShader, &compiled_program_size, stringTableArray, defaultValuesArray );
        if ( res != 0 )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "invalid CG binary program" );
            _jsCgRaiseError( CG_COMPILER_ERROR );
            if ( compiled_program )
                _cgRTCgcFreeCompiledProgramHook( compiled_program );
            return NULL;
        }

        if ( compiled_program )
            _cgRTCgcFreeCompiledProgramHook( compiled_program );

        //TODO: remove all the unnecessary data shuffling, when this gets included in the compiler
        //prepare the same buffer as the one that would be extracted from the binary file
        size_t stringTableSize = stringTableArray.size() * sizeof( stringTable[0] );
        size_t defaultTableSize = defaultValuesArray.size() * sizeof( defaultValues[0] );
        int paddedSize = _jsPad( compiled_program_size, 4 );
        //the following pointer will be remembered in the _CGprogram structure to be freed when the
        //program will be deleted, the ucode and the param info are used in place at runtime
        char *runtimeElf = ( char* )jsMemalign( 16, paddedSize + stringTableSize + defaultTableSize );
        if ( !runtimeElf )
        {
            _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
            return NULL;
        }
        bConvertedToElf = true; //to free the memalign
        //program header, ucode and parameters
        memcpy( runtimeElf, runtimeElfShader, compiled_program_size );

        //we can free the intermediate buffer used to receive the converted program //may be we should keep it rather than reallocating ?
        convertNvToElfFreeBinaryShader( runtimeElfShader );

        //default values
        //the pointer is stored right after the program data ( padded to 4 ) and after the string table pointer
        float* pDefaultValues = ( float* )(( char* )runtimeElf + paddedSize );
        defaultValues = pDefaultValues;

        if ( defaultTableSize )
            memcpy( pDefaultValues, &defaultValuesArray[0], defaultTableSize );

        //string table
        //the pointer is stored right after the program data ( padded to 4 )
        char *pStringTable = ( char* )runtimeElf + paddedSize + defaultTableSize;
        stringTable = pStringTable;

        if ( stringTableSize )
            memcpy( pStringTable, &stringTableArray[0], stringTableSize );


        //success
        programHeader = ( CgProgramHeader* )runtimeElf;
        size_t elfUcodeSize = programHeader->instructionCount * 16;
        size_t ucodeOffset = _jsPad( sizeof( CgProgramHeader ), 16 );
        size_t parameterOffset = _jsPad( ucodeOffset + elfUcodeSize, 16 );
        ucode = ( char* )runtimeElf + ucodeOffset;
        parameterHeader = ( CgParameterTableHeader* )(( char* )runtimeElf + parameterOffset );
    }
    else
    {
        //we have an ELF file, it will be used in place ( case where the elf file has been passed as a memory pointer )
        CGELFBinary elfBinary;
        CGELFProgram elfProgram;
        if ((( intptr_t )binaryBuffer ) & 15 )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "CG Binary not aligned on 16 bytes, needed for ucode section" );
            _jsCgRaiseError( CG_PROGRAM_LOAD_ERROR );
            return NULL;
        }
        bool res = cgOpenElf( binaryBuffer, 0, &elfBinary );
        if ( !res )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "not a valid ELF" );
            _jsCgRaiseError( CG_PROGRAM_LOAD_ERROR );
            return NULL;
        }
        if ( !cgGetElfProgramByName( &elfBinary, entry, &elfProgram ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "couldn't find the shader entry in the CG binary" );
            cgCloseElf( &elfBinary );
            return NULL;
        }

        //success
        programHeader = ( CgProgramHeader* )elfBinary.shadertab + elfProgram.index;
        ucode = ( char* )elfProgram.texttab;
        parameterHeader = ( CgParameterTableHeader* )elfProgram.paramtab;
        stringTable = elfBinary.strtab;
        defaultValues = ( float* )elfBinary.consttab;

        cgCloseElf( &elfBinary );
    }

    CGprogram prog = _jsCgCreateProgram( ctx, profile, programHeader, ucode, parameterHeader, stringTable, defaultValues );

    //if we used the runtime compiler/converter we need to delete the buffer on exit
    if ( bConvertedToElf )
    {
        _CGprogram* ptr = _cgGetProgPtr( prog );
        ptr->runtimeElf = programHeader;
    }

    return prog;
}



/**
@short Generate a new program object from a file

cgCreateProgramFromFile() generates a new <CODE>CGprogram</CODE> object and adds it to the
specified Cg context.

To use cgCreateProgramFromFile() to create a new <CODE>CGProgram</CODE> object from Cg source code, you must link
with the file <CODE>libcgc.a</CODE> and <CODE>libPSGLcgc.a</CODE>, and call the cgRTCgcInit() initialization function.

The following is a typical sequence of commands for initializing a new program:

@code
CGcontext ctx = cgCreateContext();
CGprogram prog = cgCreateProgramFromFile(ctx,
                                         CG_BINARY,
                                         program_file,
                                         CG_PROFILE_SCE_FP_RSX,
                                         "main",
                                         NULL);
@endcode

@param       ctx     Specifies the context that the new program will be added to.

@param       program_type
The <I><c>program_type</c></I> parameter is an enumerant that describes what
the file indicated by the <I><c>program_file</c></I> parameter contains.
The following is a list of valid enumerants that may be passed in:
\arg <CODE>CG_BINARY: </CODE> If <I><c>program_type</c></I> is
<CODE>CG_BINARY</CODE>, <I><c>program_file</c></I> must be a file
that contains binary code that resulted from the compilation of Cg
source code.
\arg <CODE>CG_SOURCE: </CODE> If <I><c>program_type</c></I> is <CODE>CG_SOURCE</CODE>, <I><c>program</c></I> must be a file that contains the
Cg source code to be compiled.
\arg <CODE>CG_OBJECT: </CODE> Not implemented.

@param       program_file A name of a file that contains the compiled binary code of a shader program.
               See the <I><c>program_type</c></I> parameter above for more information.

@param       profile The enumerant for the profile the program.
<CODE>CG_PROFILE_SCE_VP_RSX</CODE> and <CODE>CG_PROFILE_SCE_FP_RSX</CODE> are valid enumerants for
             vertex and fragment shader programs.

@param       entry   If <i><c>program_type</c></i> is <c>CG_SOURCE</c>, then <I><c>entry</c></I> is the entry point to the program in the Cg source.  If set to
               <CODE>NULL</CODE>, the default value is <CODE>"main"</CODE>.
	       If <i><c>program_type</c></i> is <c>CG_BINARY</c>, then <I><c>entry</c></I> is the
	       name (without extension) of the Cg program to create within the binary.  If set to <c>NULL</c>, the name of the first program in the binary is used.

@param       args    If <i><c>program_type</c></i> is <c>CG_SOURCE</c> and <I><c>args</c></I> is not <CODE>NULL</CODE>, it is assumed to be an array of
null-terminated strings that will be passed as directly to the compiler
as arguments.  The last value of the array must be a <CODE>NULL</CODE>.  For other program types, <i><c>args</c></i> is ignored.

@note
Programs that use the old Cg binary format will be converted to the new binary format when the program object is created.  Because the
name of the converted program is undefined, <I><c>entry</c></I> must be <c>NULL</c>.

@return Returns a <CODE>CGprogram</CODE> handle on success.
@par
Returns <CODE>NULL</CODE> if any error occurs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>program_type</c></I> is an invalid enumerant.</TD>
</TR>
<TR>
<TD><CODE>CG_UNKNOWN_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is not a supported profile.</TD>
</TR>
<TR>
<TD><CODE>CG_FILE_READ_ERROR</CODE></TD>
<TD><I><c>program_file</c></I> is not a name of an existing file.</TD>
</TR>
<TR>
<TD><CODE>CG_COMPILER_ERROR</CODE></TD>
<TD>The compile failed.</TD>
</TR>
</TABLE>

@sa       cgCreateContext, cgGetProgramString, cgCreateProgram, cgRTCgcInit
 */
CG_API CGprogram cgCreateProgramFromFile( CGcontext ctx,
        CGenum program_type,
        const char* program_file,
        CGprofile profile,
        const char* entry,
        const char** args )
{
    JS_APIENTRY_TIMER( cgCreateProgramFromFile );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        const int parameterCount = 6;
        int argCount = 0;
        if ( _JS_DPM_STATE( CAPTURE ) )
        {
            const char **a = args;
            if ( a != NULL ) while ( *a != NULL )   argCount++;
        }

        if ( entry == NULL )
        {
            _JS_DPM_API_ENTRY_6( cgCreateProgramFromFile, JSDPM_CALL_TYPE_STATE_CG, NULL, parameterCount + argCount + 1,
                                 ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 program_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 program_file, strlen( program_file ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                                 profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 entry, sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,
                                 args, argCount*sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_6( cgCreateProgramFromFile, JSDPM_CALL_TYPE_STATE_CG, NULL, parameterCount + argCount + 1,
                                 ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 program_type, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 program_file, strlen( program_file ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                                 profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 entry, strlen( entry ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                                 args, argCount*sizeof( char * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }

        int i;
        if ( _JS_DPM_STATE( CAPTURE ) )
        {
            for ( i = 0; i < argCount; ++i )
            {
#ifdef	JS_DPM
#ifndef	JS_DPM_V2
                _JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(( parameterCount + 1 + i ), args[i], strlen( args[i] ) + 1, _NOBRACKET, INPUT_BY_REFERENCE, 0 );
#else
                _JS_DPM_API_ENTRY_ALLOCATE_AND_STORE(( parameterCount + 1 + i ), args[i], args[i], strlen( args[i] ) + 1, _NOBRACKET, INPUT_BY_REFERENCE, ( DPMAPIArgumentType )0 );
#endif
#endif
            }	// for
            _JS_DPM_CURRENT_ARGUMENT_PTR( parameterCount + argCount + 1 ) = NULL;
        }
        else if ( _JS_DPM_STATE( REPLAY ) )
        {
            args = ( const char ** )_JS_DPM_CURRENT_ARGUMENT_PTR( parameterCount + 1 );
        }
    }
#endif

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

    //hack to counter change of defines for program_type at r5294
    // previously CG_BINARY was defined the same as CG_ROW_MAJOR
    // if those values are passed in here, move them to the new values and remove this hack after we have
    // an sdk that incorporates these changes so that prebuild libs (aka debugfont) can be used meanwhile
    if ( program_type == CG_ROW_MAJOR )
        program_type = CG_BINARY;

    if ( !_jsCgCreateProgramChecks( ctx, profile, program_type ) )
        return NULL;

    // load a program from a file
    // NOTE: in our API all programs are pre-compiled binaries
    // so profile and compiler arguments are ignored.
    FILE* fp = NULL;
    if ( JS_LIKELY( program_type == CG_BINARY ) )
    {
        CGprogram ret = NULL;
        //assume we have an elf at that point
        //if (filetag == ElfTag)

        _CGcontext *context = _cgGetContextPtr( ctx );
        CGprogramGroup group = NULL;

        //can we find it in the groups already loaded ?
        group = context->groupList;
        while ( group )
        {
            //check the group name
            const char *groupName = _jsCgGetProgramGroupName( group );
            if ( groupName && !strcmp( groupName, program_file ) )
            {
                int index;
                if ( entry == NULL )
                    index = 0;
                else
                    index = _jsCgGetProgramIndex( group, entry );
                if ( index >= 0 )
                {
                    ret = _jsCgUpdateProgramAtIndex( group, index, 1 );
                    break;
                }
                else
                {
                    //we couldn't find the entry in the group which has the right name
                    return ( CGprogram )NULL;
                }
            }
            group = group->next;
        }

        if ( ret )
            return ret;
        else
        {
            //do we have an elf file ?
            //read file tag:
            // check that file exists
            fp = fopen( program_file, "rb" );

            if ( NULL == fp )
            {
                _jsCgRaiseError( CG_FILE_READ_ERROR );
                return ( CGprogram )NULL;
            }

            unsigned int filetag = 0;
            int res = fread( &filetag, sizeof( filetag ), 1, fp );
            if ( !res )
            {
                fclose( fp );
                _jsCgRaiseError( CG_FILE_READ_ERROR );
                return ( CGprogram )NULL;
            }
            const unsigned int ElfTag = 0x7F454C46; // == MAKEFOURCC(0x7F,'E','L','F');
            if ( filetag == ElfTag )
            {
                fclose( fp );

                group = _jsCgCreateProgramGroupFromFile( ctx, program_file );
                if ( group )
                {
                    _CGprogramGroup *_group = ( _CGprogramGroup * )group;
                    _group->userCreated = false;
                    if ( entry == NULL )
                    {
                        if ( group->programCount == 1 )
                        {
                            ret = _jsCgUpdateProgramAtIndex( group, 0, 1 );
                        }
                    }
                    else
                    {
                        int index = _jsCgGetProgramIndex( group, entry );
                        if ( index == -1 )
                        {
                            _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "couldn't find the shader entry in the CG binary" );
                        }
                        else
                        {
                            ret = _jsCgUpdateProgramAtIndex( group, index, 1 );
                        }
                    }
                }
                return ret;
            }
            //else
            //rewind(); //we should rewind here, but no need since we are doing fseek after
        }
    }

    //we have a NV file or a CG source:
    if ( !fp )
    {
        fp = fopen( program_file, "rb" );
        if ( NULL == fp )
        {
            _jsCgRaiseError( CG_FILE_READ_ERROR );
            return ( CGprogram )NULL;
        }
    }

    // find the file length
    size_t file_size = 0;
    fseek( fp, 0, SEEK_END );
    file_size = ftell( fp );
    rewind( fp );

    // alloc memory for the file
    char* ptr = ( char* )jsMalloc( file_size + 1 );
    if ( NULL == ptr )
    {
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        fclose( fp );
        return ( CGprogram )NULL;
    }

    // read the entire file into memory then close the file
    // TODO ********* just loading the file is a bit lame really. We can do better.
    fread( ptr, file_size, 1, fp );
    fclose( fp );

    if ( program_type == CG_SOURCE )
    {
        ptr[file_size] = '\0';
    }

    // call the CreateProgram API to do the rest of the job.
    _JS_DPM_TRACE_OFF // create prog
    CGprogram ret = cgCreateProgram( ctx, program_type, ptr, profile, entry, args );
    _JS_DPM_TRACE_ON

    // free the memory for the file, we're done with it
    jsFree( ptr );

    return ret;
}

/**
@short Make a copy of a program object

cgCopyProgram() creates a new program object that is a copy of <I><c>program</c></I> and
adds it to the same context as <I><c>program</c></I>.  This function is useful for
creating a new instance of a program whose parameter properties have been
modified by the run-time API.

@param      program    Specifies the program to copy.

@return Returns a copy of <I><c>program</c></I> on success.
@par
Returns <CODE>NULL</CODE> if <I><c>program</c></I> is invalid or allocation fails.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is an invalid program handle.</TD>
</TR>
</TABLE>
@sa cgCreateProgram, cgDestroyProgram
 */
CG_API CGprogram cgCopyProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgCopyProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgCopyProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check input parameter

    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return NULL;
    }
    _CGprogram* prog = _cgGetProgPtr( program );
    if ( NULL == prog )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return ( CGprogram )NULL;
    }
    
    _CGprogram* newprog;
    size_t paddedProgramSize = 0;
    size_t ucodeSize = 0;

    if (prog->header.profile == CG_PROFILE_SCE_FP_TYPEB || prog->header.profile == CG_PROFILE_SCE_FP_RSX)
	{
	    paddedProgramSize = _jsPad( sizeof( _CGprogram ), 16);
	    ucodeSize = prog->header.instructionCount * 16;
	    newprog = ( _CGprogram* )jsMalloc( paddedProgramSize + ucodeSize );
	}
    else
	{
	    newprog = ( _CGprogram* )jsMalloc( sizeof( _CGprogram ) );
	}

    if ( NULL == newprog )
    {
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        return ( CGprogram )NULL;
    }
    _jsCgProgramZero( newprog );

    // copy information from the old program
    newprog->header.profile = prog->header.profile;
    newprog->parentContext = prog->parentContext;

    // generate a new id
    newprog->id = ( CGprogram )_jsCreateName( &_CurrentContext->cgProgramNameSpace, newprog );

    // copy all the parameter information


    // copy the binary program information
    // TODO ******** copy the entire elf here, not just the executable.
    int success = 0;
    switch ( prog->header.profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            success = _jsPlatformCopyProgram( prog, newprog );
            break;
        default:
            _jsCgRaiseError( CG_UNKNOWN_PROFILE_ERROR );
            success = 0;
            break;
    }

    if ( success == 0 )
    {
        // we failed to create a new program object, clean up
        jsFree( newprog );
        _jsEraseName( &_CurrentContext->cgProgramNameSpace, ( jsName )newprog->id );
        return ( CGprogram )NULL;
    }

    if (prog->header.profile == CG_PROFILE_SCE_FP_TYPEB || prog->header.profile == CG_PROFILE_SCE_FP_RSX)
	{
	    newprog->ucode = (char*)newprog + paddedProgramSize;
	    memcpy((char*)newprog->ucode, (char*)prog->ucode, ucodeSize);
	}

    //handle refcounting for string table and default values
    if ( prog->programGroup )
    {
        //refcount for the string and the default table
        newprog->programGroup = prog->programGroup;
        newprog->programIndexInGroup = -1;
        _jsCgUpdateProgramAtIndex( newprog->programGroup, -1, 1 );
    }

    // add the new program object to the program list in the context
    _jsCgProgramPushFront( newprog->parentContext, newprog );

    if ( _cgProgramCopyHook ) _cgProgramCopyHook( newprog, prog );

    return newprog->id;
}


/**
@short Delete a program object

cgDestroyProgram() removes the specified program object and all its associated
data. Any <CODE>CGprogram</CODE> variables that reference this program object
will become invalid after the program is deleted.  Likewise, any
objects contained by this program (e.g. <CODE>GLparameter</CODE> objects) will also
become invalid after the program is deleted.

@param       program    Specifies the program object to delete.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is an invalid program handle.</TD>
</TR>
</TABLE>

@sa cgCreateProgram
 */
CG_API void cgDestroyProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgDestroyProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgDestroyProgram, JSDPM_CALL_TYPE_STATE_CG, , 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // remove the program from the program list

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
    _CGprogram* ptr = _cgGetProgPtr( program );
    if ( NULL == ptr )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }

    if ( ptr->programGroup )
    {
        if ( !ptr->programGroup->userCreated )
        {
            if ( ptr->programIndexInGroup != -1 && ptr->programGroup->programs[ptr->programIndexInGroup].refCount == 0 )
            {
                //not a valid handle, the refcount is 0, this handle has already been freed
                _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
                return;
            }
            else
            {
                bool isGroupMember = ( ptr->programIndexInGroup != -1 );
                _jsCgUpdateProgramAtIndex( ptr->programGroup, ptr->programIndexInGroup, -1 );
                if ( isGroupMember )
                    return;
                //else continue, it needs to be deleted here, the group won't delete it.
                //such program which have a programGroup but are not member of the group are
                //clone of a program coming from a group. but those program still need the group
                //because the group holds their stringtable and their default table.
            }
        }
    }

    // get the context that this program belongs to.
    _CGcontext* ctx = ptr->parentContext;

    // find and unlink the program from the program list
    if ( ptr == ctx->programList )
    {
        // node is the head of the list, so unlink it.
        _CGprogram* p = ctx->programList;
        ctx->programList = p->next;
        // erase the program
        _jsCgProgramEraseAndFree( p );
    }
    else
    {
        // node not the head, so use find_previous and delete_after
        // NOTE: if the program is not found in the list, returns silently.
        _CGprogram* p = _jsCgProgramFindPrev( ctx, ptr );
        _jsCgProgramEraseAfter( p );
    }
    return;
}


/**
@short Get the first program in a context

cgGetFirstProgram() is used to begin iteration over all of the programs
contained within a context.  See cgGetNextProgram() for more information.

@param       ctx     Specifies the context to retrieve the first program from.

@return      cgGetFirstProgram() returns the first <CODE>CGprogram</CODE> object in <I><c>ctx</c></I>.
If <I><c>ctx</c></I> contains no programs, <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>ctx</c></I> does not refer to a valid context.</TD>
</TR>
</TABLE>

@sa cgGetNextProgram, cgCreateProgram, cgDestroyProgram, cgIsProgram
*/
CG_API CGprogram cgGetFirstProgram( CGcontext ctx )
{
    JS_APIENTRY_TIMER( cgGetFirstProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetFirstProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    if ( !CG_IS_CONTEXT( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return ( CGprogram )NULL;
    }
    // check context
    _CGcontext* c = _cgGetContextPtr( ctx );

    // return the id of the head of the program list in the context (got all that? good)
    _CGprogram* ptr = c->programList;
    if ( ptr )
    {
        // if any programs have been allocated...
        return c->programList->id;
    }

    return ( CGprogram )NULL;
}

/**
@short Iterate through programs in a context

The programs within a context can be iterated over by using the cgGetNextProgram()
function.  The following example code illustrates one way to do this:

@code
CGprogram prog = cgGetFirstProgram( ctx );
while( prog )
{
   // do something with prog
   prog = cgGetNextProgram( prog )
}
@endcode

Note that no specific order of traversal is defined by this mechanism.
The only guarantee is that each program will be visited exactly once.
No guarantees can be made if programs are generated or deleted during
iteration.

@param       current    Specifies the program.

@return cgGetNextProgram() returns the next program in the context&rsquo;s internal sequence of programs.
Returns 0 when <I><c>current</c></I> is the last program in the context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>current</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgCreateProgram, cgDestroyProgram, cgIsProgram
*/
CG_API CGprogram cgGetNextProgram( CGprogram current )
{
    JS_APIENTRY_TIMER( cgGetNextProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNextProgram, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, current, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( current ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return NULL;
    }
    _CGprogram* ptr = _cgGetProgPtr( current );

    // increment the iterator down the program list
    if ( ptr->next != NULL )
    {
        return ptr->next->id;
    }

    // failed, so return an empty program
    return NULL;
}

/**
@short Get a program&rsquo;s parent context

cgGetProgramContext() allows the application to retrieve a handle to the
context a given program belongs to.

@param       prog    Specifies the program.

@return Returns a <CODE>CGcontext</CODE> handle to the parent context.
In the event of an error <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgCreateProgram, cgCreateContext
*/
CG_API CGcontext cgGetProgramContext( CGprogram prog )
{
    JS_APIENTRY_TIMER( cgGetProgramContext );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetProgramContext, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return NULL;
    }
    _CGprogram* ptr = _cgGetProgPtr( prog );

    return ptr->parentContext->id;
}

/**
@short Determine if a <CODE>CGprogram</CODE> handle references a Cg program object

cgIsProgram() returns <CODE>CG_TRUE</CODE> if <I><c>program</c></I> references a valid program object.
Note that this does not imply that the program has been successfully
compiled.

@param       program    Specifies the program handle to check.

@return Returns <CODE>CG_TRUE</CODE> if <I><c>program</c></I> references a valid program object.
@par
Returns <CODE>CG_FALSE</CODE> otherwise.

@sa cgCreateProgram, cgDestroyProgram, cgGetNextProgram
*/
CG_API CGbool cgIsProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgIsProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsProgram, JSDPM_CALL_TYPE_STATE_CG, 0, 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    if ( CG_IS_PROGRAM( program ) )
    {
        // the id was valid.
        return CG_TRUE;
    }
    // failed to find a valid id.
    return CG_FALSE;
}



/**
@short Compile a program object (Not implemented)

This function is not implemented in PSGL. See cgCreateProgram() for details on using the cgc run-time
compiler.

This function compiles the specified Cg program for its target profile. However, because the game console has a fixed profile and limited resources, runtime compilation is not recommended.

A program must be compiled before it can be loaded (by the API-specific
part of the runtime). It must also be compiled before its
parameters can be inspected.

Certain actions invalidate a compiled program and the current value of
all of its parameters.  If one of these actions is performed, the program
must be recompiled before it can be used.  A program is invalidated
if the program source is modified, if the compile arguments are
modified, or if the entry point is changed.

If one of the parameter bindings for a program is changed, that action
invalidates the compiled program, but does not invalidate the current
value of the program&rsquo;s parameters.

See cgIsProgramCompiled() for related information

@param       program    Specifies the program object to compile.

@par Examples
@code
if( ! cgIsProgramCompiled( program ) )
    cgCompileProgram( program );
@endcode

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is an invalid program handle.</TD>
</TR>
<TR>
<TD><CODE>CG_COMPILER_ERROR</CODE></TD>
<TD>The compile failed.</TD>
</TR>
</TABLE>

@sa cgCreateProgram, cgGetNextParameter, cgIsParameter, cgGetProgramString
*/
CG_API void cgCompileProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgCompileProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgCompileProgram, JSDPM_CALL_TYPE_STATE_CG, , 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }

    // TODO ****** use this function to re-link our program after creating parameter objects?

    return;
}

/**
@short Check if a program is compiled (Not implemented)

This function is not implemented in PSGL.

This function is used to determine whether a compile is necessary.
However, because the game console has a fixed profile and limited resources, runtime compilation is not recommended and all the Cg programs must be pre-compiled. This makes cgIsProgramCompiled() irrelevant for game development.
See cgCompileProgram() for related information

@param       program    Specifies the program object to inspect.

@return <CODE>CG_FALSE</CODE> if the program has never been compiled, or when the compiled program becomes invalid.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is an invalid program handle.</TD>
</TR>
</TABLE>

@sa cgCreateProgram, cgGetNextParameter, cgIsParameter, cgGetProgramString
*/
CG_API CGbool cgIsProgramCompiled( CGprogram program )
{
    JS_APIENTRY_TIMER( cgIsProgramCompiled );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsProgramCompiled, JSDPM_CALL_TYPE_STATE_CG, 0, 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_FALSE;
    }

    // TODO ********** use this function to find out if our program has unresolved symbols?

    return CG_TRUE;
}

/**
@short Get strings from a program object

cgGetProgramString() allows the application to retrieve program strings
that have been set via functions that modify program state.

@note
In the current software version, only <CODE>CG_PROGRAM_PROFILE</CODE> is implemented.

@param       prog    Specifies the Cg program to get strings from.

@param       pname   Specifies the string to retrieve. <I><c>pname</c></I> can be one of
<CODE>CG_PROGRAM_SOURCE</CODE>, <CODE>CG_PROGRAM_ENTRY</CODE>,
<CODE>CG_PROGRAM_PROFILE</CODE>, <CODE>CG_COMPILED_PROGRAM</CODE>, or <CODE>CG_BINARY</CODE>.  In the
current software version, only <CODE>CG_PROGRAM_PROFILE</CODE> is implemented.

@return When <I><c>pname</c></I> is <CODE>CG_PROGRAM_SOURCE</CODE>, the original Cg source program is returned.
@par 
When <I><c>pname</c></I> is <CODE>CG_PROGRAM_ENTRY</CODE>, the main entry point for the program is returned.
@par 
When <I><c>pname</c></I> is <CODE>CG_PROGRAM_PROFILE</CODE>, the profile for the program is returned.
@par 
When <I><c>pname</c></I> is <CODE>CG_COMPILED_PROGRAM</CODE>, the string for the compiled program is returned.
@par 
When <I><c>pname</c></I> is <CODE>CG_PROGRAM_FILENAME</CODE>, the filename for the compiled binary is returned.
@par 
When <I><c>pname</c></I> is <CODE>CG_BINARY</CODE>, the string for the compiled binary is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>pname</c></I> is an invalid enumerant.</TD>
</TR>
</TABLE>

@sa cgCompileProgram
*/
CG_API const char* cgGetProgramString( CGprogram prog, CGenum pname )
{
    JS_APIENTRY_TIMER( cgGetProgramString );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetProgramString, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pname, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return NULL;
    }

    //hack to counter change of defines for program_type at r5294
    // previously CG_PROGRAM_FILENAME was defined the same as CG_COLUMN_MAJOR
    // if those values are passed in here, move them to the new values and remove this hack after we have
    // an sdk that incorporates these changes so that prebuild libs (aka debugfont) can be used meanwhile
    if ( pname == CG_COLUMN_MAJOR )
        pname = CG_PROGRAM_FILENAME;

    switch ( pname )
    {
        case CG_PROGRAM_SOURCE:
            // the original Cg source program is returned
            // all programs in our API are pre-compiled and come without source.
            return NULL;

        case CG_PROGRAM_ENTRY:
            // the main entry point for the program is returned
            // TODO *********** return the name of the entry point
            return NULL;

        case CG_PROGRAM_PROFILE:
            // the profile for the program is returned
        {
            _JS_DPM_TRACE_OFF // get profilestring
            const char *result = cgGetProfileString(( CGprofile )_cgGetProgPtr( prog )->header.profile );
            _JS_DPM_TRACE_ON
            return result;
        }

        case CG_COMPILED_PROGRAM:
            // the string for the object program is returned
            return NULL;

        case CG_PROGRAM_FILENAME:
            // TODO ***************
            // Return the filename of the source ELF this program came from
            return ( char* )_cgGetProgPtr( prog )->platformProgram;

        case CG_BINARY:
            // TODO ***************
            // Create a whole new function for handling binaries - return charand length.
            // we'll return an image pointer for now...
            return ( char* )_cgGetProgPtr( prog )->platformProgram;

        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
    }

    return NULL;
}


/**
@short Set the listing text from the last compile (not implemented)

cgSetLastListing() is not implemented in PSGL.

@param       handle     Specifies the program or effect to set
@param		 listing    pointer to the text to be used

@sa       cgGetLastListing, cgCreateContext, cgCreateProgram
 */

CG_API void CGENTRY cgSetLastListing( CGhandle handle, const char *listing )
{
    JS_APIENTRY_TIMER( cgSetLastListing );

    // NOTE: unsupported by Jetstream

    // TODO: ************************************
    return;
}



/**
@short Get a program parameter&rsquo;s resource

cgGetProgramProfile() allows the application to retrieve the profile enumerant
associated with the profile of the program.

@param       prog    Specifies the program.

@return Returns the profile enumerant of <I><c>prog</c></I>.
Returns <CODE>CG_PROFILE_UNKNOWN</CODE> if compilation has failed.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgGetProfile, cgGetProfileString
*/
CG_API CGprofile cgGetProgramProfile( CGprogram prog )
{
    JS_APIENTRY_TIMER( cgGetProgramProfile );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGetProgramProfile, JSDPM_CALL_TYPE_STATE_CG, CG_PROFILE_UNKNOWN, 1,
                             prog, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( prog ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_PROFILE_UNKNOWN;
    }

    // return the profile the program was compiled under
    return ( CGprofile )_cgGetProgPtr( prog )->header.profile;
}

/**
 * @}
 */


/**
@short Get the number of domains used in a GLSL program

cgGetNumProgramDomains() return the number of domains that a GLSL program
uses in its definition, where a domain represents a subsection of the
graphics pipeline (either vertex, fragment or geometry program).

@param       program    Specifies the program.

@return Returns the number of domains touched by this program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>
*/
CG_API int cgGetNumProgramDomains( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGetNumProgramDomains );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGetNumProgramDomains, JSDPM_CALL_TYPE_STATE_CG, NULL, 1,
                             program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_PROFILE_UNKNOWN;
    }
    // under Jetstream, unlike GLSL, all programs have a single domain.
    return 1;
}



/**
@short Combine any number of GLSL programs into a single executable

cgCombinePrograms() allows the linking of many GLSL programs into a
single executable.

@param         n  A count of the number of programs to be linked together
@param   exeList  A pointer to an array of <i><c>n</c></i> program handles.

@return Returns a handle to a new program that contains the linked programs.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>exeList</c></I> contains a program handle that does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgCombinePrograms2, cgCombinePrograms3
*/
CG_API CGprogram cgCombinePrograms( int n, const CGprogram *exeList )
{
//	JS_APIENTRY_TIMER(cgCombinePrograms);
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgCombinePrograms, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             n, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             exeList, sizeof( CGprogram* ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif // JS_DPM

    // jetstream does not support combination of GLSL programs.
    return 0;
}



/**
@short Combine two GLSL programs into a single executable program

cgCombinePrograms2() allows the linking of exactly two GLSL programs
into a single executable.

@param       exe1     Specifies the first program.
@param       exe2     Specifies the second program.

@return Returns a handle to a new program that contains the
two programs linked.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>exe1</c></I> or <i><c>exe2</c></i> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgCombinePrograms, cgCombinePrograms3
*/

CG_API CGprogram cgCombinePrograms2( const CGprogram exe1, const CGprogram exe2 )
{
//	JS_APIENTRY_TIMER(cgCombinePrograms2);
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgCombinePrograms2, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
                             exe1, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             exe2, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // jetstream does not support combination of GLSL programs.
    return 0;
}



/**
@short Combine three GLSL programs into a single executable program

cgCombinePrograms3() allows the linking of exactly three GLSL programs
into a single executable.

@param       exe1     Specifies the first program.
@param       exe2     Specifies the second program.
@param       exe3     Specifies the third program.

@return Returns a handle to a new program that contains the
three programs linked.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>exe1</c></I>, <i><c>exe2</c></i>, or <i><c>exe3</c></i> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgCombinePrograms, cgCombinePrograms2
*/

CG_API CGprogram cgCombinePrograms3( const CGprogram exe1, const CGprogram exe2, const CGprogram exe3 )
{
//	JS_APIENTRY_TIMER(cgCombinePrograms3);
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgCombinePrograms3, JSDPM_CALL_TYPE_STATE_CG, NULL, 3,
                             exe1, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             exe2, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             exe3, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif // JS_DPM

    // jetstream does not support combination of GLSL programs.
    return 0;
}



/**
@short For each domain in a GLSL program, return the profile under which
the program was compiled.

cgGetProgramDomainProfile() allows applications to iterate through the domains
in a GLSL program and inspect the profile under which each domain program
was compiled.

@param       program    Specifies the program.
@param       index      Specifies the program domain

@return Returns the profile under which the domain program was compiled.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> does not refer to a valid program.</TD>
</TR>
</TABLE>

@sa cgGetNumProgramDomains
*/

CG_API CGprofile cgGetProgramDomainProfile( CGprogram program, int index )
{
    JS_APIENTRY_TIMER( cgGetProgramDomainProfile );
#ifdef JS_DPM
    /*	if (_jsdpmState) {
    		_JS_DPM_API_ENTRY_2(cgGetProgramDomainProfile, JSDPM_CALL_TYPE_STATE_CG, NULL, 2,
    							program, sizeof(CGprogram),INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
    							index, sizeof(int), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE); }*/
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_PROFILE_UNKNOWN;
    }

    if ( index >= 1 )
    {
        // jetstream programs can only have a single domain
        return CG_PROFILE_UNKNOWN;
    }

    // return the single profile under which the shader was compiled.
    return ( CGprofile )_cgGetProgPtr( program )->header.profile;
}

/** @addtogroup Cg
 *@{
 */
/**

@short Get compiler options for a <code>CGprogram</code> (not implemented)

cgGetProgramOptions() is not implemented in PSGL.

Get the compiler options that a <code>CGprogram</code> was compiled with.

@param       program    Program to get options for

@return Pointer to a zero-terminated string of compiler options.
 */
CG_API char const * const * cgGetProgramOptions( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGetProgramOptions );
#ifdef JS_DPM
    /*	if (_jsdpmState) {
    		_JS_DPM_API_ENTRY_1(cgGetProgramOptions, JSDPM_CALL_TYPE_STATE_CG, NULL, 1,
    							program, sizeof(CGprogram),INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE); }*/
#endif // JS_DPM

    // check the program input
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return NULL;
    }

    // NOTE: currently unsupported by Jetstream precompiled programs
    // TODO: get program options from ".note.MyShader" section of CG ELF Binary
    // or from compiler arguments of a runtime-compiled program.
    return NULL;
}


/**
 * @}
 */

/////////////////////////////////////////////
// program groups

static CGprogram _jsCgUpdateProgramAtIndex( CGprogramGroup group, int index, int refcount )
{
    if ( index < ( int )group->programCount )
    {
        //index can be < 0 , in that case refcount update on the group only, used when destroying a copied program
        if ( index >= 0 )
        {
            //if it has already been referenced duplicate instead of returning the same index, until the API offer a native support for
            //group of programs ( //fixed bug 13007 )
            if ( refcount == 1 && group->programs[index].refCount == 1 )
            {
                //it will handle the refcounting
                CGprogram res = cgCopyProgram( group->programs[index].program );
                return res;
            }
            group->programs[index].refCount += refcount;
        }

        group->refCount += refcount;
        JS_ASSERT( group->refCount >= 0 );
        if ( refcount < 0 )
        {
            if ( group->refCount == 0 && !group->userCreated )
            {
                _jsCgDestroyProgramGroup( group );
            }
            return NULL;
        }
        else
            return group->programs[index].program;
    }
    else
        return NULL;
}

//add the group to the context:
static void _jsCgAddGroup( CGcontext ctx, CGprogramGroup group )
{
    _CGcontext *context = _cgGetContextPtr( ctx );
    if ( !context->groupList )
        context->groupList = group;
    else
    {
        _CGprogramGroup *current = context->groupList;
        while ( current->next )
            current = current->next;
        current->next = group;
    }
}

static void _jsCgRemoveGroup( CGcontext ctx, CGprogramGroup group )
{
    _CGcontext *context = _cgGetContextPtr( ctx );
    _CGprogramGroup *current = context->groupList;
    _CGprogramGroup *previous = NULL;
    while ( current && current != group )
    {
        previous = current;
        current = current->next;
    }
    if ( current )
    {
        if ( !previous )
            context->groupList = current->next;
        else
            previous->next = current->next;
    }
}

CGprogramGroup _jsCgCreateProgramGroupFromFile( CGcontext ctx, const char *group_file )
{
    // check that file exists
    FILE* fp = fopen( group_file, "rb" );

    if ( NULL == fp )
    {
        _jsCgRaiseError( CG_FILE_READ_ERROR );
        return ( CGprogramGroup )NULL;
    }

    // find the file length
    size_t file_size = 0;
    fseek( fp, 0, SEEK_END );
    file_size = ftell( fp );
    rewind( fp );

    // alloc memory for new binary program and read the data
    char* ptr = ( char* )jsMalloc( file_size + 1 );
    if ( NULL == ptr )
    {
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        return ( CGprogramGroup )NULL;
    }

    // read the entire file into memory then close the file
    // TODO ********* just loading the file is a bit lame really. We can do better.
    fread( ptr, file_size, 1, fp );
    fclose( fp );

    CGprogramGroup group = _jsCgCreateProgramGroup( ctx, group_file, ptr, file_size );
    if ( !group )
        jsFree( ptr );

    return group;
}

CGprogramGroup _jsCgCreateProgramGroup( CGcontext ctx,  const char *name, void *ptr, int size )
{
    _CGprogramGroup *group = NULL;
    CGELFBinary elfBinary;
    elfBinary.elfFile = NULL;

    while ( 1 )
    {
        bool res = cgOpenElf( ptr, size, &elfBinary );
        if ( !res )
            break;

        size_t elfConstTableSize = ( size_t )elfBinary.consttabSize;
        size_t elfStringTableSize = ( size_t )elfBinary.strtabSize;

        //first pass to get the size of each item ( could be faster if the embedded constants index table size was in the header
        int programCount = elfBinary.shadertabSize / sizeof( CgProgramHeader );
        int i;

        //structure of the memory buffer storing the group
        size_t nvProgramNamesOffset = _jsPad( sizeof( _CGprogramGroup ), sizeof( _CGnamedProgram ) ); //program name offset
        size_t nvDefaultValuesTableOffset = _jsPad( nvProgramNamesOffset + programCount * sizeof( _CGnamedProgram ), 16 );//shared default value table

        size_t nvStringTableOffset = nvDefaultValuesTableOffset + elfConstTableSize; //shared string table
        size_t structureSize = nvStringTableOffset + elfStringTableSize;//total structure size

        //create the program group
        group = ( CGprogramGroup )jsMalloc( structureSize );
        if ( !group ) //out of memory
            break;

        //fill the group structure
        group->ctx = ctx;
        group->next = NULL;
        group->programCount = ( unsigned int )programCount;
        group->constantTable = ( unsigned int * )(( char* )group + nvDefaultValuesTableOffset );
        JS_ASSERT((( unsigned int )( group->constantTable )&15 ) == 0 ); //aligned on 16 bytes
        group->stringTable = ( unsigned int * )(( char* )group + nvStringTableOffset );
        group->programs = ( _CGnamedProgram * )(( char* )group + nvProgramNamesOffset );
        group->userCreated = true;
        group->refCount = 0;
        group->filedata = ( char* )ptr;
        if ( name )
        {
            int len = strlen( name );
            group->name = ( char* )jsMalloc( len + 1 );
            if ( !group->name )//out of memory
                break;
            strcpy( group->name, name );
        }
        else
            group->name = NULL;

        //copy the default values
        if ( elfConstTableSize )
            memcpy(( char* )group + nvDefaultValuesTableOffset, elfBinary.consttab, elfConstTableSize );
        //copy the string table
        if ( elfStringTableSize )
            memcpy(( char* )group + nvStringTableOffset, elfBinary.strtab, elfStringTableSize );

        //add the group to the context:
        _jsCgAddGroup( ctx, group );

        //create all the shaders contained in the package and add them to the group
        for ( i = 0;i < ( int )group->programCount;i++ )
        {
            CgProgramHeader *cgShader = ( CgProgramHeader* )elfBinary.shadertab + i;

            //hack to counter removal of TypeC during beta
            if ( cgShader->profile == ( CGprofile )7005 )
                cgShader->profile = CG_PROFILE_SCE_VP_RSX;
            if ( cgShader->profile == ( CGprofile )7006 )
                cgShader->profile = CG_PROFILE_SCE_FP_RSX;

            CGELFProgram elfProgram;
            bool res = cgGetElfProgramByIndex( &elfBinary, i, &elfProgram );
            if ( !res )
                return false;

            //I reference the buffer passed as parameter here, so it will have to stay around
            CgProgramHeader *programHeader = cgShader;
            char *ucode = ( char * )elfProgram.texttab;
            CgParameterTableHeader *parameterHeader  = ( CgParameterTableHeader * )elfProgram.paramtab;

#ifdef JS_DEBUG
            //make sure we have consistent info
            size_t actualUcodeSize = programHeader->instructionCount * 16;
            JS_ASSERT(( size_t )elfProgram.texttabSize == actualUcodeSize );
            size_t actualParameterSize = parameterHeader->semanticIndexTableOffset + parameterHeader->semanticIndexCount * sizeof( CgParameterSemantic );
            JS_ASSERT(( size_t )elfProgram.paramtabSize == actualParameterSize );
#endif

            const char *programName = getSymbolByIndexInPlace( elfBinary.symtab, elfBinary.symbolSize, elfBinary.symbolCount, elfBinary.symbolstrtab, i + 1 );
            group->programs[i].name = programName;
            group->programs[i].program = _jsCgCreateProgram( ctx, ( CGprofile )cgShader->profile, programHeader, ucode, parameterHeader, ( const char* )group->stringTable, ( const float* )group->constantTable );
            JS_ASSERT( group->programs[i].program && "corrupted CG binary data" );
            _CGprogram *cgProgram = _cgGetProgPtr( group->programs[i].program );
            cgProgram->programGroup = group;
            cgProgram->programIndexInGroup = i;
            group->programs[i].refCount = 0;
        }
        break;
    }

    cgCloseElf( &elfBinary );

    return group;
}

void _jsCgDestroyProgramGroup( CGprogramGroup group )
{
    _CGprogramGroup *_group = ( _CGprogramGroup * )group;
    for ( int i = 0;i < ( int )_group->programCount;i++ )
    {
        JS_ASSERT( _group->programs[i].refCount == 0 );
        //unlink the program
        _CGprogram *cgProgram = _cgGetProgPtr( group->programs[i].program );
        cgProgram->programGroup = NULL;
        cgDestroyProgram( _group->programs[i].program );
    }
    jsFree( _group->filedata );
    if ( _group->name )
        jsFree( _group->name );

    //remove the group from the group list
    _jsCgRemoveGroup( group->ctx, group );
    jsFree( _group );
}

const char *_jsCgGetProgramGroupName( CGprogramGroup group )
{
    _CGprogramGroup *_group = ( _CGprogramGroup * )group;
    return _group->name;
}

int _jsCgGetProgramIndex( CGprogramGroup group, const char *name )
{
    int i;
    for ( i = 0;i < ( int )group->programCount;i++ )
    {
        if ( !strcmp( name, group->programs[i].name ) )
            return i;
    }
    return -1;
}

CGprogram _jsCgGetProgramAtIndex( CGprogramGroup group, unsigned int index )
{
    return _jsCgUpdateProgramAtIndex( group, index, 0 );
}

int _jsCgGetProgramCount( CGprogramGroup group )
{
    return group->programCount;
}


// ------------------------------------------------------

