/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Annotations
 */

#ifndef _ANNOTATION_H_
#define _ANNOTATION_H_

#include <Cg/CgCommon.h>
#include <PSGLFX/Base.h>
#include <PSGLFX/AnyValue.h>

namespace PSGLFX
{

class Annotation: public Named
{
    jsName id;
    Annotation *next;

    AnyValue value;

public:
    Annotation( const char *n, AnyValue::Type type, int nelem = 1 );
    Annotation( const Annotation &other );
    ~Annotation();

    inline CGannotation getId() const { return reinterpret_cast<CGannotation>( id ); }

    inline Annotation *getNext() const { return next; }
    void append( Annotation *other );

    inline AnyValue &getValue() { return value; }
};

}


#endif
