/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONSOLE_SCREEN_SHOT_H
#define __CONSOLE_SCREEN_SHOT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cell/console/console.h>

/**
 * @short Initializes the GL Screenshot console command.
 *
 * cellConsoleScreenShotPluginInit() registers the Screenshot console
 * command with the Control Console.  Users can capture the contents of
 * the frame buffer contents at the time when cellConsolePoll() is called.
 * In order to take a complete and final screen shot, cellConsolePoll()
 * should be called after all the rendering has been completed but before
 * the front and back buffers have been swapped.  Interim frame buffer
 * screen shots can be taken by moving the cellConsolePoll() call earlier
 * in the rendering process.
 * 
 * Currently, this function only supports GL (e.g. PSGL or OpenGL), and
 * outputs a .tga file.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. 
 * @par
 *       After the first successful call, subsequent calls are a no-op.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been initialized</li>
 * </ul>
 *
 * @sa cellConsoleInit
 */
	int cellConsoleScreenShotPluginInit(void);


/**
 * @short Takes a screen shot to a .tga file
 *
 * This function is a convenience function that takes a screen shot of
 * the current GL back buffer contents and writes it out to a .tga file.
 * This is the same function called by the console's Screenshot function.
 * 
 * Currently, this function only supports GL (e.g. PSGL or OpenGL), and
 * outputs a .tga file.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *
 * @param pcFile the file to save the screenshot to.
 * @param uiX The left-most X coordinate to save
 * @param uiY The top-most Y coordinate to save
 * @param uiWidth the width of the image to save
 * @param uiHeight the height of the image to save
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was specified </li>
 * <li><c>CELL_CONSOLE_IO_ERROR</c> if there was an I/O failure writing the image</li>
 * </ul>
 */
	int cellConsoleScreenShot(const char *pcFile, unsigned int uiX,
							  unsigned int uiY, unsigned int uiWidth,
							  unsigned int uiHeight);
#ifdef __cplusplus
}
#endif

#endif
