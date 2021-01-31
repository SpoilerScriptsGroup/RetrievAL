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

__inline void Attribute_undef(TSSGCtrl *this, LPCSTR Line)
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
				TSSGAttributeSelector_EraseElement(&this->attributeSelector, *cur);
				return;
			}
			else
				base = cur + 1;
		}
	}
}
