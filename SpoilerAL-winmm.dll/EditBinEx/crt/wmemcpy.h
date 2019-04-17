#ifndef wmemcpy
#if _MSC_VER > 1200
#include <intrin.h>
#pragma intrinsic(__movsw)
#elif !defined(__movsw)
#define __movsw(Destination, Source, Count)           \
do                                                    \
{                                                     \
    unsigned short *      _Destination = Destination; \
    const unsigned short *_Source      = Source;      \
    size_t                _Count       = Count;       \
                                                      \
    __asm   mov     ecx, dword ptr [_Count]           \
    __asm   mov     esi, dword ptr [_Source]          \
    __asm   mov     edi, dword ptr [_Destination]     \
    __asm   rep movsw                                 \
} while (0)
#endif
#define wmemcpy __movsw
#endif
