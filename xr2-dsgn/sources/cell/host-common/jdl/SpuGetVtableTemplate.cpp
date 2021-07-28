%(/*)
#########################################################
# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################
%(*/)\
$(copyrightSourceHeader)

#include <cell/jdl.h>
#include <cell/swcache.h>
#include "./$(managedFilePrefix)$(class)_ClassIdTable.h"

#include "$(class)$(userDefinedHeaderPostfix).h"

///////////////////////////////////////////////////////////////////////////////
//
//  JDL GENERATED FILE - DO NOT EDIT THIS FILE DIRECTLY
// 
///////////////////////////////////////////////////////////////////////////////
cell::swcache::Vtab *g_BuildVtable_$(class)(unsigned short id, cell::swcache::Vtab *pVtab);
cell::swcache::Vtab *g_BuildVtable_$(class)(unsigned short id, cell::swcache::Vtab *pVtab)
{
	switch(id)
	{
%(FOR_TOKENS:VirtualClassList)\
%( IF_NOT_EXIST:$(TOKEN)_mangle)\
%(  ERROR) mangled symbol for class $(TOKEN) in %(MAKE_SYMBOL)$(TOKEN)%(END)mangle is not specified%(END)\
%( END)\
	case %(IF_EXIST:namespace)$(namespace)::%(END)ClassIdTabFor_$(class)::id_%(MAKE_SYMBOL)$(TOKEN)%(END):
		return cell::swcache::ClassIdTabHeader::buildVtable(
			%(IF_EXIST:namespace)$(namespace)::%(END)ClassIdTabFor_$(class)::id_%(MAKE_SYMBOL)$(TOKEN)%(END),
			pVtab, sizeof($(TOKEN)),
%( FOR_METHODS)%(IF_EXIST:cppClass)%(IF_EQUAL:ppuCall:0)%(IF_EXIST:M.EXTRA.virtual)\
%(  DEFINE:M.className)%(IF_EXIST:namespace)$(namespace)::%(END)$(cppClass)%(END)\
%(  IF_EQUAL:M.className:$(TOKEN))\
		   (void *)($(M.TYPE) ($(M.className)::*)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%( FOR_PARAMS)\
%(  IF_NOT_EQUAL:P.NAME:_pthis_)\
%(   PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%(  END)\
%( END))%(IF_EXIST:M.EXTRA.const) const%(END))&$(M.className)::$(M.NAME),
		   ($(M.TYPE) ($(M.className)::*)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%( FOR_PARAMS)\
%(  IF_NOT_EQUAL:P.NAME:_pthis_)\
%(   PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%(  END)\
%( END))%(IF_EXIST:M.EXTRA.const) const%(END))&$(M.className)::$(M.NAME),
%(  END)\
%( END)%(END)%(END)%(END)\
			0);
%(END)\
	default:
		JDL_ASSERT(0 && "undefined class id is passed to g_BuildVtable_$(class)");
		break;
	}
	return 0;
}

		
