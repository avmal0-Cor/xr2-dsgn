/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc., Sony Corporation
* All Rights Reserved.
*/

#ifndef __CELL_FACE_FACE_H__
#define __CELL_FACE_FACE_H__ 1

#include <stdint.h>

#define CELL_FACE_DETECT_NUM_MAX	512
#define CELL_FACE_IMG_WIDTH_MAX		2048

typedef struct CellFaceDetectionResult {
	float		faceX;
	float		faceY;
	float		faceW;
	float		faceH;
	float		faceRoll;
	float		facePitch;
	float		faceYaw;
	uint8_t		padding[4];
	uint32_t	angleLabel;
	int32_t		score;
	int32_t		averageScore;
	uint32_t	numDetect;
} __attribute__((aligned(16))) CellFaceDetectionResult;

typedef struct CellFaceDetectionParam {
	uint64_t	eaImage;
	uint64_t	eaWorkingArea;
	uint64_t	eaDetectionDict;		/* cellFaceDetectionGetDictEa(); */
	uint64_t	eaFaceResult;			/* (CellFaceDetectionResult*) */
	uint64_t	eaOptParamMultiSpu;		/* (CellFaceOptParamMultiSpu*) */
	uint64_t	eaOptParamLocalSearch;	/* (CellFaceOptParamLocalSearch*) */
	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint32_t	imageRowstride;
	uint32_t	minFaceSize;
	uint32_t	maxFaceSize;
	uint32_t	xScanStep;
	uint32_t	yScanStep;
	float		scalingStep;
	float		detectionThreshold;
	uint32_t	maxNumResult;
	uint8_t		padding[8];
} __attribute__((aligned(16))) CellFaceDetectionParam;

typedef struct CellFaceDetectionResultData {
	uint64_t	eaFaceResult;			/* (CellFaceDetectionResult*) */
	int32_t		numFace;
	uint8_t		padding[4];
} __attribute__((aligned(16))) CellFaceDetectionResultData;

typedef struct CellFaceOptParamMultiSpu {
	uint32_t	xScanStart;
	uint32_t	yScanStart;
	uint32_t	storeOverlap;
	uint8_t		padding[4];
} __attribute__((aligned(16))) CellFaceOptParamMultiSpu;

#define CELL_FACE_LOCAL_NUM_MAX			8

typedef struct CellFaceOptParamLocalSearch {
	uint64_t	eaLocalData;			/* (CellFaceDetectionResultData*) */
	uint32_t	numLocal;
	float		scanFaceSizeMin;
	float		scanFaceSizeMax;
	uint32_t	scanFaceMarginX;
	uint32_t	scanFaceMarginY;
	uint8_t		padding[4];
} __attribute__((aligned(16))) CellFaceOptParamLocalSearch;

#define CELL_FACE_MERGE_NUM_MAX			16

typedef struct CellFaceMergeResultParam {
	uint64_t	eaMergeData;			/* (CellFaceDetectionResultData*) */
	uint64_t	eaFaceResult;			/* (CellFaceDetectionResult*) */
	uint32_t	numMerge;
	uint32_t	maxNumResult;
	uint8_t		padding[8];
} __attribute__((aligned(16))) CellFaceMergeResultParam;

/* eye nose mouth */
#define CELL_FACE_PARTS_NUM_MAX			4 /* depend on dictionary */
#define CELL_FACE_PARTS_ID_UNDEF		0
#define CELL_FACE_PARTS_ID_R_EYE_CENTER	1
#define CELL_FACE_PARTS_ID_L_EYE_CENTER	2
#define CELL_FACE_PARTS_ID_NOSE_CENTER	3
#define CELL_FACE_PARTS_ID_MOUTH_CENTER	4

/* all parts */
#define CELL_FACE_ALL_PARTS_NUM_MAX		55 /* depend on dictionary */
#define CELL_FACE_PARTS_ID_ALL_BASE		100
#define CELL_FACE_PARTS_ID_ALL_00		(CELL_FACE_PARTS_ID_ALL_BASE+0)
#define CELL_FACE_PARTS_ID_ALL_01		(CELL_FACE_PARTS_ID_ALL_BASE+1)
#define CELL_FACE_PARTS_ID_ALL_02		(CELL_FACE_PARTS_ID_ALL_BASE+2)
#define CELL_FACE_PARTS_ID_ALL_03		(CELL_FACE_PARTS_ID_ALL_BASE+3)
#define CELL_FACE_PARTS_ID_ALL_04		(CELL_FACE_PARTS_ID_ALL_BASE+4)
#define CELL_FACE_PARTS_ID_ALL_05		(CELL_FACE_PARTS_ID_ALL_BASE+5)
#define CELL_FACE_PARTS_ID_ALL_06		(CELL_FACE_PARTS_ID_ALL_BASE+6)
#define CELL_FACE_PARTS_ID_ALL_07		(CELL_FACE_PARTS_ID_ALL_BASE+7)
#define CELL_FACE_PARTS_ID_ALL_08		(CELL_FACE_PARTS_ID_ALL_BASE+8)
#define CELL_FACE_PARTS_ID_ALL_09		(CELL_FACE_PARTS_ID_ALL_BASE+9)
#define CELL_FACE_PARTS_ID_ALL_10		(CELL_FACE_PARTS_ID_ALL_BASE+10)
#define CELL_FACE_PARTS_ID_ALL_11		(CELL_FACE_PARTS_ID_ALL_BASE+11)
#define CELL_FACE_PARTS_ID_ALL_12		(CELL_FACE_PARTS_ID_ALL_BASE+12)
#define CELL_FACE_PARTS_ID_ALL_13		(CELL_FACE_PARTS_ID_ALL_BASE+13)
#define CELL_FACE_PARTS_ID_ALL_14		(CELL_FACE_PARTS_ID_ALL_BASE+14)
#define CELL_FACE_PARTS_ID_ALL_15		(CELL_FACE_PARTS_ID_ALL_BASE+15)
#define CELL_FACE_PARTS_ID_ALL_16		(CELL_FACE_PARTS_ID_ALL_BASE+16)
#define CELL_FACE_PARTS_ID_ALL_17		(CELL_FACE_PARTS_ID_ALL_BASE+17)
#define CELL_FACE_PARTS_ID_ALL_18		(CELL_FACE_PARTS_ID_ALL_BASE+18)
#define CELL_FACE_PARTS_ID_ALL_19		(CELL_FACE_PARTS_ID_ALL_BASE+19)
#define CELL_FACE_PARTS_ID_ALL_20		(CELL_FACE_PARTS_ID_ALL_BASE+20)
#define CELL_FACE_PARTS_ID_ALL_21		(CELL_FACE_PARTS_ID_ALL_BASE+21)
#define CELL_FACE_PARTS_ID_ALL_22		(CELL_FACE_PARTS_ID_ALL_BASE+22)
#define CELL_FACE_PARTS_ID_ALL_23		(CELL_FACE_PARTS_ID_ALL_BASE+23)
#define CELL_FACE_PARTS_ID_ALL_24		(CELL_FACE_PARTS_ID_ALL_BASE+24)
#define CELL_FACE_PARTS_ID_ALL_25		(CELL_FACE_PARTS_ID_ALL_BASE+25)
#define CELL_FACE_PARTS_ID_ALL_26		(CELL_FACE_PARTS_ID_ALL_BASE+26)
#define CELL_FACE_PARTS_ID_ALL_27		(CELL_FACE_PARTS_ID_ALL_BASE+27)
#define CELL_FACE_PARTS_ID_ALL_28		(CELL_FACE_PARTS_ID_ALL_BASE+28)
#define CELL_FACE_PARTS_ID_ALL_29		(CELL_FACE_PARTS_ID_ALL_BASE+29)
#define CELL_FACE_PARTS_ID_ALL_30		(CELL_FACE_PARTS_ID_ALL_BASE+30)
#define CELL_FACE_PARTS_ID_ALL_31		(CELL_FACE_PARTS_ID_ALL_BASE+31)
#define CELL_FACE_PARTS_ID_ALL_32		(CELL_FACE_PARTS_ID_ALL_BASE+32)
#define CELL_FACE_PARTS_ID_ALL_33		(CELL_FACE_PARTS_ID_ALL_BASE+33)
#define CELL_FACE_PARTS_ID_ALL_34		(CELL_FACE_PARTS_ID_ALL_BASE+34)
#define CELL_FACE_PARTS_ID_ALL_35		(CELL_FACE_PARTS_ID_ALL_BASE+35)
#define CELL_FACE_PARTS_ID_ALL_36		(CELL_FACE_PARTS_ID_ALL_BASE+36)
#define CELL_FACE_PARTS_ID_ALL_37		(CELL_FACE_PARTS_ID_ALL_BASE+37)
//#define CELL_FACE_PARTS_ID_ALL_38		(CELL_FACE_PARTS_ID_ALL_BASE+38)	/* unsupported */
//#define CELL_FACE_PARTS_ID_ALL_39		(CELL_FACE_PARTS_ID_ALL_BASE+39)	/* unsupported */
#define CELL_FACE_PARTS_ID_ALL_40		(CELL_FACE_PARTS_ID_ALL_BASE+40)
#define CELL_FACE_PARTS_ID_ALL_41		(CELL_FACE_PARTS_ID_ALL_BASE+41)
#define CELL_FACE_PARTS_ID_ALL_42		(CELL_FACE_PARTS_ID_ALL_BASE+42)
//#define CELL_FACE_PARTS_ID_ALL_43		(CELL_FACE_PARTS_ID_ALL_BASE+43)	/* unsupported */
//#define CELL_FACE_PARTS_ID_ALL_44		(CELL_FACE_PARTS_ID_ALL_BASE+44)	/* unsupported */
#define CELL_FACE_PARTS_ID_ALL_45		(CELL_FACE_PARTS_ID_ALL_BASE+45)
#define CELL_FACE_PARTS_ID_ALL_46		(CELL_FACE_PARTS_ID_ALL_BASE+46)
#define CELL_FACE_PARTS_ID_ALL_47		(CELL_FACE_PARTS_ID_ALL_BASE+47)
#define CELL_FACE_PARTS_ID_ALL_48		(CELL_FACE_PARTS_ID_ALL_BASE+48)
#define CELL_FACE_PARTS_ID_ALL_49		(CELL_FACE_PARTS_ID_ALL_BASE+49)
#define CELL_FACE_PARTS_ID_ALL_50		(CELL_FACE_PARTS_ID_ALL_BASE+50)
#define CELL_FACE_PARTS_ID_ALL_51		(CELL_FACE_PARTS_ID_ALL_BASE+51)
#define CELL_FACE_PARTS_ID_ALL_52		(CELL_FACE_PARTS_ID_ALL_BASE+52)
#define CELL_FACE_PARTS_ID_ALL_53		(CELL_FACE_PARTS_ID_ALL_BASE+53)
#define CELL_FACE_PARTS_ID_ALL_54		(CELL_FACE_PARTS_ID_ALL_BASE+54)
#define CELL_FACE_PARTS_ID_ALL_55		(CELL_FACE_PARTS_ID_ALL_BASE+55)
#define CELL_FACE_PARTS_ID_ALL_56		(CELL_FACE_PARTS_ID_ALL_BASE+56)
#define CELL_FACE_PARTS_ID_ALL_57		(CELL_FACE_PARTS_ID_ALL_BASE+57)
#define CELL_FACE_PARTS_ID_ALL_58		(CELL_FACE_PARTS_ID_ALL_BASE+58)

typedef struct CellFacePartsResult {
	uint32_t	partsId;
	float		partsX;
	float		partsY;
	float		score;
} __attribute__((aligned(16))) CellFacePartsResult;

typedef struct CellFacePosition {
	float		regionX[4];
	float		regionY[4];
	float		pose[3];
	float		scale[3];
	float       shift[2];
} __attribute__((aligned(16))) CellFacePosition;

typedef struct CellFacePartsParam {
	uint64_t	eaImage;
	uint64_t	eaWorkingArea;
	uint64_t	eaPartsDict;		/* cellFacePartsGetDictEa(); */
	uint64_t	eaPartsResult;		/* (CellFacePartsResult*) */
	uint64_t	eaPositionResult;	/* (CellFacePosition*) */
	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint32_t	imageRowstride;
	float		faceX;
	float		faceY;
	float		faceW;
	float		faceH;
	float		faceRoll;
	float		facePitch;
	float		faceYaw;
} __attribute__((aligned(16))) CellFacePartsParam;

typedef struct CellFaceAllPartsParam {
	uint64_t	eaImage;
	uint64_t	eaWorkingArea;
	uint64_t	eaPartsDict;		/* cellFaceAllPartsGetDictEa(); */
	uint64_t	eaShapeDict;		/* reserved */
	uint64_t	eaPartsResult;		/* (CellFacePartsResult*) */
	uint64_t	eaShapeResult;		/* reserved */
	uint64_t	enablePartsBit;
	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint32_t	imageRowstride;
	float		faceX;
	float		faceY;
	float		faceW;
	float		faceH;
	float		faceRoll;
	float		facePitch;
	float		faceYaw;
} __attribute__((aligned(16))) CellFaceAllPartsParam;

typedef struct CellFaceAllPartsShapeParam {
	uint64_t	eaShapeDict;		/* cellFaceAllPartsGetShapeDictEa(); */
	uint64_t	eaPartsResult;		/* (CellFacePartsResult*) */
	uint64_t	eaShapeResult;		/* (CellFacePartsResult*) */
	float		faceYaw;
	uint8_t		padding[4];
} __attribute__((aligned(16))) CellFaceAllPartsShapeParam;

#define CELL_FACE_ATTRIB_NUM_MAX		8 /* depend on dictionary */
#define CELL_FACE_ATTRIB_ID_UNDEF		0
#define CELL_FACE_ATTRIB_ID_SMILE		1
#define CELL_FACE_ATTRIB_ID_REYEOPEN	2
#define CELL_FACE_ATTRIB_ID_LEYEOPEN	3
#define CELL_FACE_ATTRIB_ID_GENDER		4
#define CELL_FACE_ATTRIB_ID_ADULT		5
#define CELL_FACE_ATTRIB_ID_BABY		6
#define CELL_FACE_ATTRIB_ID_ELDER		7
#define CELL_FACE_ATTRIB_ID_GLASS		8

typedef struct CellFaceAttribResult {
	uint32_t	attribId;
	float		score;
	uint8_t		padding[8];
} __attribute__((aligned(16))) CellFaceAttribResult;

typedef struct CellFacePartsAttribParam {
	uint64_t	eaImage;
	uint64_t	eaWorkingArea;
	uint64_t	eaPartsDict;		/* cellFacePartsGetDictEa(); */
	uint64_t	eaAttribDict;		/* cellFaceAttribGetDictEa(); */
	uint64_t	eaPartsResult;		/* (CellFacePartsResult*) */
	uint64_t	eaAttribResult;		/* (CellFaceAttribResult*) */
	uint64_t	eaPositionResult;	/* (CellFacePosition*) */
	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint32_t	imageRowstride;
	float		faceX;
	float		faceY;
	float		faceW;
	float		faceH;
	float		faceRoll;
	float		facePitch;
	float		faceYaw;
} __attribute__((aligned(16))) CellFacePartsAttribParam;

#define CELL_FACE_FEATURE_SIZE 16384

typedef struct CellFaceFeature {
	uint8_t		data[CELL_FACE_FEATURE_SIZE];
} __attribute__((aligned(16))) CellFaceFeature;

typedef struct CellFaceFeatureParam {
	uint64_t	eaImage;
	uint64_t	eaWorkingArea;
	uint64_t	eaPosition;			/* (CellFacePosition*) */
	uint64_t	eaFeatureResult;	/* (CellFaceFeature*) */
	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint32_t	imageRowstride;
	uint8_t		padding[4];
} __attribute__((aligned(16))) CellFaceFeatureParam;

typedef struct CellFaceSimilarityParam {
	uint64_t	eaFeature;			/* (CellFaceFeature*) */
	uint64_t	eaRegFeatureArray;	/* (CellFaceFeature*) */
	uint64_t	eaSimilarityDict;	/* cellFaceSimilarityGetDictEa(); */
	uint64_t	eaScoreResultArray;	/* float score[numRegFeature] */
	uint32_t	numRegFeature;
	uint32_t	strideRegFeature;	/* must be multiple of 16 */
	uint8_t		padding[8];
} __attribute__((aligned(16))) CellFaceSimilarityParam;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __PPU__

uint64_t cellFaceDetectionGetDictEa(void);

uint64_t cellFacePartsGetDictEa(void);

uint64_t cellFaceAllPartsGetDictEa(void);

uint64_t cellFaceAllPartsShapeGetDictEa(void);

uint64_t cellFaceAttribGetDictEa(void);

uint64_t cellFaceAttribExGetDictEa(void);

uint64_t cellFaceSimilarityGetDictEa(void);

#endif /* __PPU__ */

#ifdef __SPU__
int32_t cellFaceDetection(
	CellFaceDetectionParam		*detectParam,
	int32_t						*numFace
);

int32_t cellFaceDetectionFrontal(
	CellFaceDetectionParam		*detectParam,
	int32_t						*numFace
);

int32_t cellFaceMergeResult(
	CellFaceMergeResultParam	*mergeParam,
	int32_t						*numFace
);

int32_t cellFaceParts(
	CellFacePartsParam			*partsParam
);

int32_t cellFaceAllParts(
	CellFaceAllPartsParam		*allPartsParam
);

int32_t cellFaceAllPartsShapeConstraint(
	CellFaceAllPartsShapeParam	*shapeParam
);

int32_t cellFacePartsAttribute(
	CellFacePartsAttribParam	*partsAttribParam
);

int32_t cellFacePartsAttributeEx(
	CellFacePartsAttribParam	*partsAttribParam
);

int32_t cellFaceFeature(
	CellFaceFeatureParam		*featureParam
);

int32_t cellFaceSimilarity(
	CellFaceSimilarityParam		*simParam,
	float						*maxScore,
	int32_t						*maxScoreId
);

int32_t cellFaceSetDmaTag(
	uint32_t					tag0,
	uint32_t					tag1,
	uint32_t					tag2,
	uint32_t					tag3
);

#endif /* __SPU__ */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __CELL_FACE_FACE_H__ */
