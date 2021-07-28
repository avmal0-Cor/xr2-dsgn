/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmBindVS.cpp
 */

#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmShader.h>
#include <Cg/CgCommon.h>
#include <string.h>
#include <ppu_intrinsics.h>

/* load a binary vertex program
**
** notes:
**  - this is the most (cpu) expensive function in JSGCM. The caller is supposed to
**    manage multiple program and the necessary constant downloads...
*/
void _jsGcmSetNativeCgVertexProgram
(
    const void *header,
    const void *ucode
)
{
    const CgBinaryProgram *vs;
    CgBinaryVertexProgram *binaryVertexProgram;
    const CgBinaryParameter *param;
    const GLuint *rawData;
    GLuint paramCount;
    GLuint instCount;
    GLuint instIndex;

	// [YLIN] Pre-load vertex program onto cache to avoid d-cache miss
	__dcbt(ucode);
	__dcbt(((uint8_t*)ucode)+128);
	__dcbt(((uint8_t*)ucode)+256);
	__dcbt(((uint8_t*)ucode)+384);

    vs = ( const CgBinaryProgram* ) header;
    JS_ASSERT( vs->binaryFormatRevision == CG_BINARY_FORMAT_REVISION );

    JS_ASSERT(( vs->profile == CG_PROFILE_SCE_VP_TYPEB )
              || ( vs->profile == CG_PROFILE_SCE_VP_TYPEC )
              || ( vs->profile == CG_PROFILE_SCE_VP_RSX ) );

    binaryVertexProgram = ( CgBinaryVertexProgram* )(( char* )vs + vs->program );

    rawData = ( const GLuint* )ucode;
    instCount = binaryVertexProgram->instructionCount;
    instIndex = binaryVertexProgram->instructionSlot;

    // check program size
    JS_ASSERT( instCount * 16 == vs->ucodeSize );
    JS_ASSERT( instCount <= JSGCM_VTXPRG_MAX_INST );
    JS_ASSERT(( instIndex + instCount ) <= JSGCM_VTXPRG_MAX_INST );

    CellCgbVertexProgramConfiguration conf;
    conf.instructionSlot = instIndex;
    conf.instructionCount = instCount;
    conf.registerCount = binaryVertexProgram->registerCount;
    conf.attributeInputMask = binaryVertexProgram->attributeInputMask;

    _jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo,  7 + 5 * conf.instructionCount );

    GCM_FUNC( cellGcmSetVertexProgramLoad, &conf, ucode );

    // make sure the user clip plane setting is up to date
    GCM_FUNC( cellGcmSetUserClipPlaneControl, binaryVertexProgram->userClipMask, 0, 0, 0, 0, 0 );

    // update attributes used
    jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;

    s->vertexProgramAttribMask = binaryVertexProgram->attributeOutputMask;

    // vertex output attributes
    GCM_FUNC( cellGcmSetVertexAttribOutputMask, (( s->vertexProgramAttribMask | s->pointSpriteMask ) &
              s->fragmentProgramAttribMask &
              s->clipMask &
              s->sideMask ) );

    param = ( const CgBinaryParameter * )(( const char * )vs + vs->parameterArray );
    for ( paramCount = vs->parameterCount; paramCount-- > 0; )
    {
        if ( param->defaultValue &&
                ( param->var == CG_CONSTANT ) )
        {
            GLfloat value[16];
            memcpy( &value, ( const char * )vs + param->defaultValue, sizeof( value ) );
            _jsGcmFifoGlProgramParameterfv(
                ( CGprogram )vs,
                ( CGparameter )param,
                value,
                GMM_ERROR );
        }
        ++param;
    }
}

void _jsSetNativeCgVertexProgram( const void *header )
{
    const _CGprogram *vs = ( const _CGprogram* ) header;
    JS_ASSERT(( vs->header.profile == CG_PROFILE_SCE_VP_TYPEB )
              || ( vs->header.profile == CG_PROFILE_SCE_VP_RSX ) );

    JS_ASSERT(( vs->header.vertexProgram.instructionSlot + vs->header.instructionCount ) <= JSGCM_VTXPRG_MAX_INST );

    //it will be stored in a 16 bit integer
    JS_ASSERT(( vs->header.attributeInputMask & ~0xffff ) == 0 );

	// [YLIN] Pre-load vertex program onto cache to avoid d-cache miss
	__dcbt(vs->ucode);
	__dcbt(((uint8_t*)vs->ucode)+128);
	__dcbt(((uint8_t*)vs->ucode)+256);
	__dcbt(((uint8_t*)vs->ucode)+384);

    CellCgbVertexProgramConfiguration conf;
    conf.instructionSlot = vs->header.vertexProgram.instructionSlot;
    conf.instructionCount = vs->header.instructionCount;
    conf.registerCount = vs->header.vertexProgram.registerCount;
    conf.attributeInputMask = vs->header.attributeInputMask;

    _jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo, 7 + 5 * conf.instructionCount );

    GCM_FUNC( cellGcmSetVertexProgramLoad, &conf, vs->ucode );

    GCM_FUNC( cellGcmSetUserClipPlaneControl, vs->header.vertexProgram.userClipMask, 0, 0, 0, 0, 0 );

    // update attributes used
    jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
    s->vertexProgramAttribMask = vs->header.vertexProgram.attributeOutputMask;

    GCM_FUNC( cellGcmSetVertexAttribOutputMask, (( s->vertexProgramAttribMask | s->pointSpriteMask ) &
              s->fragmentProgramAttribMask &
              s->clipMask &
              s->sideMask ) );

    _CGprogram *program = ( _CGprogram* )vs;
    int count = program->defaultValuesIndexCount;
    for ( int i = 0;i < count;i++ )
    {
        //only set the default value if we have a constant
        const CgParameterEntry *parameterEntry = program->parametersEntries + program->defaultValuesIndices[i].entryIndex;
        if (( parameterEntry->flags & CGPF_REFERENCED ) && ( parameterEntry->flags & CGPV_MASK ) == CGPV_CONSTANT )
        {
            const float *itemDefaultValues = program->defaultValues + program->defaultValuesIndices[i].defaultValueIndex;
            _jsFifoGlProgramParameterfvVP( program, parameterEntry, itemDefaultValues );
        }
    }
}
