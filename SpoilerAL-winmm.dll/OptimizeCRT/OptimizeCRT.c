#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

EXTERN_C void * __cdecl bcb6_calloc(size_t num, size_t size);
EXTERN_C void * __cdecl bcb6_malloc(size_t size);
EXTERN_C void __cdecl bcb6_free(void *memblock);
EXTERN_C void * __cdecl bcb6_realloc(void *memblock, size_t size);
EXTERN_C int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...);
EXTERN_C double __cdecl bcb6_strtod(const char *nptr, char **endptr);
EXTERN_C long __cdecl bcb6_strtol(const char *nptr, char **endptr, int base);
EXTERN_C unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base);
EXTERN_C char * __stdcall _lstrcpyA(char *lpString1, const char *lpString2);
EXTERN_C char * __stdcall _lstrcpynA(char *lpString1, const char *lpString2, int iMaxLength);
EXTERN_C int __stdcall _lstrlenA(const char *lpString);

#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X2    (WORD )0x9090
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl OptimizeCRT()
{
	*(LPBYTE )0x005D4458 = JMP_REL32;
	*(LPDWORD)0x005D4459 = (DWORD)bcb6_calloc - (0x005D4459 + sizeof(DWORD));

	*(LPBYTE )0x005D4B3C = JMP_REL32;
	*(LPDWORD)0x005D4B3D = (DWORD)bcb6_malloc - (0x005D4B3D + sizeof(DWORD));
	*(LPBYTE )0x005D4B41 = NOP;

	*(LPBYTE )0x005D4B4C = JMP_REL32;
	*(LPDWORD)0x005D4B4D = (DWORD)bcb6_free - (0x005D4B4D + sizeof(DWORD));
	*(LPBYTE )0x005D4B51 = NOP;

	*(LPBYTE )0x005D4B5C = JMP_REL32;
	*(LPDWORD)0x005D4B5D = (DWORD)bcb6_realloc - (0x005D4B5D + sizeof(DWORD));
	*(LPBYTE )0x005D4B61 = NOP;

	*(LPBYTE )0x005D50E8 = JMP_REL32;
	*(LPDWORD)0x005D50E9 = (DWORD)memchr - (0x005D50E9 + sizeof(DWORD));
	*(LPWORD )0x005D50ED = NOP_X2;

	*(LPBYTE )0x005D5108 = JMP_REL32;
	*(LPDWORD)0x005D5109 = (DWORD)memcpy - (0x005D5109 + sizeof(DWORD));

	*(LPBYTE )0x005D512C = JMP_REL32;
	*(LPDWORD)0x005D512D = (DWORD)memmove - (0x005D512D + sizeof(DWORD));

	*(LPBYTE )0x005D5178 = JMP_REL32;
	*(LPDWORD)0x005D5179 = (DWORD)memset - (0x005D5179 + sizeof(DWORD));
	*(LPWORD )0x005D517D = NOP_X2;

	*(LPBYTE )0x005D521C = JMP_REL32;
	*(LPDWORD)0x005D521D = (DWORD)strcat - (0x005D521D + sizeof(DWORD));

	*(LPBYTE )0x005D5258 = JMP_REL32;
	*(LPDWORD)0x005D5259 = (DWORD)strlen - (0x005D5259 + sizeof(DWORD));
	*(LPBYTE )0x005D525D = NOP;

	*(LPBYTE )0x005D52B4 = JMP_REL32;
	*(LPDWORD)0x005D52B5 = (DWORD)strncat - (0x005D52B5 + sizeof(DWORD));

	*(LPBYTE )0x005D5324 = JMP_REL32;
	*(LPDWORD)0x005D5325 = (DWORD)strncmp - (0x005D5325 + sizeof(DWORD));

	*(LPBYTE )0x005D534C = JMP_REL32;
	*(LPDWORD)0x005D534D = (DWORD)memcmp - (0x005D534D + sizeof(DWORD));

	*(LPBYTE )0x005D53EC = JMP_REL32;
	*(LPDWORD)0x005D53ED = (DWORD)strncpy - (0x005D53ED + sizeof(DWORD));

	*(LPBYTE )0x005D7EE8 = JMP_REL32;
	*(LPDWORD)0x005D7EE9 = (DWORD)bcb6_snprintf - (0x005D7EE9 + sizeof(DWORD));
	*(LPWORD )0x005D7EED = NOP_X2;

	*(LPBYTE )0x005D9C38 = JMP_REL32;
	*(LPDWORD)0x005D9C39 = (DWORD)_itoa - (0x005D9C39 + sizeof(DWORD));
	*(LPBYTE )0x005D9C3D = NOP;

	*(LPBYTE )0x005D9C88 = JMP_REL32;
	*(LPDWORD)0x005D9C89 = (DWORD)_ltoa - (0x005D9C89 + sizeof(DWORD));
	*(LPBYTE )0x005D9C8D = NOP;

	*(LPBYTE )0x005D9C68 = JMP_REL32;
	*(LPDWORD)0x005D9C69 = (DWORD)_ultoa - (0x005D9C69 + sizeof(DWORD));

	*(LPBYTE )0x005DAE00 = JMP_REL32;
	*(LPDWORD)0x005DAE01 = (DWORD)bcb6_strtod - (0x005DAE01 + sizeof(DWORD));
	*(LPDWORD)0x005DAE05 = NOP_X4;

	*(LPBYTE )0x005DAE5C = JMP_REL32;
	*(LPDWORD)0x005DAE5D = (DWORD)bcb6_strtol - (0x005DAE5D + sizeof(DWORD));
	*(LPBYTE )0x005DAE61 = NOP;

	*(LPBYTE )0x005DAF54 = JMP_REL32;
	*(LPDWORD)0x005DAF55 = (DWORD)bcb6_strtoul - (0x005DAF55 + sizeof(DWORD));
	*(LPBYTE )0x005DAF59 = NOP;

	*(LPBYTE )0x00600EAE = JMP_REL32;
	*(LPDWORD)0x00600EAF = (DWORD)_lstrcpyA - (0x00600EAF + sizeof(DWORD));
	*(LPBYTE )0x00600EB3 = NOP;

	*(LPBYTE )0x00600EB4 = JMP_REL32;
	*(LPDWORD)0x00600EB5 = (DWORD)_lstrcpynA - (0x00600EB5 + sizeof(DWORD));
	*(LPBYTE )0x00600EB9 = NOP;

	*(LPBYTE )0x00600EBA = JMP_REL32;
	*(LPDWORD)0x00600EBB = (DWORD)_lstrlenA - (0x00600EBB + sizeof(DWORD));
	*(LPBYTE )0x00600EBF = NOP;
}
