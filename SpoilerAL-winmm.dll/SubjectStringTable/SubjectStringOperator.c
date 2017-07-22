#include "SubjectStringTable.h"
#include "TSSGSubject.h"
#include "TStringDivision.h"

#define array   SubjectStringTable_array
#define indices SubjectStringTable_indices

#define INDEX_MEMBER(s) *(size_t *)&(s)->padding1

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

static void __fastcall SetName(TSSGSubject *SSGS, const char *s)
{
	INDEX_MEMBER(&SSGS->name) = SubjectStringTable_insert_cstr(s);
}

static void __fastcall SetCode(TSSGSubject *SSGS, const char *s)
{
	INDEX_MEMBER(&SSGS->code) = SubjectStringTable_insert_cstr(s);
}

static void __fastcall SetSubjectName(TSSGSubject *SSGS, bcb6_std_string *s)
{
	INDEX_MEMBER(&SSGS->subjectName) = SubjectStringTable_insert(s);
}

__declspec(naked) void __cdecl TSSGSubject_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress  0x0046CBF8
		#define name           (ebx + 14H)
		#define subjectName    (ebx + 44H)
		#define offsetof_index 8

		mov     eax, dword ptr [name + offsetof_index]
		push    ReturnAddress
		mov     dword ptr [subjectName + offsetof_index], eax
		ret

		#undef ReturnAddress
		#undef name
		#undef subjectName
		#undef offsetof_index
	}
}

__declspec(naked) void __cdecl TFindNameForm_EnumSubjectNameFind_GetName()
{
	__asm
	{
		#define ReturnAddress 0x00485213

		call    GetName
		push    eax
		lea     eax, [ebp - 50H]
		push    eax
		push    ReturnAddress
		jmp     dword ptr [bcb6_std_string_ctor_assign]

		#undef ReturnAddress
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

__declspec(naked) void __cdecl TSearchForm_DGridSelectCell_GetName()
{
	__asm
	{
		#define ReturnAddress 0x0049C231

		mov     ecx, dword ptr [ebp - 454H]
		call    GetName
		push    eax
		lea     eax, [ebp - 220H]
		push    eax
		push    ReturnAddress
		jmp     dword ptr [bcb6_std_string_ctor_assign]

		#undef ReturnAddress
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName()
{
	__asm
	{
		#define ReturnAddress 0x004E5DA8
		#define SSGS          (ebp - 7C8H)
		#define Code          (ebp - 110H)
		#define Name          (ebp - 128H)

		mov     edx, dword ptr [Code]
		mov     ecx, dword ptr [SSGS]
		call    SetCode
		mov     edx, dword ptr [Name]
		mov     ecx, dword ptr [SSGS]
		push    ReturnAddress
		jmp     SetName

		#undef ReturnAddress
		#undef SSGS
		#undef Code
		#undef Name
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCode()
{
	__asm
	{
		#define ReturnAddress 0x004EAE80
		#define SSGS          (ebp - 0EF4H)
		#define Src           (ebp - 0F74H)

		mov     edx, dword ptr [Src]
		mov     ecx, dword ptr [SSGS]
		push    ReturnAddress
		jmp     SetCode

		#undef ReturnAddress
		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetName()
{
	__asm
	{
		#define ReturnAddress 0x004EAFA2
		#define SSGS          (ebp - 0EF4H)
		#define Src           (ebp - 0F90H)

		mov     edx, dword ptr [Src]
		mov     ecx, dword ptr [SSGS]
		push    ReturnAddress
		jmp     SetName

		#undef ReturnAddress
		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_MakeADJFile_GetCode()
{
	__asm
	{
		#define ReturnAddress 0x00503169
		#define SSGS          esi

		mov     ecx, SSGS
		call    GetCode
		push    eax
		lea     eax, [ebp - 0F0H]
		push    eax
		push    ReturnAddress
		jmp     dword ptr [bcb6_std_string_ctor_assign]

		#undef ReturnAddress
		#undef SSGS
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

__declspec(naked) void __cdecl TSSBitList_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004B8F15
		#define _this         ebx
		#define tmpV0         edx

		push    ReturnAddress
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV0
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
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004BD463
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 320
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleList_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004BF20E
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 224
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleToggle_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004BFF92
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 188
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSCalc_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004C1BE8
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 188
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSCopy_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004C2996
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 188
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004C4016
		#define _this         ebx
		#define tmpV0         edx

		push    ReturnAddress
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004C846D
		#define _this         ebx
		#define tmpV0         edx

		push    ReturnAddress
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSFloatCalc_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x004CE107
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 188
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSList_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x00529F31
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 224
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSString_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x0052AD58
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 224
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
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

__declspec(naked) void __cdecl TSSToggle_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x0052C1DE
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 260
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
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

__declspec(naked) void __cdecl TSSTrace_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x0052CD9D
		#define _this         ebx
		#define tmpV0         edx

		push    ReturnAddress
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleFloatCalc_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress 0x0052DA97
		#define _this         ebx
		#define tmpV          edi

		mov     word ptr [esi + 10H], 188
		push    ReturnAddress
		mov     edx, dword ptr [tmpV]
		mov     ecx, _this
		jmp     SetSubjectName

		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSSplit_Setting_SetSubjectName()
{
	__asm
	{
		#define ReturnAddress  0x005305CF
		#define name           (ebx + 14H)
		#define subjectName    (ebx + 44H)
		#define offsetof_index 8

		mov     eax, dword ptr [name + offsetof_index]
		push    ReturnAddress
		mov     dword ptr [subjectName + offsetof_index], eax
		ret

		#undef ReturnAddress
		#undef _this
		#undef tmpV
		#undef offsetof_index
	}
}
