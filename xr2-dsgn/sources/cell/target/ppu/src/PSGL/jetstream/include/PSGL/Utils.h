/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _JSUtils_h
#define _JSUtils_h

#include <PSGL/Types.h>
#include <PSGL/export.h>
#include <PSGL/platform.h>

#include <PlatformPerfCounter.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MSVC
// MAX that works with side effects. Just in case.
#define MAX(A,B) ({				\
	__typeof__(A) At=(A);	\
	__typeof__(B) Bt=(B);	\
	At>Bt?At:Bt; })

#define MIN(A,B) ({				\
	__typeof__(A) At=(A);	\
	__typeof__(B) Bt=(B);	\
	At<Bt?At:Bt; })

#define JS_LIKELY(COND) __builtin_expect((COND),1)
#define JS_UNLIKELY(COND) __builtin_expect((COND),0)

#else
#define MAX(A,B) ((A)>(B)?(A):(B))
#define MIN(A,B) ((A)<(B)?(A):(B))
#define JS_LIKELY(COND) (COND)
#define JS_UNLIKELY(COND) (COND)
#endif

#define _JS_FLOAT_AS_UINT(x) ({union {float f; unsigned int i;} u; u.f=(x); u.i;})

    static inline float _jsClampf( const float value )
    {
        return MAX( MIN( value, 1.f ), 0.f );
    }

    static inline unsigned short endianSwapHalf( unsigned short v )
    {
        return ( v >> 8 & 0x00ff ) | ( v << 8 & 0xff00 );
    }

    static inline unsigned int endianSwapWord( unsigned int v )
    {
        return ( v&0xff ) << 24 | ( v&0xff00 ) << 8 |
               ( v&0xff0000 ) >> 8 | ( v&0xff000000 ) >> 24;
    }

    static inline unsigned int endianSwapWordByHalf( unsigned int v )
    {
        return ( v&0xffff ) << 16 | v >> 16;
    }

    static inline int _jsLog2( unsigned int i )
    {
        int l = 0;
        while ( i )
        {
            ++l;
            i >>= 1;
        }
        return l -1;
    }

    static inline int _jsIsPow2( unsigned int i )
    {
        return ( i&( i - 1 ) ) == 0;
    }

// Pad argument x to the next multiple of argument pad.
    static inline unsigned long _jsPad( unsigned long x, unsigned long pad )
    {
        return ( x + pad - 1 ) / pad*pad;
    }

// Pad pointer x to the next multiple of argument pad.
    static inline char* _jsPadPtr( const char* p, unsigned int pad )
    {
        JSintptr x = ( JSintptr )p;
        x = ( x + pad - 1 ) / pad * pad;
        return ( char* )x;
    }

// Performance measurment utilities
    typedef struct PerformanceCounter_t_
    {
        jsUint64 start;
        jsUint64 total;
    }
    PerformanceCounter_t;

    PSGL_EXPORT void _jsInitPerformanceCounter();

    extern PSGL_EXPORT double _PerformanceResolution;
    extern PSGL_EXPORT jsUint64 _PerformanceStep;

#define _JS_RESET_COUNTER(X) do { (X).total=0; } while (0)
#define _JS_START_COUNTER(X) do { (X).start=_jsGetPerformanceCounter(); } while (0)
#define _JS_STOP_COUNTER(X) do { (X).total+=_jsGetPerformanceCounter()-(X).start; } while (0)
#define _JS_GET_COUNTER(X) (_PerformanceResolution*(double)(X).total)
#define	_JS_ADD_COUNTER(X,Y)	do { (X).total += (Y).total; } while (0)
#define	_JS_SET_COUNTER(X,F)	do { (X).total = (jsUint64)((double)(F)/_PerformanceResolution); } while (0)
#define	_JS_COPY_COUNTER(X,Y)	do { (X).total = (Y).total; } while (0)

#ifdef JS_PERFORMANCE_INFO
#include <stdio.h>
#define _JS_PERFORMANCE_RESET(X) _JS_RESET_COUNTER(X)
#define _JS_PERFORMANCE_START(X) _JS_START_COUNTER(X)
#define _JS_PERFORMANCE_STOP(X) _JS_STOP_COUNTER(X)
#define _JS_PERFORMANCE_VALUE(X) _JS_GET_COUNTER(X)
#define	_JS_PERFORMANCE_ADD(X,Y) _JS_ADD_COUNTER(X,Y)
#define	_JS_PERFORMANCE_SET(X,F) _JS_SET_COUNTER(X,F)
#define	_JS_PERFORMANCE_COPY(X,Y) _JS_COPY_COUNTER(X,Y)
#define _JS_PERFORMANCE_WARN(X,LIMIT) \
	do { \
		if ((_jsGetPerformanceCounter()-X.start) > (LIMIT)) \
		{ \
			printf("warning: %s count > %ld at %s:%d\n", \
				#X,(unsigned long)(LIMIT),__FILE__,__LINE__); \
		} \
	} while (0)
#define _JS_PERFORMANCE_INC(X) do { (X).total+=_PerformanceStep; } while (0)
#else
#define _JS_PERFORMANCE_RESET(X) do {} while (0)
#define _JS_PERFORMANCE_START(X) do {} while (0)
#define _JS_PERFORMANCE_STOP(X) do {} while (0)
#define _JS_PERFORMANCE_VALUE(X) (0.0)
#define	_JS_PERFORMANCE_ADD(X,Y) do {} while (0)
#define	_JS_PERFORMANCE_SET(X,F) do {} while (0)
#define	_JS_PERFORMANCE_COPY(X,Y) do {} while (0)
#define _JS_PERFORMANCE_WARN(X,LIMIT)
#define _JS_PERFORMANCE_INC(X) do {} while (0)
#endif

    typedef struct MemoryBlockManager_t_
    {
        char *memory;
        GLuint size;
        GLuint minAlignment;
        GLenum method;
        GLuint *book;
        GLint bookSize;
        GLint bookAlloc;
    }
    MemoryBlockManager_t;

#define _JS_ALLOC_FIRST_FIT 0
#define _JS_ALLOC_BEST_FIT 1

    PSGL_EXPORT int _jsMemoryBlockManagerInit( MemoryBlockManager_t *block, void * memory, GLuint size, GLuint alignment, GLenum method );
    PSGL_EXPORT void _jsMemoryBlockManagerWipe( MemoryBlockManager_t *block );
    PSGL_EXPORT void _jsMemoryBlockManagerDestroy( MemoryBlockManager_t *block );
    PSGL_EXPORT void *_jsMemoryBlockManagerAlloc( MemoryBlockManager_t *block, GLuint size );
    PSGL_EXPORT void *_jsMemoryBlockManagerAllocAligned( MemoryBlockManager_t *block, GLuint size, GLuint alignment );
    PSGL_EXPORT void *_jsMemoryBlockManagerRealloc( MemoryBlockManager_t *block, void *mem, GLuint size );
    PSGL_EXPORT void *_jsMemoryBlockManagerReallocAligned( MemoryBlockManager_t *block, void *mem, GLuint size, GLuint alignment );
    PSGL_EXPORT void _jsMemoryBlockManagerFree( MemoryBlockManager_t *block, void *mem );
    PSGL_EXPORT GLboolean _jsMemoryBlockManagerTryRealloc( MemoryBlockManager_t *block, void *mem, GLuint size );

// fixed allocator
//  jsFixedAllocator provides specialized memory management for fixed size
//  elements.  It works by using the general purpose allocator to allocate
//  blocks to hold multiple fixed size elements.
//
//  Allocation is fast except when a new block of elements must be created.
//  Deallocation is fast unless there is a large number of blocks.  This
//  profile suggests tuning the initial allocation to the maximum expected
//  size for best performance.
    typedef struct jsFixedAllocator jsFixedAllocator;
    jsFixedAllocator* _jsFixedAllocatorCreate( GLuint elementBytes, GLuint initialCount );
    void _jsFixedAllocatorDestroy( jsFixedAllocator* a );
    int _jsFixedAllocatorSave( jsFixedAllocator * a, void * saveArea, unsigned long * offset, unsigned long offsetMax );
    void _jsFixedAllocatorRestore( jsFixedAllocator * a, void * saveArea, unsigned long * offset );
    void* _jsFixedAllocatorAlloc( jsFixedAllocator* a );
    void _jsFixedAllocatorFree( jsFixedAllocator* a, void* p );

// ring buffer
//  JSringBuffer manages a piece of memory whose allocation/deallocation
//  pattern occurs "mostly" first in, first out.  Allocations always occur
//  in sequential memory in the buffer, with wrap.
//
//  This API has a few quirks that cause it not to behave like a regular
//  allocator.  First, an allocation request may return NULL even if there
//  is space in the buffer.  This is because allocations always occur in
//  sequential memory.  Thus, even if there is a free block of sufficient
//  size, it won't be used unless it is sequential to the immediately
//  previous allocation.
//
//  Allocations are made with a tag, which is an unsigned integer less than
//  0xffffffff.  This allows sequential allocations with the same tag to be
//  freed all at once using _jsRingBufferFreeGroup by passing just the first
//  pointer of the group.  This is intended to reduce buffer management
//  outside the API and improve buffer usage by allowing groups to span the
//  end of the ring buffer.
//
//  A new allocation may be linked to the immediately preceeding allocation
//  using _jsRingBufferLink.  This will allocate memory using the previous
//  tag, returning a pointer.  The new block may be created by extending the
//  previous allocation if there is enough contiguous space in the ring
//  buffer, but this is not guaranteed.  Because of this uncertainty,
//  linked pointers should never be freed directly, only through
//  _jsRingBufferFreeGroup (linking to a freed buffer is an error).  Linked
//  memory may be more cache-friendly on a free, as less traversal over
//  blocks may be needed (the traversal is moved to allocation, when the
//  previous block is likely to be in cache).
//
//  Block management information is kept inside the buffer, so the actual
//  capacity of the buffer for user data depends on how many allocations
//  there are.  Four ints of overhead are used for each block (both free and
//  used).
    typedef struct JSringBuffer JSringBuffer;
    JSringBuffer* _jsRingBufferCreate( GLuint size, GLuint alignment );
    void _jsRingBufferDestroy( JSringBuffer* ringBuffer );
    void* _jsRingBufferAlloc( JSringBuffer* ringBuffer, GLuint size, GLuint tag );
    void* _jsRingBufferLink( JSringBuffer* ringBuffer, GLuint size );
    void _jsRingBufferFree( JSringBuffer* ringBuffer, void* p );
    void _jsRingBufferFreeGroup( JSringBuffer* ringBuffer, void* p );

#define _JS_LIST_DEFINE(NAME,TYPE) \
	TYPE *NAME; \
	GLint NAME##Count; \
	GLint NAME##Capacity;

#define _JS_LIST_INIT(NAME,TYPE,CAPACITY) \
do { \
	GLint __capa=(CAPACITY); \
	NAME=__capa?(TYPE *)jsMalloc(__capa*sizeof(TYPE)):NULL; \
	JS_ASSERT(NAME||!__capa); \
	NAME##Count=0; \
	NAME##Capacity=__capa; \
} while (0)

#define _JS_LIST_DESTROY(NAME,TYPE) \
do { \
	if (NAME) jsFree(NAME); \
	NAME=NULL; \
	NAME##Count=0; \
	NAME##Capacity=0; \
} while (0)

#define _JS_LIST_CHECK_CAPACITY(NAME,TYPE) \
do { \
	if (NAME##Capacity<NAME##Count) \
	{ \
		NAME##Capacity=MAX(NAME##Count,NAME##Capacity*2);\
		NAME=(TYPE *)jsRealloc(NAME,sizeof(TYPE)*NAME##Capacity); \
		JS_ASSERT(NAME); \
	} \
} while (0)

#define _JS_LIST_APPEND(NAME,TYPE,VALUE) \
do { \
	++(NAME##Count); \
	_JS_LIST_CHECK_CAPACITY(NAME,TYPE); \
	NAME[(NAME##Count)-1]=VALUE; \
} while (0)

#define _JS_LIST_INSERT(NAME,TYPE,INDEX,VALUE) \
do { \
	GLint __idx=INDEX; \
	++(NAME##Count); \
	JS_ASSERT(__idx<NAME##Count);\
	_JS_LIST_CHECK_CAPACITY(NAME,TYPE); \
	memcpy(NAME+__idx+1,NAME+__idx,sizeof(TYPE)*((NAME##Count)-__idx-1)); \
	NAME[(INDEX)]=VALUE; \
} while (0)

#define _JS_LIST_REMOVE(NAME,TYPE,INDEX) \
do { \
	GLint __idx=INDEX; \
	JS_ASSERT(__idx<NAME##Count);\
	memcpy(NAME+__idx,NAME+__idx+1,sizeof(TYPE)*((NAME##Count)-__idx-1)); \
	--(NAME##Count); \
} while (0)

// names API
    typedef unsigned long jsName;

    PSGL_EXPORT void _jsInitNameSpace( struct jsNameSpace * name );
    PSGL_EXPORT void _jsFreeNameSpace( struct jsNameSpace * name );
    PSGL_EXPORT jsName _jsCreateName( struct jsNameSpace * ns, void* object );
    PSGL_EXPORT unsigned int _jsIsName( struct jsNameSpace* ns, jsName name );
    PSGL_EXPORT void _jsEraseName( struct jsNameSpace* ns, jsName name );
    static inline void * _jsGetNamedValue( struct jsNameSpace* ns, jsName name )
    {
        JS_ASSERT( _jsIsName( ns, name ) );
        return ns->data[name - 1];
    }

    void _jsTexNameSpaceInit( jsTexNameSpace *ns, jsTexNameSpaceCreateFunction create, jsTexNameSpaceDestroyFunction destroy );
    void _jsTexNameSpaceFree( jsTexNameSpace *ns );
    void _jsTexNameSpaceResetNames( jsTexNameSpace *ns );
    GLuint _jsTexNameSpaceGetFree( jsTexNameSpace *ns );
    GLboolean _jsTexNameSpaceCreateNameLazy( jsTexNameSpace *ns, GLuint name );
    GLboolean _jsTexNameSpaceIsName( jsTexNameSpace *ns, GLuint name );
    void _jsTexNameSpaceGenNames( jsTexNameSpace *ns, GLsizei n, GLuint *names );
    void _jsTexNameSpaceDeleteNames( jsTexNameSpace *ns, GLsizei n, const GLuint *names );
    void _jsTexNameSpaceReinit( jsTexNameSpace * saved, jsTexNameSpace * active );


// Tracking of memory allocations
    typedef enum
    {
        _JS_ALLOCTYPE_TEXTURE,
        _JS_ALLOCTYPE_RENDERBUFFER,
        _JS_ALLOCTYPE_FRAGMENT_PROGRAM,
        _JS_ALLOCTYPE_COLOR_FRAMEBUFFER,
        _JS_ALLOCTYPE_DEPTH_FRAMEBUFFER,
        _JS_ALLOCTYPE_SCANOUT_FRAMEBUFFER,
        _JS_ALLOCTYPE_BUFFER_OBJECT,
        _JS_ALLOCTYPE_SHARED_FP_CONSTANTS,
        _JS_ALLOCTYPE_FIXED_PIPELINE_PROGRAM,
        _JS_ALLOCTYPE_GEOMETRY_TRANSFER,
        _JS_ALLOCTYPE_TEXTURE_UPLOAD,
        _JS_ALLOCTYPE_BLIT_QUAD_VERTEX_BUFFER,
        _JS_ALLOCTYPE_SURFACE_COPY_TEMP_BUFFER,
        _JS_ALLOCTYPE_MIPMAP_GEN_TEMP_BUFFER,
        _JS_ALLOCTYPE_HUD,
        _JS_ALLOCTYPE_UNKNOWN,
    }
    jsAllocType;


#ifdef JS_ALLOCINFO
    void _jsAllocClearHint();
    void _jsAllocSetHint( jsAllocType objectType, GLuint objectName );
#else
#define _jsAllocClearHint()                       do {} while(0)
#define _jsAllocSetHint(objectType, objectName)   do {} while(0)
#endif

// scope timer ... adds time elapsed in the scope of the object
// to a specified jsUint64
    class jsScopeTimer
    {
    protected:
        jsUint64& m_uiCounter;
        jsUint64 m_uiBegin;

    public:
        jsScopeTimer( jsUint64& uiCounter ) : m_uiCounter( uiCounter )
        {
            m_uiBegin = _jsGetPerformanceCounter();
        }
        ~jsScopeTimer()
        {
            jsUint64 uiTime = _jsGetPerformanceCounter();
            m_uiCounter += uiTime - m_uiBegin;
        }
    };

// scope timer 2 ... adds time elapsed in the scope of the object
// to a specified pair of jsUint64s
    class jsScopeTimer2
    {
    protected:
        jsUint64& m_uiCounter0;
        jsUint64& m_uiCounter1;
        jsUint64 m_uiBegin;

    public:
        jsScopeTimer2( jsUint64& uiCounter0, jsUint64& uiCounter1 ) :
                m_uiCounter0( uiCounter0 ),
                m_uiCounter1( uiCounter1 )
        {
            m_uiBegin = _jsGetPerformanceCounter();
        }
        ~jsScopeTimer2()
        {
            jsUint64 uiTime = _jsGetPerformanceCounter();
            jsUint64 uiDif = uiTime - m_uiBegin;
            m_uiCounter0 += uiDif;
            m_uiCounter1 += uiDif;
        }
    };

// APIENTRY timer
#ifdef JS_APIENTRY_TIMERS
    typedef struct APIEntryTimer_t_
    {
        jsUint64 uiElapsed;
        jsUint32 uiHitCount;
    }
    APIEntryTimer_t;

#define JS_APIENTRY_TIMER_NAME(name) _jsEntryTimer_##name
#define JS_APIENTRY_TIMER(name) \
  extern jsUint64 _jsEntryTimer_Overall; \
  extern APIEntryTimer_t JS_APIENTRY_TIMER_NAME(name); \
  JS_APIENTRY_TIMER_NAME(name).uiHitCount+=1; \
  jsScopeTimer2 kTimer_##name(_jsEntryTimer_Overall, JS_APIENTRY_TIMER_NAME(name).uiElapsed);
#define JS_APIENTRY_TIMER_SKIP_OVERALL(name) \
  extern APIEntryTimer_t JS_APIENTRY_TIMER_NAME(name); \
  JS_APIENTRY_TIMER_NAME(name).uiHitCount+=1; \
  jsScopeTimer kTimer_##name(JS_APIENTRY_TIMER_NAME(name).uiElapsed);

#define JS_APIENTRY_TIMER_HITCOUNT(name) (JS_APIENTRY_TIMER_NAME(name).uiHitCount)
#define JS_APIENTRY_TIMER_VALUE(name) (((double)JS_APIENTRY_TIMER_NAME(name).uiElapsed) * (_PerformanceResolution))
#define JS_APIENTRY_TIMER_RESET(name) \
  JS_APIENTRY_TIMER_NAME(name).uiElapsed = 0;\
  JS_APIENTRY_TIMER_NAME(name).uiHitCount = 0;
#else
#define JS_APIENTRY_TIMER(name)
#define JS_APIENTRY_TIMER_SKIP_OVERALL(name)
#endif

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration that encloses file.
#endif

#ifndef PSGLT_UNUSED
#ifdef MSVC
#define JS_UNUSED(value) value;
#else
#define JS_UNUSED(value) do { \
    __typeof__(value) _jsUnused = value; \
    (void)_jsUnused; \
    } while(false)
#endif
#endif

#endif // _JSUtils_h
