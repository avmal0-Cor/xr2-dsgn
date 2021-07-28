/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc., Sony Corporation
* All Rights Reserved.
*/

#ifndef __CELL_FACE_UTIL_H__
#define __CELL_FACE_UTIL_H__ 1

#include <cell/face/face.h>
#include <stdint.h>

#ifdef __PPU__

#define CELL_FACE_UTIL_WORK_SIZE(width, height, rowstride)						\
	(240																		\
	+(((((uint32_t)(rowstride)/   2)+0xf)&(~0xf))*((uint32_t)(height)/   2))	\
	+(((((uint32_t)(rowstride)/   4)+0xf)&(~0xf))*((uint32_t)(height)/   4))	\
	+(((((uint32_t)(rowstride)/   8)+0xf)&(~0xf))*((uint32_t)(height)/   8))	\
	+(((((uint32_t)(rowstride)/  16)+0xf)&(~0xf))*((uint32_t)(height)/  16))	\
	+(((((uint32_t)(rowstride)/  32)+0xf)&(~0xf))*((uint32_t)(height)/  32))	\
	+(((((uint32_t)(rowstride)/  64)+0xf)&(~0xf))*((uint32_t)(height)/  64))	\
	+(((((uint32_t)(rowstride)/ 128)+0xf)&(~0xf))*((uint32_t)(height)/ 128))	\
	+(((((uint32_t)(rowstride)/ 256)+0xf)&(~0xf))*((uint32_t)(height)/ 256))	\
	+(((((uint32_t)(rowstride)/ 512)+0xf)&(~0xf))*((uint32_t)(height)/ 512))	\
	+(((((uint32_t)(rowstride)/1024)+0xf)&(~0xf))*((uint32_t)(height)/1024)))

static inline
void cellFaceUtilDetectionParamInitialize(
	CellFaceDetectionParam			*detectParam,
	const void						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*faceResult,
	uint32_t						faceResultNum
)
{
	detectParam->eaImage				= (uintptr_t)imageData;
	detectParam->imageWidth				= imageWidth;
	detectParam->imageHeight			= imageHeight;
	detectParam->imageRowstride			= imageRowstride;
	detectParam->eaWorkingArea			= (uintptr_t)workingArea;
	detectParam->eaFaceResult			= (uintptr_t)faceResult;
	detectParam->eaDetectionDict		= cellFaceDetectionGetDictEa();
	detectParam->minFaceSize			= 20;
	detectParam->maxFaceSize			= (imageWidth < imageHeight) ? imageWidth : imageHeight;
	detectParam->xScanStep				= 2;
	detectParam->yScanStep				= 2;
	detectParam->scalingStep			= 0.841f;
	detectParam->detectionThreshold		= 4.0f;
	detectParam->maxNumResult			= faceResultNum;
	detectParam->eaOptParamMultiSpu		= 0;	/* 0: disable */
	detectParam->eaOptParamLocalSearch	= 0;	/* 0: disable */
}

static inline
void cellFaceUtilDetectionFrontalParamInitialize(
	CellFaceDetectionParam			*detectParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*faceResult,
	uint32_t						faceResultNum
)
{
	detectParam->eaImage				= (uintptr_t)imageData;
	detectParam->imageWidth				= imageWidth;
	detectParam->imageHeight			= imageHeight;
	detectParam->imageRowstride			= imageRowstride;
	detectParam->eaWorkingArea			= (uintptr_t)workingArea;
	detectParam->eaFaceResult			= (uintptr_t)faceResult;
	detectParam->eaDetectionDict		= 0;	/* no dictionary */
	detectParam->minFaceSize			= 20;
	detectParam->maxFaceSize			= (imageWidth < imageHeight) ? imageWidth : imageHeight;
	detectParam->xScanStep				= 2;
	detectParam->yScanStep				= 2;
	detectParam->scalingStep			= 0.841f;
	detectParam->detectionThreshold		= 4.0f;
	detectParam->maxNumResult			= faceResultNum;
	detectParam->eaOptParamMultiSpu		= 0;	/* 0: disable */
	detectParam->eaOptParamLocalSearch	= 0;	/* 0: disable */
}

static inline
void cellFaceUtilOptParamMultiSpuInitialize(
	CellFaceOptParamMultiSpu	*multiSpuParam,
	CellFaceDetectionParam		*detectionParam,
	uint32_t					numTasks,
	uint32_t					taskId
)
{
	uint32_t xScanStep					= detectionParam->xScanStep;

	detectionParam->eaOptParamMultiSpu	= (uintptr_t)multiSpuParam;
	detectionParam->xScanStep			= xScanStep * numTasks;
	multiSpuParam->xScanStart			= xScanStep * taskId;
	multiSpuParam->yScanStart			= 0;
	multiSpuParam->storeOverlap			= 1;
}

static inline
void cellFaceUtilOptParamLocalSearchInitialize(
	CellFaceOptParamLocalSearch	*localSearchParam,
	CellFaceDetectionParam		*detectionParam,
	CellFaceDetectionResultData	*localData,
	uint32_t					numLocal
)
{
	detectionParam->eaOptParamLocalSearch	= (uintptr_t)localSearchParam;
	localSearchParam->eaLocalData			= (uintptr_t)localData;
	localSearchParam->numLocal				= numLocal;
	localSearchParam->scanFaceSizeMin		= 0.8f;
	localSearchParam->scanFaceSizeMax		= 1.3f;
	localSearchParam->scanFaceMarginX		= 8;
	localSearchParam->scanFaceMarginY		= 4;
}

static inline
void cellFaceUtilMergeResultParamInitialize(
	CellFaceMergeResultParam			*mergeParam,
	CellFaceDetectionResultData			*mergeData,
	uint32_t							numMerge,
	const CellFaceDetectionResult		*faceResult,
	uint32_t							faceResultNum
)
{
	mergeParam->eaMergeData		= (uintptr_t)mergeData;
	mergeParam->eaFaceResult	= (uintptr_t)faceResult;
	mergeParam->numMerge		= numMerge;
	mergeParam->maxNumResult	= faceResultNum;
}

static inline
void cellFaceUtilPartsParamInitialize(
	CellFacePartsParam				*partsParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*partsResult,
	const CellFacePosition			*positionResult
)
{
	partsParam->eaImage				= (uintptr_t)imageData;
	partsParam->eaWorkingArea		= (uintptr_t)workingArea;
	partsParam->eaPartsDict			= cellFacePartsGetDictEa();
	partsParam->eaPartsResult		= (uintptr_t)partsResult;
	partsParam->eaPositionResult	= (uintptr_t)positionResult;
	partsParam->imageWidth			= imageWidth;
	partsParam->imageHeight			= imageHeight;
	partsParam->imageRowstride		= imageRowstride;
	partsParam->faceX				= face->faceX;
	partsParam->faceY				= face->faceY;
	partsParam->faceW				= face->faceW;
	partsParam->faceH				= face->faceH;
	partsParam->faceRoll			= face->faceRoll;
	partsParam->facePitch			= face->facePitch;
	partsParam->faceYaw				= face->faceYaw;
}

static inline
void cellFaceUtilAllPartsParamInitialize(
	CellFaceAllPartsParam			*allPartsParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*allPartsResult
)
{
	allPartsParam->eaImage			= (uintptr_t)imageData;
	allPartsParam->eaWorkingArea	= (uintptr_t)workingArea;
	allPartsParam->eaPartsDict		= cellFaceAllPartsGetDictEa();
	allPartsParam->eaShapeDict		= 0ULL;
	allPartsParam->eaPartsResult	= (uintptr_t)allPartsResult;
	allPartsParam->eaShapeResult	= 0ULL;
	allPartsParam->enablePartsBit	= 0x07ffe73fffffffffULL;
	allPartsParam->imageWidth		= imageWidth;
	allPartsParam->imageHeight		= imageHeight;
	allPartsParam->imageRowstride	= imageRowstride;
	allPartsParam->faceX			= face->faceX;
	allPartsParam->faceY			= face->faceY;
	allPartsParam->faceW			= face->faceW;
	allPartsParam->faceH			= face->faceH;
	allPartsParam->faceRoll			= face->faceRoll;
	allPartsParam->facePitch		= face->facePitch;
	allPartsParam->faceYaw			= face->faceYaw;
}

static inline
void cellFaceUtilAllPartsWithShapeConstraintParamInitialize(
	CellFaceAllPartsParam			*allPartsParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*allPartsResult,
	const CellFacePartsResult		*allPartsShapeResult
)
{
	allPartsParam->eaImage			= (uintptr_t)imageData;
	allPartsParam->eaWorkingArea	= (uintptr_t)workingArea;
	allPartsParam->eaPartsDict		= cellFaceAllPartsGetDictEa();
	allPartsParam->eaShapeDict		= cellFaceAllPartsShapeGetDictEa();
	allPartsParam->eaPartsResult	= (uintptr_t)allPartsResult;
	allPartsParam->eaShapeResult	= (uintptr_t)allPartsShapeResult;
	allPartsParam->enablePartsBit	= 0x07ffe73fffffffffULL;
	allPartsParam->imageWidth		= imageWidth;
	allPartsParam->imageHeight		= imageHeight;
	allPartsParam->imageRowstride	= imageRowstride;
	allPartsParam->faceX			= face->faceX;
	allPartsParam->faceY			= face->faceY;
	allPartsParam->faceW			= face->faceW;
	allPartsParam->faceH			= face->faceH;
	allPartsParam->faceRoll			= face->faceRoll;
	allPartsParam->facePitch		= face->facePitch;
	allPartsParam->faceYaw			= face->faceYaw;
}

static inline
void cellFaceUtilAllPartsEnableByID(
	CellFaceAllPartsParam			*allPartsParam,
	uint32_t						*listPartsId,
	uint32_t						listPartsIdNum
)
{
	uint64_t bit = 0;
	uint32_t i;
	for (i=0; i<listPartsIdNum; i++) {
		bit |= (1ULL)<<(listPartsId[i] - CELL_FACE_PARTS_ID_ALL_BASE);
	}
	allPartsParam->enablePartsBit	= bit;
}

static inline
void cellFaceUtilAllPartsShapeParamInitialize(
	CellFaceAllPartsShapeParam		*allPartsShapeParam,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*allPartsResult,
	const CellFacePartsResult		*allPartsShapeResult
)
{
	allPartsShapeParam->eaShapeDict		= cellFaceAllPartsShapeGetDictEa();
	allPartsShapeParam->eaPartsResult	= (uintptr_t)allPartsResult;
	allPartsShapeParam->eaShapeResult	= (uintptr_t)allPartsShapeResult;
	allPartsShapeParam->faceYaw			= face->faceYaw;
}

static inline
void cellFaceUtilPartsAttribParamInitialize(
	CellFacePartsAttribParam		*partsAttribParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*partsResult,
	const CellFaceAttribResult		*attribResult,
	const CellFacePosition			*positionResult
)
{
	partsAttribParam->eaImage			= (uintptr_t)imageData;
	partsAttribParam->imageWidth		= imageWidth;
	partsAttribParam->imageHeight		= imageHeight;
	partsAttribParam->imageRowstride	= imageRowstride;
	partsAttribParam->eaWorkingArea		= (uintptr_t)workingArea;
	partsAttribParam->eaPartsResult		= (uintptr_t)partsResult;
	partsAttribParam->eaAttribResult	= (uintptr_t)attribResult;
	partsAttribParam->eaPositionResult	= (uintptr_t)positionResult;
	partsAttribParam->eaPartsDict		= cellFacePartsGetDictEa();
	partsAttribParam->eaAttribDict		= cellFaceAttribGetDictEa();
	partsAttribParam->faceX				= face->faceX;
	partsAttribParam->faceY				= face->faceY;
	partsAttribParam->faceW				= face->faceW;
	partsAttribParam->faceH				= face->faceH;
	partsAttribParam->faceRoll			= face->faceRoll;
	partsAttribParam->facePitch			= face->facePitch;
	partsAttribParam->faceYaw			= face->faceYaw;
}

static inline
void cellFaceUtilPartsAttribExParamInitialize(
	CellFacePartsAttribParam		*partsAttribParam,
	const void 						*imageData,
	uint32_t						imageWidth,
	uint32_t						imageHeight,
	uint32_t						imageRowstride,
	const void						*workingArea,
	const CellFaceDetectionResult	*face,
	const CellFacePartsResult		*partsResult,
	const CellFaceAttribResult		*attribResult,
	const CellFacePosition			*positionResult
)
{
	partsAttribParam->eaImage			= (uintptr_t)imageData;
	partsAttribParam->imageWidth		= imageWidth;
	partsAttribParam->imageHeight		= imageHeight;
	partsAttribParam->imageRowstride	= imageRowstride;
	partsAttribParam->eaWorkingArea		= (uintptr_t)workingArea;
	partsAttribParam->eaPartsResult		= (uintptr_t)partsResult;
	partsAttribParam->eaAttribResult	= (uintptr_t)attribResult;
	partsAttribParam->eaPositionResult	= (uintptr_t)positionResult;
	partsAttribParam->eaPartsDict		= cellFacePartsGetDictEa();
	partsAttribParam->eaAttribDict		= cellFaceAttribExGetDictEa();
	partsAttribParam->faceX				= face->faceX;
	partsAttribParam->faceY				= face->faceY;
	partsAttribParam->faceW				= face->faceW;
	partsAttribParam->faceH				= face->faceH;
	partsAttribParam->faceRoll			= face->faceRoll;
	partsAttribParam->facePitch			= face->facePitch;
	partsAttribParam->faceYaw			= face->faceYaw;
}

static inline
void cellFaceUtilFeatureParamInitialize(
	CellFaceFeatureParam	*featureParam,
	const void				*imageData,
	uint32_t				imageWidth,
	uint32_t				imageHeight,
	uint32_t				imageRowstride,
	const void				*workingArea,
	const CellFacePosition	*position,
	const CellFaceFeature	*featureResult
)
{
	featureParam->eaImage			= (uintptr_t)imageData;
	featureParam->imageWidth		= imageWidth;
	featureParam->imageHeight		= imageHeight;
	featureParam->imageRowstride	= imageRowstride;
	featureParam->eaWorkingArea		= (uintptr_t)workingArea;
	featureParam->eaPosition		= (uintptr_t)position;
	featureParam->eaFeatureResult	= (uintptr_t)featureResult;
}

static inline
void cellFaceUtilSimilarityParamInitialize(
	CellFaceSimilarityParam	*similarityParam,
	const CellFaceFeature	*feature,
	const CellFaceFeature	*regFeatureArray,
	uint32_t				numRegFeature,
	uint32_t				strideRegFeature,
	const float				*scoreResultArray
)
{
	similarityParam->eaFeature			= (uintptr_t)feature;
	similarityParam->eaRegFeatureArray	= (uintptr_t)regFeatureArray;
	similarityParam->eaSimilarityDict	= cellFaceSimilarityGetDictEa();
	similarityParam->eaScoreResultArray	= (uintptr_t)scoreResultArray;
	similarityParam->numRegFeature		= numRegFeature;
	similarityParam->strideRegFeature	= strideRegFeature;	/* must be multiple of 16 */
}

#endif /* __PPU__ */

#endif /* __CELL_FACE_UTIL_H__ */
