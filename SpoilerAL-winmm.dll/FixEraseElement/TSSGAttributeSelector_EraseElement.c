#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeSelector.h"
#include "TSSGAttributeElement.h"
#include "bcb6_std_deque.h"
#include "bcb6_std_list.h"

void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *attributeSelector, TSSGAttributeElement *AElem)
{
	map_iterator MIt = map_find(attributeSelector->stackElemMap, &AElem->type);
	if (MIt == map_end(attributeSelector->stackElemMap))
		return;
	deque *second = (deque *)pair_second(MIt, unsigned long);
	if (second->_M_finish._M_cur == second->_M_start._M_cur)
		return;
	deque_iterator it = second->_M_finish;
	for (deque_iterator_decrement(&it); deque_iterator_greater_or_equal(&it, &second->_M_start); deque_iterator_decrement(&it))
	{
		if (!TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)it._M_cur))
			continue;
		deque_erase_element_size_4(second, &it);
		if (second->_M_finish._M_cur == second->_M_start._M_cur)
			map_erase(attributeSelector->stackElemMap, MIt);
		for (list_iterator *SIt = list_begin(attributeSelector->nowAttributeList); SIt != list_end(attributeSelector->nowAttributeList); list_iterator_increment(SIt))
		{
			if (!TSSGAttributeElement_IsEqual(AElem, (TSSGAttributeElement *)SIt->_M_node->_M_data))
				continue;
			list_erase(SIt);
			break;
		}
		break;
	}
	TSSGAttributeSelector_MakeNowAttributeVec(attributeSelector);
}
