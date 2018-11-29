#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

void __stdcall Attribute_offset_open(TSSGCtrl *this, string *code)
{
	TReplaceAttribute *replace = bcb6_operator_new(sizeof(TReplaceAttribute));
	replace->VTable = (void *)0x00640390;
	replace->type = atREPLACE;
	string_ctor(&replace->offsetCode);
	string_ctor(&replace->fileName);
	replace->offsetNum = 0;
	ReplaceDefine(&this->attributeSelector, code);
	TReplaceAttribute_Setting(replace, &this->strD, string_c_str(code));
	TSSGAttributeSelector_AddElement(&this->attributeSelector, replace);
}

void __stdcall Attribute_offset_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
}
