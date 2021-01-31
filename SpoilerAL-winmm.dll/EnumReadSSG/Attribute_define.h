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

__inline void Attribute_define(TSSGCtrl *this, LPVOID ParentStack, LPCSTR Line, LPCSTR EndOfLine)
{
	#define stack_PTSSDir_top(Stack) stack_top((stack *)(Stack), TSSDir *)[0]

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

	if (attribute = TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector))
	{
		signed rel;
		const COORD coord = TSSGAttributeElement_GetViaCoord(atDEFINE, attribute).dwFontSize;
		for (TAdjustmentAttribute **cur,
			 **base = &vector_type_at(attribute, TAdjustmentAttribute *, coord.Y),
			 **apex = base + coord.X;
			 base < apex; )
		{
			cur = &base[apex - base >> 1];
			rel = strncmp((*cur)->c_str + 1, key, keyLength);
			if (rel > 0)
				apex = cur;
			else if (rel == 0 && (*cur)->seqElement == keyLength + 2)
			{
				string_assign_cstr_with_length(&((TDefineAttribute *)*cur)->outputCode, value, valueLength);
				return;
			}
			else
				base = cur + 1;
		}
	}

	if (define = new_TIO_FEPAttribute())
	{
		define->type = atDEFINE;
		string_reserve(&define->inputCode, keyLength + 2);
		*string_begin(&define->inputCode) = '{';
		memcpy(string_begin(&define->inputCode) + 1, key, keyLength);
		string_at(&define->inputCode, 1 + keyLength) = '}';
		*(string_end(&define->inputCode) = string_begin(&define->inputCode) + keyLength + 2) = '\0';
		string_assign_cstr_with_length(&define->outputCode, value, valueLength);
		TSSGAttributeSelector_AddElement(&this->attributeSelector, define);
	}

	#undef stack_PTSSDir_top
}
