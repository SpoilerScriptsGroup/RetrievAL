#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_stack.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

static char const stFileNameCH[] = "<lexical>";
bcb6_std_string const stFileName = {
	(LPSTR)stFileNameCH,
	(LPSTR)stFileNameCH + _countof(stFileNameCH) - 1,
	NULL,
	NULL,
	stFileNameCH,
	MAXDWORD,
};

__inline void Attribute_offset_open(TSSGCtrl *this, string *code, TSSDir *ctx)
{
	TReplaceAttribute *replace = new_TReplaceAttribute();
	ReplaceDefine(&this->attributeSelector, code);
	TReplaceAttribute_Setting(replace, &this->strD, string_c_str(code));
	if (string_equals(&replace->fileName, &stFileName))
	{
		replace->context = (TSSGSubject *)ctx;
		for (list_iterator SIt = list_end(TSSGCtrl_GetAttributeSelector(this)->nowAttributeList);
			 list_iterator_decrement(SIt) != list_end(TSSGCtrl_GetAttributeSelector(this)->nowAttributeList);)
			if (TSSGAttributeElement_GetType(*(TSSGAttributeElement **)SIt->_M_data) == atREPLACE)
				SIt = list_erase(SIt);
	}
	TSSGAttributeSelector_AddElement(&this->attributeSelector, replace);
}

__inline void Attribute_offset_close(TSSGCtrl *this)
{
	static DWORD const key = atREPLACE;
	map_iterator MIt = bcb6_std_map_find(TSSGCtrl_GetAttributeSelector(this)->stackElemMap, &key);
	if (MIt != map_end(TSSGCtrl_GetAttributeSelector(this)->stackElemMap))
	{
		pdeque second = stack_Get_c(pair_second_aligned(MIt, key));
		if (!deque_empty(second))
		{
			deque_iterator it = deque_end(second);
			deque_iterator_decrement(&it, void *);
			TReplaceAttribute *replace = *(void **)it._M_cur;
			if (replace->context) while (deque_iterator_decrement(&it, void *), !deque_iterator_less_than(&it, &deque_begin(second)))
			{
				list_dword_push_back(TSSGCtrl_GetAttributeSelector(this)->nowAttributeList, it._M_cur);
				if ((*(TReplaceAttribute **)it._M_cur)->context) break;
			}
		}
	}
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
}
