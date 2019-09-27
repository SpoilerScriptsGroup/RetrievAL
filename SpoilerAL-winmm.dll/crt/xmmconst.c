#include <wchar.h>

__declspec(align(16)) const char xmm_ahighA[16] = {
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'
};

__declspec(align(16)) const wchar_t xmm_ahighW[8] = {
	L'A', L'A', L'A', L'A', L'A', L'A', L'A', L'A'
};

__declspec(align(16)) const char xmm_alowA[16] = {
	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'
};

__declspec(align(16)) const wchar_t xmm_alowW[8] = {
	L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a'
};

__declspec(align(16)) const char xmm_azrangeA[16] = {
	#define x19 ('Z' - 'A')
	x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19, x19
	#undef x19
};

__declspec(align(16)) const wchar_t xmm_azrangeW[8] = {
	#define x19 (L'Z' - L'A')
	x19, x19, x19, x19, x19, x19, x19, x19
	#undef x19
};

__declspec(align(16)) const char xmm_casebitA[16] = {   // bit to change when changing case
	#define x20 ('a' - 'A')
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const wchar_t xmm_casebitW[8] = { // bit to change when changing case
	#define x20 (L'a' - L'A')
	x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const char xmm_maskbit[32] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
