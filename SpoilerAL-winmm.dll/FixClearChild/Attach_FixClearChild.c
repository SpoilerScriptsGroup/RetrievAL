#define USING_NAMESPACE_BCB6_STD
#include "SubjectStringOperator.h"
#include "bcb6_operator.h"
#include "bcb6_std_deque.h"
#include "TSSDir.h"
#include "TSSGAttributeElement.h"
#include "SSGSubjectProperty.h"
#include "TSSGAttributeSelector.h"

extern BOOL ExtensionTSSDir;
void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

extern int AttributeElementOrder;
extern DWORD RepeatDepth;
void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	LPVOID       ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        ParentRepeat,
	TSSGSubject  *SSGS);

static void(__cdecl * const stack_ptr_ctor)(pdeque this, void* zero) = (void*)0x004E49B0;
static void(__cdecl * const stack_ptr_push)(pdeque this, void** val) = (void*)0x004E4BC0;
static void(__cdecl * const stack_ptr_dtor)(pdeque this, void* zero) = (void*)0x004E4FA8;
static void __fastcall TSSDir_prepareGetSubjectVec(TSSGSubject* SSDir, TSSGCtrl* SSGC) {
	if (ExtensionTSSDir) {
		const string *Code = SubjectStringTable_GetString(&SSDir->code);
		if (!string_empty(Code)) {
			string Token;
			vector_string List;
			// omits this for the performance
			//string LineS; string_ctor_assign(&LineS, Code); FormatNameString(SSGC, SSDir, &LineS); Code = &LineS;
			vector_ctor(&List);
			string_ctor_assign_cstr_with_length(&Token, ",", 1);
			if (TStringDivision_List(&SSGC->strD, Code, Token, &List, 0) < 2)
				Parsing(SSGC, SSDir, Code, 0);
			else {
				vector* attrs = TSSGSubject_GetAttribute(SSDir);
				TSSDir* this = (TSSDir*)SSDir;
				if (SSDir->evaluateAtRead) {
					TSSGSubject** offset = &vector_type_at(&this->childVec, TSSGSubject*, SSDir->fixed);
					for (TSSGSubject **it = offset; it != vector_end(&this->childVec); it++) {
						if ((*it)->status & 2)
							TSSGCtrl_SetLock(SSGC, FALSE, *it, NULL);
						if ((*it)->type == stDIR)
							TSSDir_ClearChild((TSSDir *)*it);
						delete_TSSGSubject(*it);
					}
					vector_end(&this->childVec) = offset;
				} else {
					SSDir->evaluateAtRead = TRUE;
					SSDir->fixed = (WORD)vector_size_by_type(&this->childVec, TSSGSubject*);
				}
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
					NewAElem->VTable = TDirAttribute_VTable;
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
					repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, prop->RepeatIndex, prop->ParentRepeat, SSDir);
					stack_ptr_dtor(&ParentStack, 0);
				}
				TSSGAttributeSelector_EndElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
			}
			vector_string_dtor(&List);
			//string_dtor(&LineS);
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
