#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "intrinsic.h"
#include "TMainForm.h"

extern HANDLE hHeap;

void __stdcall TSSGActionListner_OnParsingDoubleProcess(LPCSTR code, size_t codeLength, double topVal)
{
	char buf[512];
	UINT length;
	char *message;
	char *p;

	if (MainForm->userMode != 3 && MainForm->userMode != 4)
		return;
	length = _snprintf(buf, _countof(buf), "%.*f", 1 + 16, topVal);
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
