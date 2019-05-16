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
#include <time.h>

/* Period parameters */
#define N          624
#define M          397
#define MATRIX_A   0x9908B0DF   /* constant vector a */
#define UPPER_MASK INT32_MIN    /* most significant w - r bits */
#define LOWER_MASK INT32_MAX    /* least significant r bits */

static uint32_t mt[N];          /* the array for the state vector  */
static size_t   mti = N + 1;    /* mti == N + 1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void __cdecl internal_srand(unsigned int seed)
{
	mt[0] = seed;
	for (mti = 1; mti < N; mti++)
		mt[mti] = seed = (UINT32_C(1812433253) * (seed ^ (seed >> 30)) + mti);
}

int __cdecl internal_rand()
{
	uint32_t __cdecl rand32();

	return rand32() & 0x00007FFF;
}

uint16_t __cdecl rand16()
{
	uint32_t __cdecl rand32();

	return (uint16_t)rand32();
}

/* generates a random number on [0, 0xFFFFFFFF] - interval */
uint32_t __cdecl rand32()
{
	static void generate_matrix();

	uint32_t x;

	if (mti >= N) {
		/* generate N words at one time */
		generate_matrix();
		mti = 0;
	}

	x = mt[mti++];

	/* Tempering */
	x ^= (x >> 11);
	x ^= (x <<  7) & 0x9D2C5680;
	x ^= (x << 15) & 0xEFC60000;
	x ^= (x >> 18);

	return x;
}

/* generates a random number on [0, 0xFFFFFFFFFFFFFFFF] - interval */
uint64_t __cdecl rand64()
{
	static void generate_matrix();

	uint64_t x;

	mti = (mti + 1) & -2;
	if (mti >= N) {
		/* generate N words at one time */
		generate_matrix();
		mti = 0;
	}

	x = *(uint64_t *)(mt + mti);
	mti += 2;

	x ^= (x >> 29) & 0x5555555555555555;
	x ^= (x << 17) & 0x71D67FFFEDA60000;
	x ^= (x << 37) & 0xFFF7EEE000000000;
	x ^= (x >> 43);

	return x;
}

static void generate_matrix()
{
	static uint32_t mag01[2] = { 0, MATRIX_A }; /* mag01[x] = x * MATRIX_A  for x = 0,1 */
	uint32_t x;
	size_t kk;

	if (mti == N + 1)                               /* if init_genrand() has not been called, */
		internal_srand((unsigned int)time(NULL));   /* a default initial seed is used */

	for (kk = 0; kk < N - M; kk++) {
		x = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
		mt[kk] = mt[kk + M] ^ (x >> 1) ^ mag01[x & 1];
	}
	for (; kk < N - 1; kk++) {
		x = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
		mt[kk] = mt[kk + (M - N)] ^ (x >> 1) ^ mag01[x & 1];
	}
	x = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
	mt[N - 1] = mt[M - 1] ^ (x >> 1) ^ mag01[x & 1];
}

double __cdecl randf64()
{
	uint64_t value;

	value = ((uint64_t)(rand32() & 1) << 63) | ((uint64_t)(rand32() & 0x07FF) << 52) | (rand64() & 0x000FFFFFFFFFFFFF);
	return *(double *)&value;
}

float __cdecl randf32()
{
	uint32_t value;

	value = ((rand32() & 1) << 31) | ((rand32() & 0xFF) << 23) | (rand32() & 0x007FFFFF);
	return *(float *)&value;
}
