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
%(/*) Error and warning checking %(*/)\
%(FOR_METHODS)\
%( IF_EXIST:M.EXTRA.static)%(IF_EXIST:M.EXTRA.virtual)%(ERROR)Both static and virtual keyword are specified to "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)"%(END)%(END)%(END)\
%( IF_NOT_EXIST:cppClass)\
%(  IF_EXIST:M.EXTRA.virtual)%(ERROR)virtual keyword is specified to non member function "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)" %(END)%(END)\
%(  IF_EXIST:M.EXTRA.static)%(ERROR)static keyword is specified to non class function "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)" %(END)%(END)\
%( END)\
%( IF_EQUAL:ppuCall:0)\
%(  IF_INT_GREATER:$(jobDescriptorSize):$(jobDescriptorSizeMax))%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": jobDescriptorSize($(jobDescriptorSize)) is bigger than jobDescriptorSizeMax($(jobDescriptorSizeMax)). Set jobDescriptorSizeMax = $(jobDescriptorSize) in .jdl file %(END)%(END)\
%( END)\
%( FOR_PARAMS)\
%(  IF_EXIST:P.virtual)%(DEFINE:useVirtual)1%(END)%(END)\
%(  IF_EXIST:P.fence)%(IF_NOT_EXIST:P.output)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): fence can only be used on out or inout buffers%(END)%(END)%(END)\
%(  IF_EXIST:P.scratch)%(IF_EXIST:P.buffer.2)%(WARNING)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): variable sized scratch buffer may degrade performance%(END)%(END)%(END)\
%(  IF_EXIST:P.swcache)%(IF_NOT_EXIST:P.input)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): Alias detect feature cannot be applied to non-input argument%(END)%(END)%(END)\
%(  IF_EXIST:P.manual)\
%(   IF_NOT_EXIST:P.output)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): manual can only be used on out or inout buffers%(END)%(END)\
%(  END)\
%(  IF_EXIST:P.pass_by_ref)\
%(   IF_NOT_EXIST:P.const)%(WARNING)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): pass_by_ref without const, changes on SPU are not applied in main memory%(END)%(END)\
%(   IF_EXIST:P.buffer)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)": parameter $(P.NAME): pass_by_ref is not valid on buffer parameters%(END)%(END)\
%(  END)\
%(  IF_EXIST:P.reserve)\
%(   IF_NOT_EXIST:P.input)%(IF_NOT_EXIST:P.out)\
%(    ERROR)reserve buffer $(P.NAME) must be in, inout or out.%(END)\
%(   END)%(END)\
%(  END)\
%( END)\
%( IF_EQUAL:useVirtual:0)\
%(  IF_INT_GREATER:M.cache.count:4)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)" has too many cached inputs ($(M.cache.count)), max is 4%(END)%(END)\
%( ELSE)\
%(  IF_INT_GREATER:M.cache.count:3)%(ERROR)Job "%(IF_EXIST:namespace)$(namespace)$(COLON)$(COLON)%(END)$(JobMethodName_macro)" has too many cached inputs ($(M.cache.count)), max is 3%(END)%(END)\
%( END)\
%(END)\

#ifndef %(MAKE_SYMBOL)$(_FILENAME_)%(END)
#define %(MAKE_SYMBOL)$(_FILENAME_)%(END)

#include <stdint.h>
#include <cell/spurs/job_descriptor.h>
#ifdef __SPU__
#include <cell/spurs/job_context.h>
#endif
%(IF_EXIST:VirtualClassList)\
#include "$(managedFilePrefix)$(class)_ClassIdTable.h"
#ifdef __PPU__
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
extern ClassIdTabFor_$(class) gClassIdTableFor_$(class);
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)%(END)\
#endif
%(END)\
#include "$(class)$(userDefinedHeaderPostfix).h"

%(IF_NOT_EQUAL:useSwcache:0)\
#ifndef __JDL_CACHE_MANAGEMENT__
#define __JDL_CACHE_MANAGEMENT__
#ifdef __SPU__
#include <cell/swcache/default_cache.h>

class _JdlCacheManagement
{
private:
	static inline void _cacheManagement(int flush, unsigned int tag)
	{
		static int isFlushed = 0;
		if (flush)
		{
			if (!isFlushed)
			{
				$(Swcache)::flushAll(tag);
			}
			isFlushed = 1;
		} else {
			if (isFlushed)
			{
				$(Swcache)::refreshAll(tag);
			}
			isFlushed = 0;
		}
	}

public:
	static inline void flushAll(unsigned int tag) { _cacheManagement(1, tag); }
	static inline void refreshAll(unsigned int tag) { _cacheManagement(0, tag); }
};
#endif //__SPU__
#endif //__JDL_CACHE_MANAGEMENT__
%(END)\

%(FOR_METHODS)%(IF_NOT_EQUAL:genJob:0)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
struct %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix);
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\
%(END)%(END)\

%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\
/// Structure containing pointers/sizes of job binaries.
/// eaBinary is already ORed with cache-inhibit bit when cacheInhibit==1.
/// The sizes are equal to CELL_SPURS_GET_SIZE_BINARY(byteSize).
struct $(class)_info
{
%(FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)%(IF_EQUAL:spursJobBinary2:0)\
    uint32_t $(JobFileName_macro)_eaBinary;
%(END)%(END)%(END)%(END)\
%(FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)%(IF_EQUAL:spursJobBinary2:0)\
    uint16_t $(JobFileName_macro)_sizeBinary;
%(END)%(END)%(END)%(END)\
%(FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)%(IF_EQUAL:spursJobBinary2:1)\
    uint8_t $(JobFileName_macro)_binaryInfo[10];
%(END)%(END)%(END)%(END)\
};

///////////////////////////////////////////////////////////////////////////////

#ifndef __SPU__

/// Loads $(class).sprx.
/// Fills the members of retInfo with the job binary EAs and sizes.
/// @param retInfo Return struct.
/// @param path Path to $(class).sprx.
/// @return 0 if load is successful, -error from PRX API otherwise.
int $(class)_load(%(SET_TAB:50:14)%(END)$(class)_info* retInfo,
%(TAB)%(END)const char* path = "$(prxDefaultPathPrefix)$(class).sprx");

/// Unloads $(class).sprx.
/// @return 0 if unload is successful, -error from PRX API otherwise.
int $(class)_unload();

#endif // !__SPU__

///////////////////////////////////////////////////////////////////////////////
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\

%(FOR_METHODS)%(IF_NOT_EQUAL:genJob:0)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\

%( IF_NOT_EXIST:cppClass)\
%(  IF_NOT_EQUAL:ppuCall:0)\
/// Implemented by user PPU code.
%(  ELSE)\
/// Implemented by user SPU code.
#ifdef __SPU__
%(  END)\
%(  IF_EXIST:M.TYPE)$(M.TYPE)%(ELSE)void%(END) $(M.NAME)$(jobCallbackPostfix)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%(  FOR_PARAMS)\
%(   PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%(  END));
%(  IF_EQUAL:ppuCall:0)\
#endif
%(  END)\
%( END)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\
%(END)%(END)\

#ifdef __SPU__
%(FOR_METHODS)%(IF_EQUAL:ppuCall:0)%(IF_NOT_EQUAL:genJob:0)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\

struct $(JobMethodName_macro)_context
{
    /// SPURS Job Context pointer.
    CellSpursJobContext2* pContext;
    /// Job Descriptor pointer.
    $(JobMethodName_macro)$(jobDescriptorPostfix)* pJob;
    /// DMA tag for asynchronous outputs from out or inout buffers.
    uint32_t getDmaTag() const { return pContext->dmaTag; }
} __attribute__((aligned(16)));

extern $(JobMethodName_macro)_context gContext_$(JobMethodName_macro);
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\
%(END)%(END)%(END)\

#endif // __SPU__

%(FOR_METHODS)%(IF_NOT_EQUAL:genJob:0)\
%( FOR_PARAMS)\
%(  IF_EXIST:P.input)\
%(   DEFINE:M.NEED_SERIALIZE)%(END)\
%(  END)\
%(  IF_EXIST:P.out)\
%(   DEFINE:M.NEED_SERIALIZE)%(END)\
%(  END)\
%( END)\

///////////////////////////////////////////////////////////////////////////////
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))namespace $(TOKEN) { %(END)$(NEWLINE)%(END)\

/// %(MULTILINE:M.DOC)/// %(END)
struct %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)
{
%(IF_EXIST:M.NEED_SERIALIZE)\
#ifdef __SPU__
	struct SerializeInfo
	{
%(FOR_PARAMS)%(IF_NOT_EXIST:P.reserve)\
%( IF_EXIST:P.input)\
		uint32_t _$(P.NAME)_size;
%( END)\
%( IF_EXIST:P.out)\
        uint32_t _$(P.NAME)_size;
%( END)\
%(END)%(END)\
%(FOR_PARAMS)%(IF_NOT_EXIST:P.reserve)\
%( IF_EXIST:P.input)\
		uint16_t _$(P.NAME)_qwLsa;
%( END)\
%( IF_EXIST:P.out)\
        uint16_t _$(P.NAME)_qwLsa;
%( END)\
%(END)%(END)\
	};
#endif //__SPU__
%(END)\
    %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)() {}

#ifndef __SPU__
    /// @param info Struct containing the job binary addresses/sizes
    ///        retrieved from jdl_load_$(class).
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
    /// @param %(SET_TAB:80:16)%(END)$(P.NAME) ($(P.STRING)) %(MULTILINE:P.DOC)    /// %(TAB)%(END)%(END)
%( END)\
%(END)\
    %(IF_NOT_EQUAL:ppuCall:0)$(ppuCallPrefix)%(END)$(JobMethodName_macro)$(jobDescriptorPostfix)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%(IF_EQUAL:ppuCall:0)%(PARAM)%(TAB)%(END)const $(_GLOBAL_NS_PREFIX)$(class)_info* _p$(class)_info%(END)%(END)\
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)$(P.DEFAULT)%(END)\
%( END)\
%(END))
    {
        $(initHeaderMethodName)(%(IF_EQUAL:ppuCall:0)_p$(class)_info%(END));
        $(setParamsMethodName)(%(START_PARAMS),%(END)\
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM)$(P.NAME)%(END)\
%( END)\
%(END));
    }
#endif //!__SPU__

    /// Must be called at least once by application to initialize Header.
    /// Must also be called after reloading job PRX to update.
    /// This function does not change parameters that are set by setParams.
    /// @param info Struct containing the job binary addresses/sizes
    ///        retrieved from jdl_load_$(class).
    void $(initHeaderMethodName)(%(IF_EQUAL:ppuCall:0)const $(_GLOBAL_NS_PREFIX)$(class)_info* info%(END));

    /// Initialize user parameters and buffer data.
#ifndef __SPU__
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
    /// @param %(SET_TAB:80:16)%(END)$(P.NAME) ($(P.STRING)) %(MULTILINE:P.DOC)    /// %(TAB)%(END)%(END)
%( END)\
%(END)\
    void $(setParamsMethodName)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)$(P.DEFAULT)%(END)\
%( END)\
%(END));
#else
%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
    /// @param %(SET_TAB:80:16)%(END)$(P.NAME) ($(P.STRING)) %(MULTILINE:P.DOC)    /// %(TAB)%(END)%(END)
%( END)\
%(END)\
    /// @return Memory size needed to serialize job parameters
    unsigned int $(setParamsMethodName)(%(START_PARAMS)%(END)%(SET_TAB:50:14)%(END)\
unsigned int tag%(FOR_PARAMS)\
%( IF_NOT_EXIST:P.phony)\
%(  PARAM),$(NEWLINE)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)$(P.DEFAULT)%(END)\
%( END)\
%(END));
%(IF_EXIST:M.NEED_SERIALIZE)\

    /// Serialize job parameter to main memory
    void $(serializeMethodName)(uint32_t eaSerializeMemory, unsigned int sizeSerializeMemory, unsigned int tag);

    /// Transfer job's output data to corresponding variables in LS
    void $(getOutputMethodName)(unsigned int tag);
%(END)\
#endif
%(IF_EXIST:M.TYPE)%(IF_NOT_EQUAL:M.TYPE:void)\

    /// After the job has completed, getResult may be called to get the 
    /// return value.
    /// @return Return value of job function.
    $(M.TYPE) getResult() const { return m.__return__value; }

    /// Initialize the result value (optional).
    /// @param result New value for return result.
    void setResult($(M.TYPE) result) { m.__return__value = result; }
%(END)%(END)\

    struct _jd_data {
        CellSpursJobHeader Header;
%(FOR_PARAMS)%(IF_EXIST:P.input)%(IF_NOT_EXIST:P.reserve)\
        struct{ __extension__ union { __extension__ struct { uint32_t qwLsa:17; uint32_t size:15; }; uint32_t lsaAndSize; }; uint32_t eal; } $(P.NAME)[(($(_P_BUFFER_1)+16*1024-1%(IF_INT_LESS:P.aligned:dmaPadAlignThreshold)+2*$(_ALIGNSUB1)%(END))/(16*1024))];
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.cache)\
        struct{ uint32_t size; uint32_t eal; } $(P.NAME);
%(END)%(END)\
%(FOR_PARAMS)\
%( IF_EXIST:P.virtual)%(DEFINE:useVirtual)1%(END)%(END)\
%(END)\
%(IF_NOT_EQUAL:useVirtual:0)%(IF_EQUAL:ppuCall:0)\
        struct{ uint32_t size; uint32_t eal; } swcacheClassIdTab;
%(END)%(END)\
%(FOR_PARAMS)%(IF_EXIST:P.out)%(IF_NOT_EXIST:P.reserve)\
        struct{ uint32_t eal; } $(P.NAME);
%(END)%(END)%(END)\
%(FOR_PARAMS)%(IF_NOT_EXIST:P.buffer)\
        $(P.TYPE) $(P.NAME);
%(END)%(END)\
%(IF_EXIST:M.TYPE)%(IF_NOT_EQUAL:M.TYPE:void)\
        $(M.TYPE) __return__value;
%(END)%(END)\
%(FOR_PARAMS)\
%( IF_INT_LESS:P.aligned:dmaPadAlignThreshold)\
%(  IF_EXIST:P.input)%(IF_NOT_EXIST:P.reserve)\
        uint8_t $(P.NAME)_addr_offset;
%(  END)%(END)\
%(  IF_EXIST:P.cache)\
        uint8_t $(P.NAME)_addr_offset;
%(  END)\
%( END)\
%(END)\
    } m;
    char _skip[(%(IF_EQUAL:ppuCall:0)$(jobDescriptorSize)%(ELSE)$(ppuJobDescriptorSize)%(END)) - sizeof(_jd_data) - ($(jobDescriptorReservedSpace))];
    char _reserved[$(jobDescriptorReservedSpace)];
%(IF_EXIST:M.NEED_SERIALIZE)\
#ifdef __SPU__
	struct SerializeInfo serInfo;
#endif //__SPU__
%(END)\
}%(IF_EXIST:jobDescriptorAlignment) __attribute__((__aligned__($(jobDescriptorAlignment))))%(END);
%( IF_EXIST:$(M.NAME)_jl_params)%(IF_EXIST:$(M.NAME)_jl_code)\
%(  IF_NOT_EQUAL:ppuCall:0)%(ERROR)$(M.NAME): ppuCall jobs can not be in job lists%(END)%(END)\

%( IF_EXIST:$(M.NAME)_jl_doc)\
$($(M.NAME)_jl_doc)
%(  END)\
struct $(JobMethodName_macro)_jl : public CellSpursJobList
{
    /// Initialize CellSpursJobList structure and static data in array of jobs.
    /// @param jobsArray Pointer to array of jobs.
    /// @param numJobs Number of jobs in jobsArray.
    /// @param jobInfo Job binary addresses.
    void $(initHeaderMethodName)($(JobMethodName_macro)$(jobDescriptorPostfix)* jobsArray, uint32_t numJobs, const $(_GLOBAL_NS_PREFIX)$(class)_info* jobInfo)
    {
        this->numJobs = numJobs;
        this->sizeOfJob = sizeof($(JobMethodName_macro)$(jobDescriptorPostfix));
        this->eaJobList = (uint64_t)(uint32_t)jobsArray;
        for(uint32_t iJob=0; iJob<numJobs; ++iJob)
        {
            jobsArray[iJob].$(initHeaderMethodName)(jobInfo);
        }
    }

%( IF_EXIST:$(M.NAME)_jl_params_doc)\
    $($(M.NAME)_jl_params_doc)
%(  END)\
    void $(setParamsMethodName)($($(M.NAME)_jl_params));

    $(JobMethodName_macro)$(jobDescriptorPostfix)* getJobs() { return ($(JobMethodName_macro)$(jobDescriptorPostfix)*)(uint32_t)eaJobList; }

    #ifdef __SPU__
    /// For SPU usage, call setEaJobs after $(initHeaderMethodName) and $(setParamsMethodName).
    /// Then the CellSpursJobList and array of Job Descriptors can be transfered to main memory.
    /// @param eaJobs Effective address of Job Descriptor array.
    void setEaJobs(uint32_t eaJobs)
    {
        this->eaJobList = (uint64_t)eaJobs;
    }
    #endif
};
%( END)%(END)\
%( IF_EXIST:namespace)%(FOR_TOKENS:namespace:$(COLON)$(SPACE))} %(END)// namespace$(NEWLINE)%(END)\
%( UNDEF:M.NEED_SERIALIZE)%(END)\
%(END)%(END)\

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#endif // %(MAKE_SYMBOL)$(_FILENAME_)%(END)
