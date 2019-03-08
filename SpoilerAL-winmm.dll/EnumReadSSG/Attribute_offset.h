#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

__inline void Attribute_offset_open(TSSGCtrl *this, string *code)
{
	TReplaceAttribute *replace = new_TReplaceAttribute();
	ReplaceDefine(&this->attributeSelector, code);
	TReplaceAttribute_Setting(replace, &this->strD, string_c_str(code));
	TSSGAttributeSelector_AddElement(&this->attributeSelector, replace);
}

__inline void Attribute_offset_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
}
