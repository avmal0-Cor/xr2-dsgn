/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_USBD_LIBUSBD_H__
#define __CELL_USBD_LIBUSBD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usb.h"
#include <sys/types.h>
#include <sys/return_code.h>

/* Logical Device Driver Operations */

typedef struct CellUsbdLddOps {
    const char *name;
    int32_t (*probe)(int32_t dev_id);
    int32_t (*attach)(int32_t dev_id);
    int32_t (*detach)(int32_t dev_id);
} CellUsbdLddOps;

#define CELL_USBD_MAX_ISOCH_PACKETS     (8)

typedef struct CellUsbdIsochPswLen {
    uint16_t len:11;
    uint16_t reserved:1;
    uint16_t PSW:4;
} CellUsbdIsochPswLen;

typedef struct CellUsbdHSIsochPswLen {
    uint16_t len:12;
    uint16_t PSW:4;
} CellUsbdHSIsochPswLen;

typedef struct CellUsbdHSIsochRequest {
    void *buffer_base;
    int32_t relative_start_frame;
    int32_t num_packets;
    CellUsbdHSIsochPswLen Packets[CELL_USBD_MAX_ISOCH_PACKETS];
} CellUsbdHSIsochRequest;

typedef struct CellUsbdIsochRequest {
    void *buffer_base;
    int32_t relative_start_frame;
    int32_t num_packets;
    CellUsbdIsochPswLen Packets[CELL_USBD_MAX_ISOCH_PACKETS];
} CellUsbdIsochRequest;

typedef void (*CellUsbdDoneCallback)(int32_t result, int32_t count, void* arg);
typedef void (*CellUsbdIsochDoneCallback)(int32_t result, CellUsbdIsochRequest *req, void *arg);
typedef void (*CellUsbdHSIsochDoneCallback)(int32_t result, CellUsbdHSIsochRequest *req, void *arg);

#define CELL_USBD_DEVICE_SPEED_LS   (0)
#define CELL_USBD_DEVICE_SPEED_FS   (1)
#define CELL_USBD_DEVICE_SPEED_HS   (2)
                
#define CELL_USBD_THREAD_TYPE_EVENT_HANDLER     (0)
#define CELL_USBD_THREAD_TYPE_USBD_THREAD       (1)
#define CELL_USBD_THREAD_TYPE_CALLBACK_THREAD   (2)

int32_t cellUsbdInit(void);

int32_t cellUsbdEnd(void);

int32_t cellUsbdRegisterLdd(CellUsbdLddOps *lddops);

int32_t cellUsbdUnregisterLdd(CellUsbdLddOps *lddops);

int32_t cellUsbdRegisterExtraLdd(CellUsbdLddOps *lddops, uint16_t id_vendor,
                                 uint16_t id_product);

int32_t cellUsbdRegisterExtraLdd2(CellUsbdLddOps *lddops, uint16_t id_vendor,
                                  uint16_t id_product_min, uint16_t id_product_max);

int32_t cellUsbdUnregisterExtraLdd(CellUsbdLddOps *lddops);

int32_t cellUsbdOpenPipe(int32_t dev_id, UsbEndpointDescriptor *ed);

int32_t cellUsbdClosePipe(int32_t pipe_id);

void *cellUsbdScanStaticDescriptor(int32_t dev_id, void *ptr, unsigned char type);

int32_t cellUsbdSetPrivateData(int32_t dev_id, void *priv);

void *cellUsbdGetPrivateData(int32_t dev_id);

int32_t cellUsbdGetDeviceLocation(int32_t dev_id, unsigned char *location);

int32_t cellUsbdGetDeviceSpeed(int32_t dev_id, uint8_t *speed);

int32_t cellUsbdControlTransfer(int32_t pipe_id, UsbDeviceRequest *dr, void *buf,
                                CellUsbdDoneCallback done_cb, void *arg);

int32_t cellUsbdBulkTransfer(int32_t pipe_id, void *buf, int32_t len, 
                             CellUsbdDoneCallback done_cb, void* arg);

int32_t cellUsbdInterruptTransfer(int32_t pipe_id, void *buf, int32_t len, 
                                  CellUsbdDoneCallback done_cb, void *arg);

int32_t cellUsbdIsochronousTransfer(int32_t pipe_id, CellUsbdIsochRequest *req,
                                    CellUsbdIsochDoneCallback done_cb, void *arg);

int32_t cellUsbdHSIsochronousTransfer(int32_t pipe_id, CellUsbdHSIsochRequest *req,
                                      CellUsbdHSIsochDoneCallback done_cb, void *arg);

int32_t cellUsbdSetThreadPriority2(int32_t event_prio, int32_t usbd_prio,
                                   int32_t callback_prio);

int32_t cellUsbdGetThreadPriority(int32_t thread_type);

int32_t cellUsbdAllocateMemory(void **ptr, size_t size);

int32_t cellUsbdFreeMemory(void *ptr);

/* Standard Control transfer */

#define cellUsbdClearDeviceFeature(pid, fs, cb, arg) ({  \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x00; \
  _dr.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = 0; \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdClearInterfaceFeature(pid, fs, interface, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x01; \
  _dr.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = (interface); \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdClearEndpointFeature(pid, fs, endpoint, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x02; \
  _dr.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = (endpoint); \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdGetConfiguration(pid, ptr, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x80; \
  _dr.bRequest = USB_REQUEST_GET_CONFIGURATION; \
  _dr.wValue = 0; \
  _dr.wIndex = 0; \
  _dr.wLength = 1; \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdGetDescriptor(pid, type, index, lang_id, ptr, len, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x80; \
  _dr.bRequest = USB_REQUEST_GET_DESCRIPTOR; \
  _dr.wValue = ((type) << 8) | (index); \
  _dr.wIndex = (lang_id); \
  _dr.wLength = (len); \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdGetInterface(pid, interface, ptr, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x81; \
  _dr.bRequest = USB_REQUEST_GET_INTERFACE; \
  _dr.wValue = 0; \
  _dr.wIndex = (interface); \
  _dr.wLength = 1; \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdGetDeviceStatus(pid, ptr, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x80; \
  _dr.bRequest = USB_REQUEST_GET_STATUS; \
  _dr.wValue = 0; \
  _dr.wIndex = 0; \
  _dr.wLength = 2; \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdGetInterfaceStatus(pid, interface, ptr, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x81; \
  _dr.bRequest = USB_REQUEST_GET_STATUS; \
  _dr.wValue = 0; \
  _dr.wIndex = (interface); \
  _dr.wLength = 2; \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdGetEndpointStatus(pid, endpoint, ptr, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x82; \
  _dr.bRequest = USB_REQUEST_GET_STATUS; \
  _dr.wValue = 0; \
  _dr.wIndex = (endpoint); \
  _dr.wLength = 2; \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdSetAddress(pid, address, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x00; \
  _dr.bRequest = USB_REQUEST_SET_ADDRESS; \
  _dr.wValue = (address); \
  _dr.wIndex = 0; \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSetConfiguration(pid, config, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x00; \
  _dr.bRequest = USB_REQUEST_SET_CONFIGURATION; \
  _dr.wValue = (config); \
  _dr.wIndex = 0; \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSetDeviceDescriptor(pid, type, index, lang_id, ptr, len, \
              cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x00; \
  _dr.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  _dr.wValue = ((type) << 8) | (index); \
  _dr.wIndex = (lang_id); \
  _dr.wLength = (len); \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdSetInterfaceDescriptor(pid, type, index, lang_id, ptr, len, \
              cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x01; \
  _dr.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  _dr.wValue = ((type) << 8) | (index); \
  _dr.wIndex = (lang_id); \
  _dr.wLength = (len); \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdSetEndpointDescriptor(pid, type, index, lang_id, ptr, len, \
                cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x02; \
  _dr.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  _dr.wValue = ((type) << 8) | (index); \
  _dr.wIndex = (lang_id); \
  _dr.wLength = (len); \
  cellUsbdControlTransfer((pid), (&_dr), (ptr), (cb), (arg)); })

#define cellUsbdSetDeviceFeature(pid, fs, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x00; \
  _dr.bRequest = USB_REQUEST_SET_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = 0; \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSetInterfaceFeature(pid, fs, interface, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x01; \
  _dr.bRequest = USB_REQUEST_SET_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = (interface); \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSetEndpointFeature(pid, fs, endpoint, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x02; \
  _dr.bRequest = USB_REQUEST_SET_FEATURE; \
  _dr.wValue = (fs); \
  _dr.wIndex = (endpoint); \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSetInterface(pid, interface, alt_setting, cb, arg) ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x01; \
  _dr.bRequest = USB_REQUEST_SET_INTERFACE; \
  _dr.wValue = (alt_setting); \
  _dr.wIndex = (interface); \
  _dr.wLength = 0; \
  cellUsbdControlTransfer((pid), (&_dr), NULL, (cb), (arg)); })

#define cellUsbdSynchFrame(pid, endpoint, pfn, cb, arg)  ({ \
  UsbDeviceRequest _dr; \
  _dr.bmRequestType = 0x82; \
  _dr.bRequest = USB_REQUEST_SYNCH_FRAME; \
  _dr.wValue = 0; \
  _dr.wIndex = (endpoint); \
  _dr.wLength = 2; \
  cellUsbdControlTransfer((pid), (&_dr), pfn, (cb), (arg)); })

/* Return Code */

#define CELL_USBD_PROBE_SUCCEEDED       (0)
#define CELL_USBD_PROBE_FAILED          (-1)
#define CELL_USBD_ATTACH_SUCCEEDED      (0)
#define CELL_USBD_ATTACH_FAILED         (-1)
#define CELL_USBD_DETACH_SUCCEEDED      (0)
#define CELL_USBD_DETACH_FAILED         (-1)

#ifdef __cplusplus
}
#endif

#endif /* __CELL_USBD_LIBUSBD_H__ */

