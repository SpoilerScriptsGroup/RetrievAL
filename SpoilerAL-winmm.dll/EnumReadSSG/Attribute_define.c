#include <windows.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_stack.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSDir.h"
#include "TSSGAttributeElement.h"
#include "TSSGAttributeSelector.h"

extern HANDLE hHeap;

void __stdcall Attribute_define(TSSGCtrl *this, LPVOID ParentStack, LPCSTR Line, LPCSTR EndOfLine)
{
	#define stack_PTSSDir_top(Stack) ((TSSDir *)stack_dword_top((stack_dword *)Stack))

	const char       *key, *value, *p;
	char             c;
	size_t           keyLength, valueLength;
	TIO_FEPAttribute *define;
	bcb6_std_vector  *attribute;

	assert(*Line != '\0' && Line < EndOfLine);
	assert(*EndOfLine == '\0');
	assert(*Line != ' ' && *Line != '\t' && *Line != ']');

	key = p = Line;
	while ((c = *(++p)) != ']')
	{
		if (__intrinsic_isleadbyte(c))
			c = *(++p);
		if (!c)
			return;
	}
	value = p;
	while ((c = *(--p)) == ' ' || c == '\t');
	keyLength = ++p - key;
	while ((c = *(++value)) == ' ' || c == '\t');
	p = EndOfLine;
	while (--p >= value && ((c = *p) == ' ' || c == '\t'));
	valueLength = ++p - value;
	for (TIO_FEPAttribute **it = this->attributeSelector.nowAttributeVec->_M_start, **end = this->attributeSelector.nowAttributeVec->_M_finish; it < end; it++)
	{
		if ((*it)->type != atDEFINE)
			continue;
		if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
			continue;
		if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
			continue;
		string_assign_cstr_with_length(&(*it)->outputCode, value, valueLength);
		return;
	}

	if (attribute = stack_PTSSDir_top(ParentStack)->super.attribute)
	{
		for (TIO_FEPAttribute **it = attribute->_M_start, **end = attribute->_M_finish; it < end; it++)
		{
			if ((*it)->type != atDEFINE)
				continue;
			if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
				continue;
			if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
				continue;
			string_assign_cstr_with_length(&(*it)->outputCode, value, valueLength);
			return;
		}
	}

	define = new_TIO_FEPAttribute();
	if (define)
	{
		define->type = atDEFINE;
		string_reserve(&define->inputCode, keyLength + 2);
		*define->inputCode._M_start = '{';
		memcpy(define->inputCode._M_start + 1, key, keyLength);
		define->inputCode._M_finish = define->inputCode._M_start + keyLength + 2;
		*(LPWORD)(define->inputCode._M_finish - 1) = BSWAP16('}\0');
		string_assign_cstr_with_length(&define->outputCode, value, valueLength);
		TSSGAttributeSelector_AddElement(&this->attributeSelector, define);
	}
}
