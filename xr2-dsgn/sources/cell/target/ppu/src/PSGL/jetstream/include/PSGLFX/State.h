/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - States
 */

#ifndef _STATE_H_
#define _STATE_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/Base.h>
#include <PSGLFX/AnyValue.h>
#include <PSGLFX/StateAssignment.h>
#include <string.h>

namespace PSGLFX
{

struct EnumMapEntry
{
    GLenum value;
    const char *name;
    ~EnumMapEntry() { freeString( name ); }
    EnumMapEntry(): value( 0 ), name( 0 ) {}
    EnumMapEntry( GLenum v, const char *n ): value( v ), name( makeString( n ) ) {}
    EnumMapEntry( const EnumMapEntry &other ): value( other.value ), name( makeString( other.name ) ) {}

    inline void set( GLenum v, const char *n )
    {
        value = v;
        freeString( name );
        name = makeString( n );
    }
};

class State: public Named
{
    _CGcontext *parent;

    jsName id;
    State *next;

    CGstatecallback setCallback;
    CGstatecallback resetCallback;
    CGstatecallback validateCallback;

    static bool defaultSetFunction( StateAssignment *sa );
    static bool defaultResetFunction( StateAssignment *sa );
    static bool defaultValidateFunction( StateAssignment *sa );
    static bool defaultNoopFunction( StateAssignment *sa );

    typedef bool( * UserFunction )( StateAssignment *sa );

    AnyValue::Type type;
    int elemCount;

    PSGL::Vector<EnumMapEntry> enumMap;

protected:
    UserFunction setFunction;
    UserFunction resetFunction;
    UserFunction validateFunction;

public:
    State( _CGcontext *parent, const char *n, AnyValue::Type type, int nelem = 1 );
    virtual ~State();

    inline void setCallbacks( CGstatecallback set, CGstatecallback reset, CGstatecallback validate )
    {
        setCallback = set;
        resetCallback = reset;
        validateCallback = validate;

        setFunction = set ? &State::defaultSetFunction : &State::defaultNoopFunction;
        resetFunction = reset ? &State::defaultResetFunction : &State::defaultNoopFunction;
        validateFunction = validate ? &State::defaultValidateFunction : &State::defaultNoopFunction;
    }

    inline void setUserFunctions( UserFunction set, UserFunction reset, UserFunction validate )
    {
        setFunction = set;
        resetFunction = reset;
        validateFunction = validate;
    }

CGstate getId() const { return reinterpret_cast<CGstate>( id ); }
    _CGcontext *getParent() const { return parent; }

    inline State *getNext() const { return next; }
    inline void unlink() { next = 0; }

    inline AnyValue::Type getType() const { return type; }
    inline int getElemCount() const { return elemCount; }

    void append( State *other );
    void remove( State *other );
    void removeAll();

    inline CGstatecallback getSetCallback()
    {
        return setCallback;
    }

    inline CGstatecallback getResetCallback()
    {
        return resetCallback;
    }

    inline CGstatecallback getValidateCallback()
    {
        return validateCallback;
    }

    inline bool validate( StateAssignment *sa ) const
    {
        return validateFunction( sa );
    }

    inline bool set( StateAssignment *sa ) const
        {
            return setFunction( sa );
        }

    inline bool reset( StateAssignment *sa ) const
    {
        return resetFunction( sa );
    }

    inline void addEnum( GLenum value, const char *name )
    {
        enumMap.appendNew().set( value, name );
    }

    inline bool findEnumValueByName( GLenum &value, const char *name )
    {
        for ( unsigned int i = 0; i < enumMap.getCount(); ++i )
        {
            if ( strcmp( name, enumMap[i].name ) == 0 )
            {
                value = enumMap[i].value;
                return true;
            }
        }
        return false;
    }

    inline bool findEnumNameByValue( GLenum value, const char *&name )
    {
        for ( unsigned int i = 0; i < enumMap.getCount(); ++i )
        {
            if ( value == enumMap[i].value )
            {
                name = enumMap[i].name;
                return true;
            }
        }
        return false;
    }
};

inline bool StateAssignment::validate() { validated = state->validate( this ); return validated; }

}

#endif
