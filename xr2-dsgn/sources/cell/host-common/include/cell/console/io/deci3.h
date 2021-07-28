/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONTROL_CONSOLE_DECI3_H
#define __CONTROL_CONSOLE_DECI3_H

#include <cell/console/console.h>

#ifdef __cplusplus
extern "C" {
#endif

/// The maximum number of simultaneous connections through DECI3
#define CELL_CONSOLE_DECI3_MAX_CONNECTIONS 12

/// The DECI3 protocol number used by the Control Console.  The port numbers
/// will be 0..<c>CELL_CONSOLE_DECI3_MAX_CONNECTIONS</c>-1
/// Console client applications should try to connect to each port in order,
/// starting from 0 up to <c>CELL_CONSOLE_DECI3_MAX_CONNECTIONS</c>-1.  If they
/// all fail, then there are no remaining available connections.
#define CELL_CONSOLE_DECI3_PROTOCOL CELL_CONSOLE_DEFAULT_PORT

/// The size, in bytes, of the largest DECI3 payload that should be sent
/// by either the user application or the console.  This does not count
/// DECI3 headers.
#define CELL_CONSOLE_DECI3_MAX_MESSAGE_SIZE 16384

#ifdef __PPU__
/// DECI3 is only supported on the PPU.
/// This header is provided to expose the above macros on other platforms so
/// that they may correctly interact with the target.

/**
 * @short Initializes the console DECI3 server
 *
 * cellConsoleDeci3ServerInit() binds the console to a DECI3 protocol number
 * <c>CELL_CONSOLE_DECI3_PROTOCOL</c>.
 *
 * @note 
 * Calling this function after it has succeeded once is a no-op
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_IO_ERROR</c> on other failure setting up the server</li>
 * </ul>
 *
 * @sa cellConsoleInit
 */
	unsigned int cellConsoleDeci3ServerInit(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
