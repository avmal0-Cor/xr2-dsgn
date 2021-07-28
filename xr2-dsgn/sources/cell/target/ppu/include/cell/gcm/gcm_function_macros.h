/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

//
//	define function macros
//
#ifdef CELL_GCM_FUNCTION_MACROS

//
//	CELL_GCM_CALLBACK_FUNC
//
#if defined(__SNC__)
# if __SN_VER__ <= 25002
#  if defined(CELL_GCM_SNC_NOTOCRESTORE_2)
#   define CELL_GCM_CALLBACK_FUNC	cellGcmCallbackForSnc
#  else  // !defined(CELL_GCM_SNC_NOTOCRESTORE_2)
#   define CELL_GCM_CALLBACK_FUNC	(*CELL_GCM_CALLBACK)
#  endif
# else  // __SN_VER__ >25002
#  if __option(notocrestore)==2
#   define CELL_GCM_CALLBACK_FUNC	cellGcmCallbackForSnc
#  else  // notocrestore<2
#   define CELL_GCM_CALLBACK_FUNC	(*CELL_GCM_CALLBACK)
#  endif
# endif
#else  // gcc
#  if defined(CELL_GCM_SNC_NOTOCRESTORE_2)
#   define CELL_GCM_CALLBACK_FUNC	cellGcmCallbackForSnc
#  else  // !defined(CELL_GCM_SNC_NOTOCRESTORE_2)
#   define CELL_GCM_CALLBACK_FUNC	(*CELL_GCM_CALLBACK)
#  endif
#endif

//
//	CELL_GCM_RESERVE, CELL_GCM_RESERVE_RET
//
#if (CELL_GCM_MEASURE == 2)
# define CELL_GCM_RESERVE(count) CELL_GCM_THIS += (count)
# define CELL_GCM_RESERVE_RET(count, value) CELL_GCM_THIS += (count)
#elif (CELL_GCM_MEASURE == 1)
# define CELL_GCM_RESERVE(count) do { CELL_GCM_CURRENT += (count); } while(false)
# define CELL_GCM_RESERVE_RET(count, value) do { CELL_GCM_CURRENT += (count); } while(false)
#else
# if CELL_GCM_UNSAFE
#  define CELL_GCM_RESERVE(count) do { } while(false)
#  define CELL_GCM_RESERVE_RET(count, value) do { } while(false)
# else
#  define CELL_GCM_RESERVE(count) do { \
	    if(CELL_GCM_CURRENT + (count) > CELL_GCM_END) \
		{ \
			if(CELL_GCM_CALLBACK_FUNC(CELL_GCM_THIS, (count)) != CELL_OK) \
				return; \
		} \
		} while(false)

#  define CELL_GCM_RESERVE_RET(count, value) do { \
	    if(CELL_GCM_CURRENT + (count) > CELL_GCM_END) \
		{ \
			if(CELL_GCM_CALLBACK_FUNC(CELL_GCM_THIS, (count)) != CELL_OK) \
				return (value); \
		} \
		} while(false)
# endif
#endif

//
//	CELL_GCM_FUNC_TYPE, CELL_GCM_FUNC_RETURN
//
#if (CELL_GCM_MEASURE == 2)
# define CELL_GCM_FUNC_TYPE			uint32_t
# define CELL_GCM_FUNC_TYPE_INT32	uint32_t
# define CELL_GCM_FUNC_TYPE_UINT32	uint32_t
# define CELL_GCM_FUNC_RETURN			return CELL_GCM_THIS
# define CELL_GCM_FUNC_RETURN_CELL_OK	return CELL_GCM_THIS
#else
# define CELL_GCM_FUNC_TYPE			void
# define CELL_GCM_FUNC_TYPE_INT32	int32_t
# define CELL_GCM_FUNC_TYPE_UINT32	uint32_t
# define CELL_GCM_FUNC_RETURN			return
# define CELL_GCM_FUNC_RETURN_CELL_OK	return CELL_OK
#endif

//
//	CELL_GCM_DECL
//
#if CELL_GCM_INLINE
# ifdef __cplusplus
#	ifdef WIN32	// use VC++
#		define CELL_GCM_DECL inline
#  	else		// use g++
#		define CELL_GCM_DECL inline __attribute__((always_inline))
#	endif
# else			// use gcc
#  define CELL_GCM_DECL extern inline __attribute__((always_inline))
# endif
#else	// not inline
# define CELL_GCM_DECL
#endif

//
//	CELL_GCM_FUNC, CELL_GCM_NO_ARGS, CELL_GCM_ARGS, CELL_GCM_ARGS_FUNC, CELL_GCM_NO_ARGS_FUNC
//
#if (CELL_GCM_MODE == CELL_GCM_CPP_IMPLICIT)

# define CELL_GCM_FUNC(func) 		cellGcm ## func
# define CELL_GCM_NO_ARGS()
# define CELL_GCM_ARGS(...)			__VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()
# define CELL_GCM_ARGS_FUNC(...) 	__VA_ARGS__

#elif (CELL_GCM_MODE == CELL_GCM_CPP_EXPLICIT_METHOD)

# define CELL_GCM_FUNC(func)		func
# define CELL_GCM_NO_ARGS()
# define CELL_GCM_ARGS(...)			__VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()
# define CELL_GCM_ARGS_FUNC(...)	__VA_ARGS__


#elif(CELL_GCM_MODE == CELL_GCM_CPP_EXPLICIT_NAMESPACE)

# if (CELL_GCM_MEASURE == 2)
# define CELL_GCM_FUNC(func)		cellGcm ## func
# define CELL_GCM_NO_ARGS()			uint32_t CELL_GCM_THIS
# define CELL_GCM_ARGS(...)			uint32_t CELL_GCM_THIS, __VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()	0
# define CELL_GCM_ARGS_FUNC(...)	0, __VA_ARGS__
# else
# define CELL_GCM_FUNC(func)		cellGcm ## func
# define CELL_GCM_NO_ARGS()			struct CellGcmContextData *CELL_GCM_THIS
# define CELL_GCM_ARGS(...)			struct CellGcmContextData *CELL_GCM_THIS, __VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()	CELL_GCM_THIS
# define CELL_GCM_ARGS_FUNC(...)	CELL_GCM_THIS, __VA_ARGS__
# endif

#elif(CELL_GCM_MODE == CELL_GCM_CPP_EXPLICIT_PRIVATE)

# define CELL_GCM_FUNC(func)		func
# define CELL_GCM_NO_ARGS()			struct CellGcmContextData *CELL_GCM_THIS
# define CELL_GCM_ARGS(...)			struct CellGcmContextData *CELL_GCM_THIS, __VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()	CELL_GCM_THIS
# define CELL_GCM_ARGS_FUNC(...)	CELL_GCM_THIS, __VA_ARGS__


#else	// if (CELL_GCM_MODE == CELL_GCM_C)

# if (CELL_GCM_MEASURE == 2)
# define CELL_GCM_NO_ARGS() 		uint32_t CELL_GCM_THIS
# define CELL_GCM_ARGS(...) 		uint32_t CELL_GCM_THIS, __VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()	0
# define CELL_GCM_ARGS_FUNC(...)	0, __VA_ARGS__
# else
# define CELL_GCM_NO_ARGS() 		struct CellGcmContextData *CELL_GCM_THIS
# define CELL_GCM_ARGS(...) 		struct CellGcmContextData *CELL_GCM_THIS, __VA_ARGS__
# define CELL_GCM_NO_ARGS_FUNC()	CELL_GCM_THIS
# define CELL_GCM_ARGS_FUNC(...)	CELL_GCM_THIS, __VA_ARGS__
# endif

# if CELL_GCM_INLINE
#	if (CELL_GCM_MEASURE == 2)
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## MeasureSizeInline
#		define CELL_GCM_FUNC_CALL(func)	CELL_GCM_THIS += CELL_GCM_FUNC(func)
# 	elif (CELL_GCM_MEASURE == 1)
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## MeasureInline
# 		define CELL_GCM_FUNC_CALL(func)	CELL_GCM_FUNC(func)
# 	elif CELL_GCM_UNSAFE
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## UnsafeInline
# 	else	// CELL_GCM_SAFE
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## Inline
# 	endif

# else	// if (CELL_GCM_INLINE == 0)
# 	if (CELL_GCM_MEASURE == 2)
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## MeasureSize
#		define CELL_GCM_FUNC_CALL(func)	CELL_GCM_THIS += CELL_GCM_FUNC(func)
# 	elif (CELL_GCM_MEASURE == 1)
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## Measure
#		define CELL_GCM_FUNC_CALL(func)	CELL_GCM_FUNC(func)
# 	elif CELL_GCM_UNSAFE
# 		define CELL_GCM_FUNC(func)	cellGcm ## func ## Unsafe
# 	else	// CELL_GCM_SAFE
# 		define CELL_GCM_FUNC(func)	cellGcm ## func
# 	endif
# endif // CELL_GCM_INLINE

#endif	// CELL_GCM_MODE


//
//	CELL_GCM_CALLEE		only if (CELL_GCM_MODE == CELL_GCM_CPP)
//
#if CELL_GCM_INLINE
#  if (CELL_GCM_MEASURE == 2)
#	   define CELL_GCM_CALLEE(func) cellGcm ## func ## MeasureSizeInline
#  elif (CELL_GCM_MEASURE == 1)
#	   define CELL_GCM_CALLEE(func) cellGcm ## func ## MeasureInline
#  else
#    if CELL_GCM_UNSAFE
#      define CELL_GCM_CALLEE(func) cellGcm ## func ## UnsafeInline
#    else
#      define CELL_GCM_CALLEE(func) cellGcm ## func ## Inline
#    endif
#  endif 
#else
#  if (CELL_GCM_MEASURE == 2)
#    define CELL_GCM_CALLEE(func) cellGcm ## func ## MeasureSize
#  elif (CELL_GCM_MEASURE == 1)
#    define CELL_GCM_CALLEE(func) cellGcm ## func ## Measure
#  else
#    if CELL_GCM_UNSAFE
#      define CELL_GCM_CALLEE(func) cellGcm ## func ## Unsafe
#    else
#      define CELL_GCM_CALLEE(func) cellGcm ## func
#    endif
#  endif
#endif

//
//	CELL_GCM_GLOBAL_DECL	only use in gcm_global_implementation.h
//
#ifdef WIN32	// use VC++
# define CELL_GCM_GLOBAL_DECL inline
#else			// use g++
# define CELL_GCM_GLOBAL_DECL inline __attribute__((always_inline))
#endif

//
//	CELL_GCM_ASM_IN, CELL_GCM_ASM_OUT
//
#define	CELL_GCM_ASM_IN()
#define	CELL_GCM_ASM_OUT()

//
//	CELL_GCM_ASM_RESERVE
//
#define CELL_GCM_ASM_RESERVE_IMM(count, args)	CELL_GCM_RESERVE(count)
#define CELL_GCM_ASM_RESERVE_REG(count, args)	CELL_GCM_RESERVE(count)

//
// endif function macros
//
#endif	

//
//	undef function macros
//
#ifndef CELL_GCM_FUNCTION_MACROS
#undef CELL_GCM_CALLBACK_FUNC
#undef CELL_GCM_RESERVE
#undef CELL_GCM_RESERVE_RET
#undef CELL_GCM_FUNC_TYPE
#undef CELL_GCM_FUNC_TYPE_INT32
#undef CELL_GCM_FUNC_TYPE_UINT32
#undef CELL_GCM_FUNC_RETURN
#undef CELL_GCM_FUNC_RETURN_CELL_OK
#undef CELL_GCM_DECL
#undef CELL_GCM_FUNC
#undef CELL_GCM_FUNC_CALL
#undef CELL_GCM_NO_ARGS
#undef CELL_GCM_ARGS
#undef CELL_GCM_ARGS_FUNC
#undef CELL_GCM_NO_ARGS_FUNC
#undef CELL_GCM_CALLEE
#undef CELL_GCM_GLOBAL_DECL
#undef CELL_GCM_ASM_IN
#undef CELL_GCM_ASM_OUT
#undef CELL_GCM_ASM_RESERVE
#endif
