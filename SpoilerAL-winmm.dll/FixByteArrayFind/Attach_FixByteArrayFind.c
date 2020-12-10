#include <windows.h>

EXTERN_C void __cdecl TSSGCtrl_ByteArrayFind();

EXTERN_C void __cdecl Attach_FixByteArrayFind()
{
	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )(0x00508E27 + 0) = 0xE9;
	*(LPDWORD)(0x00508E27 + 1) = 0;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x00508E70 + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x00508E70 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x00509074 + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x00509074 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x00509280 + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x00509280 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )(0x0050AAC7 + 0) = 0xE9;
	*(LPDWORD)(0x0050AAC7 + 1) = 0;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050AB10 + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x0050AB10 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050AD14 + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x0050AD14 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )(0x0050C072 + 0) = 0xE9;
	*(LPDWORD)(0x0050C072 + 1) = 0;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050C0BB + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x0050C0BB + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050C2BE + 1) = (DWORD)TSSGCtrl_ByteArrayFind - (0x0050C2BE + 1 + sizeof(DWORD));
}
