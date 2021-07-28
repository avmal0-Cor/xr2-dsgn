/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_MUSIC_PLAYBACK_COOMON_H_
#define _CELL_SYSUTIL_MUSIC_PLAYBACK_COOMON_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

/*J コンテキストサイズ */
#define CELL_MUSIC_SELECTION_CONTEXT_SIZE				2048

typedef struct{
	char data[CELL_MUSIC_SELECTION_CONTEXT_SIZE];
} CellMusicSelectionContext;


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MUSIC_PLAYBACK_COOMON_H_*/
