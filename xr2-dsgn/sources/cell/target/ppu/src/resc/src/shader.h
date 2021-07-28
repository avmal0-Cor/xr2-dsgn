/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: shader.h
 * Description:
 *
 */

#ifndef __CELL_RESC_SHADER_H__
#define __CELL_RESC_SHADER_H__

#include <Cg/cg.h>
#include <stdint.h>
#include <stdlib.h> // for abort()
#include <new>
#include "resc_internal.h"

namespace cell {
namespace Resc {
namespace Shader {

#define PLACEMENT_BUFFER_SIZE  128    // 76 is enough in sdk130

#define DECLARE_PLACEMENT_NEW_AND_DELETE                                         \
	static void* operator new(size_t size, void* buf){ (void)size; return buf; } \
	static void operator delete(void* p, void* buf){ (void)p; (void)buf; }       \
	static void operator delete(void* p){ (void)p; }                             


// ============================================================================
// CShaderBase  definition (abstruct crass)
// ============================================================================
typedef enum RescShader_t {
	CG_PROGRAM_VERTEX   = 0, 
	CG_PROGRAM_FRAGMENT, 
} RescShader_t;

class CShaderBase
{
protected:
	CGprogram   m_program;
	void*       m_pUcodeEA;
	uint32_t    m_ucodeSize;

	static char s_placementBuffer[PLACEMENT_BUFFER_SIZE], *s_pInstanceBuffer;

public:
	DECLARE_PLACEMENT_NEW_AND_DELETE;

	CShaderBase(CGprogram prog);
	CShaderBase() { abort(); }  // Default is illegal constructor in this class
	virtual ~CShaderBase() = 0;

	CGprogram GetCGprogram(void) const { return m_program; }
	void* GetUcode(void) const { return m_pUcodeEA; }
	uint32_t GetUcodeSize(void) const { return m_ucodeSize; }

	virtual void Bind(CellGcmContextData* con) = 0;
	static CShaderBase* Create(const RescShader_t type, const CGprogram prog);
	static void Destroy(const CShaderBase* const shader);
};

// ============================================================================
// CVertexShader  definition
// ============================================================================
class CVertexShader : public CShaderBase
{
public:
	DECLARE_PLACEMENT_NEW_AND_DELETE;

	CVertexShader(CGprogram prog);
	CVertexShader() : CShaderBase() {}
	~CVertexShader(void) {};
	virtual void Bind(CellGcmContextData* con);
};

// ============================================================================
// CFragmentShader  definition
// ============================================================================
class CFragmentShader : public CShaderBase
{
protected:
	uint32_t m_ucodeOffset;

public:
	DECLARE_PLACEMENT_NEW_AND_DELETE;

	CFragmentShader(CGprogram prog);
	CFragmentShader() : CShaderBase() {}
	~CFragmentShader(void) {};
	uint32_t LoadToLocal(const void* const pLoadEA);
	uint32_t GetUcodeOffset(void) const { return m_ucodeOffset; }
	inline int32_t GetRegisterCount(void) const { return GcmCgTypePrefix::cellGcmCgGetRegisterCount(m_program); }
	inline void SetRegisterCount(int32_t regCount) { GcmCgTypePrefix::cellGcmCgSetRegisterCount(m_program, regCount); }

	virtual void Bind(CellGcmContextData* con);
};

#undef  DECLARE_PLACEMENT_NEW_AND_DELETE
}; /* namespace Shader */
}; /* namespace Resc */
}; /* namespace cell */

#endif /* __CELL_RESC_SHADER_H__ */
