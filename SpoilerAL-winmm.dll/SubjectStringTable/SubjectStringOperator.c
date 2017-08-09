#include "SubjectStringTable.h"
#include "TStringDivision.h"
#include "TSSGSubject.h"
#include "TSSString.h"
#include "TSSGCtrl.h"
#include "TMainForm.h"

void __stdcall FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS, bcb6_std_string *s);
void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, bcb6_std_string *line);

#define array SubjectStringTable_array

#define INDEX_MEMBER(s) *(size_t *)&(s)->padding1
#define offsetof_index 8

#define offsetof_TMainForm_ssgCtrl 0x738

void __cdecl SubjectStringTable_StringCtor(bcb6_std_string *s)
{
	s->_M_start          = NULL;
	s->_M_finish         = NULL;
	s->padding1          = NULL;
	s->padding2          = NULL;
	s->_M_end_of_storage = NULL;
	s->padding3          = NULL;
}

bcb6_std_string * __fastcall SubjectStringTable_GetString(bcb6_std_string *s)
{
	return ((bcb6_std_string *)array._M_start) + INDEX_MEMBER(s);
}

void __fastcall SubjectStringTable_SetString(bcb6_std_string *dest, bcb6_std_string *src)
{
	INDEX_MEMBER(dest) = SubjectStringTable_insert(src);
}

static void __fastcall SetName(TSSGSubject *SSGS, bcb6_std_string *s)
{
	INDEX_MEMBER(&SSGS->name) = SubjectStringTable_insert(s);
}

static void __fastcall SetCode(TSSGSubject *SSGS, bcb6_std_string *s)
{
	INDEX_MEMBER(&SSGS->code) = SubjectStringTable_insert(s);
}

static void __fastcall TMainForm_FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS);

__declspec(naked) void __cdecl TMainForm_SubjectAccess_TSSToggle_GetNowValHeadStr()
{
	__asm
	{
		#define _this ebx
		#define SSGS  (ebp - 2BCH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
		jmp     TMainForm_FormatNameString

		#undef _this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SubjectAccess_TSSString_GetNowValHeadStr()
{
	__asm
	{
		#define _this ebx
		#define SSGS  (ebp - 2FCH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
		jmp     TMainForm_FormatNameString

		#undef _this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr()
{
	__asm
	{
		#define _this ebx
		#define SSGS  edi

		mov     edx, SSGS
		lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
		jmp     TMainForm_FormatNameString

		#undef _this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SetCalcNowValue_TSSCalc_GetNowValHeadStr()
{
	__asm
	{
		#define _this esi
		#define SSGS  (ebp - 3B0H)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
		jmp     TMainForm_FormatNameString

		#undef _this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValHeadStr()
{
	__asm
	{
		#define _this esi
		#define SSGS  (ebp - 47CH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
		jmp     TMainForm_FormatNameString

		#undef _this
		#undef SSGS
	}
}

__declspec(naked) static void __fastcall TMainForm_FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]
		push    eax
		push    edx
		push    ecx
		push    eax
		mov     ecx, dword ptr [esp + 16 + 8]
		push    eax
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 4], eax
		call    dword ptr [bcb6_std_string_ctor_assign]
		add     esp, 8
		call    FormatNameString
		ret
	}
}

static void __fastcall ModifySplit(bcb6_std_string *dest, bcb6_std_string *src, TMainForm *_this, TSSGSubject *TSSS);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_GetStrParam()
{
	__asm
	{
		#define _this ebx
		#define SSGS  (ebp - 1E0H)

		mov     edx, dword ptr [SSGS]
		pop     eax
		mov     ecx, dword ptr [esp - 4 + 8]
		push    edx
		push    _this
		push    eax
		call    SubjectStringTable_GetString
		mov     edx, eax
		mov     ecx, dword ptr [esp + 8 + 4]
		jmp     ModifySplit

		#undef _this
		#undef SSGS
	}
}

static void __fastcall ModifySplit(bcb6_std_string *dest, bcb6_std_string *src, TMainForm *_this, TSSGSubject *SSGS)
{
	if (!bcb6_std_string_empty(src))
	{
		bcb6_std_string_ctor_assign(dest, src);
		ReplaceDefineDynamic(SSGS, dest);
		FormatNameString(&_this->ssgCtrl, SSGS, dest);
	}
	else
	{
		TSSGSubject_GetSubjectName(dest, SSGS, &_this->ssgCtrl);
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Read_GetFileName()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		jmp     dword ptr [bcb6_std_string_ctor_assign]
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetCode()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		jmp     dword ptr [bcb6_std_string_concat]
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetName()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		jmp     TStringDivision_List
	}
}

__declspec(naked) void __cdecl TSSBitList_Read_GetAddressStr()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 12]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 12], eax
		jmp     dword ptr [TSSGCtrl_GetAddress]
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName()
{
	__asm
	{
		#define SSGS (ebp - 7C8H)
		#define Code (ebp - 110H)
		#define Name (ebp - 128H)

		mov     ecx, dword ptr [SSGS]
		lea     edx, [Code]
		call    SetCode
		mov     ecx, dword ptr [SSGS]
		lea     edx, [Name]
		call    SetName
		ret

		#undef SSGS
		#undef Code
		#undef Name
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCode()
{
	__asm
	{
		#define SSGS (ebp - 0EF4H)
		#define Src  eax

		mov     edx, Src
		mov     ecx, dword ptr [SSGS]
		jmp     SetCode

		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetName()
{
	__asm
	{
		#define SSGS (ebp - 0EF4H)
		#define Src  eax

		mov     edx, Src
		mov     ecx, dword ptr [SSGS]
		jmp     SetName

		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_MakeADJFile_GetAddressStr()
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     eax, dword ptr [eax]
		jmp     dword ptr [eax + 8 * 4]
	}
}

static __inline void TSSString_Setting_CheckUnicode(TSSString *_this, bcb6_std_string *s)
{
	_this->isUnicode = (
		bcb6_std_string_length(s) == 7 &&
		*(LPDWORD) s->_M_start      == BSWAP32('unic') &&
		*(LPDWORD)(s->_M_start + 4) == BSWAP32('ode\0'));
	if (_this->isUnicode)
	{
		*(LPDWORD)s->_M_start = '0000';
		*(s->_M_finish = s->_M_start + 4) = '\0';
		_this->size &= -2;
	}
}

void __fastcall TSSString_Setting_SetEndWord(TSSString *_this, bcb6_std_string *s)
{
	TSSString_Setting_CheckUnicode(_this, s);
	SubjectStringTable_SetString(&_this->endWord, s);
}

__declspec(naked) void __cdecl TSSString_Read_GetEndWord()
{
	__asm
	{
		#define _this ebx

		lea     ecx, [_this + 98H]
		call    SubjectStringTable_GetString
		mov     edi, eax
		ret

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSString_Write_GetEndWord()
{
	__asm
	{
		#define _this (ebp + 8H)

		mov     ecx, dword ptr [_this]
		add     ecx, 152
		jmp     SubjectStringTable_GetString

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSToggle_Read_GetOnCode1()
{
	__asm
	{
		#define _this ebx

		lea     ecx, [_this + 90H]
		jmp     SubjectStringTable_GetString

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSToggle_Read_GetOffCode()
{
	__asm
	{
		#define _this ebx

		lea     ecx, [_this + 0A8H]
		jmp     SubjectStringTable_GetString

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSTrace_Write_GetFileName()
{
	__asm
	{
		#define _this ebx

		lea     ecx, [_this + 78H]
		call    SubjectStringTable_GetString
		mov     ecx, eax
		pop     eax
		push    1
		jmp     eax

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSGSubject_GetSubjectName_GetSubjectName()
{
	__asm
	{
		#define _this ebx

		lea     ecx, [_this + 44H]
		call    SubjectStringTable_GetString
		pop     ecx
		push    eax
		push    ebx
		jmp     ecx

		#undef _this
	}
}
