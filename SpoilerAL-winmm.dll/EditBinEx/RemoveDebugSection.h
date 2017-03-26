#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD RemoveDebugSection(PVOID BaseAddress, DWORD SizeOfImage, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus);

#ifdef __cplusplus
}
#endif
