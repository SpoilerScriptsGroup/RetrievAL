#include "SubjectStringTable.h"
#include "TSSGSubject.h"
#include "TStringDivision.h"

#define array   SubjectStringTable_array
#define indices SubjectStringTable_indices

#define INDEX_MEMBER(s) *(size_t *)&(s)->padding1
#define offsetof_index 8

bcb6_std_string * __fastcall SubjectStringTable_GetString(bcb6_std_string *s)
{
	return ((bcb6_std_string *)array._M_start) + INDEX_MEMBER(s);
}

static bcb6_std_string * __fastcall GetName(TSSGSubject *SSGS)
{
	return ((bcb6_std_string *)array._M_start) + INDEX_MEMBER(&SSGS->name);
}

static bcb6_std_string * __fastcall GetCode(TSSGSubject *SSGS)
{
	return ((bcb6_std_string *)array._M_start) + INDEX_MEMBER(&SSGS->code);
}

static bcb6_std_string * __fastcall GetSubjectName(TSSGSubject *SSGS)
{
	return ((bcb6_std_string *)array._M_start) + INDEX_MEMBER(&SSGS->subjectName);
}

void __fastcall SubjectStringTable_SetString(bcb6_std_string *dest, bcb6_std_string *src)
{
	INDEX_MEMBER(dest) = SubjectStringTable_insert(src);
}

static void __fastcall SetName(TSSGSubject *SSGS, const char *s)
{
	INDEX_MEMBER(&SSGS->name) = SubjectStringTable_insert_cstr(s);
}

static void __fastcall SetCode(TSSGSubject *SSGS, const char *s)
{
	INDEX_MEMBER(&SSGS->code) = SubjectStringTable_insert_cstr(s);
}

__declspec(naked) void __cdecl TSSGSubject_Setting_SetSubjectName()
{
	__asm
	{
		#define name        (ebx + 14H)
		#define subjectName (ebx + 44H)

		mov     eax, dword ptr [name + offsetof_index]
		mov     dword ptr [subjectName + offsetof_index], eax
		ret

		#undef name
		#undef subjectName
	}
}

__declspec(naked) void __cdecl TFindNameForm_EnumSubjectNameFind_GetName()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		jmp     dword ptr [bcb6_std_string_ctor_assign]
	}
}

__declspec(naked) void __cdecl TSearchForm_Init_GetName()
{
	__asm
	{
		mov     ecx, dword ptr [ebp + 14H]
		jmp     GetName
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName()
{
	__asm
	{
		#define SSGS (ebp - 7C8H)
		#define Code (ebp - 110H)
		#define Name (ebp - 128H)

		mov     edx, dword ptr [Code]
		mov     ecx, dword ptr [SSGS]
		call    SetCode
		mov     edx, dword ptr [Name]
		mov     ecx, dword ptr [SSGS]
		call    SetName
		ret

		#undef SSGS
		#undef Code
		#undef Name
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCodeOrName()
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

__declspec(naked) void __cdecl TSSGSubject_GetSubjectName_GetSubjectName()
{
	__asm
	{
		#define _this ebx

		mov     ecx, _this
		call    GetSubjectName
		pop     ecx
		push    eax
		push    ebx
		jmp     ecx

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetCode()
{
	__asm
	{
		#define _this ebx

		add     dword ptr [esi + 1CH], 4
		mov     ecx, _this
		call    GetCode
		pop     ecx
		push    eax
		lea     eax, [ebp - 58H]
		jmp     ecx

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetName()
{
	__asm
	{
		#define _this ebx
		#define Src   (esp + 8)

		mov     ecx, _this
		call    GetName
		mov     dword ptr [Src], eax
		jmp     TStringDivision_List

		#undef _this
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSToggle_Setting_GetCode()
{
	__asm
	{
		#define _this ebx

		add     dword ptr [esi + 1CH], 4
		mov     ecx, _this
		call    GetCode
		pop     ecx
		push    eax
		lea     eax, [ebp - 70H]
		jmp     ecx

		#undef _this
	}
}

__declspec(naked) void __cdecl TSSTrace_Setting_GetCode()
{
	__asm
	{
		#define _this ebx

		mov     ecx, _this
		call    GetCode
		mov     dword ptr [ebp - 68H], eax
		ret

		#undef _this
	}
}
