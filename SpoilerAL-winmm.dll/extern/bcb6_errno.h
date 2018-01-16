#pragma once

#include <windows.h>

EXTERN_C int *(__cdecl * const _bcb6_errno)();
#define bcb6_errno *_bcb6_errno()

#define BCB6_ENOMEM 8
#define BCB6_EINVAL 19
#define BCB6_ERANGE 34

