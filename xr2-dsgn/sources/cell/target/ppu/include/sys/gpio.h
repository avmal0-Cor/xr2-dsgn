/*
 *   SCE CONFIDENTIAL                                      
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2007 Sony Computer Entertainment Inc.   
 *   All Rights Reserved.                                  
 */

#ifndef __LV2_IOSYS_SYSCALL_GPIO_H__
#define __LV2_IOSYS_SYSCALL_GPIO_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>

CDECL_BEGIN

/*
 * GPIO USER AVAILABLE BITS
 */
#define SYS_GPIO_LED_USER_AVAILABLE_BITS        0x000000000000000FULL
#define SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS 0x000000000000000FULL

/*
 * GPIO DEVICE ID
 */
#define SYS_GPIO_UNKNOWN_DEVICE_ID          0x0000000000000000ULL
#define SYS_GPIO_LED_DEVICE_ID              0x0000000000000001ULL
#define SYS_GPIO_DIP_SWITCH_DEVICE_ID       0x0000000000000002ULL

/**
 * \defgroup GPIO_SYSCALLS GPIO System Calls
 * \brief Control GPIO devices like LEDs or Dip Switches on the front panel.
 */
/*@{*/

/**
 * \brief The sys_gpio_set() sets GPIOs.
 *
 * \param[in] device_id Specify the target GPIO device
 * \param[in] mask      Specify the mask bits of the value argument.
 * \param[in] value     Specify the value of GPIO. 
 *
 * \retval CELL_OK The specified GPIO value was set.
 * \retval ESRCH   Can't find the target GPIO device.
 * \retval EINVAL  The target GPIO device is read-only.
 *
 * \note If the specified device_id is SYS_GPIO_LED_DEVICE_ID, this system call always returns CELL_OK even if the platform does not have LEDs. But if the platform does not have LEDs, this system call does nothing.
 * 
 * \note The valid bits of the value argument depend on the device. If device_id is SYS_GPIO_LED_DEVICE_ID, 0-7 bits(LSB is 0) are valid.  
 */
extern inline int sys_gpio_set(uint64_t device_id, uint64_t mask,
							   uint64_t value);

extern inline int sys_gpio_set(uint64_t device_id, uint64_t mask,
							   uint64_t value)
{
	system_call_3(SYS_GPIO_SET, device_id, mask, value);

#ifdef REG_PASS_SYS_GPIO_SET
	REG_PASS_SYS_GPIO_SET;
#endif
	return_to_user_prog(int);
}


/**
 * \brief The sys_gpio_get() returns the current GPIO value.
 *
 * \param[in]  device_id Specify the target GPIO device.
 * \param[out] value     Specify the pointer of storage for the GPIO value.
 *
 * \retval CELL_OK Success.
 * \retval ESRCH   Can't find the target GPIO device.
 *
 * \note If the specified device_id is SYS_GPIO_LED_DEVICE_ID or SYS_GPIO_DIP_SWITCH_DEVICE_ID, this system call always returns CELL_OK even if the platform does not have LEDs and Dip switches. But if the platform does not have those devices, this system call set 0 to the value parameter.
 * 
 * \note The valid bits of the value argument depend on the device. If device_id is SYS_GPIO_LED_DEVICE_ID or SYS_GPIO_DIP_SWITCH_DEVICE_ID, 0-7 bits(LSB is 0) are valid.  
 */
extern inline int sys_gpio_get(uint64_t device_id, uint64_t * value);

extern inline int sys_gpio_get(uint64_t device_id, uint64_t * value)
{
	system_call_2(SYS_GPIO_GET, device_id, (uint32_t) value);

#ifdef REG_PASS_SYS_GPIO_GET
	REG_PASS_SYS_GPIO_GET;
#endif
	return_to_user_prog(int);
}


/*@}*/

CDECL_END

#endif /* __LV2_IOSYS_SYSCALL_GPIO_H__  */
