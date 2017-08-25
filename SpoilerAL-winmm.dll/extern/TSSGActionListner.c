#define USING_NAMESPACE_BCB6_STD
#include "TSSGActionListner.h"

void(__cdecl *TSSGActionListner_OnProcessOpen)(LPVOID _this, TSSGSubject *SSGS, DWORD Mode) = (LPVOID)0x0052EB20;
void(__cdecl *TSSGActionListner_OnSubjectDisabled)(LPVOID _this, string Code) = (LPVOID)0x0052EBC4;
void(__cdecl *TSSGActionListner_OnProcessOpenError)(LPVOID _this, TSSGSubject *SSGS) = (LPVOID)0x0052EE78;
void(__cdecl *TSSGActionListner_OnSubjectReadError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052EF8C;
void(__cdecl *TSSGActionListner_OnSubjectWriteError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F13C;
void(__cdecl *TSSGActionListner_OnSubjectReadSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F31C;
void(__cdecl *TSSGActionListner_OnSubjectWriteSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F6D0;

__declspec(naked) void __stdcall TSSGActionListner_OnParsingProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)
		#define TopVal           (esp + 16)

		mov     eax, dword ptr [TopVal]
		sub     esp, 28
		mov     edx, dword ptr [Code + 28]
		mov     dword ptr [esp + 24], eax
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 28]
		mov     edx, dword ptr [SSGS + 28]
		mov     eax, dword ptr [ecx]
		push    edx
		push    ecx
		call    dword ptr [eax + 44]
		add     esp, 36
		ret     16

		#undef ssgActionListner
		#undef SSGS
		#undef Code
		#undef TopVal
	}
}

__declspec(naked) void __stdcall TSSGActionListner_OnParsingDoubleProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)
		#define TopVal           (esp + 16)

		mov     eax, dword ptr [TopVal + 4]
		mov     ecx, dword ptr [TopVal]
		push    eax
		push    ecx
		mov     edx, dword ptr [Code + 8]
		sub     esp, 24
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 32]
		mov     edx, dword ptr [SSGS + 32]
		mov     eax, dword ptr [ecx]
		push    edx
		push    ecx
		call    dword ptr [eax + 48]
		add     esp, 40
		ret     20

		#undef ssgActionListner
		#undef SSGS
		#undef Code
		#undef TopVal
	}
}

__declspec(naked) void __stdcall TSSGActionListner_OnParsingError(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)

		mov     edx, dword ptr [Code]
		sub     esp, 24
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 24]
		mov     edx, dword ptr [SSGS + 24]
		mov     eax, dword ptr [ecx]
		push    edx
		push    ecx
		call    dword ptr [eax + 40]
		add     esp, 32
		ret     12

		#undef ssgActionListner
		#undef SSGS
		#undef Code
	}
}
