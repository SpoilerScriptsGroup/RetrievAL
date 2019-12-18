/*
FUNCTION
<<qsort>>---sort an array
INDEX
	qsort
ANSI_SYNOPSIS
	#include <stdlib.h>
	void qsort(void *<[base]>, size_t <[num]>, size_t <[width]>,
	           int (*<[compare]>)(const void *, const void *));
TRAD_SYNOPSIS
	#include <stdlib.h>
	qsort(<[base]>, <[num]>, <[width]>, <[compare]>)
	char *<[base]>;
	size_t <[num]>;
	size_t <[width]>;
	int (*<[compare]>)();
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

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define swapcode(TYPE, a, b, size)      \
do {                                    \
    size_t __n = (size) / sizeof(TYPE); \
    register TYPE *__pa = (TYPE *)(a);  \
    register TYPE *__pb = (TYPE *)(b);  \
    do {                                \
        register TYPE __t = *__pa;      \
        *__pa++ = *__pb;                \
        *__pb++ = __t;                  \
    } while (--__n);                    \
} while (0)

#define swapinit(base, width)                                   \
    ((size_t)(base) % sizeof(long) || (width) % sizeof(long) ?  \
        2 :                                                     \
        (width) == sizeof(long) ? 0 : 1)

#define swapfunc(a, b, size, swaptype)  \
    if ((swaptype) <= 1)                \
        swapcode(long, a, b, size);     \
    else                                \
        swapcode(char, a, b, size)

#define swap(a, b, size, swaptype)      \
    if ((swaptype) == 0) {              \
        long t = *(long *)(a);          \
        *(long *)(a) = *(long *)(b);    \
        *(long *)(b) = t;               \
    } else                              \
        swapfunc(a, b, size, swaptype)

#define vecswap(a, b, size, swaptype)   \
    if (size)                           \
        swapfunc(a, b, size, swaptype); \
    else                                \
        while (0)

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
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
	size_t d;
	int r;
	unsigned int swaptype, swap_cnt;

	swaptype = swapinit(base, width);
	for (; ; ) {
		if (num >= 7) {
			swap_cnt = 0;
			pm = (char *)base + (num / 2) * width;
			if (num > 7) {
				pl = base;
				pn = (char *)base + (num - 1) * width;
				if (num > 40) {
					d = (num / 8) * width;
					pl = med3(pl, pl + d, pl + d * 2, compare);
					pm = med3(pm - d, pm, pm + d, compare);
					pn = med3(pn - d * 2, pn - d, pn, compare);
				}
				pm = med3(pl, pm, pn, compare);
			}
			swap(base, pm, width, swaptype);
			pa = pb = (char *)base + width;
			pc = pd = (char *)base + (num - 1) * width;
			for (; ; ) {
				while (pb <= pc && (r = compare(pb, base)) <= 0) {
					if (r == 0) {
						swap_cnt = 1;
						swap(pa, pb, width, swaptype);
						pa += width;
					}
					pb += width;
				}
				while (pb <= pc && (r = compare(pc, base)) >= 0) {
					if (r == 0) {
						swap_cnt = 1;
						swap(pc, pd, width, swaptype);
						pd -= width;
					}
					pc -= width;
				}
				if (pb > pc)
					break;
				swap(pb, pc, width, swaptype);
				swap_cnt = 1;
				pb += width;
				pc -= width;
			}
			if (swap_cnt != 0) {
				pn = (char *)base + num * width;
				d = min((size_t)(pa - (char *)base), (size_t)(pb - pa));
				vecswap(base, pb - d, d, swaptype);
				d = min((size_t)(pd - pc), pn - pd - width);
				vecswap(pb, pn - d, d, swaptype);
				if ((d = pb - pa) > width)
					qsort(base, d / width, width, compare);
				if ((d = pd - pc) <= width)
					break;
				/* Iterate rather than recurse to save stack space */
				base = pn - d;
				num = d / width;
				/* qsort(pn - d, d / width, width, compare); */
				continue;
			}
		}
		/* Switch to insertion sort */
		for (pm = (char *)base + width; pm < (char *)base + num * width; pm += width)
			for (pl = pm; pl > (char *)base && compare(pl - width, pl) > 0; pl -= width)
				swap(pl, pl - width, width, swaptype);
		break;
	}
}
