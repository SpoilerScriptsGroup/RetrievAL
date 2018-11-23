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

void __stdcall Attribute_allcate(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	ULARGE_INTEGER     qwValue;
	DWORD              nId;
	size_t             nSize;
	DWORD              flProtect;
	LPSTR              p, endptr;
	size_t             nLength;
	PROCESSMEMORYBLOCK *lpElement;

	qwValue.QuadPart = _strtoui64(code->_M_start, &endptr, 0);
	if (qwValue.HighPart != 0)
		return;
	nId = qwValue.LowPart;
	if (endptr == code->_M_start || *endptr == '\0')
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
	if (endptr == p || *endptr == '\0')
		return;
	p = endptr;
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != ',')
		return;
	p++;
	while (__intrinsic_isspace(*p))
		p++;
	nLength = code->_M_finish - p;
	switch (nLength)
	{
	case 1:
		if (*(LPBYTE)p == 'R')
			flProtect = PAGE_READONLY;
		else if (*(LPBYTE)p == 'E')
			flProtect = PAGE_EXECUTE;
		else if (*(LPBYTE)p == 'L')
			flProtect = 0;
		else
			goto UNMATCHED;
		break;
	case 2:
		if (*(LPWORD)p == BSWAP16('NA'))
			flProtect = PAGE_NOACCESS;
		else if (*(LPWORD)p == BSWAP16('RW'))
			flProtect = PAGE_READWRITE;
		else if (*(LPWORD)p == BSWAP16('ER'))
			flProtect = PAGE_EXECUTE_READ;
		else
			goto UNMATCHED;
		break;
	case 3:
		if (*(LPDWORD)p == BSWAP32('ERW\0'))
			flProtect = PAGE_EXECUTE_READWRITE;
		else if (*(LPDWORD)p == BSWAP32('RNC\0'))
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		else if (*(LPDWORD)p == BSWAP32('ENC\0'))
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	case 4:
		if (*(LPDWORD)p == BSWAP32('RWNC'))
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		else if (*(LPDWORD)p == BSWAP32('ERNC'))
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	case 5:
		if (*(LPDWORD)p == BSWAP32('ERWN'))
			if (*(LPBYTE )(p + 4) == (BYTE)'C')
				flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
			else
				goto UNMATCHED;
		else if (*(LPDWORD)p == BSWAP32('Loca'))
			if (*(LPBYTE )(p + 4) == (BYTE)'l')
				flProtect = 0;
			else
				goto UNMATCHED;
		else
			goto UNMATCHED;
		break;
	case 7:
		if (*(LPDWORD) p      == BSWAP32('Exec') &&
			*(LPDWORD)(p + 4) == BSWAP32('ute\0'))
			flProtect = PAGE_EXECUTE;
		else
			goto UNMATCHED;
		break;
	case 8:
		if (*(LPDWORD)p == BSWAP32('Read'))
			if (*(LPDWORD)(p + 4) == BSWAP32('Only'))
				flProtect = PAGE_READONLY;
			else
				goto UNMATCHED;
		else if (*(LPDWORD)p == BSWAP32('NoAc'))
			if (*(LPDWORD)(p + 4) == BSWAP32('cess'))
				flProtect = PAGE_NOACCESS;
			else
				goto UNMATCHED;
		else
			goto UNMATCHED;
		break;
	case 9:
		if (*(LPDWORD) p      == BSWAP32('Read') &&
		    *(LPDWORD)(p + 4) == BSWAP32('Writ') &&
		    *(LPBYTE )(p + 8) == (BYTE)  'e')
			flProtect = PAGE_READWRITE;
		else
			goto UNMATCHED;
		break;
	case 11:
		if (*(LPDWORD) p      == BSWAP32('Exec') &&
		    *(LPDWORD)(p + 4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p + 8) == BSWAP32('ead\0'))
			flProtect = PAGE_EXECUTE_READ;
		else
			goto UNMATCHED;
		break;
	case 14:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteN') &&
		    *(LPDWORD)(p +  8) == BSWAP32('oCac') &&
		    *(LPWORD )(p + 12) == BSWAP16('he'))
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	case 15:
		if (*(LPDWORD) p       == BSWAP32('Read') &&
		    *(LPDWORD)(p +  4) == BSWAP32('Only') &&
		    *(LPDWORD)(p +  8) == BSWAP32('NoCa') &&
		    *(LPDWORD)(p + 12) == BSWAP32('che\0'))
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	case 16:
		if (*(LPDWORD)p == BSWAP32('Exec'))
			if (*(LPDWORD)(p +  4) == BSWAP32('uteR') &&
			    *(LPDWORD)(p +  8) == BSWAP32('eadW') &&
			    *(LPDWORD)(p + 12) == BSWAP32('rite'))
				flProtect = PAGE_EXECUTE_READWRITE;
			else
				goto UNMATCHED;
		else if (*(LPDWORD)p == BSWAP32('Read'))
			if (*(LPDWORD)(p +  4) == BSWAP32('Writ') &&
			    *(LPDWORD)(p +  8) == BSWAP32('eNoC') &&
			    *(LPDWORD)(p + 12) == BSWAP32('ache'))
				flProtect = PAGE_READONLY | PAGE_NOCACHE;
			else
				goto UNMATCHED;
		else
			goto UNMATCHED;
		break;
	case 18:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p +  8) == BSWAP32('eadN') &&
		    *(LPDWORD)(p + 12) == BSWAP32('oCac') &&
		    *(LPWORD )(p + 16) == BSWAP16('he'))
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	case 23:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p +  8) == BSWAP32('eadW') &&
		    *(LPDWORD)(p + 12) == BSWAP32('rite') &&
		    *(LPDWORD)(p + 16) == BSWAP32('NoCa') &&
		    *(LPDWORD)(p + 20) == BSWAP32('che\0'))
			flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
		else
			goto UNMATCHED;
		break;
	default:
	UNMATCHED:
		qwValue.QuadPart = _strtoui64(p, &endptr, 0);
		if (qwValue.HighPart != 0)
			return;
		flProtect = qwValue.LowPart;
		if (endptr == p || *endptr != '\0')
			return;
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
