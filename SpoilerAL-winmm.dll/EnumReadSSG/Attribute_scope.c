#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <regex.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_list.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"
#include "HashBytes.h"
#include "intrinsic.h"
#include "TMainForm.h"

EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

#define HAS_ORDER(e) ((e)->type & (atREPLACE | atENABLED | atSCOPE))

int AttributeElementOrder = 0;

static TSSGAttributeElement*(__cdecl * const TSSGAttributeSelector_MakeOnlyOneAtteribute)(TSSGAttributeSelector*, TSSGAttributeElement *) = (void *)0x004D5764;

TSSGAttributeElement* __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(TSSGAttributeSelector *this, TSSGAttributeElement *AElem)
{
	if (HAS_ORDER(AElem))
	{
		((TAdjustmentAttribute *)AElem)->elemOrder = ++AttributeElementOrder;	// string's padding (except atSCOPE)
	}
	return TSSGAttributeSelector_MakeOnlyOneAtteribute(this, AElem);
}

static int compareAttributeElement(LPCVOID A, LPCVOID B)
{
	TAdjustmentAttribute *a = *(TAdjustmentAttribute **)A;
	TAdjustmentAttribute *b = *(TAdjustmentAttribute **)B;
	int diff = a->type - b->type;
	return diff ? diff : HAS_ORDER(a) ? a->elemOrder - b->elemOrder : 0;
}

static void(__cdecl * const list_vector_push_back)(list *, vector **) = (void *)0x004D5FBC;

void __cdecl allAtteributeVecList_push_back(list * allAtteributeVecList, vector ** NewVec)
{
	vector *vec = *NewVec;
	qsort(vector_begin(vec), vector_size_by_type(vec, void *), sizeof(void *), compareAttributeElement);
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
	THeapAdjustmentAttribute *heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	heap->type = atSCOPE;
	heap->super.adjustVal = (intptr_t)heap;// guarantee unique
	return TSSGAttributeSelector_AddElement(attributeSelector, heap);
}

void __stdcall Attribute_scope_open(TSSGCtrl *this, string *code)
{
	string tag, Token;
	vector_string tmpV;

	ReplaceDefine(TSSGCtrl_GetAttributeSelector(this), code);
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute *heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	heap->type = atSCOPE;
	heap->super.adjustVal = (intptr_t)heap;// guarantee unique
	TSSGAttributeSelector_AddElement(&this->attributeSelector, heap);

	string_ctor_assign_cstr_with_length(&Token, ";", 1);
	TStringDivision_Half(&tag, &this->strD, code, Token, 0, 0);
	if (string_at(&tag, 0) != ';')
		string_assign(code, &tag);
	string_dtor(&tag);

	vector_ctor(&tmpV);
	string_ctor_assign_cstr_with_length(&Token, ",", 1);
	TStringDivision_List(&this->strD, code, Token, &tmpV, 12);
	for (string* tmpS = (string*)vector_begin(&tmpV); tmpS < (string*)vector_end(&tmpV); ++tmpS) {
		string_ctor_assign_cstr_with_length(&Token, "=", 1);
		TStringDivision_Half(&tag, &this->strD, tmpS, Token, 0, 12);
		if (!string_empty(&tag) & !string_empty(tmpS)) {
			BOOL hasVal = string_at(&tag, 0) != '=';
			string* var = hasVal ? &tag : tmpS;
			LPCSTR data = string_c_str(var);
			size_t size = string_length(var);
			if (data[0] == SCOPE_PREFIX) {
				++data;
				--size;
			}
			heapMapValue val = { HashBytes(data, size), 0, 0 };
			map_iterator it = map_lower_bound(&heap->heapMap, &val.key);
			map_insert(&it, &heap->heapMap, it, &val);
			if (hasVal) {
				const char *nptr, *p;
				char       *endptr, c;

				nptr = string_c_str(tmpS);
				while (__intrinsic_isspace(*nptr))
					nptr++;
				errno = 0;
				*(uint64_t *)&it->first[sizeof(val.key)] = _strtoui64(nptr, &endptr, 0);
				do	/* do { ... } while (0); */
				{
					if (errno != ERANGE)
						switch (*endptr)
						{
						case 'P':
						case 'p':
							if ((c = (p = nptr)[0]) == '0' || ((c == '+' || c == '-') && (p++)[1] == '0'))
								if ((c = p[1]) == 'x' || c == 'X')
									break;
								else
									continue;
						default:
							if (nptr[0] != '-' || *(int64_t *)&it->first[sizeof(val.key)] <= 0)
								continue;
						case '.':
						case 'E':
						case 'e':
							break;
						}
					*(double *)&it->first[sizeof(val.key)] = strtod(nptr, NULL);
				} while (0);
			}
		}
		string_dtor(&tag);
	}
	if (heap->heapMap._M_node_count < vector_size_by_type(&tmpV, string))
		TMainForm_Guide(string_c_str(code), FALSE);
	vector_string_dtor(&tmpV);
}

void __stdcall Attribute_scope_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atSCOPE);
}
