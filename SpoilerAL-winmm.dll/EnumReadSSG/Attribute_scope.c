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

#define HAS_ORDER(e) ((e)->type & (atREPLACE | atENABLED | atSCOPE | atFORMAT))

int AttributeElementOrder = 0;

TSSGAttributeElement*(__cdecl * const TSSGAttributeSelector_MakeOnlyOneAtteribute)(TSSGAttributeSelector*, TSSGAttributeElement *) = (LPVOID)0x004D5764;

TSSGAttributeElement* __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(TSSGAttributeSelector *this, TSSGAttributeElement *AElem)
{
	AElem = TSSGAttributeSelector_MakeOnlyOneAtteribute(this, AElem);
	if (HAS_ORDER(AElem))
	{// store to string's padding2 (except atSCOPE)
		((TAdjustmentAttribute *)AElem)->elemOrder = AttributeElementOrder++;
	}// renumbering when redeclared too
	return AElem;
}

static int compareAttributeElement(LPCVOID A, LPCVOID B)
{
	TAdjustmentAttribute *a = *(TAdjustmentAttribute **)A;
	TAdjustmentAttribute *b = *(TAdjustmentAttribute **)B;
	int diff = a->type - b->type;
	return diff ? diff : HAS_ORDER(a) ? a->elemOrder - b->elemOrder : 0;
}

static void(__cdecl * const list_vector_push_back)(list *, vector **) = (void *)0x004D5FBC;

void __cdecl TSSGAttributeSelector_MakeNowAttributeVec_push_back(list * allAtteributeVecList, vector ** NewVec)
{
	vector *vec = *NewVec;
	qsort(vector_begin(vec), vector_size_by_type(vec, void *), sizeof(void *), compareAttributeElement);
	list_vector_push_back(allAtteributeVecList, NewVec);
}

static char tag[] = "heap";
static string const Tag = { tag, tag + sizeof(tag) - 1, NULL, NULL, tag + sizeof(tag), 0 };

vector * __cdecl TSSGCtrl_ReadSSG_PushElement(TSSGAttributeSelector *attributeSelector, TSSGAttributeElement *NewAElem)
{
	AttributeElementOrder = 0;
	// original TDirAttribute *
	TSSGAttributeSelector_PushElement(attributeSelector, NewAElem);
	// global scope setup
	TScopeAttribute *scope = TSSGCtrl_MakeAdjustmentClass(&Tag);
	scope->type = atSCOPE;
	scope->super.adjustVal = 0;
	return TSSGAttributeSelector_AddElement(attributeSelector, scope);
}

void __stdcall Attribute_scope_open(TSSGCtrl *this, string *code)
{
	string label, Token;
	vector_string tmpV = { NULL };

	ReplaceDefine(TSSGCtrl_GetAttributeSelector(this), code);
	TScopeAttribute *scope = TSSGCtrl_MakeAdjustmentClass(&Tag);
	scope->type = atSCOPE;
	scope->super.adjustVal = -(intptr_t)scope;// guarantee unique

	TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, code, ":", 1, 0, FALSE);
	if (string_at(&label, 0) != ':') {
		LPSTR end;
		uint32_t val = strtoul(string_c_str(&label), &end, 0);
		if (end == string_end(&label)) scope->super.adjustVal = val;
	}
	string_dtor(&label);

	TSSGAttributeSelector_AddElement(&this->attributeSelector, scope);
	scope = *(TScopeAttribute**)list_end(this->attributeSelector.nowAttributeList)->_M_prev->_M_data;

	string_ctor_assign_char(&Token, ',');
	TStringDivision_List(&this->strD, code, Token, &tmpV, etTRIM);
	for (string* tmpS = (string*)vector_begin(&tmpV); tmpS < (string*)vector_end(&tmpV); ++tmpS) {
		TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, tmpS, "=", 1, 0, etTRIM);
		if (!string_empty(&label) && !string_empty(tmpS)) {
			BOOL hasVal = string_at(&label, 0) != '=';
			string* var = hasVal ? &label : tmpS;
			LPCSTR data = string_c_str(var);
			size_t size = string_length(var);
			if (data[0] == SCOPE_PREFIX) {
				++data;
				--size;
			}
			heapMapPair val = { HashBytes(data, size), 0, 0 };
			map_iterator it = map_lower_bound(&scope->heapMap, &val.key);
			if (it == map_end(&scope->heapMap) || *(LPDWORD)pair_first(it) != val.key)
				map_insert(&it, &scope->heapMap, it, &val);
			if (hasVal) {
				const char *nptr, *p;
				char       *endptr, c;

				nptr = string_c_str(tmpS);
				while (__intrinsic_isspace(*nptr))
					nptr++;
				errno = 0;
				*(uint64_t *)pair_second(it, val.key) = _strtoui64(nptr, &endptr, 0);
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
							if (nptr[0] != '-' || *(int64_t *)pair_second(it, val.key) <= 0)
								continue;
						case '.':
						case 'E':
						case 'e':
							break;
						}
					*(double *)pair_second(it, val.key) = strtod(nptr, NULL);
				} while (0);
			}
		}
		string_dtor(&label);
	}
	if (scope->heapMap._M_node_count < vector_size(&tmpV))
		TMainForm_Guide(string_c_str(code), FALSE);
	vector_string_dtor(&tmpV);
}

void __stdcall Attribute_scope_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atSCOPE);
}
