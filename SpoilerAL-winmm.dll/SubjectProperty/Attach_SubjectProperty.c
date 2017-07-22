#include <windows.h>

EXTERN_C void __cdecl Caller_TSSGSubject_ctor1();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor2();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor3();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor4();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor5();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor6();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor7();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor8();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor9();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor10();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor11();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor12();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor13();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor14();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor15();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor16();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor17();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor18();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor19();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor20();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor21();
EXTERN_C void __cdecl Caller_TSSGSubject_ctor22();

#define LEA_EAX_EBP_IMM8 (WORD)0x458D
#define NOP              (BYTE)0x90
#define NOP_X2           (WORD)0x9090
#define MOV_EAX_IMM32    (BYTE)0xB8
#define CALL_REL32       (BYTE)0xE8
#define JMP_REL8         (BYTE)0xEB

EXTERN_C void __cdecl Attach_SubjectProperty()
{
	*(LPBYTE )0x004DA488 = CALL_REL32;
	*(LPDWORD)0x004DA489 = (DWORD)Caller_TSSGSubject_ctor1 - (0x004DA489 + sizeof(DWORD));
	*(LPBYTE )0x004DA48D = JMP_REL8;
	*(LPBYTE )0x004DA48E = 0x004DA492 - (0x004DA48E + sizeof(BYTE));
	*(LPBYTE )0x004DA48F = NOP;
	*(LPWORD )0x004DA490 = NOP_X2;

	*(LPBYTE )0x004EA7F4 = CALL_REL32;
	*(LPDWORD)0x004EA7F5 = (DWORD)Caller_TSSGSubject_ctor2 - (0x004EA7F5 + sizeof(DWORD));
	*(LPBYTE )0x004EA7F9 = JMP_REL8;
	*(LPBYTE )0x004EA7FA = 0x004EA7FE - (0x004EA7FA + sizeof(BYTE));
	*(LPBYTE )0x004EA7FB = NOP;
	*(LPWORD )0x004EA7FC = NOP_X2;

	*(LPBYTE )0x004EAA9E = CALL_REL32;
	*(LPDWORD)0x004EAA9F = (DWORD)Caller_TSSGSubject_ctor3 - (0x004EAA9F + sizeof(DWORD));
	*(LPBYTE )0x004EAAA3 = JMP_REL8;
	*(LPBYTE )0x004EAAA4 = 0x004EAAA8 - (0x004EAAA4 + sizeof(BYTE));
	*(LPBYTE )0x004EAAA5 = NOP;
	*(LPWORD )0x004EAAA6 = NOP_X2;

	*(LPBYTE )0x004EAD6A = CALL_REL32;
	*(LPDWORD)0x004EAD6B = (DWORD)Caller_TSSGSubject_ctor4 - (0x004EAD6B + sizeof(DWORD));
	*(LPBYTE )0x004EAD6F = JMP_REL8;
	*(LPBYTE )0x004EAD70 = 0x004EAD74 - (0x004EAD70 + sizeof(BYTE));
	*(LPBYTE )0x004EAD71 = NOP;
	*(LPWORD )0x004EAD72 = NOP_X2;

	*(LPBYTE )0x004EC4D2 = CALL_REL32;
	*(LPDWORD)0x004EC4D3 = (DWORD)Caller_TSSGSubject_ctor5 - (0x004EC4D3 + sizeof(DWORD));
	*(LPBYTE )0x004EC4D7 = JMP_REL8;
	*(LPBYTE )0x004EC4D8 = 0x004EC4DC - (0x004EC4D8 + sizeof(BYTE));
	*(LPBYTE )0x004EC4D9 = NOP;
	*(LPWORD )0x004EC4DA = NOP_X2;

	*(LPBYTE )0x004EC5C8 = CALL_REL32;
	*(LPDWORD)0x004EC5C9 = (DWORD)Caller_TSSGSubject_ctor6 - (0x004EC5C9 + sizeof(DWORD));
	*(LPBYTE )0x004EC5CD = JMP_REL8;
	*(LPBYTE )0x004EC5CE = 0x00;

	*(LPBYTE )0x004EC6DA = CALL_REL32;
	*(LPDWORD)0x004EC6DB = (DWORD)Caller_TSSGSubject_ctor7 - (0x004EC6DB + sizeof(DWORD));
	*(LPBYTE )0x004EC6DF = JMP_REL8;
	*(LPBYTE )0x004EC6E0 = 0x00;

	*(LPBYTE )0x004EC7C7 = CALL_REL32;
	*(LPDWORD)0x004EC7C8 = (DWORD)Caller_TSSGSubject_ctor8 - (0x004EC7C8 + sizeof(DWORD));
	*(LPBYTE )0x004EC7CC = JMP_REL8;
	*(LPBYTE )0x004EC7CD = 0x00;

	*(LPBYTE )0x004EC8A0 = CALL_REL32;
	*(LPDWORD)0x004EC8A1 = (DWORD)Caller_TSSGSubject_ctor9 - (0x004EC8A1 + sizeof(DWORD));
	*(LPBYTE )0x004EC8A5 = JMP_REL8;
	*(LPBYTE )0x004EC8A6 = 0x00;

	*(LPBYTE )0x004EC9D8 = CALL_REL32;
	*(LPDWORD)0x004EC9D9 = (DWORD)Caller_TSSGSubject_ctor10 - (0x004EC9D9 + sizeof(DWORD));
	*(LPBYTE )0x004EC9DD = JMP_REL8;
	*(LPBYTE )0x004EC9DE = 0x00;

	*(LPBYTE )0x004ECB12 = CALL_REL32;
	*(LPDWORD)0x004ECB13 = (DWORD)Caller_TSSGSubject_ctor11 - (0x004ECB13 + sizeof(DWORD));
	*(LPBYTE )0x004ECB17 = JMP_REL8;
	*(LPBYTE )0x004ECB18 = 0x00;

	*(LPBYTE )0x004ECC29 = CALL_REL32;
	*(LPDWORD)0x004ECC2A = (DWORD)Caller_TSSGSubject_ctor12 - (0x004ECC2A + sizeof(DWORD));
	*(LPBYTE )0x004ECC2E = JMP_REL8;
	*(LPBYTE )0x004ECC2F = 0x00;

	*(LPBYTE )0x004ECD1F = CALL_REL32;
	*(LPDWORD)0x004ECD20 = (DWORD)Caller_TSSGSubject_ctor13 - (0x004ECD20 + sizeof(DWORD));
	*(LPBYTE )0x004ECD24 = JMP_REL8;
	*(LPBYTE )0x004ECD25 = 0x00;

	*(LPBYTE )0x004ECE3D = CALL_REL32;
	*(LPDWORD)0x004ECE3E = (DWORD)Caller_TSSGSubject_ctor14 - (0x004ECE3E + sizeof(DWORD));
	*(LPBYTE )0x004ECE42 = JMP_REL8;
	*(LPBYTE )0x004ECE43 = 0x00;

	*(LPBYTE )0x004ECF75 = CALL_REL32;
	*(LPDWORD)0x004ECF76 = (DWORD)Caller_TSSGSubject_ctor15 - (0x004ECF76 + sizeof(DWORD));
	*(LPBYTE )0x004ECF7A = JMP_REL8;
	*(LPBYTE )0x004ECF7B = 0x00;

	*(LPBYTE )0x004ED078 = CALL_REL32;
	*(LPDWORD)0x004ED079 = (DWORD)Caller_TSSGSubject_ctor16 - (0x004ED079 + sizeof(DWORD));
	*(LPBYTE )0x004ED07D = JMP_REL8;
	*(LPBYTE )0x004ED07E = 0x00;

	*(LPBYTE )0x004ED144 = CALL_REL32;
	*(LPDWORD)0x004ED145 = (DWORD)Caller_TSSGSubject_ctor17 - (0x004ED145 + sizeof(DWORD));
	*(LPBYTE )0x004ED149 = JMP_REL8;
	*(LPBYTE )0x004ED14A = 0x00;

	*(LPBYTE )0x004ED226 = CALL_REL32;
	*(LPDWORD)0x004ED227 = (DWORD)Caller_TSSGSubject_ctor18 - (0x004ED227 + sizeof(DWORD));
	*(LPBYTE )0x004ED22B = JMP_REL8;
	*(LPBYTE )0x004ED22C = 0x00;

	*(LPBYTE )0x004ED31C = CALL_REL32;
	*(LPDWORD)0x004ED31D = (DWORD)Caller_TSSGSubject_ctor19 - (0x004ED31D + sizeof(DWORD));
	*(LPBYTE )0x004ED321 = JMP_REL8;
	*(LPBYTE )0x004ED322 = 0x00;

	*(LPBYTE )0x004ED43A = CALL_REL32;
	*(LPDWORD)0x004ED43B = (DWORD)Caller_TSSGSubject_ctor20 - (0x004ED43B + sizeof(DWORD));
	*(LPBYTE )0x004ED43F = JMP_REL8;
	*(LPBYTE )0x004ED440 = 0x00;

	*(LPBYTE )0x004FD4FD = CALL_REL32;
	*(LPDWORD)0x004FD4FE = (DWORD)Caller_TSSGSubject_ctor21 - (0x004FD4FE + sizeof(DWORD));
	*(LPBYTE )0x004FD502 = MOV_EAX_IMM32;
	*(LPDWORD)0x004FD503 = 0x006403C0;
	*(LPBYTE )0x004FD507 = JMP_REL8;
	*(LPBYTE )0x004FD508 = 0x004FD50A - (0x004FD508 + sizeof(BYTE));
	*(LPBYTE )0x004FD509 = NOP;

	*(LPBYTE )0x00501C89 = CALL_REL32;
	*(LPDWORD)0x00501C8A = (DWORD)Caller_TSSGSubject_ctor22 - (0x00501C8A + sizeof(DWORD));
	*(LPWORD )0x00501C8E = LEA_EAX_EBP_IMM8;
	*(LPBYTE )0x00501C90 = (char)-0x78;
	*(LPBYTE )0x00501C91 = JMP_REL8;
	*(LPBYTE )0x00501C92 = 0x00501C94 - (0x00501C92 + sizeof(BYTE));
	*(LPBYTE )0x00501C93 = NOP;
}
