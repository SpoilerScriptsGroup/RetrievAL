#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#define IsExcutableProtect(Protect) (   \
    ((Protect) & (                      \
        PAGE_EXECUTE_READ      |        \
        PAGE_EXECUTE_READWRITE |        \
        PAGE_EXECUTE_WRITECOPY)) &&     \
    !((Protect) & (                     \
        PAGE_NOACCESS          |        \
        PAGE_GUARD)))

BOOL __stdcall IsBadCodePtrCompatible(
	IN FARPROC lpfn)
{
	MEMORY_BASIC_INFORMATION mbi;

	return
		!VirtualQuery(lp, &mbi, sizeof(mbi)) ||
		!IsExcutableProtect(mbi.Protect);
}
