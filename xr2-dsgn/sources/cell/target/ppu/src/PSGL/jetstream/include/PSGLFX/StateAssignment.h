/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - State Assignments
 */

#ifndef _STATE_ASSIGNMENT_H_
#define _STATE_ASSIGNMENT_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/AnyValue.h>

namespace PSGLFX
{
class State;
class Pass;

class StateAssignment: public PSGL::Base
{
    Pass *parentPass;
    CgRuntimeParameter *parentParameter;

    jsName id;
    StateAssignment *next;

    State *state;

    AnyValue value;

    int arrayIndex;
    bool validated;

public:
    StateAssignment( Pass *p, State *s );
    StateAssignment( CgRuntimeParameter *p, State *s );
    ~StateAssignment();

    CGstateassignment getId() const { return reinterpret_cast<CGstateassignment>( id ); }

    inline Pass *getParentPass() const { return parentPass; }
    inline CgRuntimeParameter *getParentParameter() const { return parentParameter; }

    inline StateAssignment *getNext() const { return next; }
    void append( StateAssignment *other );

    inline State *getState() const { return state; }

    inline AnyValue &getValue() { return value; }
    inline int getArrayIndex() { return arrayIndex; }
    inline void setArrayIndex( int index ) { arrayIndex = index; }

    bool validate();
    void invalidate();
    inline bool isValidated() { return validated; }
};

}

#endif
