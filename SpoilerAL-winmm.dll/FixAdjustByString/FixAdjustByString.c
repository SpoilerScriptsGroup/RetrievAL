#include <windows.h>

EXTERN_C void FixAdjustByString()
{
	// TSSString::ToByteCode
	*(LPBYTE)(0x0052B830 + 1) = 0xB6;
}
