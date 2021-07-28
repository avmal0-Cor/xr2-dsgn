/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Program FX extensions
 */

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <PSGLFX/Base.h>
#include <PSGL/private.h>
#include <Cg/CgCommon.h>

namespace PSGLFX
{

class CgProgramExtension: public PSGL::Base
{
    Annotation *annotations;

public:
    CgProgramExtension();
    CgProgramExtension( const CgProgramExtension &other );
    ~CgProgramExtension();

    inline Annotation *getFirstAnnotation() const { return annotations; }
    void appendAnnotation( Annotation *annotation );

    static inline CgProgramExtension *fromProgram( _CGprogram *prog ) { return static_cast<CgProgramExtension *>( prog->FXprogramExtension );}
    static inline CgProgramExtension *fromProgramSafe( _CGprogram *prog )
    {
        if ( !prog->FXprogramExtension )
        {
            prog->FXprogramExtension = new CgProgramExtension();
        }
        return static_cast<CgProgramExtension *>( prog->FXprogramExtension );
    }
};

}

#endif
