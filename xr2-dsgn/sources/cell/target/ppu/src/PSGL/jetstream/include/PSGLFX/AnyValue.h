/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Any value helper class
 */

#ifndef _ANYVALUE_H_
#define _ANYVALUE_H_

#include <PSGL/private.h>
#include <Cg/CgCommon.h>
#include <string.h>

namespace PSGLFX
{
inline const char *makeString( const char *string )
{
    return string ? jsStrdup( string ) : NULL;
}

inline void freeString( const char *string )
{
    if ( string ) jsFree( const_cast<char *>( string ) );
}

class AnyValue
{
public:
    union Value
    {
        const char *asString;
        float asFloat;
        int asInt;
        CGbool asBool;
        _CGprogram *asProgram;
        CgRuntimeParameter *asTexture;
        CgRuntimeParameter *asSampler;
        void *arrayData;
    };

    enum Type
    {
        FLOAT,
        INT,
        BOOL,
        STRING,
        PROGRAM,
        TEXTURE,
        SAMPLER1D,
        SAMPLER2D,
        SAMPLER3D,
        SAMPLERCUBE,
        SAMPLERRECT,
        NONE
    };

private:
    Type type;
    Value value;
    int elemCount;

public:
    AnyValue( Type t, int count ):
            type( t ),
            elemCount( count )
    {
        if ( isArray() )
        {
            JS_ASSERT(( type == FLOAT ) || ( type == INT ) || ( type == BOOL ) );
            value.arrayData = jsMalloc( getArraySize() );
        }
        else value.asString = 0;
    }
    AnyValue( const AnyValue &other ):
            type( other.type ),
            elemCount( other.elemCount )
    {
        if ( isArray() )
        {
            JS_ASSERT(( type == FLOAT ) || ( type == INT ) || ( type == BOOL ) );
            value.arrayData = jsMalloc( getArraySize() );
            memcpy( value.arrayData, other.value.arrayData, getArraySize() );
        }
        else
        {
            if ( type == STRING ) value.asString = makeString( other.value.asString );
            else value = other.value;
        }
    }

    ~AnyValue()
    {
        if ( isArray() ) jsFree( value.arrayData );
        else if ( type == STRING ) freeString( value.asString );
    }

inline void setInt( int i ) { JS_ASSERT( type == INT ); value.asInt = i; }
    inline void setFloat( float f ) { JS_ASSERT( type == FLOAT ); value.asFloat = f; }
    inline void setBool( CGbool b ) { JS_ASSERT( type == BOOL ); value.asBool = b; }
    inline void setString( const char *s ) { JS_ASSERT( type == STRING ); freeString( value.asString ); value.asString = makeString( s ); }
    inline void setProgram( _CGprogram *p ) { JS_ASSERT( type == PROGRAM ); value.asProgram = p; }
    inline void setTexture( CgRuntimeParameter *p ) { JS_ASSERT( type == TEXTURE ); value.asTexture = p; }
    inline void setSampler( CgRuntimeParameter *p ) { JS_ASSERT( isSamplerType( type ) ); JS_ASSERT( getSamplerTypeFromCGtype( _jsGetParameterCGtype( p->program, p->parameterEntry ) ) == type ); value.asSampler = p; }

    inline int getInt() const { JS_ASSERT( type == INT ); return value.asInt; }
    inline float getFloat() const { JS_ASSERT( type == FLOAT ); return value.asFloat; }
    inline CGbool getBool() const { JS_ASSERT( type == BOOL ); return value.asBool; }
    inline const char *getString() const { JS_ASSERT( type == STRING ); return value.asString; }
    inline _CGprogram *getProgram() const { JS_ASSERT( type == PROGRAM ); return value.asProgram; }
    inline CgRuntimeParameter *getTexture() const { JS_ASSERT( type == TEXTURE ); return value.asTexture; }
    inline CgRuntimeParameter *getSampler() const { JS_ASSERT( isSamplerType( type ) ); return value.asSampler; }

    inline void *getArrayData() const { JS_ASSERT( isArray() ); JS_ASSERT(( type == FLOAT ) || ( type == INT ) || ( type == BOOL ) ); return value.arrayData; }
    inline const void *getData() const { if ( isArray() ) return getArrayData(); else return &value; }

inline Type getType() const { return type; }
    inline int getElemCount() const { return elemCount; }
    inline bool isArray() const { return elemCount > 1; }

    inline size_t getTypeSize()
    {
        switch ( type )
        {
            case FLOAT: return sizeof( float );
            case INT: return sizeof( int );
            case BOOL: return sizeof( CGbool );
            default:
                JS_ASSERT( 0 );
                return 0;
        }
    }

    inline size_t getArraySize()
    {
        JS_ASSERT( isArray() );
        return elemCount*getTypeSize();
    }

    inline static Type getAnnotationTypeFromCGtype( CGtype type )
    {
        switch ( type )
        {
            case CG_FLOAT: return FLOAT;
            case CG_INT: return INT;
            case CG_BOOL: return BOOL;
            case CG_STRING: return STRING;
            default: return NONE;
        }
    }

    inline static Type getStateTypeFromCGtype( CGtype type )
    {
        switch ( type )
        {
            case CG_FLOAT: return FLOAT;
            case CG_INT: return INT;
            case CG_BOOL: return BOOL;
            case CG_PROGRAM_TYPE: return PROGRAM;
            case CG_TEXTURE: return TEXTURE;
            case CG_SAMPLER1D:
            case CG_SAMPLER2D:
            case CG_SAMPLER3D:
            case CG_SAMPLERCUBE:
            case CG_SAMPLERRECT:
                return getSamplerTypeFromCGtype( type );
            default: return NONE;
        }
    }

    inline static Type getSamplerTypeFromCGtype( CGtype type )
    {
        switch ( type )
        {
            case CG_SAMPLER1D:
                return SAMPLER1D;
            case CG_SAMPLER2D:
                return SAMPLER2D;
            case CG_SAMPLER3D:
                return SAMPLER3D;
            case CG_SAMPLERCUBE:
                return SAMPLERCUBE;
            case CG_SAMPLERRECT:
                return SAMPLERRECT;
            default:
                return NONE;
        }
    }

    inline static CGtype getCGtypeFromType( Type type )
    {
        switch ( type )
        {
            case FLOAT: return CG_FLOAT;
            case INT: return CG_INT;
            case BOOL: return CG_BOOL;
            case STRING: return CG_STRING;
            case PROGRAM: return CG_PROGRAM_TYPE;
            case TEXTURE: return CG_TEXTURE;
            case SAMPLER1D:
            case SAMPLER2D:
            case SAMPLER3D:
            case SAMPLERCUBE:
            case SAMPLERRECT:
                return getSamplerCGtypeFromType( type );
            default: return CG_UNKNOWN_TYPE;
        }
    }

    inline static bool isSamplerType( Type type )
    {
        switch ( type )
        {
            case SAMPLER1D:
            case SAMPLER2D:
            case SAMPLER3D:
            case SAMPLERCUBE:
            case SAMPLERRECT:
                return true;
            default:
                return false;
        }
    }

    inline static CGtype getSamplerCGtypeFromType( Type type )
    {
        switch ( type )
        {
            case SAMPLER1D:
                return CG_SAMPLER1D;
            case SAMPLER2D:
                return CG_SAMPLER2D;
            case SAMPLER3D:
                return CG_SAMPLER3D;
            case SAMPLERCUBE:
                return CG_SAMPLERCUBE;
            case SAMPLERRECT:
                return CG_SAMPLERRECT;
            default:
                return CG_UNKNOWN_TYPE;
        }
    }
};

}
#endif
