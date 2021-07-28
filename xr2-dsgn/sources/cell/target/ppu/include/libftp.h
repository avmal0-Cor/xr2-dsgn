/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved. 
 */

#ifndef __CELL_FTP_SERVICE_H__
#define __CELL_FTP_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************
 * DEFINES
 ***********************/
/** Error conditions reported by libftp */
#define CELL_FTP_SERVICE_OK CELL_OK
/** Network access has not been initialized */
#define CELL_FTP_SERVICE_ERROR_NETWORK_NOT_INITIALIZED  CELL_ERROR_CAST(0x80130701)
/** HDD access has not been initialized */
#define CELL_FTP_SERVICE_ERROR_STORAGE_NOT_INITIALIZED  CELL_ERROR_CAST(0x80130702)
/** Real-time Clock has not been initialized */
#define CELL_FTP_SERVICE_ERROR_RTCLOCK_NOT_INITIALIZED  CELL_ERROR_CAST(0x80130703)
/** An instance of the service is already running */
#define CELL_FTP_SERVICE_ERROR_ALREADY_RUNNING          CELL_ERROR_CAST(0x80130704)
/** Service is not currently running */
#define CELL_FTP_SERVICE_ERROR_NOT_RUNNING              CELL_ERROR_CAST(0x80130705)
/** A core network error has occurred */
#define CELL_FTP_SERVICE_ERROR_NETWORK_ERROR            CELL_ERROR_CAST(0x80130706)
/** Null pointer error */
#define CELL_FTP_SERVICE_ERROR_NULL_POINTER             CELL_ERROR_CAST(0x80130707)
/** An internal error has occurred */
#define CELL_FTP_SERVICE_ERROR_INTERNAL                 CELL_ERROR_CAST(0x801307FF)

/************************
 * TYPES
 ***********************/

/** @brief Enumerated type of the possible FTP service events */
typedef enum CellFtpServiceEvent
{
	
	CELL_FTP_SERVICE_EVENT_FATAL = -1, /**< The service has terminated due to a fatal error */
	CELL_FTP_SERVICE_EVENT_STARTED, /**< The local FTP service has started successfully */
	CELL_FTP_SERVICE_EVENT_STOPPED, /**< The local FTP service has been gracefully shut down */
	CELL_FTP_SERVICE_EVENT_CLIENT_CONNECTED, /**< A new connection request has been received and processed */
	CELL_FTP_SERVICE_EVENT_CLIENT_DISCONNECTED, /**< An existing connection has been closed */
  CELL_FTP_SERVICE_EVENT_SHUTDOWN /**< Service has received the remote shutdown command and has shut down */
} CellFtpServiceEvent;

/************************
 * CALLBACK FUNCTIONS
 ***********************/

/**
 * @brief FTP callback function
 *
 * @param event Event code of received event
 * @param data Pointer to associated event data
 * @param datalen Size of associated event data
 *
 * This is the function signature for an FTP callback function. The data and datalen parameters are currently
 * unused, but will in future revisions provide a means of passing more information about the event.
 *
 * @see cellFtpServiceRegisterHandler
 * @see cellFtpServiceUnregisterHandler
 * @see CellFtpServiceEvent
 */
typedef void (*CellFtpServiceEventHandler)( CellFtpServiceEvent event, void *data, size_t datalen );

/************************
 * FUNCTIONS
 ***********************/

/**
 * @brief Initialises and starts the local FTP service
 *
 * @retval CELL_FTP_SERVICE_OK Normal termination
 * @retval <0 Error
 *
 * This function initializes the network connections responsible for handling new FTP client connection 
 * requests and starts the service on a separate thread.
 *
 * @see cellFtpServiceStop
 */
int cellFtpServiceStart(void);

/**
 * @brief Notifies the FTP service that it should shut down
 *
 * @param result Pointer to 64-bit unsigned integer in which to store exit state of ftp service thread
 *
 * @retval CELL_FTP_SERVICE_OK Normal termination
 * @retval <0 Error
 *
 * This function sets the exit state flag in order to notify the local FTP service that it should start
 * an orderly shutdown of all connections. Once it has done this, it rejoins the service thread and waits
 * for termination, at which point it stores the thread痴 exit status at the specified address.
 *
 * The result stored when calling this function is, basically, the return value of the service thread. This 
 * returns 0 for normal termination, or an error code if fatal errors were encountered during the normal 
 * running of the service.
 *
 * @see cellFtpServiceStart
 */
int cellFtpServiceStop(uint64_t *result);

/**
 * @brief Registers a function for event callbacks
 *
 * @param handler Pointer to function to call back to, when notifiable events occur during the running of the service.
 *
 * @retval CELL_FTP_SERVICE_OK Normal termination
 * @retval <0 Error
 *
 * This function registers a function to call when a CellFtpServiceEvent occurs during the normal running of the 
 * service. Supported event types are detailed in the CellFtpServiceEvent enumeration. The number of event types 
 * for which there will be notifications is expected to increase in future versions of the library. Program the 
 * application so that unknown events will be ignored.
 *
 * @see CellFtpServiceEvent
 * @see cellFtpServiceUnregisterHandler
 * @see cellFtpServiceEventHandler
 */
int cellFtpServiceRegisterHandler(CellFtpServiceEventHandler handler);

/**
 * @brief Unregisters the callback function
 *
 * This function deletes the association to the registered callback function to stop receiving event notifications
 * from the local FTP service.
 *
 * @see cellFtpServiceRegisterHandler
 * @see cellFtpServiceEventHandler
 */
void cellFtpServiceUnregisterHandler(void);

#ifdef __cplusplus
}
#endif

#endif
