/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

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

#include "rand.h"
#include <emmintrin.h>

/* definitions */
#define MEXP    19937               // Mersenne Exponent. The period of the sequence is a multiple of 2^MEXP-1.
#define N       (MEXP / 128 + 1)    // SFMT generator has an internal state array of 128-bit integers, and N is its size.
#define N32     (N * 4)             // N32 is the size of internal state array when regarded as an array of 32-bit integers.
#define POS     122                 // the pick up position of the array.
#define POS32   (POS * 4)           //
#define SL1     18                  // the parameter of shift left as four 32-bit registers.
#define SL2     1                   // the parameter of shift left as one 128-bit register. The 128-bit integer is shifted by (SL2 * 8) bits.
#define SR1     11                  // the parameter of shift right as four 32-bit registers.
#define SR2     1                   // the parameter of shift right as one 128-bit register. The 128-bit integer is shifted by (SR2 * 8) bits.
#define MSK1    0xDFFFFFEFU         // A bitmask, used in the recursion.  These parameters are introduced to break symmetry of SIMD.
#define MSK2    0xDDFECB7FU         //
#define MSK3    0xBFFAFFFFU         //
#define MSK4    0xBFFFFFF6U         //
#define PARITY1 0x00000001U         // These definitions are part of a 128-bit period certification vector. */
#define PARITY2 0x00000000U         //
#define PARITY3 0x00000000U         //
#define PARITY4 0x13C9E684U         //

/* variables */
#if defined(_M_IX86) || defined(_M_X64)
__declspec(align(16))
static __m128i si[N];               // the array for the state vector
#define        mt ((uint32_t *)si)
#else
uint32_t       mt[N32];             // the array for the state vector
#endif
static size_t  mti;                 // index of mt array

/* functions */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_sse2();
static void sfmt_gen_rand_all_generic();
static void sfmt_gen_rand_all_cpu_dispatch();

static void (*sfmt_gen_rand_all_dispatch)() = sfmt_gen_rand_all_cpu_dispatch;
#endif

/* This function certificate the period of 2^{MEXP} */
static void period_certification()
{
	const uint32_t parity[4] = { PARITY1, PARITY2, PARITY3, PARITY4 };
	uint32_t inner;
	size_t i, j;
	uint32_t work;

	inner = 0;
	for (i = 0; i < 4; i++)
		inner ^= mt[i] & parity[i];
	for (i = 16; i; i >>= 1)
		inner ^= inner >> i;
	inner &= 1;
	/* check OK */
	if (inner == 1)
		return;
	/* check NG, and modification */
	for (i = 0; i < 4; i++) {
		work = 1;
		for (j = 0; j < 32; j++) {
			if ((work & parity[i]) != 0) {
				mt[i] ^= work;
				return;
			}
			work = work << 1;
		}
	}
}

void __cdecl srand(unsigned int seed)
{
	size_t i;

	mt[0] = seed;
	for (i = 1; i < N32; i++)
	    mt[i] = 1812433253UL * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
	mti = N32;
	period_certification();
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

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all()
{
	__asm
	{
		jmp     dword ptr [sfmt_gen_rand_all_dispatch]
	}
}
#endif

#if defined(_M_IX86) || defined(_M_X64)
__declspec(align(16)) static const uint32_t sse2_param_mask[4] = { MSK1, MSK2, MSK3, MSK4 };

/* This function represents the recursion formula. */
#define mm_recursion(r, a, b, c, d)                     \
do {                                                    \
    __m128i v, x, y, z;                                 \
                                                        \
    y = _mm_srli_epi32(b, SR1);                         \
    z = _mm_srli_si128(c, SR2);                         \
    v = _mm_slli_epi32(d, SL1);                         \
    z = _mm_xor_si128(z, a);                            \
    z = _mm_xor_si128(z, v);                            \
    x = _mm_slli_si128(a, SL2);                         \
    y = _mm_and_si128(y, *(__m128i *)sse2_param_mask);  \
    z = _mm_xor_si128(z, x);                            \
    z = _mm_xor_si128(z, y);                            \
    *(r) = z;                                           \
} while (0)

/*
 * This function fills the internal state array with pseudorandom
 * integers.
 */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_sse2()
#else
static void sfmt_gen_rand_all()
#endif
{
	__m128i r1, r2;
	size_t i;

	r1 = si[N - 2];
	r2 = si[N - 1];
	for (i = 0; i < N - POS; i++) {
		mm_recursion(si + i, si[i], si[i + POS], r1, r2);
		r1 = r2;
		r2 = si[i];
	}
	for (; i < N; i++) {
		mm_recursion(si + i, si[i], si[i + POS - N], r1, r2);
		r1 = r2;
		r2 = si[i];
	}
}
#endif

#if !defined(_M_X64)
/*
 * This function simulates SIMD 128-bit left shift by the standard C.
 * The 128-bit integer given in in is shifted by (shift * 8) bits.
 * This function simulates the LITTLE ENDIAN SIMD.
 */
#define lshift128(out, in, shift)                       \
do {                                                    \
    uint64_t oh, ol;                                    \
                                                        \
    oh = ((uint64_t *)(in))[1] << ((shift) * 8);        \
    ol = ((uint64_t *)(in))[0] << ((shift) * 8);        \
    oh |= ((uint64_t *)(in))[0] >> (64 - (shift) * 8);  \
    ((uint64_t *)(out))[0] = ol;                        \
    ((uint64_t *)(out))[1] = oh;                        \
} while (0)

/*
 * This function simulates SIMD 128-bit right shift by the standard C.
 * The 128-bit integer given in in is shifted by (shift * 8) bits.
 * This function simulates the LITTLE ENDIAN SIMD.
 */
#define rshift128(out, in, shift)                       \
do {                                                    \
    uint64_t oh, ol;                                    \
                                                        \
    oh = ((uint64_t *)(in))[1] >> ((shift) * 8);        \
    ol = ((uint64_t *)(in))[0] >> ((shift) * 8);        \
    ol |= ((uint64_t *)(in))[1] << (64 - (shift) * 8);  \
    ((uint64_t *)(out))[0] = ol;                        \
    ((uint64_t *)(out))[1] = oh;                        \
} while (0)

/* This function represents the recursion formula. */
#define do_recursion(r, a, b, c, d)                                             \
do {                                                                            \
    uint32_t x[4];                                                              \
    uint32_t y[4];                                                              \
                                                                                \
    lshift128(x, a, SL2);                                                       \
    rshift128(y, c, SR2);                                                       \
    (r)[0] = (a)[0] ^ x[0] ^ (((b)[0] >> SR1) & MSK1) ^ y[0] ^ ((d)[0] << SL1); \
    (r)[1] = (a)[1] ^ x[1] ^ (((b)[1] >> SR1) & MSK2) ^ y[1] ^ ((d)[1] << SL1); \
    (r)[2] = (a)[2] ^ x[2] ^ (((b)[2] >> SR1) & MSK3) ^ y[2] ^ ((d)[2] << SL1); \
    (r)[3] = (a)[3] ^ x[3] ^ (((b)[3] >> SR1) & MSK4) ^ y[3] ^ ((d)[3] << SL1); \
} while (0)

/*
 * This function fills the internal state array with pseudorandom
 * integers.
 */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_generic()
#else
static void sfmt_gen_rand_all()
#endif
{
	size_t i;
	uint32_t *r1, *r2;

	r1 = mt + N32 - 8;
	r2 = mt + N32 - 4;
	for (i = 0; i < N32 - POS32; i += 4) {
		do_recursion(mt + i, mt + i, mt + i + POS32, r1, r2);
		r1 = r2;
		r2 = mt + i;
	}
	for (; i < N32; i += 4) {
		do_recursion(mt + i, mt + i, mt + i + POS32 - N32, r1, r2);
		r1 = r2;
		r2 = mt + i;
	}
}
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

/*
 * This function generates and returns 32-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 */
uint32_t __cdecl rand32()
{
	if (mti >= N32) {
		sfmt_gen_rand_all();
		mti = 0;
	}
	return mt[mti++];
}

/*
 * This function generates and returns 64-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 * The function gen_rand64 should not be called after gen_rand32,
 * unless an initialization is again executed.
 */
uint64_t __cdecl rand64()
{
	uint64_t r;

	mti = (mti + 1) & -2;
	if (mti >= N32) {
		sfmt_gen_rand_all();
		mti = 0;
	}
	r = *(uint64_t *)(mt + mti);
	mti += 2;
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
