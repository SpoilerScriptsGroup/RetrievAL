#include <string.h>

#pragma function(strcat, strlen, memcpy)

__declspec(naked) char * __cdecl strcat(char *string1, const char *string2)
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]                    // src
		push    eax
		call    strlen                                      // length of src
		inc     eax                                         // include terminating zero in length
		push    eax                                         // strlen(src)+1
		mov     edx, dword ptr [esp + 4 + 8]                // dest
		push    edx
		call    strlen                                      // length of dest
		pop     edx                                         // dest. Assume unchanged by _A_strlen
		add     edx, eax                                    // dest+strlen(dest)
		mov     ecx, dword ptr [esp + 8 + 8]                // src
		                                                    // strlen(src)+1 is on stack
		push    ecx                                         // src
		push    edx                                         // dest+strlen(dest)
		call    memcpy                                      // copy
		add     esp, 16                                     // clean up stack
		mov     eax, dword ptr [esp + 4]                    // return dest
		ret
	}
}
