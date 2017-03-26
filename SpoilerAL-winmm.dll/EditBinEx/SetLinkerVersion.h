#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD SetLinkerVersion(PVOID BaseAddress, DWORD SizeOfImage, PIMAGE_NT_HEADERS NtHeaders, LPWSTR lpParameter);

#ifdef __cplusplus
}
#endif
