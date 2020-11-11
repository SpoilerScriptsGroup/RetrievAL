#include <windows.h>

extern HANDLE hHeap;

#ifndef _M_IX86
unsigned char * __cdecl _mbsrev(unsigned char *string)
{
	size_t         length;
	unsigned short *buffer;

	if ((length = strlen((const char *)string)) &&
		length <= (~(size_t)0 >> 1) &&
		(buffer = (unsigned short *)HeapAlloc(hHeap, 0, length * 2)))
	{
		unsigned char  *mbs;
		unsigned short *wcs, w;

		mbs = string;
		wcs = buffer;
		do
			*(wcs++) =
				!IsDBCSLeadByteEx(CP_THREAD_ACP, *mbs) || !mbs[1] ?
					*(mbs++) :
					*(((unsigned short *)mbs)++);
		while (*mbs);
		mbs = string;
		do
			if ((w = *(--wcs)) < 0x100)
				*(mbs++) = (unsigned char)w;
			else
				*(((unsigned short *)mbs)++) = w;
		while (wcs != buffer);
		HeapFree(hHeap, 0, buffer);
	}
	return string;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsrev(unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebx, dword ptr [string + 16]
		xor     eax, eax
		mov     edi, ebx
		mov     ecx, -1
		repne scasb
		dec     eax
		inc     ecx
		xor     eax, ecx
		mov     esi, ebx
		add     eax, eax
		jbe     L7                                          // CF=1 or ZF=1
		push    eax
		push    0
		push    dword ptr [hHeap]
		call    HeapAlloc
		test    eax, eax
		jz      L7
		mov     edi, eax
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		mov     ebp, edi

		align   16
	L1:
		mov     byte ptr [edi], al
		inc     edi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
	L2:
		mov     byte ptr [edi], al
		inc     edi
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jnz     L1
		jmp     L4

		align   16
	L3:
		mov     byte ptr [edi], al
		inc     edi
	L4:
		mov     ecx, ebx

		align   16
	L5:
		mov     ax, word ptr [edi - 2]
		sub     edi, 2
		mov     byte ptr [ecx], al
		inc     ecx
		test    ah, ah
		jz      L6
		mov     byte ptr [ecx], ah
		inc     ecx
	L6:
		cmp     edi, ebp
		jne     L5

		push    ebp
		push    0
		push    dword ptr [hHeap]
		call    HeapFree
	L7:
		mov     eax, ebx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
	}
}
#endif
