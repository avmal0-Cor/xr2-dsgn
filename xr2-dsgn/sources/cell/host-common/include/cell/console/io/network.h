/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONTROL_CONSOLE_NETWORK_H
#define __CONTROL_CONSOLE_NETWORK_H

#include <cell/console/console.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @short Default network port
 *
 * The default network port.
 */
#define CELL_CONSOLE_DEFAULT_NETWORK_PORT CELL_CONSOLE_DEFAULT_PORT

/**
 * @short Convenience function to perform system-specific network
 *        initialization.
 *
 * cellConsoleNetworkInitialize() is used to initialize the network.
 *
 * It only serves as a convenience to programmers and is by no means required
 * to use the network support in the console. 
 *
 * For a PLAYSTATION&reg;3 build of the Control Console, cellConsoleNetworkInitialize() calls
 * <c>sys_net_initialize_network</c>, cellNetCtlInit(), and then uses <c>libnetctl</c> to
 * wait until an IP address is assigned.
 *
 * For a Windows build of the Control Console, cellConsoleNetworkInitialize()
 * initializes <c>winsock</c>. 
 *
 * For a Linux build of the Control Console, cellConsoleNetworkInitialize() is
 * a no-op.
 *
 * @return <c>CELL_CONSOLE_OK</c> on success, or <c>CELL_CONSOLE_IO_ERROR</c> on failure
 */
	unsigned int cellConsoleNetworkInitialize(void);

/**
 * @short Initializes the console network server
 *
 * cellConsoleNetworkServerInit() binds the console to a network port and
 * initializes the network server so that console requests can be accepted
 * through the network (using TCP).
 *
 * @note 
 * <ul>
 * <li>Calling this function after it has succeeded once is a no-op.</li>
 *       <li>Calling this function before initializing the network yields
 *       unspecified behavior.  Use cellConsoleNetworkInitialize() for a
 *       convenient way to initialize the network.</li>
 * </ul>
 *
 * @param iPort is the port to listen to. Valid values are:<br>
 *           1-65535=a specific port number<br>
 *           0 = bind to port 0, the OS will decide<br>
 *           <0 = bind to the default port number<br>
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad port was specified</li>
 * <li><c>CELL_CONSOLE_IO_ERROR</c> on other failure setting up the server</li>
 * </ul>
 *
 * @sa cellConsoleInit
 */
	unsigned int cellConsoleNetworkServerInit(int iPort);


#ifdef __cplusplus
}
#endif

#endif
