#include <windows.h>
#include "intrinsic.h"
#include "TMainForm.h"

extern HANDLE hHeap;
size_t __fastcall _ui64to16a(unsigned __int64 value, char *buffer, BOOL upper);

void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal)
{
	char *message, *p;

	if (MainForm->userMode != 3 && MainForm->userMode != 4)
		return;
	message = (char *)HeapAlloc(hHeap, 0, codeLength + 34);
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
	*(LPDWORD)(p + 4) = BSWAP24('[0x');
	p += 7;
	p += _ui64to16a(topVal, p, TRUE);
	*(LPWORD)p = BSWAP16(']\0');
	TMainForm_Guide(message, 0);
	HeapFree(hHeap, 0, message);
}
