#include <string.h>

char * __cdecl strlcpy(char *string1, const char *string2, size_t count);

__declspec(naked) char * __stdcall _lstrcpynA(char *lpString1, const char *lpString2, int iMaxLength)
{
	__asm
	{
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    ecx
		push    eax
		call    strlcpy
		add     esp, 12
		ret     12
	}
}
