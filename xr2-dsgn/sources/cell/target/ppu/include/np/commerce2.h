/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *
 *      Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 */

#ifndef _SCE_NP_COMMERCE2_H_
#define _SCE_NP_COMMERCE2_H_

#include <cell/rtc.h>
#include <sys/memory.h>
#include <np/common.h>
#include <np/error.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*
 * Np Commerce2 Context
 */
#define SCE_NP_COMMERCE2_CTX_MAX						1
#define SCE_NP_COMMERCE2_REQ_MAX						1

/*
 * Np Commerce2 Current version
 */
#define SCE_NP_COMMERCE2_VERSION					2

/*
 * Np Commerce2 string length (without NULL termination)
 */
#define SCE_NP_COMMERCE2_CURRENCY_CODE_LEN              3
#define SCE_NP_COMMERCE2_CURRENCY_SYMBOL_LEN            3
#define SCE_NP_COMMERCE2_THOUSAND_SEPARATOR_LEN         4
#define SCE_NP_COMMERCE2_DECIMAL_LETTER_LEN             4
#define SCE_NP_COMMERCE2_SP_NAME_LEN                    256
#define SCE_NP_COMMERCE2_CATEGORY_ID_LEN                56
#define SCE_NP_COMMERCE2_CATEGORY_NAME_LEN              256
#define SCE_NP_COMMERCE2_CATEGORY_DESCRIPTION_LEN       1024
#define SCE_NP_COMMERCE2_PRODUCT_ID_LEN                 48
#define SCE_NP_COMMERCE2_PRODUCT_NAME_LEN               256
#define SCE_NP_COMMERCE2_PRODUCT_SHORT_DESCRIPTION_LEN  1024
#define SCE_NP_COMMERCE2_PRODUCT_LONG_DESCRIPTION_LEN   4000
#define SCE_NP_COMMERCE2_SKU_ID_LEN                     56
#define SCE_NP_COMMERCE2_SKU_NAME_LEN                   180
#define SCE_NP_COMMERCE2_URL_LEN                        256
#define SCE_NP_COMMERCE2_RATING_SYSTEM_ID_LEN           16
#define SCE_NP_COMMERCE2_RATING_DESCRIPTION_LEN         60
#define SCE_NP_COMMERCE2_PRODUCT_CODE_BLOCK_LEN         4

/* max count of sku in checking out */
#define SCE_NP_COMMERCE2_SKU_CHECKOUT_MAX			16
#define SCE_NP_COMMERCE2_SKU_DL_LIST_MAX			16

#define SCE_NP_COMMERCE2_RECV_BUF_SIZE (256 * 1024)

/*
 * Np Commerce2 Event
 */
#define SCE_NP_COMMERCE2_EVENT_REQUEST_ERROR				0x0001

#define SCE_NP_COMMERCE2_EVENT_CREATE_SESSION_DONE			0x0011
#define SCE_NP_COMMERCE2_EVENT_CREATE_SESSION_ABORT			0x0012

#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_STARTED			0x0021
#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_SUCCESS			0x0022
#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_BACK				0x0023
#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_FINISHED			0x0024

#define SCE_NP_COMMERCE2_EVENT_DO_DL_LIST_STARTED			0x0031
#define SCE_NP_COMMERCE2_EVENT_DO_DL_LIST_SUCCESS			0x0032
#define SCE_NP_COMMERCE2_EVENT_DO_DL_LIST_FINISHED			0x0034

#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_STARTED		0x0041
#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_SUCCESS		0x0042
#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_BACK			0x0043
#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_FINISHED		0x0044
#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_OPENED		0x0045

#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_STARTED		0x0051
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_SUCCESS		0x0052
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_BACK			0x0053
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_FINISHED		0x0054

/* memory container size */
#define SCE_NP_COMMERCE2_DO_CHECKOUT_MEMORY_CONTAINER_SIZE (10*1024*1024)
#define SCE_NP_COMMERCE2_DO_DL_LIST_MEMORY_CONTAINER_SIZE  (10*1024*1024)
#define SCE_NP_COMMERCE2_DO_PROD_BROWSE_MEMORY_CONTAINER_SIZE (16*1024*1024)
#define SCE_NP_COMMERCE2_DO_PRODUCT_CODE_MEMORY_CONTAINER_SIZE (16*1024*1024)

/*
 * Np Commerce2 Handler
 */
typedef void (*SceNpCommerce2Handler)(
	uint32_t ctx_id,
	uint32_t subject_id,
	int event,
	int error_code,
	void *arg
	);

/*
 * Np Commerce2 API
 */
int
sceNpCommerce2Init(
	void
	);

int
sceNpCommerce2Term(
	void
	);

int
sceNpCommerce2CreateCtx(
	uint32_t version,
	const SceNpId *npId,
	SceNpCommerce2Handler handler,
	void *arg,
	uint32_t *ctx_id
	);

int
sceNpCommerce2DestroyCtx(
	uint32_t ctx_id
	);

int
sceNpCommerce2DestroyReq(
	uint32_t req_id
	);

int
sceNpCommerce2AbortReq(
	uint32_t req_id
	);

int
sceNpCommerce2CreateSessionStart(
	uint32_t ctx_id
	);

int
sceNpCommerce2CreateSessionAbort(
	uint32_t ctx_id
	);

/* symbolPosition */
#define SCE_NP_COMMERCE2_SYM_POS_PRE   0
#define SCE_NP_COMMERCE2_SYM_POS_POST  1

typedef struct SceNpCommerce2SessionInfo_
{
	char currencyCode[SCE_NP_COMMERCE2_CURRENCY_CODE_LEN + 1];
	uint32_t decimals;
	char currencySymbol[SCE_NP_COMMERCE2_CURRENCY_SYMBOL_LEN + 1];
	uint32_t symbolPosition;    /* SCE_NP_COMMERCE2_SYM_POS_XXX */
	bool symbolWithSpace;
	char thousandSeparator[SCE_NP_COMMERCE2_THOUSAND_SEPARATOR_LEN + 1];
	char decimalLetter[SCE_NP_COMMERCE2_DECIMAL_LETTER_LEN + 1];
	char padding[1];
	uint32_t reserved[4];
} SceNpCommerce2SessionInfo;

int
sceNpCommerce2CreateSessionFinish(
	uint32_t ctx_id,
	SceNpCommerce2SessionInfo *sessionInfo
	);

int
sceNpCommerce2GetPrice(
	uint32_t ctx_id,
	char *buf,
	size_t buflen,
	uint32_t price
	);

/* checkout */
int
sceNpCommerce2DoCheckoutStartAsync(
	uint32_t ctx_id,
	const char *sku_ids[],
	uint32_t sku_num,
	sys_memory_container_t container
	);

int
sceNpCommerce2DoCheckoutFinishAsync(
	uint32_t ctx_id
	);

/* download list */
int
sceNpCommerce2DoDlListStartAsync(
	uint32_t ctx_id,
	const char *service_id,
	const char *sku_ids[],
	uint32_t sku_num,
	sys_memory_container_t container
	);

int
sceNpCommerce2DoDlListFinishAsync(
	uint32_t ctx_id
	);

/* product browse */

typedef struct SceNpCommerce2ProductBrowseParam
{
	size_t size;
} SceNpCommerce2ProductBrowseParam;

int
sceNpCommerce2DoProductBrowseStartAsync(
	uint32_t ctx_id,
	const char *product_id,
	sys_memory_container_t container,
	const SceNpCommerce2ProductBrowseParam *param
	);

int
sceNpCommerce2DoProductBrowseFinishAsync(
	uint32_t ctx_id
	);

/* product code */
#define SCE_NP_COMMERCE2_PRODUCT_CODE_INPUT_MODE_USER_INPUT       0
#define SCE_NP_COMMERCE2_PRODUCT_CODE_INPUT_MODE_CODE_SPECIFIED   1

typedef struct SceNpCommerce2ProductCodeParam
{
	size_t size;
	unsigned int inputMode; /* SCE_NP_COMMERCE2_PRODUCT_CODE_INPUT_MODE_XXX */
	char code1[SCE_NP_COMMERCE2_PRODUCT_CODE_BLOCK_LEN + 1];
	char padding1[3];
	char code2[SCE_NP_COMMERCE2_PRODUCT_CODE_BLOCK_LEN + 1];
	char padding2[3];
	char code3[SCE_NP_COMMERCE2_PRODUCT_CODE_BLOCK_LEN + 1];
	char padding3[3];
} SceNpCommerce2ProductCodeParam;

int
sceNpCommerce2DoProductCodeStartAsync(
	uint32_t ctx_id,
	sys_memory_container_t container,
	const SceNpCommerce2ProductCodeParam *param
	);

int
sceNpCommerce2DoProductCodeFinishAsync(
	uint32_t ctx_id
	);


/* store browse */

/* target type */
#define SCE_NP_COMMERCE2_STORE_BROWSE_TYPE_CATEGORY        1
#define SCE_NP_COMMERCE2_STORE_BROWSE_TYPE_PRODUCT         2
#define SCE_NP_COMMERCE2_STORE_BROWSE_TYPE_PRODUCT_CODE    3

int
sceNpCommerce2ExecuteStoreBrowse(
	int targetType,
	const char *targetId,
	int userdata
	);
int
sceNpCommerce2GetStoreBrowseUserdata(
	int *userdata
	);

/* set/get option */

int
sceNpCommerce2GetBGDLAvailability(
	bool *bgdlAvailability
	);
int
sceNpCommerce2SetBGDLAvailability(
	bool bgdlAvailability
	);

/*
 * accessor API
 */
typedef struct SceNpCommerce2CommonData_
{
	uint32_t version;
	const void *buf_head;
	const void *buf_size;
	const void *data;
	size_t data_size;
	const void *data2;
	unsigned int ov;
	uint32_t reserved[3];
} SceNpCommerce2CommonData;

typedef struct SceNpCommerce2Range_
{
	uint32_t startPosition;
	uint32_t count;
	uint32_t totalCountOfResults;
} SceNpCommerce2Range;

/* ContentRatingInfo */
#define SCE_NP_COMMERCE2_CONTENT_RATING_DESC_TYPE_ICON  1
#define SCE_NP_COMMERCE2_CONTENT_RATING_DESC_TYPE_TEXT  2
typedef struct SceNpCommerce2ContentRatingDescriptor_
{
	SceNpCommerce2CommonData commonData;

	/* value */
	uint32_t descriptorType;
	const char *imageUrl;                  /* valid only if descType is icon */
	const char *contentRatingDescription;  /* valid only if descType is text */
} SceNpCommerce2ContentRatingDescriptor;

typedef struct SceNpCommerce2ContentRatingInfo_
{
	SceNpCommerce2CommonData commonData;

	/* value */
	const char *ratingSystemId;
	const char *imageUrl;
	uint32_t countOfContentRatingDescriptor;
} SceNpCommerce2ContentRatingInfo;

int
sceNpCommerce2GetContentRatingDescriptor(
	const SceNpCommerce2ContentRatingInfo *contentRatingInfo,
	unsigned int index,
	SceNpCommerce2ContentRatingDescriptor *contentRatingDescriptor
	);

/* Category */
enum SceNpCommerce2CategoryDataType {
	SCE_NP_COMMERCE2_CAT_DATA_TYPE_THIN = 0,
	SCE_NP_COMMERCE2_CAT_DATA_TYPE_NORMAL,
	SCE_NP_COMMERCE2_CAT_DATA_TYPE_MAX
};
typedef struct SceNpCommerce2CategoryInfo_
{
	SceNpCommerce2CommonData commonData;
	enum SceNpCommerce2CategoryDataType dataType;

	/* value */
	const char *categoryId;
	CellRtcTick releaseDate;
	const char *categoryName;
	const char *categoryDescription;
	const char *imageUrl;
	const char *spName;

	/*E valid only if dataType == SCE_NP_COMMERCE2_CAT_DATA_TYPE_NORMAL */
	uint32_t countOfSubCategory;
	uint32_t countOfProduct;
} SceNpCommerce2CategoryInfo;

/* Game SKU */
#define SCE_NP_COMMERCE2_SKU_PURCHASABILITY_FLAG_ON   1
#define SCE_NP_COMMERCE2_SKU_PURCHASABILITY_FLAG_OFF  0

#define SCE_NP_COMMERCE2_SKU_ANN_PURCHASED_CANNOT_PURCHASE_AGAIN  0x80000000
#define SCE_NP_COMMERCE2_SKU_ANN_PURCHASED_CAN_PURCHASE_AGAIN     0x40000000
#define SCE_NP_COMMERCE2_SKU_ANN_IN_THE_CART                      0x20000000
#define SCE_NP_COMMERCE2_SKU_ANN_CONTENTLINK_SKU                  0x10000000

enum SceNpCommerce2GameSkuDataType {
	SCE_NP_COMMERCE2_GAME_SKU_DATA_TYPE_THIN = 0,
	SCE_NP_COMMERCE2_GAME_SKU_DATA_TYPE_NORMAL,
	SCE_NP_COMMERCE2_GAME_SKU_DATA_TYPE_MAX
};

typedef struct SceNpCommerce2GameSkuInfo_
{
	SceNpCommerce2CommonData commonData;
	enum SceNpCommerce2GameSkuDataType dataType;

	/* value */
	const char *skuId;
	uint32_t skuType;
	uint32_t countUntilExpiration;
	uint32_t timeUntilExpiration;
	uint32_t purchasabilityFlag;
	uint32_t annotation;
	bool downloadable;
	uint8_t padding[3];
	uint32_t price;
	const char *skuName;

	/*E valid only if dataType == SCE_NP_COMMERCE2_GAME_SKU_DATA_TYPE_NORMAL */
	const char *productId;
	const char *contentLinkUrl;

	uint32_t countOfRewardInfo;
	uint32_t reserved[8];
} SceNpCommerce2GameSkuInfo;

/* Game Product */
enum SceNpCommerce2GameProductDataType {
	SCE_NP_COMMERCE2_GAME_PRODUCT_DATA_TYPE_THIN = 0,
	SCE_NP_COMMERCE2_GAME_PRODUCT_DATA_TYPE_NORMAL,
	SCE_NP_COMMERCE2_GAME_PRODUCT_DATA_TYPE_MAX
};

typedef struct SceNpCommerce2GameProductInfo_
{
	SceNpCommerce2CommonData commonData;
	enum SceNpCommerce2GameProductDataType dataType;

	/* value */
	const char *productId;
	uint32_t countOfSku;
	uint8_t padding[4];
	CellRtcTick releaseDate;
	const char *productName;
	const char *productShortDescription;
	const char *imageUrl;
	const char *spName;

	/*E valid only if dataType == SCE_NP_COMMERCE2_GAME_PRODUCT_DATA_TYPE_NORMAL */
	const char *productLongDescription;
	const char *legalDescription;
} SceNpCommerce2GameProductInfo;

int
sceNpCommerce2GetGameSkuInfoFromGameProductInfo(
	const SceNpCommerce2GameProductInfo *gameProductInfo,
	uint32_t index,
	SceNpCommerce2GameSkuInfo *gameSkuInfo
	);

/*
 * Content
 */
#define SCE_NP_COMMERCE2_CONTENT_TYPE_CATEGORY  1
#define SCE_NP_COMMERCE2_CONTENT_TYPE_PRODUCT   2
typedef struct SceNpCommerce2ContentInfo_
{
	SceNpCommerce2CommonData commonData;

	/* value */
	uint32_t contentType;
} SceNpCommerce2ContentInfo;

int
sceNpCommerce2GetCategoryInfoFromContentInfo(
	const SceNpCommerce2ContentInfo *contentInfo,
	SceNpCommerce2CategoryInfo *categoryInfo
	);
int
sceNpCommerce2GetGameProductInfoFromContentInfo(
	const SceNpCommerce2ContentInfo *contentInfo,
	SceNpCommerce2GameProductInfo *gameProductInfo
	);


/*
 * GetCategoryContents
 */
#define SCE_NP_COMMERCE2_GETCAT_MAX_COUNT 60

typedef struct SceNpCommerce2GetCategoryContentsResult_
{
	SceNpCommerce2CommonData commonData;

	/* value */
	SceNpCommerce2Range rangeOfContents;
} SceNpCommerce2GetCategoryContentsResult;

int
sceNpCommerce2GetCategoryContentsCreateReq(
	uint32_t ctx_id,
	uint32_t *req_id
	);
int
sceNpCommerce2GetCategoryContentsStart(
	uint32_t req_id,
	const char *categoryId,
	uint32_t startPosition,
	uint32_t maxCountOfResults
	);
int
sceNpCommerce2GetCategoryContentsGetResult(
	uint32_t req_id,
	void *buf,
	size_t buf_size,
	size_t *fill_size
	);
int
sceNpCommerce2InitGetCategoryContentsResult(
	SceNpCommerce2GetCategoryContentsResult *result,
	void *data,
	size_t data_size
	);
int
sceNpCommerce2DestroyGetCategoryContentsResult(
	SceNpCommerce2GetCategoryContentsResult *result
	);
int
sceNpCommerce2GetCategoryInfo(
	const SceNpCommerce2GetCategoryContentsResult *result,
	SceNpCommerce2CategoryInfo *categoryInfo
	);
int
sceNpCommerce2GetContentInfo(
	const SceNpCommerce2GetCategoryContentsResult *result,
	unsigned int index,
	SceNpCommerce2ContentInfo *contentInfo
	);
int
sceNpCommerce2GetContentRatingInfoFromCategoryInfo(
	const SceNpCommerce2CategoryInfo *categoryInfo,
	SceNpCommerce2ContentRatingInfo *contentRatingInfo
	);

/*
 * GetProductInfo
 */
typedef struct SceNpCommerce2GetProductInfoResult_
{
	SceNpCommerce2CommonData commonData;

} SceNpCommerce2GetProductInfoResult;

int
sceNpCommerce2GetProductInfoCreateReq(
	uint32_t ctx_id,
	uint32_t *req_id
	);
int
sceNpCommerce2GetProductInfoStart(
	uint32_t req_id,
	const char *categoryId,
	const char *productId
	);
int
sceNpCommerce2GetProductInfoGetResult(
	uint32_t req_id,
	void *buf,
	size_t buf_size,
	size_t *fill_size
	);
int
sceNpCommerce2InitGetProductInfoResult(
	SceNpCommerce2GetProductInfoResult *result,
	void *data,
	size_t data_size
	);
int
sceNpCommerce2DestroyGetProductInfoResult(
	SceNpCommerce2GetProductInfoResult *result
	);
int
sceNpCommerce2GetGameProductInfo(
	const SceNpCommerce2GetProductInfoResult *result,
	SceNpCommerce2GameProductInfo *gameProductInfo
	);
int
sceNpCommerce2GetContentRatingInfoFromGameProductInfo(
	const SceNpCommerce2GameProductInfo *gameProductInfo,
	SceNpCommerce2ContentRatingInfo *contentRatingInfo
	);

/*
 * GetProductInfoList
 */
#define SCE_NP_COMMERCE2_GETPRODLIST_MAX_COUNT 60

typedef struct SceNpCommerce2GetProductInfoListResult_
{
	SceNpCommerce2CommonData commonData;

	/* value */
	unsigned int countOfProductInfo;
} SceNpCommerce2GetProductInfoListResult;

int
sceNpCommerce2GetProductInfoListCreateReq(
	uint32_t ctx_id,
	uint32_t *req_id
	);
int
sceNpCommerce2GetProductInfoListStart(
	uint32_t req_id,
	const char *productIds[],
	uint32_t productNum
	);
int
sceNpCommerce2GetProductInfoListGetResult(
	uint32_t req_id,
	void *buf,
	size_t buf_size,
	size_t *fill_size
	);
int
sceNpCommerce2InitGetProductInfoListResult(
	SceNpCommerce2GetProductInfoListResult *result,
	void *data,
	size_t data_size
	);
int
sceNpCommerce2DestroyGetProductInfoListResult(
	SceNpCommerce2GetProductInfoListResult *result
	);
int
sceNpCommerce2GetGameProductInfoFromGetProductInfoListResult(
	const SceNpCommerce2GetProductInfoListResult *result,
	unsigned int index,
	SceNpCommerce2GameProductInfo *gameProductInfo
	);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /* _SCE_NP_COMMERCE2_H_ */
