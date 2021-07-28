/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Context, library initialization
 */


#include <PSGL/private.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Effect.h>
#include <PSGLFX/State.h>
#include <PSGLFX/Parameter.h>
#include <PSGLFX/Program.h>
#include <Cg/CgCommon.h>

namespace PSGLFX
{

GLContextExtension::GLContextExtension()
{
    _jsInitNameSpace( &effectNameSpace );
    _jsInitNameSpace( &techniqueNameSpace );
    _jsInitNameSpace( &passNameSpace );
    _jsInitNameSpace( &stateAssignmentNameSpace );
    _jsInitNameSpace( &stateNameSpace );
    _jsInitNameSpace( &annotationNameSpace );
}

GLContextExtension::~GLContextExtension()
{
    _jsFreeNameSpace( &effectNameSpace );
    _jsFreeNameSpace( &techniqueNameSpace );
    _jsFreeNameSpace( &passNameSpace );
    _jsFreeNameSpace( &stateAssignmentNameSpace );
    _jsFreeNameSpace( &stateNameSpace );
    _jsFreeNameSpace( &annotationNameSpace );
}

CgContextExtension::CgContextExtension():
        effects( 0 ),
        states( 0 ),
        samplerStates( 0 )
{}

CgContextExtension::~CgContextExtension()
{
    delete effects;
    if ( states )
    {
        states->removeAll();
        delete states;
    }
    if ( samplerStates )
    {
        samplerStates->removeAll();
        delete samplerStates;
    }
}

void CgContextExtension::appendEffect( Effect *effect )
{
    if ( effects ) effects->append( effect );
    else effects = effect;
}

void CgContextExtension::removeEffect( Effect *effect )
{
    JS_ASSERT( effect );
    if ( effects == effect )
    {
        effects = effect->getNext();
        effect->unlink();
        delete effect;
    }
    else if ( effects ) effects->remove( effect );
}

void CgContextExtension::appendState( State *state )
{
    if ( states ) states->append( state );
    else states = state;
}

void CgContextExtension::appendSamplerState( State *state )
{
    if ( samplerStates ) samplerStates->append( state );
    else samplerStates = state;
}

JScontextHookFunction glContextCreateHookSave = NULL;
JScontextHookFunction glContextDestroyHookSave = NULL;
CgcontextHookFunction cgContextCreateHookSave = NULL;
CgcontextHookFunction cgContextDestroyHookSave = NULL;
CgparameterHookFunction cgParameterCreateHookSave = NULL;
CgparameterHookFunction cgParameterDestroyHookSave = NULL;
CgprogramHookFunction cgProgramCreateHookSave = NULL;
CgprogramHookFunction cgProgramDestroyHookSave = NULL;
CgprogramCopyHookFunction cgProgramCopyHookSave = NULL;

void glContextCreateHook( PSGLcontext *context )
{
    if ( glContextCreateHookSave ) glContextCreateHookSave( context );
    JS_ASSERT( !context->psglFXContextExtension );

    context->psglFXContextExtension = new GLContextExtension;
}

void glContextDestroyHook( PSGLcontext *context )
{
    JS_ASSERT( context->psglFXContextExtension );
    delete static_cast<GLContextExtension *>( context->psglFXContextExtension );
    context->psglFXContextExtension = 0;

    if ( glContextDestroyHookSave ) glContextDestroyHookSave( context );
}

void cgContextCreateHook( _CGcontext *context )
{
    if ( cgContextCreateHookSave ) cgContextCreateHookSave( context );

    JS_ASSERT( !context->FXcontextExtension );
    context->FXcontextExtension = new CgContextExtension;
}

void cgContextDestroyHook( _CGcontext *context )
{
    JS_ASSERT( context->FXcontextExtension );
    delete static_cast<CgContextExtension *>( context->FXcontextExtension );
    context->FXcontextExtension = 0;

    if ( cgContextDestroyHookSave ) cgContextDestroyHookSave( context );
}

void cgParameterCreateHook( CgRuntimeParameter *parameter )
{
    if ( cgParameterCreateHookSave ) cgParameterCreateHookSave( parameter );
    // the parameter extension is lasy-allocated to avoid bloat
}

void cgParameterDestroyHook( CgRuntimeParameter *parameter )
{
    JS_ASSERT( parameter->program );
    // search the program's fx extension table to find any fx extensions for this parameter
    STL_NAMESPACE vector<CgParameterFX>::iterator fxExtIter = parameter->program->fxParameterExtensions.begin();
    while ( fxExtIter != parameter->program->fxParameterExtensions.end() )
    {
        if (( *fxExtIter ).param == parameter )
        {
            // free the fx memory
            jsFree(( *fxExtIter ).fx );
            // remove the entry for this param from the table
            parameter->program->fxParameterExtensions.erase( fxExtIter );
            break;
        }
        fxExtIter++;
    }

    if ( cgParameterDestroyHookSave ) cgParameterDestroyHookSave( parameter );
}

void cgProgramCreateHook( _CGprogram *program )
{
    if ( cgProgramCreateHookSave ) cgProgramCreateHookSave( program );
    // the program extension is lasy-allocated to avoid bloat
}

void cgProgramDestroyHook( _CGprogram *program )
{
    if ( program->FXprogramExtension ) delete static_cast<CgProgramExtension *>( program->FXprogramExtension );
    program->FXprogramExtension = 0;

    // destroy program parameter extensions as well
    STL_NAMESPACE vector<CgParameterFX>::iterator fxExtIter = program->fxParameterExtensions.begin();
    while ( fxExtIter != program->fxParameterExtensions.end() )
    {
        delete static_cast<CgParameterExtension *>(( *fxExtIter ).fx );
        fxExtIter++;
    }
    program->fxParameterExtensions.clear();

    if ( cgProgramDestroyHookSave ) cgProgramDestroyHookSave( program );
}

void cgProgramCopyHook( _CGprogram *newprog, _CGprogram *oldprog )
{
    // copy program annotations
    if ( oldprog->FXprogramExtension ) newprog->FXprogramExtension = new CgProgramExtension( *static_cast<CgProgramExtension *>( oldprog->FXprogramExtension ) );

    // copy parameter annotations and state assignments
    newprog->fxParameterExtensions.reserve( oldprog->fxParameterExtensions.size() );
    STL_NAMESPACE vector<CgParameterFX>::iterator fxExtIter = oldprog->fxParameterExtensions.begin();
    while ( fxExtIter != oldprog->fxParameterExtensions.end() )
    {
        CgParameterFX newfx;
        newfx.param = ( *fxExtIter ).param;
        newfx.fx = new CgParameterExtension( *static_cast<CgParameterExtension *>(( *fxExtIter ).fx ) );
        newprog->fxParameterExtensions.push_back( newfx );
        fxExtIter++;
    }

    if ( cgProgramCopyHookSave ) cgProgramCopyHookSave( newprog, oldprog );
}

void Init()
{
	if ( _jsContextCreateHook == NULL )
	{
		glContextCreateHookSave =::_jsContextCreateHook;
		glContextDestroyHookSave =::_jsContextDestroyHook;
		cgContextCreateHookSave =::_cgContextCreateHook;
		cgContextDestroyHookSave =::_cgContextDestroyHook;
		cgParameterCreateHookSave =::_cgParameterCreateHook;
		cgParameterDestroyHookSave =::_cgParameterDestroyHook;
		cgProgramCreateHookSave =::_cgProgramCreateHook;
		cgProgramDestroyHookSave =::_cgProgramDestroyHook;
		cgProgramCopyHookSave =::_cgProgramCopyHook;

		::_jsContextCreateHook = &glContextCreateHook;
		::_jsContextDestroyHook = &glContextDestroyHook;
		::_cgContextCreateHook = &cgContextCreateHook;
		::_cgContextDestroyHook = &cgContextDestroyHook;
		::_cgParameterCreateHook = &cgParameterCreateHook;
		::_cgParameterDestroyHook = &cgParameterDestroyHook;
		::_cgProgramCreateHook = &cgProgramCreateHook;
		::_cgProgramDestroyHook = &cgProgramDestroyHook;
		::_cgProgramCopyHook = &cgProgramCopyHook;
	}
}

void Exit()
{
    ::_jsContextCreateHook = glContextCreateHookSave;
    ::_jsContextDestroyHook = glContextDestroyHookSave;
    ::_cgContextCreateHook = cgContextCreateHookSave;
    ::_cgContextDestroyHook = cgContextDestroyHookSave;
    ::_cgParameterCreateHook = cgParameterCreateHookSave;
    ::_cgParameterDestroyHook = cgParameterDestroyHookSave;
    ::_cgProgramCreateHook = cgProgramCreateHookSave;
    ::_cgProgramDestroyHook = cgProgramDestroyHookSave;
    ::_cgProgramCopyHook = cgProgramCopyHookSave;
}

}

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Initializes the PSGL FX library

psglFXInit() initializes the PSGL FX library. This needs to be called before any
PSGL context is created for PSGL FX to work properly.

@sa psglFXExit
 */
CGFX_API void psglFXInit()
{
    PSGLFX::Init();
}

/**
@short Terminates the PSGL FX library

psglFXExit() terminates the PSGL FX library. This must be called after all
PSGL contexts are destroyed.

@sa psglFXInit
 */
CGFX_API void psglFXExit()
{
    PSGLFX::Exit();
}
/**
 * @}
 */
// ------------------------------------------------------
