#include <time.h>

double __cdecl _difftime32(__time32_t end, __time32_t start)
{
	return (double)(end - start);
}

double __cdecl _difftime64(__time64_t end, __time64_t start)
{
	return (double)(end - start);
}
