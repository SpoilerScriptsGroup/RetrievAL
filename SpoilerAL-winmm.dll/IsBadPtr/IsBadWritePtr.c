#define IsBadWritePtrEx IsBadWritePtrCompatible
#define VirtualQueryEx(hProcess, lpAddress, lpBuffer, dwLength) VirtualQuery(lpAddress, lpBuffer, dwLength)
#include "IsBadWritePtrEx.c"
