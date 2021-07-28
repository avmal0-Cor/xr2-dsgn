/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONTROL_CONSOLE_H
#define __CONTROL_CONSOLE_H

// For size_t
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @short Default port
*
* This is used where ports make sense, for instance, network ports,
* logical console ID's, and so on.  It's completely optional and just
* used to provide a little bit of consistency. 
*/
#define CELL_CONSOLE_DEFAULT_PORT 1713
	
/**
* @short Maximum number of active connections.
*
* Defines the maximum number of active connections. 
*/
#define CELL_CONSOLE_MAX_CONNECTIONS 16

/**
* @short Success from most console functions 
*
* Returned by most console functions on success
*/
#define CELL_CONSOLE_OK              0x00000000U

/**
* @short Base error value
*
* A return value equal to or large than this typically indicates an error
*/
#define CELL_CONSOLE_ERROR_BASE      0xfffffff0U

/**
* @short Console not intialized
*
* The console has not been initialized
*/
#define CELL_CONSOLE_NOT_INITIALIZED 0xffffffffU

/**
* @short Invalid Argument
*
* An invalid argument was specified
*/
#define CELL_CONSOLE_EINVAL          0xfffffffeU


/**
* @short Insufficient memory
*
* A memory allocation failed due to insufficient memory
*/
#define CELL_CONSOLE_ENOMEM          0xfffffffdU

/**
* @short I/O device error
*
* An error reading or writing to an IO device (network, file, etc.)
*/
#define CELL_CONSOLE_IO_ERROR        0xfffffffcU

/**
 * @short Memalign callback for the Control Console
 *
 * The type of the callback function used by the Control Console to allocate memory.
 *
 * @param uiAlign The required alignment of the memory.  '1' is equivalent to
 *        a malloc.
 * @param uiSize The number of bytes the Control Console needs.
 *
 * @return A pointer to a buffer of size <c><i>uiSize</i></c>
 *
 * @sa CellConsoleCustomFree, cellConsoleInitWithAllocator, cellConsoleMemalign
 */
typedef void *(*CellConsoleCustomMemalign)(size_t uiAlign, size_t uiSize);

/**
 * @short Free callback for the Control Console
 *
 * The type of the callback function used by the Control Console to free memory.
 *
 * @param pvAddress The address to free, returned by a call to
 *        CellConsoleCustomMemalign()
 *
 * @sa CellConsoleCustomMemalign, cellConsoleInitWithAllocator, cellConsoleFree
 */
typedef void (*CellConsoleCustomFree)(void *pvAddress);

/**
 * @short Initializes the console, with the system default memory allocator
 *
 * cellConsoleInit() initializes the Control Console.  It sets up some data
 * and initializes the Variable Tracker plug-in.
 * 
 * Any memory allocations performed by the Control Console will call the
 * system default <c>memalign()</c> and <c>free()</c> functions.  For a version that allows
 * overloading of <c>memalign()</c> and <c>free()</c>, please see cellConsoleInitWithAllocator()
 * 
 * After the first successful call to cellConsoleInit() or
 * cellConsoleInitWithAllocator(), subsequent calls are
 * no-ops.
 * 
 * Once the console has been initialized, it should be polled periodically
 * using the cellConsolePoll() function.
 *
 * @return 
 * <c>CELL_CONSOLE_OK</c> &mdash; currently this call always succeeds
 *
 * @see cellConsoleInitWithAllocator, cellConsolePoll, cellConsoleInitialized
 */
unsigned int cellConsoleInit(void);

/**
 * @short Initializes the console with a custom memory allocator.
 *
 * cellConsoleInitWithAllocator() initializes the Control Console.  It sets
 * up some data and initializes the Variable Tracker plug-in.
 * 
 * This call also allows the user to specify custom allocate and free functions,
 * rather than defaulting to <c>memalign()</c> and <c>free()</c>
 * For a version without a custom allocator, please see cellConsoleInit()
 * 
 * After the first successful call to cellConsoleInit() or
 * cellConsoleInitWithAllocator(), subsequent calls are
 * no-ops.
 * 
 * Once the console has been initialized, it should be polled periodically
 * using the cellConsolePoll() function.
 *
 * @param memalignFunction The function used to allocate memory
 * @param freeFunction The function used to free memory
 *
 * @return  
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success.<li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter was specified.</li>
 * </ul>
 *
 * @see cellConsolePoll, cellConsoleInitialized
 */
unsigned int cellConsoleInitWithAllocator(CellConsoleCustomMemalign memalignFunction, CellConsoleCustomFree freeFunction);

/**
 * @short Invokes recurring console tasks and polls the console I/O
 *        connections.
 *
 * cellConsolePoll() is responsible for executing recurring tasks and for
 * checking for and processing input from all active connections.  The basic
 * operation of cellConsolePoll() is as follows:
 * 
 * <ol>
 * <li>&nbsp;For each once-per-poll CellConsoleRecurringTask(), execute task</li>
 * <li>&nbsp;For each non-once-per-poll CellConsoleRecurringTask(), execute task</li>
 * <li>&nbsp;For each connection:<br>
 * &nbsp;&nbsp;a.&nbsp;Execute connection CellConsoleReadLine()<br>
 * &nbsp;&nbsp;b.&nbsp;If data is returned:<br>
 * &nbsp;&nbsp;b.i.&nbsp;&nbsp;&nbsp;If an input processor is awaiting more input, pass input to that processor and go to 3.a. <br>
 * &nbsp;&nbsp;b.ii.&nbsp;&nbsp;Else parse command to find trigger word<br>
 * &nbsp;&nbsp;b.iii.&nbsp;Find an input processor for that trigger word and execute<br>
 * &nbsp;&nbsp;b.iv.&nbsp;&nbsp;If input processor rejects it, go to 3.b.iii else go to 3.a.<br>
 * &nbsp;&nbsp;b.v.&nbsp;&nbsp;&nbsp;It no more input processors left, output "Unrecognized command"<br>
 * &nbsp;&nbsp;b.vi.&nbsp;&nbsp;Go to 3.a.<br>
 * &nbsp;&nbsp;c.&nbsp;If data is not returned by CellConsoleReadLine(), loop to step 3.</li>
 * <li>&nbsp;If console is in "pause" mode, do a micro sleep and go to 2.</li>
 * </ol>
 *
 * @note The placement of the cellConsolePoll() call is important depending on
 *       what functionality you expect from the console.  For example, if you
 *       wish to be able to take screen captures of your frame buffer once it
 *       has been fully drawn (using GL, see cellConsoleScreenShotPluginInit())
 *       then the cellConsolePoll() command should be placed after all draw
 *       calls but before swapping the front and back buffers. 
 * @par
 *       If you wish to use the console to configure the behavior of your
 *       application, either by using the Variable Tracker, or by adding
 *       your own plug-ins/input processors, then you want to place the
 *       cellConsolePoll() call at some point in your code where it makes sense
 *       to reconfigure your program's state. 
 * @par
 *       Note also that if you use the Variable Tracker to configure your
 *       application's parameters, you may want to confirm the validity of the
 *       variables after the cellConsolePoll() call to make sure that the
 *       console user did not enter an invalid value.  An alternative to this
 *       is to make your configuration through plug-ins/input processors that
 *       automatically validate values before setting them.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been initialized</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleRecurringTaskAdd, cellConsoleConnectionAdd
 */
unsigned int cellConsolePoll(void);

/**
 * @short Indicates whether the console has been initialized.
 *
 * cellConsoleInitialized() indicates whether the console has been initialized.
 *
 * @return 
 * <ul>
 * <li>0 if the console has not been initialized</li>
 * <li>non-zero if the console has been initialized</li>
 * </ul>
 *
 * @sa cellConsoleInit
 */
int cellConsoleInitialized(void);

/**
 * @short Allocates memory, possibly using custom allocator
 *
 * This function allocates memory to be used by the Control Console or
 * a Control Console plugin.  If cellConsoleInitWithAllocator() was called,
 * then this function calls the custom memory allocator, otherwise it
 * calls memalign().
 * 
 * The memory allocated should be freed using cellConsoleFree()
 *
 * @param uiAlign The required alignment of the memory.  '1' is equivalent to
 *        a malloc.
 * @param uiSize The number of bytes the Control Console needs.
 *
 * @return A pointer to a buffer of size <c><i>uiSize</i></c>
 *
 * @sa cellConsoleInitWithAllocator, cellConsoleFree
 */
void *cellConsoleMemalign(size_t uiAlign, size_t uiSize);


/**
 * @short Frees memory, possibly using custom allocator
 *
 * This function frees memory allocated by cellConsoleMemalign().  If
 * cellConsoleInitWithAllocator() was called, then this function calls the
 * custom memory allocator, otherwise it calls <c>free()</c>.
 *
 * @param pvAddress The address to free, returned by a call to
 * cellConsoleMemalign()
 *
 * @sa cellConsoleInitWithAllocator, cellConsoleMemalign
 */
void cellConsoleFree(void *pvAddress);

/**
 * @short Recurring task callback
 *
 * This is the type of the callback function used as a recurring task.
 *
 * @param pvPrivateData The private data pointer passed in when the callback function was registered.
 *
 * @sa cellConsoleRecurringTaskAdd, cellConsoleRecurringTaskRemove
 */
typedef void (*CellConsoleRecurringTask)(void *pvPrivateData);

/**
 * @short Adds a recurring task to the console
 *
 * cellConsoleRecurringTaskAdd() adds a recurring task to the console.  This
 * function is called one or more times every time that cellConsolePoll() is
 * called.  Typically, cellConsolePoll() is called once per frame, so this
 * could be used to perform per-frame tasks (though cellConsolePoll() is not
 * required to be called once per frame).
 * 
 * A recurring task also provides a useful opportunity to query for new
 * incoming connections, for example, for a new connection request from a TCP
 * server socket.
 *
 * @param pvPrivateData An optional pointer to private data that will be
 *        passed into the recurring task whenever it is called.
 * @param task A pointer to the recurring task callback function.
 * @param iOncePerPoll Indicates whether this task should be invoked only
 *        once per cellConsolePoll() call, or repeatedly in the case where the
 *        console is used to "pause" the system.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success<li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been initialized</li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if memory for the task was not available </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter was specified.</li>
 * </ul>
 *
 * @sa cellConsolePoll, cellConsoleRecurringTaskRemove, CellConsoleRecurringTask
 */
unsigned int cellConsoleRecurringTaskAdd(void *pvPrivateData,
										 CellConsoleRecurringTask task,
										 int iOncePerPoll);
/**
 * @short Removes a recurring task from the console.
 *
 * cellConsoleRecurringTaskRemove() removes a recurring task from the console.
 * It will no longer be invoked during a cellConsolePoll() call.
 *
 * @param pvPrivateData The private data pointer associated with the task.
 * @param task The task to remove
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a the task could not be found</li>
 * </ul>
 *
 * @sa cellConsolePoll, cellConsoleRecurringTaskAdd
 */
unsigned int cellConsoleRecurringTaskRemove(void *pvPrivateData,
											CellConsoleRecurringTask task);

/**
 * @short Return type from an input processor
 *
 * This enum defines the allowed return values from an input processor.
 */
typedef enum CellConsoleInputProcessorResult {
	/// Input processed successfully
	CELL_CONSOLE_INPUT_PROCESSED,  ///< Input processed successfully and next line should also go to this handler 
	CELL_CONSOLE_NEED_NEXT_INPUT,  ///< Further input is required from the console
	CELL_CONSOLE_INPUT_NOT_PROCESSED,  ///< Input does not belong to this handler, it should be parsed and processed by the system. 
} CellConsoleInputProcessorResult;

/**
 * @short Input processor/plugin callback
 *
 * This is type of callback function is used by an input processor.
 * An input processor will get invoked whenever a command is passed to the
 * console that matches the keyword specified when the input processor was
 * registered with the cellConsoleInputProcessorAdd() function.
 * 
 * The input processor should parse and act upon the command, and return one
 * of three values: 
 * <ul>
 * <li><c>CELL_CONSOLE_INPUT_PROCESSED</c> if the input has been successfully
 *      consumed.</li>
 * <li><c>CELL_CONSOLE_NEED_NEXT_INPUT</c> if the processor requires subsequent input
 *      from the console.  This subsequent input need not be begin with the
 *      input processor's trigger keyword.</li>
 * <li><c>CELL_CONSOLE_INPUT_NOT_PROCESSED</c> if the keyword was correct, however
 *      the input does not belong to this processor.</li>
 * </ul>
 *
 * @param uiConnection The connection through which the input was obtained.
 *        This can be used to allow per-connection behavior of input
 *        processors.  This is also used to send responses back using
 *        cellConsolePrintf().
 * @param pcInput The input string, including the keyword.
 * @param pvPrivateData The private data pointer passed in when the callback
 *        function was registered.
 * @param iContinuation Indicates that this input is a follow-up line on a
 *        multi-line input.  This is non-zero for subsequent calls to this
 *        callback after it has returned <c>CELL_CONSOLE_NEED_NEXT_INPUT</c>
 * @return 
 * The input processor returns a value depending on whether it has
 *         successfully consumed the input data, needs more input (for multi-
 *         line commands), or whether the input does not belong to this
 *         processor.  If the input processor returns
 * <c>CELL_CONSOLE_INPUT_NOT_PROCESSED</c>, then the Control Console will
 *         look for additional processors associated with this input keyword.
 *
 * @sa cellConsoleRecurringTaskAdd, cellConsoleRecurringTaskRemove
 */
typedef CellConsoleInputProcessorResult (*CellConsoleInputProcessor)
	 (unsigned int uiConnection, const char *pcInput,
	  void *pvPrivateData, int iContinuation);

/**
 * @short Adds an input processor to the console.
 *
 * cellConsoleInputProcessorAdd() adds an input processor to the console.  This
 * processor is bound to a trigger keyword, and will be invoked whenever a
 * user (or application) inputs that keyword through the console. 
 * 
 * This is the Control Console's primary method for extending its behavior.  By
 * default, the Control Console includes a number of such input processors,
 * such as:
 * <ul>
 * <li>The <c>pause</c> and <c>resume</c> commands.</li>
 * <li>The <c>print</c> and <c>set</c> Variable Tracker commands.</li>
 * <li>The <c>help</c> command</li>
 * </ul>
 *
 * @note The following defines the convention to describe command
 *       arguments:
 * @par
 *       &lt;type required_name&gt; &lt;type required_name&gt; ... 
 * @par
 *       or 
 * @par
 *       &lt;type required_name&gt; &lt;type required_name&gt; 
 *       ... [type optional_name] ... 
 * @par
 *       <ul>
 *       <li><i>required_name</i> and <i>optional_name</i> are ornamental&mdash;they are used to
 *       make user friendly prompts in GUI applications (e.g. "filename")</li>
 *       <li>Type is one of <c>bool</c>, <c>char</c>, <c>int</c>, <c>uint</c>, <c>float</c>, <c>string</c></li>
 *       <li>For ambiguous/varying types, use <c>string</c> type</li>
 *		 </ul>
 *
 * @param pcKeyword The console keyword to trigger this input processor.  Only
 *        the first 64 bytes are used.
 * @param pcHelpDescription A brief description of what the command does.
 *        Only the first 128 bytes are used.
 * @param pcArguments The arguments this command expects.  See the Notes section for
 *        the argument convention.
 * @param pvPrivateData An optional pointer to private data that will be
 *        passed into the input processor whenever it is called.
 * @param processor A pointer to the input processor callback function.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *         initialized</li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if memory for the processor was not
           available</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter was specified.</li>
 * </ul>
 *
 * @sa CellConsoleInputProcessor, cellConsoleInputProcessorRemove
 */
unsigned int cellConsoleInputProcessorAdd(const char *pcKeyword,
										  const char *pcHelpDescription,
										  const char *pcArguments,
										  void *pvPrivateData,
										  CellConsoleInputProcessor processor);

/**
 * @short Removes an input processor/plug-in from the console.
 *
 * cellConsoleInputProcessorRemove() removes an input processor from the console.
 * It will no longer be invoked when the keyword is entered at the console.
 *
 * @param pvPrivateData The private data pointer passed in when the connection
 *        was registered.
 *        This private data pointer will typically contain some identifying
 *        information about the actual connection, if applicable.
 * @param pcKeyword Keyword associated with the processor/plug-in
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *     initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a the task could not be found</li>
 * </ul>
 *
 * @sa cellConsolePoll, cellConsoleInputProcessorAdd
 */
unsigned int cellConsoleInputProcessorRemove(void *pvPrivateData,
											 const char *pcKeyword);

/**
 * @short Connection write callback
 *
 * This is the type of the callback function to output data from the console
 * to a connection. 
 * 
 * It is possible for a connection to buffer its output; the CellConsoleFlush()
 * callback will be used when the buffered output must be flushed.
 *
 * @note The buffer that contains the output data is no longer valid once this
 *       function returns.  The connection may need to buffer the output
 *       separately if it cannot be transmitted immediately.
 *
 * @param pvPrivateData The private data pointer passed in when the connection
 *        was registered.
 *        This private data pointer will typically contain some identifying
 *        information about the actual connection, if applicable.
 * @param pcBuffer The data to write to the connection
 * @param iLength The length of the data to write
 *
 * @sa cellConsoleConnectionAdd, cellConsoleConnectionRemove,
 *     CellConsoleReadLine, CellConsoleFlush
 */
typedef void (*CellConsoleWrite)(void *pvPrivateData, const char *pcBuffer,
								 int iLength);

/**
 * @short Connection read line callback
 *
 * The type of the callback function to read a single line of input
 * from a connection to the console.
 *
 * @note 
 *		 <ul>
 *		 <li>The read callback is responsible for allocating a buffer for
 *       this data.  The buffer can be reused once another call to
 *       CellConsoleReadLine() is made.</li>
 *       <li>The read callback is responsible for removing the trailing <c>CR</c> or
 *       <c>CR/LF</c> that terminates the line, if any.</li>
 *       <li>Data should be buffered until a complete line of input is
 *       available.</li>
 *		 </ul>
 *
 * @param pvPrivateData The private data pointer passed in when the connection
 *        was registered.
 *        This private data pointer will typically contain some identifying
 *        information about the actual connection, if applicable.
 * @param ppcBuffer A pointer to a string containing the null-terminated line
 *        of input data should be placed at this location.
 * @return 
 * <ul>
 * <li>0 if there is no data to be processed </li>
 * <li>non-zero if there is a line of input to be processed</li>
 * </ul>
 *
 * @sa cellConsoleConnectionAdd, cellConsoleConnectionRemove,
 *     CellConsoleWrite, CellConsoleFlush
 */
typedef int (*CellConsoleReadLine)(void *pvPrivateData, char **ppcBuffer);

/**
 * @short Connection flush callback
 *
 * The type of the callback function to flush buffered output from a
 * connection. 
 * 
 * For connections that buffer their output, this callback should initiate
 * transmission of any buffered output.
 * 
 * For connections that do not buffer their output, this can be a no-op.
 *
 * @param pvPrivateData The private data pointer passed in when the connection
 *        was registered.
 *        This private data pointer will typically contain some identifying
 *        information about the actual connection, if applicable.
 *
 * @sa cellConsoleConnectionAdd, cellConsoleConnectionRemove
 *     CellConsoleWrite, CellConsoleReadLine
 */
typedef void (*CellConsoleFlush)(void *pvPrivateData);

/**
 * @short Adds an I/O connection to the console
 *
 * cellConsoleConnectionAdd() is used to add a newly established I/O connection
 * to the Control Console.  Connections can be any communication channels that
 * afford a read line and a write mechanism, such as a network connection, a
 * DECI3 connection, or even a terminal.
 * 
 * Though typically users are not required to add custom connections (the built
 * in console communication mechanisms are sufficient), this API is exposed to
 * allow users alternate ways to communicate with the console.  For example,
 * if a game engine already has an existing command line system, a console
 * connection can be created to receive input through that command line and to
 * redirect output to the appropriate output stream (e.g. the frame
 * buffer).
 * 
 * A console connection needs to implement three functions:
 * <ul>
 * <li>CellConsoleWrite() is a function to allow output from the console
 * through the connection.  The cellConsolePrintf() function, for example, uses
 * this write function for output.</li>
 * <li>CellConsoleReadLine() is a function to allow input from the connection
 * to the console.</li>
 * <li>CellConsoleFlush() is a function that causes buffered output to be
 * transmitted.</li>
 * </ul>
 * 
 * CellConsoleReadLine() is repeatedly called by cellConsolePoll() as long as it returns data.
 *
 * @param pvPrivateData An optional pointer to private data that will be
 *        passed into the callback functions.
 *        This private data pointer will typically contain some identifying
 *        information about the actual connection, if applicable.
 * @param write A pointer to the write callback
 * @param readLine A pointer to the read line callback
 * @param flush A pointer to the flush callback
 * @return 
 * <ul>
 * <li>On success, the number of connection added</li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE </c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified</li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if <c>CELL_CONSOLE_MAX_CONNECTIONS</c> connections have already been established.</li>
 * </ul>
 *
 * @sa cellConsoleConnectionRemove, CellConsoleWrite, CellConsoleReadLine,
 *     CellConsoleFlush
 */
unsigned int cellConsoleConnectionAdd(void * pvPrivateData, 
									  CellConsoleWrite write,
									  CellConsoleReadLine readLine,
									  CellConsoleFlush flush);

/**
 * @short Removes an I/O connection
 * 
 * cellConsoleConnectionRemove() is typically used when a connection has been
 * terminated or disconnected to prevent the console from polling that
 * connection during subsequent cellConsolePoll() calls.
 *
 * @param uiConnectionNumber The number of the connection to remove, returned
 *        by a call to cellConsoleConnectionAdd()
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified</li>
 * </ul>
 *
 * @sa cellConsoleConnectionAdd()
 */
unsigned int cellConsoleConnectionRemove(unsigned int uiConnectionNumber);
  
/**
 * @short Sends output through the console.
 *
 * cellConsolePrintf() sends output through the console to a given connection.
 * Aside from the connection parameter, cellConsolePrintf() is modeled to
 * behave much like a printf() call.
 *
 * @note cellConsolePrintf() uses an internal 4096 byte buffer and the
 *       <c>vsnprintf</c> call.  Because of this, it is limited to a maximum string
 *       size of 4095 bytes.
 *
 * @param uiConnection The connection number to use
 * @param pcFormat A printf-style format string
 * @param ... Arguments for the printf.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified</li>
 * </ul>
 *
 * @sa cellConsoleFlush
 */
unsigned int cellConsolePrintf(unsigned int uiConnection,
							   const char *pcFormat, ...);

/**
 * @short Flushes the output of a connection.
 *
 * cellConsoleFlush() flushes the output of a console connection.  For
 * connections that have buffered outputs, cellConsoleFlush() causes all
 * uncommitted output to be sent.  For connections that do not have buffered
 * outputs this function is a no-op.
 *
 * @param uiConnection Connection to flush
 *
 * @return
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified</li>
 * </ul>
 *
 * @sa cellConsolePrintf
 */
unsigned int cellConsoleFlush(unsigned int uiConnection);

/**
 * @short Gets the compatibility mode requested by a connection.
 *
 * cellConsoleGetCompatibility() returns the compatibility version for a
 * given connection.  When a client connects to the console, it can use
 * the _COMPATIBILITY command to request a certain level of compatibility.
 * By default, applications are set to 0x10000000.
 * By specifying compatibility at the connection level, different versions
 * of front-end applications can be mixed and matched.
 *
 * Compatibility versions are hex numbers broken up into three components:
 *
 * The hexadecimal digits are: <c>0xvvvruuuu</c>, where
 * <ul>
 * <li><c>vvv</c>: Corresponds to the SDK version (e.g. 160)</li>
 * <li><c>r  </c>: Corresponds to a revision, should compatibility need to change in minor
 *                revisions of the same SDK.</li>
 * <li><c>uuuu</c>: Can be used by developers for their own compatibility control.</li>
 * </ul>
 *
 * Applications can test that the compatibility is greater than or equal
 * to some initial required version.
 *
 * @param uiConnection Connection to get compatibility
 *
 * @return
 * <ul>
 * <li><c>0x00000000</c> if the connection is not connected, the console is
 *     not initialized, or a bad connection number is specified</li>
 * <li><c>0x10000000</c> if the console is specified </li>
 * <li><c>0xvvvruuuu</c> where vvvr corresponds to a Cell SDK version/
 *       revision, and uuuu are user defined.  See the description for details.</li>
 * </ul>
 */
unsigned int cellConsoleGetCompatibility(unsigned int uiConnection);


/**
 * @short Passes a batch of commands to the console
 *
 * cellConsoleProcessBatch() passes a batch of commands to the console as
 * if it was entered interactively through a connection. 
 * 
 * This function is useful to allow scripting and other systems to interact
 * with the console functionality.  For instance, if a scripting system wants
 * to invoke some of the console's Variable Tracker functionality, it can
 * invoke 
 * 
 * <c>cellConsoleProcessBatch(uiConn, "set myvar true", 1);</c> 
 * 
 * Note that connections can be bound to a particular input processor/plug-in
 * if that plug-in requests multiple lines of input (see
 * CellConsoleInputProcessorResult() or cellConsoleInputProcessorAdd()).
 * To allow such an input processor to use this call (for example, an 
 * interactive python scripting console plugin), the input processor can be
 * temporarily "suspended" pending the execution of the command batch.
 * 
 * The input processor is restored once the batch has executed.
 * 
 * If the input processor is not suspended, then the command batch would
 * be fed back into the input processor.
 *
 * @param uiConnection The connection number to use for this command batch.
 *        Typically this call is made by an input processor that is invoked
 *        with a given connection.
 * @param pcBatch The batch of commands to pass into the console.
 * @param iSuspendProcessor If a multi-line input processor is executing,
 *        setting this to non-zero temporarily suspends the processor pending
 *        the execution of the batch.
 *
 * @return 
 * <ul>
 * <li>On success, the number of command lines executed</li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been 
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified</li>
 * </ul>
 */
unsigned int cellConsoleProcessBatch(unsigned int uiConnection,
									 const char *pcBatch,
									 int iSuspendProcessor);

#include "plugins/variable_tracker.h"

#ifdef __cplusplus
}
#endif

#endif
