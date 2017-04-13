#include <windows.h>

EXTERN_C DWORD __fastcall bcb6_fastcall_bridge1(DWORD Function, DWORD A);

EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge2(DWORD Function, DWORD B, DWORD A);

#define bcb6_fastcall_bridge2(Function, A, B) \
	_bcb6_fastcall_bridge2(Function, B, A)

EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge3(DWORD A, DWORD B, DWORD Function, DWORD C);

#define bcb6_fastcall_bridge3(Function, A, B, C) \
	_bcb6_fastcall_bridge3(A, B, Function, C)

#define bcb6_fastcall_bridge4(Function, A, B, C, D) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(A, B, Function, C, D)

#define bcb6_fastcall_bridge5(Function, A, B, C, D, E) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(A, B, Function, C, E, D)

#define bcb6_fastcall_bridge6(Function, A, B, C, D, E, F) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(A, B, Function, C, F, E, D)

#define bcb6_fastcall_bridge7(Function, A, B, C, D, E, F, G) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(A, B, Function, C, G, F, E, D)

#define bcb6_fastcall_bridge8(Function, A, B, C, D, E, F, G, H) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(A, B, Function, C, H, G, F, E, D)

