#ifndef _WMEMMOVE_H_
#define _WMEMMOVE_H_

#if defined(_M_IX86) || defined(_M_X64)
#include "wmemcpy.h"
#ifdef _M_X64
#pragma intrinsic(__readeflags)
#pragma intrinsic(__writeeflags)
#endif
#endif

#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
void wmemmove(unsigned short *Dest, unsigned short *Source, size_t Count)
{
	if (Dest <= Source || Dest >= Source + Count)
	{
		__movsw(Dest, Source, Count);
	}
	else
	{
#ifdef _M_IX86
		Source += Count - 1;
		Dest += Count - 1;
		__asm { std }
		__movsw(Dest, Source, Count);
		__asm { cld }
#elif defined _M_X64
		size_t rflags;

		Source += Count - 1;
		Dest += Count - 1;
		rflags = __readeflags();
		__writeeflags(rflags | 0x00000400);
		__movsw(Dest, Source, Count);
		__writeeflags(rflags);
#else
		if (Count != 0)
		{
			Source += Count;
			Dest += Count;
			do
			{
				*(--Dest) = *(--Source);
			}
			while (--Count != 0);
		}
#endif
	}
}

#endif  /* _WMEMMOVE_H_ */
