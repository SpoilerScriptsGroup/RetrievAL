#ifndef _INTPROPS_H
#define _INTPROPS_H

#include <stdbool.h>
#include <limits.h>

/* The type of indexes into strings.  This is signed, not size_t,
   since the API requires indexes to fit in regoff_t anyway, and using
   signed integers makes the code a bit smaller and presumably faster.
   The traditional GNU regex implementation uses int for indexes.
   The POSIX-compatible implementation uses a possibly-wider type.
   The name 'Idx' is three letters to minimize the hassle of
   reindenting a lot of regex code that formerly used 'int'.  */
#ifndef IDX_MAX
# if defined _REGEX_LARGE_OFFSETS && defined PTRDIFF_MAX && PTRDIFF_MAX >= INT_MAX
#  define IDX_MAX PTRDIFF_MAX
# else
#  define IDX_MAX INT_MAX
# endif
#endif

/* True if the real type T is signed.  */
#define TYPE_SIGNED(t) ((t)0 >= (t)-1)

/* True if adding the nonnegative Idx values A and B would overflow.
   If false, set *R to A + B.  A, B, and R may be evaluated more than
   once, or zero times.  Although this is not a full implementation of
   Gnulib INT_ADD_WRAPV, it is good enough for glibc regex code.
   FIXME: This implementation is a fragile stopgap, and this file would
   be simpler and more robust if intprops.h were migrated into glibc.  */
#define INT_ADD_WRAPV(a, b, r) \
   (IDX_MAX - (a) < (b) ? true : (*(r) = (a) + (b), false))

#endif
