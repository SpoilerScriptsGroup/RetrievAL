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
	TDefineAttribute *define;
	bcb6_std_vector  *attribute;

	assert(*Line != '\0' && Line < EndOfLine);
	assert(*EndOfLine == '\0');
	assert(*Line != ' ' && *Line != '\t' && *Line != ']');

	key = p = Line;
	while ((c = *(p++)) != ']')
	{
		if (__intrinsic_isleadbyte(c))
			c = *(p++);
		if (!c)
			return;
	}
	value = --p;
	while ((c = *(--p)) == ' ' || c == '\t');
	keyLength = ++p - key;
	while ((c = *(++value)) == ' ' || c == '\t');
	p = EndOfLine;
	while (--p >= value && ((c = *p) == ' ' || c == '\t'));
	valueLength = ++p - value;

	for (TDefineAttribute **it = vector_begin(this->attributeSelector.nowAttributeVec), **end = vector_end(this->attributeSelector.nowAttributeVec); it < end; it++)
	{
		if ((*it)->type != atDEFINE)
			continue;
		if (string_length(&(*it)->inputCode) != keyLength + 2)
			continue;
		if (memcmp(string_c_str(&(*it)->inputCode) + 1, key, keyLength) != 0)
			continue;
		string_assign_cstr_with_length(&(*it)->outputCode, value, valueLength);
		return;
	}

	if (attribute = stack_PTSSDir_top(ParentStack)->super.attribute)
	{
		for (TDefineAttribute **it = vector_begin(attribute), **end = vector_end(attribute); it < end; it++)
		{
			if ((*it)->type != atDEFINE)
				continue;
			if (string_length(&(*it)->inputCode) != keyLength + 2)
				continue;
			if (memcmp(string_c_str(&(*it)->inputCode) + 1, key, keyLength) != 0)
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
		*string_begin(&define->inputCode) = '{';
		memcpy(string_begin(&define->inputCode) + 1, key, keyLength);
		string_end(&define->inputCode) = string_begin(&define->inputCode) + keyLength + 2;
		*(LPWORD)(string_end(&define->inputCode) - 1) = BSWAP16('}\0');
		string_assign_cstr_with_length(&define->outputCode, value, valueLength);
		TSSGAttributeSelector_AddElement(&this->attributeSelector, define);
	}
}
