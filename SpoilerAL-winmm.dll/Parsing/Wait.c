#include <windows.h>
#include <stdint.h>
#include <intrin.h>

#pragma intrinsic(__emulu)

void __stdcall Wait(DWORD dwMilliseconds)
{
	static __forceinline uint32_t HundredNano64ToMilli32(uint64_t dwHundredNano);

	ULONGLONG qwNow, qwEnd;
	MSG       msg;

	if (!dwMilliseconds)
		return;
	if (dwMilliseconds != INFINITE)
	{
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
		qwEnd = qwNow + __emulu(dwMilliseconds, 10000);
	}
	do
	{
		if (MsgWaitForMultipleObjects(0, NULL, FALSE, dwMilliseconds, QS_ALLEVENTS) == WAIT_TIMEOUT)
			break;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				PostQuitMessage(msg.wParam);
				return;
			}
		if (dwMilliseconds == INFINITE)
			continue;
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
	} while (qwEnd > qwNow && (dwMilliseconds = HundredNano64ToMilli32(qwEnd - qwNow)));
}

static __forceinline uint32_t HundredNano64ToMilli32(uint64_t qwHundredNano)
{
	static uint32_t __fastcall InternalHundredNano64ToMilli32(uint32_t low, uint32_t high);

	return InternalHundredNano64ToMilli32((uint32_t)qwHundredNano, (uint32_t)(qwHundredNano >> 32));
}

__declspec(naked) static uint32_t __fastcall InternalHundredNano64ToMilli32(uint32_t low, uint32_t high)
{
	/* reciprocal divisor:
	 *   ((1 << 64) + 10000 - 1) / 10000 = ((1 << 64) + 0x270F) / 0x2710
	 *                                   = 0x1000000000000270 / 0x271
	 *                                   = 0x00068DB8BAC710CC
	 * division:
	 *   x / 10000 = (x * 0x00068DB8BAC710CC) >> 64
	 */
	__asm
	{
		cmp     edx, 0x00068DB8
		jb      L1
		ja      L2
		cmp     ecx, 0xBAC710CC
		jae     L2
	L1:
		mov     eax, ecx
		mov     ecx, 10000
		div     ecx
		ret

		align   16
	L2:
		push    ebx
		push    esi
		mov     ebx, edx
		mov     eax, 0xBAC710CC
		mul     ecx
		mov     esi, edx
		mov     eax, 0x00068DB8
		mul     ecx
		xor     ecx, ecx
		add     esi, eax
		adc     ecx, edx
		mov     eax, 0xBAC710CC
		mul     ebx
		imul    ebx, 0x00068DB8
		add     esi, eax
		mov     eax, ebx
		adc     ecx, edx
		pop     esi
		add     eax, ecx
		pop     ebx
		ret
	}
}
