#include <windows.h>
#include "intrinsic.h"
#include "TMainForm.h"

extern HANDLE hHeap;

void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal)
{
	char          *message;
	char          *p;
	unsigned long index;

	if (MainForm->userMode < 3)
		return;
	message = (char *)HeapAlloc(hHeap, 0, codeLength + 34);
	if (message == NULL)
		return;
	p = message;
	*(LPDWORD) p      = BSWAP32('Pars');
	*(LPDWORD)(p + 4) = BSWAP24('ing') | ((DWORD)BSWAP16('u') << 24);
	*(LPBYTE )(p + 8) = (BYTE)(BSWAP16('u') >> 8);
	p += 9;
	__movsb((LPBYTE)p, (LPCBYTE)code, codeLength);
	p += codeLength;
	*(LPDWORD) p      = BSWAP32('v¨');
	*(LPDWORD)(p + 4) = BSWAP24('[0x');
	p += 7;
	if (_BitScanReverse64(&index, topVal))
		p += index >> 2;
	*(LPWORD)(p + 1) = BSWAP16(']\0');
	do
	{
		BYTE c;

		c = (BYTE)topVal & 0x0F;
		c += c < 0x0A ? (BYTE)'0' : (BYTE)('A' - 0x0A);
		*(p--) = c;
	} while (topVal >>= 4);
	TMainForm_Guide(message, FALSE);
	HeapFree(hHeap, 0, message);
}
