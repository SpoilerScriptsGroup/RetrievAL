#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#define IsReadableProtect(Protect) ( \
    ((Protect) & (                   \
        PAGE_READONLY          |     \
        PAGE_READWRITE         |     \
        PAGE_WRITECOPY         |     \
        PAGE_EXECUTE_READ      |     \
        PAGE_EXECUTE_READWRITE |     \
        PAGE_EXECUTE_WRITECOPY)) &&  \
    !((Protect) & (                  \
        PAGE_NOACCESS          |     \
        PAGE_GUARD)))

BOOL __stdcall IsBadCodePtrEx(
#ifndef VirtualQueryEx
	IN HANDLE  hProcess,
#endif
	IN FARPROC lpfn)
{
	MEMORY_BASIC_INFORMATION mbi;

	return
		!VirtualQueryEx(hProcess, lpfn, &mbi, sizeof(mbi)) ||
		!IsReadableProtect(mbi.Protect);
}
