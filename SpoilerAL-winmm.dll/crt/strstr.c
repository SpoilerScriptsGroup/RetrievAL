/*
FUNCTION
	<<strstr>>---find string segment

INDEX
	strstr

ANSI_SYNOPSIS
	#include <string.h>
	char *strstr(const char *<[s1]>, const char *<[s2]>);

TRAD_SYNOPSIS
	#include <string.h>
	char *strstr(<[s1]>, <[s2]>)
	char *<[s1]>;
	char *<[s2]>;

DESCRIPTION
	Locates the first occurrence in the string pointed to by <[s1]> of
	the sequence of characters in the string pointed to by <[s2]>
	(excluding the terminating null character).

RETURNS
	Returns a pointer to the located string segment, or a null
	pointer if the string <[s2]> is not found. If <[s2]> points to
	a string with zero length, <[s1]> is returned.

PORTABILITY
<<strstr>> is ANSI C.

<<strstr>> requires no supporting OS subroutines.

QUICKREF
	strstr ansi pure
*/

#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define AVAILABLE(h, h_l, j, n_l)                       \
    (!memchr((h) + (h_l), '\0', (j) + (n_l) - (h_l))    \
     && ((h_l) = (j) + (n_l)))

/* Point at which computing a bad-byte shift table is likely to be
   worthwhile.  Small needles should not compute a table, since it
   adds (1 << CHAR_BIT) + NEEDLE_LEN computations of preparation for a
   speedup no greater than a factor of NEEDLE_LEN.  The larger the
   needle, the better the potential performance gain.  On the other
   hand, on non-POSIX systems with CHAR_BIT larger than eight, the
   memory required for the table is prohibitive. */
#if CHAR_BIT < 10
# define LONG_NEEDLE_THRESHOLD 32U
#else
# define LONG_NEEDLE_THRESHOLD SIZE_MAX
#endif

/* Perform a critical factorization of NEEDLE, of length NEEDLE_LEN.
   Return the index of the first byte in the right half, and set
   *PERIOD to the global period of the right half.

   The global period of a string is the smallest index (possibly its
   length) at which all remaining bytes in the string are repetitions
   of the prefix (the last repetition may be a subset of the prefix).

   When NEEDLE is factored into two halves, a local period is the
   length of the smallest word that shares a suffix with the left half
   and shares a prefix with the right half.  All factorizations of a
   non-empty NEEDLE have a local period of at least 1 and no greater
   than NEEDLE_LEN.

   A critical factorization has the property that the local period
   equals the global period.  All strings have at least one critical
   factorization with the left half smaller than the global period.

   Given an ordered alphabet, a critical factorization can be computed
   in linear time, with 2 * NEEDLE_LEN comparisons, by computing the
   larger of two ordered maximal suffixes.  The ordered maximal
   suffixes are determined by lexicographic comparison of
   periodicity. */
static size_t critical_factorization(
	const unsigned char *needle,
	size_t needle_len,
	size_t* period)
{
	/* Index of last byte of left half, or SIZE_MAX. */
	size_t        max_suffix, max_suffix_rev;
	size_t        j;    /* Index into NEEDLE for current candidate suffix. */
	size_t        k;    /* Offset into current period. */
	size_t        p;    /* Intermediate period. */
	unsigned char a, b; /* Current comparison bytes. */

	/* Invariants:
	   0 <= j < NEEDLE_LEN - 1
	   -1 <= max_suffix{,_rev} < j (treating SIZE_MAX as if it were signed)
	   min(max_suffix, max_suffix_rev) < global period of NEEDLE
	   1 <= p <= global period of NEEDLE
	   p == global period of the substring NEEDLE[max_suffix{,_rev}+1...j]
	   1 <= k <= p
	*/

	/* Perform lexicographic search. */
	max_suffix = SIZE_MAX;
	j = 0;
	k = p = 1;
	while (j + k < needle_len)
	{
		a = needle[j + k];
		b = needle[(size_t)(max_suffix + k)];
		if (a < b)
		{
			/* Suffix is smaller, period is entire prefix so far. */
			j += k;
			k = 1;
			p = j - max_suffix;
		}
		else if (a == b)
		{
			/* Advance through repetition of the current period. */
			if (k != p)
				++k;
			else
			{
				j += p;
				k = 1;
			}
		}
		else /* b < a */
		{
			/* Suffix is larger, start over from current location. */
			max_suffix = j++;
			k = p = 1;
		}
	}
	*period = p;

	/* Perform reverse lexicographic search. */
	max_suffix_rev = SIZE_MAX;
	j = 0;
	k = p = 1;
	while (j + k < needle_len)
	{
		a = needle[j + k];
		b = needle[max_suffix_rev + k];
		if (b < a)
		{
			/* Suffix is smaller, period is entire prefix so far. */
			j += k;
			k = 1;
			p = j - max_suffix_rev;
		}
		else if (a == b)
		{
			/* Advance through repetition of the current period. */
			if (k != p)
				++k;
			else
			{
				j += p;
				k = 1;
			}
		}
		else /* a < b */
		{
			/* Suffix is larger, start over from current location. */
			max_suffix_rev = j++;
			k = p = 1;
		}
	}

	/* Choose the longer suffix.  Return the first byte of the right
	   half, rather than the last byte of the left half. */
	if (max_suffix_rev + 1 < max_suffix + 1)
		return max_suffix + 1;
	*period = p;
	return max_suffix_rev + 1;
}

/* Return the first location of non-empty NEEDLE within HAYSTACK, or
   NULL.  HAYSTACK_LEN is the minimum known length of HAYSTACK.  This
   method is optimized for NEEDLE_LEN < LONG_NEEDLE_THRESHOLD.
   Performance is guaranteed to be linear, with an initialization cost
   of 2 * NEEDLE_LEN comparisons.

   If AVAILABLE does not modify HAYSTACK_LEN (as in memmem), then at
   most 2 * HAYSTACK_LEN - NEEDLE_LEN comparisons occur in searching.
   If AVAILABLE modifies HAYSTACK_LEN (as in strstr), then at most 3 *
   HAYSTACK_LEN - NEEDLE_LEN comparisons occur in searching. */
static char *two_way_short_needle(
	const unsigned char *haystack,
	size_t              haystack_len,
	const unsigned char *needle,
	size_t              needle_len)
{
	size_t i;       /* Index into current byte of NEEDLE. */
	size_t j;       /* Index into current window of HAYSTACK. */
	size_t period;  /* The period of the right half of needle. */
	size_t suffix;  /* The index of the right half of needle. */

	/* Factor the needle into two halves, such that the left half is
	   smaller than the global period, and the right half is
	   periodic (with a period as large as NEEDLE_LEN - suffix). */
	suffix = critical_factorization(needle, needle_len, &period);

	/* Perform the search.  Each iteration compares the right half
	   first. */
	if (memcmp(needle, needle + period, suffix) == 0)
	{
		/* Entire needle is periodic; a mismatch can only advance by the
		   period, so use memory to avoid rescanning known occurrences
		   of the period. */
		size_t memory = 0;
		j = 0;
		while (AVAILABLE(haystack, haystack_len, j, needle_len))
		{
			/* Scan for matches in right half. */
			i = max(suffix, memory);
			while (i < needle_len && needle[i] == haystack[i + j])
				++i;
			if (needle_len <= i)
			{
				/* Scan for matches in left half. */
				i = suffix - 1;
				while (memory < i + 1 && needle[i] == haystack[i + j])
					--i;
				if (i + 1 < memory + 1)
					return (char *)(haystack + j);
				/* No match, so remember how many repetitions of period
				   on the right half were scanned. */
				j += period;
				memory = needle_len - period;
			}
			else
			{
				j += i - suffix + 1;
				memory = 0;
			}
		}
	}
	else
	{
		/* The two halves of needle are distinct; no extra memory is
		   required, and any mismatch results in a maximal shift. */
		period = max(suffix, needle_len - suffix) + 1;
		j = 0;
		while (AVAILABLE(haystack, haystack_len, j, needle_len))
		{
			/* Scan for matches in right half. */
			i = suffix;
			while (i < needle_len && needle[i] == haystack[i + j])
				++i;
			if (needle_len <= i)
			{
				/* Scan for matches in left half. */
				i = suffix - 1;
				while (i != SIZE_MAX && needle[i] == haystack[i + j])
					--i;
				if (i == SIZE_MAX)
					return (char *)(haystack + j);
				j += period;
			}
			else
				j += i - suffix + 1;
		}
	}
	return NULL;
}

/* Return the first location of non-empty NEEDLE within HAYSTACK, or
   NULL.  HAYSTACK_LEN is the minimum known length of HAYSTACK.  This
   method is optimized for LONG_NEEDLE_THRESHOLD <= NEEDLE_LEN.
   Performance is guaranteed to be linear, with an initialization cost
   of 3 * NEEDLE_LEN + (1 << CHAR_BIT) operations.

   If AVAILABLE does not modify HAYSTACK_LEN (as in memmem), then at
   most 2 * HAYSTACK_LEN - NEEDLE_LEN comparisons occur in searching,
   and sublinear performance O(HAYSTACK_LEN / NEEDLE_LEN) is possible.
   If AVAILABLE modifies HAYSTACK_LEN (as in strstr), then at most 3 *
   HAYSTACK_LEN - NEEDLE_LEN comparisons occur in searching, and
   sublinear performance is not possible. */
static char *two_way_long_needle(
	const unsigned char *haystack,
	size_t              haystack_len,
	const unsigned char *needle,
	size_t              needle_len)
{
	size_t i;                           /* Index into current byte of NEEDLE. */
	size_t j;                           /* Index into current window of HAYSTACK. */
	size_t period;                      /* The period of the right half of needle. */
	size_t suffix;                      /* The index of the right half of needle. */
	size_t shift_table[1U << CHAR_BIT]; /* See below. */

	/* Factor the needle into two halves, such that the left half is
	   smaller than the global period, and the right half is
	   periodic (with a period as large as NEEDLE_LEN - suffix). */
	suffix = critical_factorization(needle, needle_len, &period);

	/* Populate shift_table.  For each possible byte value c,
	   shift_table[c] is the distance from the last occurrence of c to
	   the end of NEEDLE, or NEEDLE_LEN if c is absent from the NEEDLE.
	   shift_table[NEEDLE[NEEDLE_LEN - 1]] contains the only 0. */
	for (i = 0; i < 1U << CHAR_BIT; i++)
		shift_table[i] = needle_len;
	for (i = 0; i < needle_len; i++)
		shift_table[needle[i]] = needle_len - i - 1;

	/* Perform the search.  Each iteration compares the right half
	   first. */
	if (memcmp(needle, needle + period, suffix) == 0)
	{
		/* Entire needle is periodic; a mismatch can only advance by the
		   period, so use memory to avoid rescanning known occurrences
		   of the period. */
		size_t memory = 0;
		size_t shift;
		j = 0;
		while (AVAILABLE(haystack, haystack_len, j, needle_len))
		{
			/* Check the last byte first; if it does not match, then
			   shift to the next possible match location. */
			shift = shift_table[haystack[j + needle_len - 1]];
			if (0 < shift)
			{
				if (memory && shift < period)
				{
					/* Since needle is periodic, but the last period has
					   a byte out of place, there can be no match until
					   after the mismatch. */
					shift = needle_len - period;
				}
				memory = 0;
				j += shift;
				continue;
			}
			/* Scan for matches in right half.  The last byte has
			   already been matched, by virtue of the shift table. */
			i = max(suffix, memory);
			while (i < needle_len - 1 && needle[i] == haystack[i + j])
				++i;
			if (needle_len - 1 <= i)
			{
				/* Scan for matches in left half. */
				i = suffix - 1;
				while (memory < i + 1 && needle[i] == haystack[i + j])
					--i;
				if (i + 1 < memory + 1)
					return (char *)(haystack + j);
				/* No match, so remember how many repetitions of period
				   on the right half were scanned. */
				j += period;
				memory = needle_len - period;
			}
			else
			{
				j += i - suffix + 1;
				memory = 0;
			}
		}
	}
	else
	{
		/* The two halves of needle are distinct; no extra memory is
		   required, and any mismatch results in a maximal shift. */
		size_t shift;
		period = max(suffix, needle_len - suffix) + 1;
		j = 0;
		while (AVAILABLE(haystack, haystack_len, j, needle_len))
		{
			/* Check the last byte first; if it does not match, then
			   shift to the next possible match location. */
			shift = shift_table[haystack[j + needle_len - 1]];
			if (0 < shift)
			{
				j += shift;
				continue;
			}
			/* Scan for matches in right half.  The last byte has
			   already been matched, by virtue of the shift table. */
			i = suffix;
			while (i < needle_len - 1 && needle[i] == haystack[i + j])
				++i;
			if (needle_len - 1 <= i)
			{
				/* Scan for matches in left half. */
				i = suffix - 1;
				while (i != SIZE_MAX && needle[i] == haystack[i + j])
					--i;
				if (i == SIZE_MAX)
					return (char *)(haystack + j);
				j += period;
			}
			else
				j += i - suffix + 1;
		}
	}
	return NULL;
}

#undef AVAILABLE

char * __cdecl strstr(
	const char *searchee,
	const char *lookfor)
{
	/* Larger code size, but guaranteed linear performance. */
	const char *haystack;
	const char *needle;
	size_t     needle_len;      /* Length of NEEDLE. */
	size_t     haystack_len;    /* Known minimum length of HAYSTACK. */
	int        ok;              /* True if NEEDLE is prefix of HAYSTACK. */

	/* Determine length of NEEDLE, and in the process, make sure
	   HAYSTACK is at least as long (no point processing all of a long
	   NEEDLE if HAYSTACK is too short). */
	haystack = searchee;
	needle = lookfor;
	ok = 1;
	while (*haystack && *needle)
		ok &= *haystack++ == *needle++;
	if (*needle)
		return NULL;
	if (ok)
		return (char *)searchee;

	/* Reduce the size of haystack using strchr, since it has a smaller
	   linear coefficient than the Two-Way algorithm. */
	needle_len = needle - lookfor;
	haystack = strchr(searchee + 1, *lookfor);
	if (!haystack || needle_len == 1)
		return (char *)haystack;
	haystack_len = searchee + needle_len <= haystack ? 1
	               : searchee + needle_len - haystack;

	/* Perform the search. */
	if (needle_len < LONG_NEEDLE_THRESHOLD)
		return two_way_short_needle((const unsigned char *)haystack,
		                            haystack_len,
		                            (const unsigned char *)lookfor, needle_len);
	return two_way_long_needle((const unsigned char *)haystack, haystack_len,
	                           (const unsigned char *)lookfor, needle_len);
}
