/*
 * bsearch.c
 * Original Author: G. Haley
 * Rewritten by:    G. Noer
 *
 * Searches an array of num members, the initial member of which is pointed
 * to by base, for a member that matches the object pointed to by key. The
 * contents of the array shall be in ascending order according to a comparison
 * function pointed to by compare. The function shall return an integer less
 * than, equal to or greater than zero if the first argument is considered to be
 * respectively less than, equal to or greater than the second. Returns a
 * pointer to the matching member of the array, or a null pointer if no match
 * is found.
 */

/*
FUNCTION
<<bsearch>>---binary search
INDEX
	bsearch
ANSI_SYNOPSIS
	#include <stdlib.h>
	void *bsearch(const void *<[key]>, const void *<[base]>,
		size_t <[num]>, size_t <[width]>,
		int(__cdecl *<[compare]>)(const void *, const void *));
TRAD_SYNOPSIS
	#include <stdlib.h>
	char *bsearch(<[key]>, <[base]>, <[num]>, <[width]>, <[compare]>)
	char *<[key]>;
	char *<[base]>;
	size_t <[num]>, <[width]>;
	int(__cdecl *<[compare]>)(const void *, const void *);
DESCRIPTION
<<bsearch>> searches an array beginning at <[base]> for any element
that matches <[key]>, using binary search.  <[num]> is the element
count of the array; <[width]> is the size of each element.
The array must be sorted in ascending order with respect to the
comparison function <[compare]> (which you supply as the last argument of
<<bsearch>>).
You must define the comparison function <<(*<[compare]>)>> to have two
arguments; its result must be negative if the first argument is
less than the second, zero if the two arguments match, and
positive if the first argument is greater than the second (where
``less than'' and ``greater than'' refer to whatever arbitrary
ordering is appropriate).
RETURNS
Returns a pointer to an element of <[array]> that matches <[key]>.  If
more than one matching element is available, the result may point to
any of them.
PORTABILITY
<<bsearch>> is ANSI.
No supporting OS subroutines are required.
*/

#include <stdlib.h>

void * __cdecl bsearch(
	const void *key,
	const void *base,
	size_t num,
	size_t width,
	int(__cdecl *compare)(const void *, const void *))
{
	void *current;
	size_t lower, upper, index;
	int result;

	if (num == 0 || width == 0)
		return NULL;
	lower = 0;
	upper = num;
	while (lower < upper)
	{
		index = (lower + upper) / 2;
		current = (void *)(((char *)base) + (index * width));
		result = compare(key, current);
		if (result < 0)
			upper = index;
		else if (result > 0)
			lower = index + 1;
		else
			return current;
	}
	return NULL;
}
