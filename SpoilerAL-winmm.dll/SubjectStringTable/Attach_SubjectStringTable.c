#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"

void __cdecl Caller_TSSGSubject_string_ctor1();
void __cdecl Caller_TSSGSubject_string_ctor2();
void __cdecl Caller_TSSGSubject_string_ctor3();
void __cdecl Caller_TSSGSubject_string_ctor4();
void __cdecl Caller_TSSGSubject_string_ctor5();
void __cdecl Caller_TSSGSubject_string_ctor6();
void __cdecl Caller_TSSGSubject_string_ctor7();
void __cdecl Caller_TSSGSubject_string_ctor8();
void __cdecl Caller_TSSGSubject_string_ctor9();
void __cdecl Caller_TSSGSubject_string_ctor10();
void __cdecl Caller_TSSGSubject_string_ctor11();
void __cdecl Caller_TSSGSubject_string_ctor12();
void __cdecl Caller_TSSGSubject_string_ctor13();
void __cdecl Caller_TSSGSubject_string_ctor14();
void __cdecl Caller_TSSGSubject_string_ctor15();
void __cdecl Caller_TSSGSubject_string_ctor16();
void __cdecl Caller_TSSGSubject_string_ctor17();
void __cdecl Caller_TSSGSubject_string_ctor18();
void __cdecl Caller_TSSGSubject_string_ctor19();
void __cdecl Caller_TSSGSubject_string_ctor20();
void __cdecl Caller_TSSGSubject_string_ctor21();
void __cdecl Caller_TSSGSubject_string_ctor22();

void __fastcall SubjectStringTable_SetString(bcb6_std_string *dest, bcb6_std_string *src);

void __cdecl TSSGSubject_Setting_SetSubjectName();
#define      TSSSplit_Setting_SetSubjectName            TSSGSubject_Setting_SetSubjectName
void __cdecl TFindNameForm_EnumSubjectNameFind_GetName();
#define      TSearchForm_DGridSelectCell_GetName        TFindNameForm_EnumSubjectNameFind_GetName
#define      TSSGCtrl_MakeADJFile_GetCode               TFindNameForm_EnumSubjectNameFind_GetName
void __cdecl TSearchForm_Init_GetName();
void __cdecl TSSGCtrl_ReadSSG_ctor();
void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName();
void __cdecl TSSGCtrl_EnumReadSSG_SetCode();
void __cdecl TSSGCtrl_EnumReadSSG_SetName();
void __cdecl TSSGSubject_GetSubjectName_GetSubjectName();
void __cdecl TSSBitList_Setting_GetCode();
#define      TSSBundleCalc_Setting_GetCode              TSSBitList_Setting_GetCode
#define      TSSBundleList_Setting_GetCode              TSSBitList_Setting_GetCode
#define      TSSBundleToggle_Setting_GetCode            TSSBitList_Setting_GetCode
#define      TSSCalc_Setting_GetCode                    TSSBitList_Setting_GetCode
#define      TSSCopy_Setting_GetCode                    TSSBitList_Setting_GetCode
#define      TSSDoubleList_Setting_GetCode              TSSBitList_Setting_GetCode
#define      TSSDoubleToggle_Setting_GetCode            TSSBitList_Setting_GetCode
#define      TSSFloatCalc_Setting_GetCode               TSSBitList_Setting_GetCode
#define      TSSList_Setting_GetCode                    TSSBitList_Setting_GetCode
#define      TSSString_Setting_GetCode                  TSSBitList_Setting_GetCode
#define      TSSBundleFloatCalc_Setting_GetCode         TSSBitList_Setting_GetCode
#define      TSSSplit_Setting_GetCode                   TSSBitList_Setting_GetCode
#define      TSSBitList_Setting_SetSubjectName          SubjectStringTable_SetString
#define      TSSBundleCalc_Setting_SetSubjectName       SubjectStringTable_SetString
#define      TSSBundleList_Setting_SetSubjectName       SubjectStringTable_SetString
#define      TSSBundleToggle_Setting_SetSubjectName     SubjectStringTable_SetString
#define      TSSCalc_Setting_SetSubjectName             SubjectStringTable_SetString
#define      TSSCopy_Setting_SetSubjectName             SubjectStringTable_SetString
#define      TSSDoubleList_Setting_SetSubjectName       SubjectStringTable_SetString
#define      TSSDoubleToggle_Setting_SetSubjectName     SubjectStringTable_SetString
#define      TSSFloatCalc_Setting_SetSubjectName        SubjectStringTable_SetString
#define      TSSList_Setting_SetSubjectName             SubjectStringTable_SetString
#define      TSSString_Setting_SetSubjectName           SubjectStringTable_SetString
#define      TSSToggle_Setting_SetSubjectName           SubjectStringTable_SetString
#define      TSSTrace_Setting_SetSubjectName            SubjectStringTable_SetString
#define      TSSBundleFloatCalc_Setting_SetSubjectName  SubjectStringTable_SetString
void __cdecl TSSBitList_Setting_GetName();
#define      TSSBundleCalc_Setting_GetName              TSSBitList_Setting_GetName
#define      TSSBundleList_Setting_GetName              TSSBitList_Setting_GetName
#define      TSSBundleToggle_Setting_GetName            TSSBitList_Setting_GetName
#define      TSSCalc_Setting_GetName                    TSSBitList_Setting_GetName
#define      TSSCopy_Setting_GetName                    TSSBitList_Setting_GetName
#define      TSSDoubleList_Setting_GetName              TSSBitList_Setting_GetName
#define      TSSDoubleToggle_Setting_GetName            TSSBitList_Setting_GetName
#define      TSSFloatCalc_Setting_GetName               TSSBitList_Setting_GetName
#define      TSSList_Setting_GetName                    TSSBitList_Setting_GetName
#define      TSSString_Setting_GetName                  TSSBitList_Setting_GetName
#define      TSSToggle_Setting_GetName                  TSSBitList_Setting_GetName
#define      TSSTrace_Setting_GetName                   TSSBitList_Setting_GetName
#define      TSSBundleFloatCalc_Setting_GetName         TSSBitList_Setting_GetName
void __cdecl TSSToggle_Setting_GetCode();
void __cdecl TSSTrace_Setting_GetCode();

#define OPCODE_NOP      (BYTE )0x90
#define OPCODE_NOP_X2   (WORD )0x9090
#define OPCODE_NOP_X4   (DWORD)0x90909090
#define OPCODE_CALL     (BYTE )0xE8
#define OPCODE_JMP_REL8 (BYTE )0xEB

#define PUSH_EAX(addr) *(LPBYTE)(addr) = 0x50
#define PUSH_ECX(addr) *(LPBYTE)(addr) = 0x51
#define PUSH_EDX(addr) *(LPBYTE)(addr) = 0x52
#define PUSH_EBX(addr) *(LPBYTE)(addr) = 0x53
#define PUSH_ESP(addr) *(LPBYTE)(addr) = 0x54
#define PUSH_EBP(addr) *(LPBYTE)(addr) = 0x55
#define PUSH_ESI(addr) *(LPBYTE)(addr) = 0x56
#define PUSH_EDI(addr) *(LPBYTE)(addr) = 0x57
#define POP_EAX(addr)  *(LPBYTE)(addr) = 0x58
#define POP_ECX(addr)  *(LPBYTE)(addr) = 0x59
#define POP_EDX(addr)  *(LPBYTE)(addr) = 0x5A
#define POP_EBX(addr)  *(LPBYTE)(addr) = 0x5B
#define POP_ESP(addr)  *(LPBYTE)(addr) = 0x5C
#define POP_EBP(addr)  *(LPBYTE)(addr) = 0x5D
#define POP_ESI(addr)  *(LPBYTE)(addr) = 0x5E
#define POP_EDI(addr)  *(LPBYTE)(addr) = 0x5F

#define SET_PROC(addr, proc) \
    *(LPDWORD)((addr) + 1) = (DWORD)(proc) - ((addr) + 1 + sizeof(DWORD))

#define CALL(addr, proc)            \
do                                  \
{                                   \
    *(LPBYTE )(addr) = OPCODE_CALL; \
    SET_PROC(addr, proc);           \
} while (0)

#define JMP_REL8(addr, to)                                       \
do                                                               \
{                                                                \
    *(LPBYTE ) (addr)      = OPCODE_JMP_REL8;                    \
    *(LPBYTE )((addr) + 1) = (to) - ((addr) + 1 + sizeof(BYTE)); \
} while (0)

#define NPAD1(addr) \
    *(LPBYTE)(addr) = OPCODE_NOP

#define NPAD2(addr) \
    *(LPWORD)(addr) = OPCODE_NOP_X2

#define NPAD3(addr)                                      \
do                                                       \
{                                                        \
    if (~(addr) & 3) { NPAD2(addr); NPAD1((addr) + 2); } \
    else             { NPAD1(addr); NPAD2((addr) + 1); } \
} while (0)

#define NPAD4(addr) \
    *(LPDWORD)(addr) = OPCODE_NOP_X4

#define NPAD5(addr)                                      \
do                                                       \
{                                                        \
    if (~(addr) & 3) { NPAD4(addr); NPAD1((addr) + 4); } \
    else             { NPAD1(addr); NPAD4((addr) + 1); } \
} while (0)

#define NPAD6(addr)                                            \
do                                                             \
{                                                              \
    if (((addr) & 3) != 2) { NPAD4(addr); NPAD2((addr) + 4); } \
    else                   { NPAD2(addr); NPAD4((addr) + 2); } \
} while (0)

#define NPAD7(addr)                                                                    \
do                                                                                     \
{                                                                                      \
    if      (((addr) & 3) == 0) { NPAD4(addr); NPAD2((addr) + 4); NPAD1((addr) + 6); } \
    else if (((addr) & 3) == 1) { NPAD1(addr); NPAD2((addr) + 1); NPAD4((addr) + 3); } \
    else if (((addr) & 3) == 2) { NPAD2(addr); NPAD4((addr) + 2); NPAD1((addr) + 6); } \
    else                        { NPAD1(addr); NPAD4((addr) + 1); NPAD2((addr) + 5); } \
} while (0)

#define NPAD8(addr)    \
do                     \
{                      \
    NPAD4( addr     ); \
    NPAD4((addr) + 4); \
} while (0)

#define NPAD9(addr)                                      \
do                                                       \
{                                                        \
    if (~(addr) & 3) { NPAD8(addr); NPAD1((addr) + 8); } \
    else             { NPAD1(addr); NPAD8((addr) + 1); } \
} while (0)

#define NPAD10(addr)                                           \
do                                                             \
{                                                              \
    if (((addr) & 3) != 2) { NPAD8(addr); NPAD2((addr) + 8); } \
    else                   { NPAD2(addr); NPAD8((addr) + 2); } \
} while (0)

#define NPAD(addr, count) \
    if      ((count) ==  1) npad1 (addr);                                 \
    else if ((count) ==  2) npad2 (addr);                                 \
    else if ((count) ==  3) npad3 (addr);                                 \
    else if ((count) ==  4) npad4 (addr);                                 \
    else if ((count) ==  5) npad5 (addr);                                 \
    else if ((count) ==  6) npad6 (addr);                                 \
    else if ((count) ==  7) npad7 (addr);                                 \
    else if ((count) ==  8) npad8 (addr);                                 \
    else if ((count) ==  9) npad9 (addr);                                 \
    else if ((count) == 10) npad10(addr);                                 \
    else                                                                  \
        for (LPBYTE p = (LPBYTE)(addr), end = p + (count); p != end; p++) \
            *p = OPCODE_NOP

EXTERN_C void __cdecl Attach_SubjectStringTable()
{
	CALL     (0x004DA44C, Caller_TSSGSubject_string_ctor1);
	JMP_REL8 (0x004DA451, 0x004DA488);
	NPAD2    (0x004DA453);

	CALL     (0x004EA7B8, Caller_TSSGSubject_string_ctor2);
	JMP_REL8 (0x004EA7BD, 0x004EA7F4);
	NPAD2    (0x004EA7BF);

	CALL     (0x004EAA62, Caller_TSSGSubject_string_ctor3);
	JMP_REL8 (0x004EAA67, 0x004EAA9E);
	NPAD2    (0x004EAA69);

	CALL     (0x004EAD2E, Caller_TSSGSubject_string_ctor4);
	JMP_REL8 (0x004EAD33, 0x004EAD6A);
	NPAD2    (0x004EAD35);

	CALL     (0x004EC49F, Caller_TSSGSubject_string_ctor5);
	JMP_REL8 (0x004EC4A4, 0x004EC4D2);

	CALL     (0x004EC595, Caller_TSSGSubject_string_ctor6);
	JMP_REL8 (0x004EC59A, 0x004EC5C8);

	CALL     (0x004EC6A7, Caller_TSSGSubject_string_ctor7);
	JMP_REL8 (0x004EC6AC, 0x004EC6DA);

	CALL     (0x004EC794, Caller_TSSGSubject_string_ctor8);
	JMP_REL8 (0x004EC799, 0x004EC7C7);

	CALL     (0x004EC86D, Caller_TSSGSubject_string_ctor9);
	JMP_REL8 (0x004EC872, 0x004EC8A0);

	CALL     (0x004EC9A5, Caller_TSSGSubject_string_ctor10);
	JMP_REL8 (0x004EC9AA, 0x004EC9D8);

	CALL     (0x004ECADF, Caller_TSSGSubject_string_ctor11);
	JMP_REL8 (0x004ECAE4, 0x004ECB12);

	CALL     (0x004ECBF6, Caller_TSSGSubject_string_ctor12);
	JMP_REL8 (0x004ECBFB, 0x004ECC29);

	CALL     (0x004ECCEC, Caller_TSSGSubject_string_ctor13);
	JMP_REL8 (0x004ECCF1, 0x004ECD1F);

	CALL     (0x004ECE0A, Caller_TSSGSubject_string_ctor14);
	JMP_REL8 (0x004ECE0F, 0x004ECE3D);

	CALL     (0x004ECF42, Caller_TSSGSubject_string_ctor15);
	JMP_REL8 (0x004ECF47, 0x004ECF75);

	CALL     (0x004ED045, Caller_TSSGSubject_string_ctor16);
	JMP_REL8 (0x004ED04A, 0x004ED078);

	CALL     (0x004ED111, Caller_TSSGSubject_string_ctor17);
	JMP_REL8 (0x004ED116, 0x004ED144);

	CALL     (0x004ED1F3, Caller_TSSGSubject_string_ctor18);
	JMP_REL8 (0x004ED1F8, 0x004ED226);

	CALL     (0x004ED2E9, Caller_TSSGSubject_string_ctor19);
	JMP_REL8 (0x004ED2EE, 0x004ED31C);

	CALL     (0x004ED407, Caller_TSSGSubject_string_ctor20);
	JMP_REL8 (0x004ED40C, 0x004ED43A);

	CALL     (0x004FD4DA, Caller_TSSGSubject_string_ctor21);
	JMP_REL8 (0x004FD4DF, 0x004FD4FD);

	CALL     (0x00501C66, Caller_TSSGSubject_string_ctor22);
	JMP_REL8 (0x00501C6B, 0x00501C89);

	// TSSGSubject::Setting
	CALL     (0x0046CBCE, TSSGSubject_Setting_SetSubjectName);
	JMP_REL8 (0x0046CBD3, 0x0046CBF8);
	NPAD1    (0x0046CBD5);

	// TFindNameForm::EnumSubjectNameFind
	SET_PROC (0x0048520E, TFindNameForm_EnumSubjectNameFind_GetName);

	// TSearchForm::Init
	CALL     (0x00491CB1, TSearchForm_Init_GetName);
	NPAD1    (0x00491CB6);

	CALL     (0x00491D9B, TSearchForm_Init_GetName);
	PUSH_EAX (0x00491DA0);
	NPAD1    (0x00491DA1);

	// TSearchForm::DGridSelectCell
	SET_PROC (0x0049C22C, TSearchForm_DGridSelectCell_GetName);

	// TSSGCtrl::ReadSSG
	SET_PROC (0x004E44D7, TSSGCtrl_ReadSSG_ctor);

	// TSSGCtrl::EnumReadSSG
	CALL     (0x004E5D3C, TSSGCtrl_EnumReadSSG_SetCodeAndName);
	JMP_REL8 (0x004E5D41, 0x004E5DA8);

	CALL     (0x004EAE2C, TSSGCtrl_EnumReadSSG_SetCode);
	JMP_REL8 (0x004EAE31, 0x004EAE80);
	NPAD5    (0x004EAE33);

	CALL     (0x004EAF4E, TSSGCtrl_EnumReadSSG_SetName);
	JMP_REL8 (0x004EAF53, 0x004EAFA2);
	NPAD5    (0x004EAF55);

	// TSSGCtrl::MakeADJFile
	SET_PROC (0x00503164, TSSGCtrl_MakeADJFile_GetCode);

	// TSSGSubject::GetSubjectName
	CALL     (0x0052CF86, TSSGSubject_GetSubjectName_GetSubjectName);

	// TSSBitList::Setting
	CALL     (0x004B8296, TSSBitList_Setting_GetCode);
	PUSH_EAX (0x004B829B);

	/*
		lea     ecx, [ebx + 44H]                        ; 004B8EC9 _ 8D. 4B, 44
		call    TSSBitList_Setting_SetSubjectName       ; 004B8ECC _ E8, ????????
	*/
	*(LPDWORD)0x004B8EC9 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004B8ECC, TSSBitList_Setting_SetSubjectName);
	JMP_REL8 (0x004B8ED1, 0x004B8F15);
	NPAD5    (0x004B8ED3);

	SET_PROC (0x004B8D68, TSSBitList_Setting_GetName);

	// TSSBundleCalc::Setting
	CALL     (0x004BC94D, TSSBundleCalc_Setting_GetCode);

	SET_PROC (0x004BD294, TSSBundleCalc_Setting_GetName);

	CALL     (0x004BD408, TSSBundleCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004BD40D, 0x004BD463);
	NPAD5    (0x004BD40F);

	// TSSBundleList::Setting
	CALL     (0x004BEAF1, TSSBundleList_Setting_GetCode);

	SET_PROC (0x004BF03F, TSSBundleList_Setting_GetName);

	CALL     (0x004BF1B3, TSSBundleList_Setting_SetSubjectName);
	JMP_REL8 (0x004BF1B8, 0x004BF20E);
	NPAD5    (0x004BF1BA);

	// TSSBundleToggle::Setting
	CALL     (0x004BF9C9, TSSBundleToggle_Setting_GetCode);

	SET_PROC (0x004BFDC6, TSSBundleToggle_Setting_GetName);

	CALL     (0x004BFF37, TSSBundleToggle_Setting_SetSubjectName);
	JMP_REL8 (0x004BFF3C, 0x004BFF92);
	NPAD5    (0x004BFF3E);

	// TSSCalc::Setting
	CALL     (0x004C1501, TSSCalc_Setting_GetCode);

	SET_PROC (0x004C1A1C, TSSCalc_Setting_GetName);

	CALL     (0x004C1B8D, TSSCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004C1B92, 0x004C1BE8);
	NPAD5    (0x004C1B94);

	// TSSCopy::Setting
	CALL     (0x004C2455, TSSCopy_Setting_GetCode);

	SET_PROC (0x004C27CE, TSSCopy_Setting_GetName);

	CALL     (0x004C293B, TSSCopy_Setting_SetSubjectName);
	JMP_REL8 (0x004C2940, 0x004C2996);
	NPAD5    (0x004C2942);

	// TSSDoubleList::Setting
	CALL     (0x004C366E, TSSDoubleList_Setting_GetCode);
	PUSH_EAX (0x004C3673);

	SET_PROC (0x004C3E69, TSSDoubleList_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 004C3FCA _ 8D. 4B, 44
		call    TSSDoubleList_Setting_SetSubjectName    ; 004C3FCD _ E8, ????????
	*/
	*(LPDWORD)0x004C3FCA = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004C3FCD, TSSDoubleList_Setting_SetSubjectName);
	JMP_REL8 (0x004C3FD2, 0x004C4016);
	NPAD5    (0x004C3FD4);

	// TSSDoubleToggle::Setting
	CALL     (0x004C7B6A, TSSDoubleToggle_Setting_GetCode);
	PUSH_EAX (0x004C7B6F);

	SET_PROC (0x004C82C0, TSSDoubleToggle_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 004C8421 _ 8D. 4B, 44
		call    TSSDoubleToggle_Setting_SetSubjectName  ; 004C8424 _ E8, ????????
	*/
	*(LPDWORD)0x004C8421 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004C8424, TSSDoubleToggle_Setting_SetSubjectName);
	JMP_REL8 (0x004C8429, 0x004C846D);
	NPAD5    (0x004C842B);

	// TSSFloatCalc::Setting
	CALL     (0x004CDAF5, TSSFloatCalc_Setting_GetCode);

	SET_PROC (0x004CDF3B, TSSFloatCalc_Setting_GetName);

	CALL     (0x004CE0AC, TSSFloatCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004CE0B1, 0x004CE107);
	NPAD5    (0x004CE0B3);

	// TSSList::Setting
	CALL     (0x0052992D, TSSList_Setting_GetCode);

	SET_PROC (0x00529D62, TSSList_Setting_GetName);

	CALL     (0x00529ED6, TSSList_Setting_SetSubjectName);
	JMP_REL8 (0x00529EDB, 0x00529F31);
	NPAD5    (0x00529EDD);

	// TSSString::Setting
	CALL     (0x0052A6ED, TSSString_Setting_GetCode);

	SET_PROC (0x0052AB89, TSSString_Setting_GetName);

	CALL     (0x0052ACFD, TSSString_Setting_SetSubjectName);
	JMP_REL8 (0x0052AD02, 0x0052AD58);
	NPAD5    (0x0052AD04);

	// TSSToggle::Setting
	CALL     (0x0052BAF1, TSSToggle_Setting_GetCode);

	SET_PROC (0x0052C00F, TSSToggle_Setting_GetName);

	CALL     (0x0052C183, TSSToggle_Setting_SetSubjectName);
	JMP_REL8 (0x0052C188, 0x0052C1DE);
	NPAD5    (0x0052C18A);

	// TSSTrace::Setting
	CALL     (0x0052CB5E, TSSTrace_Setting_GetCode);
	NPAD1    (0x0052CB63);

	SET_PROC (0x0052CC08, TSSTrace_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 0052CD51 _ 8D. 4B, 44
		call    TSSTrace_Setting_SetSubjectName         ; 0052CD54 _ E8, ????????
	*/
	*(LPDWORD)0x0052CD51 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x0052CD54, TSSTrace_Setting_SetSubjectName);
	JMP_REL8 (0x0052CD59, 0x0052CD9D);
	NPAD5    (0x0052CD5B);

	// TSSBundleFloatCalc::Setting
	CALL     (0x0052D36D, TSSBundleFloatCalc_Setting_GetCode);

	SET_PROC (0x0052D8CB, TSSBundleFloatCalc_Setting_GetName);

	CALL     (0x0052DA3C, TSSBundleFloatCalc_Setting_SetSubjectName);
	JMP_REL8 (0x0052DA41, 0x0052DA97);
	NPAD5    (0x0052DA43);

	// TSSSplit::Setting
	CALL     (0x0052FF71, TSSSplit_Setting_GetCode);

	CALL     (0x0053056E, TSSSplit_Setting_SetSubjectName);
	JMP_REL8 (0x00530573, 0x005305CF);
	NPAD5    (0x00530575);
}
