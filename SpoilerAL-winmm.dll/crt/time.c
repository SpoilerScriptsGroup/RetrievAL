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
__time32_t __cdecl _time32(__time32_t *timer)
{
	static __time32_t __cdecl internal_time32();

	__time32_t time;

	time = internal_time32();
	if (timer)
		*timer = time;
	return time;
}

__declspec(naked) static __time32_t __cdecl internal_time32()
{
	__asm
	{
		push    0
		push    0
		push    esp
		call    GetSystemTimeAsFileTime
		pop     eax
		pop     edx
		sub     eax, 0xD53E8000
		mov     ecx, 10000000
		sbb     edx, 0x019DB1DE
		div     ecx
		ret
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
__time64_t __cdecl _time64(__time64_t *timer)
{
	static __time64_t __cdecl internal_time64();

	__time64_t time;

	time = internal_time64();
	if (timer)
		*timer = time;
	return time;
}

__declspec(naked) static __time64_t __cdecl internal_time64()
{
	__asm
	{
		push    esi
		push    edi
		push    0
		push    0
		push    esp
		call    GetSystemTimeAsFileTime
		pop     ecx
		pop     esi
		sub     ecx, 0xD53E8000
		mov     eax, 0xE57A42BD
		sbb     esi, 0x019DB1DE
		push    edi
		mul     ecx
		mov     eax, 0xE57A42BD
		mov     edi, edx
		mul     esi
		xor     edi, edi
		add     edi, eax
		adc     edi, edx
		mov     eax, 0xD6BF94D5
		mul     ecx
		xor     ecx, ecx
		add     edi, eax
		adc     edi, edx
		mov     eax, 0xD6BF94D5
		adc     ecx, ecx
		mul     esi
		add     eax, edi
		pop     edi
		adc     ecx, edx
		pop     edi
		shr     eax, 23
		mov     edx, ecx
		shl     ecx, 9
		pop     esi
		shr     edx, 23
		or      eax, ecx
		ret
	}
}
#endif
