#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;

extern HANDLE             hHeap;
extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;

__inline void Attribute_allcate(LPCSTR Code, LPCSTR EndOfCode)
{
	ULARGE_INTEGER     qwValue;
	char               c, *p;
	DWORD              nId;
	size_t             nSize;
	DWORD              flProtect;
	LPSTR              endptr;
	size_t             nLength;
	PROCESSMEMORYBLOCK *lpElement;

	if (Code >= EndOfCode)
		return;
	while ((c = *Code++) == ' ' || c == '\t');
	do
		if (--EndOfCode <= Code)
			return;
	while ((c = *EndOfCode) == ' ' || c == '\t');
	Code--;
	EndOfCode++;
	qwValue.QuadPart = _strtoui64(Code, &endptr, 0);
	if (qwValue.HighPart != 0 && qwValue.HighPart != ULONG_MAX)
		return;
	nId = qwValue.LowPart;
	if (endptr == Code || endptr == EndOfCode)
		return;
	for (size_t i = 0; i < nNumberOfProcessMemory; i++)
		if ((DWORD)nId == lpProcessMemory[i].Id)
			return;
	p = endptr;
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != ',')
		return;
	p++;
	while (__intrinsic_isspace(*p))
		p++;
#ifndef _WIN64
	qwValue.QuadPart = _strtoui64(p, &endptr, 0);
	if (qwValue.HighPart != 0)
		return;
	nSize = qwValue.LowPart;
#else
	nSize = _strtoui64(p, &endptr, 0);
#endif
	if (endptr == p || endptr == EndOfCode)
		return;
	p = endptr;
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != ',')
		return;
	p++;
	while (__intrinsic_isspace(*p))
		p++;
	nLength = EndOfCode - p;
	switch (nLength)
	{
	case 0:
		return;
	case 1:
		switch (*(LPBYTE)p)
		{
		case 'R':
			flProtect = PAGE_READONLY;
			break;
		case 'E':
			flProtect = PAGE_EXECUTE;
			break;
		case 'L':
			flProtect = 0;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 2:
		switch (*(LPWORD)p)
		{
		case BSWAP16('NA'):
			flProtect = PAGE_NOACCESS;
			break;
		case BSWAP16('RW'):
			flProtect = PAGE_READWRITE;
			break;
		case BSWAP16('ER'):
			flProtect = PAGE_EXECUTE_READ;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 3:
		switch (*(LPDWORD)p & 0x00FFFFFF)
		{
		case BSWAP32('ERW\0'):
			flProtect = PAGE_EXECUTE_READWRITE;
			break;
		case BSWAP32('RNC\0'):
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
			break;
		case BSWAP32('ENC\0'):
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 4:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('RWNC'):
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
			break;
		case BSWAP32('ERNC'):
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 5:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('ERWN'):
			if (*(LPBYTE )(p + 4) != (BYTE)'C')
				return;
			flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
			break;
		case BSWAP32('Loca'):
			if (*(LPBYTE )(p + 4) != (BYTE)'l')
				return;
			flProtect = 0;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 7:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if ((*(LPDWORD)(p + 4) & 0x00FFFFFF) != BSWAP32('ute\0'))
				return;
			flProtect = PAGE_EXECUTE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 8:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Read'):
			if (*(LPDWORD)(p + 4) != BSWAP32('Only'))
				return;
			flProtect = PAGE_READONLY;
			break;
		case BSWAP32('NoAc'):
			if (*(LPDWORD)(p + 4) != BSWAP32('cess'))
				return;
			flProtect = PAGE_NOACCESS;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 9:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Read'):
			if (*(LPDWORD)(p + 4) != BSWAP32('Writ') ||
				*(LPBYTE )(p + 8) != (BYTE)  'e')
				return;
			flProtect = PAGE_READWRITE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 11:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if (*(LPDWORD)(p + 4) != BSWAP32('uteR') ||
				(*(LPDWORD)(p + 8) & 0x00FFFFFF) != BSWAP32('ead\0'))
				return;
			flProtect = PAGE_EXECUTE_READ;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 14:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if (*(LPDWORD)(p +  4) != BSWAP32('uteN') ||
				*(LPDWORD)(p +  8) != BSWAP32('oCac') ||
				*(LPWORD )(p + 12) != BSWAP16('he'))
				return;
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 15:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Read'):
			if (*(LPDWORD)(p +  4) != BSWAP32('Only') ||
				*(LPDWORD)(p +  8) != BSWAP32('NoCa') ||
				(*(LPDWORD)(p + 12) & 0x00FFFFFF) != BSWAP32('che\0'))
				return;
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 16:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if (*(LPDWORD)(p +  4) != BSWAP32('uteR') ||
				*(LPDWORD)(p +  8) != BSWAP32('eadW') ||
				*(LPDWORD)(p + 12) != BSWAP32('rite'))
				return;
			flProtect = PAGE_EXECUTE_READWRITE;
			break;
		case BSWAP32('Read'):
			if (*(LPDWORD)(p +  4) != BSWAP32('Writ') ||
				*(LPDWORD)(p +  8) != BSWAP32('eNoC') ||
				*(LPDWORD)(p + 12) != BSWAP32('ache'))
				return;
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 18:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if (*(LPDWORD)(p +  4) != BSWAP32('uteR') ||
				*(LPDWORD)(p +  8) != BSWAP32('eadN') ||
				*(LPDWORD)(p + 12) != BSWAP32('oCac') ||
				*(LPWORD )(p + 16) != BSWAP16('he'))
				return;
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	case 23:
		switch (*(LPDWORD)p)
		{
		case BSWAP32('Exec'):
			if (*(LPDWORD)(p +  4) != BSWAP32('uteR') ||
				*(LPDWORD)(p +  8) != BSWAP32('eadW') ||
				*(LPDWORD)(p + 12) != BSWAP32('rite') ||
				*(LPDWORD)(p + 16) != BSWAP32('NoCa') ||
				(*(LPDWORD)(p + 20) & 0x00FFFFFF) != BSWAP32('che\0'))
				return;
			flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
			break;
		default:
			goto UNMATCHED;
		}
		break;
	default:
	UNMATCHED:
		qwValue.QuadPart = _strtoui64(p, &endptr, 0);
		if (qwValue.HighPart || endptr == p || ((c = *endptr) && !__intrinsic_isspace(c)))
			return;
		flProtect = qwValue.LowPart;
		break;
	}
	if ((nNumberOfProcessMemory & 0x0F) == 0)
		if (nNumberOfProcessMemory != 0)
		{
			LPVOID lpMem;
			size_t nBytes;

			nBytes = (nNumberOfProcessMemory + 0x10) * sizeof(PROCESSMEMORYBLOCK);
			lpMem = HeapReAlloc(hHeap, 0, lpProcessMemory, nBytes);
			if (lpMem == NULL)
				return;
			lpProcessMemory = (PROCESSMEMORYBLOCK *)lpMem;
		}
		else
		{
			lpProcessMemory = (PROCESSMEMORYBLOCK *)HeapAlloc(hHeap, 0, 0x10 * sizeof(PROCESSMEMORYBLOCK));
			if (lpProcessMemory == NULL)
				return;
		}
	lpElement = lpProcessMemory + nNumberOfProcessMemory;
	lpElement->Id = nId;
	lpElement->Address = NULL;
	lpElement->Size = nSize;
	lpElement->Protect = flProtect;
	nNumberOfProcessMemory++;
}
