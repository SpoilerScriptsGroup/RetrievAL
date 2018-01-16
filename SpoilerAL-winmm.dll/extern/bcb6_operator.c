#include "bcb6_operator.h"

#if !OPTIMIZE_ALLOCATOR
void(__cdecl * const bcb6_operator_delete)(void *p) = (LPVOID)0x005D4484;
void *(__cdecl * const bcb6_operator_new)(size_t n) = (LPVOID)0x005D44B8;
#endif
