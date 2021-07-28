/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Techniques
 */

#ifndef _TECHNIQUE_H_
#define _TECHNIQUE_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/Base.h>

namespace PSGLFX
{

class Effect;
class Pass;
class Annotation;

class Technique: public Named
{
    Effect *parent;

    jsName id;
    Technique *next;

    Pass *passes;
    Annotation *annotations;

    bool validated;

public:
    Technique( Effect *effect, const char *n );
    ~Technique();

    CGtechnique getId() const { return reinterpret_cast<CGtechnique>( id ); }

    inline Effect *getParent() const { return parent; }

    inline Technique *getNext() const { return next; }
    void append( Technique *other );

    inline Pass *getFirstPass() const { return passes; }
    void appendPass( Pass *pass );

    inline Annotation *getFirstAnnotation() const { return annotations; }
    void appendAnnotation( Annotation *annotation );

    bool validate();
    void invalidate();
    bool isValidated();
};

}

#endif
