#pragma once

#ifdef __cplusplus
extern "C" {
#endif

UINT SetOperatingSystemVersion(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL HasCheckSum, LPWSTR lpParameter);

#ifdef __cplusplus
}
#endif
