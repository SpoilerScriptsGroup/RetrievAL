#include <string.h>

#pragma function(strcpy, strlen, memcpy)

__declspec(naked) char * __cdecl strcpy(char *string1, const char *string2)
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]                    // src
		push    eax
		call    strlen                                      // length of src
		pop     ecx                                         // ecx = src. Assume unchanged by _A_strlen
		inc     eax                                         // include terminating zero in length
		mov     edx, dword ptr [esp + 4]                    // dest
		push    eax                                         // length+1
		push    ecx                                         // src
		push    edx                                         // dest
		call    memcpy                                      // copy
		add     esp, 12                                     // clean up stack
		ret
	}
}
