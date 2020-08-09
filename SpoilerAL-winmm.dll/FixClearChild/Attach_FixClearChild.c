#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_stack.h"
#include "TSSDir.h"
#include "SSGSubjectProperty.h"
#include "SubjectStringOperator.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);
extern unsigned seqElement;
extern BOOL  FixTheProcedure;
extern DWORD RepeatDepth;
extern void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	stack        *ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS);
#define ABBREV_SELECT 1
static void(__cdecl *const stack_ptr_ctor)(pdeque this, rsize_t __n) = (void *)0x004E49B0;
static void(__cdecl *const stack_ptr_dtor)(pdeque this, BOOL delete) = (void *)0x004E4FA8;
static void __fastcall TSSDir_GetSubjectVec_onOpen(TSSGSubject *const SSGS, TSSGCtrl *const SSGC)
{
	const string *Code = SubjectStringTable_GetString(&SSGS->code);
	if (SSGS->isRepeatable)
	{
		TSSDir *const this = (void *)SSGS;
		vector *const attr = TSSGSubject_GetAttribute(SSGS);
		AeType const multi = atREPLACE | FixTheProcedure << 4 | FixTheProcedure << 5 | atENABLED
#if !ABBREV_SELECT
			| atDEFINE
#endif
			| atSCOPE | atFORMAT | atREPEAT;
		if (SSGS->stable != MAXWORD)
		{
			TSSGSubject **discard = &vector_at(&this->childVec, SSGS->stable);
			for (TSSGSubject **it = discard; it < vector_end(&this->childVec); it++)
			{
				if ((*it)->isLocked)
					TSSGCtrl_SetLock(SSGC, FALSE, *it, NULL);
				delete_TSSGSubject(*it);
			}
			vector_end(&this->childVec) = discard;
		}
		else
			SSGS->stable = (WORD)vector_size(&this->childVec);
		TSSGAttributeSelector_StartElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
		for (TAdjustmentAttribute *const *pos = &vector_type_at(attr, TAdjustmentAttribute *, 1);
			 pos < (TAdjustmentAttribute **)vector_end(attr);
			 pos++)
		{
			TAdjustmentAttribute *const AElem = *pos;
#if ABBREV_SELECT
			list_dword_push_back(TSSGCtrl_GetAttributeSelector(SSGC)->nowAttributeList, (LPDWORD)pos);
			if (TSSGAttributeElement_GetType(AElem) & multi && AElem->seqElement >= seqElement)
				seqElement = AElem->seqElement + 1;
#else
			for (list_iterator LIt = list_begin(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
				 LIt != list_end(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
				 list_iterator_increment(LIt))
			{
				if (TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)LIt->_M_data))
				{
					list_erase(LIt);// prevent delete in TSSGAttributeSelector::MakeOnlyOneAtteribute
					break;
				}
			}
			if (TSSGAttributeElement_GetType(AElem) & multi)
				TSSGAttributeSelector_AddElement (TSSGCtrl_GetAttributeSelector(SSGC), AElem);
			else
				TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
#endif
		}
		{
			TDirAttribute *NewAElem = new_TDirAttribute();
			TDirAttribute_Setting(NewAElem, TSSGCtrl_GetDirLevel(SSGC, SSGS) + 1);
			TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), NewAElem);
		}
		{
			stack_ptr ParentStack;
			const TSSGSubjectProperty *prop;

			stack_ptr_ctor(&ParentStack, 0);
			stack_ptr_push(&ParentStack, &SSGS);
			if (prop = GetSubjectProperty(SSGS))
			{
				RepeatDepth = prop->RepeatDepth;
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, prop->RepeatIndex, prop->OuterRepeat, SSGS);
			}
			else
			{
				RepeatDepth = 0;
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, 0, MAXDWORD, SSGS);
			}
			stack_ptr_dtor(&ParentStack, FALSE);
		}
		TSSGAttributeSelector_EndElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
	}
	else if (!string_empty(Code))
		Parsing(SSGC, SSGS, Code, 0);
}

static void __declspec(naked) TSSGCtrl_ChangeDirectorySubject_GetSubjectVec(TSSDir *this, long Mode, pvector_PTSSGSubject Vec)
{
	extern BOOL  ExtensionTSSDir;
	static DWORD const TSSDir_GetSubjectVec = 0x004C2D64;
	__asm {// ecx is TSSDir* already
		cmp  ExtensionTSSDir, 0
		je   PASS
		mov  edx, edi
		call TSSDir_GetSubjectVec_onOpen
	PASS:
		jmp  dword ptr [TSSDir_GetSubjectVec]
		ud2
	}
}

#define NOP       (BYTE)0x90
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixClearChild()
{
	// TSSDir::ClearChild
	*(LPBYTE )0x004C2CCC = JMP_REL32;
	*(LPDWORD)0x004C2CCD = (DWORD)TSSDir_ClearChild - (0x004C2CCD + sizeof(DWORD));
	*(LPBYTE )0x004C2CD1 = NOP;

	// TSSGCtrl::ChangeDirectorySubject
	*(LPDWORD)0x00502A40 = (DWORD)TSSGCtrl_ChangeDirectorySubject_GetSubjectVec - (0x00502A40 + sizeof(DWORD));
}
