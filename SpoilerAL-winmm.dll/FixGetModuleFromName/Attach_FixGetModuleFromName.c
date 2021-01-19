#include <windows.h>
#include "intrinsic.h"

EXTERN_C void *__fastcall FixGetModuleFromName(void *);

EXTERN_C void __cdecl Attach_FixGetModuleFromName()
{
	// TProcessCtrl::GetModuleFromName
	*(LPBYTE )(0x004A6305 + 0) = 0x83;// sub
	*(LPBYTE )(0x004A6305 + 1) = 0x6B;// dword ptr [ebx + ...]
	*(LPBYTE )(0x004A6305 + 3) =    2;
	*(LPBYTE )(0x004A6309 + 0) =         0x8B   ;// mov ecx,
	*(LPWORD )(0x004A6309 + 1) = BSWAP16(0x4D08);// this
	*(LPBYTE )(0x004A630C + 0) = 0xE8;// call
	*(LPDWORD)(0x004A630C + 1) = (DWORD)FixGetModuleFromName - (0x004A630C + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004A6311 + 1) = 0xB0;// ecx => eax
}
