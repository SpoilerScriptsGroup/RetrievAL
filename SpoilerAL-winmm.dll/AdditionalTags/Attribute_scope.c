#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

vector * __cdecl rootAttributeHook(TSSGAttributeSelector *attributeSelector, TSSGAttributeElement *NewAElem)
{
	TSSGAttributeSelector_PushElement(attributeSelector, NewAElem);
	// global scope setup
	string tag;
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute* heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	heap->type = atSCOPE;
	return TSSGAttributeSelector_PushElement(attributeSelector, heap);
}

void __stdcall Attribute_scope_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	string tag;
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute *heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	TAdjustmentAttribute_Setting(heap, &SSGCtl, "");
	heap->type = atSCOPE;
	TSSGAttributeSelector_PushElement(&SSGCtrl->attributeSelector, heap);
}

void __stdcall Attribute_scope_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	TSSGAttributeSelector_PopElementByType(&SSGCtrl->attributeSelector, atSCOPE);
}
