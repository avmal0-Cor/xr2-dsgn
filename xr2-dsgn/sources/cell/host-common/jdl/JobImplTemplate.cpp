%(/*)
#########################################################
# SCE CONFIDENTIAL
# PlayStation(R)3 Programmer Tool Runtime Library 310.001
# Copyright (C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.
#########################################################
%(*/)\
// This file is safe to edit.

#include "$(managedFilePrefix)$(class).h"

%(IF_EXIST:namespace)\
%(FOR_TOKENS:namespace:$(COLON)$(SPACE))\
namespace $(TOKEN) {
%(END)\
%(END)\

%(IF_EXIST:M.TYPE)$(M.TYPE)%(ELSE)void%(END) %(IF_EXIST:cppClass)$(cppClass)$(COLON)$(COLON)%(END)\
$(M.NAME)%(IF_NOT_EXIST:cppClass)$(jobCallbackPostfix)%(END)(%(START_PARAMS),$(NEWLINE)%(END)%(SET_TAB:50:14)%(END)\
%( FOR_PARAMS)\
%(  IF_NOT_EQUAL:P.NAME:_pthis_)\
%(   PARAM)%(TAB)%(END)%(IF_EXIST:P.const)const %(END)$(P.TYPE)%(IF_EXIST:P.pass_by_ref)&%(END) $(P.NAME)%(END)\
%(  END)\
%( END))%(IF_EXIST:M.EXTRA.const) const%(END)
{
}

%(IF_EXIST:namespace)\
%(FOR_TOKENS:namespace:$(COLON)$(SPACE))\
}
%(END)\
%(END)\

