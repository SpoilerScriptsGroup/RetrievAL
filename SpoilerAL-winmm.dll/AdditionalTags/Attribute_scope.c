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

EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

#define HAS_ORDER(e) ((e)->type & (atREPLACE | atENABLED | atSCOPE))

static int AttributeElementOrder = 0;

static TSSGAttributeElement*(__cdecl * const TSSGAttributeSelector_MakeOnlyOneAtteribute)(TSSGAttributeSelector*, TSSGAttributeElement *) = (void *)0x004D5764;

TSSGAttributeElement* __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(TSSGAttributeSelector *this, TSSGAttributeElement *AElem)
{
	if (HAS_ORDER(AElem))
	{
		((TAdjustmentAttribute *)AElem)->elemOrder = ++AttributeElementOrder;	// string's padding (except atSCOPE)
		if (AElem->type == atSCOPE)
		{
			TAdjustmentAttribute *scope = (TAdjustmentAttribute *)AElem;
			if (!scope->adjustVal) scope->adjustVal = scope->elemOrder;	// guarantee unique
		}
	}
	return TSSGAttributeSelector_MakeOnlyOneAtteribute(this, AElem);
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
	heap->super.adjustVal = 0;
	return TSSGAttributeSelector_AddElement(attributeSelector, heap);
}

void __stdcall Attribute_scope_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	string tag;
	string_ctor_assign_cstr_with_length(&tag, "heap", 4);
	THeapAdjustmentAttribute *heap = TSSGCtrl_MakeAdjustmentClass(&tag);
	string_dtor(&tag);
	heap->type = atSCOPE;
	heap->super.adjustVal = 0;
	regex_t reg;
	if (!regcomp(&reg, "[[:space:]]*[#@]?([^[:space:]=,;]+)[[:space:]]*=?[[:space:]]*([^[:space:],;]*)[[:space:]]*[,;]?", REG_EXTENDED | REG_NEWLINE))
	{
		regmatch_t m[3];
		ReplaceDefine(&SSGCtrl->attributeSelector, code);
		const char *p = string_c_str(code);
		while (!regexec(&reg, p, _countof(m), m, 0))
		{
			const char *f = p + m[1].rm_so;
			heapMapValue val = { HashBytes(f, p + m[1].rm_eo - f), 0, 0 };
			map_iterator it = map_lower_bound(&heap->heapMap, &val.key);
			map_insert(&it, &heap->heapMap, it, &val);
			if (m[2].rm_so < m[2].rm_eo)
			{
				const char *nptr;
				char       *endptr;

				nptr = p + m[2].rm_so;
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
							if (nptr[0] == '0')
								if (nptr[1] == 'x' || nptr[1] == 'X')
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
			p += m[0].rm_eo;
		}
		regfree(&reg);
	}
	TSSGAttributeSelector_AddElement(&SSGCtrl->attributeSelector, heap);
}

void __stdcall Attribute_scope_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	TSSGAttributeSelector_EraseElementByType(&SSGCtrl->attributeSelector, atSCOPE);
}
