/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
#ifndef __DTLIB_HEADER__
#define __DTLIB_HEADER__

#ifdef WIN32
#include <winsock2.h>
#   ifdef DTLIB_EXPORTS
#   define WINDTLIB_API __declspec(dllexport)
#   else
#   define WINDTLIB_API __declspec(dllimport)
#   endif
#else
#define WINDTLIB_API
#endif // WIN32

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>

#ifndef M__NO_NAMESPACE
#   define M__BEGIN_NAMESPACE  namespace deci3_lib {
#   define M__END_NAMESPACE    }
#   define M__USING_NAMESPACE  using namespace deci3_lib;
#   define M__NAME_SPEC        ::deci3_lib
#else
#   define M__BEGIN_NAMESPACE
#   define M__END_NAMESPACE
#   define M__USING_NAMESPACE
#   define M__NAME_SPEC
#endif

#ifdef WIN32

typedef HANDLE HCONNECT;
typedef HANDLE HDECI;
typedef HANDLE HSELECT;
typedef HANDLE HDRIVER;
typedef HANDLE HPROTO;
typedef BOOL (CALLBACK DECIRECVPROC)(HDECI, BYTE*, int size, int status);
typedef BOOL (CALLBACK DCMPRECVPROC)(HCONNECT, BYTE*, int size);
typedef BOOL (CALLBACK DRIVERRECVPROC)(HDRIVER, BYTE*, int size);

#else // WIN32
#include <stdint.h>

typedef uint8_t byte;             /*   8bit */
typedef uint8_t BYTE;             /*   8bit */
typedef uint8_t UINT8;            /*   8bit */
typedef uint16_t half;            /*  16bit */
typedef uint16_t UINT16;          /*  16bit */
typedef int16_t INT16;            /*  16bit */
typedef uint32_t word;            /*  32bit */
typedef uint32_t UINT32;          /*  32bit */
typedef uint32_t UINT;            /*  32bit */
typedef uint64_t UINT64;          /*  64bit */
typedef bool BOOL;

typedef void* HANDLE;
typedef void* HCONNECT;
typedef void* HDECI;
typedef void* HSELECT;
typedef void* HDRIVER;
typedef void* HPROTO;

typedef int SOCKET;

typedef BOOL DECIRECVPROC(HDECI, BYTE*, int size, int status);
typedef BOOL DCMPRECVPROC(HCONNECT, BYTE*, int size);
typedef BOOL DRIVERRECVPROC(HDRIVER, BYTE*, int size);

#include <pthread.h>

typedef pthread_mutex_t CRITICAL_SECTION;

#define FALSE 0
#define TRUE 1
#define CALLBACK

#endif //WIN32

using namespace std;

typedef std::vector<string> DtLParList;

typedef struct {
    string  lparName;
    UINT32  protocol;
    UINT32  port;
} DtProtocolType;

typedef struct {
    string  lparName;
    string  info;
    UINT32  protocol;
    UINT32  registered_time;
    BYTE    priority;
    BYTE    portNo;
} DtRegisteredInfo;

typedef std::vector<DtRegisteredInfo> DtRegisteredList;

typedef struct {
    UINT32  protocol;
    BYTE    port;
    BYTE    reserved[3];
} DtProtocolInfo;

typedef std::vector<DtProtocolInfo> DtProtocolList;

M__BEGIN_NAMESPACE

typedef enum DT_DESTINATION {
    HOST   = 0, // Host Application
    MGR    = 1, // Communication Processor
    TARGET = 2, // Protocol Driver
};

class WINDTLIB_API dtlib
{
public:
    dtlib(void);
    ~dtlib(void);

    UINT32 dt_set_option(UINT32 value);

    HCONNECT dt_connect(std::string hostname, int portNo,
                        std::string programName);
    void dt_disconnect(HCONNECT connect);

    HDECI dt_register(HCONNECT connectH, int protocol, int port,
                      DT_DESTINATION dst, std::string lparName,
                      int priority=128);
    int dt_unregister(HDECI deciHandle);

    int dt_send(HDECI deciHandle, BYTE* data, int length);
    int dt_receive(HANDLE deciHandle, BYTE* data, int length);

    // error & error messages
    int dt_getlasterror();
    std::string dt_geterrormsg(int error);
    int dt_get_protocol(HANDLE handle, int &protocol, int &port);

    DtRegisteredList dt_get_registered_list(HCONNECT connectH);
    DtLParList dt_get_lparlist(HCONNECT connectH);
    DtProtocolList dt_get_protocol_list(HCONNECT connectH, string lparName);
    std::string dt_get_version(HCONNECT connectH);
    int dt_power_status(HCONNECT connH, int &status);

    HANDLE dt_select(HCONNECT connH, int waitTime);
    int dt_set_dcmp_status_function(HCONNECT connectH, DCMPRECVPROC* func);
    int dt_add_recv_function(HDECI deicH, DECIRECVPROC* func);
    int dt_delete_recv_function(HDECI deciH);
private:
    int m_lastError;
    int m_waitTime;
    UINT32 m_option;

public:
    int dt_send_dcmp(HCONNECT connectH, string lparName, BYTE *data, int dataSize);
    int dt_set_dcmp_echo_function(HCONNECT connectH, DCMPRECVPROC* func);
};

M__END_NAMESPACE
#endif // __DTLIB_HEADER__
