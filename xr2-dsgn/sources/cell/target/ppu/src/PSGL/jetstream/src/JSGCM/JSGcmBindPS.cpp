/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
 /*
 * File description here
 */

#include <JSGCM/JSGcmUtils.h>
#include <Cg/CgCommon.h>

// load a binary fragment program
void _jsGcmSetNativeCgFragmentProgram
(
    const GLvoid *header,
    const GLuint ucodeId
)
{
    const CgBinaryProgram *ps = ( const CgBinaryProgram* ) header;
    CgBinaryFragmentProgram *binaryFragmentProgram;

    JS_ASSERT( ps->binaryFormatRevision == CG_BINARY_FORMAT_REVISION );

    JS_ASSERT(   ( ps->profile == CG_PROFILE_SCE_FP_TYPEB )
              || ( ps->profile == CG_PROFILE_SCE_FP_TYPEC )
              || ( ps->profile == CG_PROFILE_SCE_FP_RSX ) );
    
    binaryFragmentProgram = ( CgBinaryFragmentProgram* )(( char* )ps +  ps->program );

    // check program size & location
    JS_ASSERT( ps->ucodeSize > 0 );

    CellCgbFragmentProgramConfiguration conf;

    conf.offset = gmmIdToOffset(ucodeId);
    JS_ASSERT(( conf.offset & 63 ) == 0 );

    jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;

    s->fragmentProgramAttribMask =
        CELL_GCM_ATTRIB_OUTPUT_MASK_POINTSIZE |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC0 |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC1 |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC2 |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC3 |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC4 |
        CELL_GCM_ATTRIB_OUTPUT_MASK_UC5 ;

    s->fragmentProgramAttribMask |= binaryFragmentProgram->attributeInputMask | CELL_GCM_ATTRIB_OUTPUT_MASK_POINTSIZE;

    conf.attributeInputMask = ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                              s->fragmentProgramAttribMask & s->clipMask & s->sideMask;

    conf.texCoordsInputMask = binaryFragmentProgram->texCoordsInputMask;
    conf.texCoords2D = binaryFragmentProgram->texCoords2D;
    conf.texCoordsCentroid = binaryFragmentProgram->texCoordsCentroid;

    int fragmentControl = ( 1 << 15 ) | ( 1 << 10 );
    fragmentControl |= binaryFragmentProgram->depthReplace ? 0xE : 0x0;
    fragmentControl |= binaryFragmentProgram->outputFromH0 & CGF_OUTPUTFROMH0 ? 0x00 : 0x40;
    fragmentControl |= binaryFragmentProgram->pixelKill & CGF_PIXELKILL ? 0x80 : 0x00;

    conf.fragmentControl  = fragmentControl;
    conf.registerCount = binaryFragmentProgram->registerCount < 2 ? 2 : binaryFragmentProgram->registerCount;;

	// going to disable TXP Demotion by default for non Cgb PSGL interfaces for 2.40 and later  
	// for more information on the effects of Control TXP 
	// see cellGcmSetFragmentProgramLoad in the libGcm Reference
	// TXP Demotion can be enabled via glEnable( GL_FRAGMENT_PROGRAM_CONTROL_CONTROLTXP_SCE )
	uint32_t controlTxp = _CurrentContext->AllowTXPDemotion; 
	conf.fragmentControl &= ~CELL_GCM_MASK_SET_SHADER_CONTROL_CONTROL_TXP; 
	conf.fragmentControl |= controlTxp << CELL_GCM_SHIFT_SET_SHADER_CONTROL_CONTROL_TXP; 

	GCM_FUNC( cellGcmSetFragmentProgramLoad, &conf )

    JS_ASSERT( _jsGcmSetPixelShaderConsistency( !binaryFragmentProgram->outputFromH0 ) );
    GCM_FUNC( cellGcmSetZMinMaxControl, binaryFragmentProgram->depthReplace ? JSGCM_FALSE : JSGCM_TRUE, JSGCM_FALSE, JSGCM_FALSE );
}


void _jsSetNativeCgFragmentProgram( const GLvoid *header )
{
    const _CGprogram *ps = ( const _CGprogram * )header;


    JS_ASSERT(   ( ps->header.profile == CG_PROFILE_SCE_FP_TYPEB )
              || ( ps->header.profile == CG_PROFILE_SCE_FP_RSX ) );

    CellCgbFragmentProgramConfiguration conf;

    conf.offset = gmmIdToOffset(ps->loadProgramId) + ps->loadProgramOffset;
    JS_ASSERT(( conf.offset & 63 ) == 0 );

    jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
    s->fragmentProgramAttribMask |= ps->header.attributeInputMask | CELL_GCM_ATTRIB_OUTPUT_MASK_POINTSIZE;

    conf.attributeInputMask = ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                              s->fragmentProgramAttribMask & s->clipMask & s->sideMask;

    conf.texCoordsInputMask = ps->header.fragmentProgram.texcoordInputMask;
    conf.texCoords2D = ps->header.fragmentProgram.texcoord2d;
    conf.texCoordsCentroid = ps->header.fragmentProgram.texcoordCentroid;

    int fragmentControl = ( 1 << 15 ) | ( 1 << 10 );
    fragmentControl |= ps->header.fragmentProgram.flags & CGF_DEPTHREPLACE ? 0xE : 0x0;
    fragmentControl |= ps->header.fragmentProgram.flags & CGF_OUTPUTFROMH0 ? 0x00 : 0x40;
    fragmentControl |= ps->header.fragmentProgram.flags & CGF_PIXELKILL ? 0x80 : 0x00;

    conf.fragmentControl  = fragmentControl;
    conf.registerCount = ps->header.fragmentProgram.registerCount < 2 ? 2 : ps->header.fragmentProgram.registerCount;

	// going to disable TXP Demotion by default for non Cgb PSGL interfaces for 2.40 and later  
	// for more information on the effects of Control TXP 
	// see cellGcmSetFragmentProgramLoad in the libGcm Reference
	// TXP Demotion can be enabled via glEnable( GL_FRAGMENT_PROGRAM_CONTROL_CONTROLTXP_SCE )
	uint32_t controlTxp = _CurrentContext->AllowTXPDemotion; 
	conf.fragmentControl &= ~CELL_GCM_MASK_SET_SHADER_CONTROL_CONTROL_TXP; 
	conf.fragmentControl |= controlTxp << CELL_GCM_SHIFT_SET_SHADER_CONTROL_CONTROL_TXP; 

	// based on the location flag set to load program from local or main memory 
	if ( ps->inLocalMemory )
	{
	    GCM_FUNC( cellGcmSetFragmentProgramLoad, &conf );
	}
	else
	{
		GCM_FUNC( cellGcmSetFragmentProgramLoadLocation, &conf, CELL_GCM_LOCATION_MAIN ); 
	}

    JS_ASSERT( _jsGcmSetPixelShaderConsistency( !( ps->header.fragmentProgram.flags & CGF_OUTPUTFROMH0 ) ) );

	GCM_FUNC( cellGcmSetZMinMaxControl, ( ps->header.fragmentProgram.flags & CGF_DEPTHREPLACE ) ? JSGCM_FALSE : JSGCM_TRUE, JSGCM_FALSE, JSGCM_FALSE );
}
