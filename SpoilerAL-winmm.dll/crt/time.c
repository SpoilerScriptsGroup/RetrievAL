#include <windows.h>

#ifndef _M_IX86
__time32_t __cdecl _time32(__time32_t *timer)
{
	ULONGLONG time;

	GetSystemTimeAsFileTime((LPFILETIME)&time);
	time = (time - 116444736000000000) / 10000000;
	if (timer)
		*timer = (__time32_t)time;
	return (__time32_t)time;
}
#else
__declspec(naked) __time32_t __cdecl _time32(__time32_t *timer)
{
	__asm
	{
		#define timer (esp + 4)

		push    0
		push    0
		push    esp
		call    GetSystemTimeAsFileTime
		pop     eax
		pop     edx
		sub     eax, 0D53E8000H
		mov     ecx, 10000000
		sbb     edx, 0019DB1DEH
		div     ecx
		mov     ecx, dword ptr [timer]
		test    ecx, ecx
		jz      L1
		mov     dword ptr [ecx], eax
	L1:
		ret

		#undef timer
	}
}
#endif

#ifndef _M_IX86
__time64_t __cdecl _time64(__time64_t *timer)
{
	ULONGLONG time;

	GetSystemTimeAsFileTime((LPFILETIME)&time);
	time = (time - 116444736000000000) / 10000000;
	if (timer)
		*timer = (__time64_t)time;
	return (__time64_t)time;
}
#else
__declspec(naked) __time64_t __cdecl _time64(__time64_t *timer)
{
	__asm
	{
		#define timer (esp + 4)

		push    ebx
		push    0
		push    0
		push    esp
		call    GetSystemTimeAsFileTime
		pop     ebx
		pop     eax
		sub     ebx, 0D53E8000H
		mov     ecx, 10000000
		sbb     eax, 0019DB1DEH
		xor     edx, edx
		div     ecx
		push    eax
		mov     eax, ebx
		div     ecx
		pop     edx
		pop     ebx
		mov     ecx, dword ptr [timer]
		test    ecx, ecx
		jz      L1
		mov     dword ptr [ecx], eax
		mov     dword ptr [ecx + 4], edx
	L1:
		ret

		#undef timer
	}
}
#endif
