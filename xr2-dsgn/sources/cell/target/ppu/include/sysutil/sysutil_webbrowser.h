/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_WEBBROWSER_H_
#define _CELL_SYSUTIL_WEBBROWSER_H_

#include "sysutil_common.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


#define CELL_WEBBROWSER_STRING_SIZE	(512)		/*J メッセージストリングのサイズ */

#define CELL_SYSUTIL_WEBBROWSER_INITIALIZING_FINISHED  1
#define CELL_SYSUTIL_WEBBROWSER_SHUTDOWN_FINISHED      4 // 3(SDK103) -> 4(SDK110)
#define CELL_SYSUTIL_WEBBROWSER_LOADING_FINISHED       5
#define CELL_SYSUTIL_WEBBROWSER_UNLOADING_FINISHED     7
#define CELL_SYSUTIL_WEBBROWSER_RELEASED               9
#define CELL_SYSUTIL_WEBBROWSER_GRABBED               11

	enum {
		CELL_WEBBROWSER_FUNCTION2_NOTHING  =   0x00000000, // WEBBROWSER_FUNCTION2_NOTHING
		CELL_WEBBROWSER_FUNCTION2_BOOKMARK =   0x00000001, // WEBBROWSER_FUNCTION2_BOOKMARK
		CELL_WEBBROWSER_FUNCTION2_HISTORY  =   0x00000002, // WEBBROWSER_FUNCTION2_HISTORY
		CELL_WEBBROWSER_FUNCTION2_TABS     =   0x00000004, // WEBBROWSER_FUNCTION2_TABS
		CELL_WEBBROWSER_FUNCTION2_SETTING  =   0x00000008, // WEBBROWSER_FUNCTION2_SETTING
		CELL_WEBBROWSER_FUNCTION2_DOWNLOADER = 0x00000010, // WEBBROWSER_FUNCTION2_DOWNLOADER
		CELL_WEBBROWSER_FUNCTION2_MOUSE    =   0x00000020, // WEBBROWSER_FUNCTION2_MOUSE
		CELL_WEBBROWSER_FUNCTION2_URL_INPUT=   0x00000040, // WEBBROWSER_FUNCTION2_URL_INPUT
		CELL_WEBBROWSER_FUNCTION2_HOME     =   0x00000080, // WEBBROWSER_FUNCTION2_HOME
		CELL_WEBBROWSER_FUNCTION2_SEARCH   =   0x00000100, // WEBBROWSER_FUNCTION2_SEARCH
		CELL_WEBBROWSER_FUNCTION2_LOCAL    =   0x00000200, // WEBBROWSER_FUNCTION2_LOCAL
		CELL_WEBBROWSER_FUNCTION2_PLUGINS  =   0x00001000, // WEBBROWSER_FUNCTION2_PLUGINS
		CELL_WEBBROWSER_FUNCTION2_ALL      =   0xffffffff, // WEBBROWSER_FUNCTION2_ALL
	};

	// view condition
	enum {
		CELL_WEBBROWSER_VIEWCOND2_NO_FULL_SCREEN = 0x00000001,
		CELL_WEBBROWSER_VIEWCOND2_TRANSPARENT    = 0x00000002,
		CELL_WEBBROWSER_VIEWCOND2_OVERFLOW_AUTO  = 0x00000004,
		CELL_WEBBROWSER_VIEWCOND2_RENDER_ONLY    = 0x00000008,
		CELL_WEBBROWSER_VIEWCOND2_RENDER_BUSYICON= 0x00000010
	};

	enum {
		CELL_WEBBROWSER_FUNCTION_NOTHING  =   0x0000, // WEBBROWSER_FUNCTION_NOTHING
		CELL_WEBBROWSER_FUNCTION_BOOKMARK =   0x0001, // WEBBROWSER_FUNCTION_BOOKMARK
		CELL_WEBBROWSER_FUNCTION_HISTORY  =   0x0002, // WEBBROWSER_FUNCTION_HISTORY
		CELL_WEBBROWSER_FUNCTION_TABS     =   0x0004, // WEBBROWSER_FUNCTION_TABS
		CELL_WEBBROWSER_FUNCTION_SETTING  =   0x0008, // WEBBROWSER_FUNCTION_SETTING
		CELL_WEBBROWSER_FUNCTION_DOWNLOADER = 0x0010, // WEBBROWSER_FUNCTION_DOWNLOADER
		CELL_WEBBROWSER_FUNCTION_MOUSE    =   0x0020, // WEBBROWSER_FUNCTION_MOUSE
		CELL_WEBBROWSER_FUNCTION_URL_INPUT=   0x0040, // WEBBROWSER_FUNCTION_URL_INPUT
		CELL_WEBBROWSER_FUNCTION_HOME     =   0x0080, // WEBBROWSER_FUNCTION_HOME
		CELL_WEBBROWSER_FUNCTION_PLUGINS  =   0x1000, // WEBBROWSER_FUNCTION_PLUGINS
		CELL_WEBBROWSER_FUNCTION_ALL      =   0xffff, // WEBBROWSER_FUNCTION_ALL
	};

	// under 16bits are reserved for safe-area's offset
	enum {
		CELL_WEBBROWSER_HORIZONAL_CENTER= 0x00000000,
		CELL_WEBBROWSER_HORIZONAL_LEFT  = 0x00010000,
		CELL_WEBBROWSER_HORIZONAL_RIGHT = 0x00020000,

		CELL_WEBBROWSER_VERTICAL_CENTER = 0x00000000,
		CELL_WEBBROWSER_VERTICAL_TOP    = 0x01000000,
		CELL_WEBBROWSER_VERTICAL_BOTTOM = 0x02000000
	};

	enum {
		CELL_WEBBROWSER_CALLBACK_REQUEST_COMPLETED     = 0x0001,
		CELL_WEBBROWSER_CALLBACK_DOWNLOADING_REQUESTED = 0x0002,
		CELL_WEBBROWSER_CALLBACK_EXIT_BROWSER          = 0x0004
	};

/*J エラーコード */
#define CELL_WEBBROWSER_ERROR_NOMEM					(0x8002b901)	/*J  */
#define CELL_WEBBROWSER_ERROR_INVALID				(0x8002b902)	/*J  */
#define CELL_WEBBROWSER_ERROR_EXCLUSIVE				(0x8002b903)	/*J  */

	typedef int CellWebBrowserId;
	typedef void* CellWebBrowserClientSession;
	typedef void (*CellWebBrowserCallback)     (int cb_type, CellWebBrowserClientSession, void* usrdata);
	typedef void (*CellWebComponentCallback)     (CellWebBrowserId, int cb_type, CellWebBrowserClientSession, void* usrdata);
	typedef void (*CellWebBrowserSystemCallback)(int cb_type, void* usrdata);

	typedef void (*CellWebBrowserMIMETypeCallback)(const char* mimetype, const char* url, void* usrdata);
	typedef void (*CellWebBrowserErrorCallback)(int err_type, void* usrdata);
	typedef void (*CellWebBrowserStatusCallback)(int err_type, void* usrdata);
	typedef void (*CellWebBrowserNotify)(const char* message, void* usrdata);
	typedef void (*CellWebBrowserUsrdata)(void* usrdata);

#define DECL_WEBBROWSER_UI_CALLBACK(func, cb_type_arg, session_arg, usrdata_arg) static void func(int cb_type_arg, CellWebBrowserClientSession session_arg, void* usrdata_arg)
#define DECL_WEBBROWSER_SYSTEM_CALLBACK(func, cb_type_arg, systemdata_arg) static void func(int cb_type_arg, void* systemdata_arg)

	typedef struct {
		const char* const type;
		const char* const directory;
	} CellWebBrowserMimeSet;

	typedef struct {
		int x;
		int y;
	} CellWebBrowserPos;

	typedef struct {
		int width;
		int height;
	} CellWebBrowserSize;

	typedef struct {
		CellWebBrowserPos pos;
		CellWebBrowserSize size;
	} CellWebBrowserRect;

	int cellWebBrowserInitialize(CellWebBrowserSystemCallback, sys_memory_container_t);
	int cellWebBrowserShutdown();
	
	typedef struct {
		int version;
		int heap_size;
		const CellWebBrowserMimeSet* mimesets;
		int mimeset_num;
		int functions;
		int tab_count;
		CellWebBrowserCallback exit_cb;
		CellWebBrowserCallback download_cb;
		CellWebBrowserCallback navigated_cb;
	} CellWebBrowserConfig;

	typedef struct {
		int version;
		int heap_size;
		int functions;
		int tab_count;
		int size_mode;
		int view_restriction;

		CellWebBrowserMIMETypeCallback unknown_mimetype_cb;
		CellWebBrowserErrorCallback error_cb;
		CellWebBrowserStatusCallback status_error_cb;
		CellWebBrowserNotify notify_cb;
		CellWebBrowserCallback request_cb;

		CellWebBrowserRect rect;
		float resolution_factor;

		int magic_number_;
	} CellWebBrowserConfig2;

	// helpers
#define CELL_WEBBROWSER_MK_VER(major, minor) (((0xffff & (major))<<16) | (0xffff & (minor)))

	// configure for version2
	int  cellWebBrowserEstimate2(const CellWebBrowserConfig2* const cfg, int* memsize);
	int  cellWebBrowserSetSystemCallbackUsrdata(void* usrdata);
	int  cellWebBrowserConfig2(CellWebBrowserConfig2* const cfg, unsigned int version);
	int  cellWebBrowserConfigSetFullVersion2(CellWebBrowserConfig2* const cfg);
	int  cellWebBrowserConfigSetUnknownMIMETypeHook2(CellWebBrowserConfig2* const cfg, CellWebBrowserMIMETypeCallback cb, void* usrdata);
	int  cellWebBrowserConfigSetErrorHook2(CellWebBrowserConfig2* const cfg, CellWebBrowserErrorCallback cb, void* usrdata);
	int  cellWebBrowserConfigSetStatusHook2(CellWebBrowserConfig2* const cfg, CellWebBrowserStatusCallback cb, void* usrdata);
	int  cellWebBrowserConfigSetNotifyHook2(CellWebBrowserConfig2* const cfg, CellWebBrowserNotify cb, void* usrdata);
	int  cellWebBrowserConfigSetRequestHook2(CellWebBrowserConfig2* const cfg, CellWebBrowserCallback cb, void* usrdata);
	int  cellWebBrowserConfigSetFunction2(CellWebBrowserConfig2* const cfg, int func);
	int  cellWebBrowserConfigSetViewCondition2(CellWebBrowserConfig2* const cfg, int cond);
	int  cellWebBrowserConfigSetViewRect2(CellWebBrowserConfig2* const cfg, const CellWebBrowserRect rect, float resolution_factor);
	int  cellWebBrowserConfigSetHeapSize2(CellWebBrowserConfig2* const cfg, int heap);
	int  cellWebBrowserConfigGetHeapSize2(const CellWebBrowserConfig2* const cfg, int* memsize);
	int  cellWebBrowserConfigSetTabCount2(CellWebBrowserConfig2* const cfg, int tab_count);
	int  cellWebBrowserConfigSetFullScreen2(CellWebBrowserConfig2* const cfg, int full);
	
	int  cellWebBrowserSetLocalContentsAdditionalTitleID(const char* titleID);

	// create and destroy for version2
	int  cellWebBrowserCreate2(const CellWebBrowserConfig2* const, const char* url);
	int  cellWebBrowserNavigate2(const char* url);
	int  cellWebBrowserDestroy2();

	int  cellWebBrowserWakeupWithGameExit(const char* url, void* usrdata);
	int  cellWebBrowserGetUsrdataOnGameExit(CellWebBrowserUsrdata cb);
	// following APIs are all obsolated

	void cellWebBrowserConfig(CellWebBrowserConfig* const cfg); // obsolated
	void cellWebBrowserConfigSetVersion(CellWebBrowserConfig* const cfg, unsigned int version); // obsolated
	void cellWebBrowserConfigWithVer(CellWebBrowserConfig* const cfg, unsigned int version);
	int  cellWebBrowserEstimate(const CellWebBrowserConfig* const cfg);
//	int  cellWebBrowserConfigSetNavigated(CellWebBrowserConfig* const cfg, CellWebBrowserCallback cb, void* usrdata); // not implemented
//	int  cellWebBrowserConfigSetMimeSet (CellWebBrowserConfig* const cfg, const CellWebBrowserMimeSet* mime, int entries, CellWebBrowserCallback cb, void* usrdata); // not implemented
	int  cellWebBrowserConfigSetFunction(CellWebBrowserConfig* const cfg, int func);
	int  cellWebBrowserConfigSetHeapSize(CellWebBrowserConfig* const cfg, int heap);
	int  cellWebBrowserConfigGetHeapSize(const CellWebBrowserConfig* const cfg);

	int  cellWebBrowserConfigSetDisableTabs(CellWebBrowserConfig* const cfg, int tab_count);
//	int  cellWebBrowserConfigSetCustomExit (CellWebBrowserConfig* const cfg, CellWebBrowserCallback cb, void* usrdata); // not implemented

	// obtain the full browser
	int  cellWebBrowserCreate(const char* url);
	int  cellWebBrowserCreateWithConfig(const char* url, const CellWebBrowserConfig* const);
	int  cellWebBrowserCreateWithConfigFull(const char* url, const CellWebBrowserConfig* const);
	void cellWebBrowserDestroy();

	int  cellWebBrowserActivate();
	int  cellWebBrowserDeactivate(float);

//	int  cellWebBrowserClientSessionAck(CellWebBrowserClientSession, int code);

	typedef struct {
		int version;
		CellWebBrowserMimeSet* mimesets;
		int mimeset_num;
		int functions;
		int width;
		int height;
		CellWebBrowserCallback exit_cb;
		CellWebBrowserCallback download_cb;
		CellWebBrowserCallback navigated_cb;
	} CellWebComponentConfig;
/*
	// not implemented
	CellWebBrowserId cellWebComponentCreate    (int hor_pos, int vert_pos, int width, int height, const char* url, const CellWebComponentConfig* const config, int* err);
	CellWebBrowserId cellWebComponentCreateAsync(int hor_pos, int vert_pos, int width, int height, const char* url, const CellWebComponentConfig* const config, int* err);
	void             cellWebComponentDestroy  (CellWebBrowserId id);
	int              cellWebComponentNavigate (CellWebBrowserId id, const char* url);
	int				 cellWebComponentSendEvent(CellWebBrowserId id, int event_type, int event_code);
*/
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_MSGDIALOG_H_*/
