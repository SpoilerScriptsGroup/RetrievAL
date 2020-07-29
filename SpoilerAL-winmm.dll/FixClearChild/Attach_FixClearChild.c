#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_stack.h"
#include "TSSDir.h"
#include "SSGSubjectProperty.h"
#include "SubjectStringOperator.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

extern DWORD RepeatDepth;
extern void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	LPVOID       ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS);

static void(__cdecl * const stack_ptr_ctor)(pdeque this, rsize_t __n) = (void*)0x004E49B0;
static void(__cdecl * const stack_ptr_dtor)(pdeque this, BOOL delete) = (void*)0x004E4FA8;
static void __fastcall TSSDir_GetSubjectVec_onOpen(TSSGSubject *SSGS, TSSGCtrl *SSGC)
{
	const string *Code = SubjectStringTable_GetString(&SSGS->code);
	if (!string_empty(Code))
	{
		string Token;
		vector_string List = { NULL };
		string_ctor_assign_char(&Token, ',');
		if (TStringDivision_List(&SSGC->strD, Code, Token, &List, FALSE) > 1)
		{
			vector *attr = TSSGSubject_GetAttribute(SSGS);
			TSSDir *this = (TSSDir *)SSGS;
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
			{
				TDirAttribute *NewAElem = new_TDirAttribute();
				TDirAttribute_Setting(NewAElem, TSSGCtrl_GetDirLevel(SSGC, SSGS) + 1);
				NewAElem->identity = (NewAElem->ref = SSGS)->name.sstIndex;
				TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), NewAElem);
			}
			for (TSSGAttributeElement **pos = &vector_type_at(attr, TSSGAttributeElement *, 1);
				 pos < (TSSGAttributeElement **)vector_end(attr);
				 pos++)
			{
				TSSGAttributeElement *AElem = *pos;
				for (list_iterator SIt = list_begin(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
					 SIt != list_end(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
					 list_iterator_increment(SIt))
				{
					if (TSSGAttributeElement_IsEqual(*(TSSGAttributeElement **)SIt->_M_data, AElem))
					{
						list_erase(SIt);// prevent delete in TSSGAttributeSelector::MakeOnlyOneAtteribute
						break;
					}
				}
				if (TSSGAttributeElement_GetType(AElem) & (atREPLACE | atENABLED | atDEFINE | atSCOPE | atFORMAT))
					TSSGAttributeSelector_AddElement (TSSGCtrl_GetAttributeSelector(SSGC), AElem);
				else
					TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
			}
			{
				stack_ptr ParentStack;
				TSSGSubjectProperty *prop = GetSubjectProperty(SSGS);
				RepeatDepth = prop ? prop->RepeatDepth : 0;

				stack_ptr_ctor(&ParentStack, 0);
				stack_ptr_push(&ParentStack, &SSGS);
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, prop ? prop->RepeatIndex : 0, prop ? prop->OuterRepeat : MAXDWORD, SSGS);
				stack_ptr_dtor(&ParentStack, FALSE);
			}
			TSSGAttributeSelector_EndElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
		}
		else Parsing(SSGC, SSGS, Code, 0);
		vector_string_dtor(&List);
	}
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
