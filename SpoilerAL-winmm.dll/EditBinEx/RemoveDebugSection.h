#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD RemoveDebugSection(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum);

#ifdef __cplusplus
}
#endif
