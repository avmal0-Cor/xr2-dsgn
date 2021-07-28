/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: Implementation of CGcontext class for Jetstream Cg Runtime

#define CG_LIB
#include <Cg/cg.h>
#include <stdio.h>
#include <stdlib.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>

#include <Cg/cgBinary.h>
#include <Cg/CgCommon.h>

PSGL_EXPORT CgcontextHookFunction _cgContextCreateHook = NULL;
PSGL_EXPORT CgcontextHookFunction _cgContextDestroyHook = NULL;

// externs ----------------------------------------------

#ifdef JS_DEBUG
extern void _jsCgProgramDump( CGprogram prog, unsigned int indent );
#endif

// helper functions -------------------------------------


#ifdef JS_DEBUG
void _jsCgContextDumpList( CGcontext root, unsigned int indent )
{
    _JS_DPM_TRACE_OFF //dump
    // print out every context and every program it contains.
    _CGcontext* ctx = _cgGetContextPtr( root );
    while ( ctx )
    {
        _jsCgPrintSpaces( indent );
        printf( "Context (%p) id=%p n_prog=%d compile=%s\n",
                ctx->id,
                ctx,
                ctx->programCount,
                cgGetEnumString( ctx->compileType ) );

        // loop over all programs in the list and dump them
        CGprogram prog = cgGetFirstProgram( root );
        while ( prog )
        {
            _jsCgProgramDump( prog, indent + 3 );
            prog = cgGetNextProgram( prog );
        }
        ctx = ctx->next;
    }
    printf( "---- done\n" );
    _JS_DPM_TRACE_ON
}
#endif

// linked list functions. Driver code needs to use the minimum storage,
// plus CGcontext is not going to be used many times, maybe once or twice a
// program. Therefore we use a singly linked list, no sentinel, not circular.
// Have to special case insertions and deletions to the head. Also have two
// find routines, one to find the node itself (check to see if the pointer
// is present in the global list, needed a lot for runtime checks) and one
// to find the previous node, allowing us to insert_after or erase_after.

void _jsCgContextZero( _CGcontext* p )
{
    memset( p, 0, sizeof( *p ) );
    p->compileType = CG_UNKNOWN;

}

void _jsCgContextPushFront( _CGcontext* ctx )
{
    if ( _CurrentContext->JScgContextHead )
    {
        _CGcontext* head = _cgGetContextPtr( _CurrentContext->JScgContextHead );
        // insert this context at the head of the list
        ctx->next = head;
    }
    _CurrentContext->JScgContextHead = ctx->id;
}

static void destroy_context( _CGcontext*ctx )
{
    if ( _cgContextDestroyHook ) _cgContextDestroyHook( ctx );
    // free the id
    _jsEraseName( &_CurrentContext->cgContextNameSpace, ( jsName )ctx->id );
    // zero the memory
    _jsCgContextZero( ctx );
    // return context to free store
    jsFree( ctx );
}

void _jsCgContextPopFront()
{
    // remove and delete the context at the head of the list
    if ( _CurrentContext->JScgContextHead )
    {
        _CGcontext* head = _cgGetContextPtr( _CurrentContext->JScgContextHead );
        _CGcontext* temp = head->next;
        // free the id as well
        destroy_context( head );

        if ( temp )
        {
            // this is not the end of the list, feel free to dereference it.
            _CurrentContext->JScgContextHead = temp->id;
        }
        else
        {
            // nothing left, no dereferenceing for you, mister.
            _CurrentContext->JScgContextHead = 0;
        }
    }
}


void _jsCgContextEraseAfter( _CGcontext* c )
{
    JS_ASSERT( c );
    _CGcontext* eraseme = c->next;
    JS_ASSERT( eraseme );
    c->next = eraseme->next;

    destroy_context( eraseme );
}

/**
 * @addtogroup Cg
 *
 * @{
 */

// API functions ----------------------------------------

/**
 * @short Creates a Cg context
 *
 cgCreateContext() creates a Cg context object and returns its handle.  A
 Cg context is a container for Cg programs.  All Cg programs must be
 added as part of a context.

 @return Returns a valid <CODE>CGcontext</CODE> on success.  <CODE>NULL</CODE> is returned if context creation fails.

 @par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_MEMORY_ALLOC_ERROR</CODE></TD>
<TD>A context could not be created.</TD>
</TR>
</TABLE>

 @sa cgDestroyContext
*/
CG_API CGcontext cgCreateContext( void )
{
    JS_APIENTRY_TIMER( cgCreateContext );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( cgCreateContext, JSDPM_CALL_TYPE_STATE_CG, 0 ); }
#endif
    // create a context out of thin air and add it to the hidden global list.

    _CGcontext* ptr = NULL;

    // alloc new context
    ptr = ( _CGcontext* )jsMalloc( sizeof( _CGcontext ) );
    if ( NULL == ptr )
    {
        _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
        return ( CGcontext )NULL;
    }

    // initialise member variables
    _jsCgContextZero( ptr );

    // get a new id for the object
    CGcontext result = ( CGcontext )_jsCreateName( &_CurrentContext->cgContextNameSpace, ptr );
    if ( !result ) { jsFree( ptr ); return NULL; }

    ptr->id = result;
    ptr->defaultProgram.parentContext = ptr;

    // insert context at head of global list
    _jsCgContextPushFront( ptr );

    if ( _cgContextCreateHook ) _cgContextCreateHook( ptr );

    return result;
}

/**
@short Delete a Cg context

cgDestroyContext() deletes a Cg context object and all the programs it contains.

@param c  Specifies the context to be deleted

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD>The <I><c>c</c></I> context handle is invalid.</TD>
</TR>
</TABLE>

@sa  cgCreateContext
*/
CG_API void cgDestroyContext( CGcontext c )
{
    JS_APIENTRY_TIMER( cgDestroyContext );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgDestroyContext, JSDPM_CALL_TYPE_STATE_CG, , 1, c, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif // JS_DPM

    // check if context is valid
    if ( !CG_IS_CONTEXT( c ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return;
    }

    _CGcontext* ctx = _cgGetContextPtr( c );

    // if we are really destroying the context, let's remove all the connections first.
    // if we don't do this first, I think the clean up gets grumpy.
    ctx->defaultProgram.connectionTable.clear();
    struct _CGprogram* programIter = ctx->programList;
    while ( programIter != NULL )
    {
        programIter->connectionTable.clear();
        programIter = programIter->next;
    }


    _jsCgProgramErase( &ctx->defaultProgram );

    // destroy all programs
    _jsCgProgramDestroyAll( ctx );

    // unlink from global CGContext list
    _CGcontext * const head = _cgGetContextPtr( _CurrentContext->JScgContextHead );
    if ( head != ctx )
    {
        // node is not the head, find and erase it

        // find the context that occurs before this one
        _CGcontext* ptr = head;
        while ( ptr->next != ctx ) ptr = ptr->next;
        // relink
        ptr->next = ctx->next;
        destroy_context( ctx );
    }
    else
    {

        // node is the head, erase it
        _CGcontext* second = head->next;
        destroy_context( head );

        if ( second )
        {
            // link to second element
            _CurrentContext->JScgContextHead = second->id;
        }
        else
        {
            // nothing left
            _CurrentContext->JScgContextHead = 0;
        }
    }
}

/**
@short Determine if a CGcontext references a Cg valid context

cgIsContext() determines if <I><c>ctx</c></I> is a valid Cg context object.

@param       ctx     Specifies the context handle to check.

@return <CODE>CG_TRUE</CODE> if <I><c>ctx</c></I> references a valid context, <CODE>CG_FALSE</CODE>
otherwise.

@sa       cgCreateContext, cgDestroyContext
*/
CG_API CGbool cgIsContext( CGcontext ctx )
{
    JS_APIENTRY_TIMER( cgIsContext );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsContext, JSDPM_CALL_TYPE_STATE_CG, 0, 1, ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif //JS_DPM

    // is the pointer valid?
    if ( CG_IS_CONTEXT( ctx ) )
    {
        return CG_TRUE;
    }
    return CG_FALSE;
}

/**
@short Get the listing text from the last compile (not implemented)

cgGetLastListing() is not implemented in PSGL.

Returns the listing (usually output to stdout and stderr) that you would usually get
from the command-line version of the Cg compiler.

<CODE>NULL</CODE> is returned if cgCreateProgram() was not called with the
context <i><c>c</c></i>.

@param       c     The context handle.

@return Returns the string containing the listing associated with the compile
from the last call to cgCreateProgram().

@sa       cgSetLastListing, cgCreateContext, cgCreateProgram
 */
CG_API const char* cgGetLastListing( CGcontext c )
{
    JS_APIENTRY_TIMER( cgGetLastListing );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetLastListing, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, c, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif //JS_DPM

    // check to see if context is a valid one
    // NOTE: Cg API Reference docs omit this test for this API function.
    if ( !CG_IS_CONTEXT( c ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return NULL;
    }

    // currently does nothing
    // TODO ****************************

    return NULL;
}


/**
@short Sets the auto-compile mode for a context (not implemented)

cgSetAutoCompile() is not implemented in PSGL.

cgSetAutoCompile() sets the auto compile mode for a given context.  By
default, programs are immediately recompiled when they enter an uncompiled state.
This may happen for a variety of reasons including :
<UL>
<LI>Setting the value of a literal parameter.</LI>
<LI>Resizing arrays.</LI>
<LI>Binding structs to interface parameters.</LI>
</UL>

<I><c>flag</c></I> may be one of the following three enumerants:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>CG_COMPILE_IMMEDIATE</CODE></TD>
 * <TD>
 * <CODE>CG_COMPILE_IMMEDIATE</CODE> will force recompilation automatically and
immediately when a program enters an uncompiled state.  This is the default mode.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>CG_COMPILE_MANUAL</CODE></TD>
 * <TD>
 * With this method the application is responsible for manually recompiling a program.
It may check to see if a program requires recompilation with the entry point cgIsProgramCompiled().
cgCompileProgram() can then be used to force compilation.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>CG_COMPILE_LAZY</CODE></TD>
 * <TD>
 *  This method is similar to <CODE>CG_COMPILE_IMMEDIATE</CODE> but will delay
program recompilation until the program object code is needed.  The
advantage of this method is the reduction of extraneous recompilations.
The disadvantage is that compile time Errors
Use cgGetError() to retrieve the value of the error flag. will not be
encountered when the program is enters the uncompiled state but
will instead be encountered at some later time.
 * </TD>
 * </TR>
 * </TABLE>

@param       c     Specifies the context.
@param       flag    The auto-compile mode to set the <I><c>c</c></I> to.  Must be one of the following :
@arg @c CG_COMPILE_MANUAL
@arg @c CG_COMPILE_IMMEDIATE
@arg @c CG_COMPILE_LAZY

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>c</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>flag</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCompileProgram, cgIsProgramCompiled
 */
CG_API void cgSetAutoCompile( CGcontext c, CGenum flag )
{
    JS_APIENTRY_TIMER( cgSetAutoCompile );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgSetAutoCompile, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             c, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             flag, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif //JS_DPM

    // check to see if context is a valid one
    if ( !CG_IS_CONTEXT( c ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return;
    }

    // check if enum has any meaning here
    switch ( flag )
    {
        case CG_COMPILE_MANUAL:
        case CG_COMPILE_IMMEDIATE:
        case CG_COMPILE_LAZY:
            // set the value and return
            _cgGetContextPtr( c )->compileType = flag;
            break;
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
    }
}
/**
 * @}
 */

// -----------------------------------------------------
