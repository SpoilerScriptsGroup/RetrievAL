#include <wchar.h>

__declspec(align(16)) const char casebitA[16] = {    // bit to change when changing case
	#define x20 ('a' - 'A')
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const wchar_t casebitW[8] = {  // bit to change when changing case
	#define x20 (L'a' - L'A')
	x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const char maskbit[32] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
