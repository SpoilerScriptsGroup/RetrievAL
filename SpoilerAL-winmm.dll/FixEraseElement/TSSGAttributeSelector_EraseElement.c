#include <windows.h>
#include "intrinsic.h"
#include "TSSGAttributeSelector.h"
#include "TSSGAttributeElement.h"
#include "bcb6_std_deque.h"
#include "bcb6_std_list.h"

void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *attributeSelector, TSSGAttributeElement *AElem)
{
	bcb6_std_map_iterator MIt = bcb6_std_map_find(attributeSelector->stackElemMap, &AElem->type);
	if (MIt == bcb6_std_map_end(attributeSelector->stackElemMap))
		return;
	bcb6_std_deque *second = (bcb6_std_deque *)bcb6_std_pair_second(MIt, unsigned long);
	if (second->_M_finish._M_cur == second->_M_start._M_cur)
		return;
	bcb6_std_deque_iterator it = second->_M_finish;
	for (bcb6_std_deque_iterator_decrement(&it); bcb6_std_deque_iterator_greater_or_equal(&it, &second->_M_start); bcb6_std_deque_iterator_decrement(&it))
	{
		if (!TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)it._M_cur))
			continue;
		bcb6_std_deque_erase_element_size_4(second, &it);
		if (second->_M_finish._M_cur == second->_M_start._M_cur)
			bcb6_std_map_erase(attributeSelector->stackElemMap, MIt);
		for (bcb6_std_list_iterator *SIt = bcb6_std_list_begin(attributeSelector->nowAttributeList); SIt != bcb6_std_list_end(attributeSelector->nowAttributeList); bcb6_std_list_iterator_increment(SIt))
		{
			if (!TSSGAttributeElement_IsEqual(AElem, (TSSGAttributeElement *)SIt->_M_node->_M_data))
				continue;
			bcb6_std_list_erase(SIt);
			break;
		}
		break;
	}
	TSSGAttributeSelector_MakeNowAttributeVec(attributeSelector);
}
