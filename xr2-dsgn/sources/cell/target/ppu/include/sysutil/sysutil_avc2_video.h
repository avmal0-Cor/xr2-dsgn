/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_AVC2_VOICE_H_
#define _CELL_SYSUTIL_AVC2_VOICE_H_

#include "np.h"
#include "sysutil_common.h"
#include "sysutil_avc2.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J リターンーコード ----------------------------------------------------- */
//


/*J イベント ------------------------------------------------------------- */
//


/*J 定数定義 ------------------------------------------------------------- */
/*J AVチャット2サービスのウィンドウ属性 */
typedef enum {
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_ALPHA                    = 0x00002001,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_TRANSITION_TYPE          = 0x00002002,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_TRANSITION_DURATION      = 0x00002003,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_STRING_VISIBLE           = 0x00002004,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_ROTATION                 = 0x00002005,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_ZORDER                   = 0x00002006,
    CELL_SYSUTIL_AVC2_WINDOW_ATTRIBUTE_SURFACE					= 0x00002007,
} CellSysutilAvc2WindowAttributeId;

/*J AVチャット2サービスのアニメーション種別 */
typedef enum {
    CELL_SYSUTIL_AVC2_TRANSITION_NONE                           = 0xffffffff,
    CELL_SYSUTIL_AVC2_TRANSITION_LINEAR                         = 0x00000000,
    CELL_SYSUTIL_AVC2_TRANSITION_SLOWDOWN                       = 0x00000001,
    CELL_SYSUTIL_AVC2_TRANSITION_FASTUP                         = 0x00000002,
    CELL_SYSUTIL_AVC2_TRANSITION_ANGULAR                        = 0x00000003,
    CELL_SYSUTIL_AVC2_TRANSITION_EXPONENT                       = 0x00000004,
} CellSysutilAvc2TransitionType;

/*J AVチャット2サービスのＺオーダーモード */
typedef enum
{
    CELL_SYSUTIL_AVC2_ZORDER_FORWARD_MOST                       = 0x00000001,
    CELL_SYSUTIL_AVC2_ZORDER_BEHIND_MOST                        = 0x00000002,
} CellSysutilAvc2WindowZorderMode;

/*J 特殊な意味があるroom member id */
#define AVC2_SPECIAL_ROOM_MEMBER_ID_CUSTOM_VIDEO_WINDOW			0xfff0

/*J 構造体 --------------------------------------------------------------- */
/*J ウィンドウ属性パラメータ構造体 */
typedef union CellSysutilAvc2WindowAttributeParam
{
    int   int_vector[4];
    float float_vector[4];
    void* ptr_vector[4];
} CellSysutilAvc2WindowAttributeParam;

/*J ウィンドウ属性構造体 */
typedef struct CellSysutilAvc2WindowAttribute
{
    CellSysutilAvc2WindowAttributeId    attr_id;
    CellSysutilAvc2WindowAttributeParam attr_param;
} CellSysutilAvc2WindowAttribute;


/*J 映像制御機能 --------------------------------------------------------- */
/*J チャットスクリーンの表示状態を表示化する */
int cellSysutilAvc2ShowScreen( void );

/*J チャットスクリーンの表示状態を非表示化する */
int cellSysutilAvc2HideScreen( void );

/*J チャットスクリーン表示状態を取得する */
int cellSysutilAvc2GetScreenShowStatus( uint8_t* visible );

/*J チャットウィンドウを生成する */
int cellSysutilAvc2CreateWindow( const SceNpMatching2RoomMemberId member_id );

/*J チャットウィンドウを解放する */
int cellSysutilAvc2DestroyWindow( const SceNpMatching2RoomMemberId member_id );

/*J ウィンドウ文字列を設定する */
int cellSysutilAvc2SetWindowString( const SceNpMatching2RoomMemberId member_id,
                                    const char* string );

/*J ウィンドウ文字列を取得する */
int cellSysutilAvc2GetWindowString( const SceNpMatching2RoomMemberId member_id,
                                    char* string, uint8_t* len );

/*J チャットウィンドウの位置を設定する */
int cellSysutilAvc2SetWindowPosition( const SceNpMatching2RoomMemberId member_id,
                                      const float x,
                                      const float y,
                                      const float z );

/*J チャットウィンドウの位置を取得する */
int cellSysutilAvc2GetWindowPosition( const SceNpMatching2RoomMemberId member_id,
                                      float* x,
                                      float* y,
                                      float* z );

/*J チャットウィンドウのサイズを設定する */
int cellSysutilAvc2SetWindowSize( const SceNpMatching2RoomMemberId member_id,
                                  const float width,
                                  const float height );

/*J チャットウィンドウのサイズを取得する */
int cellSysutilAvc2GetWindowSize( const SceNpMatching2RoomMemberId member_id,
                                  float* width,
                                  float* height );

/*J チャットウィンドウの表示状態を表示化する */
int cellSysutilAvc2ShowWindow( const SceNpMatching2RoomMemberId member_id );

/*J チャットウィンドウの表示状態を非表示化する */
int cellSysutilAvc2HideWindow( const SceNpMatching2RoomMemberId member_id );

/*J チャットウィンドウの表示状態を取得する */
int cellSysutilAvc2GetWindowShowStatus( const SceNpMatching2RoomMemberId member_id,
                                        uint8_t* visible );

/*J 映像のミューティング状態を設定する */
int cellSysutilAvc2SetVideoMuting( const uint8_t muting );

/*J 映像のミューティング状態を取得する */
int cellSysutilAvc2GetVideoMuting( uint8_t* muting );

/*J カメラスステータスを取得する */
int cellSysutilAvc2IsCameraAttached( uint8_t* status );

/*J ビデオ解像度を変更する */
int cellSysutilAvc2ChangeVideoResolution( const CellSysutilAvc2VideoResolution resolution );


/*J チャットウィンドウ属性設定 ------------------------------------------- */
/*J チャットウィンドウの属性を設定する */
int cellSysutilAvc2SetWindowAttribute( const SceNpMatching2RoomMemberId member_id,
                                       const CellSysutilAvc2WindowAttribute* attr );

/*J チャットウィンドウの属性を取得する */
int cellSysutilAvc2GetWindowAttribute( const SceNpMatching2RoomMemberId member_id,
                                       CellSysutilAvc2WindowAttribute* attr );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_AVC2_VOICE_H_*/

