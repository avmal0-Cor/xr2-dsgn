/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SOUND_MIXER_ERROR_H__
#define __CELL_SOUND_MIXER_ERROR_H__

#include <cell/error.h>



#if defined(__cplusplus)
extern "C" {
#endif

/*
  CELL_ERROR_FACILITY_SOUND       0x031
  libmixer       : 0x8031_0001 - 0x8031_00ff
*/

/*
#define CELL_SOUND_MAKE_ERROR(status) \
	CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SOUND, status)
*/

#define  CELL_LIBMIXER_ERROR_UNDEFINED          CELL_ERROR_CAST(0x80310001)
#define  CELL_LIBMIXER_ERROR_NOT_INITIALIZED    CELL_ERROR_CAST(0x80310002)
#define  CELL_LIBMIXER_ERROR_INVALID_PARAMETER  CELL_ERROR_CAST(0x80310003)
#define  CELL_LIBMIXER_ERROR_NO_MEMORY          CELL_ERROR_CAST(0x80310005)
#define  CELL_LIBMIXER_ERROR_ALREADY_EXIST      CELL_ERROR_CAST(0x80310006)
#define  CELL_LIBMIXER_ERROR_FULL               CELL_ERROR_CAST(0x80310007)
#define  CELL_LIBMIXER_ERROR_NOT_EXIST          CELL_ERROR_CAST(0x80310008)
#define  CELL_LIBMIXER_ERROR_TYPE_MISMATCH      CELL_ERROR_CAST(0x80310009)
#define  CELL_LIBMIXER_ERROR_NOT_FOUND          CELL_ERROR_CAST(0x8031000A)
#define  CELL_LIBMIXER_ERROR_NO_EFFECT          CELL_ERROR_CAST(0x8031000B)



#if defined(__cplusplus)
}
#endif

/*
 * Local variables:
 * indent-tabs-mode: t
 * tab-width: 4
 * End:
 */
#endif  /* __CELL_SOUND_MIXER_ERROR_H__ */

