#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD SetDelayImport(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum, LPWSTR lpParameter);

#ifdef __cplusplus
}
#endif
