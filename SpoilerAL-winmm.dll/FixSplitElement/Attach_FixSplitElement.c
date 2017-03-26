#include <windows.h>

EXTERN_C void __cdecl SplitElement();

EXTERN_C void Attach_FixSplitElement()
{
	// TSSDoubleList::Read
	*(LPDWORD)(0x004C45EB + 1) = (DWORD)SplitElement - (0x004C45EB + 1 + sizeof(DWORD));

	// TSSDoubleList::Write
	*(LPDWORD)(0x004C56EB + 1) = (DWORD)SplitElement - (0x004C56EB + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004C6B9D + 1) = (DWORD)SplitElement - (0x004C6B9D + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Read
	*(LPDWORD)(0x004C9AC4 + 1) = (DWORD)SplitElement - (0x004C9AC4 + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Write
	*(LPDWORD)(0x004CC467 + 1) = (DWORD)SplitElement - (0x004CC467 + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004CCA35 + 1) = (DWORD)SplitElement - (0x004CCA35 + 1 + sizeof(DWORD));
}
