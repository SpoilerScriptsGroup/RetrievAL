#pragma once

#ifdef __cplusplus
extern "C" {
#endif

DWORD SetLastWriteTime(HANDLE hFile, LPFILETIME lpLastWriteTime, LPWSTR lpParameter);

#ifdef __cplusplus
}
#endif
