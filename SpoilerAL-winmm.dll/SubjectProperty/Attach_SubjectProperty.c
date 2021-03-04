#include <windows.h>

EXTERN_C void __fastcall TSSGSubject_ctor(void*, BOOL);
#if 0
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
#endif

#define XOR_EDX_EDX      (WORD)0xD233
#define MOV_ECX_EDX      (WORD)0xCA8B
#define MOV_ECX_EDI      (WORD)0xCF8B
#define LEA_EAX_EBP_IMM8 (WORD)0x458D
#define NOP              (BYTE)0x90
#define NOP_X2           (WORD)0x9066
#define MOV_EDX_IMM32    (BYTE)0xBA
#define CALL_REL32       (BYTE)0xE8
#define JMP_REL8         (BYTE)0xEB

EXTERN_C void __cdecl Attach_SubjectProperty()
{
	// TSSGCtrl::TSSGCtrl - new TSSDir()
	*(LPBYTE )0x004DA487 = 0x05;
	*(LPBYTE )0x004DA489 = 0x8D;// edx => ecx
	*(LPWORD )0x004DA48E = XOR_EDX_EDX;
	*(LPBYTE )0x004DA490 = CALL_REL32;
	*(LPDWORD)0x004DA491 = (DWORD)TSSGSubject_ctor - (0x004DA491 + sizeof(DWORD));

#if 0
	// TSSGCtrl::EnumReadSSG - new TSSCalc()
	*(LPBYTE )0x004EA7F4 = CALL_REL32;
	*(LPDWORD)0x004EA7F5 = (DWORD)Caller_TSSGSubject_ctor2 - (0x004EA7F5 + sizeof(DWORD));
	*(LPBYTE )0x004EA7F9 = JMP_REL8;
	*(LPBYTE )0x004EA7FA = 0x004EA7FE - (0x004EA7FA + sizeof(BYTE));
	*(LPBYTE )0x004EA7FB = NOP;
	*(LPWORD )0x004EA7FC = NOP_X2;

	// TSSGCtrl::EnumReadSSG - new TSSToggle()
	*(LPBYTE )0x004EAA9E = CALL_REL32;
	*(LPDWORD)0x004EAA9F = (DWORD)Caller_TSSGSubject_ctor3 - (0x004EAA9F + sizeof(DWORD));
	*(LPBYTE )0x004EAAA3 = JMP_REL8;
	*(LPBYTE )0x004EAAA4 = 0x004EAAA8 - (0x004EAAA4 + sizeof(BYTE));
	*(LPBYTE )0x004EAAA5 = NOP;
	*(LPWORD )0x004EAAA6 = NOP_X2;

	// TSSGCtrl::EnumReadSSG - new TSSString()
	*(LPBYTE )0x004EAD6A = CALL_REL32;
	*(LPDWORD)0x004EAD6B = (DWORD)Caller_TSSGSubject_ctor4 - (0x004EAD6B + sizeof(DWORD));
	*(LPBYTE )0x004EAD6F = JMP_REL8;
	*(LPBYTE )0x004EAD70 = 0x004EAD74 - (0x004EAD70 + sizeof(BYTE));
	*(LPBYTE )0x004EAD71 = NOP;
	*(LPWORD )0x004EAD72 = NOP_X2;
#endif

	// TSSGCtrl::MakeSubjectClass - new TSSCalc()
	*(LPBYTE )0x004EC4D1 = 0x05;
	*(LPWORD )0x004EC4D5 = MOV_ECX_EDX;
	*(LPBYTE )0x004EC4D7 = CALL_REL32;
	*(LPDWORD)0x004EC4D8 = (DWORD)TSSGSubject_ctor - (0x004EC4D8 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSToggle()
	*(LPBYTE )0x004EC5C7 = 0x05;
	*(LPWORD )0x004EC5CB = MOV_ECX_EDX;
	*(LPBYTE )0x004EC5CD = CALL_REL32;
	*(LPDWORD)0x004EC5CE = (DWORD)TSSGSubject_ctor - (0x004EC5CE + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSList()
	*(LPBYTE )0x004EC6D9 = 0x05;
	*(LPWORD )0x004EC6DD = MOV_ECX_EDX;
	*(LPBYTE )0x004EC6DF = CALL_REL32;
	*(LPDWORD)0x004EC6E0 = (DWORD)TSSGSubject_ctor - (0x004EC6E0 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSDir()
	*(LPBYTE )0x004EC7C6 = 0x05;
	*(LPWORD )0x004EC7CA = MOV_ECX_EDX;
	*(LPBYTE )0x004EC7CC = CALL_REL32;
	*(LPDWORD)0x004EC7CD = (DWORD)TSSGSubject_ctor - (0x004EC7CD + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList()
	*(LPBYTE )0x004EC89F = 0x05;
	*(LPWORD )0x004EC8A3 = MOV_ECX_EDX;
	*(LPBYTE )0x004EC8A5 = CALL_REL32;
	*(LPDWORD)0x004EC8A6 = (DWORD)TSSGSubject_ctor - (0x004EC8A6 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle()
	*(LPBYTE )0x004EC9D7 = 0x05;
	*(LPWORD )0x004EC9DB = MOV_ECX_EDX;
	*(LPBYTE )0x004EC9DD = CALL_REL32;
	*(LPDWORD)0x004EC9DE = (DWORD)TSSGSubject_ctor - (0x004EC9DE + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList()
	*(LPBYTE )0x004ECB11 = 0x05;
	*(LPWORD )0x004ECB15 = MOV_ECX_EDX;
	*(LPBYTE )0x004ECB17 = CALL_REL32;
	*(LPDWORD)0x004ECB18 = (DWORD)TSSGSubject_ctor - (0x004ECB18 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSString()
	*(LPBYTE )0x004ECC28 = 0x05;
	*(LPWORD )0x004ECC2C = MOV_ECX_EDX;
	*(LPBYTE )0x004ECC2E = CALL_REL32;
	*(LPDWORD)0x004ECC2F = (DWORD)TSSGSubject_ctor - (0x004ECC2F + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc()
	*(LPBYTE )0x004ECD1E = 0x05;
	*(LPWORD )0x004ECD22 = MOV_ECX_EDX;
	*(LPBYTE )0x004ECD24 = CALL_REL32;
	*(LPDWORD)0x004ECD25 = (DWORD)TSSGSubject_ctor - (0x004ECD25 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle()
	*(LPBYTE )0x004ECE3C = 0x05;
	*(LPWORD )0x004ECE40 = MOV_ECX_EDX;
	*(LPBYTE )0x004ECE42 = CALL_REL32;
	*(LPDWORD)0x004ECE43 = (DWORD)TSSGSubject_ctor - (0x004ECE43 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSBitList()
	*(LPBYTE )0x004ECF74 = 0x05;
	*(LPWORD )0x004ECF78 = MOV_ECX_EDX;
	*(LPBYTE )0x004ECF7A = CALL_REL32;
	*(LPDWORD)0x004ECF7B = (DWORD)TSSGSubject_ctor - (0x004ECF7B + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSTrace()
	*(LPBYTE )0x004ED077 = 0x05;
	*(LPWORD )0x004ED07B = MOV_ECX_EDX;
	*(LPBYTE )0x004ED07D = CALL_REL32;
	*(LPDWORD)0x004ED07E = (DWORD)TSSGSubject_ctor - (0x004ED07E + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSCopy()
	*(LPBYTE )0x004ED143 = 0x05;
	*(LPWORD )0x004ED147 = MOV_ECX_EDX;
	*(LPBYTE )0x004ED149 = CALL_REL32;
	*(LPDWORD)0x004ED14A = (DWORD)TSSGSubject_ctor - (0x004ED14A + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc()
	*(LPBYTE )0x004ED225 = 0x05;
	*(LPWORD )0x004ED229 = MOV_ECX_EDX;
	*(LPBYTE )0x004ED22B = CALL_REL32;
	*(LPDWORD)0x004ED22C = (DWORD)TSSGSubject_ctor - (0x004ED22C + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc()
	*(LPBYTE )0x004ED31B = 0x05;
	*(LPWORD )0x004ED31F = MOV_ECX_EDX;
	*(LPBYTE )0x004ED321 = CALL_REL32;
	*(LPDWORD)0x004ED322 = (DWORD)TSSGSubject_ctor - (0x004ED322 + sizeof(DWORD));

	// TSSGCtrl::MakeSubjectClass - new TSSSplit()
	*(LPBYTE )0x004ED439 = 0x05;
	*(LPWORD )0x004ED43D = MOV_ECX_EDX;
	*(LPBYTE )0x004ED43F = CALL_REL32;
	*(LPDWORD)0x004ED440 = (DWORD)TSSGSubject_ctor - (0x004ED440 + sizeof(DWORD));

	// TSSGCtrl::ReadADJFile - TSSDir RootSubject;
	*(LPBYTE )0x004FD505 = 0x06;
	*(LPWORD )0x004FD506 = 0x0789;// mov dword ptr [edi], eax
	*(LPWORD )0x004FD508 = XOR_EDX_EDX;
	*(LPWORD )0x004FD50A = MOV_ECX_EDI;
	*(LPWORD )0x004FD50C = CALL_REL32 << 8 | NOP;
	*(LPDWORD)0x004FD50E = (DWORD)TSSGSubject_ctor - (0x004FD50E + sizeof(DWORD));

	// TSSGCtrl::LoopSSRFile - TSSGSubject SSGS;
	*(LPWORD )0x00501C8A = 0xA04D;// eax,[ebp-78h] => ecx,[ebp-60h]
	*(LPBYTE )0x00501C8F = 0x05;
	*(LPBYTE )0x00501C90 = MOV_EDX_IMM32;
	*(LPDWORD)0x00501C91 = 1;
	*(LPBYTE )0x00501C95 = CALL_REL32;
	*(LPDWORD)0x00501C96 = (DWORD)TSSGSubject_ctor - (0x00501C96 + sizeof(DWORD));
	*(LPWORD )0x00501C9A = LEA_EAX_EBP_IMM8;
	*(LPBYTE )0x00501C9C = (BYTE)-0x78;
}
