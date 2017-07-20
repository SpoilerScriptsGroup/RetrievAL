#include <windows.h>

EXTERN_C void __cdecl FixAdjustByString()
{
	// TSSString::ToByteCode
	*(LPBYTE)(0x0052B830 + 1) = 0xB6;
}
