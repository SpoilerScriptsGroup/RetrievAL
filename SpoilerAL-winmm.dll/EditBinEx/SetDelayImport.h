#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD SetDelayImport(PVOID BaseAddress, DWORD SizeOfImage, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, LPWSTR lpParameter);

#ifdef __cplusplus
}
#endif
