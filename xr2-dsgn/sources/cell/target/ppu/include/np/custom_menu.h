/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_CUSTOM_MENU_H_
#define _SCE_NP_CUSTOM_MENU_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define SCE_NP_CUSTOM_MENU_ACTION_CHARACTER_MAX					(21)
#define SCE_NP_CUSTOM_MENU_ACTION_ITEMS_MAX						(7)
#define SCE_NP_CUSTOM_MENU_ACTION_ITEMS_TOTAL_MAX				(16)
#define SCE_NP_CUSTOM_MENU_EXCEPTION_ITEMS_MAX					(256)

/* custom menu index array */
#define SCE_NP_CUSTOM_MENU_INDEX_SETSIZE	(64)
#define SCE_NP_CUSTOM_MENU_INDEX_BITS		(sizeof(SceNpCustomMenuIndexMask) * 8)
#define SCE_NP_CUSTOM_MENU_INDEX_BITS_ALL	((SceNpCustomMenuIndexMask) - 1)
#define SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT	(5)
#define SCE_NP_CUSTOM_MENU_INDEX_BITS_MASK	(SCE_NP_CUSTOM_MENU_INDEX_BITS - 1)
#define SCE_NP_CUSTOM_MENU_INDEX_BITS_MAX	(SCE_NP_CUSTOM_MENU_INDEX_SETSIZE - 1)

#define SCE_NP_CUSTOM_MENU_INDEX_SET(n, p) \
	((p)->index_bits[(n) >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT] |= (1 << ((n) & SCE_NP_CUSTOM_MENU_INDEX_BITS_MASK)))
#define SCE_NP_CUSTOM_MENU_INDEX_CLR(n, p) \
	((p)->index_bits[(n) >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT] &= ~(1 << ((n) & SCE_NP_CUSTOM_MENU_INDEX_BITS_MASK)))
#define SCE_NP_CUSTOM_MENU_INDEX_ISSET(n, p) \
	((p)->index_bits[(n) >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT] & (1 << ((n) & SCE_NP_CUSTOM_MENU_INDEX_BITS_MASK)))
#define SCE_NP_CUSTOM_MENU_INDEX_ZERO(p) do { \
	SceNpCustomMenuIndexArray *__fa = (p); \
	uint32_t __i; \
	for(__i = 0; __i < (SCE_NP_CUSTOM_MENU_INDEX_SETSIZE >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT); __i++) \
		__fa->index_bits[__i] = 0; \
	} while(0)
#define SCE_NP_CUSTOM_MENU_INDEX_SET_ALL(p) do { \
	SceNpCustomMenuIndexArray *__fa = (p); \
	uint32_t __i; \
	for(__i = 0; __i < (SCE_NP_CUSTOM_MENU_INDEX_SETSIZE >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT); __i++) \
		__fa->index_bits[__i] = SCE_NP_CUSTOM_MENU_INDEX_BITS_ALL; \
	} while(0)

typedef uint32_t SceNpCustomMenuIndexMask;

typedef struct SceNpCustomMenuIndexArray {
	SceNpCustomMenuIndexMask index_bits[SCE_NP_CUSTOM_MENU_INDEX_SETSIZE >> SCE_NP_CUSTOM_MENU_INDEX_BITS_SHIFT];
} SceNpCustomMenuIndexArray;

#define SCE_NP_CUSTOM_MENU_ACTION_MASK_ALL   \
	(SCE_NP_CUSTOM_MENU_ACTION_MASK_ME     | \
	 SCE_NP_CUSTOM_MENU_ACTION_MASK_FRIEND | \
	 SCE_NP_CUSTOM_MENU_ACTION_MASK_PLAYER )
#define SCE_NP_CUSTOM_MENU_ACTION_MASK_ME		(0x00000001)
#define SCE_NP_CUSTOM_MENU_ACTION_MASK_FRIEND	(0x00000002)
#define SCE_NP_CUSTOM_MENU_ACTION_MASK_PLAYER	(0x00000004)

#define SCE_NP_CUSTOM_MENU_SELECTED_TYPE_ME		(1)
#define SCE_NP_CUSTOM_MENU_SELECTED_TYPE_FRIEND	(2)
#define SCE_NP_CUSTOM_MENU_SELECTED_TYPE_PLAYER	(3)

typedef uint32_t SceNpCustomMenuActionMask;
typedef uint32_t SceNpCustomMenuSelectedType;

typedef struct SceNpCustomMenuAction {
	uint32_t options;
	const char *name;
	SceNpCustomMenuActionMask mask;
} SceNpCustomMenuAction;

typedef struct SceNpCustomMenu {
	uint64_t options;
	SceNpCustomMenuAction *actions;
	size_t numActions;
} SceNpCustomMenu;

typedef struct SceNpCustomMenuActionExceptions {
	uint32_t options;
	SceNpId npid;
	SceNpCustomMenuIndexArray actions;
	uint8_t reserved[4];
} SceNpCustomMenuActionExceptions;

typedef int (*SceNpCustomMenuEventHandler)(
	int retCode,
	size_t index,
	const SceNpId *npid,
	SceNpCustomMenuSelectedType type,
	void *arg
	);

int sceNpCustomMenuRegisterActions(
	const SceNpCustomMenu *menu,
	SceNpCustomMenuEventHandler handler,
	void *userArg,
	uint64_t options
	);

int sceNpCustomMenuActionSetActivation(
	const SceNpCustomMenuIndexArray *array,
	uint64_t options
	);

int sceNpCustomMenuRegisterExceptionList(
	const SceNpCustomMenuActionExceptions *items,
	size_t numItems,
	uint64_t options
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_CUSTOM_MENU_H_ */
