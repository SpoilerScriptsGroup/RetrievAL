#pragma once

#include <windows.h>

EXTERN_C int *(__cdecl *_bcb6_errno)();
#define bcb6_errno *_bcb6_errno()

#define BCB6_EINVAL 19
#define BCB6_ERANGE 34

