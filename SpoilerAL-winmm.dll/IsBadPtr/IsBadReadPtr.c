#define IsBadReadPtrEx IsBadReadPtrCompatible
#define VirtualQueryEx(hProcess, lpAddress, lpBuffer, dwLength) VirtualQuery(lpAddress, lpBuffer, dwLength)
#include "IsBadReadPtrEx.c"
