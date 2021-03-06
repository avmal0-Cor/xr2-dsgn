%(/*)
#########################################################
# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 320.001
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################
%(*/)\
$(copyrightSourceHeader)

%(DEFINE:_GLOBAL_NS_PREFIX)%(IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))$(TOKEN)::%(END)%(END)%(END)\
///////////////////////////////////////////////////////////////////////////////
//
//  JDL GENERATED FILE - DO NOT EDIT THIS FILE DIRECTLY
// 
///////////////////////////////////////////////////////////////////////////////

#define CELL_JDL_JOB_DESCRIPTOR
#include "$(managedFilePrefix)$(class).h"
#include <cell/jdl.h>


%(IF_EXIST:VirtualClassList)\
%( DEFINE:CID_TAB_NAME)%(IF_EXIST:namespace)$(namespace)::%(END)gClassIdTableFor_$(class)%(END)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
#ifdef __PPU__
ClassIdTabFor_$(class) gClassIdTableFor_$(class) __attribute__((aligned(16)));
#endif
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)%(END)\
%(END)\
%(FOR_METHODS)%(IF_NOT_EQUAL:genJob:0)\

///////////////////////////////////////////////////////////////////////////////
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
%(DEFINE:USE_SWCACHE)0%(END)\
%(IF_EXIST:useSwcache)%(IF_NOT_EQUAL:useSwcache:0)\
%( IF_NOT_EXIST:Swcache)%(ERROR)Swcache implementation is not defined%(END)%(END)\
%( IF_NOT_EXIST:sizeSwcache)%(ERROR)sizeSwcache is not defined%(END)%(END)\
%( DEFINE:USE_SWCACHE)1%(END)\
%(END)%(END)\

%(DEFINE:M.USE_INOUT_BUFFER)0%(END)\
%(FOR_PARAMS)\
%(  IF_EXIST:P.input)\
%(   DEFINE:M.NEED_SERIALIZE)%(END)\
%(  END)\
%(  IF_EXIST:P.out)\
%(   DEFINE:M.NEED_SERIALIZE)%(END)\
%(  END)\
%(  IF_EXIST:P.inout)\
%(   DEFINE:M.USE_INOUT_BUFFER)1%(END)\
%(  END)\
%(  IF_EXIST:P.virtual)\
%(   IF_EQUAL:USE_SWCACHE:0)%(ERROR)virtual attribute is used, but useSwcache is not defined%(END)%(END)\
%(   DEFINE:useVirtual)1%(END)\
%(  END)\
%(END)\
%(DEFINE:M.SIZE_EXPRESSION_IN)\
(0%( FOR_PARAMS)%(IF_EXIST:P.input) + $(PARAM_CONST_LS_SIZE_EXPR)%(END)%(END)\
)%(END)\
%(DEFINE:M.MAX_SIZE_EXPRESSION_IN)\
(0%( FOR_PARAMS)%(IF_EXIST:P.input) + $(PARAM_MAX_LS_SIZE_EXPR)%(END)%(END)\
)%(END)\
%(DEFINE:M.SIZE_EXPRESSION_OUT)\
(0%( FOR_PARAMS)%(IF_EXIST:P.out) + $(PARAM_CONST_LS_SIZE_EXPR)%(END)%(END)\
)%(END)\
%(DEFINE:M.MAX_SIZE_EXPRESSION_OUT)\
(0%( FOR_PARAMS)%(IF_EXIST:P.out) + $(PARAM_MAX_LS_SIZE_EXPR)%(END)%(END)\
)%(END)\
%(DEFINE:M.SIZE_EXPRESSION_SCRATCH)\
(0%(FOR_PARAMS)%(IF_EXIST:P.scratch)%(IF_NOT_EXIST:P.buffer.2)+(($(_P_BUFFER_1)+15)&~15)%(END)%(END)%(END)\
)%(END)\
%(DEFINE:M.MAX_SIZE_EXPRESSION_SCRATCH)\
(0%(FOR_PARAMS)%(IF_EXIST:P.scratch)+(($(_P_BUFFER_1)+15)&~15)%(END)%(END)\
)%(END)\
%(DEFINE:M.NUM_DMA_EXPRESSION_IN)\
(0%( FOR_PARAMS)%(IF_EXIST:P.input) + $(PARAM_DMA_LIST_NUM_EXPR)%(END)%(END)\
)%(END)\

void %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)::$(initHeaderMethodName)(%(IF_EQUAL:ppuCall:0)const $(_GLOBAL_NS_PREFIX)$(class)_info* info%(END))
{
%(IF_EQUAL:ppuCall:0)\
%( IF_EQUAL:spursJobBinary2:1)\
    JDL_ASSERT( *((uint64_t*)info->$(JobFileName_macro)_binaryInfo) != 0 && "$(JobFileName_macro)_binaryInfo is invalid -- was jdl_load_$(class) called?");
	__builtin_memcpy(m.Header.binaryInfo, info->$(JobFileName_macro)_binaryInfo, 10);
	uint32_t* _jobType___ = (uint32_t*)(void*)&m.Header.jobType; *_jobType___ = ((uint32_t)($(spursJobType)) | CELL_SPURS_JOB_TYPE_BINARY2) << 24;
%( ELSE)\
    JDL_ASSERT( (uint32_t)info->$(JobFileName_macro)_eaBinary != 0 && "$(JobFileName_macro)_eaBinary is invalid -- was jdl_load_$(class) called?");
    m.Header.eaBinary = info->$(JobFileName_macro)_eaBinary;
    m.Header.sizeBinary = info->$(JobFileName_macro)_sizeBinary;
	uint32_t* _jobType___ = (uint32_t*)(void*)&m.Header.jobType; *_jobType___ = ((uint32_t)($(spursJobType))) << 24;
%( END)\
%(END)\

    m.Header.sizeDmaList = 8*($(M.NUM_DMA_EXPRESSION_IN));
	m.Header.eaHighInput = 0;
    m.Header.sizeInOrInOut = $(M.MAX_SIZE_EXPRESSION_IN)%(IF_EQUAL:useSwcache:1)%(IF_EQUAL:ppuCall:0) + ($(sizeSwcache))%(END)%(END);
    m.Header.useInOutBuffer = $(M.USE_INOUT_BUFFER);
    m.Header.sizeOut = $(M.MAX_SIZE_EXPRESSION_OUT);
    m.Header.sizeStack = ($(stackSize))/16;
    m.Header.sizeScratch = ($(M.MAX_SIZE_EXPRESSION_SCRATCH))/16;
	m.Header.eaHighCache = 0;
	m.Header.sizeCacheDmaList = 8*($(M.cache.count)%(IF_NOT_EQUAL:useVirtual:0)+1%(END));
%(IF_NOT_EQUAL:useVirtual:0)%(IF_EQUAL:ppuCall:0)\
%( IF_EXIST:VirtualClassList)
#ifdef __PPU__
	$(CID_TAB_NAME).init();
	m.swcacheClassIdTab.eal = (uintptr_t)(&$(CID_TAB_NAME));
    m.swcacheClassIdTab.size = ($(CID_TAB_NAME).getSize()+15)&~15;
#endif
%( ELSE)\
%(  IF_NOT_EXIST:SwcacheClassIdTab)%(ERROR) Variable SwcacheClassIdTab is not defined.%(END)%(END)\
#ifdef __PPU__
    m.swcacheClassIdTab.eal = (uintptr_t)($(SwcacheClassIdTab));
    m.swcacheClassIdTab.size = ($(SwcacheClassIdTab)->getSize()+15)&~15;
#endif
%( END)\
%(END)%(END)\
    uint32_t* _zeros___ = (uint32_t*)(void*)m.Header.__reserved2__; *_zeros___ = 0;
}

#ifndef __SPU__
void %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)::$(setParamsMethodName)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%( END)\
%(END))
#else
unsigned int %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)::$(setParamsMethodName)(%(START_PARAMS)%(END)%(SET_TAB:50:14)%(END)\
unsigned int tag%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM),$(NEWLINE)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%( END)\
%(END))
#endif
{
#ifdef __SPU__
%(IF_NOT_EQUAL:useSwcache:0)\
	/*E libswcache managed pointer handlings */
%( FOR_PARAMS)%(IF_EXIST:P.buffer)\
	unsigned int _dmaTag$(P.IDX_STR);
	uint32_t _ea$(P.IDX_STR);
	int _cacheStat$(P.IDX_STR) = $(Swcache)::query((void *)$(P.NAME), $(_P_BUF_SIZE), _dmaTag$(P.IDX_STR), _ea$(P.IDX_STR));
%( END)%(END)\
	_JdlCacheManagement::flushAll(tag);
%(ELSE)\
	(void)tag;
%(END)\
#endif //__SPU__
    uint32_t _sizeIn = $(M.SIZE_EXPRESSION_IN);
    uint32_t _sizeOut = $(M.SIZE_EXPRESSION_OUT);
    uint32_t _sizeScratch = $(M.SIZE_EXPRESSION_SCRATCH);

%(DEFINE_MACRO:INPUT_OR_CACHE_SIZE_CODE)\
    uint32_t _sizeLeft$(P.IDX_STR) = $(_P_BUF_SIZE);
#ifndef __SPU__
    uint32_t _eal$(P.IDX_STR) = (uintptr_t)$(P.NAME);
#else
    uint32_t _eal$(P.IDX_STR) = %(IF_NOT_EQUAL:useSwcache:0)(_cacheStat$(P.IDX_STR)!=CELL_SWCACHE_ERROR_NON_CACHED_POINTER) ? _ea$(P.IDX_STR) : %(END)(uintptr_t)$(P.NAME);
#endif
    CELL_JDL_CHECK_BUFFER($(JobMethodName_macro),$(P.NAME),$(_P_BUFFER_1),_eal$(P.IDX_STR),_sizeLeft$(P.IDX_STR),$(P.aligned));
%( IF_INT_LESS:P.aligned:dmaPadAlignThreshold)\
    m.$(P.NAME)_addr_offset = (_eal$(P.IDX_STR)&$(_ALIGNSUB1));
    _eal$(P.IDX_STR) &= ~$(_ALIGNSUB1);
    _sizeLeft$(P.IDX_STR) = ((m.$(P.NAME)_addr_offset + _sizeLeft$(P.IDX_STR) + $(_ALIGNSUB1))&~$(_ALIGNSUB1));
%( END)\
%( IF_EXIST:P.swcache)%(IF_EQUAL:ppuCall:0)\
    if (__builtin_expect(_eal$(P.IDX_STR) >= 0x40000, 1))
    {
        _eal$(P.IDX_STR) -= 16; _sizeLeft$(P.IDX_STR) += 16;
	}
%( END)%(END)\
%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.input)%(IF_NOT_EXIST:P.reserve)\
%( CALL_MACRO:INPUT_OR_CACHE_SIZE_CODE)%(END)\
%( IF_EXIST:P.buffer.2)\
    _sizeIn += _sizeLeft$(P.IDX_STR);
%( END)\
#ifdef __SPU__
    serInfo._$(P.NAME)_size = $(_P_BUF_SIZE);
#endif
    for(uint16_t i=0; i<$(PARAM_DMA_LIST_NUM_EXPR); ++i)
    {
		uint32_t _sizeChunk = JDL_MIN(_sizeLeft$(P.IDX_STR),16*1024);
        m.$(P.NAME)[i].eal = _eal$(P.IDX_STR);
#ifdef __SPU__
		m.$(P.NAME)[i].qwLsa = (i == 0 && _eal$(P.IDX_STR) < 0x40000) ? (_eal$(P.IDX_STR)>>4) : 0;
        m.$(P.NAME)[i].size = _sizeChunk;
#else
        m.$(P.NAME)[i].lsaAndSize = _sizeChunk;
#endif
        _eal$(P.IDX_STR) += _sizeChunk;
        _sizeLeft$(P.IDX_STR) -= _sizeChunk;
    }
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.out)%(IF_NOT_EXIST:P.reserve)\
#ifndef __SPU__
    m.$(P.NAME).eal = (uintptr_t)$(P.NAME);
#else
    serInfo._$(P.NAME)_size = $(_P_BUF_SIZE);
    m.$(P.NAME).eal = %(IF_NOT_EQUAL:useSwcache:0)(_cacheStat$(P.IDX_STR)!=CELL_SWCACHE_ERROR_NON_CACHED_POINTER) ? _ea$(P.IDX_STR) : %(END)(uintptr_t)$(P.NAME);
#endif
    uint32_t _$(P.NAME)_size = $(_P_BUF_SIZE); (void)_$(P.NAME)_size;
    CELL_JDL_CHECK_BUFFER($(JobMethodName_macro),$(P.NAME),$(_P_BUFFER_1),m.$(P.NAME).eal,_$(P.NAME)_size,$(P.aligned));
%( IF_INT_LESS:P.aligned:dmaPadAlignThreshold)\
    _$(P.NAME)_size = (((m.$(P.NAME).eal&$(_ALIGNSUB1)) + _$(P.NAME)_size + $(_ALIGNSUB1))&~$(_ALIGNSUB1));
%( END)\
%( IF_EXIST:P.buffer.2)\
    _sizeOut += _$(P.NAME)_size;
%( END)\
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.cache)\
%( CALL_MACRO:INPUT_OR_CACHE_SIZE_CODE)%(END)\
    m.$(P.NAME).eal = _eal$(P.IDX_STR);
    m.$(P.NAME).size = _sizeLeft$(P.IDX_STR);
%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.scratch)\
%( IF_EXIST:P.buffer.2)\
    _sizeScratch += ((($(_P_BUFFER_2))+15)&~15);
%( END)\
%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.reserve)%(IF_EXIST:P.buffer.2)\
%( IF_EXIST:P.input)\
    _sizeIn += (($(_P_BUFFER_2)+15)&~15);
%( END)\
%( IF_EXIST:P.out)\
    _sizeOut += (($(_P_BUFFER_2)+15)&~15);
%( END)\
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_NOT_EXIST:P.buffer)\
    m.$(P.NAME) = $(P.NAME);
%(END)%(END)\

    m.Header.sizeInOrInOut = _sizeIn%(IF_EXIST:InOutReserveSpace)+$(InOutReserveSpace)%(END);
    m.Header.sizeOut = _sizeOut;
    m.Header.sizeScratch = _sizeScratch/16;
%(IF_EQUAL:ppuCall:0)\
    JDL_ASSERT($(spursJobCheckFunction)((const CellSpursJob256 *)(void*)&m.Header, $(jobDescriptorSize), $(jobDescriptorSizeMax)) == CELL_OK &&
               "total size of buffers and binary for $(JobMethodName_macro) is too big");
%(END)\
#ifdef __SPU__
	unsigned int _size = 0;
%(FOR_PARAMS)%(IF_EXIST:P.input)%(IF_NOT_EXIST:P.reserve)\
    if (_eal$(P.IDX_STR) < 0x40000) _size += ($(_P_BUF_SIZE) + (_eal$(P.IDX_STR)&($(dmaPadAlignThreshold))) + $(_ALIGNSUB1))&~$(_ALIGNSUB1);
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.out)%(IF_NOT_EXIST:P.reserve)\
    if (m.$(P.NAME).eal < 0x40000) _size += ($(_P_BUF_SIZE) + (m.$(P.NAME).eal&($(dmaPadAlignThreshold))) + $(_ALIGNSUB1))&~$(_ALIGNSUB1);
%(END)%(END)%(END)\
    return _size;
#endif //__SPU__
}
%(IF_EXIST:M.NEED_SERIALIZE)\
#ifdef __SPU__
/// Serialize job parameters to main memory
void %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)::$(serializeMethodName)(uint32_t eaSerializeMemory, unsigned int sizeSerializeMemory, unsigned int tag)
{
    uint32_t _eaSer = eaSerializeMemory;
    int _sizeSer = sizeSerializeMemory;
%(FOR_PARAMS)%(IF_NOT_EXIST:P.reserve)\
%( IF_EXIST:P.input)\
    if (m.$(P.NAME)[0].qwLsa) {
		void    *_ls = (void*)(m.$(P.NAME)[0].qwLsa<<4);
		uint32_t _ea = _eaSer;
		serInfo._$(P.NAME)_qwLsa = m.$(P.NAME)[0].qwLsa;
		unsigned int _tsize = 0;
		for(uint16_t i=0; i<$(PARAM_DMA_LIST_NUM_EXPR); ++i)
		{
			m.$(P.NAME)[i].eal = _eaSer;
			_eaSer += m.$(P.NAME)[i].size;
			_tsize += m.$(P.NAME)[i].size;
		}
		_jdl_mfc_put_large(_ls, _ea, _tsize, tag, MFC_PUTF_CMD);
		_sizeSer -= _tsize;
		JDL_ASSERT(_sizeSer >= 0 && "Run short of serialize memory");
	} else serInfo._$(P.NAME)_qwLsa = 0;
%( END)\
%( IF_EXIST:P.out)\
    if (m.$(P.NAME).eal > 0 && m.$(P.NAME).eal < 0x40000) {
        $(P.TYPE) $(P.NAME) = ($(P.TYPE))(uintptr_t)m.$(P.NAME).eal;
        uint32_t _$(P.NAME)_size = serInfo._$(P.NAME)_size;
		serInfo._$(P.NAME)_qwLsa = m.$(P.NAME).eal>>4;
		m.$(P.NAME).eal = _eaSer | (m.$(P.NAME).eal&$(dmaPadAlignThreshold));
		_eaSer += (_$(P.NAME)_size + (m.$(P.NAME).eal&$(dmaPadAlignThreshold)) + $(_ALIGNSUB1))&~$(_ALIGNSUB1);
		_sizeSer -= (_$(P.NAME)_size + (m.$(P.NAME).eal&$(dmaPadAlignThreshold)) + $(_ALIGNSUB1))&~$(_ALIGNSUB1);
		JDL_ASSERT(_sizeSer >= 0 && "Run short of serialize memory");
	} else serInfo._$(P.NAME)_qwLsa = 0;
%( END)\
%(END)%(END)\
}

/// Transfer job's output data to corresponding variables in LS.
void %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)::$(getOutputMethodName)(unsigned int tag)
{
%(FOR_PARAMS)%(IF_NOT_EXIST:P.reserve)\
%( IF_EXIST:P.inout)\
    if (serInfo._$(P.NAME)_qwLsa) {
%(  IF_INT_LESS:P.aligned:dmaPadAlignThreshold)\
        $(P.TYPE) $(P.NAME) = ($(P.TYPE))(uintptr_t)m.$(P.NAME)[0].eal; (void)$(P.NAME);
        _jdl_mfc_put_large%(IF_INT_LESS:P.aligned:16)_unaligned%(END)((void*)(uintptr_t)((serInfo._$(P.NAME)_qwLsa<<4)%(IF_INT_LESS:P.aligned:16)|m.$(P.NAME)_addr_offset%(END)), m.$(P.NAME)[0].eal%(IF_INT_LESS:P.aligned:16)|m.$(P.NAME)_addr_offset%(END), serInfo._$(P.NAME)_size, tag, MFC_GET_CMD%(IF_INT_LESS:P.aligned:16), $(P.aligned)%(END));
%(  ELSE)\
        mfc_getl((void*)(uintptr_t)(serInfo._$(P.NAME)_qwLsa<<4), 0, m.$(P.NAME), (($(_P_BUFFER_1)+16*1024-1)/(16*1024))*sizeof(uint64_t), tag, 0, 0);
%(  END)\
	}
%( END)\
%( IF_EXIST:P.out)\
    if (serInfo._$(P.NAME)_qwLsa) {
        $(P.TYPE) $(P.NAME) = ($(P.TYPE))(uintptr_t)m.$(P.NAME).eal;
        _jdl_mfc_put_large%(IF_INT_LESS:P.aligned:16)_unaligned%(END)((void *)(uintptr_t)((((uint32_t)serInfo._$(P.NAME)_qwLsa)<<4)|(m.$(P.NAME).eal&$(dmaPadAlignThreshold))), m.$(P.NAME).eal, serInfo._$(P.NAME)_size, tag, MFC_GET_CMD%(IF_INT_LESS:P.aligned:16), $(P.aligned)%(END));
	}
%( END)\
%(END)%(END)\
%(IF_NOT_EQUAL:useSwcache:0)\
    /*E read back libswcache managed main memory data into local storage */
    _JdlCacheManagement::refreshAll(tag);
%(END)\
}
#endif // __SPU__
%(END)\
%( IF_EXIST:$(M.NAME)_jl_params)%(IF_EXIST:$(M.NAME)_jl_code)\

void $(JobMethodName_macro)_jl::$(setParamsMethodName)($($(M.NAME)_jl_params))
{
	$($(M.NAME)_jl_code)
}
%( END)%(END)\
%(UNDEF:M.NEED_SERIALIZE)%(END)\
%(UNDEF:M.USE_SWCACHE)%(END)\
%(UNDEF:M.SIZE_EXPRESSION_IN)%(END)\
%(UNDEF:M.MAX_SIZE_EXPRESSION_IN)%(END)\
%(UNDEF:M.SIZE_EXPRESSION_OUT)%(END)\
%(UNDEF:M.MAX_SIZE_EXPRESSION_OUT)%(END)\
%(UNDEF:M.SIZE_EXPRESSION_SCRATCH)%(END)\
%(UNDEF:M.MAX_SIZE_EXPRESSION_SCRATCH)%(END)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\
%(END)%(END)\
%(IF_EXIST:VirtualClassList)%(UNDEF:CID_TAB_NAME)%(END)%(END)\

///////////////////////////////////////////////////////////////////////////////

#ifndef __SPU__

%(IF_EQUAL:genPrx:1)\
#include <sys/prx.h>

%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\

static bool $(class)_isLoaded = false;
static sys_prx_id_t $(class)_prxid = 0;
%(ELSE)\
%( FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)\
%(  IF_EQUAL:spursJobBinary2:1)\
extern "C" CellSpursJobHeader _binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_jobbin2_jobheader;
%(  ELSE)\
extern "C" char _binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_bin_start[];
extern "C" char _binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_bin_size[];
%(  END)\
%( END)%(END)%(END)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
%(END)\

int $(class)_load(%(SET_TAB:50:14)%(END)$(class)_info* retInfo,
%(TAB)%(END)const char* path)
{
%(IF_EQUAL:genPrx:1)\
    if(!$(class)_isLoaded)
    {
        // load and start
        $(class)_prxid = sys_prx_load_module(path, 0ull, 0);
        if($(class)_prxid < 0) return (int)$(class)_prxid;
        int modres;
        int res = sys_prx_start_module($(class)_prxid, 0, retInfo, &modres, 0ull, 0);
        if(res < 0) return res;
        $(class)_isLoaded = true;
    }
%(ELSE)\
    (void)path;
%( FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)\
%(  IF_EQUAL:spursJobBinary2:1)\
	__builtin_memcpy(retInfo->$(JobFileName_macro)_binaryInfo, _binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_jobbin2_jobheader.binaryInfo, 10);
%(  ELSE)\
	retInfo->$(JobFileName_macro)_eaBinary = (((uint32_t)_binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_bin_start) | $(cacheInhibit));
	retInfo->$(JobFileName_macro)_sizeBinary = CELL_SPURS_GET_SIZE_BINARY((uint32_t)_binary_$(spuBinaryPrefix)$(managedFilePrefix)$(JobFileName_macro)$(spuBinaryPostfix)_bin_size);
%(  END)\
%( END)%(END)%(END)\
%(END)\
    return 0;
}

int $(class)_unload()
{
%(IF_EQUAL:genPrx:1)\
    if($(class)_isLoaded)
    {
        // stop and unload
        int modres;
        int res = sys_prx_stop_module($(class)_prxid, 0, 0, &modres, 0ull, 0);
        if(res < 0) return res;
        res = sys_prx_unload_module($(class)_prxid, 0ull, 0);
        if(res < 0) return res;
        $(class)_isLoaded = false;
    }
%(END)\
    return 0;
}

%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\

#endif // __SPU__

