/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_AVC2_H_
#define _CELL_SYSUTIL_AVC2_H_

#include "np.h"
#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/*J リターンーコード ------------------------------------------------------ */
#define CELL_AVC2_ERROR_UNKNOWN                 ( 0x8002b701 )
#define CELL_AVC2_ERROR_NOT_SUPPORTED           ( 0x8002b702 )
#define CELL_AVC2_ERROR_NOT_INITIALIZED         ( 0x8002b703 )
#define CELL_AVC2_ERROR_ALREADY_INITIALIZED     ( 0x8002b704 )
#define CELL_AVC2_ERROR_INVALID_ARGUMENT        ( 0x8002b705 )
#define CELL_AVC2_ERROR_OUT_OF_MEMORY           ( 0x8002b706 )
#define CELL_AVC2_ERROR_BAD_ID                  ( 0x8002b707 )
#define CELL_AVC2_ERROR_INVALID_STATUS          ( 0x8002b70a )
#define CELL_AVC2_ERROR_TIMEOUT                 ( 0x8002b70b )
#define CELL_AVC2_ERROR_NO_SESSION              ( 0x8002b70d )
#define CELL_AVC2_ERROR_INCOMPATIBLE_PROTOCOL   ( 0x8002b70e )
#define CELL_AVC2_ERROR_WINDOW_ALREADY_EXISTS   ( 0x8002b70f )
#define CELL_AVC2_ERROR_TOO_MANY_WINDOWS        ( 0x8002b710 )
#define CELL_AVC2_ERROR_TOO_MANY_PEER_WINDOWS   ( 0x8002b711 )
#define CELL_AVC2_ERROR_WINDOW_NOT_FOUND        ( 0x8002b712 )

/*J イベント ------------------------------------------------------------- */

/*J AVチャット2ユーティリティの非同期関数呼び出しの結果として通知されるイベント */
#define CELL_AVC2_EVENT_LOAD_SUCCEEDED                      (0x00000001)
#define CELL_AVC2_EVENT_LOAD_FAILED                         (0x00000002)
#define CELL_AVC2_EVENT_UNLOAD_SUCCEEDED                    (0x00000003)
#define CELL_AVC2_EVENT_UNLOAD_FAILED                       (0x00000004)
#define CELL_AVC2_EVENT_JOIN_SUCCEEDED                      (0x00000005)
#define CELL_AVC2_EVENT_JOIN_FAILED                         (0x00000006)
#define CELL_AVC2_EVENT_LEAVE_SUCCEEDED                     (0x00000007)
#define CELL_AVC2_EVENT_LEAVE_FAILED                        (0x00000008)

/*J チャットセッションの状態の変化を通知するイベント */
#define CELL_AVC2_EVENT_SYSTEM_NEW_MEMBER_JOINED            (0x10000001)
#define CELL_AVC2_EVENT_SYSTEM_MEMBER_LEFT                  (0x10000002)
#define CELL_AVC2_EVENT_SYSTEM_SESSION_ESTABLISHED          (0x10000003)
#define CELL_AVC2_EVENT_SYSTEM_SESSION_CANNOT_ESTABLISHED   (0x10000004)
#define CELL_AVC2_EVENT_SYSTEM_SESSION_DISCONNECTED         (0x10000005)
#define CELL_AVC2_EVENT_SYSTEM_VOICE_DETECTED               (0x10000006)
#define CELL_AVC2_EVENT_SYSTEM_MIC_DETECTED                 (0x10000007)
#define CELL_AVC2_EVENT_SYSTEM_CAMERA_DETECTED              (0x10000008)

/*J イベントパラメータ */
#define CELL_AVC2_EVENT_PARAM_ERROR_UNKNOWN                 (0x0000000000000001)
#define CELL_AVC2_EVENT_PARAM_ERROR_NOT_SUPPORTED           (0x0000000000000002)
#define CELL_AVC2_EVENT_PARAM_ERROR_INVALID_ARGUMENT        (0x0000000000000003)
#define CELL_AVC2_EVENT_PARAM_ERROR_OUT_OF_MEMORY           (0x0000000000000004)
#define CELL_AVC2_EVENT_PARAM_ERROR_INVALID_STATUS          (0x0000000000000005)
#define CELL_AVC2_EVENT_PARAM_ERROR_CONTEXT_DOES_NOT_EXIST  (0x0000000000000006)
#define CELL_AVC2_EVENT_PARAM_ERROR_ROOM_DOES_NOT_EXIST     (0x0000000000000007)
#define CELL_AVC2_EVENT_PARAM_ERROR_NETWORK_ERROR           (0x0000000000000008)

/*J リクエストID --------------------------------------------------------- */
#define CELL_AVC2_REQUEST_ID_SYSTEM_EVENT                   (0x00000000)

/*J プレイヤーのマイク状態 ----------------------------------------------- */
#define CELL_AVC2_MIC_STATUS_DETACHED                       (0)
#define CELL_AVC2_MIC_STATUS_ATTACHED_OFF                   (1)
#define CELL_AVC2_MIC_STATUS_ATTACHED_ON                    (2)
#define CELL_AVC2_MIC_STATUS_UNKNOWN                        (3)

/*J プレイヤーのカメラ状態 ----------------------------------------------- */
#define CELL_AVC2_CAMERA_STATUS_DETACHED                    (0)
#define CELL_AVC2_CAMERA_STATUS_ATTACHED_OFF                (1)
#define CELL_AVC2_CAMERA_STATUS_ATTACHED_ON                 (2)
#define CELL_AVC2_CAMERA_STATUS_UNKNOWN                     (3)

/*J ストリーミングモード ----------------------------------------------- */
#define CELL_SYSUTIL_AVC2_STREAMING_MODE_NORMAL             (0)
#define CELL_SYSUTIL_AVC2_STREAMING_MODE_DIRECT_WAN         (1)
#define CELL_SYSUTIL_AVC2_STREAMING_MODE_DIRECT_LAN         (2)

/*J 定数定義 ------------------------------------------------------------- */
/*J AVチャット2サービスで提供されるメディア */
typedef enum {
    CELL_SYSUTIL_AVC2_VOICE_CHAT                            = 0x00000001,
    CELL_SYSUTIL_AVC2_VIDEO_CHAT                            = 0x00000010,
} CellSysutilAvc2MediaType;

/*J AVチャット2サービスで選択可能な音声品質 */
typedef enum {
    CELL_SYSUTIL_AVC2_VOICE_QUALITY_NORMAL                  = 0x00000001,
} CellSysutilAvc2VoiceQuality;

/*J AVチャット2サービスで選択可能な映像品質 */
typedef enum {
    CELL_SYSUTIL_AVC2_VIDEO_QUALITY_NORMAL                  = 0x00000001,
} CellSysutilAvc2VideoQuality;

/*J 音声送信先 */
typedef enum {
    CELL_SYSUTIL_AVC2_CHAT_TARGET_MODE_ROOM                 = 0x00000100,
    CELL_SYSUTIL_AVC2_CHAT_TARGET_MODE_TEAM                 = 0x00000200,
    CELL_SYSUTIL_AVC2_CHAT_TARGET_MODE_PRIVATE              = 0x00000300,
    CELL_SYSUTIL_AVC2_CHAT_TARGET_MODE_DIRECT               = 0x00001000,
} CellSysutilAvc2ChatTargetMode;

/*J 映像解像度 */
typedef enum {
    CELL_SYSUTIL_AVC2_VIDEO_RESOLUTION_QQVGA                = 0x00000001,
    CELL_SYSUTIL_AVC2_VIDEO_RESOLUTION_QVGA                 = 0x00000002,
} CellSysutilAvc2VideoResolution;

/*J 映像フレーム送出モード */
typedef enum {
    CELL_SYSUTIL_AVC2_FRAME_MODE_NORMAL                     = 0x00000001,
    CELL_SYSUTIL_AVC2_FRAME_MODE_INTRA_ONLY                 = 0x00000002,
} CellSysutilAvc2FrameMode;

/*J 座標形式 */
typedef enum {
    CELL_SYSUTIL_AVC2_VIRTUAL_COORDINATES                   = 0x00000001,
    CELL_SYSUTIL_AVC2_ABSOLUTE_COORDINATES                  = 0x00000002,
} CellSysutilAvc2CoordinatesForm;

/*J AVチャット2サービスの属性 */
typedef enum {
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_DETECT_EVENT_TYPE     = 0x00001001,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_DETECT_INTERVAL_TIME  = 0x00001002,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_DETECT_SIGNAL_LEVEL   = 0x00001003,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_MAX_BITRATE           = 0x00001004,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_DATA_FEC              = 0x00001005,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_PACKET_CONTENTION     = 0x00001006,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_DTX_MODE              = 0x00001007,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_MIC_STATUS_DETECTION        = 0x00001008,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_MIC_SETTING_NOTIFICATION    = 0x00001009,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_VOICE_MUTING_NOTIFICATION   = 0x0000100A,
    CELL_SYSUTIL_AVC2_ATTRIBUTE_CAMERA_STATUS_DETECTION     = 0x0000100B,
} CellSysutilAvc2AttributeId;

/*J AVチャット2ロードパラメータバージョン */
//#define CELL_SYSUTIL_AVC2_INIT_PARAM_VERSION        (110)
//#define CELL_SYSUTIL_AVC2_INIT_PARAM_VERSION        (120)
#define CELL_SYSUTIL_AVC2_INIT_PARAM_VERSION          (130)

/*J 構造体 --------------------------------------------------------------- */
/*J 音声設定パラメータ構造体 */
typedef struct CellSysutilAvc2VoiceInitParam
{
    CellSysutilAvc2VoiceQuality voice_quality;
    uint16_t                    max_speakers;
    uint8_t                     mic_out_stream_sharing;
    uint8_t                     reserved[25];
} CellSysutilAvc2VoiceInitParam;

/*J 映像設定パラメータ構造体 */
typedef struct CellSysutilAvc2VideoInitParam
{
    CellSysutilAvc2VideoQuality     video_quality;
    CellSysutilAvc2FrameMode        frame_mode;
    CellSysutilAvc2VideoResolution  max_video_resolution;
    uint16_t                        max_video_windows;
    uint16_t                        max_video_framerate;
    uint32_t                        max_video_bitrate;
    CellSysutilAvc2CoordinatesForm  coordinates_form;
    uint8_t                         video_stream_sharing;
    uint8_t                         reserved[7];
} CellSysutilAvc2VideoInitParam;

/*J LAN内チャットモードパラメータ構造体 */
typedef struct CellSysutilAvc2StreamingModeParam
{
    uint16_t                              mode;
    in_port_t                             port;
    uint8_t                               reserved[10];
} CellSysutilAvc2StreamingModeParam;

/*J ロードオプションパラメータ構造体 */
typedef struct CellSysutilAvc2InitParam
{
    uint16_t                      avc_init_param_version;
    uint16_t                      max_players;
    uint16_t                      spu_load_average;
    union
    {
        uint16_t                             direct_streaming_mode;
        CellSysutilAvc2StreamingModeParam    streaming_mode;  /*J LAN内チャットモード対応で追加 */
    };
    uint8_t                       reserved[18];
    CellSysutilAvc2MediaType      media_type;
    CellSysutilAvc2VoiceInitParam voice_param;
    CellSysutilAvc2VideoInitParam video_param;
    uint8_t                       reserved2[22];
} CellSysutilAvc2InitParam;

/*J プレイヤー情報構造体 */
typedef struct CellSysutilAvc2PlayerInfo
{
    SceNpMatching2RoomMemberId member_id;
    uint8_t                    joined;
    uint8_t                    connected;
    uint8_t                    mic_attached;
    uint8_t                    reserved[11];
} CellSysutilAvc2PlayerInfo;

/*J 属性パラメータ構造体 */
typedef union CellSysutilAvc2AttributeParam
{
    uint64_t int_param;
    float    float_param;
    void    *ptr_param;
    uint8_t  reserved[64];
} CellSysutilAvc2AttributeParam;

/*J 属性構造体 */
typedef struct CellSysutilAvc2Attribute
{
    CellSysutilAvc2AttributeId    attr_id;
    CellSysutilAvc2AttributeParam attr_param;
} CellSysutilAvc2Attribute;

/*J プレイヤーリスト構造体 */
typedef struct CellSysutilAvc2MemberList
{
    SceNpMatching2RoomMemberId *member_id;
    uint8_t                     member_num;
    uint8_t                     reserved[3];
} CellSysutilAvc2RoomMemberList;

typedef struct CellSysutilAvc2MemberIpAndPortList
{
    SceNpMatching2RoomMemberId *member_id;
    struct in_addr             *dst_addr;
    in_port_t                  *dst_port;
    SceNpMatching2RoomMemberId  my_member_id;
    uint8_t                     member_num;
    uint8_t                     reserved[7];
} CellSysutilAvc2MemberIpAndPortList;

/*J ストリーム送受信対象指定構造体 */
typedef struct CellSysutilAvc2StreamingTarget
{
    CellSysutilAvc2ChatTargetMode target_mode;
    union {
        CellSysutilAvc2RoomMemberList room_member_list;
        SceNpMatching2TeamId          team_id;
        CellSysutilAvc2MemberIpAndPortList ip_and_port_list;
        uint8_t                       reserved[32];
    };
    uint8_t reserved2[28];
} CellSysutilAvc2StreamingTarget;

/*J コールバック関連定義 ------------------------------------------------- */
/*J イベントID */
typedef uint32_t CellSysutilAvc2EventId;

/*J イベントパラメータ */
typedef uint64_t CellSysutilAvc2EventParam;

/*J AVCHAT2サービスからの通知を受け取る為のCallBack */
typedef void (*CellSysutilAvc2Callback)( CellSysutilAvc2EventId event_id,
                                         CellSysutilAvc2EventParam event_param,
                                         void *userdata );

/*J 初期化・終了 --------------------------------------------------------- */
/*J オプションパラメータ初期化関数 */
int cellSysutilAvc2InitParam( const uint16_t version,
                              CellSysutilAvc2InitParam *option );

/*J メモリ所要量の算出 */
int cellSysutilAvc2EstimateMemoryContainerSize( const CellSysutilAvc2InitParam *option,
                                                uint32_t *size );

/*J ロード関数(非同期処理) */
int cellSysutilAvc2LoadAsync( SceNpMatching2ContextId ctx_id,
                              const sys_memory_container_t container,
                              CellSysutilAvc2Callback callback_func,
                              void *user_data,
                              const CellSysutilAvc2InitParam *init_param );

/*J アンロード関数(非同期処理) */
int cellSysutilAvc2UnloadAsync(void);

/*J ロード関数(同期処理) */
int cellSysutilAvc2Load( SceNpMatching2ContextId ctx_id,
                         const sys_memory_container_t container,
                         CellSysutilAvc2Callback callback_func,
                         void *user_data,
                         const CellSysutilAvc2InitParam *init_param );

/*J アンロード関数(同期処理) */
int cellSysutilAvc2Unload(void);

/*J アンロード関数(非同期処理) */
int cellSysutilAvc2UnloadAsync2( const CellSysutilAvc2MediaType mediaType );

/*J アンロード関数(同期処理) */
int cellSysutilAvc2Unload2( const CellSysutilAvc2MediaType mediaType );

/*J ストリーム制御機能 --------------------------------------------------- */
/*J ストリーミング開始関数 */
int cellSysutilAvc2StartStreaming( void );

/*J ストリーミング終了関数 */
int cellSysutilAvc2StopStreaming( void );

/*J ストリーミング開始関数 */
int cellSysutilAvc2StartStreaming2( const CellSysutilAvc2MediaType mediaType );

/*J ストリーミング終了関数 */
int cellSysutilAvc2StopStreaming2( const CellSysutilAvc2MediaType mediaType );

/*J ストリーミング送受信対象設定関数 */
int cellSysutilAvc2SetStreamingTarget( const CellSysutilAvc2StreamingTarget target );

/*J ストリーム優先度設定関数 */
int cellSysutilAvc2SetStreamPriority( const uint8_t priority );

/*J チャットセッション管理機能 ------------------------------------------- */
/*J チャット接続関数(非同期処理) */
int cellSysutilAvc2JoinChatRequest( const SceNpMatching2RoomId *room_id );

/*J チャット切断関数(非同期処理) */
int cellSysutilAvc2LeaveChatRequest( void );

/*J チャット接続関数(同期処理) */
int cellSysutilAvc2JoinChat( const SceNpMatching2RoomId *room_id, CellSysutilAvc2EventId *eventId, CellSysutilAvc2EventParam *eventParam );

/*J チャット切断関数(同期処理) */
int cellSysutilAvc2LeaveChat( CellSysutilAvc2EventId *eventId, CellSysutilAvc2EventParam *eventParam );

/*J プレイヤー一覧取得 */
int cellSysutilAvc2EnumPlayers( int *players_num, SceNpMatching2RoomMemberId players_id[] );

/*J プレイヤー情報取得関数 */
int cellSysutilAvc2GetPlayerInfo( const SceNpMatching2RoomMemberId *player_id,
                                  CellSysutilAvc2PlayerInfo *player_info );

/*J 音声制御機能 --------------------------------------------------------- */
/*J 音声のミューティング状態を設定する */
int cellSysutilAvc2SetVoiceMuting( const uint8_t muting );

/*J 音声のミューティング状態を取得する */
int cellSysutilAvc2GetVoiceMuting( uint8_t *muting );

/*J 音声のミューティング状態を設定する */
int cellSysutilAvc2SetSpeakerMuting( const uint8_t muting );

/*J 音声のミューティング状態を取得する */
int cellSysutilAvc2GetSpeakerMuting( uint8_t *muting );

/*J 特定の参加者の音声のミューティング状態を設定する */
int cellSysutilAvc2SetPlayerVoiceMuting( const SceNpMatching2RoomMemberId member_id,
                                         const uint8_t muting );

/*J 特定の参加者の音声のミューティング状態を取得する */
int cellSysutilAvc2GetPlayerVoiceMuting( const SceNpMatching2RoomMemberId member_id,
                                         uint8_t *muting );

/*J スピーカーの音量レベルを設定する */
int cellSysutilAvc2SetSpeakerVolumeLevel( const float level );

/*J スピーカーの音量レベルを取得する */
int cellSysutilAvc2GetSpeakerVolumeLevel( float *level );

/*J 音声出力イベント通知を開始する */
int cellSysutilAvc2StartVoiceDetection( void );

/*J 音声出力イベント通知を停止する */
int cellSysutilAvc2StopVoiceDetection( void );

/*J マイクスステータスを取得する */
int cellSysutilAvc2IsMicAttached( uint8_t *status );

/*J チャットの内部属性を取得する */
int cellSysutilAvc2MicRead(void* ptr, uint32_t* pSize);

/*J 内部属性設定 --------------------------------------------------------- */
/*J チャットの内部属性を設定する */
int cellSysutilAvc2SetAttribute( const CellSysutilAvc2Attribute *attr );

/*J チャットの内部属性を取得する */
int cellSysutilAvc2GetAttribute( CellSysutilAvc2Attribute *attr );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif  /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_AVC2_H_*/

