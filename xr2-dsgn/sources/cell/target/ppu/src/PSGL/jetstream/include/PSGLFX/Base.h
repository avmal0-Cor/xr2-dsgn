/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Base class (for new and delete)
 */


#ifndef _PSGLFX_BASE_H_
#define _PSGLFX_BASE_H_

#include <PSGL/private.h>
#include <PSGLFX/AnyValue.h>
#include <PSGL/Base.h>

namespace PSGLFX
{

class Named: public PSGL::Base
{
    const char *name;
public:
    const char *getName() const { return name; }
    void setName( const char *n ) { freeString( name ); name = makeString( n ); }
    Named( const char *n ): name( makeString( n ) ) {}
    ~Named() { freeString( name ); }
};

}

#endif
