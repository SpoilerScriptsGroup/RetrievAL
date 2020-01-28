#include <errno.h>
#ifndef _M_IX86
#include <stdint.h>

#define I64(x)  (*(int64_t *)&(x))
#define UI64(x) (*(uint64_t *)&(x))

double __cdecl _nextafter(double x, double y)
{
	if ((UI64(x) & INT64_MAX) > 0x7FF0000000000000 ||                                       // x is NaN ?
	    (UI64(y) & INT64_MAX) > 0x7FF0000000000000)                                         // y is NaN ?
		return x + y;
	if (!(UI64(x) & INT64_MAX)) {                                                           // x == 0 ?
		UI64(y) = ((UI64(y) & INT64_MAX) != 0) | (UI64(y) & INT64_MIN);
		errno = ERANGE;
		return y;
	}
	if (UI64(x) == UI64(y))                                                                 // x == y ?
		return y;
	if (I64(x) < I64(y) && UI64(x) != 0xFFF0000000000000 && ~I64(x) < 0x7FEFFFFFFFFFFFFF)   // (x < y) == (x >= 0) && x != -HUGE_VAL && x > -DBL_MIN ?
		UI64(x)++;
	else
		UI64(x)--;
	if ((UI64(x) & INT64_MAX) >= 0x7FF0000000000000 ||                                      // overflow ?
	    (UI64(x) & INT64_MAX) < 0x0010000000000000)                                         // underflow ?
		errno = ERANGE;
	return x;
}
#else
__declspec(naked) double __cdecl _nextafter(double x, double y)
{
#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		#define X     (esp + 4)
		#define X_LSW (esp + 4)
		#define X_MSW (esp + 8)
		#define Y     (esp + 12)
		#define Y_LSW (esp + 12)
		#define Y_MSW (esp + 16)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebx, dword ptr [X_LSW + 16]
		mov     ecx, dword ptr [X_MSW + 16]
		mov     esi, dword ptr [Y_LSW + 16]
		mov     edx, dword ptr [Y_MSW + 16]
		mov     ebp, ecx                        // x is NaN ?
		and     ecx, 7FFFFFFFH
		cmp     ebx, 1
		mov     eax, ecx
		sbb     eax, 7FF00000H
		jae     L4
		mov     edi, edx                        // y is NaN ?
		and     edx, 7FFFFFFFH
		cmp     esi, 1
		mov     eax, edx
		sbb     eax, 7FF00000H
		jae     L4
		or      ecx, ebx                        // x == 0 ?
		jz      L6
		mov     eax, ebp                        // (x >= y) == (x >= 0) ?
		cmp     ebx, esi
		sbb     eax, edi
		jge     L1
		cmp     ebp, 0FFF00000H                 // x == -HUGE_VAL ?
		je      L2
		mov     eax, ebp                        // x <= -DBL_MIN ?
		mov     ecx, ebx
		xor     eax, -1
		add     ecx, -1
		sbb     eax, 7FEFFFFFH
		jge     L2
		add     ebx, 1                          // increment
		adc     ebp, 0
		jmp     L3

		align   16
	L1:
		mov     eax, edi                        // x == y ?
		cmp     esi, ebx
		sbb     eax, ebp
		jge     L5
	L2:
		sub     ebx, 1                          // decrement
		sbb     ebp, 0
	L3:
		lea     eax, [ebp + ebp]
		cmp     eax, 7FF00000H * 2              // overflow ?
		jae     L7
		cmp     eax, 00100000H * 2              // underflow ?
		jae     L8
		set_errno(ERANGE)
		jmp     L8

		align   16
	L4:
		fld     qword ptr [X + 16]
		fld     qword ptr [Y + 16]
		fadd
		jmp     L9

		align   16
	L5:
		fld     qword ptr [Y + 16]
		jmp     L9

		align   16
	L6:
		xor     ebp, ebp
		add     edi, edi
		rcr     ebp, 1
		or      esi, edi
		jz      L7
		inc     ebx
	L7:
		set_errno(ERANGE)
	L8:
		mov     dword ptr [esp - 8], ebx
		mov     dword ptr [esp - 4], ebp
		fld     qword ptr [esp - 8]
	L9:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef X
		#undef X_LSW
		#undef X_MSW
		#undef Y
		#undef Y_LSW
		#undef Y_MSW
	}

	#undef set_errno
}
#endif
