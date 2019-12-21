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

#include <stdio.h>
#include <stdint.h>

/* Period parameters */
#define MT_N          624
#define MT_M          397
#define MT_MATRIX_A   0x9908B0DF            /* constant vector a */
#define MT_UPPER_MASK INT32_MIN             /* most significant w - r bits */
#define MT_LOWER_MASK INT32_MAX             /* least significant r bits */
#define SFMT_MEXP     19937                 /* Mersenne Exponent. The period of the sequence is a multiple of 2^MEXP-1. */
#define SFMT_N        (SFMT_MEXP / 128 + 1) /* SFMT generator has an internal state array of 128-bit integers, and MT_N is its size. */
#define SFMT_N32      (SFMT_N * 4)          /* N32 is the size of internal state array when regarded as an array of 32-bit integers. */
#define SFMT_POS1     122
#define SFMT_SL1      18
#define SFMT_SL2      1
#define SFMT_SR1      11
#define SFMT_SR2      1
#define SFMT_MSK1     0xDFFFFFEFU
#define SFMT_MSK2     0xDDFECB7FU
#define SFMT_MSK3     0xBFFAFFFFU
#define SFMT_MSK4     0xBFFFFFF6U
#define SFMT_PARITY1  0x00000001U
#define SFMT_PARITY2  0x00000000U
#define SFMT_PARITY3  0x00000000U
#define SFMT_PARITY4  0x13C9E684U

/* 128-bit data structure */
typedef union {
	uint32_t u[4];
	uint64_t u64[2];
} w128_t;

/* SFMT internal state */
typedef struct {
	/* the 128-bit internal state array */
	w128_t state[SFMT_N];
	/* index counter to the 32-bit internal state array */
	uintptr_t idx;
} sfmt_t;

#if defined(_M_IX86) || defined(_M_X64)
#if defined(_M_X64)
__declspec(align(8))
#endif
static sfmt_t    sfmt;                      /* sfmt internal state vector */
#endif
#if !defined(_M_X64)
#if !defined(_M_IX86)
static uint32_t  mt[MT_N];                  /* the array for the state vector */
#else
#define          mt (&sfmt.state[0].u[0])   /* the array for the state vector */
#endif
static uintptr_t mti = -2;                  /* mti == -2 means mt[MT_N] is not initialized */
#endif

#if defined(_M_IX86)
void __cdecl srand(unsigned int seed);
static void __cdecl srandSSE2(unsigned int seed);
static void __cdecl srandGeneric(unsigned int seed);
static void __cdecl srandCPUDispatch(unsigned int seed);
uint32_t __cdecl rand32();
static uint32_t __cdecl rand32SSE2();
static uint32_t __cdecl rand32Generic();
static uint32_t __cdecl rand32CPUDispatch();
uint64_t __cdecl rand64();
static uint64_t __cdecl rand64SSE2();
static uint64_t __cdecl rand64Generic();
static uint64_t __cdecl rand64CPUDispatch();

static void (__cdecl *srandDispatch)(unsigned int seed) = srandCPUDispatch;
static uint32_t (__cdecl *rand32Dispatch)() = rand32CPUDispatch;
static uint64_t (__cdecl *rand64Dispatch)() = rand64CPUDispatch;
#endif

#if defined(_M_IX86) || defined(_M_X64)
/* This function certificate the period of 2^{MEXP} */
static void period_certification()
{
	uint32_t inner = 0;
	uintptr_t i, j;
	uint32_t work;
	uint32_t *psfmt32 = &sfmt.state[0].u[0];
	const uint32_t parity[4] = { SFMT_PARITY1, SFMT_PARITY2,
	                             SFMT_PARITY3, SFMT_PARITY4 };

	for (i = 0; i < 4; i++)
		inner ^= psfmt32[i] & parity[i];
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
				psfmt32[i] ^= work;
				return;
			}
			work = work << 1;
		}
	}
}
#endif

#if defined(_M_IX86)
__declspec(naked) void __cdecl srand(unsigned int seed)
{
	__asm
	{
		jmp     dword ptr [srandDispatch]
	}
}
#endif

#if defined(_M_IX86) || defined(_M_X64)
#if defined(_M_IX86)
static void __cdecl srandSSE2(unsigned int seed)
#else
void __cdecl srand(unsigned int seed)
#endif
{
	static void period_certification();

	uintptr_t i;
	uint32_t *psfmt32 = &sfmt.state[0].u[0];

	psfmt32[0] = seed;
	for (i = 1; i < SFMT_N32; i++)
	    psfmt32[i] = 1812433253UL * (psfmt32[i - 1] ^ (psfmt32[i - 1] >> 30)) + i;
	sfmt.idx = SFMT_N32;
	period_certification(&sfmt);
}
#endif

#if !defined(_M_X64)
/* initializes mt[MT_N] with a seed */
#if defined(_M_IX86)
static void __cdecl srandGeneric(unsigned int seed)
#else
void __cdecl srand(unsigned int seed)
#endif
{
	mt[0] = seed;
	for (mti = 1; mti < MT_N; mti++)
		mt[mti] = seed = (UINT32_C(1812433253) * (seed ^ (seed >> 30)) + mti);
}
#endif

#if defined(_M_IX86)
__declspec(naked) static void __cdecl srandCPUDispatch(unsigned int seed)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [srandDispatch], offset srandGeneric
		mov     dword ptr [rand32Dispatch], offset rand32Generic
		mov     dword ptr [rand64Dispatch], offset rand64Generic
		jmp     srandGeneric
	L1:
		mov     dword ptr [srandDispatch], offset srandSSE2
		mov     dword ptr [rand32Dispatch], offset rand32SSE2
		mov     dword ptr [rand64Dispatch], offset rand64SSE2
		jmp     srandSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

int __cdecl rand()
{
#if !defined(_M_IX86)
	uint32_t __cdecl rand32();
#else
	#define rand32 rand32Dispatch
#endif

	return rand32() & 0x00007FFF;

	#undef rand32
}

uint8_t __cdecl rand8()
{
#if !defined(_M_IX86)
	uint32_t __cdecl rand32();
#else
	#define rand32 rand32Dispatch
#endif

	return (uint8_t)rand32();

	#undef rand32
}

uint16_t __cdecl rand16()
{
#if !defined(_M_IX86)
	uint32_t __cdecl rand32();
#else
	#define rand32 rand32Dispatch
#endif

	return (uint16_t)rand32();

	#undef rand32
}

#if defined(_M_IX86) || defined(_M_X64)
/*
 * This function simulates SIMD 128-bit left shift by the standard C.
 * The 128-bit integer given in in is shifted by (shift * 8) bits.
 * This function simulates the LITTLE ENDIAN SIMD.
 */
inline static void lshift128(w128_t *out, w128_t const *in, int shift)
{
	uint64_t th, tl, oh, ol;

	th = ((uint64_t)in->u[3] << 32) | ((uint64_t)in->u[2]);
	tl = ((uint64_t)in->u[1] << 32) | ((uint64_t)in->u[0]);

	oh = th << (shift * 8);
	ol = tl << (shift * 8);
	oh |= tl >> (64 - shift * 8);
	out->u[1] = (uint32_t)(ol >> 32);
	out->u[0] = (uint32_t)ol;
	out->u[3] = (uint32_t)(oh >> 32);
	out->u[2] = (uint32_t)oh;
}
/*
 * This function simulates SIMD 128-bit right shift by the standard C.
 * The 128-bit integer given in in is shifted by (shift * 8) bits.
 * This function simulates the LITTLE ENDIAN SIMD.
 */
inline static void rshift128(w128_t *out, w128_t const *in, int shift)
{
	uint64_t th, tl, oh, ol;

	th = ((uint64_t)in->u[3] << 32) | ((uint64_t)in->u[2]);
	tl = ((uint64_t)in->u[1] << 32) | ((uint64_t)in->u[0]);

	oh = th >> (shift * 8);
	ol = tl >> (shift * 8);
	ol |= th << (64 - shift * 8);
	out->u[1] = (uint32_t)(ol >> 32);
	out->u[0] = (uint32_t)ol;
	out->u[3] = (uint32_t)(oh >> 32);
	out->u[2] = (uint32_t)oh;
}

inline static void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *c, w128_t *d)
{
	inline static void lshift128(w128_t *out, w128_t const *in, int shift);
	inline static void rshift128(w128_t *out, w128_t const *in, int shift);

	w128_t x;
	w128_t y;

	lshift128(&x, a, SFMT_SL2);
	rshift128(&y, c, SFMT_SR2);
	r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SFMT_SR1) & SFMT_MSK1)
	    ^ y.u[0] ^ (d->u[0] << SFMT_SL1);
	r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SFMT_SR1) & SFMT_MSK2)
	    ^ y.u[1] ^ (d->u[1] << SFMT_SL1);
	r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> SFMT_SR1) & SFMT_MSK3)
	    ^ y.u[2] ^ (d->u[2] << SFMT_SL1);
	r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> SFMT_SR1) & SFMT_MSK4)
	    ^ y.u[3] ^ (d->u[3] << SFMT_SL1);
}

/*
 * This function fills the internal state array with pseudorandom
 * integers.
 */
static void sfmt_gen_rand_all()
{
	inline static void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *c, w128_t *d);

	uintptr_t i;
	w128_t *r1, *r2;

	r1 = &sfmt.state[SFMT_N - 2];
	r2 = &sfmt.state[SFMT_N - 1];
	for (i = 0; i < SFMT_N - SFMT_POS1; i++) {
		do_recursion(&sfmt.state[i], &sfmt.state[i],
		             &sfmt.state[i + SFMT_POS1], r1, r2);
		r1 = r2;
		r2 = &sfmt.state[i];
	}
	for (; i < SFMT_N; i++) {
		do_recursion(&sfmt.state[i], &sfmt.state[i],
		             &sfmt.state[i + SFMT_POS1 - SFMT_N], r1, r2);
		r1 = r2;
		r2 = &sfmt.state[i];
	}
}
#endif

#if !defined(_M_X64)
/* generate MT_N words at one time */
static void generate_matrix()
{
#if !defined(_M_IX86)
	void __cdecl srand(unsigned int seed);
#else
	#define srand srandGeneric
#endif

	static const uint32_t mag01[2] = { 0, MT_MATRIX_A };	/* mag01[x] = x * MT_MATRIX_A for x = 0, 1 */
	uint32_t x;
	uintptr_t kk;

	if (mti == -2)              /* if srand() has not been called, */
		srand(UINT32_C(5489));  /* a default initial seed is used */

	for (kk = 0; kk < MT_N - MT_M; kk++) {
		x = (mt[kk] & MT_UPPER_MASK) | (mt[kk + 1] & MT_LOWER_MASK);
		mt[kk] = mt[kk + MT_M] ^ (x >> 1) ^ mag01[x & 1];
	}
	for (; kk < MT_N - 1; kk++) {
		x = (mt[kk] & MT_UPPER_MASK) | (mt[kk + 1] & MT_LOWER_MASK);
		mt[kk] = mt[kk + (MT_M - MT_N)] ^ (x >> 1) ^ mag01[x & 1];
	}
	x = (mt[MT_N - 1] & MT_UPPER_MASK) | (mt[0] & MT_LOWER_MASK);
	mt[MT_N - 1] = mt[MT_M - 1] ^ (x >> 1) ^ mag01[x & 1];

	#undef srand
}
#endif

#if defined(_M_IX86)
__declspec(naked) uint32_t __cdecl rand32()
{
	__asm
	{
		jmp     dword ptr [rand32Dispatch]
	}
}
#endif

#if defined(_M_IX86) || defined(_M_X64)
/*
 * This function generates and returns 32-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 */
#if defined(_M_IX86)
static uint32_t __cdecl rand32SSE2()
#else
uint32_t __cdecl rand32()
#endif
{
	static void sfmt_gen_rand_all();

	uint32_t r;
	uint32_t *psfmt32 = &sfmt.state[0].u[0];

	if (sfmt.idx >= SFMT_N32) {
		sfmt_gen_rand_all(&sfmt);
		sfmt.idx = 0;
	}
	r = psfmt32[sfmt.idx++];
	return r;
}
#endif

#if !defined(_M_X64)
/* generates a random number on [0, 0xFFFFFFFF] - interval */
#if defined(_M_IX86)
static uint32_t __cdecl rand32Generic()
#else
uint32_t __cdecl rand32()
#endif
{
	static void generate_matrix();

	uint32_t x;

	if (mti >= MT_N) {
		/* generate MT_N words at one time */
		generate_matrix();
		mti = 0;
	}

	x = mt[mti++];

	/* Tempering */
	x ^= (x >> 11);
	x ^= (x <<  7) & 0x9D2C5680;
	x ^= (x << 15) & 0xEFC00000;
	x ^= (x >> 18);

	return x;
}
#endif

#if defined(_M_IX86)
__declspec(naked) static uint32_t __cdecl rand32CPUDispatch()
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [srandDispatch], offset srandGeneric
		mov     dword ptr [rand32Dispatch], offset rand32Generic
		mov     dword ptr [rand64Dispatch], offset rand64Generic
		jmp     rand32Generic
	L1:
		mov     dword ptr [srandDispatch], offset srandSSE2
		mov     dword ptr [rand32Dispatch], offset rand32SSE2
		mov     dword ptr [rand64Dispatch], offset rand64SSE2
		jmp     rand32SSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

#if defined(_M_IX86)
__declspec(naked) uint64_t __cdecl rand64()
{
	__asm
	{
		jmp     dword ptr [rand64Dispatch]
	}
}
#endif

#if defined(_M_IX86) || defined(_M_X64)
/*
 * This function generates and returns 64-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 * The function gen_rand64 should not be called after gen_rand32,
 * unless an initialization is again executed.
 */
#if defined(_M_IX86)
static uint64_t __cdecl rand64SSE2()
#else
uint64_t __cdecl rand64()
#endif
{
	static void sfmt_gen_rand_all();

	uint64_t r;
	uint64_t *psfmt64 = &sfmt.state[0].u64[0];

	sfmt.idx = (sfmt.idx + 1) & -2;
	if (sfmt.idx >= SFMT_N32) {
		sfmt_gen_rand_all(&sfmt);
		sfmt.idx = 0;
	}
	r = psfmt64[sfmt.idx / 2];
	sfmt.idx += 2;
	return r;
}
#endif

#if !defined(_M_X64)
/* generates a random number on [0, 0xFFFFFFFFFFFFFFFF] - interval */
#if defined(_M_IX86)
static uint64_t __cdecl rand64Generic()
#else
uint64_t __cdecl rand64()
#endif
{
	static void generate_matrix();

	uint64_t x;

	mti = (mti + 1) & -2;
	if (mti >= MT_N) {
		/* generate MT_N words at one time */
		generate_matrix();
		mti = 0;
	}

	x = *(uint64_t *)(mt + mti);
	mti += 2;

	/* Tempering */
	x ^= (x >> 29) & 0x0000000555555555;
	x ^= (x << 17) & 0x71D67FFFEDA60000;
	x ^= (x << 37) & 0xFFF7EEE000000000;
	x ^= (x >> 43);

	return x;
}
#endif

#if defined(_M_IX86)
__declspec(naked) static uint64_t __cdecl rand64CPUDispatch()
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [srandDispatch], offset srandGeneric
		mov     dword ptr [rand32Dispatch], offset rand32Generic
		mov     dword ptr [rand64Dispatch], offset rand64Generic
		jmp     rand64Generic
	L1:
		mov     dword ptr [srandDispatch], offset srandSSE2
		mov     dword ptr [rand32Dispatch], offset rand32SSE2
		mov     dword ptr [rand64Dispatch], offset rand64SSE2
		jmp     rand64SSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

float __cdecl randf32()
{
#if !defined(_M_IX86)
	uint32_t __cdecl rand32();
#else
	#define rand32 rand32Dispatch
#endif

	uint32_t value;

	value = ((rand32() & 1) << 31) | ((rand32() & 0xFF) << 23) | (rand32() & 0x007FFFFF);
	return *(float *)&value;

	#undef rand32
}

double __cdecl randf64()
{
#if !defined(_M_IX86)
	uint32_t __cdecl rand32();
	uint64_t __cdecl rand64();
#else
	#define rand32 rand32Dispatch
	#define rand64 rand64Dispatch
#endif

	uint64_t value;

	value = ((uint64_t)(rand32() & 1) << 63) | ((uint64_t)(rand32() & 0x07FF) << 52) | (rand64() & 0x000FFFFFFFFFFFFF);
	return *(double *)&value;

	#undef rand32
	#undef rand64
}
