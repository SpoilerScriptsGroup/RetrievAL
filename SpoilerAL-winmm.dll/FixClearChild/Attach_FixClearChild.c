#include "SubjectStringTable/SubjectStringOperator.h"
#include "bcb6_operator.h"
#include "bcb6_std_deque.h"
#include "bcb6_std_list.h"
#include "TSSDir.h"
#include "TSSGAttributeElement.h"
#include "SSGSubjectProperty.h"

extern BOOL FixTheProcedure;
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);
void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);
void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);
extern DWORD RepeatDepth;
void __stdcall repeat_ReadSSRFile(
	TSSGCtrl *this,
	LPVOID   ParentStack,
	LPVOID   ADJElem,
	string   *LineS,
	DWORD    RepeatIndex,
	DWORD    ParentRepeat);

static void(__cdecl * const TSSGAttributeSelector_StartElementCheck)(TSSGAttributeSelector* this) = (void*)0x004D2D54;
static void(__cdecl * const stack_ptr_ctor)(pdeque this, void* zero) = (void*)0x004E49B0;
static void(__cdecl * const stack_ptr_push)(pdeque this, void** val) = (void*)0x004E4BC0;
static void(__cdecl * const stack_ptr_dtor)(pdeque this, void* zero) = (void*)0x004E4FA8;
static void(__cdecl * const TSSGAttributeSelector_EndElementCheck)(TSSGAttributeSelector* this) = (void*)0x004D3670;

static void __fastcall TSSGCtrl_ChangeDirectorySubject_prepare(TSSGCtrl* SSGC, TSSGSubject* SSD) {
	if (FixTheProcedure) {
		string* code = SubjectStringTable_GetString(&SSD->code);
		if (!string_empty(code)) {
			string Token;
			vector_string List;
			string_ctor_assign_cstr_with_length(&Token, ",", 1);
			vector_ctor(&List);
			if (TStringDivision_List(&SSGC->strD, code, Token, &List, 0) < 2)
				Parsing(SSGC, SSD, code, 0);
			else {
				string LineS;
				deque ParentStack;
				TDirAttribute* NewAElem;
				TSSGSubjectProperty* prop = GetSubjectProperty(SSD);
				bcb6_std_string_ctor_assign(&LineS, code);
				//ReplaceDefineDynamic(SSD, &LineS);
				FormatNameString(SSGC, SSD, &LineS);
				TSSDir_ClearChild((TSSDir*)SSD);
				TSSGAttributeSelector_StartElementCheck(&SSGC->attributeSelector);

				vector* attrs = (vector*)SSD->attribute;
				for (TSSGAttributeElement** pos = (TSSGAttributeElement**)vector_begin(attrs); pos < (TSSGAttributeElement**)vector_end(attrs); ++pos) {
					TSSGAttributeElement* AElem = *pos;
					if (AElem->type == atDIR_LEVEL) continue;
					for (list_iterator *SIt = list_begin(&SSGC->attributeSelector.allAtteributeList);
						 SIt != list_end(&SSGC->attributeSelector.allAtteributeList);
						 list_iterator_increment(SIt)) {
						if (TSSGAttributeElement_IsEqual((TSSGAttributeElement*)SIt->_M_node->_M_data, AElem)) {
							list_erase(SIt);
							break;
						}
					}
					if (AElem->type & (atREPLACE | atENABLED | atDEFINE | atSCOPE))
						TSSGAttributeSelector_AddElement(&SSGC->attributeSelector, AElem);
					else
						TSSGAttributeSelector_PushElement(&SSGC->attributeSelector, AElem);
				}
				NewAElem = bcb6_operator_new(sizeof(TDirAttribute));
				NewAElem->VTable = (void*)0x006403A8;
				NewAElem->type = atDIR_LEVEL;
				NewAElem->level = TSSGCtrl_GetDirLevel(SSGC, SSD) + 1;
				TSSGAttributeSelector_PushElement(&SSGC->attributeSelector, NewAElem);

				stack_ptr_ctor(&ParentStack, 0);
				stack_ptr_push(&ParentStack, &SSD);
				RepeatDepth = prop->RepeatDepth;
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, &LineS, prop->RepeatIndex, prop->ParentRepeat);
				stack_ptr_dtor(&ParentStack, 0);

				TSSGAttributeSelector_EndElementCheck(&SSGC->attributeSelector);
				string_dtor(&LineS);
			}
			vector_string_dtor(&List);
			string_dtor(&Token);
		}
	}
}

static void __declspec(naked) TSSGCtrl_ChangeDirectorySubject_stub() {
	__asm {
		mov  edx, ecx
		mov  ecx, edi
		push 0x004C2D64// GetSubjectVec
		jmp  TSSGCtrl_ChangeDirectorySubject_prepare
	}
}

#define NOP       (BYTE)0x90
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixClearChild()
{
	// TSSDir::FixClearChild
	*(LPBYTE )0x004C2CCC = JMP_REL32;
	*(LPDWORD)0x004C2CCD = (DWORD)TSSDir_ClearChild - (0x004C2CCD + sizeof(DWORD));
	*(LPBYTE )0x004C2CD1 = NOP;

	// TSSGCtrl::ChangeDirectorySubject
	*(LPDWORD)0x00502A40 = (DWORD)TSSGCtrl_ChangeDirectorySubject_stub - (0x00502A40 + sizeof(DWORD));
}
