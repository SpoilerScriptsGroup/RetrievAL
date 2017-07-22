#include <windows.h>

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
void __cdecl TSSGSubject_Setting_SetSubjectName();
void __cdecl TFindNameForm_EnumSubjectNameFind_GetName();
void __cdecl TSearchForm_Init_GetName();
void __cdecl TSearchForm_DGridSelectCell_GetName();
void __cdecl TSSGCtrl_ReadSSG_ctor();
void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName();
void __cdecl TSSGCtrl_EnumReadSSG_SetCode();
void __cdecl TSSGCtrl_EnumReadSSG_SetName();
void __cdecl TSSGCtrl_MakeADJFile_GetCode();
void __cdecl TSSGSubject_GetSubjectName_GetSubjectName();
void __cdecl TSSBitList_Setting_GetCode();
void __cdecl TSSBitList_Setting_SetSubjectName();
void __cdecl TSSBitList_Setting_GetName();
#define TSSBundleCalc_Setting_GetCode      TSSBitList_Setting_GetCode
#define TSSBundleList_Setting_GetCode      TSSBitList_Setting_GetCode
#define TSSBundleToggle_Setting_GetCode    TSSBitList_Setting_GetCode
#define TSSCalc_Setting_GetCode            TSSBitList_Setting_GetCode
#define TSSCopy_Setting_GetCode            TSSBitList_Setting_GetCode
#define TSSDoubleList_Setting_GetCode      TSSBitList_Setting_GetCode
#define TSSDoubleToggle_Setting_GetCode    TSSBitList_Setting_GetCode
#define TSSFloatCalc_Setting_GetCode       TSSBitList_Setting_GetCode
#define TSSList_Setting_GetCode            TSSBitList_Setting_GetCode
#define TSSString_Setting_GetCode          TSSBitList_Setting_GetCode
#define TSSBundleFloatCalc_Setting_GetCode TSSBitList_Setting_GetCode
#define TSSSplit_Setting_GetCode           TSSBitList_Setting_GetCode
#define TSSBundleCalc_Setting_GetName      TSSBitList_Setting_GetName
#define TSSBundleList_Setting_GetName      TSSBitList_Setting_GetName
#define TSSBundleToggle_Setting_GetName    TSSBitList_Setting_GetName
#define TSSCalc_Setting_GetName            TSSBitList_Setting_GetName
#define TSSCopy_Setting_GetName            TSSBitList_Setting_GetName
#define TSSDoubleList_Setting_GetName      TSSBitList_Setting_GetName
#define TSSDoubleToggle_Setting_GetName    TSSBitList_Setting_GetName
#define TSSFloatCalc_Setting_GetName       TSSBitList_Setting_GetName
#define TSSList_Setting_GetName            TSSBitList_Setting_GetName
#define TSSString_Setting_GetName          TSSBitList_Setting_GetName
#define TSSToggle_Setting_GetName          TSSBitList_Setting_GetName
#define TSSTrace_Setting_GetName           TSSBitList_Setting_GetName
#define TSSBundleFloatCalc_Setting_GetName TSSBitList_Setting_GetName
void __cdecl TSSBundleCalc_Setting_SetSubjectName();
void __cdecl TSSBundleList_Setting_SetSubjectName();
void __cdecl TSSBundleToggle_Setting_SetSubjectName();
void __cdecl TSSCalc_Setting_SetSubjectName();
void __cdecl TSSCopy_Setting_SetSubjectName();
void __cdecl TSSDoubleList_Setting_SetSubjectName();
void __cdecl TSSDoubleToggle_Setting_SetSubjectName();
void __cdecl TSSFloatCalc_Setting_SetSubjectName();
void __cdecl TSSList_Setting_SetSubjectName();
void __cdecl TSSString_Setting_SetSubjectName();
void __cdecl TSSToggle_Setting_GetCode();
void __cdecl TSSToggle_Setting_SetSubjectName();
void __cdecl TSSTrace_Setting_GetCode();
void __cdecl TSSTrace_Setting_SetSubjectName();
void __cdecl TSSBundleFloatCalc_Setting_SetSubjectName();
void __cdecl TSSSplit_Setting_SetSubjectName();

#define PUSH_EAX   (BYTE )0x50
#define NOP        (BYTE )0x90
#define NOP_X2     (WORD )0x9090
#define NOP_X4     (DWORD)0x90909090
#define CALL_REL32 (BYTE )0xE8
#define JMP_REL32  (BYTE )0xE9
#define JMP_REL8   (BYTE )0xEB

EXTERN_C void __cdecl Attach_SubjectStringTable()
{
	*(LPBYTE )0x004DA44C = CALL_REL32;
	*(LPDWORD)0x004DA44D = (DWORD)Caller_TSSGSubject_string_ctor1 - (0x004DA44D + sizeof(DWORD));
	*(LPBYTE )0x004DA451 = JMP_REL8;
	*(LPBYTE )0x004DA452 = 0x004DA488 - (0x004DA452 + sizeof(BYTE));
	*(LPWORD )0x004DA453 = NOP_X2;

	*(LPBYTE )0x004EA7B8 = CALL_REL32;
	*(LPDWORD)0x004EA7B9 = (DWORD)Caller_TSSGSubject_string_ctor2 - (0x004EA7B9 + sizeof(DWORD));
	*(LPBYTE )0x004EA7BD = JMP_REL8;
	*(LPBYTE )0x004EA7BE = 0x004EA7F4 - (0x004EA7BE + sizeof(BYTE));
	*(LPWORD )0x004EA7BF = NOP_X2;

	*(LPBYTE )0x004EAA62 = CALL_REL32;
	*(LPDWORD)0x004EAA63 = (DWORD)Caller_TSSGSubject_string_ctor3 - (0x004EAA63 + sizeof(DWORD));
	*(LPBYTE )0x004EAA67 = JMP_REL8;
	*(LPBYTE )0x004EAA68 = 0x004EAA9E - (0x004EAA68 + sizeof(BYTE));
	*(LPWORD )0x004EAA69 = NOP_X2;

	*(LPBYTE )0x004EAD2E = CALL_REL32;
	*(LPDWORD)0x004EAD2F = (DWORD)Caller_TSSGSubject_string_ctor4 - (0x004EAD2F + sizeof(DWORD));
	*(LPBYTE )0x004EAD33 = JMP_REL8;
	*(LPBYTE )0x004EAD34 = 0x004EAD6A - (0x004EAD34 + sizeof(BYTE));
	*(LPWORD )0x004EAD35 = NOP_X2;

	*(LPBYTE )0x004EC49F = CALL_REL32;
	*(LPDWORD)0x004EC4A0 = (DWORD)Caller_TSSGSubject_string_ctor5 - (0x004EC4A0 + sizeof(DWORD));
	*(LPBYTE )0x004EC4A4 = JMP_REL8;
	*(LPBYTE )0x004EC4A5 = 0x004EC4D2 - (0x004EC4A5 + sizeof(BYTE));

	*(LPBYTE )0x004EC595 = CALL_REL32;
	*(LPDWORD)0x004EC596 = (DWORD)Caller_TSSGSubject_string_ctor6 - (0x004EC596 + sizeof(DWORD));
	*(LPBYTE )0x004EC59A = JMP_REL8;
	*(LPBYTE )0x004EC59B = 0x004EC5C8 - (0x004EC59B + sizeof(BYTE));

	*(LPBYTE )0x004EC6A7 = CALL_REL32;
	*(LPDWORD)0x004EC6A8 = (DWORD)Caller_TSSGSubject_string_ctor7 - (0x004EC6A8 + sizeof(DWORD));
	*(LPBYTE )0x004EC6AC = JMP_REL8;
	*(LPBYTE )0x004EC6AD = 0x004EC6DA - (0x004EC6AD + sizeof(BYTE));

	*(LPBYTE )0x004EC794 = CALL_REL32;
	*(LPDWORD)0x004EC795 = (DWORD)Caller_TSSGSubject_string_ctor8 - (0x004EC795 + sizeof(DWORD));
	*(LPBYTE )0x004EC799 = JMP_REL8;
	*(LPBYTE )0x004EC79A = 0x004EC7C7 - (0x004EC79A + sizeof(BYTE));

	*(LPBYTE )0x004EC86D = CALL_REL32;
	*(LPDWORD)0x004EC86E = (DWORD)Caller_TSSGSubject_string_ctor9 - (0x004EC86E + sizeof(DWORD));
	*(LPBYTE )0x004EC872 = JMP_REL8;
	*(LPBYTE )0x004EC873 = 0x004EC8A0 - (0x004EC873 + sizeof(BYTE));

	*(LPBYTE )0x004EC9A5 = CALL_REL32;
	*(LPDWORD)0x004EC9A6 = (DWORD)Caller_TSSGSubject_string_ctor10 - (0x004EC9A6 + sizeof(DWORD));
	*(LPBYTE )0x004EC9AA = JMP_REL8;
	*(LPBYTE )0x004EC9AB = 0x004EC9D8 - (0x004EC9AB + sizeof(BYTE));

	*(LPBYTE )0x004ECADF = CALL_REL32;
	*(LPDWORD)0x004ECAE0 = (DWORD)Caller_TSSGSubject_string_ctor11 - (0x004ECAE0 + sizeof(DWORD));
	*(LPBYTE )0x004ECAE4 = JMP_REL8;
	*(LPBYTE )0x004ECAE5 = 0x004ECB12 - (0x004ECAE5 + sizeof(BYTE));

	*(LPBYTE )0x004ECBF6 = CALL_REL32;
	*(LPDWORD)0x004ECBF7 = (DWORD)Caller_TSSGSubject_string_ctor12 - (0x004ECBF7 + sizeof(DWORD));
	*(LPBYTE )0x004ECBFB = JMP_REL8;
	*(LPBYTE )0x004ECBFC = 0x004ECC29 - (0x004ECBFC + sizeof(BYTE));

	*(LPBYTE )0x004ECCEC = CALL_REL32;
	*(LPDWORD)0x004ECCED = (DWORD)Caller_TSSGSubject_string_ctor13 - (0x004ECCED + sizeof(DWORD));
	*(LPBYTE )0x004ECCF1 = JMP_REL8;
	*(LPBYTE )0x004ECCF2 = 0x004ECD1F - (0x004ECCF2 + sizeof(BYTE));

	*(LPBYTE )0x004ECE0A = CALL_REL32;
	*(LPDWORD)0x004ECE0B = (DWORD)Caller_TSSGSubject_string_ctor14 - (0x004ECE0B + sizeof(DWORD));
	*(LPBYTE )0x004ECE0F = JMP_REL8;
	*(LPBYTE )0x004ECE10 = 0x004ECE3D - (0x004ECE10 + sizeof(BYTE));

	*(LPBYTE )0x004ECF42 = CALL_REL32;
	*(LPDWORD)0x004ECF43 = (DWORD)Caller_TSSGSubject_string_ctor15 - (0x004ECF43 + sizeof(DWORD));
	*(LPBYTE )0x004ECF47 = JMP_REL8;
	*(LPBYTE )0x004ECF48 = 0x004ECF75 - (0x004ECF48 + sizeof(BYTE));

	*(LPBYTE )0x004ED045 = CALL_REL32;
	*(LPDWORD)0x004ED046 = (DWORD)Caller_TSSGSubject_string_ctor16 - (0x004ED046 + sizeof(DWORD));
	*(LPBYTE )0x004ED04A = JMP_REL8;
	*(LPBYTE )0x004ED04B = 0x004ED078 - (0x004ED04B + sizeof(BYTE));

	*(LPBYTE )0x004ED111 = CALL_REL32;
	*(LPDWORD)0x004ED112 = (DWORD)Caller_TSSGSubject_string_ctor17 - (0x004ED112 + sizeof(DWORD));
	*(LPBYTE )0x004ED116 = JMP_REL8;
	*(LPBYTE )0x004ED117 = 0x004ED144 - (0x004ED117 + sizeof(BYTE));

	*(LPBYTE )0x004ED1F3 = CALL_REL32;
	*(LPDWORD)0x004ED1F4 = (DWORD)Caller_TSSGSubject_string_ctor18 - (0x004ED1F4 + sizeof(DWORD));
	*(LPBYTE )0x004ED1F8 = JMP_REL8;
	*(LPBYTE )0x004ED1F9 = 0x004ED226 - (0x004ED1F9 + sizeof(BYTE));

	*(LPBYTE )0x004ED2E9 = CALL_REL32;
	*(LPDWORD)0x004ED2EA = (DWORD)Caller_TSSGSubject_string_ctor19 - (0x004ED2EA + sizeof(DWORD));
	*(LPBYTE )0x004ED2EE = JMP_REL8;
	*(LPBYTE )0x004ED2EF = 0x004ED31C - (0x004ED2EF + sizeof(BYTE));

	*(LPBYTE )0x004ED407 = CALL_REL32;
	*(LPDWORD)0x004ED408 = (DWORD)Caller_TSSGSubject_string_ctor20 - (0x004ED408 + sizeof(DWORD));
	*(LPBYTE )0x004ED40C = JMP_REL8;
	*(LPBYTE )0x004ED40D = 0x004ED43A - (0x004ED40D + sizeof(BYTE));

	*(LPBYTE )0x004FD4DA = CALL_REL32;
	*(LPDWORD)0x004FD4DB = (DWORD)Caller_TSSGSubject_string_ctor21 - (0x004FD4DB + sizeof(DWORD));
	*(LPBYTE )0x004FD4DF = JMP_REL8;
	*(LPBYTE )0x004FD4E0 = 0x004FD4FD - (0x004FD4E0 + sizeof(BYTE));

	*(LPBYTE )0x00501C66 = CALL_REL32;
	*(LPDWORD)0x00501C67 = (DWORD)Caller_TSSGSubject_string_ctor22 - (0x00501C67 + sizeof(DWORD));
	*(LPBYTE )0x00501C6B = JMP_REL8;
	*(LPBYTE )0x00501C6C = 0x00501C89 - (0x00501C6C + sizeof(BYTE));

	// TSSGSubject::Setting
	*(LPBYTE )0x0046CBCE = JMP_REL32;
	*(LPDWORD)0x0046CBCF = (DWORD)TSSGSubject_Setting_SetSubjectName - (0x0046CBCF + sizeof(DWORD));
	*(LPBYTE )0x0046CBD3 = NOP;

	// TFindNameForm::EnumSubjectNameFind
	*(LPBYTE )0x00485206 = JMP_REL32;
	*(LPDWORD)0x00485207 = (DWORD)TFindNameForm_EnumSubjectNameFind_GetName - (0x00485207 + sizeof(DWORD));
	*(LPBYTE )0x0048520B = NOP;

	// TSearchForm::Init
	*(LPBYTE )0x00491CB1 = CALL_REL32;
	*(LPDWORD)0x00491CB2 = (DWORD)TSearchForm_Init_GetName - (0x00491CB2 + sizeof(DWORD));
	*(LPBYTE )0x00491CB6 = NOP;

	*(LPBYTE )0x00491D9B = CALL_REL32;
	*(LPDWORD)0x00491D9C = (DWORD)TSearchForm_Init_GetName - (0x00491D9C + sizeof(DWORD));
	*(LPBYTE )0x00491DA0 = PUSH_EAX;
	*(LPBYTE )0x00491DA1 = NOP;

	// TSearchForm::DGridSelectCell
	*(LPBYTE )0x0049C21B = JMP_REL32;
	*(LPDWORD)0x0049C21C = (DWORD)TSearchForm_DGridSelectCell_GetName - (0x0049C21C + sizeof(DWORD));
	*(LPBYTE )0x0049C220 = NOP;

	// TSSGCtrl::ReadSSG
	*(LPDWORD)(0x004E44D7 + 1) = (DWORD)TSSGCtrl_ReadSSG_ctor - (0x004E44D7 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004E5D3C = JMP_REL32;
	*(LPDWORD)0x004E5D3D = (DWORD)TSSGCtrl_EnumReadSSG_SetCodeAndName - (0x004E5D3D + sizeof(DWORD));
	*(LPBYTE )0x004E5D41 = NOP;

	*(LPBYTE )0x004EAE40 = JMP_REL32;
	*(LPDWORD)0x004EAE41 = (DWORD)TSSGCtrl_EnumReadSSG_SetCode - (0x004EAE41 + sizeof(DWORD));
	*(LPBYTE )0x004EAE45 = NOP;

	*(LPBYTE )0x004EAF62 = JMP_REL32;
	*(LPDWORD)0x004EAF63 = (DWORD)TSSGCtrl_EnumReadSSG_SetName - (0x004EAF63 + sizeof(DWORD));
	*(LPBYTE )0x004EAF67 = NOP;

	// TSSGCtrl::MakeADJFile
	*(LPBYTE )0x00503159 = JMP_REL32;
	*(LPDWORD)0x0050315A = (DWORD)TSSGCtrl_MakeADJFile_GetCode - (0x0050315A + sizeof(DWORD));
	*(LPDWORD)0x0050315E = NOP_X4;
	*(LPBYTE )0x00503162 = NOP;

	// TSSGSubject::GetSubjectName
	*(LPBYTE )0x0052CF86 = CALL_REL32;
	*(LPDWORD)0x0052CF87 = (DWORD)TSSGSubject_GetSubjectName_GetSubjectName - (0x0052CF87 + sizeof(DWORD));

	// TSSBitList::Setting
	*(LPBYTE )0x004B8296 = CALL_REL32;
	*(LPDWORD)0x004B8297 = (DWORD)TSSBitList_Setting_GetCode - (0x004B8297 + sizeof(DWORD));
	*(LPBYTE )0x004B829B = PUSH_EAX;

	*(LPBYTE )0x004B8EC9 = JMP_REL32;
	*(LPDWORD)0x004B8ECA = (DWORD)TSSBitList_Setting_SetSubjectName - (0x004B8ECA + sizeof(DWORD));
	*(LPBYTE )0x004B8ECE = NOP;

	*(LPDWORD)(0x004B8D68 + 1) = (DWORD)TSSBitList_Setting_GetName - (0x004B8D68 + 1 + sizeof(DWORD));

	// TSSBundleCalc::Setting
	*(LPBYTE )0x004BC94D = CALL_REL32;
	*(LPDWORD)0x004BC94E = (DWORD)TSSBundleCalc_Setting_GetCode - (0x004BC94E + sizeof(DWORD));

	*(LPDWORD)(0x004BD294 + 1) = (DWORD)TSSBundleCalc_Setting_GetName - (0x004BD294 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004BD3FD = JMP_REL32;
	*(LPDWORD)0x004BD3FE = (DWORD)TSSBundleCalc_Setting_SetSubjectName - (0x004BD3FE + sizeof(DWORD));
	*(LPDWORD)0x004BD402 = NOP_X4;

	// TSSBundleList::Setting
	*(LPBYTE )0x004BEAF1 = CALL_REL32;
	*(LPDWORD)0x004BEAF2 = (DWORD)TSSBundleList_Setting_GetCode - (0x004BEAF2 + sizeof(DWORD));

	*(LPDWORD)(0x004BF03F + 1) = (DWORD)TSSBundleList_Setting_GetName - (0x004BF03F + 1 + sizeof(DWORD));

	*(LPBYTE )0x004BF1A8 = JMP_REL32;
	*(LPDWORD)0x004BF1A9 = (DWORD)TSSBundleList_Setting_SetSubjectName - (0x004BF1A9 + sizeof(DWORD));
	*(LPDWORD)0x004BF1AD = NOP_X4;

	// TSSBundleToggle::Setting
	*(LPBYTE )0x004BF9C9 = CALL_REL32;
	*(LPDWORD)0x004BF9CA = (DWORD)TSSBundleToggle_Setting_GetCode - (0x004BF9CA + sizeof(DWORD));

	*(LPDWORD)(0x004BFDC6 + 1) = (DWORD)TSSBundleToggle_Setting_GetName - (0x004BFDC6 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004BFF2C = JMP_REL32;
	*(LPDWORD)0x004BFF2D = (DWORD)TSSBundleToggle_Setting_SetSubjectName - (0x004BFF2D + sizeof(DWORD));
	*(LPDWORD)0x004BFF31 = NOP_X4;

	// TSSCalc::Setting
	*(LPBYTE )0x004C1501 = CALL_REL32;
	*(LPDWORD)0x004C1502 = (DWORD)TSSCalc_Setting_GetCode - (0x004C1502 + sizeof(DWORD));

	*(LPDWORD)(0x004C1A1C + 1) = (DWORD)TSSCalc_Setting_GetName - (0x004C1A1C + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C1B82 = JMP_REL32;
	*(LPDWORD)0x004C1B83 = (DWORD)TSSCalc_Setting_SetSubjectName - (0x004C1B83 + sizeof(DWORD));
	*(LPDWORD)0x004C1B87 = NOP_X4;

	// TSSCopy::Setting
	*(LPBYTE )0x004C2455 = CALL_REL32;
	*(LPDWORD)0x004C2456 = (DWORD)TSSCopy_Setting_GetCode - (0x004C2456 + sizeof(DWORD));

	*(LPDWORD)(0x004C27CE + 1) = (DWORD)TSSCopy_Setting_GetName - (0x004C27CE + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C2930 = JMP_REL32;
	*(LPDWORD)0x004C2931 = (DWORD)TSSCopy_Setting_SetSubjectName - (0x004C2931 + sizeof(DWORD));
	*(LPDWORD)0x004C2935 = NOP_X4;

	// TSSDoubleList::Setting
	*(LPBYTE )0x004C366E = CALL_REL32;
	*(LPDWORD)0x004C366F = (DWORD)TSSDoubleList_Setting_GetCode - (0x004C366F + sizeof(DWORD));
	*(LPBYTE )0x004C3673 = PUSH_EAX;

	*(LPDWORD)(0x004C3E69 + 1) = (DWORD)TSSDoubleList_Setting_GetName - (0x004C3E69 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C3FCA = JMP_REL32;
	*(LPDWORD)0x004C3FCB = (DWORD)TSSDoubleList_Setting_SetSubjectName - (0x004C3FCB + sizeof(DWORD));
	*(LPDWORD)0x004C3FCF = NOP_X4;

	// TSSDoubleToggle::Setting
	*(LPBYTE )0x004C7B6A = CALL_REL32;
	*(LPDWORD)0x004C7B6B = (DWORD)TSSDoubleToggle_Setting_GetCode - (0x004C7B6B + sizeof(DWORD));
	*(LPBYTE )0x004C7B6F = PUSH_EAX;

	*(LPDWORD)(0x004C82C0 + 1) = (DWORD)TSSDoubleToggle_Setting_GetName - (0x004C82C0 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C8421 = JMP_REL32;
	*(LPDWORD)0x004C8422 = (DWORD)TSSDoubleToggle_Setting_SetSubjectName - (0x004C8422 + sizeof(DWORD));
	*(LPDWORD)0x004C8426 = NOP_X4;

	// TSSFloatCalc::Setting
	*(LPBYTE )0x004CDAF5 = CALL_REL32;
	*(LPDWORD)0x004CDAF6 = (DWORD)TSSFloatCalc_Setting_GetCode - (0x004CDAF6 + sizeof(DWORD));

	*(LPDWORD)(0x004CDF3B + 1) = (DWORD)TSSFloatCalc_Setting_GetName - (0x004CDF3B + 1 + sizeof(DWORD));

	*(LPBYTE )0x004CE0A1 = JMP_REL32;
	*(LPDWORD)0x004CE0A2 = (DWORD)TSSFloatCalc_Setting_SetSubjectName - (0x004CE0A2 + sizeof(DWORD));
	*(LPDWORD)0x004CE0A6 = NOP_X4;

	// TSSList::Setting
	*(LPBYTE )0x0052992D = CALL_REL32;
	*(LPDWORD)0x0052992E = (DWORD)TSSList_Setting_GetCode - (0x0052992E + sizeof(DWORD));

	*(LPDWORD)(0x00529D62 + 1) = (DWORD)TSSList_Setting_GetName - (0x00529D62 + 1 + sizeof(DWORD));

	*(LPBYTE )0x00529ECB = JMP_REL32;
	*(LPDWORD)0x00529ECC = (DWORD)TSSList_Setting_SetSubjectName - (0x00529ECC + sizeof(DWORD));
	*(LPDWORD)0x00529ED0 = NOP_X4;

	// TSSString::Setting
	*(LPBYTE )0x0052A6ED = CALL_REL32;
	*(LPDWORD)0x0052A6EE = (DWORD)TSSString_Setting_GetCode - (0x0052A6EE + sizeof(DWORD));

	*(LPDWORD)(0x0052AB89 + 1) = (DWORD)TSSString_Setting_GetName - (0x0052AB89 + 1 + sizeof(DWORD));

	*(LPBYTE )0x0052ACF2 = JMP_REL32;
	*(LPDWORD)0x0052ACF3 = (DWORD)TSSString_Setting_SetSubjectName - (0x0052ACF3 + sizeof(DWORD));
	*(LPDWORD)0x0052ACF7 = NOP_X4;

	// TSSToggle::Setting
	*(LPBYTE )0x0052BAF1 = CALL_REL32;
	*(LPDWORD)0x0052BAF2 = (DWORD)TSSToggle_Setting_GetCode - (0x0052BAF2 + sizeof(DWORD));

	*(LPDWORD)(0x0052C00F + 1) = (DWORD)TSSToggle_Setting_GetName - (0x0052C00F + 1 + sizeof(DWORD));

	*(LPBYTE )0x0052C178 = JMP_REL32;
	*(LPDWORD)0x0052C179 = (DWORD)TSSToggle_Setting_SetSubjectName - (0x0052C179 + sizeof(DWORD));
	*(LPDWORD)0x0052C17D = NOP_X4;

	// TSSTrace::Setting
	*(LPBYTE )0x0052CB5E = CALL_REL32;
	*(LPDWORD)0x0052CB5F = (DWORD)TSSTrace_Setting_GetCode - (0x0052CB5F + sizeof(DWORD));
	*(LPBYTE )0x0052CB63 = NOP;

	*(LPDWORD)(0x0052CC08 + 1) = (DWORD)TSSTrace_Setting_GetName - (0x0052CC08 + 1 + sizeof(DWORD));

	*(LPBYTE )0x0052CD51 = JMP_REL32;
	*(LPDWORD)0x0052CD52 = (DWORD)TSSTrace_Setting_SetSubjectName - (0x0052CD52 + sizeof(DWORD));
	*(LPDWORD)0x0052CD56 = NOP_X4;

	// TSSBundleFloatCalc::Setting
	*(LPBYTE )0x0052D36D = CALL_REL32;
	*(LPDWORD)0x0052D36E = (DWORD)TSSBundleFloatCalc_Setting_GetCode - (0x0052D36E + sizeof(DWORD));

	*(LPDWORD)(0x0052D8CB + 1) = (DWORD)TSSBundleFloatCalc_Setting_GetName - (0x0052D8CB + 1 + sizeof(DWORD));

	*(LPBYTE )0x0052DA31 = JMP_REL32;
	*(LPDWORD)0x0052DA32 = (DWORD)TSSBundleFloatCalc_Setting_SetSubjectName - (0x0052DA32 + sizeof(DWORD));
	*(LPDWORD)0x0052DA36 = NOP_X4;

	// TSSSplit::Setting
	*(LPBYTE )0x0052FF71 = CALL_REL32;
	*(LPDWORD)0x0052FF72 = (DWORD)TSSSplit_Setting_GetCode - (0x0052FF72 + sizeof(DWORD));

	*(LPBYTE )0x0053056E = JMP_REL32;
	*(LPDWORD)0x0053056F = (DWORD)TSSSplit_Setting_SetSubjectName - (0x0053056F + sizeof(DWORD));
	*(LPBYTE )0x00530573 = NOP;
}
