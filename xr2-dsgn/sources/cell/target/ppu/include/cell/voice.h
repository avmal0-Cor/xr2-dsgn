/*****************************************************************
 * SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (c) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * R & D Division, Sony Computer Entertainment Inc.
 *****************************************************************/

#ifndef __CELL_VOICE_H__
#define __CELL_VOICE_H__

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

#include "voiceTypes.h"

/// Initializes System Voice Service for use with system memory space.
/// Your application must use <c>cellVoiceStart()</c> to start System Voice Service.
/// <c>cellVoiceInit()</c> does the following:
/// <ul>
/// <li>Initializes the System Voice Service.</li> 
/// <li>Allocates resources for the System Voice Service.</li>
/// </ul> 
/// @param pArg         [in] Initialization parameters.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_INITIALIZED The libvoice library has already been initialized.
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_ADDRESS_INVALID Invalid address.
/// @retval CELL_VOICE_ERROR_EVENT_QUEUE Event Queue error.
/// @retval CELL_VOICE_ERROR_SHAREDMEMORY Shared memory error.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceInit(CellVoiceInitParam *pArg);

/// Initializes System Voice Service for use with game memory space.
/// Your application must pass initialization parameters <c>CELLVOICE_APPTYPE_GAME_1MB</c> 
/// for the application type, and the current version of libvoice that the application uses (CELLVOICE_VERSION_100 for the current release)
/// to initialize the System Voice Service for use with game memory space.
/// Your application must use <c>cellVoiceStartEx()</c> to start System Voice Service.
/// <c>cellVoiceInitEx()</c> does the following:
/// <ul>
/// <li>Initializes the System Voice Service.</li> 
/// <li>Allocates resources for the System Voice Service.</li>
/// </ul> 
/// Your application must pass <c>CellVoiceAppType</c> and <c>CELLVOICE_VERSION_100</c> to <c>cellVoiceInitEx()</c> 
/// to initialize the System Voice Service.
/// @param pArg         [in] Initialization parameters.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_INITIALIZED The libvoice library has already been initialized.
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_ADDRESS_INVALID Invalid address.
/// @retval CELL_VOICE_ERROR_EVENT_QUEUE Event Queue error.
/// @retval CELL_VOICE_ERROR_SHAREDMEMORY Shared memory error.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceInitEx(CellVoiceInitParam *pArg);

///	Terminates System Voice Service.
/// <c>cellVoiceEnd()</c> does the following:
/// <ul>
/// <li>Frees resources.</li>
/// <li>Exits System Voice Service.</li>
/// </ul>
/// @retval CELL_OK Successful completion.  
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_SHAREDMEMORY Shared memory error.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceEnd();

///	Starts the System Voice Service with system memory space.
/// The application calls <c>cellVoiceStart()</c> to request the start of the System Voice Service.  
/// Changes the port state from IDLE to READY if the topology validation is successful. 
/// 
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @note Must be used in conjunction with <c>cellVoiceInit()</c>.
int cellVoiceStart();

///	Starts the System Voice Service with game memory space.
/// The application provides a memory container and calls <c>cellVoiceStartEx()</c> to request the start 
/// of the System Voice Service. Changes the port state from IDLE to READY if the topology validation is 
/// successful.  It does not use system memory, and allows game chat and the system Background Music feature to
/// coexist at the same time. If the <c><i>appType</i></c> and the size of the memory container do not match, 
/// it returns <c>CELL_VOICE_ERROR_ARGUMENT_INVALID</c>.
/// @param pArg         [in] System Voice Service start parameters.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument: the <c><i>appType</i></c> and the size of the memory container do not match.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @note Must be used in conjunction with <c>cellVoiceInitEx()</c>.
int cellVoiceStartEx(CellVoiceStartParam *pArg);

///	Stops the System Voice Service. 
/// The System Voice Service releases memory and detaches.
/// @retval CELL_OK Successful completion.  
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_SERVICE_DETACHED The service was not previously attached. 
/// @retval CELL_VOICE_ERROR_SHAREDMEMORY Shared memory error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceStop();

///
/// Flushes the port's internal buffer.
/// @param portId  [in] The port ID.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error. 
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceResetPort(uint32_t portId);


///
/// Allows the application to debug the port topology. 
/// System Voice Service does the following:
/// <ul>
/// <li>Validates each port's binding.</li>
/// <li>Returns the number of the created ports.</li>
/// <li>Returns the information for the created ports.</li>
/// </ul> 
/// 
/// @param[out] numPort [out] Number of created ports. 
/// @param[out] pInfo   [out] List of port information.  If <c>pInfo[n]->numEdge = 0</c>, 
///						the connect is invalid.  If <c>pInfo[n]->numEdge != 0</c>, the connection 
///						is valid.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
int cellVoiceDebugTopology(uint32_t *numPort, CellVoiceBasePortInfo *pInfo);

///
/// Creates the inbound input port (IPort) and the outbound output port (OPort).
/// 
/// @param portId  [out] The port ID.
/// @param pArg    [in] Port Parameters.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceCreatePort(uint32_t *portId, const CellVoicePortParam *pArg);

///
/// Updates the specified port with parameters specified to the <c>CellVoicePortParam</c> structure. <c><i>portType</i></c> cannot be updated. There is no effect in updating 
/// device <c><i>playerId</i></c> and <c><i>pcmaudio</i></c> structures. 
/// @param portId   [in] The port ID.
/// @param pArg     [in] Port parameters.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument. 
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources. 
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceUpdatePort(uint32_t portId, const CellVoicePortParam *pArg);

///
/// Connects the specified input port (IPort) to the specified output port (OPort).
/// To avoid recursive connections, the reverse connection
/// is not allowed. That is, connecting OPort to IPort is not allowed.
/// @param ips  [in] The input port ID.
/// @param ops  [in] The output port ID.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceDisconnectIPortFromOPort    
int cellVoiceConnectIPortToOPort(uint32_t ips, uint32_t ops);

///
/// Disconnects the specified input port (IPort) from the specified output port (OPort).
/// @param ips  [in] The input port ID.
/// @param ops  [in] The output port ID.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceConnectIPortToOPort 
int cellVoiceDisconnectIPortFromOPort(uint32_t ips, uint32_t ops);

///
/// Deletes the specified port. 
/// It releases the port memory and binding. If the port is a microphone or a speaker device, it
/// also deletes the device.
/// @param portId   [in] The port ID. 
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceDeletePort(uint32_t portId);

///
/// Writes data to the specified input port's (IPort) input buffer.
/// @param ips          [in] The input port ID.
/// @param data         [in] A constant buffer pointer to hold the write data.
/// @param size     [in/out] For input, the size of the data to write. For
/// 					     output, the actual number of bytes written.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_SERVICE_DETACHED System Voice Service is detached. 
int cellVoiceWriteToIPort(uint32_t ips, const void *data, uint32_t *size);

///
/// Writes data to the specified input port's (IPort) input buffer, with number of lost frames.
/// @param ips          [in] The input port ID.
/// @param data         [in] A constant buffer pointer to hold the write data.
/// @param size     [in/out] For input, the size of the data to write. For
/// 					     output, the actual number of bytes written.
/// @param numFrameLost [in] The number of frames sent by the application that were lost when
///                          it detected the frame drop over the network.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_SERVICE_DETACHED System Voice Service is detached. 
int cellVoiceWriteToIPortEx(uint32_t ips, const void *data, uint32_t *size, uint32_t numFrameLost);

///
/// Reads data from the specified output port's (OPort) output buffer.
/// @param ops              [in] The output port ID.
/// @param data            [out] The buffer pointer to hold the read data.
/// @param size         [in/out] For input, the number of requested bytes.
///                              For output, the actual number of  bytes	read.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_SERVICE_DETACHED System Voice Service is detached. 
int cellVoiceReadFromOPort(uint32_t ops, void *data, uint32_t *size);


///
/// Creates an event queue for receiving System Voice Service events. The application registers the event 
/// queue with <c>cellVoiceSetNotifyEventQueue()</c>, unregisters the event queue with 
/// <c>cellVoiceRemoveNotifyEventQueue()</c> and frees the queue with <c>sys_event_queue_destroy()</c>.
/// @param id		[out] Address of the returned event queue ID.
/// @param key      [out] Address of the returned event queue key.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_EVENT_QUEUE Event Queue error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceSetNotifyEventQueue, cellVoiceRemoveNotifyEventQueue 
int cellVoiceCreateNotifyEventQueue(sys_event_queue_t *id, sys_ipc_key_t *key );

///
/// Registers an event queue to receive a System Voice Service event.
/// @param key      [in] Event queue key.
/// @param source   [in] The source of the queue. 
///                 The source is later returned in the <c>sys_event_t</c> structure when an 
///					event is received from System Voice Service. 
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_EVENT_QUEUE Event Queue error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceCreateNotifyEventQueue, cellVoiceRemoveNotifyEventQueue  
int cellVoiceSetNotifyEventQueue(sys_ipc_key_t key, uint64_t source);

///
/// Unregisters an event queue, set with <c>cellVoiceSetNotifyEventQueue()</c>, for receiving a System Voice 
/// Service event notification. Use <c>sys_event_queue_destroy()</c> to free the event queue.
/// @param key      [in] Event queue key.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_EVENT_QUEUE Event Queue error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceCreateNotifyEventQueue, cellVoiceSetNotifyEventQueue  
int cellVoiceRemoveNotifyEventQueue(sys_ipc_key_t key);

///
/// Enables or disables mute at all input ports (IPorts) and output ports (OPorts). This does not
/// change the <c>CellVoicePortParam.<i>bMute</i></c>  flag setting for each port.  When a port is muted, its data is still being processed
/// but with an effective volume of 0.  
/// @param bMuted   [in] If true, enables mute for all the IPorts/OPorts, no audio is generated.
///                 If false, the audio generation depends on the <c>CellVoicePortParam.<i>bMute</i></c> flag setting for each port.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see CellVoicePortParam
int cellVoiceSetMuteFlagAll(uint16_t bMuted);

///
/// Enables or disables mute for the specified port. When a port is muted, its data is still being processed but with an effective 
/// volume of 0. 
/// @param portId   [in] The port ID.
/// @param bMuted   [in] If true,  mute is enabled for the port, no audio is generated.
///                     If false, mute is disabled for the port, audio is generated.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceGetMuteFlag, cellVoiceSetMuteFlagAll 
int cellVoiceSetMuteFlag(uint32_t portId, uint16_t bMuted);

///
/// Gets the mute flag for the specified port.
/// @param portId   [in] The port ID.
/// @param bMuted  [out] The address containing the <c><i>bMute</i></c> flag value
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceSetMuteFlag, cellVoiceSetMuteFlagAll 
int cellVoiceGetMuteFlag(uint32_t portId, uint16_t *bMuted);

///
/// Sets the volume for the specified port
/// @param portId   [in] The port ID.
/// @param volume   [in] The value of volume. A value of 0 silences the port.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceGetVolume  
int cellVoiceSetVolume(uint32_t portId, float volume);

///
/// Gets the volume for the specified port
/// @param[in] portId   [in] The port ID.
/// @param[out] volume  [out] The address containing the value of the volume.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @see cellVoiceSetVolume 
int cellVoiceGetVolume(uint32_t portId, float *volume);

///
/// Sets a port's attribute to a specified value. 
/// See CellVoicePortAttr for a description of the attributes for which the application can
/// set values. 
/// @param portId   [in] The port ID.
/// @param attr     [in] An enumeration value that specifies the port attribute
///                 for which to set a value. It can pass through CellMicSignalAttr. 
/// @param attrValue   	[in] The new attribute value for the port.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_DEVICE_NOT_PRESENT Microphone device is not present.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
/// @retval other libmic related error code.
int cellVoiceSetPortAttr(uint32_t portId, CellVoicePortAttr attr, void *attrValue);

///
/// Gets the calculated values of the specified port attribute for the specified port.
/// @param portId   		[in] The port ID.
/// @param attr   			[in] An enumeration value that specifies the port attribute for which to 
/// 						get the calculated value. It can pass through CellMicSignalAttr.
/// @param attrValue   	[out] The address of the actual calculated attribute value.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_DEVICE_NOT_PRESENT Microphone device is not present.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceGetPortAttr(uint32_t portId, CellVoicePortAttr attr, void *attrValue);

///
/// Gets the calculated values of the signal state for the microphone port. This function is only applicable
/// to the microphone device.
/// @param portId   		[in] The microphone port id.
/// @param attr   			[in] An enumeration value that specifies a CellMicSignalState.
/// @param attrValue   	[out] The address of the actual calculated attribute value.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_DEVICE_NOT_PRESENT Microphone device is not present.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceGetSignalState(uint32_t portId, CellVoicePortAttr attr, void *attrValue);

///
/// Sets the specific bit rate for the specified port. 
/// @param portId   [in] The port ID.
/// @param bitrate  [in] The bit rate, provided by the enumeration.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_ARGUMENT_INVALID Invalid argument.
/// @retval CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT Insufficient resources. 
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceSetBitRate(uint32_t portId, CellVoiceBitRate bitrate);

///
/// Gets the actual bit rate value for the specified port
/// @param portId       [in] The port ID.
/// @param bitrate     [out] The actual bit rate value (not the enumeration).
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceGetBitRate(uint32_t portId, uint32_t *bitrate);

///
/// Gets basic port information for the specified port.
/// @param portId       [in] The port ID.
/// @param pInfo    [in/out] The pointer to the basic port information. The application must allocate 
///                          the memory for <c>pInfo->pEdge</c> if it wants the array of the port's 
///                          connection edge information returned. System Voice Service goes 
///							 through the internal binding to get the edge connection information. 
///                          This is a time-consuming process.
///                          The application should pass <c>pInfo->pEdge</c> as <c>NULL</c>
///						     if it  does not need to know the connection edges.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.
/// @retval CELL_VOICE_ERROR_SERVICE_DETACHED System Voice Service is detached. 
int cellVoiceGetPortInfo(uint32_t portId, CellVoiceBasePortInfo *pInfo);

///
/// Pauses streaming of the specified port.
/// @param portId  [in] The port ID.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error. 
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoicePausePort(uint32_t portId);

///
/// Resumes streaming of the specified port.
/// @param portId  [in] The port ID.
/// @retval CELL_OK Successful completion.
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.  
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceResumePort(uint32_t portId);

///
/// Pauses streaming for all ports.
/// @retval CELL_OK Successful completion. 
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoicePausePortAll();

///
/// Resumes streaming for all ports.
/// @retval CELL_OK Successful completion.  
/// @retval CELL_VOICE_ERROR_LIBVOICE_NOT_INIT The libvoice library has not been initialized.
/// @retval CELL_VOICE_ERROR_TOPOLOGY Topology error.  
/// @retval CELL_VOICE_ERROR_EVENT_DISPATCH Voice event dispatch fails.
int cellVoiceResumePortAll();

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}

#endif
#endif // __CELL_VOICE_H__