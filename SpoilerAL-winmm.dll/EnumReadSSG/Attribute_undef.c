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
	while ((c = *(++p)) != ']')
	{
		if (__intrinsic_isleadbyte(c))
			c = *(++p);
		if (!c)
			return;
	}
	while ((c = *(--p)) == ' ' || c == '\t');
	keyLength = ++p - key;

	attribute = this->attributeSelector.nowAttributeVec;
	for (TDefineAttribute **it = (TDefineAttribute **)attribute->_M_finish - 1, **end = (TDefineAttribute **)attribute->_M_start - 1; it > end; it--)
	{
		if ((*it)->type != atDEFINE)
			continue;
		if ((*it)->inputCode._M_finish - (*it)->inputCode._M_start != keyLength + 2)
			continue;
		if (memcmp((*it)->inputCode._M_start + 1, key, keyLength) != 0)
			continue;
		TSSGAttributeSelector_EraseElement(&this->attributeSelector, *it);
		break;
	}
}
