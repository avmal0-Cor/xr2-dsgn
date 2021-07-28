/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef __CELL_CGUTIL_H__
#define __CELL_CGUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef int int32_t;
#ifndef _cg_h
#include <Cg/cg.h>
#include <cgb.h>
#endif

/* cg functions */
void cellGcmCgInitProgram(CGprogram prog);
void cellGcmCgGetUCode(CGprogram prog, void **pUCode, uint32_t *pUCodeSize);
CGparameter cellGcmCgGetNamedParameter(CGprogram prog, const char *name);
CGparameter cellGcmCgGetFirstLeafParameter(CGprogram prog, CGenum name_space);
CGparameter cellGcmCgGetNextLeafParameter(CGprogram prog, CGparameter current);
uint32_t cellGcmCgGetCountParameter(CGprogram prog);
uint32_t cellGcmCgGetInstructions(CGprogram prog);
CGparameter cellGcmCgGetIndexParameter(CGprogram prog, const uint32_t Index);
CGtype cellGcmCgGetParameterType(CGprogram prog, CGparameter param);
CGresource cellGcmCgGetParameterResource(CGprogram prog, CGparameter param);
const char *cellGcmCgGetParameterName(CGprogram prog, CGparameter param);
CGenum cellGcmCgGetParameterVariability(CGprogram prog, CGparameter param);
uint32_t cellGcmCgGetParameterResourceIndex(CGprogram prog, CGparameter param);
const float *cellGcmCgGetParameterValues(CGprogram prog, CGparameter param);
int32_t cellGcmCgGetParameterOrdinalNumber(CGprogram prog, CGparameter param);
CGenum cellGcmCgGetParameterDirection(CGprogram prog, CGparameter param);
const char *cellGcmCgGetParameterSemantic(CGprogram prog, CGparameter param);
CGprofile cellGcmCgGetProgramProfile(CGprogram prog);
CGbool cellGcmCgGetParameterReferenced(CGprogram prog, CGparameter param);
uint32_t cellGcmCgGetTotalBinarySize(CGprogram prog);
uint32_t cellGcmCgGetRegisterCount(const CGprogram prog);
void cellGcmCgSetRegisterCount(const CGprogram prog, const uint8_t registerCount);
uint16_t cellGcmCgGetVertexAttribInputMask(const CGprogram prog);
uint32_t cellGcmCgGetAttribOutputMask(const CGprogram prog);
void cellGcmCgSetAttribOutputMask(const CGprogram prog, const uint32_t mask);
uint32_t cellGcmCgGetEmbeddedConstantOffset(const CGprogram prog, CGparameter param, const uint32_t index);
uint32_t cellGcmCgGetEmbeddedConstantCount(const CGprogram prog, CGparameter param);
int32_t cellGcmCgGetCgbFragmentProgramConfiguration(const CGprogram prog, CellCgbFragmentProgramConfiguration *conf, uint8_t controlTxp, uint8_t reserved0, uint8_t reserved1);
int32_t cellGcmCgGetCgbVertexProgramConfiguration(const CGprogram prog, CellCgbVertexProgramConfiguration *conf);
int32_t cellGcmCgSetInstructionSlot(const CGprogram prog, const uint32_t newSlot);
int32_t cellGcmCgSetInstructionSlotUCode(const uint32_t newSlot, const uint32_t currentSlot, const uint32_t instCount, const void *ucode);
uint32_t cellGcmCgGetVertexUserClipMask(const CGprogram prog);
uint32_t cellGcmCgGetInstructionSlot(const CGprogram prog);
CGbool cellGcmCgFlipBigEndian(CGprogram prog);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __CELL_CGUTIL_H__ */
