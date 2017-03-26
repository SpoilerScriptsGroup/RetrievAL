#ifdef __BORLANDC__
#include <windows.h>
#include "tlhelp32fix.h"
#pragma warn -8070

__declspec(naked) BOOL WINAPI Process32FirstA(HANDLE hSnapshot, LPPROCESSENTRY32A lppe)
{
	__asm
	{
		jmp     Process32First
	}
}

__declspec(naked) BOOL WINAPI Process32NextA(HANDLE hSnapshot, LPPROCESSENTRY32A lppe)
{
	__asm
	{
		jmp     Process32Next
	}
}

__declspec(naked) BOOL WINAPI Module32FirstA(HANDLE hSnapshot, LPMODULEENTRY32A lpme)
{
	__asm
	{
		jmp     Module32First
	}
}

__declspec(naked) BOOL WINAPI Module32NextA(HANDLE hSnapshot, LPMODULEENTRY32A lpme)
{
	__asm
	{
		jmp     Module32Next
	}
}
#endif
