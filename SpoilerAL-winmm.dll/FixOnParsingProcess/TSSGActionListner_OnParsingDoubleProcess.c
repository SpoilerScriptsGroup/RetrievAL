#include <windows.h>
#include "intrinsic.h"
#include "bcb6_stdio.h"
#include "TMainForm.h"

extern HANDLE hHeap;

void __stdcall TSSGActionListner_OnParsingDoubleProcess(LPCSTR code, size_t codeLength, double topVal)
{
	char buf[256];
	UINT length;
	char *message;
	char *p;

	if (MainForm->userMode < 3)
		return;
	length = bcb6__snprintf(buf, _countof(buf), "%f", topVal);
	if (length >= _countof(buf))
		length = (int)length >= 0 ? _countof(buf) - 1 : 0;
	message = (char *)HeapAlloc(hHeap, 0, codeLength + length + 16);
	if (message == NULL)
		return;
	p = message;
	*(LPDWORD) p      = BSWAP32('Pars');
	*(LPDWORD)(p + 4) = BSWAP24('ing') | ((DWORD)BSWAP16('Åu') << 24);
	*(LPBYTE )(p + 8) = (BYTE)(BSWAP16('Åu') >> 8);
	p += 9;
	__movsb((LPBYTE)p, (LPCBYTE)code, codeLength);
	p += codeLength;
	*(LPDWORD) p      = BSWAP32('ÅvÅ®');
	*(LPBYTE )(p + 4) = (BYTE)'[';
	p += 5;
	__movsb((LPBYTE)p, (LPCBYTE)buf, length);
	p += length;
	*(LPWORD)p = BSWAP16(']\0');
	TMainForm_Guide(message, FALSE);
	HeapFree(hHeap, 0, message);
}
