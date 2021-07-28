/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
#ifndef CELL_GCM_METHOD_CHECK_H
#define CELL_GCM_METHOD_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

enum CellGcmMethodCheckErrorStatus{
  CELL_GCM_METHOD_CHECK_OK=0,
  CELL_GCM_METHOD_CHECK_ERROR_JUMP=1,
  CELL_GCM_METHOD_CHECK_ERROR_RETURN=2,
  CELL_GCM_METHOD_CHECK_ERROR_CALL=3,
  CELL_GCM_METHOD_CHECK_ERROR_OTHER=4,
  CELL_GCM_METHOD_CHECK_ERROR_METHOD_UNCLOSED=100,
  CELL_GCM_METHOD_CHECK_ERROR_METHOD_INVALID_ADDRESS=1000,
  CELL_GCM_METHOD_CHECK_ERROR_METHOD_INCORRECT_DATA=2000,
  CELL_GCM_METHOD_CHECK_ERROR_METHOD_NOP_NOT_DELIVER=10000,
  CELL_GCM_METHOD_CHECK_ERROR_ETC=777,
};


int32_t cellGcmMethodCheck(const uint32_t* start, const uint32_t* end,const uint32_t** problem_ptr,uint32_t reserved);

#ifdef __cplusplus
}
#endif

#endif
