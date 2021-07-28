/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Parameter FX extensions
 */

#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include <PSGLFX/Base.h>
#include <PSGL/private.h>
#include <Cg/CgCommon.h>

namespace PSGLFX
{

class CgParameterExtension: public PSGL::Base
{
    Annotation *annotations;
    StateAssignment *stateAssignments;

public:
    CgParameterExtension();
    CgParameterExtension( const CgParameterExtension &other );
    ~CgParameterExtension();

    inline StateAssignment *getFirstStateAssignment() const { return stateAssignments; }
    void appendStateAssignment( StateAssignment *stateAssignment );

    inline Annotation *getFirstAnnotation() const { return annotations; }
    void appendAnnotation( Annotation *annotation );

    static inline CgParameterExtension *fromParameter( CgRuntimeParameter *param )
    {
        STL_NAMESPACE  vector<CgParameterFX>::iterator fxExtIter = param->program->fxParameterExtensions.begin();
        while ( fxExtIter != param->program->fxParameterExtensions.end() )
        {
            if (( *fxExtIter ).param == param )
            {
                return static_cast<CgParameterExtension *>(( *fxExtIter ).fx );
            }
            fxExtIter++;
        }
        return NULL;
    }

    static inline CgParameterExtension *fromParameterSafe( CgRuntimeParameter *param )
    {
        STL_NAMESPACE vector<CgParameterFX>::iterator fxExtIter = param->program->fxParameterExtensions.begin();
        while ( fxExtIter != param->program->fxParameterExtensions.end() )
        {
            if (( *fxExtIter ).param == param )
            {
                return static_cast<CgParameterExtension *>(( *fxExtIter ).fx );
            }
            fxExtIter++;
        }
        // if we reach the end of this loop, we never found the fx extension for this param
        CgParameterExtension *newFx = new CgParameterExtension();
        JS_ASSERT( newFx );
        CgParameterFX newParamFX;
        newParamFX.param = param;
        newParamFX.fx = newFx;
        param->program->fxParameterExtensions.push_back( newParamFX );
        return newFx;
    }

    void setState();
};



}

#endif
