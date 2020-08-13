#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_stack.h"
#include "TSSGCtrl.h"

extern void __stdcall  ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);
extern void __fastcall TSSGAttributeSelector_AESet_erase(TSSGAttributeSelector *this, void *AElem);

__inline void Attribute_offset_open(TSSGCtrl *this, string *code)
{
	TReplaceAttribute *replace = new_TReplaceAttribute();
	ReplaceDefine(&this->attributeSelector, code);
	TReplaceAttribute_Setting(replace, &this->strD, string_c_str(code));
	if (replace->displace = string_size(&replace->fileName) >= 4 && *(LPDWORD)string_begin(&replace->fileName) == BSWAP32('*dir'))
	{
		replace->displace += string_size(&replace->fileName) > 6 && (*(LPDWORD)&string_at(&replace->fileName, 4) & 0x00FFFFFF) == BSWAP32('ect\0');
		for (list_iterator SIt = list_end(this->attributeSelector.nowAttributeList);
			 list_iterator_decrement(SIt) != list_end(this->attributeSelector.nowAttributeList);
			 )
		{
			TSSGAttributeElement* const AElem = *(void **)SIt->_M_data;
			if (TSSGAttributeElement_GetType(AElem) == atREPLACE)
			{
				TSSGAttributeSelector_AESet_erase(&this->attributeSelector, AElem);
				SIt = list_erase(SIt);
			}
		}
	}
	TSSGAttributeSelector_AddElement(&this->attributeSelector, replace);
}

__inline void Attribute_offset_close(TSSGCtrl *this)
{
	static const DWORD key = atREPLACE;
	map_iterator MIt = map_find(this->attributeSelector.stackElemMap, &key);
	if (MIt != map_end(this->attributeSelector.stackElemMap))
	{
		pdeque second = stack_Get_c(pair_second_aligned(MIt, key));
		if (!deque_empty(second))
		{
			deque_iterator it = deque_end(second);
			deque_iterator_decrement(&it, void *);
			TReplaceAttribute *replace = *(void **)it._M_cur;
			if (replace->displace) while (deque_iterator_decrement(&it, void *), !deque_iterator_less_than(&it, &deque_begin(second)))
			{
				list_dword_push_back(this->attributeSelector.nowAttributeList, it._M_cur);
				if ((*(TReplaceAttribute **)it._M_cur)->displace) break;
			}
		}
	}
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
}
