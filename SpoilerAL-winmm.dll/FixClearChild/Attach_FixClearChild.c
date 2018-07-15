#include "SubjectStringTable/SubjectStringOperator.h"
#include "bcb6_operator.h"
#include "bcb6_std_deque.h"
#include "TSSDir.h"
#include "TSSGAttributeElement.h"
#include "SSGSubjectProperty.h"

extern BOOL FixTheProcedure;
void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

extern int AttributeElementOrder;
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

static void __fastcall TSSDir_prepareGetSubjectVec(TSSGSubject* SSDir, TSSGCtrl* SSGC) {
	if (FixTheProcedure) {
		string* Code = SubjectStringTable_GetString(&SSDir->code);
		if (!string_empty(Code)) {
			string LineS, Token;
			vector_string List;
			string_ctor_assign(&LineS, Code);
			FormatNameString(SSGC, SSDir, &LineS);
			vector_ctor(&List);
			string_ctor_assign_cstr_with_length(&Token, ",", 1);
			if (TStringDivision_List(&SSGC->strD, &LineS, Token, &List, 0) < 2)
				Parsing(SSGC, SSDir, &LineS, 0);
			else {
				vector* attrs = TSSGSubject_GetAttribute(SSDir);
				TSSDir_ClearChild((TSSDir*)SSDir);
				AttributeElementOrder = 0;
				TSSGAttributeSelector_StartElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
				for (TSSGAttributeElement** pos = (TSSGAttributeElement**)vector_begin(attrs);
					 pos < (TSSGAttributeElement**)vector_end(attrs);
					 ++pos) {
					TSSGAttributeElement* AElem = *pos;
					if (TSSGAttributeElement_GetType(AElem) == atDIR_LEVEL) continue;
					for (list_iterator *SIt = list_begin(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
						 SIt != list_end(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
						 list_iterator_increment(SIt)) {
						if (TSSGAttributeElement_IsEqual((TSSGAttributeElement*)SIt->_M_node->_M_data, AElem)) {
							list_erase(SIt);// avoid delete
							break;
						}
					}
					if (TSSGAttributeElement_GetType(AElem) & (atREPLACE | atENABLED | atDEFINE | atSCOPE))
						TSSGAttributeSelector_AddElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
					else
						TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
				}
				{
					TDirAttribute* NewAElem = bcb6_operator_new(sizeof(TDirAttribute));
					NewAElem->VTable = (void*)0x006403A8;
					NewAElem->type   = atDIR_LEVEL;
					NewAElem->level  = TSSGCtrl_GetDirLevel(SSGC, SSDir) + 1;
					TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), NewAElem);
				}
				{
					deque ParentStack;
					TSSGSubjectProperty* prop = GetSubjectProperty(SSDir);
					stack_ptr_ctor(&ParentStack, 0);
					stack_ptr_push(&ParentStack, &SSDir);
					RepeatDepth = prop->RepeatDepth;
					repeat_ReadSSRFile(SSGC, &ParentStack, NULL, &LineS, prop->RepeatIndex, prop->ParentRepeat);
					stack_ptr_dtor(&ParentStack, 0);
				}
				TSSGAttributeSelector_EndElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
			}
			vector_string_dtor(&List);
			string_dtor(&LineS);
		}
	}
}

static void __declspec(naked) TSSGCtrl_ChangeDirectorySubject_GetSubjectVec() {
	__asm {
		mov  edx, edi  // ecx is TSSDir* already
		push 0x004C2D64// TSSGSubject::GetSubjectVec
		jmp  TSSDir_prepareGetSubjectVec
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
	*(LPDWORD)0x00502A40 = (DWORD)TSSGCtrl_ChangeDirectorySubject_GetSubjectVec - (0x00502A40 + sizeof(DWORD));
}
