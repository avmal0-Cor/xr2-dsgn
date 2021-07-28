/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: shader.cc
 * Description:
 *
 */

#define CELL_GCM_DENY_IMPLICIT_ARG

#include <stdio.h>
#include <cell/gcm.h>

#include "resc_internal.h"
#include "shader.h"

using namespace cell::Gcm;

namespace cell {
namespace Resc {
namespace Shader {

char  CShaderBase::s_placementBuffer[PLACEMENT_BUFFER_SIZE];
char *CShaderBase::s_pInstanceBuffer = CShaderBase::s_placementBuffer;

// ============================================================================
// CShaderBase  implementation
// ============================================================================
CShaderBase* CShaderBase::Create(const RescShader_t type, const CGprogram prog)
{
	char* pBuf;

	switch (type) {
	case CG_PROGRAM_FRAGMENT:
		pBuf = s_pInstanceBuffer;
		s_pInstanceBuffer += sizeof(CFragmentShader);
		if(s_pInstanceBuffer - s_placementBuffer > PLACEMENT_BUFFER_SIZE) {
			//RESC_DEBUG_PRINTF("No more space to create CFragmentShader instance!\n");
			return NULL;
		}
		return new(pBuf) CFragmentShader(prog);
	case CG_PROGRAM_VERTEX:
	default:
		pBuf = s_pInstanceBuffer;
		s_pInstanceBuffer += sizeof(CVertexShader);
		if(s_pInstanceBuffer - s_placementBuffer > PLACEMENT_BUFFER_SIZE) {
			//RESC_DEBUG_PRINTF("No more space to create CVertexShader instance!\n");
			return NULL;
		}
		return new(pBuf) CVertexShader(prog);
	}
}

void CShaderBase::Destroy(const CShaderBase* const shader)
{
	if(shader){
		shader->~CShaderBase();
		//delete shader;
	}
}

CShaderBase::CShaderBase(const CGprogram prog) :
	m_program(prog),
	m_pUcodeEA(0),
	m_ucodeSize(0)
{
	//RESC_DEBUG_PRINTF("CShaderBase::CShaderBase prog = %x\n", prog);
	GcmCgTypePrefix::cellGcmCgInitProgram(prog);
	GcmCgTypePrefix::cellGcmCgGetUCode(prog, &m_pUcodeEA, &m_ucodeSize);
}

CShaderBase::~CShaderBase(void)
{
	return;
}

// ============================================================================
// CVertexShader  implementation
// ============================================================================
CVertexShader::CVertexShader(const CGprogram prog) :
	CShaderBase(prog)
{
	//RESC_DEBUG_PRINTF2("CVertexShader::CVertexShader is called   prog = 0x%p\n", prog);
	//E nothing to do here
}

void CVertexShader::Bind(CellGcmContextData* con)
{
	GcmCmdTypePrefix::cellGcmSetVertexProgram(con, m_program, m_pUcodeEA);
}

// ============================================================================
// CFragmentShader  implementation
// ============================================================================
CFragmentShader::CFragmentShader(const CGprogram prog) :
	CShaderBase(prog),
	m_ucodeOffset((uint32_t)-1)
{
	//RESC_DEBUG_PRINTF2("CFragmentShader::CFragmentShader is called   prog = 0x%p\n", prog);
	//E nothing to do here
}

uint32_t CFragmentShader::LoadToLocal(const void* const pLoadEA)
{
    if (m_program && m_ucodeSize) {
		_memcpy(const_cast<void*>(pLoadEA), m_pUcodeEA, m_ucodeSize);
		GcmSysTypePrefix::cellGcmAddressToOffset(pLoadEA, &m_ucodeOffset);
		return m_ucodeSize;
	} else {
		return 0;
	}
}

void CFragmentShader::Bind(CellGcmContextData* con)
{
	GcmCmdTypePrefix::cellGcmSetFragmentProgram(con, m_program, m_ucodeOffset);
}


}; /* namespace Shader */
}; /* namespace Resc */
}; /* namespace cell */

