#include <windows.h>
#include "intrinsic.h"

EXTERN_C void __cdecl FixMaskBytes()
{
	// TSSGCtrl::StrToProcessAccessElementVec
	/*
		lea     eax, [esi + 2]  ; 0050C6ED _ 8D. 46, 02
	*/
	*(LPDWORD)(0x0050C6EC) = BSWAP32(0xFF8D4602);

	/*
		lea     ecx, [esi + 2]  ; 0050C702 _ 8D. 4E, 02
	*/
	*(LPDWORD)(0x0050C700) = BSWAP32(0xFFFF8D4E);
}
