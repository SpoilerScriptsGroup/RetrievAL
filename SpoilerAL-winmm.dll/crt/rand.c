/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using srand(seed).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include <stdint.h>
#if defined(_M_IX86) || defined(_M_X64)
#include <emmintrin.h>
#endif
#include <assert.h>

/*-----------------
  BASIC DEFINITIONS
  -----------------*/
#define SFMT_MEXP       19937                   // Mersenne Exponent. The period of the sequence is a multiple of 2^MEXP-1.
#define SFMT_N          (SFMT_MEXP / 128 + 1)   // SFMT generator has an internal state array of 128-bit integers, and N is its size.
#define SFMT_N32        (SFMT_N * 4)            // N32 is the size of internal state array when regarded as an array of 32-bit integers.

/*----------------------
  the parameters of SFMT
  ----------------------*/
#define SFMT_POS1       122                     // the pick up position of the array.
#define SFMT_SL1        18                      // the parameter of shift left as four 32-bit registers.
#define SFMT_SL2        1                       // the parameter of shift left as one 128-bit register. The 128-bit integer is shifted by (SFMT_SL2 * 8) bits.
#define SFMT_SR1        11                      // the parameter of shift right as four 32-bit registers.
#define SFMT_SR2        1                       // the parameter of shift right as one 128-bit register. The 128-bit integer is shifted by (SFMT_SR2 * 8) bits.
#define SFMT_MSK1       0xDFFFFFEFU             // A bitmask, used in the recursion.  These parameters are introduced to break symmetry of SIMD.
#define SFMT_MSK2       0xDDFECB7FU             //
#define SFMT_MSK3       0xBFFAFFFFU             //
#define SFMT_MSK4       0xBFFFFFF6U             //
#define SFMT_PARITY1    0x00000001U             // These definitions are part of a 128-bit period certification vector. */
#define SFMT_PARITY2    0x00000000U             //
#define SFMT_PARITY3    0x00000000U             //
#define SFMT_PARITY4    0x13C9E684U             //

/*------------------------------------------
  128-bit SIMD like data type for standard C
  ------------------------------------------*/
/* 128-bit data structure */
typedef union __declspec(align(16)) {
	uint32_t    u[4];
	uint64_t    u64[2];
#if defined(_M_IX86) || defined(_M_X64)
	__m128i     si;
#endif
} w128_t;

/*---------
  VARIABLES
  ---------*/
/* SFMT internal state */
static w128_t   state[SFMT_N];                  // the 128-bit internal state array
static size_t   idx;                            // index counter to the 32-bit internal state array

#define sfmt   ((__m128i *)state)
#define sfmt32 ((uint32_t *)state)

/*------
  MACROS
  ------*/
#define _BSF8(x, default) ( \
    ((x) & 0x01) ?  0 :     \
    ((x) & 0x02) ?  1 :     \
    ((x) & 0x04) ?  2 :     \
    ((x) & 0x08) ?  3 :     \
    ((x) & 0x10) ?  4 :     \
    ((x) & 0x20) ?  5 :     \
    ((x) & 0x40) ?  6 :     \
    ((x) & 0x80) ?  7 :     \
    (default))
#define _BSF16(x, default) _BSF8(x, _BSF8((x) >> 8, (default) - 8) + 8)
#define _BSF32(x, default) _BSF16(x, _BSF16((x) >> 16, (default) - 16) + 16)
#define BSF8(x) _BSF8(x, -1)
#define BSF16(x) _BSF16(x, -1)
#define BSF32(x) _BSF32(x, -1)

/*----------------
  STATIC FUNCTIONS
  ----------------*/
#if defined(_M_IX86)
static void sfmt_gen_rand_all_sse2();
static void sfmt_gen_rand_all_generic();
static void sfmt_gen_rand_all_cpu_dispatch();

static void (*sfmt_gen_rand_all_dispatch)() = sfmt_gen_rand_all_cpu_dispatch;
#endif

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all()
{
	__asm
	{
		jmp     dword ptr [sfmt_gen_rand_all_dispatch]
	}
}
#endif

#if defined(_M_X64)
/* This function represents the recursion formula. */
#define mm_recursion(r2, r3, r4, p, index)  \
do {                                        \
    __m128i r0, r1;                         \
                                            \
    r1 = _mm_load_si128(p);                 \
    r0 = r2;                                \
    r3 = _mm_srli_si128(r3, SFMT_SR2);      \
    r3 = _mm_xor_si128(r3, r1);             \
    r0 = _mm_slli_epi32(r0, SFMT_SL1);      \
    r1 = _mm_slli_si128(r1, SFMT_SL2);      \
    r0 = _mm_xor_si128(r0, r3);             \
    r3 = r2;                                \
    r2 = _mm_load_si128(p + index);         \
    r1 = _mm_xor_si128(r1, r0);             \
    r2 = _mm_srli_epi32(r2, SFMT_SR1);      \
    r2 = _mm_and_si128(r2, r4);             \
    r2 = _mm_xor_si128(r2, r1);             \
    _mm_store_si128(p, r2);                 \
} while (0)

/* This function fills the internal state array with pseudorandom
   integers. */
static void sfmt_gen_rand_all()
{
	static const w128_t mask = { {
		SFMT_MSK1 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK2 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK3 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK4 & (UINT32_MAX >> SFMT_SR1) } };

	__m128i r2, r3, r4;
	ptrdiff_t offset;

	assert((size_t)sfmt % 16 == 0);
	assert((size_t)&mask % 16 == 0);

	r2 = _mm_load_si128(sfmt + SFMT_N - 1);
	r3 = _mm_load_si128(sfmt + SFMT_N - 2);
	r4 = _mm_load_si128(&mask.si);
	offset = -(SFMT_N - SFMT_POS1) * 16;
	do
		mm_recursion(r2, r3, r4,
			(__m128i *)((char *)(sfmt + SFMT_N - SFMT_POS1) + offset),
			SFMT_POS1);
	while (offset += 16);
	offset = -SFMT_POS1 * 16;
	do
		mm_recursion(r2, r3, r4,
			(__m128i *)((char *)(sfmt + SFMT_N) + offset),
			-(SFMT_N - SFMT_POS1));
	while (offset += 16);
}
#elif defined(_M_IX86)
/* This function fills the internal state array with pseudorandom
   integers. */
__declspec(naked) static void __cdecl sfmt_gen_rand_all_sse2()
{
	static const w128_t mask = { {
		SFMT_MSK1 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK2 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK3 & (UINT32_MAX >> SFMT_SR1),
		SFMT_MSK4 & (UINT32_MAX >> SFMT_SR1) } };

	__asm
	{
		movdqa  xmm2, xmmword ptr [state + (SFMT_N - 1) * 16]
		movdqa  xmm3, xmmword ptr [state + (SFMT_N - 2) * 16]
		movdqa  xmm4, xmmword ptr [mask]
		mov     eax, -(SFMT_N - SFMT_POS1) * 16

		align   16
	loop1:
		movdqa  xmm1, xmmword ptr [state + (SFMT_N - SFMT_POS1) * 16 + eax]
		movdqa  xmm0, xmm2
		psrldq  xmm3, SFMT_SR2
		pxor    xmm3, xmm1
		pslld   xmm0, SFMT_SL1
		pslldq  xmm1, SFMT_SL2
		pxor    xmm0, xmm3
		movdqa  xmm3, xmm2
		movdqa  xmm2, xmmword ptr [state + SFMT_N * 16 + eax]
		pxor    xmm1, xmm0
		psrld   xmm2, SFMT_SR1
		pand    xmm2, xmm4
		pxor    xmm2, xmm1
		movdqa  xmmword ptr [state + (SFMT_N - SFMT_POS1) * 16 + eax], xmm2
		add     eax, 16
		jnz     loop1

		mov     eax, -SFMT_POS1 * 16

		align   16
	loop2:
		movdqa  xmm1, xmmword ptr [state + SFMT_N * 16 + eax]
		movdqa  xmm0, xmm2
		psrldq  xmm3, SFMT_SR2
		pxor    xmm3, xmm1
		pslld   xmm0, SFMT_SL1
		pslldq  xmm1, SFMT_SL2
		pxor    xmm0, xmm3
		movdqa  xmm3, xmm2
		movdqa  xmm2, xmmword ptr [state + SFMT_POS1 * 16 + eax]
		pxor    xmm1, xmm0
		psrld   xmm2, SFMT_SR1
		pand    xmm2, xmm4
		pxor    xmm2, xmm1
		movdqa  xmmword ptr [state + SFMT_N * 16 + eax], xmm2
		add     eax, 16
		jnz     loop2

		ret
	}
}
#endif

#if !defined(_M_X64)
/* This function represents the recursion formula. */
#if !defined(_M_IX86)
#define do_recursion(a, b, c, d)                        \
do {                                                    \
    w128_t __x;                                         \
                                                        \
    __x.u64[1] = ((a)->u64[1] << (SFMT_SL2 * 8)) |      \
                 ((a)->u64[0] >> (64 - SFMT_SL2 * 8));  \
    __x.u64[0] =  (a)->u64[0] << (SFMT_SL2 * 8);        \
    __x.u[0] ^= ((b)->u[0] >> SFMT_SR1) & SFMT_MSK1;    \
    __x.u[1] ^= ((b)->u[1] >> SFMT_SR1) & SFMT_MSK2;    \
    __x.u[2] ^= ((b)->u[2] >> SFMT_SR1) & SFMT_MSK3;    \
    __x.u[3] ^= ((b)->u[3] >> SFMT_SR1) & SFMT_MSK4;    \
    __x.u64[0] ^= ((c)->u64[0] >> (SFMT_SR2 * 8)) |     \
                  ((c)->u64[1] << (64 - SFMT_SR2 * 8)); \
    __x.u64[1] ^=  (c)->u64[1] >> (SFMT_SR2 * 8);       \
    __x.u[0] ^= (d)->u[0] << SFMT_SL1;                  \
    __x.u[1] ^= (d)->u[1] << SFMT_SL1;                  \
    __x.u[2] ^= (d)->u[2] << SFMT_SL1;                  \
    __x.u[3] ^= (d)->u[3] << SFMT_SL1;                  \
    (a)->u64[0] ^= __x.u64[0];                          \
    (a)->u64[1] ^= __x.u64[1];                          \
} while (0)
#else
__declspec(naked) static void __cdecl do_recursion(w128_t *a, w128_t *b, w128_t *c, w128_t *d)
{
	__asm
	{
		#define a (esp + 4)
		#define b (esp + 8)
		#define c (esp + 12)
		#define d (esp + 16)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebp, dword ptr [a   + 16]

		// x.u[3] = (a->u[3] << (SFMT_SL2 * 8)) | (a->u[2] >> (32 - SFMT_SL2 * 8));
		// x.u[2] = (a->u[2] << (SFMT_SL2 * 8)) | (a->u[1] >> (32 - SFMT_SL2 * 8));
		// x.u[1] = (a->u[1] << (SFMT_SL2 * 8)) | (a->u[0] >> (32 - SFMT_SL2 * 8));
		// x.u[0] =  a->u[0] << (SFMT_SL2 * 8);
		mov     eax, dword ptr [ebp     ]
		mov     ecx, dword ptr [ebp +  4]
		mov     edx, dword ptr [ebp +  8]
		mov     ebx, dword ptr [ebp + 12]
		shl     ebx, SFMT_SL2 * 8
		mov     esi, edx
		shr     esi, 32 - SFMT_SL2 * 8
		mov     edi, ecx
		shl     edx, SFMT_SL2 * 8
		or      ebx, esi
		shr     edi, 32 - SFMT_SL2 * 8
		mov     esi, eax
		shl     ecx, SFMT_SL2 * 8
		or      edx, edi
		shr     esi, 32 - SFMT_SL2 * 8
		mov     ebp, dword ptr [b   + 16]
		shl     eax, SFMT_SL2 * 8
		or      ecx, esi

		// x.u[0] ^= (b->u[0] >> SFMT_SR1) & SFMT_MSK1;
		// x.u[1] ^= (b->u[1] >> SFMT_SR1) & SFMT_MSK2;
		// x.u[2] ^= (b->u[2] >> SFMT_SR1) & SFMT_MSK3;
		// x.u[3] ^= (b->u[3] >> SFMT_SR1) & SFMT_MSK4;
		mov     esi, dword ptr [ebp     ]
		mov     edi, dword ptr [ebp +  4]
		shr     esi, SFMT_SR1
		shr     edi, SFMT_SR1
		and     esi, SFMT_MSK1 and (INT32_MAX shr (SFMT_SR1 - 1))
		and     edi, SFMT_MSK2 and (INT32_MAX shr (SFMT_SR1 - 1))
		xor     eax, esi
		mov     esi, dword ptr [ebp +  8]
		xor     ecx, edi
		shr     esi, SFMT_SR1
		mov     edi, dword ptr [ebp + 12]
		shr     edi, SFMT_SR1
		mov     ebp, dword ptr [c   + 16]
		and     esi, SFMT_MSK3 and (INT32_MAX shr (SFMT_SR1 - 1))
		and     edi, SFMT_MSK4 and (INT32_MAX shr (SFMT_SR1 - 1))
		xor     edx, esi
		xor     ebx, edi

		// x.u[0] ^= (c->u[0] >> (SFMT_SR2 * 8)) | (c->u[1] << (32 - SFMT_SR2 * 8));
		// x.u[1] ^= (c->u[1] >> (SFMT_SR2 * 8)) | (c->u[2] << (32 - SFMT_SR2 * 8));
		// x.u[2] ^= (c->u[2] >> (SFMT_SR2 * 8)) | (c->u[3] << (32 - SFMT_SR2 * 8));
		// x.u[3] ^=  c->u[3] >> (SFMT_SR2 * 8);
		mov     esi, dword ptr [ebp     ]
		mov     edi, dword ptr [ebp +  4]
		ror     edi, SFMT_SR2 * 8
		shr     esi, SFMT_SR2 * 8
		xor     eax, edi
		and     edi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		xor     eax, esi
		xor     ecx, edi
		xor     eax, edi
		mov     esi, dword ptr [ebp +  8]
		mov     edi, dword ptr [ebp + 12]
		ror     esi, SFMT_SR2 * 8
		ror     edi, SFMT_SR2 * 8
		mov     ebp, dword ptr [d   + 16]
		xor     ecx, esi
		xor     edx, edi
		and     esi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		and     edi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		xor     edx, esi
		xor     ecx, esi
		mov     esi, dword ptr [ebp     ]
		xor     edx, edi
		xor     ebx, edi

		// x.u[0] ^= d->u[0] << SFMT_SL1;
		// x.u[1] ^= d->u[1] << SFMT_SL1;
		// x.u[2] ^= d->u[2] << SFMT_SL1;
		// x.u[3] ^= d->u[3] << SFMT_SL1;
		shl     esi, SFMT_SL1
		mov     edi, dword ptr [ebp +  4]
		shl     edi, SFMT_SL1
		xor     eax, esi
		mov     esi, dword ptr [ebp +  8]
		xor     ecx, edi
		shl     esi, SFMT_SL1
		mov     edi, dword ptr [ebp + 12]
		shl     edi, SFMT_SL1
		mov     ebp, dword ptr [a   + 16]
		xor     edx, esi
		xor     ebx, edi

		// a->u[0] ^= x.u[0];
		// a->u[1] ^= x.u[1];
		// a->u[2] ^= x.u[2];
		// a->u[3] ^= x.u[3];
		xor     dword ptr [ebp     ], eax
		xor     dword ptr [ebp +  4], ecx
		xor     dword ptr [ebp +  8], edx
		xor     dword ptr [ebp + 12], ebx

		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef a
		#undef b
		#undef c
		#undef d
	}
}
#endif

/* This function fills the internal state array with pseudorandom
   integers. */
#if !defined(_M_IX86)
static void sfmt_gen_rand_all()
{
	w128_t *a, *b, *c, *d;

	a = state;
	b = state + SFMT_POS1;
	c = state + SFMT_N - 2;
	d = state + SFMT_N - 1;
	do {
		do_recursion(a, b, c, d);
		c = d;
		d = a;
		a++;
		b++;
	} while (a != state + SFMT_N - SFMT_POS1);
	b = state;
	do {
		do_recursion(a, b, c, d);
		c = d;
		d = a;
		a++;
		b++;
	} while (a != state + SFMT_N);
}
#else
__declspec(naked) static void sfmt_gen_rand_all_generic()
{
	__asm
	{
		push    ebx
		push    esi
		push    edi
		mov     ebx, offset state
		mov     esi, offset state + SFMT_POS1 * 16
		mov     eax, offset state + (SFMT_N - 2) * 16
		mov     edi, offset state + (SFMT_N - 1) * 16
		sub     esp, 16

		align   16
	loop1:
		mov     dword ptr [esp     ], ebx
		mov     dword ptr [esp +  4], esi
		mov     dword ptr [esp +  8], eax
		mov     dword ptr [esp + 12], edi
		call    do_recursion
		mov     eax, edi
		mov     edi, ebx
		add     ebx, 16
		add     esi, 16
		cmp     ebx, offset state + (SFMT_N - SFMT_POS1) * 16
		jne     loop1

		mov     esi, offset state

		align   16
	loop2:
		mov     dword ptr [esp     ], ebx
		mov     dword ptr [esp +  4], esi
		mov     dword ptr [esp +  8], eax
		mov     dword ptr [esp + 12], edi
		call    do_recursion
		mov     eax, edi
		mov     edi, ebx
		add     ebx, 16
		add     esi, 16
		cmp     ebx, offset state + SFMT_N * 16
		jne     loop2

		mov     edi, dword ptr [esp + 16]
		add     esp, 20
		pop     esi
		pop     ebx
		ret
	}
}
#endif
#endif

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all_cpu_dispatch()
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [sfmt_gen_rand_all_dispatch], offset sfmt_gen_rand_all_generic
		jmp     sfmt_gen_rand_all_generic

	L1:
		mov     dword ptr [sfmt_gen_rand_all_dispatch], offset sfmt_gen_rand_all_sse2
		jmp     sfmt_gen_rand_all_sse2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

/*----------------
  PUBLIC FUNCTIONS
  ----------------*/
uint32_t __cdecl rand32();

/* This function initializes the internal state array with a 32-bit
   integer seed. */
void __cdecl srand(unsigned int seed)
{
	uint32_t x;
	size_t i;

	sfmt32[0] = x = seed;
	for (i = 1; i < SFMT_N32; i++)
		sfmt32[i] = x = ((x >> 30) ^ x) * 1812433253UL + (uint32_t)i;
	idx = SFMT_N32;
	/* certificate the period of 2^{MEXP} */
	x =  sfmt32[0] & SFMT_PARITY1;
	x ^= sfmt32[1] & SFMT_PARITY2;
	x ^= sfmt32[2] & SFMT_PARITY3;
	x ^= sfmt32[3] & SFMT_PARITY4;
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	x &= 1;
#if SFMT_PARITY1
	sfmt32[0] ^= x << BSF32(SFMT_PARITY1);
#elif SFMT_PARITY2
	sfmt32[1] ^= x << BSF32(SFMT_PARITY2);
#elif SFMT_PARITY3
	sfmt32[2] ^= x << BSF32(SFMT_PARITY3);
#elif SFMT_PARITY4
	sfmt32[3] ^= x << BSF32(SFMT_PARITY4);
#endif
}

int __cdecl rand()
{
	return rand32() & 0x00007FFF;
}

uint8_t __cdecl rand8()
{
	return (uint8_t)rand32();
}

uint16_t __cdecl rand16()
{
	return (uint16_t)rand32();
}

/* This function generates and returns 32-bit pseudorandom number.
   srand must be called before this function. */
uint32_t __cdecl rand32()
{
	if (idx >= SFMT_N32) {
		sfmt_gen_rand_all();
		idx = 0;
	}
	return sfmt32[idx++];
}

/* This function generates and returns 64-bit pseudorandom number.
   srand must be called before this function. */
uint64_t __cdecl rand64()
{
	uint64_t r;

	idx = (idx + 1) & -2;
	if (idx >= SFMT_N32) {
		sfmt_gen_rand_all();
		idx = 0;
	}
	r = *(uint64_t *)(sfmt32 + idx);
	idx += 2;
	return r;
}

float __cdecl randf32()
{
	uint32_t r;

	r = ((rand32() & 1) << 31) | ((rand32() & 0xFF) << 23) | (rand32() & 0x007FFFFF);
	return *(float *)&r;
}

double __cdecl randf64()
{
	uint64_t r;

	r = ((uint64_t)(rand32() & 1) << 63) | ((uint64_t)(rand32() & 0x07FF) << 52) | (rand64() & 0x000FFFFFFFFFFFFF);
	return *(double *)&r;
}
