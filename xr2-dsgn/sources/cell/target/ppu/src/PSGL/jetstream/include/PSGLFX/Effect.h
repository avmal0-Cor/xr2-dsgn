/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Effects
 */

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/Base.h>

namespace PSGLFX
{

class Technique;
class Annotation;

class Effect: public Named
{
    _CGcontext *parent;

    jsName id;
    Effect *next;

    Technique *techniques;
    Annotation *annotations;
    // default program, fake owner of the effect parameters
    _CGprogram        defaultProgram;

public:
    Effect( _CGcontext *parent );
    ~Effect();

    inline CGeffect getId() const { return reinterpret_cast<CGeffect>( id ); }

    inline _CGcontext *getParent() const { return parent; }

    inline Effect *getNext() const { return next; }
    inline void unlink() { next = 0; }

    void append( Effect *other );
    void remove( Effect *other );
    void removeAll();

inline Technique *getFirstTechnique() const { return techniques; }
    void appendTechnique( Technique *technique );

    inline Annotation *getFirstAnnotation() const { return annotations; }
    void appendAnnotation( Annotation *annotation );

    inline _CGprogram *getDefaultProgram() { return &defaultProgram; }
};

}

#endif
