#include <wchar.h>

__declspec(align(16)) const char xmmconst_upperA[16] = {
	#define x25 (0x7F - 'Z')
	x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25, x25
	#undef x25
};

__declspec(align(16)) const wchar_t xmmconst_upperW[8] = {
	#define x7FA5 (0x7FFF - L'Z')
	x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5, x7FA5
	#undef x7FA5
};

__declspec(align(16)) const char xmmconst_lowerA[16] = {
	#define x05 (0x7F - 'z')
	x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05, x05
	#undef x05
};

__declspec(align(16)) const wchar_t xmmconst_lowerW[8] = {
	#define x7F85 (0x7FFF - L'z')
	x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85, x7F85
	#undef x7F85
};

__declspec(align(16)) const char xmmconst_azrangeA[16] = {
	#define x65 (0x7F - ('Z' - 'A' + 1))
	x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65, x65
	#undef x65
};

__declspec(align(16)) const wchar_t xmmconst_azrangeW[8] = {
	#define x7FE5 (0x7FFF - (L'Z' - L'A' + 1))
	x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5, x7FE5
	#undef x7FE5
};

__declspec(align(16)) const char xmmconst_casebitA[16] = {      // bit to change when changing case
	#define x20 ('a' - 'A')
	x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const wchar_t xmmconst_casebitW[8] = {    // bit to change when changing case
	#define x20 (L'a' - L'A')
	x20, x20, x20, x20, x20, x20, x20, x20
	#undef x20
};

__declspec(align(16)) const char xmmconst_maskbit[32] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
