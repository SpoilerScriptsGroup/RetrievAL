#include <windows.h>
#include "TSSGSubject.h"

static void __fastcall TSSGSubject_string_ctor(TSSGSubject *this);

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor1()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 0ACH]
		jmp     TSSGSubject_string_ctor
	}
}

#if 0
__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor2()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 674H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor3()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 698H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor4()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 6BCH]
		jmp     TSSGSubject_string_ctor
	}
}
#endif

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor5()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 4H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor6()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 8H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor7()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 0CH]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor8()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 18H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor9()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 24H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor10()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 28H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor11()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 2CH]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor12()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 30H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor13()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 34H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor14()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 40H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor15()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 44H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor16()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 48H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor17()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 4CH]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor18()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 50H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor19()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 54H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor20()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 12
		mov     ecx, dword ptr [ebp - 58H]
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor21()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 8
		mov     ecx, edi
		jmp     TSSGSubject_string_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_string_ctor22()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 8
		lea     ecx, [ebp - 60H]
		jmp     TSSGSubject_string_ctor
	}
}

static void __fastcall TSSGSubject_string_ctor(TSSGSubject *this)
{
	this->name        = (const bcb6_std_string) { NULL };
	this->code        = (const bcb6_std_string) { NULL };
	this->subjectName = (const bcb6_std_string) { NULL };
}
