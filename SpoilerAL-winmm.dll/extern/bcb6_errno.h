#pragma once

#include <windows.h>

EXTERN_C int *(__cdecl *bcb6___errno)();
#define bcb6_errno *bcb6___errno()

#define BCB6_EINVAL 19
#define BCB6_ERANGE 34

