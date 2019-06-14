#include <string.h>

size_t __cdecl strlcpy(char *string1, const char *string2, size_t count);

__declspec(naked) char * __stdcall _lstrcpynA(char *lpString1, const char *lpString2, int iMaxLength)
{
	__asm
	{
		#define lpString1  (esp + 4)
		#define lpString2  (esp + 8)
		#define iMaxLength (esp + 12)

		mov     edx, dword ptr [iMaxLength]
		mov     ecx, dword ptr [lpString2]
		mov     eax, dword ptr [lpString1]
		push    edx
		push    ecx
		push    eax
		call    strlcpy
		mov     eax, dword ptr [lpString1 + 12]
		add     esp, 12
		ret     12

		#undef lpString1
		#undef lpString2
		#undef iMaxLength
	}
}
