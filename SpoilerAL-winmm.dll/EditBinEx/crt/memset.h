#ifndef memset
#if _MSC_VER > 1200
#include <intrin.h>
#pragma intrinsic(__stosb)
#elif !defined(__stosb)
#define __stosb(Dest, Data, Count)          \
do                                          \
{                                           \
    unsigned char *_Dest  = Dest;           \
    unsigned char  _Data  = Data;           \
    size_t         _Count = Count;          \
                                            \
    __asm   mov     ecx, dword ptr [_Count] \
    __asm   mov     al, byte ptr [_Data]    \
    __asm   mov     edi, dword ptr [_Dest]  \
    __asm   rep     stosb                   \
} while (0)
#endif
#define memset(dest, c, count) __stosb((void *)(dest), (unsigned char)(int)(c), (size_t)(count))
#endif
