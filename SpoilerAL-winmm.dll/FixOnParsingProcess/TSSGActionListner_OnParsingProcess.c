#include <windows.h>
#include "intrinsic.h"
#include "TMainForm.h"

extern HANDLE hHeap;
size_t __fastcall _ui64to16a(unsigned __int64 value, char *buffer, BOOL upper);

void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal)
{
	char message[0x1000], *p;

	switch (TMainForm_GetUserMode(MainForm))
	{
	case 3:
	case 4:
		break;
	default:
		return;
	}
	if (codeLength + 34 > sizeof(message))
		return;
	p = message;
	*(LPDWORD) p      = BSWAP32('Pars');
	*(LPDWORD)(p + 4) = BSWAP24('ing') | ((DWORD)BSWAP16('�u') << 24);
	*(LPBYTE )(p + 8) = (BYTE)(BSWAP16('�u') >> 8);
	p += 9;
	__movsb((LPBYTE)p, (LPCBYTE)code, codeLength);
	p += codeLength;
	*(LPDWORD) p      = BSWAP32('�v��');
	*(LPDWORD)(p + 4) = BSWAP24('[0x');
	p += 7;
	p += _ui64to16a(topVal, p, TRUE);
	*(LPWORD)p = BSWAP16(']\0');
	TMainForm_Guide(message, 0);
}
