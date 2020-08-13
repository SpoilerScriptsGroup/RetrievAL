#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeSelector.h"
#include "TSSGAttributeElement.h"
#include "bcb6_std_stack.h"
#include "bcb6_std_list.h"

extern void __fastcall TSSGAttributeSelector_AESet_erase(TSSGAttributeSelector *attributeSelector, void *AElem);

void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *attributeSelector, TDefineAttribute *AElem)
{
	map_iterator MIt = map_find(attributeSelector->stackElemMap, (LPDWORD)&AElem->type);
	if (MIt != map_end(attributeSelector->stackElemMap))
	{
		deque *second = stack_Get_c(pair_second_aligned(MIt, AElem->type));
		if (!deque_empty(second))
		{
			for (deque_iterator it = deque_end(second);
				 deque_iterator_decrement(&it, void *), !deque_iterator_less_than(&it, &deque_begin(second));
				 )
			{
				if (!TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)it._M_cur))
					continue;
				for (list_iterator SIt = list_end(attributeSelector->nowAttributeList);
					 list_iterator_decrement(SIt) != list_end(attributeSelector->nowAttributeList);
					 )
				{
					if (!TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)SIt->_M_data))
						continue;
					list_erase(SIt);
					TSSGAttributeSelector_AESet_erase(attributeSelector, AElem);
					deque_dword_erase(second, &it);
#ifdef PURGE_WHEN_EMPTY
					if (deque_empty(second))
						map_erase(attributeSelector->stackElemMap, MIt);
#endif
					TSSGAttributeSelector_MakeNowAttributeVec(attributeSelector);
					break;
				}
				break;
			}
		}
	}
}
