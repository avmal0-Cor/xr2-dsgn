/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONSOLE_PERFORMANCE_DATA_H
#define __CONSOLE_PERFORMANCE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	typedef unsigned int uint32_t;
	typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif

#include <cell/console/console.h>

/**
* @short Performance data data types
*
* Data types for console "perf_data" command
*/
	typedef enum CellConsolePerfDataType {
	CELL_CONSOLE_PERF_TYPE_DOUBLE,     ///< Double
	CELL_CONSOLE_PERF_TYPE_UINT32,	   ///< 32 bit unsigned integer
	CELL_CONSOLE_PERF_TYPE_UINT64,     ///< 64 bit unsigned long long
	} CellConsolePerfDataType;

/**
 * @short Creates a data set, assigning names and a data type.
 *
 * cellConsolePerfDataSetColumns() initializes a data set for values that can
 * be inspected from the console.  The most up-to-date "row" of data can be
 * queried using the console "perf_data" command.  For each row, a number of
 * "columns" of data are available.  This function defines the number of 
 * columns and their names.  In addition, the data type used by these columns
 * is defined.
 * 
 * For data sets that have a large number of columns, the columns can be
 * categorized by mapping them to different groups.  The groups are created
 * using cellConsolePerfDataSetGroups() or
 * cellConsolePerfDataSetGroupList(), and columns are mapped to groups
 * using cellConsolePerfDataSetGroupMap() or
 * cellConsolePerfDataSetGroupMapList().
 * 
 * Data is updated using one of the various cellConsolePerfDataSetValue*
 * functions, and once a complete row of data is available, 
 * cellConsolePerfDataIncrementDataCounter() should be called.
 * 
 * This data set can be destroyed by the cellConsolePerfDataFree() function.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 * @par
 *       Typically, this data will be some sort of timing information,
 *       but it is in no way restricted to just timing.  An application, for
 *       instance, can use the cellConsolePerf API to make available input from
 *       an analog or motion sensing controller which can then be graphed in
 *       real time by a PC GUI application attached to the console.  Try
 *       the PA suite or refer to the control_console/perf_gui sample for examples
 *       of such real-time graphing applications.
 *
 * @param pcDataSetName The name of this data set
 * @param type The data type that will be used for the column data.
 * @param uiColumnCount The number of columns per row.
 * @param ... The remaining arguments are the column names, one for each
 *        of <i>uiColumnCount</i>
 *
 * @return 
 * <ul>
 * <li>On success, the identifier of this data set. 
 *         Use this identifier with subsequent cellConsolePerfData() calls.</li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if there is insufficient memory for the data set</li>
 * </ul>
 *
 * @sa cellConsolePerfDataSetColumnList, cellConsolePerfDataFree
 */
	unsigned int cellConsolePerfDataSetColumns(const char *pcDataSetName,
											   CellConsolePerfDataType type,
											   unsigned int uiColumnCount,
											   ...);


/**
 * @short Creates a data set, assigning names and a data type.
 *
 * cellConsolePerfDataSetColumnList() initializes a data set for values that
 * can be inspected from the console.  The most up-to-date "row" of data can be
 * queried using the console "perf_data" command.  For each row, a number of
 * "columns" of data are available.  This function defines the number of 
 * columns and their names.  In addition, the data type used by these columns
 * is defined.
 * 
 * For data sets that have a large number of columns, the columns can be
 * categorized by mapping them to different groups.  The groups are created
 * using cellConsolePerfDataSetGroups() or
 * cellConsolePerfDataSetGroupList(), and columns are mapped to groups
 * using cellConsolePerfDataSetGroupMap() or
 * cellConsolePerfDataSetGroupMapList().
 * 
 * Data is updated using one of the various cellConsolePerfDataSetValue*
 * functions, and once a complete row of data is available, 
 * cellConsolePerfDataIncrementDataCounter() should be called.
 * 
 * This data set can be destroyed by the cellConsolePerfDataFree() function.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       Typically, this data will be some sort of timing information,
 *       but it is in no way restricted to just timing.  An application, for
 *       instance can use the cellConsolePerf API to make available input from
 *       an analog or motion sensing controller which can then be graphed in
 *       real time by a PC GUI application attached to the console.  Try
 *       the PA suite or see the control_console/perf_gui sample for examples
 *       of such real-time graphing applications.
 *
 * @param pcDataSetName The name of this data set
 * @param type The data type that will be used for the column data.
 * @param uiColumnCount The number of columns per row.
 * @param ppcNames An array of column names, one for each of uiColumnCount
 *
 * @return 
 * <ul>
 * <li>On success, the identifier of this data set.
 *         Use this identifier with subsequent cellConsolePerfData calls.</li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c> </li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if there is insufficient memory for the data set</li>
 * </ul>
 *
 * @sa cellConsolePerfDataSetColumns, cellConsolePerfDataFree
 */
	unsigned int cellConsolePerfDataSetColumnList(const char *pcDataSetName,
												  CellConsolePerfDataType type,
												  unsigned int uiColumnCount,
												  const char **ppcNames);

/**
 * @short Sets a list of group names that will be assigned to the columns in
 *        a data set.
 *
 * For data sets that have a large number of columns, the columns can be
 * categorized by mapping them to different groups.  This function is used to
 * create the list of group names.  Then cellConsolePerfDataSetGroupMap()
 * or cellConsolePerfDataSetGroupMapList() can be used to map groups
 * to columns.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param uiGroupCount The number of groups in the group list
 * @param ... The remaining arguments are the group names, one for each
 *        of uiGroupCount
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if there is insufficient memory for the groups</li>
 * </ul>
 *
 * @sa  cellConsolePerfDataSetGroupList, cellConsolePerfDataSetGroupMap,
 *      cellConsolePerfDataSetGroupMapList
 */
	unsigned int cellConsolePerfDataSetGroups(unsigned int uiDataSet,
											  unsigned int uiGroupCount, ...);

/**
 * @short Sets a list of group names that will be assigned to the columns in
 *        a data set.
 *
 * For data sets that have a large number of columns, the columns can be
 * categorized by mapping them to different groups.  This function is used to
 * create the list of group names.  Then cellConsolePerfDataSetGroupMap()
 * or cellConsolePerfDataSetGroupMapList() can be used to map groups
 * to columns.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param uiGroupCount The number of groups in the group list
 * @param ppcNames An array of strings that are the group names, one for each
 *        of <c><i>uiGroupCount</i></c>
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if there is insufficient memory for the groups</li>
 * </ul>
 *
 * @sa  cellConsolePerfDataSetGroups, cellConsolePerfDataSetGroupMap,
 *      cellConsolePerfDataSetGroupMapList
 */
	unsigned int cellConsolePerfDataSetGroupList(unsigned int uiDataSet,
												 unsigned int uiGroupCount,
												 const char **ppcNames);

/**
 * @short Maps data set columns to groups.
 *
 * Maps the columns in a data set (initialized using
 * cellConsolePerfDataSetColumns() or cellConsolePerfDataSetColumnList())
 * to a group list (initialized using cellConsolePerfDataSetGroups() or
 * cellConsolePerfDataSetGroupList()).  
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param ... The column-to-group mapping.  There need to be as many arguments
 *        as there are columns, each is an unsigned int that contains a group
 *        number from 0 to <c><i>uiGroupCount</i></c>-1 (as specified in
 *        cellConsolePerfDataSetGroups() or cellConsolePerfDataSetGroupList())
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * </ul>
 *
 * @sa  cellConsolePerfDataSetGroupMapList
 */
	unsigned int cellConsolePerfDataSetGroupMap(unsigned int uiDataSet, ...);

/**
 * @short Maps data set columns to groups.
 *
 * Maps the columns in a data set (initialized using
 * cellConsolePerfDataSetColumns() or cellConsolePerfDataSetColumnList())
 * to a group list (initialized using cellConsolePerfDataSetGroups() or
 * cellConsolePerfDataSetGroupList()).  
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param puiGroupMap An array containing the column-to-group mapping.  There
 *        need to be as many entries as there are columns, each is a group
 *        number from 0 to <c><i>uiGroupCount</i></c>-1 (as specified in
 *        cellConsolePerfDataSetGroups() or cellConsolePerfDataSetGroupList())
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * </ul>
 *
 * @sa  cellConsolePerfDataSetGroupMap
 */
	unsigned int cellConsolePerfDataSetGroupMapList(unsigned int uiDataSet,
											unsigned int *puiGroupMap);

/**
 * @short Removes a data set from the system.
 * 
 * Call this when you are done using a data set to release the resources
 * associated with it.  The <c><i>uiDataSet</i></c> number is no longer valid if this call
 * succeeds.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 *
 * @sa cellConsolePerfDataSetColumns, cellConsolePerfDataSetColumnList
 */
	unsigned int cellConsolePerfDataFree(unsigned int uiDataSet);

/**
 * @short Sets an entire row of data in a data set
 * 
 * Set an entire row of data in a data set.
 *
 * @note Because data sets are double buffered, the new data will not be
 *       available through the console until
 *       cellConsolePerfDataIncrementDataCounter() is called.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param ... The data values.  The type should match the type specified
 *        when the data set was created, and there need to be as many arguments
 *        as there are columns in the data set.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * </ul>
 *
 * @sa cellConsolePerfDataSetValueList, cellConsolePerfDataSetValueByName,
 *     cellConsolePerfDataSetValueByIndex, cellConsolePerfDataSetColumns,
 *     cellConsolePerfDataSetColumnList,
 *     cellConsolePerfDataIncrementDataCounter
 */
	unsigned int cellConsolePerfDataSetValues(unsigned int uiDataSet, ...);

/**
 * @short Sets an entire row of data in a data set
 * 
 * Set an entire row of data in a data set.
 *
 * @note Because data sets are double buffered, the new data will not be
 *       available through the console until
 *       cellConsolePerfDataIncrementDataCounter() is called.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param pvValues An array of data values.  The type should match the type
 *        specified when the data set was created, and there need to be as many
 *        entries as there are columns in the data set.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * </ul>
 *
 * @sa cellConsolePerfDataSetValues, cellConsolePerfDataSetValueByName,
 *     cellConsolePerfDataSetValueByIndex, cellConsolePerfDataSetColumns,
 *     cellConsolePerfDataSetColumnList,
 *     cellConsolePerfDataIncrementDataCounter
 */
	unsigned int cellConsolePerfDataSetValueList(unsigned int uiDataSet,
										 void *pvValues);

/**
 * @short Sets a single column of data in a data set by name.
 * 
 * Sets an individual column of data in a data set by name.
 *
 * @note Because data sets are double buffered, the new data will not be
 *       available through the console until
 *       cellConsolePerfDataIncrementDataCounter() is called.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param pcName The string name of the column passed in when the data set
 *        was created.
 * @param ... The value to assign to the column. The type should match the type
 *        specified when the data set was created.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 *
 * @sa cellConsolePerfDataSetValueList, cellConsolePerfDataSetValues,
 *     cellConsolePerfDataSetValueByIndex, cellConsolePerfDataSetColumns,
 *     cellConsolePerfDataSetColumnList,
 *     cellConsolePerfDataIncrementDataCounter
 */
	unsigned int cellConsolePerfDataSetValueByName(unsigned int uiDataSet,
												   const char *pcName, ...);

/**
 * @short Sets a single column of data in a data set by index.
 * 
 * Sets an individual column of data in a data set by index.
 *
 * @note Because data sets are double buffered, the new data will not be
 *       available through the console until
 *       cellConsolePerfDataIncrementDataCounter() is called.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 * @param uiIndex The index of the column, from 0 to <c><i>uiColumnCount</i></c>-1
 * @param ... The value to assign to the column. The type should match the type
 *        specified when the data set was created.
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 * </ul>
 *
 * @sa cellConsolePerfDataSetValueList, cellConsolePerfDataSetValues,
 *     cellConsolePerfDataSetValueByName, cellConsolePerfDataSetColumns,
 *     cellConsolePerfDataSetColumnList,
 *     cellConsolePerfDataIncrementDataCounter
 */
	unsigned int cellConsolePerfDataSetValueByIndex(unsigned int uiDataSet, 
											unsigned int uiIndex, ...);

/**
 * @short Makes the latest data set values available in the console.
 *
 * cellConsolePerfDataIncrementDataCounter() increments the data row counter on
 * the data set (to indicate a new set of values is ready) and "swaps" data set
 * values so the new values are reported through the console.  This must be
 *  called after updating the data in order to make it available.
 *
 * @param uiDataSet The data set returned by cellConsolePerfDataSetColumns()
 *        or cellConsolePerfDataSetColumnList()
 *
 * @return 
 * <ul>
 * <li><c>CELL_CONSOLE_OK</c> on success</li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized</li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad argument was passed in </li>
 </ul>
 *
 * @sa cellConsolePerfDataSetValueList, cellConsolePerfDataSetValues,
 *     cellConsolePerfDataSetValueByName, cellConsolePerfDataSetValueByIndex
 */
	unsigned int cellConsolePerfDataIncrementDataCounter(unsigned int uiDataSet);


#ifdef __cplusplus
}
#endif

#endif
