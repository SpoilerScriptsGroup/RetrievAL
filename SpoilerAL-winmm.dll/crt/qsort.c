/*
FUNCTION
<<qsort>>---sort an array
INDEX
	qsort
ANSI_SYNOPSIS
	#include <stdlib.h>
	void qsort(void *<[base]>, size_t <[num]>, size_t <[width]>,
	           int(__cdecl *<[compare]>)(const void *, const void *));
TRAD_SYNOPSIS
	#include <stdlib.h>
	qsort(<[base]>, <[num]>, <[width]>, <[compare]>)
	void *<[base]>;
	size_t <[num]>;
	size_t <[width]>;
	int(__cdecl *<[compare]>)(const void *, const void *);
DESCRIPTION
<<qsort>> sorts an array (beginning at <[base]>) of <[num]> objects.
<[width]> describes the size of each element of the array.
You must supply a pointer to a comparison function, using the argument
shown as <[compare]>.  (This permits sorting objects of unknown
properties.)  Define the comparison function to accept two arguments,
each a pointer to an element of the array starting at <[base]>.  The
result of <<(*<[compare]>)>> must be negative if the first argument is
less than the second, zero if the two arguments match, and positive if
the first argument is greater than the second (where ``less than'' and
``greater than'' refer to whatever arbitrary ordering is appropriate).
The array is sorted in place; that is, when <<qsort>> returns, the
array elements beginning at <[base]> have been reordered.
RETURNS
<<qsort>> does not return a result.
PORTABILITY
<<qsort>> is required by ANSI (without specifying the sorting algorithm).
*/

/*-
 * Copyright (c) 1992, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the University of
 *    California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define swapinit(base, width)                                           \
    ((size_t)(base) % sizeof(uint32_t) || (width) % sizeof(uint32_t) ?  \
        2 :                                                             \
        (width) == sizeof(uint32_t) ? 0 : 1)

#define swapcode(type, a, b, size)      \
do {                                    \
    size_t __n = (size) / sizeof(type); \
    type *__restrict __a = (type *)(a); \
    type *__restrict __b = (type *)(b); \
    do {                                \
        type __t = *__a;                \
        *(__a++) = *__b;                \
        *(__b++) = __t;                 \
    } while (--__n);                    \
} while (0)

#define vecswap(a, b, size, swaptype)   \
    if ((swaptype) <= 1)                \
        swapcode(uint32_t, a, b, size); \
    else                                \
        swapcode(uint8_t, a, b, size)

#define swap(a, b, size, swaptype)           \
    if ((swaptype) == 0) {                   \
        uint32_t t = *(uint32_t *)(a);       \
        *(uint32_t *)(a) = *(uint32_t *)(b); \
        *(uint32_t *)(b) = t;                \
    } else                                   \
        vecswap(a, b, size, swaptype)

#define med3(a, b, c, compare)                                          \
    (compare(a, b) < 0 ?                                                \
        (compare(b, c) < 0 ? (b) : (compare(a, c) < 0 ? (c) : (a))) :   \
        (compare(b, c) > 0 ? (b) : (compare(a, c) < 0 ? (a) : (c))))

void __cdecl qsort(
	void *base,
	size_t num,
	size_t width,
	int(__cdecl *compare)(const void *, const void *))
{
	unsigned int swaptype;
	uint8_t      *first, *last, *a, *b, *c, *d;
	bool         swaped;
	size_t       n;
	int          i;

	swaptype = swapinit(base, width);
	last = (first = (uint8_t *)base) + num * width;
	for (; ; ) {
		if (num >= 7) {
			swaped = false;
			b = first + (num / 2) * width;
			if (num > 7) {
				a = first;
				c = first + (num - 1) * width;
				if (num > 40) {
					n = (num / 8) * width;
					a = med3(a, a + n, a + n * 2, compare);
					b = med3(b - n, b, b + n, compare);
					c = med3(c - n * 2, c - n, c, compare);
				}
				b = med3(a, b, c, compare);
			}
			swap(first, b, width, swaptype);
			a = b = first + width;
			c = d = first + (num - 1) * width;
			for (; ; ) {
				while (b <= c && (i = compare(b, first)) <= 0) {
					if (i == 0) {
						swap(a, b, width, swaptype);
						swaped = true;
						a += width;
					}
					b += width;
				}
				while (b <= c && (i = compare(c, first)) >= 0) {
					if (i == 0) {
						swap(c, d, width, swaptype);
						swaped = true;
						d -= width;
					}
					c -= width;
				}
				if (b > c)
					break;
				swap(b, c, width, swaptype);
				swaped = true;
				b += width;
				c -= width;
			}
			if (swaped) {
				if (n = min((size_t)(a - first), (size_t)(b - a)))
					vecswap(first, b - n, n, swaptype);
				if (n = min((size_t)(d - c), last - d - width))
					vecswap(b, last - n, n, swaptype);
				if ((n = b - a) > width)
					qsort(first, n / width, width, compare);
				if ((n = d - c) <= width)
					break;
				/* Iterate rather than recurse to save stack space */
				first = last - n;
				num = n / width;
				/* qsort(last - n, n / width, width, compare); */
				continue;
			}
		}
		/* Switch to insertion sort */
		for (b = first + width; b < last; b += width)
			for (a = b; a > first && compare(a - width, a) > 0; a -= width)
				swap(a, a - width, width, swaptype);
		break;
	}
}
