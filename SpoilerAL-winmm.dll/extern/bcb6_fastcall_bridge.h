#include <windows.h>

EXTERN_C DWORD __fastcall bcb6_fastcall_bridge1(DWORD Function, DWORD P1);

EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge2(DWORD Function, DWORD P2, DWORD P1);

#define bcb6_fastcall_bridge2(Function, P1, P2) \
	_bcb6_fastcall_bridge2(Function, P2, P1)

EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge3(DWORD P1, DWORD P2, DWORD Function, DWORD P3);

#define bcb6_fastcall_bridge3(Function, P1, P2, P3) \
	_bcb6_fastcall_bridge3(P1, P2, Function, P3)

#define bcb6_fastcall_bridge4(Function, P1, P2, P3, P4) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(P1, P2, Function, P3, P4)

#define bcb6_fastcall_bridge5(Function, P1, P2, P3, P4, P5) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(P1, P2, Function, P3, P5, P4)

#define bcb6_fastcall_bridge6(Function, P1, P2, P3, P4, P5, P6) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(P1, P2, Function, P3, P6, P5, P4)

#define bcb6_fastcall_bridge7(Function, P1, P2, P3, P4, P5, P6, P7) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(P1, P2, Function, P3, P7, P6, P5, P4)

#define bcb6_fastcall_bridge8(Function, P1, P2, P3, P4, P5, P6, P7, P8) \
	((DWORD(__fastcall *)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD))_bcb6_fastcall_bridge3) \
	(P1, P2, Function, P3, P8, P7, P6, P5, P4)

