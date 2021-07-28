/*  SCE CONFIDENTIAL                                      */
/*  PLAYSTATION(R)3 Programmer Tool Runtime Library       */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */


#ifndef __CELL_RUDP_H__
#define __CELL_RUDP_H__


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* struct sockaddr */
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

    /** @defgroup module1 librudp
     * @{
     */

    /**
     * Type for microsecond time values.
     */
    typedef uint64_t CellRudpUsec;

    /**
     * Structure for custom memory allocator. Callbacks defined on 
     * <c>CellRudpAllocator</c> override the default memory 
     * allocation functions. This object is passed in the 
     * cellRudpInit() function to control library memory allocation 
     * behavior. Calling cellRudpInit() must be done before any other
     * library call is made, or not at all.
     */
    typedef struct CellRudpAllocator
    {
        /**
         * Allocates a block of memory.
         * @note A valid return pointer must be aligned on an 8-byte
         * boundary.
         * @param size The amount of memory to allocate.
         * @return 
         * Returns a valid pointer to memory on success, NULL on 
         * failure. Beginning of the block must be on an 8-byte boundary.
         */
        void *(*app_malloc)(size_t size);
        /**
         * Frees a block of memory.
         * @param ptr The pointer to free.
         */
        void (*app_free)(void *ptr);

    } CellRudpAllocator;


/** @name Event (common) IDs
 * Event IDs used in CellRudpEventHandler.
 * @{
 */

/**
 * Request for sending UDP data.
 */
#define CELL_RUDP_EVENT_SEND                    (1)

/**
 * The socket has been released.
 */
#define CELL_RUDP_EVENT_SOCKET_RELEASED         (2)

/**
 * A UDP packet is about to be sent out.
 * This is notified only when internal network I/O is enabled.
 * @note Diagnostic purposes only. Please return with 
 * CELL_RUDP_SUCCESS (0) immediately.
 * @note When a negative value is returned, the transmission will
 * be cancelled, which can be used to simulate packet loss.
 */
#define CELL_RUDP_EVENT_DIAG_SENT               (100)

/**
 * A UDP packet has just been received.
 * This is notified only when internal network I/O is enabled.
 * @note Diagnostic purposes only. Please return with 
 * CELL_RUDP_SUCCESS (0) immediately.
 * @note When a negative value is returned, the received UDP packet will
 * be discarded, which can be used to simulate packet loss.
 */
#define CELL_RUDP_EVENT_DIAG_RCVD               (101)
/** @} */

    /** 
     * Library common event handler. The handler is registered via 
     * cellRudpSetEventHandler().
     * @note The callback is made either by one who is calling
     * cellRudpProcessEvents() or by the internal thread if enabled.
     * @param soc Socket ID this event occurred on. 
     * @param data Pointer to the data.
     * @param len Size of 'data' in bytes.
     * @param addr Socket address.
     * @param addrlen Size of 'addr' in bytes.
     * @param arg Argument specified when this callback handler
     * is registered via cellRudpSetEventHandler().
     * @returns Must return 0 on success in any event, or one of the 
     * following negative values on failure of 
     * CELL_RUDP_EVENT_SEND event:
     * - CELL_RUDP_ERROR_WOULDBLOCK: sendto() returned EWOULDBLOCK.
     * - CELL_RUDP_ERROR_INVALID_SOCKET: sendto() returned other
     *   negative errors.
     *
     * When the event ID is CELL_RUDP_EVENT_SEND, the 'data' points
     * to data to be sent on 'soc' to destination address 'addr'.
     * When the event ID is CELL_RUDP_EVENT_SOCKET_RELEASED, the 
     * 'soc' indicates the socket that has been released (or no longer
     * used by librudp). The 'data', 'len', 'addr' and 'addrlen'
     * are not used in this event and the values are undefined.
     */
    typedef int (*CellRudpEventHandler) ( 
                                    int event_id,
                                    int soc,
                                    uint8_t const *data,
                                    size_t len,
                                    struct sockaddr const *addr,
                                    socklen_t addrlen,
                                    void *arg
                                    );

/* Context Event IDs */
/** @name Context Event IDs
 * Event IDs used in CellRudpContextEventHandler.
 * @{
 */

/**
 * Connection failed, or closed. 'error_code' indicates a cause
 * of error.
 */
#define CELL_RUDP_CONTEXT_EVENT_CLOSED          (1)

/**
 * Connection established.
 */
#define CELL_RUDP_CONTEXT_EVENT_ESTABLISHED     (2)

/**
 * Error occurred in this context.
 */
#define CELL_RUDP_CONTEXT_EVENT_ERROR           (3)

/**
 * The context became writable.
 */
#define CELL_RUDP_CONTEXT_EVENT_WRITABLE        (4)

/**
 * The context became readable.
 */
#define CELL_RUDP_CONTEXT_EVENT_READABLE        (5)

/**
 * Flush operation has completed.
 */
#define CELL_RUDP_CONTEXT_EVENT_FLUSHED         (6)

/** @} */

    /** 
     * Context event handler. The handler is registered
     * when a new context is created with cellRudpCreateContext().
     * @note All callbacks are made either by the thread calling
     * cellRudpProcessEvents() or the internal thread if enabled.
     * @param ctx_id Context ID.
     * @param event_id Type of event (CELL_RUDP_CONTEXT_EVENT_XXX)
     * @param error_id Error code.
     * @param arg Arbitrary pointer specified when context was created.
     */
    typedef void (*CellRudpContextEventHandler) ( 
                                    int ctx_id,
                                    int event_id,
                                    int error_code,
                                    void *arg
                                    );


    /**
     * Initializes librudp.
     * If allocator is set to 0, system memory allocation
     * (malloc/free) is used.
     * @note This function must be called prior to any other librudp
     * API calls. Otherwise, CELL_RUDP_ERROR_NOT_INITIALIZED is
     * returned.
     * @note Multithread safe.
     * @param[in] allocator Pointer to application memory allocator.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_ALREADY_INITIALIZED Already initialized.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     */
    int cellRudpInit(CellRudpAllocator *allocator);

    /**
     * Terminates librudp. It releases all global memory resources.
     * @note Multithread safe.
     * @note Calling cellRudpEnd will cause all blocked threads to
     * resume immediately. The blocking function will return
     * CELL_RUDP_ERROR_CANCELLED.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     */
    int cellRudpEnd(void);
    
    /**
     * Enables an internal thread for event processing including 
     * network I/O events. When enabled, cellRudpProcessEvents() must 
     * not be called, otherwise CELL_RUDP_ERROR_THREAD_IN_USE is returned.
     * Since network I/O operation is handled by the thread, the
     * CellRudpSendEventHandler will only be called for diagnostic messages.
     * All callbacks are made by the thread. The thread is destroyed
     * when cellRudpEnd() is called.
     * @note This function must be called before creating any
     * contexts, otherwise CELL_RUDP_ERROR_NOT_ACCEPTABLE is returned.
     * @note Multithread safe.
     * @param[in] stackSize Internal thread stack size. The lowest value
     * is capped at 4096 (4KB).
     * @param[in] priority Internal thread priority ranged 0 (highest) 
     * to 3071.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_THREAD_IN_USE Internal thread already in use.
     * @retval CELL_RUDP_ERROR_THREAD Thread related error occurred.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_INVALID_SOCKET Internal network I/O error.
     */
    int cellRudpEnableInternalIOThread(uint32_t stackSize, uint32_t priority);

    /**
     * Registers the common event handler. This handler is required. 
     * If it is not registered, a cellRudpInitiate() or cellRudpActivate()
     * call will fail and return CELL_RUDP_ERROR_NO_EVENT_HANDLER.
     * @param[in] handler Callback function.
     * @param[in] arg User-specific pointer passed to the callback function.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     */
    int cellRudpSetEventHandler(CellRudpEventHandler handler, void *arg);

    /**
     * Set maximum segment size. The value set with this function 
     * will be the default MSS for each context. The MSS value for
     * each context set with CELL_RUDP_OPTION_MAX_PAYLOAD must not
     * exceed the value set with this function, after accounting for
     * the segment header size librudp will add to the max payload
     * size.
     * @note This function must be called before creating any contexts.
     * @param[in] mss Maximum segment size in bytes.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     */
    int cellRudpSetMaxSegmentSize(uint16_t mss);

    /**
     * Get maximum segment size.
     * @param[in] mss Maximum segment size in bytes.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     */
    int cellRudpGetMaxSegmentSize(uint16_t *mss);

    /**
     * Processes internal pending events. All callbacks are made
     * by a thread calling this function. It is recommended that this
     * function be called at least once in 16.7 msec, or 60Hz.
     * @note Multithread safe.
     * @param[in] timeout Timeout value in microseconds. If 0 is set, 
     * this function returns immediately after pending events are 
     * processed.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     */
    int cellRudpProcessEvents(CellRudpUsec timeout);

    /**
     * Create context.
     * @note Multithread safe.
     * @param[in] handler Callback function.
     * @param[in] arg User-specific pointer passed to callback function.
     * @param[out] ctx_id Area to store a new context ID.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     */
    int cellRudpCreateContext(      CellRudpContextEventHandler handler,
                                    void *arg,
                                    int *ctx_id);

/** @name Multiplexing Mode
 * Defines multiplexing behavior of a newly created context.
 * @{
 */

/**
 * The Context is used for peer-to-peer communication which requires 
 * virtual port multiplexing. Underlying UDP socket and context 
 * will have 1:N relationship. 
 * @note This mode adds a 16-bit virtual port to each RUDP segment.
 */
#define CELL_RUDP_MUXMODE_P2P       (1)
/** @} */


    /**
     * Bind a context, a UDP socket and a local virtual port.
     * @param[in] ctx_id Context ID.
     * @param[in] soc UDP socket ID this context uses. 
     * @note The socket ID can not be shared by contexts with 
     * a different multiplexing mode.
     * @param[in] vport Local virtual port. The vport must be 
     * non-zero value. Otherwise, CELL_RUDP_ERROR_INVALID_VPORT will 
     * be returned.
     * @param[in] mux_mode Multiplexing mode.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_INVALID_MUXMODE Another mux mode has
     * already been assigned for the socket. (Current version supports
     * only one multiplexing mode and this error should never occur.)
     * @retval CELL_RUDP_ERROR_INVALID_VPORT 0 was specified in
     * CELL_RUDP_MUXMODE_P2P.
     * @retval CELL_RUDP_ERROR_VPORT_EXHAUSTED Internal vport resources
     * are exhausted.
     */
    int cellRudpBind(               int ctx_id, 
                                    int soc,
                                    uint16_t vport,
                                    uint8_t mux_mode);

    /**
     * Activate RUDP context to wait for incoming connection from the
     * specified remote node.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[in] to Remote address this context will be activated for.
     * @param[in] tolen Size of 'to' in bytes. (e.g. sizeof(struct sockaddr_in))
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_NO_EVENT_HANDLER Common event handler
     * has not been registered.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID: Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_CANCELLED Blocking operation cancelled.
     * @retval CELL_RUDP_ERROR_WOULDBLOCK Blocking timeout occurred before
     * connection establishment.
     * @retval CELL_RUDP_ERROR_NOT_BOUND The context has not been bound yet.
     * @retval CELL_RUDP_ERROR_ADDR_IN_USE There is another context that has
     * the same socket connected to the same remote address.
     * @retval CELL_RUDP_ERROR_IN_PROGRESS Operation is in progress.
     */
    int cellRudpActivate(           int ctx_id,
                                    struct sockaddr const *to, 
                                    socklen_t tolen);

    /**
     * Activate RUDP context and then initiate RUDP session to the
     * specified remote node.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[in] to Destination address.
     * @param[in] tolen Size of 'to' in bytes. (e.g. sizeof(struct sockaddr_in))
     * @param[in] vport Not used.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_NO_EVENT_HANDLER Common event handler
     * has not been registered.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_CANCELLED Blocking operation cancelled.
     * @retval CELL_RUDP_ERROR_WOULDBLOCK Blocking operation timed out.
     * @retval CELL_RUDP_ERROR_NOT_BOUND Not bound with cellRudpBind().
     * @retval CELL_RUDP_ERROR_ADDR_IN_USE There is another context that has
     * the same socket connected to the same remote address.
     * @retval CELL_RUDP_ERROR_IN_PROGRESS Operation is in progress.
     */
    int cellRudpInitiate(           int ctx_id, 
                                    struct sockaddr const *to, 
                                    socklen_t tolen, 
                                    uint16_t vport);

    /**
     * Terminate RUDP session.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID to be terminated.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @note Any threads waiting on a blocking context operation on
     * this context will be resumed immediately and
     * CELL_RUDP_ERROR_CANCELLED will be returned from the blocking
     * function.
     */
    int cellRudpTerminate(int ctx_id);


/** @name Message flags
 * Message flags used with cellRudpWrite() or cellRudpRead().
 * @{
 */

/**
 * Tell RUDP context not to wait in blocking mode. It has no effect
 * in non-blocking mode.
 */
#define CELL_RUDP_MSG_DONTWAIT          (0x01)

/**
 * Mark this message as 'latency critical'. RUDP tries to send this data
 * as quickly as possible. If CELL_RUDP_MSG_ORDER_CRITICAL is not set,
 * this datagram will be sent first before any outstanding data in
 * the send buffer. If CELL_RUDP_OPTION_NODELAY is disabled, or
 * Nagle's algorithm is enabled, then RUDP immediately processes
 * aggregation to send this data right away.
 */
#define CELL_RUDP_MSG_LATENCY_CRITICAL  (0x08)

/**
 * Force send data to be aligned on 32 bit boundaries. This may
 * cause padding octets inserted in the segment header option.
 */
#define CELL_RUDP_MSG_ALIGN_32          (0x10)

/**
 * Force send data to be aligned on 64 bit boundaries. This may
 * cause padding octets inserted in the segment header option.
 */
#define CELL_RUDP_MSG_ALIGN_64          (0x20)

/** @} */

    /**
     * Write application data.
     * @note In STREAM mode or DGRAM mode with both DC and OC enabled, 
     * the size of the data may be more than MSS - 64. In this case,
     * the data will be fragmented into multiple RUDP packets
     * if the size of the data plus header (added by librudp: max 64 bytes) 
     * is larger than the maximum segment size. In DGRAM mode, those
     * fragmented packets will be defragmented by librudp on the receiver
     * side when cellRudpRead() is called. If such fragmentation is
     * undesired, make sure that the size of data is equal to or smaller
     * than MSS - 64B. This fragmentation is not supported in DGRAM mode
     * when either the DC or OC options are disabled. In that case, the
     * size of the data must always be equal to or smaller than MSS - 64,
     * otherwise CELL_RUDP_ERROR_MSG_TOO_LARGE will be returned.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[in] data Data to be sent.
     * @param[in] len Size of application data.
     * @param[in] flags Message flags (CELL_RUDP_MSG_XXX).
     * @returns Returns amount of data written on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_CANCELLED Blocking operation cancelled.
     * @retval CELL_RUDP_ERROR_WOULDBLOCK Blocking operation timed out.
     * @retval CELL_RUDP_ERROR_MSG_TOO_LARGE Message too large.
     */
    int cellRudpWrite(int ctx_id, void const *data, size_t len, uint8_t flags);

    /**
     * Additional information for read data. The retransmission count
     * and delay may be useful to detect cheating. (e.g. a lag switch).
     * If DC=0, both retransmission count and delay are always 0.
     */
    typedef struct CellRudpReadInfo
    {
        /**
         * Size of this structure.
         */
        uint8_t size;

        /** 
         * The number of retransmissions made for this data. 
         */
        uint8_t retransmissionCount;

        /** 
         * Retransmission delay in milliseconds since the first 
         * transmission of this data. 
         */
        uint16_t retransmissionDelay;

    } CellRudpReadInfo;

    /**
     * Read application data.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[out] data Pointer to a buffer into which received data will be copied.
     * @param[in] len Size of the buffer in bytes.
     * @param[in] flags Message flags. Currently, CELL_RUDP_MSG_DONTWAIT
     * is the only flag that has effect on this function.
     * @param[out] info Additional information for read data.
     * Null may be passed in if the info is not necessary.
     * @returns Returns amount of data read on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_CANCELLED Blocking operation cancelled.
     * @retval CELL_RUDP_ERROR_WOULDBLOCK Blocking operation timed out.
     * @retval CELL_RUDP_ERROR_BUFFER_TOO_SMALL Buffer too small.
     * @retval CELL_RUDP_ERROR_END_OF_DATA No more data from the peer.
     */
    int cellRudpRead(               int ctx_id, 
                                    void *data, 
                                    size_t len, 
                                    uint8_t flags,
                                    CellRudpReadInfo *info);

    /**
     * Get data size application can write.
     * @note In STREAM mode or DGRAM mode with both DC and OC options 
     * enabled, the function may return a size larger than MSS. Writing 
     * such large size would cause fragmentation at the RUDP layer if
     * the size plus maximum header size (64) becomes more than the MSS.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @returns Returns amount of data application can write.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     */
    ssize_t cellRudpGetSizeWritable(int ctx_id);

    /**
     * Get data size application can read.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @returns Returns amount of data application can read.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_END_OF_DATA No more data from the peer.
     */
    ssize_t cellRudpGetSizeReadable(int ctx_id);

    /**
     * Flush the send buffer and make sure that the all data written
     * before this call actually reached the remote node.
     * The call causes a CELL_RUDP_CONTEXT_EVENT_FLUSHED event when all the 
     * outstanding data in the internal send buffer was acknowledged 
     * by the remote. This function is useful when the
     * application wants to insure delivery of the last written data.
     * @note This function may be called only once at a time, 
     * otherwise CELL_RUDP_ERROR_IN_PROGRESS is returned for subsequent
     * calls.
     * Application may continue to call cellRudpWrite() after this
     * function call.
     * @note If CELL_RUDP_OPTION_DELIVERY_CRITICAL is disabled, 
     * CELL_RUDP_CONTEXT_EVENT_FLUSHED will be notified as soon as the last
     * data is sent out to the network.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     * @retval CELL_RUDP_ERROR_CANCELLED Blocking operation cancelled.
     * @retval CELL_RUDP_ERROR_WOULDBLOCK Blocking operation timed out.
     * @retval CELL_RUDP_ERROR_IN_PROGRESS Operation is in progress.
     */
    int cellRudpFlush(int ctx_id);

    /**
     * Pass in received data from underlying (unreliable) layer.
     * @note Multithread safe.
     * @param[in] soc UDP socket ID on which the data is received.
     * @param[in] data Data received.
     * @param[in] datalen Size of the received data.
     * @param[in] from Source address of this data.
     * @param[in] fromlen Size of 'from' in bytes. (e.g. sizeof(struct sockaddr_in))
     * @note Make sure any padding in the 'from' structure is wiped to
     * zero before calling this function.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_SOCKET Invalid socket ID.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     * @retval CELL_RUDP_ERROR_THREAD_IN_USE Internal thread already in use.
     * @retval CELL_RUDP_ERROR_MSG_MALFORMED Received data is not an 
     * RUDP packet, or is corrupted.
     */
    int cellRudpNetReceived(        int soc, 
                                    uint8_t const *data, 
                                    size_t datalen,
                                    struct sockaddr const *from, 
                                    socklen_t fromlen);


/* Option IDs */

/** @name Option flags
 * Option flags used with cellRudpSetOption() or cellRudpGetOption().
 * @{
 */
/**
 * The maximum payload that will be transmitted. This is combined with the
 * librudp maximum header size (64) to set the segment size for this
 * context. Because the send and receive buffers are divided up based on
 * the context segment size, it is strongly recommended to set this
 * parameter to a value that closely matches the maximum payload the app
 * will send. Otherwise the buffer space may be quickly exhausted.
 * The default is 1346 (bytes).
 * The 'optval' is of type uint32_t.
 * @note The size must be smaller than both the send and receive buffers.
 * Also, the size must be equal to or smaller than the value set by 
 * cellRudpSetMaxSegmentSize().
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_MAX_PAYLOAD        (1)

/**
 * Send buffer size. The default value is 65536.
 * The 'optval' is of type uint32_t.
 * @note The size must not be smaller than the maximum
 * segment size.
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_SNDBUF             (2)

/**
 * Receive buffer size. The default value is 65536.
 * The 'optval' is of type uint32_t.
 * @note The size must not be smaller than the maximum
 * segment size.
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_RCVBUF             (3)

/**
 * No delay option.
 * The 'optval' is of type int.
 * - 0: Turn on Nagle's algorithm (default)
 * - 1: Turn off Nagle's algorithm
 * @note This option can be set at any time.
 */
#define CELL_RUDP_OPTION_NODELAY            (4)

/**
 * Delivery critical option.
 * The 'optval' is of type int.
 * - 0: Not delivery critical
 * - 1: Delivery critical (default)
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_DELIVERY_CRITICAL  (5)

/**
 * Order critical option.
 * The 'optval' is of type int.
 * - 0: Not order critical
 * - 1: Order critical (default)
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_ORDER_CRITICAL     (6)

/**
 * Non-blocking option. In blocking mode, the following functions will
 * block until completion.
 * - cellRudpInitiate()
 * - cellRudpActivate()
 * - cellRudpRead()
 * - cellRudpWrite()
 * - cellRudpFlush()
 *
 * The 'optval' is of type int.
 * - 0: blocking mode
 * - 1: non-blocking mode (default)
 *
 * @note This option can be set at any time.
 * @note If a context is set to non-blocking while there are threads
 * waiting on a blocking context operation, those threads are resumed
 * immediately and the blocking function returns
 * CELL_RUDP_ERROR_CANCELLED.
 */
#define CELL_RUDP_OPTION_NONBLOCK           (7)

/**
 * Transport type option.
 * The 'optval' is of type int.
 * - 0: Transport type is DGRAM (default)
 * - 1: Transport type is STREAM
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_STREAM             (8)

/**
 * Connection timeout value in milliseconds.
 * The 'optval' is of type uint32. Default is 60,000 [msec].
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_CONNECTION_TIMEOUT (9)

/**
 * Close-wait timeout value in milliseconds.
 * The 'optval' is of type uint32. Default is 0 [msec].
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_CLOSE_WAIT_TIMEOUT (10)

/**
 * Message aggregation timeout value in milliseconds.
 * When CELL_RUDP_OPTION_NODELAY option is disabled, the context 
 * tries to aggregate short messages into a segment before 
 * sending the segment to the network. 
 *
 * The 'optval' is of type uint32. Default is 30 [msec].
 *
 * @note Beware the transmission delay (typically the last segment)
 * introduced by this timeout value. If latency is most
 * critical, consider enabling CELL_RUDP_OPTION_NODELAY
 * or @sa CELL_RUDP_MSG_LATENCY_CRITICAL.
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 */
#define CELL_RUDP_OPTION_AGGREGATION_TIMEOUT (11)

/**
 * To get last error occurred with a context. This option is only
 * valid with cellRudpGetOption().
 * The 'optval' is of type int.
 */
#define CELL_RUDP_OPTION_LAST_ERROR         (14)

/**
 * To specify blocking read operation timeout in milliseconds.
 * If no data has been read and the timeout has been reached then
 * CELL_RUDP_ERROR_WOULDBLOCK is returned just as if the context
 * was specified to be non-blocking. If the timeout is set to
 * zero (the default) then the operation will never timeout.
 * The 'optval' is of type uint32.
 * @note This option can be set at any time.
 */
#define CELL_RUDP_OPTION_READ_TIMEOUT       (15)

/**
 * To specify blocking write operation timeout in milliseconds.
 * If no data has been written and the timeout has been reached then
 * CELL_RUDP_ERROR_WOULDBLOCK is returned just as if the context
 * was specified to be non-blocking. If the timeout is set to
 * zero (the default) then the operation will never timeout.
 * The 'optval' is of type uint32.
 * @note This option can be set at any time.
 */
#define CELL_RUDP_OPTION_WRITE_TIMEOUT      (16)

/**
 * To specify blocking flush operation timeout in milliseconds.
 * If the flush operation has not completed and the timeout has 
 * been reached then CELL_RUDP_ERROR_WOULDBLOCK is returned just as 
 * if the context was specified to be non-blocking. If the timeout 
 * is set to zero (the default) then the operation will never 
 * timeout.
 * The 'optval' is of type uint32.
 * @note This option can be set at any time.
 */
#define CELL_RUDP_OPTION_FLUSH_TIMEOUT      (17)

/**
 * To specify the duration in milliseconds between two keep alive
 * transmissions in idle condition.
 * default: 0 (keep alive disabled)
 * The 'optval' is of type uint32.
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 * @note New in 3.1.
 */
#define CELL_RUDP_OPTION_KEEP_ALIVE_INTERVAL (18)

/**
 * To specify the total amount of time that rudp will retransmit keep
 * alive packets when it receives no response from the remote
 * end. After this amount of time the connection is considered
 * dead. The connection will then be closed and the user will be
 * notified with a closed event.
 * default: 20000
 * The 'optval' is of type uint32.
 * @note This option can only be set in CELL_RUDP_STATE_IDLE.
 * @note New in 3.1.
 */
#define CELL_RUDP_OPTION_KEEP_ALIVE_TIMEOUT (19)

/** @} */


    /**
     * Set RUDP option. Some options are only allowed when the
     * target context is in the CELL_RUDP_STATE_IDLE. Please refer to
     * the description of each context option.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[in] option Option ID.
     * @param[in] optval Area where a new option value is stored.
     * @param[in] optlen Length of the option value.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_INVALID_OPTION Option is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     */
    int cellRudpSetOption(          int ctx_id,
                                    int option,
                                    void const *optval,
                                    size_t optlen);

    /**
     * Get RUDP option.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[in] option Option ID.
     * @param[out] optval Area where obtained value will be stored.
     * @param[in] optlen Length of the option value.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_INVALID_OPTION Option is invalid.
     */
    int cellRudpGetOption(          int ctx_id,
                                    int option,
                                    void *optval,
                                    size_t optlen);

    /**
     * Get local address info. This function is the librudp equivalent
     * of the standard getsockname function.
     * @note Multithread safe.
     * @note The context must have been bound with cellRudpBind,
     * otherwise CELL_RUDP_ERROR_NOT_BOUND is returned.
     * @note Any of the output parameters may be NULL, in which case
     * those parameters will be ignored.
     * @note The addr and addrlen parameters are ignored in the
     * current implementation of librudp and should not be used. They
     * may be supported in the future.
     * @param[in] ctx_id Context ID.
     * @param[out] soc The socket file descriptor.
     * @param[out] addr Local socket address. Currently not used.
     * @param[in,out] addrlen It should initially contain the size of
     * 'addr'. On return it will contain the actual length of the
     * address. Currently not used.
     * @param[out] vport The local virtual port.
     * @param[out] mux_mode Multiplexing mode.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_BOUND The context has not been bound yet.
     */
    int cellRudpGetLocalInfo(       int ctx_id,
                                    int *soc,
                                    struct sockaddr *addr,
                                    socklen_t *addrlen,
                                    uint16_t *vport,
                                    uint8_t *mux_mode);

    /**
     * Get remote address info. This function is the librudp equivalent
     * of the standard getpeername function.
     * @note Multithread safe.
     * @note The context must be in the CELL_RUDP_STATE_ESTABLISHED or
     * CELL_RUDP_STATE_CLOSE_WAIT states, otherwise
     * CELL_RUDP_ERROR_NOT_ACCEPTABLE is returned.
     * @note Any of the output parameters may be NULL, in which case
     * those parameters will be ignored.
     * @param[in] ctx_id Context ID.
     * @param[out] addr Remote socket address.
     * @param[in,out] addrlen It should initially contain the size of
     * 'addr'. On return it will contain the actual length of the
     * address.
     * @param[out] vport The remote virtual port.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_NOT_ACCEPTABLE Operation not acceptable.
     */
    int cellRudpGetRemoteInfo(      int ctx_id,
                                    struct sockaddr *addr,
                                    socklen_t *addrlen,
                                    uint16_t *vport);

    /** 
     * Information about specified context obtained by calling
     * cellRudpGetContextStatus().
     */
    typedef struct CellRudpContextStatus 
    {

/** @name RUDP context state
 * @{
 */
#define CELL_RUDP_STATE_IDLE        (0)
#define CELL_RUDP_STATE_CLOSED      (1)
#define CELL_RUDP_STATE_SYN_SENT    (2)
#define CELL_RUDP_STATE_SYN_RCVD    (3)
#define CELL_RUDP_STATE_ESTABLISHED (4)
#define CELL_RUDP_STATE_CLOSE_WAIT  (5)
/** @} */

        /**
         * Current context state.
         */
        uint32_t    state;

        /**
         * Parent context ID. Currently unused.
         */
        int parentId;

        /**
         * Number of child contexts. Currently unused.
         */
        uint32_t children;

        /**
         * Number of lost packets that were supposed to be received
         * from remote.
         */
        uint32_t    lostPackets;

        /** 
         * Number of packets sent to the remote.
         */
        uint32_t    sentPackets;

        /** 
         * Number of packets received from the remote.
         */
        uint32_t    rcvdPackets;

        /**
         * Number of bytes sent to the remote.
         */
        uint64_t    sentBytes;

        /**
         * Number of bytes received from the remote.
         */
        uint64_t    rcvdBytes;

        /**
         * Number of retransmissions made by this context.
         */
        uint32_t    retransmissions;

        /**
         * Observed RTT, in microseconds, measured by this context.
         */
        uint32_t    rtt;

    } CellRudpContextStatus;

    /**
     * Get current status of a context.
     * @note Multithread safe.
     * @param[in] ctx_id Context ID.
     * @param[out] status Status of this context.
     * @param[in] statusSize Size of the status structure.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     */
    int cellRudpGetContextStatus(   int ctx_id,
                                    CellRudpContextStatus *status,
                                    size_t statusSize);


/** @name Poll operation types.
 * @{
 */
#define CELL_RUDP_POLL_OP_ADD       (1)
#define CELL_RUDP_POLL_OP_MODIFY    (2)
#define CELL_RUDP_POLL_OP_REMOVE    (3)
/** @} */

/** @name Poll event types.
 * @{
 */
#define CELL_RUDP_POLL_EV_READ      (0x0001)
#define CELL_RUDP_POLL_EV_WRITE     (0x0002)
#define CELL_RUDP_POLL_EV_FLUSH     (0x0004)
#define CELL_RUDP_POLL_EV_ERROR     (0x0008)
/** @} */


    /**
     * Poll event structure.
     */
    typedef struct CellRudpPollEvent
    {
        /**
         * Context ID this event is associated with.
         */
        int ctx_id;

        /**
         * Requested events.
         */
        uint16_t reqevents;

        /**
         * Returned (detected) events.
         */
        uint16_t rtnevents;

    } CellRudpPollEvent;

    /**
     * Create a poll.
     * @param[in] size Typical number of contexts the new poll ID watches.
     * The value must be one or greater. This value is just a hint to
     * librudp. You can in fact add more contexts to the poll via
     * CELL_RUDP_POLL_OP_ADD than you originally specified for the 'size'
     * value in cellRudpPollCreate. Librudp will allocate more memory as
     * necessary.
     * @returns Non-negative poll ID on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_MEMORY Could not allocate memory.
     */
    int cellRudpPollCreate(size_t size);

    /**
     * Destroy a poll.
     * @param[in] poll_id ID of the poll to be destroyed.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_POLL_ID Poll ID is invalid.
     * @note If a poll is destroyed while another thread is blocking
     * on a cellRudpPollWait call, the blocked thread is resumed
     * immediately and the cellRudpPollWait call returns
     * CELL_RUDP_ERROR_CANCELLED.
     */
    int cellRudpPollDestroy(int poll_id);

    /**
     * Control a poll.
     * @param[in] poll_id Poll to be controlled.
     * @param[in] op Operation type. (CELL_RUDP_POLL_OP_XXX)
     * @param[in] ctx_id Context ID.
     * @param[in] events Events to be watched. (CELL_RUDP_POLL_EV_XXX)
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_CONTEXT_ID Context ID is invalid.
     * @retval CELL_RUDP_ERROR_INVALID_POLL_ID Poll ID is invalid.
     * @retval CELL_RUDP_ERROR_TOO_MANY_CONTEXTS Too many contexts.
     */
    int cellRudpPollControl(        int poll_id, 
                                    int op, 
                                    int ctx_id, 
                                    uint16_t events);

    /**
     * Wait for events on a poll.
     * @param[in] poll_id Poll to be watched.
     * @param[out] events Pointer to an event array in which 
     * observed events will be stored.
     * @param[in] eventlen Maximum number of events of type 
     * CellRudpPollEvent the 'events' can store.
     * @param[in] timeout Timeout value in microseconds. If zero
     * is specified, the function will immediately return.
     * @returns Returns the number of events stored in
     * the 'events' array. Zero may be returned if no event occurs
     * during the requested timeout.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_POLL_ID Poll ID is invalid.
     * @retval CELL_RUDP_ERROR_CANCELLED The wait was cancelled via
     * cellRudpPollCancel or cellRudpPollDestroy.
     *
     * @note This function may return with a CellRudpPoll object
     * with events member being 0. This indicates that the 
     * context ID associated with the event object has been
     * terminated or removed.
     */
    int cellRudpPollWait(           int poll_id, 
                                    CellRudpPollEvent *events, 
                                    size_t eventlen,
                                    CellRudpUsec timeout);

    /**
     * Causes any threads waiting on the poll to be resumed. The
     * cellRudpPollWait call will return CELL_RUDP_ERROR_CANCELLED. If
     * there are no threads waiting on cellRudpPollWait, then the
     * cancel will apply to the next cellRudpPollWait call, which will
     * return immediately with CELL_RUDP_ERROR_CANCELLED.
     * @param[in] poll_id Poll to cancel.
     * @retval CELL_RUDP_SUCCESS Success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     * @retval CELL_RUDP_ERROR_INVALID_POLL_ID Poll ID is invalid.
     */
    int cellRudpPollCancel(int poll_id);


    /** 
     * Statistics collected during the operation of librudp. Obtained
     * by calling cellRudpGetStatus.
     */
    typedef struct CellRudpStatus
    {
        /** @name Udp packet and byte counts */
        /* @{ */
        uint32_t sentUdpPackets;
        uint64_t sentUdpBytes;
        uint32_t rcvdUdpPackets;
        uint64_t rcvdUdpBytes;
        /* @} */

        /** @name cellRudpWrite (aka "user") packet and byte counts */
        /* @{ */
        uint32_t sentUserPackets;
        uint64_t sentUserBytes;
        uint32_t sentLatencyCriticalPackets;
        /* @} */

        /** @name cellRudpRead (aka "user") packet and byte counts */
        /* @{ */
        uint32_t rcvdUserPackets;
        uint64_t rcvdUserBytes;
        uint32_t rcvdLatencyCriticalPackets;
        /* @} */

        /** @name Packet counts broken down by rudp segment type */
        /* @{ */
        uint32_t sentSynPackets;
        uint32_t rcvdSynPackets;
        uint32_t sentUsrPackets;
        uint32_t rcvdUsrPackets;
        uint32_t sentPrbPackets;
        uint32_t rcvdPrbPackets;
        uint32_t sentRstPackets;
        uint32_t rcvdRstPackets;
        /* @} */

        /** @name Additional packet statistics */
        /* @{ */
        uint32_t lostPackets;
        uint32_t retransmittedPackets;
        uint32_t reorderedPackets;
        /* @} */

        /** @name Quality level 1 (dc=1 oc=1) user packet and byte counts */
        /* @{ */
        uint32_t sentQualityLevel1Packets;
        uint64_t sentQualityLevel1Bytes;
        uint32_t rcvdQualityLevel1Packets;
        uint64_t rcvdQualityLevel1Bytes;
        /* @} */

        /** @name Quality level 2 (dc=1 oc=0) user packet and byte counts */
        /* @{ */
        uint32_t sentQualityLevel2Packets;
        uint64_t sentQualityLevel2Bytes;
        uint32_t rcvdQualityLevel2Packets;
        uint64_t rcvdQualityLevel2Bytes;
        /* @} */

        /** @name Quality level 3 (dc=0 oc=1) user packet and byte counts */
        /* @{ */
        uint32_t sentQualityLevel3Packets;
        uint64_t sentQualityLevel3Bytes;
        uint32_t rcvdQualityLevel3Packets;
        uint64_t rcvdQualityLevel3Bytes;
        /* @} */

        /** @name Quality level 4 (dc=0 oc=0) user packet and byte counts */
        /* @{ */
        uint32_t sentQualityLevel4Packets;
        uint64_t sentQualityLevel4Bytes;
        uint32_t rcvdQualityLevel4Packets;
        uint64_t rcvdQualityLevel4Bytes;
        /* @} */

        /** @name Current number of contexts in librudp */
        /* @{ */
        uint32_t currentContexts;
        /* @} */
        
        /** @name Memory usage statistics */
        /* @{ */
        uint32_t allocs;
        uint32_t frees;
        uint32_t memCurrent;
        uint32_t memPeak;
        /* @} */

        /** @name Connection statistics */
        /* @{ */
        uint32_t establishedConnections;
        uint32_t failedConnections;
        /* @} */

        /** @name Breakdown of failed connections by reason */
        /* @{ */
        uint32_t failedConnectionsReset;
        uint32_t failedConnectionsRefused;
        uint32_t failedConnectionsTimeout;
        uint32_t failedConnectionsVersionMismatch;
        uint32_t failedConnectionsTransportTypeMismatch;
        uint32_t failedConnectionsQualityLevelMismatch;
        /* @} */
    } CellRudpStatus;

    /**
     * Get status of librudp.
     * @note Multithread safe.
     * @param[out] status librudp status structure.
     * @param[in] statusSize Size of the status structure.
     * @returns Returns CELL_RUDP_SUCCESS (0) on success.
     * @retval CELL_RUDP_ERROR_NOT_INITIALIZED Not initialized.
     * @retval CELL_RUDP_ERROR_INVALID_ARGUMENT Invalid argument.
     */
    int cellRudpGetStatus(CellRudpStatus *status, size_t statusSize);



/** @name Miscellaneous macros.
 * @{
 */
/**
 * May be used to specify indefinite timeout for cellRudpPollWait().
 */
#define CELL_RUDP_USEC_INDEFINITE   (0xffffffffffffffffllu)
/** @} */

/* Error Code (librudp: 0x80770001 - 0x807700ff) */

/** @cond */
#ifndef CELL_ERROR_FACILITY_RUDP
#define CELL_ERROR_FACILITY_RUDP (0x077)
#endif

#define CELL_RUDP_ERROR(_sts) (0x80000000 | (CELL_ERROR_FACILITY_RUDP << 16) | (_sts))
/** @endcond */

/** @name Error code
 * @{
 */
#define CELL_RUDP_SUCCESS                                           (0)
#define CELL_RUDP_ERROR_NOT_INITIALIZED              CELL_RUDP_ERROR(1)
#define CELL_RUDP_ERROR_ALREADY_INITIALIZED          CELL_RUDP_ERROR(2)
#define CELL_RUDP_ERROR_INVALID_CONTEXT_ID           CELL_RUDP_ERROR(3)
#define CELL_RUDP_ERROR_INVALID_ARGUMENT             CELL_RUDP_ERROR(4)
#define CELL_RUDP_ERROR_INVALID_OPTION               CELL_RUDP_ERROR(5)
#define CELL_RUDP_ERROR_INVALID_MUXMODE              CELL_RUDP_ERROR(6)
#define CELL_RUDP_ERROR_MEMORY                       CELL_RUDP_ERROR(7)
#define CELL_RUDP_ERROR_INTERNAL                     CELL_RUDP_ERROR(8)
#define CELL_RUDP_ERROR_CONN_RESET                   CELL_RUDP_ERROR(9)
#define CELL_RUDP_ERROR_CONN_REFUSED                 CELL_RUDP_ERROR(10)
#define CELL_RUDP_ERROR_CONN_TIMEOUT                 CELL_RUDP_ERROR(11)
#define CELL_RUDP_ERROR_CONN_VERSION_MISMATCH        CELL_RUDP_ERROR(12)
#define CELL_RUDP_ERROR_CONN_TRANSPORT_TYPE_MISMATCH CELL_RUDP_ERROR(13)
#define CELL_RUDP_ERROR_CONN_QUALITY_LEVEL_MISMATCH  CELL_RUDP_ERROR(14)
#define CELL_RUDP_ERROR_THREAD                       CELL_RUDP_ERROR(15)
#define CELL_RUDP_ERROR_THREAD_IN_USE                CELL_RUDP_ERROR(16)
#define CELL_RUDP_ERROR_NOT_ACCEPTABLE               CELL_RUDP_ERROR(17)
#define CELL_RUDP_ERROR_MSG_TOO_LARGE                CELL_RUDP_ERROR(18)
#define CELL_RUDP_ERROR_NOT_BOUND                    CELL_RUDP_ERROR(19)
#define CELL_RUDP_ERROR_CANCELLED                    CELL_RUDP_ERROR(20)
#define CELL_RUDP_ERROR_INVALID_VPORT                CELL_RUDP_ERROR(21)
#define CELL_RUDP_ERROR_WOULDBLOCK                   CELL_RUDP_ERROR(22)
#define CELL_RUDP_ERROR_VPORT_IN_USE                 CELL_RUDP_ERROR(23)
#define CELL_RUDP_ERROR_VPORT_EXHAUSTED              CELL_RUDP_ERROR(24)
#define CELL_RUDP_ERROR_INVALID_SOCKET               CELL_RUDP_ERROR(25)
#define CELL_RUDP_ERROR_BUFFER_TOO_SMALL             CELL_RUDP_ERROR(26)
#define CELL_RUDP_ERROR_MSG_MALFORMED                CELL_RUDP_ERROR(27)
#define CELL_RUDP_ERROR_ADDR_IN_USE                  CELL_RUDP_ERROR(28)
#define CELL_RUDP_ERROR_ALREADY_BOUND                CELL_RUDP_ERROR(29)
#define CELL_RUDP_ERROR_ALREADY_EXISTS               CELL_RUDP_ERROR(30)
#define CELL_RUDP_ERROR_INVALID_POLL_ID              CELL_RUDP_ERROR(31)
#define CELL_RUDP_ERROR_TOO_MANY_CONTEXTS            CELL_RUDP_ERROR(32)
#define CELL_RUDP_ERROR_IN_PROGRESS                  CELL_RUDP_ERROR(33)
#define CELL_RUDP_ERROR_NO_EVENT_HANDLER             CELL_RUDP_ERROR(34)
#define CELL_RUDP_ERROR_PAYLOAD_TOO_LARGE            CELL_RUDP_ERROR(35)
#define CELL_RUDP_ERROR_END_OF_DATA                  CELL_RUDP_ERROR(36)
#define CELL_RUDP_ERROR_ALREADY_ESTABLISHED          CELL_RUDP_ERROR(37)
#define CELL_RUDP_ERROR_KEEP_ALIVE_FAILURE           CELL_RUDP_ERROR(38)
/** @} */

    /** @} end of module1 librudp */

#ifdef __cplusplus
}
#endif


#endif /* __CELL_RUDP_H__ */
