#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_list.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

#define HAS_ORDER(e) (e->type == atREPLACE || e->type == atENABLED || e->type == atSCOPE)

static int AttributeElementOrder = 0;

static void(__cdecl * const TSSGAttributeSelector_PushStack)(TSSGAttributeSelector*, TSSGAttributeElement *) = (void *)0x004D43AC;

void TSSGAttributeSelector_AddElement_PushStack(TSSGAttributeSelector *this, TSSGAttributeElement *AElem)
{
	if (HAS_ORDER(AElem))
	{
		((TAdjustmentAttribute *)AElem)->elemOrder = ++AttributeElementOrder;	// string's padding (except atSCOPE)
		if (AElem->type == atSCOPE)
		{
			TAdjustmentAttribute *scope = (TAdjustmentAttribute *)AElem;
			scope->status = scope->elemOrder;	// guarantee unique
		}
	}
	TSSGAttributeSelector_PushStack(this, AElem);
}

static int compareAttributeElement(LPCVOID A, LPCVOID B)
{
	TSSGAttributeElement *a = *(TSSGAttributeElement **)A;
	TSSGAttributeElement *b = *(TSSGAttributeElement **)B;
	return HAS_ORDER(a) && HAS_ORDER(b) ? ((TAdjustmentAttribute *)a)->elemOrder - ((TAdjustmentAttribute *)b)->elemOrder : 0;
}

static void(__cdecl * const list_vector_push_back)(list *, vector **) = (void *)0x004D5FBC;

void __cdecl allAtteributeVecList_push_back(list * allAtteributeVecList, vector ** NewVec)
{
	vector *vec = *NewVec;
	qsort(vec->_M_start, ((intptr_t)vec->_M_finish - (intptr_t)vec->_M_start) / sizeof(void *), sizeof(void *), compareAttributeElement);
	list_vector_push_back(allAtteributeVecList, NewVec);
}

vector * __cdecl rootAttributeHook(TSSGAttributeSelector *attributeSelector, TSSGAttributeElement *NewAElem)
{
	AttributeElementOrder = 0;
	// original TDirAttribute *
	TSSGAttributeSelector_PushElement(attributeSelector, NewAElem);
	// global scope setup
	string tag;
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute* heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	heap->type = atSCOPE;
	heap->super.checkType = 0;
	heap->super.adjustVal = 0;
	return TSSGAttributeSelector_AddElement(attributeSelector, heap);
}

void __stdcall Attribute_scope_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	string tag;
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute *heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	TAdjustmentAttribute_Setting(heap, &SSGCtl, "");
	heap->type = atSCOPE;
	heap->super.checkType = 0;
	heap->super.adjustVal = 0;
	TSSGAttributeSelector_AddElement(&SSGCtrl->attributeSelector, heap);
}

void __stdcall Attribute_scope_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	TSSGAttributeSelector_EraseElementByType(&SSGCtrl->attributeSelector, atSCOPE);
}
