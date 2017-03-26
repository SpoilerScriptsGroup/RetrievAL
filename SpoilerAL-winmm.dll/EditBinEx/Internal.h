#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void UpdateCheckSum(LPVOID BaseAddress, DWORD SizeOfImage, LPDWORD Checksum);
BOOL GetFileOffsetFromRVA(PIMAGE_NT_HEADERS NtHeaders, DWORD RVA, PDWORD FileOffset);
BOOL GetDecimalNumber(LPCWSTR lpString, LPDWORD lpdwNumber);
BOOL GetDwordNumber(LPCWSTR lpString, LPDWORD lpdwNumber);

#ifdef __cplusplus
}
#endif
