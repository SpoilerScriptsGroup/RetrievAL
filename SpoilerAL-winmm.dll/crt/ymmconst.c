#include <wchar.h>

__declspec(align(32)) const char ymmconst_upperA[32] = {
	#define x25 (0x7F - 'Z')
	x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25,
	x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25,
	#undef x25
};

__declspec(align(32)) const wchar_t ymmconst_upperW[16] = {
	#define x7FA5 (0x7FFF - L'Z')
	x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5,
	x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5,
	#undef x7FA5
};

__declspec(align(32)) const char ymmconst_lowerA[32] = {
	#define x05 (0x7F - 'z')
	x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05,
	x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05,
	#undef x05
};

__declspec(align(32)) const wchar_t ymmconst_lowerW[16] = {
	#define x7F85 (0x7FFF - L'z')
	x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85,
	x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85,
	#undef x7F85
};

__declspec(align(32)) const char ymmconst_azrangeA[32] = {
	#define x65 (0x7F - ('Z' - 'A' + 1))
	x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65,
	x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65,
	#undef x65
};

__declspec(align(32)) const wchar_t ymmconst_azrangeW[16] = {
	#define x7FE5 (0x7FFF - (L'Z' - L'A' + 1))
	x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5,
	x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5,
	#undef x7FE5
};

__declspec(align(32)) const char ymmconst_casebitA[32] = {      // bit to change when changing case
	#define x20 ('a' - 'A')
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20,
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20,
	#undef x20
};

__declspec(align(32)) const wchar_t ymmconst_casebitW[16] = {    // bit to change when changing case
	#define x20 (L'a' - L'A')
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20,
	#undef x20
};

__declspec(align(32)) const char ymmconst_maskbit[64] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
