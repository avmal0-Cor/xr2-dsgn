/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CONTROL_CONSOLE_VARIABLE_TRACKER_H
#define __CONTROL_CONSOLE_VARIABLE_TRACKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cell/console/console.h>

#ifndef __cplusplus

/**
 * @short Boolean data type
 *
 * Boolean data type
 */
typedef unsigned char CellConsoleBoolean;
#else

/**
 * @short Boolean data type
 *
 * Boolean data type
 */
typedef bool CellConsoleBoolean;

#endif

#ifndef _WIN32
#include <stdint.h>
#else
	typedef signed __int8 int8_t;
	typedef unsigned __int8 uint8_t;
	typedef signed __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef signed __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef signed __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#endif

/**
 * @short Maximum number of variables 
 *
 * Maximum number of variables allowed by the Variable Tracker.
 */
#define CELL_CONSOLE_MAX_VARIABLES 1024


/**
 * @short Maximum number of groups
 *
 * Maximum number of groups allowed by the Variable Tracker.
 */
#define CELL_CONSOLE_MAX_GROUPS 128

/**
* @short Maximum length of variable names
*
* The maximum string length of variable names, including the terminating '\0'.
*/
#define CELL_CONSOLE_MAX_VARIABLE_NAME_LENGTH 128

/**
* @short Maximum length of variable descriptions
*
* The maximum string length of variable descriptions, including the terminating '\0'.
*/
#define CELL_CONSOLE_MAX_VARIABLE_DESCRIPTION_LENGTH 512

/**
* @short Maximum length of variable group names
*
* The maximum string length of variable group names, including the terminating '\0'.
*/
#define CELL_CONSOLE_MAX_VARIABLE_GROUP_NAME_LENGTH 128

/**
* @short Maximum length of variable namespaces
*
* The maximum string length of variable namespaces, including the terminating '\0'.
*/
#define CELL_CONSOLE_MAX_VARIABLE_NAMESPACE_LENGTH 128

/**
 * @short Data type of a Control Console variable.
 *
 * This type is used to help validation callbacks determine the type of the data to validate.
 *
 * @sa CellConsoleVariableValidate
 */
typedef enum CellConsoleVariableTypes {
	CELL_CONSOLE_VARIABLE_INT8,		/**< INT8 variable type. */
	CELL_CONSOLE_VARIABLE_UINT8,	/**< UINT8 variable type. */
	CELL_CONSOLE_VARIABLE_INT16,	/**< INT16 variable type. */
	CELL_CONSOLE_VARIABLE_UINT16,	/**< UINT16 variable type. */
	CELL_CONSOLE_VARIABLE_INT32,	/**< INT32 variable type. */
	CELL_CONSOLE_VARIABLE_UINT32,	/**< UINT32 variable type. */
	CELL_CONSOLE_VARIABLE_INT64,	/**< INT64 variable type. */
	CELL_CONSOLE_VARIABLE_UINT64,	/**< UINT64 variable type. */
	CELL_CONSOLE_VARIABLE_FLOAT,	/**< Float variable type. */
	CELL_CONSOLE_VARIABLE_DOUBLE,	/**< Double variable type. */
	CELL_CONSOLE_VARIABLE_BOOL,		/**< Boolean variable type. */
	CELL_CONSOLE_VARIABLE_STRING,	/**< String variable type. */
} CellConsoleVariableTypes;

/**
 * @short Data union of a Control Console variable.
 *
 * This type is used to store any one of types supported by the Variable Tracker.
 */
typedef union CellConsoleVariableValueUnion {
	int8_t i8;		/**< INT8 value. */
	uint8_t ui8;	/**< UINT8 value. */
	int16_t i16;	/**< INT16 value. */
	uint16_t ui16;	/**< UINT16 value. */
	int32_t i32;	/**< INT32 value. */
	uint32_t ui32;	/**< UINT32 value. */
	int64_t i64;	/**< INT64 value. */
	uint64_t ui64;	/**< UINT64 value. */
	float f;		/**< Float value. */
	double d;		/**< Double value. */
	CellConsoleBoolean b;	/**< Boolean value. */
	const char *pc;	/**< String value. */
} CellConsoleVariableValueUnion;

/**
 * @short Variable data validation callback.
 *
 * This is the type of the callback function used to validate data entered by the user in the console.
 *
 * @param type The data type of the variable being checked.
 * @param value The value to be checked, entered by the user at the Console.
 * @param pvVariableAddress The pointer to the unmodified variable.
 * @param pvPrivateData The private data pointer passed in when the callback function was registered.
 *
 * @return 
 * <ul>
 * <li>Non-zero for a valid value. </li>
 * <li>Zero for a bad value. </li>
 * </ul>
 *
 * @sa cellConsoleVariableValidateNumeric,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
typedef int (*CellConsoleVariableValidate)(CellConsoleVariableTypes type,
										   CellConsoleVariableValueUnion value,
										   void *pvVariableAddress,
										   void *pvPrivateData);

/**
 * @short Variable validation data to be used with the cellConsoleVariableValidateNumeric() validator.
 *
 * Variable validation data to be used with the cellConsoleVariableValidateNumeric() validator.
 */
typedef struct CellConsoleVariableValidateNumericParam {
	CellConsoleVariableValueUnion m_min;		///< Minimum allowed value
	CellConsoleVariableValueUnion m_max;		///< Maximum allowed value
	CellConsoleVariableValueUnion m_multiple;	///< (Value - m_min) must be a multiple of m_multiple.  Use 0 for values that need not be a multiple of anything.
} CellConsoleVariableValidateNumericParam;

/**
 * @short Variable validation helper for numeric types.
 *
 * This function offers simple validation of numeric types, which may be sufficient for
 * most applications.
 *
 * The pseudo-code of the validation routine, given the value 'val', is:
 *
 * <c>return ((val &gt;= m_min &amp;&amp; val &lt;= m_max) &amp;&amp; (m_multiple==0 || mod(val - m_min, m_multiple)==0))</c>
 *
 * The parameters <c><i>m_min</i></c>, <c><i>m_max</i></c>, and <c><i>m_multiple</i></c> are specified in the CellConsoleVariableValidateNumericParam
 * structure that must be passed in as private data when this callback is registered.
 *
 * @param type The data type of the variable being checked.
 * @param value The value to be checked, entered by the user at the Console.
 * @param pvVariableAddress The pointer to the unmodified variable.
 * @param pvPrivateData The private data pointer to a CellConsoleVariableValidateNumericParam structure.
 *
 * @return 
 * <ul>
 * <li>Non-zero for a value that passes the expression shown in the description. </li>
 * <li>Zero for a value that does not pass the expression, or for a boolean or string value. </li>
 * </ul>
 *
 * @sa CellConsoleVariableValidate, CellConsoleVariableValidateNumericParam,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble
 */
int cellConsoleVariableValidateNumeric(CellConsoleVariableTypes type,
									   CellConsoleVariableValueUnion value,
									   void *pvVariableAddress,
									   void *pvPrivateData);

/**
 * @short Adds a group to the Variable Tracker to organize related values.
 *
 * cellConsoleVariableAddGroup() creates a group (category) into which related
 * variables will be organized.  This allows the user interface (command
 * line or GUI) to display the variables sorted by group.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_GROUPS</c> groups.
 *
 * @param pcName The name of the group that will be displayed to the console
 *        user.  Only the first 127 bytes will be used.
 *
 * @return 
 * <ul>
 * <li>On success, the group number to be passed into the cellConsoleVariableAdd* functions. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of groups has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad group name has been specified.
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroupWithNamespace,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddGroup(const char *pcName);

/**
 * @short Adds a group to the Variable Tracker to organize related values.
 *
 * cellConsoleVariableAddGroupWithNamespace() creates a group (category) into which related
 * variables will be organized.  This allows the user interface (command
 * line or GUI) to display the variables sorted by group.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_GROUPS</c> groups.
 *
 * @param pcName The name of the group that will be displayed to the console
 *        user.  Only the first 127 bytes will be used.
 * @param pcNamespace The namespace to prefix all variables with.  All variables
 *        on the Console will appear as Namespace.VariableName. 
 *         The Namespace.VariableName string will be truncated to fit within 127 bytes.,
 *
 * @return 
 * <ul>
 * <li>On success, the group number to be passed into the cellConsoleVariableAdd* functions. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of groups has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad group name has been specified.
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddGroupWithNamespace(const char *pcName, const char *pcNamespace);


/**
 * @short Removes a group from the Variable Tracker.
 *
 * cellConsoleVariableRemoveGroup() removes a group (category) from the
 * variable tracker.  The group must be empty, i.e. it should not have any
 * variables in it.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       <br>Also, groups cannot be removed if they still have variables that
 *        belong to them.  Please remove all variables in the group first by
 *        calling cellConsoleVariableRemove().
 *       <br> Finally, note that the "default" group (number 0) cannot be
 *        removed.
 *
 * @param uiGroup The group number to be removed.
 *
 * @return 
 * <ul>
 * <li>On success, the number of active groups remaining. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad group number has been specified,
 *     or if there are still variables assigned to the group.
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup,
 *     cellConsoleVariableAddGroupWithNamespace
 */
	unsigned int cellConsoleVariableRemoveGroup(unsigned int uiGroup);

/**
 * @short Registers an 8-bit signed integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddInt8() enables tracking of an 8-bit signed integer stored at
 * a given memory location.  The 8-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To facilitate organizing a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedInt8() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.  Note also that if
 *        a space is used in the name, the variable effectively becomes read only.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pcVariable A pointer to the 8-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddInt8(unsigned int uiGroup,
											 const char *pcName,
											 const char *pcDescription,
											 int8_t *pcVariable);

/**
 * @short Registers an 8-bit unsigned integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddUInt8() enables tracking of an 8-bit unsigned integer stored at
 * a given memory location.  The 8-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedUInt8() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pucVariable A pointer to the 8-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, 
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddUInt8(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  uint8_t *pucVariable);

/**
 * @short Registers a 16-bit signed integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddInt16() enables tracking of a 16-bit signed integer stored at
 * a given memory location.  The 16-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedInt16() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param psVariable A pointer to the 16-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddInt16(unsigned int uiGroup,
											 const char *pcName,
											 const char *pcDescription,
											 int16_t *psVariable);

/**
 * @short Registers a 16-bit unsigned integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddUInt16() enables tracking of a 16-bit unsigned integer stored at
 * a given memory location.  The 16-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedUInt16() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pusVariable A pointer to the 16-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, 
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddUInt16(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  uint16_t *pusVariable);

/**
 * @short Registers a 32-bit signed integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddInt32() enables tracking of a 32-bit signed integer stored at
 * a given memory location.  The 32-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedInt32() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param piVariable A pointer to the 32-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddInt32(unsigned int uiGroup,
											 const char *pcName,
											 const char *pcDescription,
											 int32_t *piVariable);

/**
 * @short Registers a 32-bit unsigned integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddUInt32() enables tracking of a 32-bit unsigned integer stored at
 * a given memory location.  The 32-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedUInt32() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param puiVariable A pointer to the 32-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, 
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddUInt32(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  uint32_t *puiVariable);

/**
 * @short Registers a 64-bit signed integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddInt64() enables tracking of a 64-bit signed integer stored at
 * a given memory location.  The 64-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedInt64() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param plVariable A pointer to the 64-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddInt64(unsigned int uiGroup,
											 const char *pcName,
											 const char *pcDescription,
											 int64_t *plVariable);

/**
 * @short Registers a 64-bit unsigned integer value with the Variable Tracker.
 *
 * cellConsoleVariableAddUInt64() enables tracking of a 64-bit unsigned integer stored at
 * a given memory location.  The 64-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedUInt64() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pulVariable A pointer to the 64-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddUInt64(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  uint64_t *pulVariable);

/**
 * @short Registers a 32-bit floating point value with the Variable Tracker.
 *
 * cellConsoleVariableAddFloat() enables tracking of a 32-bit floating point value stored at
 * a given memory location.  The float value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedFloat() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pfVariable A pointer to the 32-bit float.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddFloat(unsigned int uiGroup,
											 const char *pcName,
											 const char *pcDescription,
											 float *pfVariable);

/**
 * @short Registers a 64-bit floating point (double) value with the Variable Tracker.
 *
 * cellConsoleVariableAddDouble() enables tracking of a 64-bit floating point value stored at
 * a given memory location.  The double value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedDouble() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pdVariable A pointer to the 64-bit double.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat,
 *     cellConsoleVariableAddBool, cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddDouble(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  double *pdVariable);

/**
 * @short Registers a boolean value with the Variable Tracker.
 *
 * cellConsoleVariableAddBool() enables tracking of a boolean value stored at
 * a given memory location.  The boolean can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * For C programs, the bool type is actually an unsigned character that will
 * be assigned values of 0 or 1 by the console.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedBool() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pbVariable A pointer to the boolean (or unsigned char for C).
 *        Please make sure that this pointer is valid until the variable is
 *        removed with cellConsoleVariableRemove().
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified.</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddString
 */
	unsigned int cellConsoleVariableAddBool(unsigned int uiGroup,
											const char *pcName,
											const char *pcDescription,
											CellConsoleBoolean *pbVariable);

/**
 * @short Registers a string value with the Variable Tracker.
 *
 * cellConsoleVariableAddString() enables tracking of a string value stored at
 * a given memory location.  The string can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * This function also specifies the capacity of the buffer -- defining the maximum
 * length of string that can be entered.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddValidatedString() function allows the caller to add
 *       a validation callback to verify the value before they are set through the console.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pcVariable A pointer to the string.
 *        Please make sure that this pointer is valid until the variable is
 *        removed with cellConsoleVariableRemove().
 * @param uiCapacity The maximum string length allowed.  Note that the buffer
 *        size must be uiCapacity + 1, allowing for the terminating null character.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified.</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddInt8, cellConsoleVariableAddUInt8,
 *     cellConsoleVariableAddInt16, cellConsoleVariableAddUInt16,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt32,
 *     cellConsoleVariableAddInt64, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddDouble,
 *     cellConsoleVariableAddBool, 
 */
	unsigned int cellConsoleVariableAddString(unsigned int uiGroup,
											  const char *pcName,
											  const char *pcDescription,
											  char *pcVariable,
										      unsigned int uiCapacity);
/**
 * @short Registers an 8-bit signed integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedInt8() enables tracking of an 8-bit signed integer stored at
 * a given memory location.  The 8-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddInt8() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pcVariable A pointer to the 8-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedInt8(unsigned int uiGroup,
												 const char *pcName,
												 const char *pcDescription,
												 int8_t *pcVariable,
												 CellConsoleVariableValidate pValidator,
												 void *pvValidatorPrivateData);

/**
 * @short Registers an 8-bit unsigned integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedUInt8() enables tracking of an 8-bit unsigned integer stored at
 * a given memory location.  The 8-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddUInt8() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pucVariable A pointer to the 8-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, 
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedUInt8(unsigned int uiGroup,
												  const char *pcName,
												  const char *pcDescription,
												  uint8_t *pucVariable,
												  CellConsoleVariableValidate pValidator,
												  void *pvValidatorPrivateData);

/**
 * @short Registers a 16-bit signed integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedInt16() enables tracking of a 16-bit signed integer stored at
 * a given memory location.  The 16-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddInt16() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param psVariable A pointer to the 16-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedInt16(unsigned int uiGroup,
												  const char *pcName,
												  const char *pcDescription,
												  int16_t *psVariable,
												  CellConsoleVariableValidate pValidator,
												  void *pvValidatorPrivateData);

/**
 * @short Registers a 16-bit unsigned integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedUInt16() enables tracking of a 16-bit unsigned integer stored at
 * a given memory location.  The 16-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddUInt16() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pusVariable A pointer to the 16-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, 
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedUInt16(unsigned int uiGroup,
												   const char *pcName,
												   const char *pcDescription,
												   uint16_t *pusVariable,
												   CellConsoleVariableValidate pValidator,
												   void *pvValidatorPrivateData);

/**
 * @short Registers a 32-bit signed integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedInt32() enables tracking of a 32-bit signed integer stored at
 * a given memory location.  The 32-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddInt32() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param piVariable A pointer to the 32-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedInt32(unsigned int uiGroup,
												  const char *pcName,
												  const char *pcDescription,
												  int32_t *piVariable,
												  CellConsoleVariableValidate pValidator,
												  void *pvValidatorPrivateData);

/**
 * @short Registers a 32-bit unsigned integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedUInt32() enables tracking of a 32-bit unsigned integer stored at
 * a given memory location.  The 32-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddUInt32() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param puiVariable A pointer to the 32-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, 
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedUInt32(unsigned int uiGroup,
												   const char *pcName,
												   const char *pcDescription,
												   uint32_t *puiVariable,
												   CellConsoleVariableValidate pValidator,
												   void *pvValidatorPrivateData);

/**
 * @short Registers a 64-bit signed integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedInt64() enables tracking of a 64-bit signed integer stored at
 * a given memory location.  The 64-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddInt64() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param plVariable A pointer to the 64-bit signed integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedInt64(unsigned int uiGroup,
												  const char *pcName,
												  const char *pcDescription,
												  int64_t *plVariable,
												  CellConsoleVariableValidate pValidator,
												  void *pvValidatorPrivateData);

/**
 * @short Registers a 64-bit unsigned integer value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedUInt64() enables tracking of a 64-bit unsigned integer stored at
 * a given memory location.  The 64-bit value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddUInt64() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pulVariable A pointer to the 64-bit unsigned integer.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, 
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedUInt64(unsigned int uiGroup,
												   const char *pcName,
												   const char *pcDescription,
												   uint64_t *pulVariable,
												   CellConsoleVariableValidate pValidator,
												   void *pvValidatorPrivateData);

/**
 * @short Registers a 32-bit floating point value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedFloat() enables tracking of a 32-bit floating point value stored at
 * a given memory location.  The float value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddFloat() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pfVariable A pointer to the 32-bit float.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedFloat(unsigned int uiGroup,
												  const char *pcName,
												  const char *pcDescription,
												  float *pfVariable,
												  CellConsoleVariableValidate pValidator,
												  void *pvValidatorPrivateData);

/**
 * @short Registers a 64-bit floating point (double) value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedDouble() enables tracking of a 64-bit floating point value stored at
 * a given memory location.  The double value can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made. There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddDouble() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pdVariable A pointer to the 64-bit double.  Please make sure that
 *        this pointer is valid until the variable is removed with
 *        cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.  The Control Console provides a default validate function
 *        that can be used for simple checking of numeric values.  See
 *        cellConsoleVariableValidateNumeric() for more details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.  If the
 *        cellConsoleVariableValidateNumeric() callback is used, this is a pointer
 *        to a CellConsoleVariableValidateNumericParam structure.  Note that this
 *        structure is not copied by the Control Console; this pointer needs to remain
 *        valid so long as the variable is registered with the Control Console.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified. </li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, 
 *     cellConsoleVariableAddValidatedBool, cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedDouble(unsigned int uiGroup,
												   const char *pcName,
												   const char *pcDescription,
												   double *pdVariable,
												   CellConsoleVariableValidate pValidator,
												   void *pvValidatorPrivateData);

/**
 * @short Registers a boolean value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedBool() enables tracking of a boolean value stored at
 * a given memory location.  The boolean can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * For C programs, the bool type is actually an unsigned character that will
 * be assigned values of 0 or 1 by the console.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddBool() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pbVariable A pointer to the boolean (or unsigned char for C).
 *        Please make sure that this pointer is valid until the variable is
 *        removed with cellConsoleVariableRemove().
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified.</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedString
 */
unsigned int cellConsoleVariableAddValidatedBool(unsigned int uiGroup,
												 const char *pcName,
												 const char *pcDescription,
												 CellConsoleBoolean *pbVariable,
												 CellConsoleVariableValidate pValidator,
												 void *pvValidatorPrivateData);

/**
 * @short Registers a string value with the Variable Tracker.
 * Data is validated through a callback before the variable is updated.
 *
 * cellConsoleVariableAddValidatedString() enables tracking of a string value stored at
 * a given memory location.  The string can be queried and changed
 * interactively through the console when the cellConsolePoll() function is
 * called.
 * 
 * This function also specifies the capacity of the buffer -- defining the maximum
 * length of string that can be entered.
 * 
 * To organize a large number of variables, related
 * values can be grouped together with a group created using
 * cellConsoleVariableAddGroup() or cellConsoleVariableAddGroupWithNamespace().
 * If cellConsoleVariableAddGroupWithNamespace() is used, the namespace
 * and a period will prefix the variable name in the Console.
 *
 * @note The console must be initialized using cellConsoleInit() before this
 *       call is made.
 *       There is a maximum of <c>CELL_CONSOLE_MAX_VARIABLES</c> variables.
 *
 * @note The cellConsoleVariableAddString() function allows the caller to add
 *       variables that do not require data validation.
 *
 * @note The Control Console uses generally a text-based interface for
 *       its commands, with spaces as delimiters.  Because of this, if you
 *       include a space in the name of a variable, that space will be
 *       confused with a delimiter.  The consequence of this is that the "set"
 *       command will fail.  Therefore, putting a space in a variable name
 *       is a way to make a Variable Tracker value read only: print and dump
 *       commands will successfully query the value, however set commands
 *       will be unable to modify it.
 *
 * @param uiGroup The group to add the variable to, created by
 *        cellConsoleVariableAddGroup().  Specify 0 to store the variable in
 *         the "default" group.
 * @param pcName The name that the variable will be referred to as in the
 *        console.  Only the first 127 bytes will be used.
 * @param pcDescription A brief description of the variable's purpose.  This
 *        description can be viewed with the <c>print</c> command in the console.
 *        Available at or above compatibility mode 0x31000000. Only the first 511
 *        bytes will be used.
 * @param pcVariable A pointer to the string.
 *        Please make sure that this pointer is valid until the variable is
 *        removed with cellConsoleVariableRemove().
 * @param uiCapacity The maximum string length allowed.  Note that the buffer
 *        size must be uiCapacity + 1, allowing for the terminating null character.
 * @param pValidator A pointer to a validate callback routine that checks values
 *        as they are entered through the console.  See CellConsoleVariableValidate()
 *        for details.
 * @param pvValidatorPrivateData Pointer to user-specified data that is passed
 *        to the validate callback routine.  This data could, for instance, point
 *        to a structure that contains rules to validate a number in order to
 *        facilitate the implementation of a generic data validator.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_ENOMEM</c> if the maximum number of variables has been
 *         reached. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified.</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup, cellConsoleVariableAddGroupWithNamespace, cellConsoleVariableRemove,
 *     cellConsoleVariableAddValidatedInt8, cellConsoleVariableAddValidatedUInt8,
 *     cellConsoleVariableAddValidatedInt16, cellConsoleVariableAddValidatedUInt16,
 *     cellConsoleVariableAddValidatedInt32, cellConsoleVariableAddValidatedUInt32,
 *     cellConsoleVariableAddValidatedInt64, cellConsoleVariableAddValidatedUInt64,
 *     cellConsoleVariableAddValidatedFloat, cellConsoleVariableAddValidatedDouble,
 *     cellConsoleVariableAddValidatedBool
 */
unsigned int cellConsoleVariableAddValidatedString(unsigned int uiGroup,
												   const char *pcName,
												   const char *pcDescription,
												   char *pcVariable,
												   unsigned int uiCapacity,
												   CellConsoleVariableValidate pValidator,
												   void *pvValidatorPrivateData);

/**
 * @short Removes a value registered with the Variable Tracker.
 *
 * cellConsoleVariableRemove() unregisters a variable and disables tracking of
 * it.  The console will no longer maintain any pointers to that data.
 *
 * @param pvVariable A pointer to a variable to remove.  This variable was
 *        added using one of the cellConsoleVariableAdd*() functions.
 *
 * @return 
 * <ul>
 * <li>On success, the current number of variables tracked. </li>
 * <li>On failure, a value greater or equal to <c>CELL_CONSOLE_ERROR_BASE</c></li>
 * <li><c>CELL_CONSOLE_NOT_INITIALIZED</c> if the console has not been
 *         initialized. </li>
 * <li><c>CELL_CONSOLE_EINVAL</c> if a bad parameter has been specified.</li>
 * </ul>
 *
 * @sa cellConsoleInit, cellConsoleVariableAddGroup,
 *     cellConsoleVariableAddInt32, cellConsoleVariableAddUInt64,
 *     cellConsoleVariableAddFloat, cellConsoleVariableAddBool
 */
	unsigned int cellConsoleVariableRemove(void *pvVariable);
  
#ifdef __cplusplus
}
#endif

#endif
