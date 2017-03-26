#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TIO_FEPAttribute.h"
#include "TSSGAttributeSelector.h"
#pragma function(memcmp)

#define AT_DEFINE 0x1000

void __stdcall Attribute_undef(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	LPCSTR key;
	size_t keyLength;

	key = prefix->_M_start + 7;
	while (__intrinsic_isspace(*key))
		key++;
	keyLength = prefix->_M_finish - key;
	if (keyLength == 0)
		return;

	for (TIO_FEPAttribute **it = (TIO_FEPAttribute **)SSGCtrl->attributeSelector.nowAttributeVec->_M_finish - 1, **end = (TIO_FEPAttribute **)SSGCtrl->attributeSelector.nowAttributeVec->_M_start - 1; it > end; it--)
	{
		if ((*it)->type != AT_DEFINE)
			continue;
		if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
			continue;
		if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
			continue;
		TSSGAttributeSelector_EraseElement(&SSGCtrl->attributeSelector, *it);
		break;
	}
}
