#include <errno.h>
#ifndef _M_IX86
#include <stdint.h>

#define I64(x) (*(int64_t *)&(x))
#define U64(x) (*(uint64_t *)&(x))

double __cdecl _nextafter(double x, double y)
{
	uint64_t a, b;

	if ((a = (U64(x) & INT64_MAX)) > 0x7FF0000000000000 ||                                  // x is NaN ?
	    (b = (U64(y) & INT64_MAX)) > 0x7FF0000000000000)                                    // y is NaN ?
#if 0
		return x + y;	// Not add due to compiler optimization.
#elif !defined(_M_IX86)
		return (a | 0x0008000000000000) >= (b | 0x0008000000000000) ? x : y;
#else
		{ { __asm fld x __asm fld y __asm fadd __asm fstp x } return x; }
#endif
	if (!a) {                                                                               // x == 0 ?
		U64(y) = (b != 0) | (U64(y) & INT64_MIN);
		errno = ERANGE;
		return y;
	}
	if (U64(x) == U64(y))                                                                   // x == y ?
		return y;
	if (I64(x) < I64(y) && U64(x) != 0xFFF0000000000000 && ~I64(x) < 0x7FEFFFFFFFFFFFFF)    // (x < y) == (x >= 0) && x != -HUGE_VAL && x > -DBL_MIN ?
		U64(x)++;
	else
		U64(x)--;
	if ((U64(x) & INT64_MAX) >= 0x7FF0000000000000 ||                                       // overflow ?
	    (U64(x) & INT64_MAX) < 0x0010000000000000)                                          // underflow ?
		errno = ERANGE;
	return x;
}
#else
__declspec(naked) double __cdecl _nextafter(double x, double y)
{
#ifndef DISABLE_UCRT
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
		#define x      (esp + 4)
		#define y      (esp + 12)
		#define LSW(x) ((x) + 0)
		#define MSW(x) ((x) + 4)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebx, dword ptr [LSW(x) + 16]
		mov     ecx, dword ptr [MSW(x) + 16]
		mov     esi, dword ptr [LSW(y) + 16]
		mov     edx, dword ptr [MSW(y) + 16]
		mov     ebp, ecx                        // x is NaN ?
		and     ecx, 7FFFFFFFH
		cmp     ebx, 1
		mov     eax, ecx
		sbb     eax, 7FF00000H
		jae     L5
		mov     edi, edx                        // y is NaN ?
		and     edx, 7FFFFFFFH
		cmp     esi, 1
		mov     eax, edx
		sbb     eax, 7FF00000H
		jae     L5
		or      ecx, ebx                        // x == 0 ?
		jz      L7
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
		jge     L6
	L2:
		sub     ebx, 1                          // decrement
		sbb     ebp, 0
	L3:
		lea     eax, [ebp + ebp]
		cmp     eax, 7FF00000H * 2              // overflow ?
		jae     L4
		cmp     eax, 00100000H * 2              // underflow ?
		jae     L8
	L4:
		set_errno(ERANGE)
		jmp     L8

		align   16
	L5:
		fld     qword ptr [x + 16]
		fld     qword ptr [y + 16]
		fadd
		jmp     L9

		align   16
	L6:
		fld     qword ptr [y + 16]
		jmp     L9

		align   16
	L7:
		xor     ebp, ebp
		add     edi, edi
		rcr     ebp, 1
		xor     ebx, ebx
		set_errno(ERANGE)
		or      esi, edi
		setnz   bl
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

		#undef x
		#undef y
		#undef LSW
		#undef MSW
	}

	#undef set_errno
}
#endif
