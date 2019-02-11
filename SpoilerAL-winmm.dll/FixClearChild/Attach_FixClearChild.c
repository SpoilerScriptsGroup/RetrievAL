#define USING_NAMESPACE_BCB6_STD
#include "SubjectStringOperator.h"
#include "bcb6_std_deque.h"
#include "TSSDir.h"
#include "SSGSubjectProperty.h"

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
	const string *Code = SubjectStringTable_GetString(&SSDir->code);
	if (!string_empty(Code)) {
		string Token;
		vector_string List = { NULL };
		string_ctor_assign_char(&Token, ',');
		if (TStringDivision_List(&SSGC->strD, Code, Token, &List, FALSE) > 1) {
			vector* attrs = TSSGSubject_GetAttribute(SSDir);
			TSSDir* this = (TSSDir*)SSDir;
			if (SSDir->evaluateAtRead) {
				TSSGSubject** offset = &vector_at(&this->childVec, SSDir->fixed);
				for (TSSGSubject **it = offset; it < vector_end(&this->childVec); it++) {
					if ((*it)->status & ssLOCK)
						TSSGCtrl_SetLock(SSGC, FALSE, *it, NULL);
					if ((*it)->type == stDIR)
						TSSDir_ClearChild((TSSDir *)*it);
					delete_TSSGSubject(*it);
				}
				vector_end(&this->childVec) = offset;
			} else {
				SSDir->fixed = (WORD)vector_size(&this->childVec);
				SSDir->evaluateAtRead = TRUE;
			}
			AttributeElementOrder = 0;
			TSSGAttributeSelector_StartElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
			{
				TDirAttribute* NewAElem = new_TDirAttribute();
				NewAElem->level = TSSGCtrl_GetDirLevel(SSGC, SSDir) + 1;
				TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), NewAElem);
			}
			for (TSSGAttributeElement** pos = &vector_type_at(attrs, TSSGAttributeElement*, 1);
				 pos < (TSSGAttributeElement**)vector_end(attrs);
				 ++pos) {
				TSSGAttributeElement* AElem = *pos;
				for (list_iterator SIt = list_begin(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
					 SIt != list_end(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
					 list_iterator_increment(SIt)) {
					if (TSSGAttributeElement_IsEqual(*(TSSGAttributeElement**)SIt->_M_data, AElem)) {
						list_erase(&SIt);// prevent delete in TSSGAttributeSelector::MakeOnlyOneAtteribute
						break;
					}
				}
				if (TSSGAttributeElement_GetType(AElem) & (atREPLACE | atENABLED | atDEFINE | atSCOPE | atFORMAT))
					TSSGAttributeSelector_AddElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
				else
					TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
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
		} else Parsing(SSGC, SSDir, Code, 0);
		vector_string_dtor(&List);
	}
}

static void __declspec(naked) TSSGCtrl_ChangeDirectorySubject_GetSubjectVec() {
	extern BOOL ExtensionTSSDir;
	__asm {// ecx is TSSDir* already
		cmp  ExtensionTSSDir, 0
		je   SKIP
		mov  edx, edi
		call TSSDir_prepareGetSubjectVec
	SKIP:
		mov  eax, 0x004C2D64
		jmp  eax//TSSDir::GetSubjectVec
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
