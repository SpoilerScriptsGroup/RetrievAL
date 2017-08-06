#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TIO_FEPAttribute.h"
#include "TSSGAttributeSelector.h"

#define AT_DEFINE 0x1000

extern HANDLE hHeap;

void __stdcall Attribute_define(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	LPCSTR           key;
	size_t           keyLength;
	TIO_FEPAttribute *defineElement;

	if (code->_M_finish == code->_M_start)
		return;

	key = prefix->_M_start + 8;
	while (__intrinsic_isspace(*key))
		key++;
	keyLength = prefix->_M_finish - key;
	if (keyLength == 0)
		return;

	for (TIO_FEPAttribute **it = SSGCtrl->attributeSelector.nowAttributeVec->_M_start, **end = SSGCtrl->attributeSelector.nowAttributeVec->_M_finish; it < end; it++)
	{
		if ((*it)->type != AT_DEFINE)
			continue;
		if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
			continue;
		if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
			continue;
		bcb6_std_string_assign(&(*it)->outputCode, code);
		return;
	}

	if (parent->attribute)
	{
		for (TIO_FEPAttribute **it = parent->attribute->_M_start, **end = parent->attribute->_M_finish; it < end; it++)
		{
			if ((*it)->type != AT_DEFINE)
				continue;
			if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
				continue;
			if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
				continue;
			bcb6_std_string_assign(&(*it)->outputCode, code);
			return;
		}
	}

	defineElement = new_TIO_FEPAttribute();
	if (defineElement)
	{
		defineElement->type = AT_DEFINE;
		bcb6_std_string_allocate(&defineElement->inputCode, keyLength + 2);
		*defineElement->inputCode._M_start = '{';
		memcpy(defineElement->inputCode._M_start + 1, key, keyLength);
		defineElement->inputCode._M_finish = defineElement->inputCode._M_start + keyLength + 2;
		*(LPWORD)(defineElement->inputCode._M_finish - 1) = BSWAP16('}\0');
		bcb6_std_string_assign(&defineElement->outputCode, code);
		TSSGAttributeSelector_AddElement(&SSGCtrl->attributeSelector, defineElement);
	}
}
