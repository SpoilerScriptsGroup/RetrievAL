#include <windows.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"
#include "TSSGAttributeSelector.h"

void __stdcall Attribute_undef(TSSGCtrl *this, LPCSTR Line)
{
	const char *key, *p;
	char       c;
	size_t     keyLength;
	vector     *attribute;

	assert(*Line != '\0');
	assert(*Line != ' ' && *Line != '\t' && *Line != ']');

	key = p = Line;
	while ((c = *(p++)) != ']')
	{
		if (__intrinsic_isleadbyte(c))
			c = *(p++);
		if (!c)
			return;
	}
	p--;
	while ((c = *(--p)) == ' ' || c == '\t');
	keyLength = ++p - key;

	attribute = this->attributeSelector.nowAttributeVec;
	for (TDefineAttribute **it = (TDefineAttribute **)vector_end(attribute) - 1, **end = (TDefineAttribute **)vector_begin(attribute) - 1; it > end; it--)
	{
		if ((*it)->type != atDEFINE)
			continue;
		if (string_length(&(*it)->inputCode) != keyLength + 2)
			continue;
		if (memcmp(string_c_str(&(*it)->inputCode) + 1, key, keyLength) != 0)
			continue;
		TSSGAttributeSelector_EraseElement(&this->attributeSelector, *it);
		break;
	}
}
