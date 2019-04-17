#pragma once

#ifdef __cplusplus
extern "C" {
#endif

BOOL ValidateCheckSum(LPVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders);
void UpdateCheckSum(LPVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders);
BOOL GetFileOffsetFromRVA(PIMAGE_NT_HEADERS NtHeaders, DWORD RVA, PDWORD FileOffset);
BOOL GetDecimalNumber(LPCWSTR lpString, LPDWORD lpdwNumber);
BOOL GetDwordNumber(LPCWSTR lpString, LPDWORD lpdwNumber);

#ifdef __cplusplus
}
#endif
