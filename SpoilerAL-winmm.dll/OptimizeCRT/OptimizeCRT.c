#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#pragma comment(lib, "crt\\asmlib\\libacof32o.lib")

EXTERN_C void * __cdecl A_memcpy(void *dest, const void *src, size_t count);
EXTERN_C void * __cdecl A_memmove(void *dest, const void *src, size_t count);
EXTERN_C void * __cdecl A_memset(void *dest, int c, size_t count);
EXTERN_C char * __cdecl A_strcat(char *string1, const char *string2);
EXTERN_C size_t __cdecl A_strlen(const char *string);
EXTERN_C int __cdecl A_memcmp(const void *buf1, const void *buf2, size_t count);
EXTERN_C int __cdecl bcb6__snprintf(char *buffer, size_t count, const char *format, ...);
EXTERN_C unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base);
EXTERN_C int __stdcall _lstrcmpA(LPCTSTR lpString1, LPCTSTR lpString2);
EXTERN_C LPTSTR __stdcall _lstrcpyA(LPTSTR lpString1, LPCTSTR lpString2);
EXTERN_C int __stdcall _lstrlenA(LPCTSTR lpString);

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90
#define NOP_X2    (WORD)0x9090

EXTERN_C void __cdecl OptimizeCRT()
{
	*(LPBYTE )0x005D5108 = JMP_REL32;
	*(LPDWORD)0x005D5109 = (DWORD)A_memcpy - (0x005D5109 + sizeof(DWORD));

	*(LPBYTE )0x005D512C = JMP_REL32;
	*(LPDWORD)0x005D512D = (DWORD)A_memmove - (0x005D512D + sizeof(DWORD));

	*(LPBYTE )0x005D5178 = JMP_REL32;
	*(LPDWORD)0x005D5179 = (DWORD)A_memset - (0x005D5179 + sizeof(DWORD));
	*(LPWORD )0x005D517D = NOP_X2;

	*(LPBYTE )0x005D521C = JMP_REL32;
	*(LPDWORD)0x005D521D = (DWORD)A_strcat - (0x005D521D + sizeof(DWORD));

	*(LPBYTE )0x005D5258 = JMP_REL32;
	*(LPDWORD)0x005D5259 = (DWORD)A_strlen - (0x005D5259 + sizeof(DWORD));
	*(LPBYTE )0x005D525D = NOP;

	*(LPBYTE )0x005D534C = JMP_REL32;
	*(LPDWORD)0x005D534D = (DWORD)A_memcmp - (0x005D534D + sizeof(DWORD));

	*(LPBYTE )0x005D7EE8 = JMP_REL32;
	*(LPDWORD)0x005D7EE9 = (DWORD)bcb6__snprintf - (0x005D7EE9 + sizeof(DWORD));
	*(LPWORD )0x005D7EED = NOP_X2;

	*(LPBYTE )0x005D9C68 = JMP_REL32;
	*(LPDWORD)0x005D9C69 = (DWORD)_ultoa - (0x005D9C69 + sizeof(DWORD));

	*(LPBYTE )0x005DAF54 = JMP_REL32;
	*(LPDWORD)0x005DAF55 = (DWORD)bcb6_strtoul - (0x005DAF55 + sizeof(DWORD));
	*(LPBYTE )0x005DAF59 = NOP;

	*(LPBYTE )0x00600EA8 = JMP_REL32;
	*(LPDWORD)0x00600EA9 = (DWORD)_lstrcmpA - (0x00600EA9 + sizeof(DWORD));
	*(LPBYTE )0x00600EAD = NOP;

	*(LPBYTE )0x00600EAE = JMP_REL32;
	*(LPDWORD)0x00600EAF = (DWORD)_lstrcpyA - (0x00600EAF + sizeof(DWORD));
	*(LPBYTE )0x00600EB3 = NOP;

	*(LPBYTE )0x00600EBA = JMP_REL32;
	*(LPDWORD)0x00600EBB = (DWORD)_lstrlenA - (0x00600EBB + sizeof(DWORD));
	*(LPBYTE )0x00600EBF = NOP;
}
