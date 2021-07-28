/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Any value helper class
 */

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <PSGLFX/Base.h>
#include <PSGL/private.h>
#include <Cg/CgCommon.h>

namespace PSGLFX
{
class Effect;
class Technique;
class Annotation;
class Pass;
class StateAssignment;
class State;

class GLContextExtension: public PSGL::Base
{
    jsNameSpace  effectNameSpace;
    jsNameSpace  techniqueNameSpace;
    jsNameSpace  passNameSpace;
    jsNameSpace  stateAssignmentNameSpace;
    jsNameSpace  stateNameSpace;
    jsNameSpace  annotationNameSpace;

    jsName addEffect( Effect *effect ) { return _jsCreateName( &effectNameSpace, effect ); }
    void delEffect( jsName id ) { return _jsEraseName( &effectNameSpace, id ); }

    jsName addTechnique( Technique *technique ) { return _jsCreateName( &techniqueNameSpace, technique ); }
    void delTechnique( jsName id ) { return _jsEraseName( &techniqueNameSpace, id ); }

    jsName addAnnotation( Annotation *annotation ) { return _jsCreateName( &annotationNameSpace, annotation ); }
    void delAnnotation( jsName id ) { return _jsEraseName( &annotationNameSpace, id ); }

    jsName addPass( Pass *pass ) { return _jsCreateName( &passNameSpace, pass ); }
    void delPass( jsName id ) { return _jsEraseName( &passNameSpace, id ); }

    jsName addStateAssignment( StateAssignment *stateAssignment ) { return _jsCreateName( &stateAssignmentNameSpace, stateAssignment ); }
    void delStateAssignment( jsName id ) { return _jsEraseName( &stateAssignmentNameSpace, id ); }

    jsName addState( State *state ) { return _jsCreateName( &stateNameSpace, state ); }
    void delState( jsName id ) { return _jsEraseName( &stateNameSpace, id ); }

public:

    GLContextExtension();
    ~GLContextExtension();

    static GLContextExtension *fromContext( PSGLcontext *ctx ) { return static_cast<GLContextExtension *>( ctx->psglFXContextExtension );}
    static GLContextExtension *getCurrent() { return fromContext( ::_CurrentContext );}

    Effect *getEffect( CGeffect id ) { return static_cast<Effect *>( _jsGetNamedValue( &effectNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isEffect( CGeffect id ) { return _jsIsName( &effectNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    Technique *getTechnique( CGtechnique id ) { return static_cast<Technique *>( _jsGetNamedValue( &techniqueNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isTechnique( CGtechnique id ) { return _jsIsName( &techniqueNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    Annotation *getAnnotation( CGannotation id ) { return static_cast<Annotation *>( _jsGetNamedValue( &annotationNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isAnnotation( CGannotation id ) { return _jsIsName( &annotationNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    Pass *getPass( CGpass id ) { return static_cast<Pass *>( _jsGetNamedValue( &passNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isPass( CGpass id ) { return _jsIsName( &passNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    StateAssignment *getStateAssignment( CGstateassignment id ) { return static_cast<StateAssignment *>( _jsGetNamedValue( &stateAssignmentNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isStateAssignment( CGstateassignment id ) { return _jsIsName( &stateAssignmentNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    State *getState( CGstate id ) { return static_cast<State *>( _jsGetNamedValue( &stateNameSpace, reinterpret_cast<jsName>( id ) ) ); }
    bool isState( CGstate id ) { return _jsIsName( &stateNameSpace, reinterpret_cast<jsName>( id ) ) != 0; }

    friend class Effect;
    friend class Technique;
    friend class Pass;
    friend class Annotation;
    friend class StateAssignment;
    friend class State;
};

static inline bool CG_IS_EFFECT( CGeffect effect ) { return GLContextExtension::getCurrent()->isEffect( effect ); }
static inline PSGLFX::Effect *CG_GET_EFFECT( CGeffect effect ) { return GLContextExtension::getCurrent()->getEffect( effect ); }

static inline bool CG_IS_TECHNIQUE( CGtechnique technique ) { return GLContextExtension::getCurrent()->isTechnique( technique ); }
static inline PSGLFX::Technique *CG_GET_TECHNIQUE( CGtechnique technique ) { return GLContextExtension::getCurrent()->getTechnique( technique ); }

static inline bool CG_IS_ANNOTATION( CGannotation annotation ) { return GLContextExtension::getCurrent()->isAnnotation( annotation ); }
static inline PSGLFX::Annotation *CG_GET_ANNOTATION( CGannotation annotation ) { return GLContextExtension::getCurrent()->getAnnotation( annotation ); }

static inline bool CG_IS_PASS( CGpass pass ) { return GLContextExtension::getCurrent()->isPass( pass ); }
static inline PSGLFX::Pass *CG_GET_PASS( CGpass pass ) { return GLContextExtension::getCurrent()->getPass( pass ); }

static inline bool CG_IS_STATE_ASSIGNMENT( CGstateassignment stateAssignment ) { return GLContextExtension::getCurrent()->isStateAssignment( stateAssignment ); }
static inline PSGLFX::StateAssignment *CG_GET_STATE_ASSIGNMENT( CGstateassignment stateAssignment ) { return GLContextExtension::getCurrent()->getStateAssignment( stateAssignment ); }

static inline bool CG_IS_STATE( CGstate state ) { return GLContextExtension::getCurrent()->isState( state ); }
static inline PSGLFX::State *CG_GET_STATE( CGstate state ) { return GLContextExtension::getCurrent()->getState( state ); }

class CgContextExtension: public PSGL::Base
{
    Effect * effects;
    State * states;
    State * samplerStates;

public:
    CgContextExtension();
    ~CgContextExtension();

    inline Effect *getFirstEffect() const { return effects; }
    void appendEffect( Effect *effect );
    void removeEffect( Effect *effect );

    inline State *getFirstState() const { return states; }
    void appendState( State *state );

    inline State *getFirstSamplerState() const { return samplerStates; }
    void appendSamplerState( State *state );

    static inline CgContextExtension *fromContext( _CGcontext *ctx ) { return static_cast<CgContextExtension *>( ctx->FXcontextExtension );}
};

}

#endif
