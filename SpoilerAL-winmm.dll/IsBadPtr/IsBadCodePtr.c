#define IsBadCodePtrEx IsBadCodePtrCompatible
#define VirtualQueryEx(hProcess, lpAddress, lpBuffer, dwLength) VirtualQuery(lpAddress, lpBuffer, dwLength)
#include "IsBadCodePtrEx.c"
