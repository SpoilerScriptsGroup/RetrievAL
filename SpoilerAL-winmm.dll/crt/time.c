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
		pop     esi
		pop     ecx
		sub     esi, 0xD53E8000
		mov     eax, 0xE57A42BD
		sbb     ecx, 0x019DB1DE
		mul     esi
		mov     eax, 0xE57A42BD
		mov     edi, edx
		mul     ecx
		add     edi, eax
		mov     eax, esi
		mov     esi, edx
		mov     edx, 0xD6BF94D5
		adc     esi, 0
		mul     edx
		add     edi, eax
		mov     eax, ecx
		adc     esi, edx
		mov     ecx, 0
		adc     ecx, 0
		mov     edx, 0xD6BF94D5
		mul     edx
		add     eax, esi
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
