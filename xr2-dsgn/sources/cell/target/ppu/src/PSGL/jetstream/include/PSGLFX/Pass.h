/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Passes
 */

#ifndef _PASS_H_
#define _PASS_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/Base.h>

namespace PSGLFX
{

class Technique;
class StateAssignment;
class Annotation;

class Pass: public Named
{
    Technique *parent;

    jsName id;
    Pass *next;

    StateAssignment *stateAssignments;
    Annotation *annotations;

    bool validated;

public:
    Pass( Technique *parent, const char *n );
    ~Pass();

    CGpass getId() const { return reinterpret_cast<CGpass>( id ); }

    inline Technique *getParent() const { return parent; }

    inline Pass *getNext() const { return next; }
    void append( Pass *other );

    inline StateAssignment *getFirstStateAssignment() const { return stateAssignments; }
    void appendStateAssignment( StateAssignment *stateAssignment );

    inline Annotation *getFirstAnnotation() const { return annotations; }
    void appendAnnotation( Annotation *annotation );

    bool validate();
    void invalidate();
    bool isValidated();

    void setState();
    void resetState();
};

}

#endif
