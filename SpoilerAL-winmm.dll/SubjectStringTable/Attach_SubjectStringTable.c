#include <windows.h>
#include "intrinsic.h"
#include "SubjectStringOperator.h"
#include "TSSGSubject.h"

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

void __cdecl    TMainForm_SubjectAccess_TSSToggle_GetNowValHeadStr();
void __cdecl    TMainForm_SubjectAccess_TSSString_GetNowValHeadStr();
void __cdecl    TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr();
void __cdecl    TMainForm_SetCalcNowValue_TSSCalc_GetNowValHeadStr();
#define         TMainForm_SetCalcNowValue_TSSCalc_GetNowValFootStr          TMainForm_SetCalcNowValue_TSSCalc_GetNowValHeadStr
void __cdecl    TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValHeadStr();
#define         TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValFootStr     TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValHeadStr
void __cdecl    TMainForm_DrawTreeCell_GetStrParam();
void __cdecl    TFindNameForm_EnumSubjectNameFind_GetName();
static __declspec(naked) __fastcall TFindNameForm_EnumSubjectNameFind_GetSubjectName(
	TSSGSubject* SSGS, TSSGCtrl* SSGC, string* retVal, string* name) {// for debugger
	extern BOOL ExtensionTSSDir;
	__asm {
		cmp  dword ptr[ExtensionTSSDir], 0
		jne  GetSubjectName
		jmp  TFindNameForm_EnumSubjectNameFind_GetName

	GetSubjectName:
		mov  edx, ds:0x0064CE2C // &MainForm
		lea  edx, [edx + 0x0738]// ->ssgCtrl
		mov  dword ptr[edx + 0x54], 0// ssgActionListner
		push edx
		mov  ecx, [ebp - 0x0104]// SSGC
		push ecx
		push dword ptr[esp + 12]
		call TSSGSubject_GetSubjectName
		add  esp, 12
		mov  edx, ds:0x0064CE2C // &MainForm
		lea  ecx, [edx + 0x0688]// ->ssgActionListner
		lea  edx, [edx + 0x0738]// ->ssgCtrl
		mov  dword ptr[edx + 0x54], ecx// ssgActionListner
		ret
	}
}
#define         TSearchForm_Init_GetName                                    TFindNameForm_EnumSubjectNameFind_GetName
#define         TSearchForm_DGridSelectCell_GetName                         TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleCalc_Read_GetFileName                              TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleCalc_Write_GetFileName                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleList_Read_GetChainFileName                         TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleList_Write_GetChainFileName                        TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleToggle_Read_GetFileName                            TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleToggle_Write_GetFileName                           TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_Read_GetDataFileName                          TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_Write_GetDataFileName                         TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_ToByteCode_GetDataFileName                    TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleToggle_Read_GetDataFileName                        TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleToggle_Write_GetDataFileName                       TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSGCtrl_MakeADJFile_GetCode                                TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSFloatCalc_GetLockName_GetString                          TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSFloatCalc_GetAddressStr_GetString                        TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSCopy_GetLockName_GetString                               TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSCopy_GetAddressStr_GetString                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSTrace_GetLockName_GetString                              TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBitList_GetLockName_GetString                            TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBitList_GetAddressStr_GetString                          TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBitList_GetIndexFileName_GetString                       TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleToggle_GetLockName_GetString                       TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleToggle_GetAddressStr_GetString                     TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleToggle_GetIndexFileName_GetString                  TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSCalc_GetLockName_GetString                               TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSCalc_GetAddressStr_GetString                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSList_GetLockName_GetString                               TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSList_GetAddressStr_GetString                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSList_GetIndexFileName_GetString                          TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSToggle_GetLockName_GetString                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSToggle_GetAddressStr_GetString                           TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_GetLockName_GetString                         TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_GetAddressStr_GetString                       TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSDoubleList_GetIndexFileName_GetString                    TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSString_GetLockName_GetString                             TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSString_GetAddressStr_GetString                           TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSToggle_Read_GetOnCode2                                   TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSToggle_Write_GetOnCode                                   TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSToggle_ToByteCode_GetOnOffCode                           TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleFloatCalc_Read_GetFileName                         TFindNameForm_EnumSubjectNameFind_GetName
#define         TSSBundleFloatCalc_Write_GetFileName                        TFindNameForm_EnumSubjectNameFind_GetName
void __cdecl    TSSBitList_Setting_GetCode();
#define         TSSBundleCalc_Setting_GetCode                               TSSBitList_Setting_GetCode
#define         TSSBundleList_Setting_GetCode                               TSSBitList_Setting_GetCode
#define         TSSBundleToggle_Setting_GetCode                             TSSBitList_Setting_GetCode
#define         TSSCalc_Setting_GetCode                                     TSSBitList_Setting_GetCode
#define         TSSCopy_Setting_GetCode                                     TSSBitList_Setting_GetCode
#define         TSSDoubleList_Setting_GetCode                               TSSBitList_Setting_GetCode
#define         TSSDoubleToggle_Setting_GetCode                             TSSBitList_Setting_GetCode
#define         TSSFloatCalc_Setting_GetCode                                TSSBitList_Setting_GetCode
#define         TSSList_Setting_GetCode                                     TSSBitList_Setting_GetCode
#define         TSSString_Setting_GetCode                                   TSSBitList_Setting_GetCode
#define         TSSToggle_Setting_GetCode                                   TSSBitList_Setting_GetCode
#define         TSSBundleFloatCalc_Setting_GetCode                          TSSBitList_Setting_GetCode
#define         TSSSplit_Setting_GetCode                                    TSSBitList_Setting_GetCode
#define         TSSBitList_Setting_SetAddressStr                            SubjectStringTable_SetString
#define         TSSBitList_Setting_SetIndexFileName                         SubjectStringTable_SetString
#define         TSSBitList_Setting_SetSubjectName                           SubjectStringTable_SetString
#define         TSSBitList_Setting_SetLockStr                               SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetAddressStr                         SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetFileName                           SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetSubjectName                        SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetNowValHeadStr                      SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetLockStr                            SubjectStringTable_SetString
#define         TSSBundleCalc_Setting_SetNowValFootStr                      SubjectStringTable_SetString
#define         TSSBundleList_Setting_SetAddressStr                         SubjectStringTable_SetString
#define         TSSBundleList_Setting_SetIndexFileName                      SubjectStringTable_SetString
#define         TSSBundleList_Setting_SetChainFileName                      SubjectStringTable_SetString
#define         TSSBundleList_Setting_SetSubjectName                        SubjectStringTable_SetString
#define         TSSBundleList_Setting_SetLockStr                            SubjectStringTable_SetString
#define         TSSBundleToggle_Setting_SetAddressStr                       SubjectStringTable_SetString
#define         TSSBundleToggle_Setting_SetFileName                         SubjectStringTable_SetString
#define         TSSBundleToggle_Setting_SetSubjectName                      SubjectStringTable_SetString
#define         TSSBundleToggle_Setting_SetNowValHeadStr                    SubjectStringTable_SetString
#define         TSSBundleToggle_Setting_SetLockStr                          SubjectStringTable_SetString
#define         TSSCalc_Setting_SetAddressStr                               SubjectStringTable_SetString
#define         TSSCalc_Setting_SetSubjectName                              SubjectStringTable_SetString
#define         TSSCalc_Setting_SetNowValHeadStr                            SubjectStringTable_SetString
#define         TSSCalc_Setting_SetLockStr                                  SubjectStringTable_SetString
#define         TSSCalc_Setting_SetNowValFootStr                            SubjectStringTable_SetString
#define         TSSCopy_Setting_SetAddressStr                               SubjectStringTable_SetString
#define         TSSCopy_Setting_SetSrcAddressStr                            SubjectStringTable_SetString
#define         TSSCopy_Setting_SetSubjectName                              SubjectStringTable_SetString
#define         TSSCopy_Setting_SetLockStr                                  SubjectStringTable_SetString
#define         TSSDoubleList_Setting_SetAddressStr                         SubjectStringTable_SetString
#define         TSSDoubleList_Setting_SetIndexFileName                      SubjectStringTable_SetString
#define         TSSDoubleList_Setting_SetDataFileName                       SubjectStringTable_SetString
#define         TSSDoubleList_Setting_SetSubjectName                        SubjectStringTable_SetString
#define         TSSDoubleList_Setting_SetLockStr                            SubjectStringTable_SetString
#define         TSSDoubleToggle_Setting_SetAddressStr                       SubjectStringTable_SetString
#define         TSSDoubleToggle_Setting_SetIndexFileName                    SubjectStringTable_SetString
#define         TSSDoubleToggle_Setting_SetDataFileName                     SubjectStringTable_SetString
#define         TSSDoubleToggle_Setting_SetSubjectName                      SubjectStringTable_SetString
#define         TSSDoubleToggle_Setting_SetLockStr                          SubjectStringTable_SetString
#define         TSSFloatCalc_Setting_SetAddressStr                          SubjectStringTable_SetString
#define         TSSFloatCalc_Setting_SetSubjectName                         SubjectStringTable_SetString
#define         TSSFloatCalc_Setting_SetNowValHeadStr                       SubjectStringTable_SetString
#define         TSSFloatCalc_Setting_SetLockStr                             SubjectStringTable_SetString
#define         TSSFloatCalc_Setting_SetNowValFootStr                       SubjectStringTable_SetString
#define         TSSList_Setting_SetAddressStr                               SubjectStringTable_SetString
#define         TSSList_Setting_SetIndexFileName                            SubjectStringTable_SetString
#define         TSSList_Setting_SetSubjectName                              SubjectStringTable_SetString
#define         TSSList_Setting_SetLockStr                                  SubjectStringTable_SetString
#define         TSSString_Setting_SetAddressStr                             SubjectStringTable_SetString
#define         TSSString_Setting_SetSubjectName                            SubjectStringTable_SetString
#define         TSSString_Setting_SetNowValHeadStr                          SubjectStringTable_SetString
#define         TSSString_Setting_SetLockStr                                SubjectStringTable_SetString
#define         TSSToggle_Setting_SetAddressStr                             SubjectStringTable_SetString
#define         TSSToggle_Setting_SetOnCode                                 SubjectStringTable_SetString
#define         TSSToggle_Setting_SetOffCode                                SubjectStringTable_SetString
#define         TSSToggle_Setting_SetSubjectName                            SubjectStringTable_SetString
#define         TSSToggle_Setting_SetNowValHeadStr                          SubjectStringTable_SetString
#define         TSSToggle_Setting_SetLockStr                                SubjectStringTable_SetString
#define         TSSTrace_Setting_SetSubjectName                             SubjectStringTable_SetString
#define         TSSTrace_Setting_SetLockStr                                 SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetAddressStr                    SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetFileName                      SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetSubjectName                   SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetNowValHeadStr                 SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetLockStr                       SubjectStringTable_SetString
#define         TSSBundleFloatCalc_Setting_SetNowValFootStr                 SubjectStringTable_SetString
#define         TSSSplit_Setting_SetStrParam                                SubjectStringTable_SetString
void __cdecl    TSSBitList_Setting_GetName();
#define         TSSBundleFloatCalc_Setting_GetName                          TSSBitList_Setting_GetName
#define         TSSBundleCalc_Setting_GetName                               TSSBitList_Setting_GetName
#define         TSSBundleList_Setting_GetName                               TSSBitList_Setting_GetName
#define         TSSBundleToggle_Setting_GetName                             TSSBitList_Setting_GetName
#define         TSSCalc_Setting_GetName                                     TSSBitList_Setting_GetName
#define         TSSCopy_Setting_GetName                                     TSSBitList_Setting_GetName
#define         TSSDoubleList_Setting_GetName                               TSSBitList_Setting_GetName
#define         TSSDoubleToggle_Setting_GetName                             TSSBitList_Setting_GetName
#define         TSSFloatCalc_Setting_GetName                                TSSBitList_Setting_GetName
#define         TSSList_Setting_GetName                                     TSSBitList_Setting_GetName
#define         TSSString_Setting_GetName                                   TSSBitList_Setting_GetName
#define         TSSToggle_Setting_GetName                                   TSSBitList_Setting_GetName
#define         TSSTrace_Setting_GetName                                    TSSBitList_Setting_GetName
#define         TSSBundleFloatCalc_Setting_GetName                          TSSBitList_Setting_GetName
void __cdecl    TSSBitList_Read_GetAddressStr();
#define         TSSBitList_Write_GetAddressStr                              TSSBitList_Read_GetAddressStr
#define         TSSBundleCalc_Read_GetAddressStr                            TSSBitList_Read_GetAddressStr
#define         TSSBundleCalc_Write_GetAddressStr                           TSSBitList_Read_GetAddressStr
#define         TSSBundleList_Read_GetAddressStr                            TSSBitList_Read_GetAddressStr
#define         TSSBundleList_Write_GetAddressStr                           TSSBitList_Read_GetAddressStr
#define         TSSBundleToggle_Read_GetAddressStr                          TSSBitList_Read_GetAddressStr
#define         TSSBundleToggle_Write_GetAddressStr                         TSSBitList_Read_GetAddressStr
#define         TSSCalc_Read_GetAddressStr                                  TSSBitList_Read_GetAddressStr
#define         TSSCalc_Write_GetAddressStr                                 TSSBitList_Read_GetAddressStr
#define         TSSCopy_Write_GetAddressStr                                 TSSBitList_Read_GetAddressStr
#define         TSSCopy_Write_GetSrcAddressStr                              TSSBitList_Read_GetAddressStr
#define         TSSDoubleList_Read_GetAddressStr                            TSSBitList_Read_GetAddressStr
#define         TSSDoubleList_Write_GetAddressStr                           TSSBitList_Read_GetAddressStr
#define         TSSDoubleToggle_Read_GetAddressStr                          TSSBitList_Read_GetAddressStr
#define         TSSDoubleToggle_Write_GetAddressStr                         TSSBitList_Read_GetAddressStr
#define         TSSDoubleList_ToByteCode_GetAddressStr                      TSSBitList_Read_GetAddressStr
#define         TSSFloatCalc_Read_GetAddressStr                             TSSBitList_Read_GetAddressStr
#define         TSSFloatCalc_Write_GetAddressStr                            TSSBitList_Read_GetAddressStr
#define         TSSList_Read_GetAddressStr                                  TSSBitList_Read_GetAddressStr
#define         TSSList_Write_GetAddressStr                                 TSSBitList_Read_GetAddressStr
#define         TSSString_Read_GetAddressStr                                TSSBitList_Read_GetAddressStr
#define         TSSString_Write_GetAddressStr                               TSSBitList_Read_GetAddressStr
#define         TSSToggle_Read_GetAddressStr                                TSSBitList_Read_GetAddressStr
#define         TSSToggle_Write_GetAddressStr                               TSSBitList_Read_GetAddressStr
#define         TSSBundleFloatCalc_Read_GetAddressStr                       TSSBitList_Read_GetAddressStr
#define         TSSBundleFloatCalc_Write_GetAddressStr                      TSSBitList_Read_GetAddressStr
void __cdecl    TSSGCtrl_ReadSSG_ctor();
void __cdecl    TSSGCtrl_EnumReadSSG_SetCodeAndName();
void __cdecl    TSSGCtrl_EnumReadSSG_SetCode();
void __cdecl    TSSGCtrl_EnumReadSSG_SetName();
void __cdecl    TSSGCtrl_MakeADJFile_GetAddressStr();
void __fastcall TSSString_Setting_SetEndWord(void *this, void *s);
void __cdecl    TSSString_Read_GetEndWord();
void __cdecl    TSSString_Write_GetEndWord();
void __cdecl    TSSToggle_Read_GetOnCode1();
void __cdecl    TSSToggle_Read_GetOffCode();
#define         TSSToggle_Write_GetOffCode                                  TSSToggle_Read_GetOffCode
void __cdecl    TSSTrace_Write_GetFileName();
void __cdecl    TSSGSubject_GetSubjectName_GetSubjectName();

#define OPCODE_NOP       (BYTE )0x90
#define OPCODE_NOP_X2    (WORD )0x9090
#define OPCODE_NOP_X4    (DWORD)0x90909090
#define OPCODE_CALL      (BYTE )0xE8
#define OPCODE_JMP_REL32 (BYTE )0xE9
#define OPCODE_JMP_REL8  (BYTE )0xEB

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

#define SET_REL8(addr, to) \
    *(LPDWORD)(addr) = (to) - ((addr) + sizeof(BYTE))

#define SET_REL32(addr, to) \
    *(LPDWORD)(addr) = (to) - ((addr) + sizeof(DWORD))

#define SET_PROC(addr, proc) SET_REL32((addr) + 1, (DWORD)proc)

#define CALL(addr, proc)            \
do                                  \
{                                   \
    *(LPBYTE)(addr) = OPCODE_CALL;  \
    SET_PROC(addr, proc);           \
} while (0)

#define JMP_REL8(addr, to)             \
do                                     \
{                                      \
    *(LPBYTE)(addr) = OPCODE_JMP_REL8; \
    SET_REL8((addr) + 1, to);          \
} while (0)

#define JMP_REL32(addr, to)             \
do                                      \
{                                       \
    *(LPBYTE)(addr) = OPCODE_JMP_REL32; \
    SET_REL32((addr) + 1, to);          \
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

static __inline void AttachConstructor()
{
	// TSSGCtrl::TSSGCtrl - new TSSDir() - name(), code(), subjectName()
	CALL     (0x004DA44C, Caller_TSSGSubject_string_ctor1);
	JMP_REL8 (0x004DA451, 0x004DA488);
	NPAD2    (0x004DA453);

	// TSSGCtrl::EnumReadSSG - new TSSCalc() - name(), code(), subjectName()
	CALL     (0x004EA7B8, Caller_TSSGSubject_string_ctor2);
	JMP_REL8 (0x004EA7BD, 0x004EA7F4);
	NPAD2    (0x004EA7BF);

	// TSSGCtrl::EnumReadSSG - new TSSCalc() - lockStr()
	SET_PROC (0x004EA818, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSCalc() - addressStr()
	SET_PROC (0x004EA82F, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSCalc() - nowValHeadStr()
	SET_PROC (0x004EA845, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSCalc() - nowValFootStr()
	SET_PROC (0x004EA85C, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - name(), code(), subjectName()
	CALL     (0x004EAA62, Caller_TSSGSubject_string_ctor3);
	JMP_REL8 (0x004EAA67, 0x004EAA9E);
	NPAD2    (0x004EAA69);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - lockStr()
	SET_PROC (0x004EAAC2, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - addressStr()
	SET_PROC (0x004EAAD6, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - onCode()
	SET_PROC (0x004EAAEC, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - offCode()
	SET_PROC (0x004EAB03, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSToggle() - nowValHeadStr()
	SET_PROC (0x004EAB1A, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSString() - name(), code(), subjectName()
	CALL     (0x004EAD2E, Caller_TSSGSubject_string_ctor4);
	JMP_REL8 (0x004EAD33, 0x004EAD6A);
	NPAD2    (0x004EAD35);

	// TSSGCtrl::EnumReadSSG - new TSSString() - lockStr()
	SET_PROC (0x004EAD8E, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSString() - addressStr()
	SET_PROC (0x004EADA5, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSString() - endWord()
	SET_PROC (0x004EADBB, SubjectStringTable_StringCtor);

	// TSSGCtrl::EnumReadSSG - new TSSString() - nowValHeadStr()
	SET_PROC (0x004EADD2, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCalc() - name(), code(), subjectName()
	CALL     (0x004EC49F, Caller_TSSGSubject_string_ctor5);
	JMP_REL8 (0x004EC4A4, 0x004EC4D2);

	// TSSGCtrl::MakeSubjectClass - new TSSCalc() - lockStr()
	SET_PROC (0x004EC4ED, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCalc() - addressStr()
	SET_PROC (0x004EC501, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCalc() - nowValHeadStr()
	SET_PROC (0x004EC514, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCalc() - nowValFootStr()
	SET_PROC (0x004EC528, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - name(), code(), subjectName()
	CALL     (0x004EC595, Caller_TSSGSubject_string_ctor6);
	JMP_REL8 (0x004EC59A, 0x004EC5C8);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - lockStr()
	SET_PROC (0x004EC5E3, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - addressStr()
	SET_PROC (0x004EC5F4, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - onCode()
	SET_PROC (0x004EC607, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - offCode()
	SET_PROC (0x004EC61B, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSToggle() - nowValHeadStr()
	SET_PROC (0x004EC62F, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSList() - name(), code(), subjectName()
	CALL     (0x004EC6A7, Caller_TSSGSubject_string_ctor7);
	JMP_REL8 (0x004EC6AC, 0x004EC6DA);

	// TSSGCtrl::MakeSubjectClass - new TSSList() - lockStr()
	SET_PROC (0x004EC702, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSList() - addressStr()
	SET_PROC (0x004EC716, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSList() - indexFileName()
	SET_PROC (0x004EC72A, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDir() - name(), code(), subjectName()
	CALL     (0x004EC794, Caller_TSSGSubject_string_ctor8);
	JMP_REL8 (0x004EC799, 0x004EC7C7);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList() - name(), code(), subjectName()
	CALL     (0x004EC86D, Caller_TSSGSubject_string_ctor9);
	JMP_REL8 (0x004EC872, 0x004EC8A0);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList() - lockStr()
	SET_PROC (0x004EC8C8, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList() - addressStr()
	SET_PROC (0x004EC90E, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList() - indexFileName()
	SET_PROC (0x004EC922, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleList() - dataFileName()
	SET_PROC (0x004EC935, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - name(), code(), subjectName()
	CALL     (0x004EC9A5, Caller_TSSGSubject_string_ctor10);
	JMP_REL8 (0x004EC9AA, 0x004EC9D8);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - lockStr()
	SET_PROC (0x004EC9F3, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - addressStr()
	SET_PROC (0x004ECA04, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - onCode()
	SET_PROC (0x004ECA17, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - offCode()
	SET_PROC (0x004ECA2B, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - nowValHeadStr()
	SET_PROC (0x004ECA3F, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleToggle() - fileName()
	SET_PROC (0x004ECA72, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList() - name(), code(), subjectName()
	CALL     (0x004ECADF, Caller_TSSGSubject_string_ctor11);
	JMP_REL8 (0x004ECAE4, 0x004ECB12);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList() - lockStr()
	SET_PROC (0x004ECB3A, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList() - addressStr()
	SET_PROC (0x004ECB4E, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList() - indexFileName()
	SET_PROC (0x004ECB62, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleList() - chainFileName()
	SET_PROC (0x004ECB89, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSString() - name(), code(), subjectName()
	CALL     (0x004ECBF6, Caller_TSSGSubject_string_ctor12);
	JMP_REL8 (0x004ECBFB, 0x004ECC29);

	// TSSGCtrl::MakeSubjectClass - new TSSString() - lockStr()
	SET_PROC (0x004ECC44, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSString() - addressStr()
	SET_PROC (0x004ECC58, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSString() - endWord()
	SET_PROC (0x004ECC6B, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSString() - nowValHeadStr()
	SET_PROC (0x004ECC7F, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - name(), code(), subjectName()
	CALL     (0x004ECCEC, Caller_TSSGSubject_string_ctor13);
	JMP_REL8 (0x004ECCF1, 0x004ECD1F);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - lockStr()
	SET_PROC (0x004ECD3A, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - addressStr()
	SET_PROC (0x004ECD4E, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - nowValHeadStr()
	SET_PROC (0x004ECD61, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - nowValFootStr()
	SET_PROC (0x004ECD75, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleCalc() - fileName()
	SET_PROC (0x004ECD9D, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle() - name(), code(), subjectName()
	CALL     (0x004ECE0A, Caller_TSSGSubject_string_ctor14);
	JMP_REL8 (0x004ECE0F, 0x004ECE3D);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle() - lockStr()
	SET_PROC (0x004ECE65, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle() - addressStr()
	SET_PROC (0x004ECEAB, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle() - indexFileName()
	SET_PROC (0x004ECEBF, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSDoubleToggle() - dataFileName()
	SET_PROC (0x004ECED2, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBitList() - name(), code(), subjectName()
	CALL     (0x004ECF42, Caller_TSSGSubject_string_ctor15);
	JMP_REL8 (0x004ECF47, 0x004ECF75);

	// TSSGCtrl::MakeSubjectClass - new TSSBitList() - lockStr()
	SET_PROC (0x004ECF9D, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBitList() - addressStr()
	SET_PROC (0x004ECFB1, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBitList() - indexFileName()
	SET_PROC (0x004ECFC5, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBitList() - realFileName()
	SET_PROC (0x004ECFD8, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSTrace() - name(), code(), subjectName()
	CALL     (0x004ED045, Caller_TSSGSubject_string_ctor16);
	JMP_REL8 (0x004ED04A, 0x004ED078);

	// TSSGCtrl::MakeSubjectClass - new TSSTrace() - lockStr()
	SET_PROC (0x004ED093, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSTrace() - fileName()
	SET_PROC (0x004ED0A4, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCopy() - name(), code(), subjectName()
	CALL     (0x004ED111, Caller_TSSGSubject_string_ctor17);
	JMP_REL8 (0x004ED116, 0x004ED144);

	// TSSGCtrl::MakeSubjectClass - new TSSCopy() - lockStr()
	SET_PROC (0x004ED15F, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCopy() - addressStr()
	SET_PROC (0x004ED173, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSCopy() - srcAddressStr()
	SET_PROC (0x004ED186, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc() - name(), code(), subjectName()
	CALL     (0x004ED1F3, Caller_TSSGSubject_string_ctor18);
	JMP_REL8 (0x004ED1F8, 0x004ED226);

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc() - lockStr()
	SET_PROC (0x004ED241, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc() - addressStr()
	SET_PROC (0x004ED255, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc() - nowValHeadStr()
	SET_PROC (0x004ED268, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSFloatCalc() - nowValFootStr()
	SET_PROC (0x004ED27C, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - name(), code(), subjectName()
	CALL     (0x004ED2E9, Caller_TSSGSubject_string_ctor19);
	JMP_REL8 (0x004ED2EE, 0x004ED31C);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - lockStr()
	SET_PROC (0x004ED337, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - addressStr()
	SET_PROC (0x004ED34B, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - nowValHeadStr()
	SET_PROC (0x004ED35E, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - nowValFootStr()
	SET_PROC (0x004ED372, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSBundleFloatCalc() - fileName()
	SET_PROC (0x004ED39A, SubjectStringTable_StringCtor);

	// TSSGCtrl::MakeSubjectClass - new TSSSplit() - name(), code(), subjectName()
	CALL     (0x004ED407, Caller_TSSGSubject_string_ctor20);
	JMP_REL8 (0x004ED40C, 0x004ED43A);

	// TSSGCtrl::MakeSubjectClass - new TSSSplit() - strParam()
	SET_PROC (0x004ED455, SubjectStringTable_StringCtor);

	// TSSGCtrl::ReadADJFile - TSSDir RootSubject; - name(), code(), subjectName()
	CALL     (0x004FD4DA, Caller_TSSGSubject_string_ctor21);
	JMP_REL8 (0x004FD4DF, 0x004FD4FD);

	// TSSGCtrl::LoopSSRFile - TSSGSubject SSGS; - name(), code(), subjectName()
	CALL     (0x00501C66, Caller_TSSGSubject_string_ctor22);
	JMP_REL8 (0x00501C6B, 0x00501C89);
}

static __inline void AttachVirtualFunction()
{
	// TSSFloatCalc::GetLockName
	SET_PROC (0x00523B2A, TSSFloatCalc_GetLockName_GetString);

	// TSSFloatCalc::GetAddressStr
	SET_PROC (0x00523B7D, TSSFloatCalc_GetAddressStr_GetString);

	// TSSCopy::GetLockName
	SET_PROC (0x00524042, TSSCopy_GetLockName_GetString);

	// TSSCopy::GetAddressStr
	SET_PROC (0x00524095, TSSCopy_GetAddressStr_GetString);

	// TSSTrace::GetLockName
	SET_PROC (0x00524272, TSSCopy_GetAddressStr_GetString);

	// TSSBitList::GetLockName
	SET_PROC (0x00524526, TSSBitList_GetLockName_GetString);

	// TSSBitList::GetAddressStr
	SET_PROC (0x00524579, TSSBitList_GetAddressStr_GetString);

	// TSSBitList::GetIndexFileName
	SET_PROC (0x0052471D, TSSBitList_GetIndexFileName_GetString);

	// TSSDoubleToggle::GetLockName
	SET_PROC (0x00524BCA, TSSDoubleToggle_GetLockName_GetString);

	// TSSDoubleToggle::GetAddressStr
	SET_PROC (0x00524C11, TSSDoubleToggle_GetAddressStr_GetString);

	// TSSDoubleToggle::GetIndexFileName
	SET_PROC (0x00524C59, TSSDoubleToggle_GetIndexFileName_GetString);

	// TSSCalc::GetLockName
	SET_PROC (0x00524F3E, TSSCalc_GetLockName_GetString);

	// TSSCalc::GetAddressStr
	SET_PROC (0x00524F91, TSSCalc_GetAddressStr_GetString);

	// TSSList::GetLockName
	SET_PROC (0x00525292, TSSList_GetLockName_GetString);

	// TSSList::GetAddressStr
	SET_PROC (0x005252E5, TSSList_GetAddressStr_GetString);

	// TSSList::GetIndexFileName
	SET_PROC (0x00525489, TSSList_GetIndexFileName_GetString);

	// TSSToggle::GetLockName
	SET_PROC (0x005257AA, TSSToggle_GetLockName_GetString);

	// TSSToggle::GetAddressStr
	SET_PROC (0x005257FE, TSSToggle_GetAddressStr_GetString);

	// TSSDoubleList::GetLockName
	SET_PROC (0x00525B12, TSSDoubleList_GetLockName_GetString);

	// TSSDoubleList::GetAddressStr
	SET_PROC (0x00525B65, TSSDoubleList_GetAddressStr_GetString);

	// TSSDoubleList::GetIndexFileName
	SET_PROC (0x00525BAD, TSSDoubleList_GetIndexFileName_GetString);

	// TSSString::GetLockName
	SET_PROC (0x005261C6, TSSString_GetLockName_GetString);

	// TSSString::GetAddressStr
	SET_PROC (0x00526219, TSSString_GetAddressStr_GetString);
}

static __inline void AttachOperator()
{
	// TMainForm::SubjectAccess
	SET_PROC (0x0043A25B, TMainForm_SubjectAccess_TSSToggle_GetNowValHeadStr);
	SET_PROC (0x0043A3B0, TMainForm_SubjectAccess_TSSToggle_GetNowValHeadStr);
	SET_PROC (0x0043A7F2, TMainForm_SubjectAccess_TSSString_GetNowValHeadStr);
	SET_PROC (0x0043AA0A, TMainForm_SubjectAccess_TSSString_GetNowValHeadStr);
	SET_PROC (0x0043AB40, TMainForm_SubjectAccess_TSSString_GetNowValHeadStr);

	// TMainForm::StringEnterBtnClick
	SET_PROC (0x0043EE30, TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr);
	SET_PROC (0x0043EFCA, TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr);
	SET_PROC (0x0043F0DF, TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr);

	// TMainForm::SetCalcNowValue
	SET_PROC (0x0043FD50, TMainForm_SetCalcNowValue_TSSCalc_GetNowValHeadStr);
	SET_PROC (0x0043FE39, TMainForm_SetCalcNowValue_TSSCalc_GetNowValFootStr);
	SET_PROC (0x00440449, TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValHeadStr);
	SET_PROC (0x00440532, TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValFootStr);

	// TMainForm::DrawTreeCell
	SET_PROC (0x00444FBB, TMainForm_DrawTreeCell_GetStrParam);
	SET_PROC (0x004451C3, TMainForm_DrawTreeCell_GetStrParam);

	// TSSGSubject::Setting
	/*
		mov     eax, dword ptr [ebx + 1CH]              ; 0046CBCE _ 8B. 43, 1C
		mov     dword ptr [ebx + 4CH], eax              ; 0046CBD1 _ 89. 43, 4C
		jmp     0046CBF8H                               ; 0046CBD4 _ EB, 22
	*/
	*(LPDWORD)0x0046CBCE = BSWAP32(0x8B431C89);
	*(LPDWORD)0x0046CBD2 = BSWAP32(0x434CEB22);

	// TFindNameForm::EnumSubjectNameFind
	SET_PROC (0x0048520E, TFindNameForm_EnumSubjectNameFind_GetSubjectName);

	// TSearchForm::Init
	SET_PROC (0x00491CBC, TSearchForm_Init_GetName);
	SET_PROC (0x00491DA6, TSearchForm_Init_GetName);

	// TSearchForm::DGridSelectCell
	SET_PROC (0x0049C22C, TSearchForm_DGridSelectCell_GetName);

	// TSSBitList::Setting
	SET_PROC (0x004B829C, TSSBitList_Setting_GetCode);

	/*
		lea     ecx, [ebx + 80H]                        ; 004B84A2 _ 8D. 8B, 00000080
	*/
	*(LPWORD )0x004B84A2 = BSWAP16(0x8D8B);
	*(LPDWORD)0x004B84A4 = 0x00000080;
	CALL     (0x004B84A8, TSSBitList_Setting_SetAddressStr);
	JMP_REL32(0x004B84AD, 0x004B859B);
	NPAD2    (0x004B84B2);

	/*
		lea     ecx, [ebx + 98H]                        ; 004B84F7 _ 8D. 8B, 00000098
	*/
	*(LPWORD )0x004B84F7 = BSWAP16(0x8D8B);
	*(LPDWORD)0x004B84F9 = 0x00000098;
	CALL     (0x004B84FD, TSSBitList_Setting_SetIndexFileName);
	JMP_REL32(0x004B8502, 0x004B8D22);
	NPAD2    (0x004B8507);

	/*
		mov     ecx, dword ptr [ebp - 18H]              ; 004B85EB _ 8B. 4D, E8
		add     ecx, 24                                 ; 004B85EE _ 83. C1, 18
	*/
	*(LPDWORD)0x004B85EB = BSWAP32(0x8B4DE883);
	*(LPWORD )0x004B85EF = BSWAP16(0xC118);

	SET_REL32(0x004B8629, 0x004B84F1);
	SET_REL32(0x004B86EB, 0x004B84F1);

	/*
		mov     edx, dword ptr [ebp - 18H]              ; 004B8752 _ 8B. 55, E8
		add     edx, 24                                 ; 004B8755 _ 83. C2, 18
	*/
	*(LPWORD )0x004B8752 = BSWAP16(0x8B55);
	*(LPDWORD)0x004B8754 = BSWAP32(0xE883C218);

	/*
		mov     eax, dword ptr [ebp - 18H]              ; 004B8A49 _ 8B. 45, E8
		add     eax, 24                                 ; 004B8A4C _ 83. C0, 18
	*/
	*(LPDWORD)0x004B8A49 = BSWAP32(0x8B45E883);
	*(LPWORD )0x004B8A4D = BSWAP16(0xC018);

	/*
		lea     edx, [ebp - 0F4H]                       ; 004B8A6B _ 8D. 95, FFFFFF0C
		lea     ecx, [ebx + 98H]                        ; 004B8A71 _ 8D. 8B, 00000098
	*/
	*(LPBYTE )0x004B8A6C = 0x95;
	*(LPBYTE )0x004B8A72 = 0x8B;
	CALL     (0x004B8A77, TSSBitList_Setting_SetIndexFileName);
	JMP_REL8 (0x004B8A7C, 0x004B8AD5);
	NPAD5    (0x004B8A7E);

	SET_PROC (0x004B8D68, TSSBitList_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 004B8EC9 _ 8D. 4B, 44
	*/
	*(LPDWORD)0x004B8EC9 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004B8ECC, TSSBitList_Setting_SetSubjectName);
	JMP_REL8 (0x004B8ED1, 0x004B8F15);
	NPAD5    (0x004B8ED3);

	/*
		lea     ecx, [ebx + 60H]                        ; 004B8F1B _ 8D. 4B, 60
	*/
	*(LPDWORD)0x004B8F1B = BSWAP32(0x8D4B60E8);
	SET_PROC (0x004B8F1E, TSSBitList_Setting_SetLockStr);
	JMP_REL8 (0x004B8F23, 0x004B8F67);
	NPAD5    (0x004B8F25);

	// TSSBitList::Read
	SET_PROC (0x004B90EB, TSSBitList_Read_GetAddressStr);

	// TSSBitList::Write
	SET_PROC (0x004BACF2, TSSBitList_Write_GetAddressStr);

	// TSSBundleCalc::Setting
	SET_PROC (0x004BC953, TSSBundleCalc_Setting_GetCode);

	CALL     (0x004BCBA1, TSSBundleCalc_Setting_SetAddressStr);
	JMP_REL8 (0x004BCBA6, 0x004BCBFC);
	NPAD5    (0x004BCBA8);

	/*
		mov     edx, dword ptr [edi]                    ; 004BCBFC _ 8B. 17
		lea     ecx, [ebx + 0D8H]                       ; 004BCBFE _ 8D. 8B, 000000D8
		add     edx, 24                                 ; 004BCC04 _ 83. C2, 18
	*/
	*(LPDWORD)0x004BCBFC = BSWAP32(0x8B178D8B);
	*(LPBYTE )0x004BCC05 = 0xC2;
	CALL     (0x004BCC07, TSSBundleCalc_Setting_SetFileName);
	JMP_REL8 (0x004BCC0C, 0x004BCC62);
	NPAD5    (0x004BCC0E);

	/*
		mov     ecx, dword ptr [edi]                    ; 004BCD66 _ 8B. 0F
		add     ecx, 24                                 ; 004BCD68 _ 83. C1, 18
		nop                                             ; 004BCD6B _ 90
	*/
	*(LPWORD )0x004BCD66 = BSWAP16(0x8B0F);
	*(LPDWORD)0x004BCD68 = BSWAP32(0x83C11890);

	SET_PROC (0x004BD294, TSSBundleCalc_Setting_GetName);

	CALL     (0x004BD408, TSSBundleCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004BD40D, 0x004BD463);
	NPAD5    (0x004BD40F);

	/*
		mov     edx, dword ptr [edi]                    ; 004BD463 _ 8B. 17
		lea     ecx, [ebx + 0A0H]                       ; 004BD465 _ 8D. 8B, 000000A0
		add     edx, 24                                 ; 004BD46B _ 83. C2, 18
	*/
	*(LPDWORD)0x004BD464 = BSWAP32(0x178D8BA0);
	*(LPBYTE )0x004BD46C = 0xC2;
	CALL     (0x004BD46E, TSSBundleCalc_Setting_SetNowValHeadStr);
	JMP_REL8 (0x004BD473, 0x004BD4C9);
	NPAD5    (0x004BD475);

	/*
		mov     edx, dword ptr [edi]                    ; 004BD4C9 _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004BD4CB _ 8D. 4B, 60
		add     edx, 48                                 ; 004BD4CE _ 83. C2, 30
	*/
	*(LPBYTE )0x004BD4CA = 0x17;
	*(LPDWORD)0x004BD4CC = BSWAP32(0x4B6083C2);
	CALL     (0x004BD4D1, TSSBundleCalc_Setting_SetLockStr);
	JMP_REL8 (0x004BD4D6, 0x004BD52C);
	NPAD5    (0x004BD4D8);

	CALL     (0x004BD537, TSSBundleCalc_Setting_SetNowValFootStr);
	JMP_REL8 (0x004BD53C, 0x004BD592);
	NPAD5    (0x004BD53E);

	// TSSBundleCalc::Read
	SET_PROC (0x004BD737, TSSBundleCalc_Read_GetAddressStr);
	SET_PROC (0x004BD7AD, TSSBundleCalc_Read_GetFileName);

	// TSSBundleCalc::Write
	SET_PROC (0x004BE07C, TSSBundleCalc_Write_GetAddressStr);
	SET_PROC (0x004BE0C5, TSSBundleCalc_Write_GetFileName);

	// TSSBundleList::Setting
	SET_PROC (0x004BEAF7, TSSBundleList_Setting_GetCode);

	CALL     (0x004BED45, TSSBundleList_Setting_SetAddressStr);
	JMP_REL8 (0x004BED4A, 0x004BEDA0);
	NPAD5    (0x004BED4C);

	/*
		mov     edx, dword ptr [edi]                    ; 004BEDA0 _ 8B. 17
		lea     ecx, [ebx + 0A0H]                       ; 004BEDA2 _ 8D. 8B, 000000A0
		add     edx, 24                                 ; 004BEDA8 _ 83. C2, 18
	*/
	*(LPDWORD)0x004BEDA0 = BSWAP32(0x8B178D8B);
	*(LPBYTE )0x004BEDA9 = 0xC2;
	CALL     (0x004BEDAB, TSSBundleList_Setting_SetIndexFileName);
	JMP_REL8 (0x004BEDB0, 0x004BEE06);
	NPAD5    (0x004BEDB2);

	CALL     (0x004BEE3A, TSSBundleList_Setting_SetChainFileName);
	JMP_REL8 (0x004BEE3F, 0x004BEE95);
	NPAD5    (0x004BEE41);

	/*
		mov     edx, dword ptr [edi]                    ; 004BEF87 _ 8B. 17
		add     edx, 24                                 ; 004BEF89 _ 83. C2, 18
		nop                                             ; 004BEF8C _ 90
	*/
	*(LPDWORD)0x004BEF87 = BSWAP32(0x8B1783C2);
	*(LPWORD )0x004BEF8B = BSWAP16(0x1890);

	SET_PROC (0x004BF03F, TSSBundleList_Setting_GetName);

	CALL     (0x004BF1B3, TSSBundleList_Setting_SetSubjectName);
	JMP_REL8 (0x004BF1B8, 0x004BF20E);
	NPAD5    (0x004BF1BA);

	/*
		mov     edx, dword ptr [edi]                    ; 004BF20E _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004BF210 _ 8D. 4B, 60
		add     edx, 24                                 ; 004BF213 _ 83. C2, 18
	*/
	*(LPBYTE )0x004BF20F = 0x17;
	*(LPBYTE )0x004BF211 = 0x4B;
	*(LPBYTE )0x004BF214 = 0xC2;
	CALL     (0x004BF216, TSSBundleList_Setting_SetLockStr);
	JMP_REL8 (0x004BF21B, 0x004BF271);
	NPAD5    (0x004BF21D);

	// TSSBundleList::Read
	SET_PROC (0x004BF416, TSSBundleList_Read_GetAddressStr);
	SET_PROC (0x004BF48A, TSSBundleList_Read_GetChainFileName);

	// TSSBundleList::Write
	SET_PROC (0x004BF72F, TSSBundleList_Write_GetAddressStr);
	SET_PROC (0x004BF775, TSSBundleList_Write_GetChainFileName);

	// TSSBundleToggle::Setting
	SET_PROC (0x004BF9CF, TSSBundleToggle_Setting_GetCode);

	CALL     (0x004BFC17, TSSBundleToggle_Setting_SetAddressStr);
	JMP_REL8 (0x004BFC1C, 0x004BFC72);
	NPAD5    (0x004BFC1E);

	/*
		mov     edx, dword ptr [edi]                    ; 004BFC72 _ 8B. 17
		lea     ecx, [ebx + 0E8H]                       ; 004BFC74 _ 8D. 8B, 000000E8
		add     edx, 24                                 ; 004BFC7A _ 83. C2, 18
	*/
	*(LPBYTE )0x004BFC73 = 0x17;
	*(LPBYTE )0x004BFC75 = 0x8B;
	*(LPBYTE )0x004BFC7B = 0xC2;
	CALL     (0x004BFC7D, TSSBundleToggle_Setting_SetFileName);
	JMP_REL8 (0x004BFC82, 0x004BFCD8);
	NPAD5    (0x004BFC84);

	SET_PROC (0x004BFDC6, TSSBundleToggle_Setting_GetName);

	CALL     (0x004BFF37, TSSBundleToggle_Setting_SetSubjectName);
	JMP_REL8 (0x004BFF3C, 0x004BFF92);
	NPAD5    (0x004BFF3E);

	/*
		mov     edx, dword ptr [edi]                    ; 004BFF92 _ 8B. 17
		lea     ecx, [ebx + 0C0H]                       ; 004BFF94 _ 8D. 8B, 000000C0
		add     edx, 24                                 ; 004BFF9A _ 83. C2, 18
	*/
	*(LPBYTE )0x004BFF93 = 0x17;
	*(LPBYTE )0x004BFF95 = 0x8B;
	*(LPBYTE )0x004BFF9B = 0xC2;
	CALL     (0x004BFF9D, TSSBundleToggle_Setting_SetNowValHeadStr);
	JMP_REL8 (0x004BFFA2, 0x004BFFF8);
	NPAD5    (0x004BFFA4);

	/*
		mov     edx, dword ptr [edi]                    ; 004BFFF8 _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004BFFFA _ 8D. 4B, 60
		add     edx, 48                                 ; 004BFFFD _ 83. C2, 30
	*/
	*(LPDWORD)0x004BFFF8 = BSWAP32(0x8B178D4B);
	*(LPBYTE )0x004BFFFE = 0xC2;
	CALL     (0x004C0000, TSSBundleToggle_Setting_SetLockStr);
	JMP_REL8 (0x004C0005, 0x004C005B);
	NPAD5    (0x004C0007);

	// TSSBundleToggle::Read
	SET_PROC (0x004C01FF, TSSBundleToggle_Read_GetAddressStr);
	SET_PROC (0x004C0248, TSSBundleToggle_Read_GetFileName);

	// TSSBundleToggle::Write
	SET_PROC (0x004C0BFB, TSSBundleToggle_Write_GetAddressStr);
	SET_PROC (0x004C0C44, TSSBundleToggle_Write_GetFileName);

	// TSSCalc::Setting
	SET_PROC (0x004C1507, TSSCalc_Setting_GetCode);

	/*
		lea     ecx, [ebx + 88H]                        ; 004C1757 _ 8D. 8B, 00000088
		mov     edx, dword ptr [edi]                    ; 004C175D _ 8B. 17
	*/
	*(LPBYTE )0x004C1758 = 0x8B;
	*(LPBYTE )0x004C175E = 0x17;
	CALL     (0x004C1762, TSSCalc_Setting_SetAddressStr);
	JMP_REL8 (0x004C1767, 0x004C17BD);
	NPAD5    (0x004C1769);

	SET_PROC (0x004C1A1C, TSSCalc_Setting_GetName);

	CALL     (0x004C1B8D, TSSCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004C1B92, 0x004C1BE8);
	NPAD5    (0x004C1B94);

	/*
		mov     edx, dword ptr [edi]                    ; 004C1BE8 _ 8B. 17
		lea     ecx, [ebx + 0A0H]                       ; 004C1BEA _ 8D. 8B, 000000A0
		add     edx, 24                                 ; 004C1BF0 _ 83. C2, 18
	*/
	*(LPDWORD)0x004C1BE8 = BSWAP32(0x8B178D8B);
	*(LPBYTE )0x004C1BF1 = 0xC2;
	CALL     (0x004C1BF3, TSSCalc_Setting_SetNowValHeadStr);
	JMP_REL8 (0x004C1BF8, 0x004C1C4E);
	NPAD5    (0x004C1BFA);

	/*
		mov     edx, dword ptr [edi]                    ; 004C1C4E _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004C1C50 _ 8D. 4B, 60
		add     edx, 48                                 ; 004C1C53 _ 83. C2, 30
	*/
	*(LPBYTE )0x004C1C4F = 0x17;
	*(LPBYTE )0x004C1C51 = 0x4B;
	*(LPBYTE )0x004C1C54 = 0xC2;
	CALL     (0x004C1C56, TSSCalc_Setting_SetLockStr);
	JMP_REL8 (0x004C1C5B, 0x004C1CB1);
	NPAD5    (0x004C1C5D);

	CALL     (0x004C1CBC, TSSCalc_Setting_SetNowValFootStr);
	JMP_REL8 (0x004C1CC1, 0x004C1D17);
	NPAD5    (0x004C1CC3);

	// TSSCalc::Read
	SET_PROC (0x004C1EA7, TSSCalc_Read_GetAddressStr);

	// TSSCalc::Write
	SET_PROC (0x004C2067, TSSCalc_Write_GetAddressStr);

	// TSSCopy::Setting
	SET_PROC (0x004C245B, TSSCopy_Setting_GetCode);

	/*
		lea     ecx, [ebx + 80H]                        ; 004C26A8 _ 8D. 8B, 00000080
		mov     edx, dword ptr [edi]                    ; 004C26AE _ 8B. 17
	*/
	*(LPBYTE )0x004C26A9 = 0x8B;
	*(LPBYTE )0x004C26AF = 0x17;
	CALL     (0x004C26B3, TSSCopy_Setting_SetAddressStr);
	JMP_REL8 (0x004C26B8, 0x004C270E);
	NPAD5    (0x004C26BA);

	CALL     (0x004C2719, TSSCopy_Setting_SetSrcAddressStr);
	JMP_REL8 (0x004C271E, 0x004C2774);
	NPAD5    (0x004C2720);

	SET_PROC (0x004C27CE, TSSCopy_Setting_GetName);

	CALL     (0x004C293B, TSSCopy_Setting_SetSubjectName);
	JMP_REL8 (0x004C2940, 0x004C2996);
	NPAD5    (0x004C2942);

	/*
		mov     edx, dword ptr [edi]                    ; 004C2996 _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004C2998 _ 8D. 4B, 60
		add     edx, 24                                 ; 004C299B _ 83. C2, 18
	*/
	*(LPBYTE )0x004C2997 = 0x17;
	*(LPBYTE )0x004C2999 = 0x4B;
	*(LPBYTE )0x004C299C = 0xC2;
	CALL     (0x004C299E, TSSCopy_Setting_SetLockStr);
	JMP_REL8 (0x004C29A3, 0x004C29F9);
	NPAD5    (0x004C29A5);

	// TSSCopy::Write
	SET_PROC (0x004C2B4C, TSSCopy_Write_GetAddressStr);

	SET_PROC (0x004C2B62, TSSCopy_Write_GetSrcAddressStr);

	// TSSDoubleList::Setting
	SET_PROC (0x004C3674, TSSDoubleList_Setting_GetCode);

	/*
		mov     edx, dword ptr [ebp - 18H]              ; 004C388A _ 8B. 55, E8
		lea     ecx, [ebx + 98H]                        ; 004C388D _ 8D. 8B, 00000098
	*/
	*(LPDWORD)0x004C388B = BSWAP32(0x55E88D8B);
	*(LPDWORD)0x004C388F = 0x00000098;
	CALL     (0x004C3893, TSSDoubleList_Setting_SetAddressStr);
	JMP_REL8 (0x004C3898, 0x004C38DC);
	NPAD5    (0x004C389A);

	/*
		lea     ecx, [ebx + 0B0H]                       ; 004C38E2 _ 8D. 8B, 000000B0
	*/
	*(LPWORD )0x004C38E2 = BSWAP16(0x8D8B);
	*(LPDWORD)0x004C38E4 = 0x000000B0;
	CALL     (0x004C38E8, TSSDoubleList_Setting_SetIndexFileName);
	JMP_REL8 (0x004C38ED, 0x004C3931);
	NPAD5    (0x004C38EF);

	/*
		lea     ecx, [ebx + 0C8H]                       ; 004C3937 _ 8D. 8B, 000000C8
	*/
	*(LPWORD )0x004C3937 = BSWAP16(0x8D8B);
	*(LPDWORD)0x004C3939 = 0x000000C8;
	CALL     (0x004C393D, TSSDoubleList_Setting_SetDataFileName);
	JMP_REL8 (0x004C3942, 0x004C3986);
	NPAD5    (0x004C3944);

	/*
		mov     eax, dword ptr [ebp - 18H]              ; 004C3AAF _ 8B. 45, E8
		add     eax, 96                                 ; 004C3AB2 _ 83. C0, 60
	*/
	*(LPDWORD)0x004C3AAF = BSWAP32(0x8B45E883);
	*(LPWORD )0x004C3AB3 = BSWAP16(0xC060);

	SET_PROC (0x004C3E69, TSSDoubleList_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 004C3FCA _ 8D. 4B, 44
	*/
	*(LPDWORD)0x004C3FCA = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004C3FCD, TSSDoubleList_Setting_SetSubjectName);
	JMP_REL8 (0x004C3FD2, 0x004C4016);
	NPAD5    (0x004C3FD4);

	/*
		lea     ecx, [ebx + 60H]                        ; 004C401C _ 8D. 4B, 60
	*/
	*(LPDWORD)0x004C401C = BSWAP32(0x8D4B60E8);
	SET_PROC (0x004C401F, TSSDoubleList_Setting_SetLockStr);
	JMP_REL8 (0x004C4024, 0x004C4068);
	NPAD5    (0x004C4026);

	// TSSDoubleList::Read
	SET_PROC (0x004C4212, TSSDoubleList_Read_GetAddressStr);
	SET_PROC (0x004C422F, TSSDoubleList_Read_GetAddressStr);
	SET_PROC (0x004C4274, TSSDoubleList_Read_GetDataFileName);

	// TSSDoubleList::Write
	SET_PROC (0x004C535A, TSSDoubleList_Write_GetAddressStr);
	SET_PROC (0x004C53A3, TSSDoubleList_Write_GetDataFileName);
	SET_PROC (0x004C54EE, TSSDoubleList_Write_GetAddressStr);

	// TSSDoubleList::ToByteCode
	SET_PROC (0x004C6542, TSSDoubleList_ToByteCode_GetAddressStr);
	SET_PROC (0x004C6591, TSSDoubleList_ToByteCode_GetDataFileName);

	// TSSDoubleToggle::Setting
	SET_PROC (0x004C7B70, TSSDoubleToggle_Setting_GetCode);

	/*
		lea     ecx, [ebx + 98H]                        ; 004C7D76 _ 8D. 8B, 00000098
	*/
	*(LPWORD )0x004C7D76 = BSWAP16(0x8D8B);
	*(LPDWORD)0x004C7D78 = 0x00000098;
	CALL     (0x004C7D7C, TSSDoubleToggle_Setting_SetAddressStr);
	JMP_REL8 (0x004C7D81, 0x004C7DC5);
	NPAD5    (0x004C7D83);

	/*
		lea     ecx, [ebx + 0B0H]                       ; 004C7DCB _ 8D. 8B, 000000B0
	*/
	*(LPWORD )0x004C7DCB = BSWAP16(0x8D8B);
	*(LPDWORD)0x004C7DCD = 0x000000B0;
	CALL     (0x004C7DD1, TSSDoubleToggle_Setting_SetIndexFileName);
	JMP_REL8 (0x004C7DD6, 0x004C7E1A);
	NPAD5    (0x004C7DD8);

	/*
		lea     ecx, [ebx + 0C8H]                       ; 004C7E20 _ 8D. 8B, 000000C8
	*/
	*(LPDWORD)0x004C7E20 = BSWAP32(0x8D8BC800);
	*(LPWORD )0x004C7E24 = BSWAP16(0x0000);
	CALL     (0x004C7E26, TSSDoubleToggle_Setting_SetDataFileName);
	JMP_REL8 (0x004C7E2B, 0x004C7E6F);
	NPAD5    (0x004C7E2D);

	/*
		mov     eax, dword ptr [ebp - 18H]              ; 004C7F2E _ 8B. 45, E8
		add     eax, 48                                 ; 004C7F31 _ 83. C0, 30
	*/
	*(LPWORD )0x004C7F2E = BSWAP16(0x8B45);
	*(LPDWORD)0x004C7F30 = BSWAP32(0xE883C030);

	SET_PROC (0x004C82C0, TSSDoubleToggle_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 004C8421 _ 8D. 4B, 44
	*/
	*(LPDWORD)0x004C8421 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x004C8424, TSSDoubleToggle_Setting_SetSubjectName);
	JMP_REL8 (0x004C8429, 0x004C846D);
	NPAD5    (0x004C842B);

	/*
		lea     ecx, [ebx + 60H]                        ; 004C8473 _ 8D. 4B, 60
	*/
	*(LPDWORD)0x004C8473 = BSWAP32(0x8D4B60E8);
	SET_PROC (0x004C8476, TSSDoubleToggle_Setting_SetLockStr);
	JMP_REL8 (0x004C847B, 0x004C84BF);
	NPAD5    (0x004C847D);

	// TSSDoubleToggle::Read
	SET_PROC (0x004C866A, TSSDoubleToggle_Read_GetAddressStr);
	SET_PROC (0x004C86B3, TSSDoubleToggle_Read_GetDataFileName);

	// TSSDoubleToggle::Write
	SET_PROC (0x004CAF09, TSSDoubleToggle_Write_GetAddressStr);
	SET_PROC (0x004CAF54, TSSDoubleToggle_Write_GetDataFileName);

	// TSSFloatCalc::Setting
	SET_PROC (0x004CDAFB, TSSFloatCalc_Setting_GetCode);

	/*
		lea     ecx, [ebx + 98H]                        ; 004CDD4B _ 8D. 8B, 00000098
		mov     edx, dword ptr [edi]                    ; 004CDD51 _ 8B. 17
	*/
	*(LPBYTE )0x004CDD4C = 0x8B;
	*(LPBYTE )0x004CDD52 = 0x17;
	CALL     (0x004CDD56, TSSFloatCalc_Setting_SetAddressStr);
	JMP_REL8 (0x004CDD5B, 0x004CDDB1);
	NPAD5    (0x004CDD5D);

	SET_PROC (0x004CDF3B, TSSFloatCalc_Setting_GetName);

	CALL     (0x004CE0AC, TSSFloatCalc_Setting_SetSubjectName);
	JMP_REL8 (0x004CE0B1, 0x004CE107);
	NPAD5    (0x004CE0B3);

	/*
		mov     edx, dword ptr [edi]                    ; 004CE107 _ 8B. 17
		lea     ecx, [ebx + 0B0H]                       ; 004CE109 _ 8D. 8B, 000000B0
		add     edx, 24                                 ; 004CE10F _ 83. C2, 18
	*/
	*(LPDWORD)0x004CE108 = BSWAP32(0x178D8BB0);
	*(LPBYTE )0x004CE110 = 0xC2;
	CALL     (0x004CE112, TSSFloatCalc_Setting_SetNowValHeadStr);
	JMP_REL8 (0x004CE117, 0x004CE16D);
	NPAD5    (0x004CE119);

	/*
		mov     edx, dword ptr [edi]                    ; 004CE16D _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 004CE16F _ 8D. 4B, 60
		add     edx, 48                                 ; 004CE172 _ 83. C2, 30
	*/
	*(LPBYTE )0x004CE16E = 0x17;
	*(LPDWORD)0x004CE170 = BSWAP32(0x4B6083C2);
	CALL     (0x004CE175, TSSFloatCalc_Setting_SetLockStr);
	JMP_REL8 (0x004CE17A, 0x004CE1D0);
	NPAD5    (0x004CE17C);

	CALL     (0x004CE1DB, TSSFloatCalc_Setting_SetNowValFootStr);
	JMP_REL8 (0x004CE1E0, 0x004CE236);
	NPAD5    (0x004CE1E2);

	// TSSFloatCalc::Read
	SET_PROC (0x004CE3BB, TSSFloatCalc_Read_GetAddressStr);

	// TSSFloatCalc::Write
	SET_PROC (0x004CE593, TSSFloatCalc_Write_GetAddressStr);

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
	CALL     (0x005030B9, TSSGCtrl_MakeADJFile_GetAddressStr);

	SET_PROC (0x00503164, TSSGCtrl_MakeADJFile_GetCode);

	// TSSGCtrl::Open
	JMP_REL32(0x0051C338, (DWORD)TSSGCtrl_Open);

	// TSSList::Setting
	SET_PROC (0x00529933, TSSList_Setting_GetCode);

	/*
		lea     ecx, [ebx + 88H]                        ; 00529B80 _ 8D. 8B, 00000088
		mov     edx, dword ptr [edi]                    ; 00529B86 _ 8B. 17
	*/
	*(LPBYTE )0x00529B81 = 0x8B;
	*(LPBYTE )0x00529B87 = 0x17;
	CALL     (0x00529B8B, TSSList_Setting_SetAddressStr);
	JMP_REL8 (0x00529B90, 0x00529BE6);
	NPAD5    (0x00529B92);

	CALL     (0x00529BF1, TSSList_Setting_SetIndexFileName);
	JMP_REL8 (0x00529BF6, 0x00529C4C);
	NPAD5    (0x00529BF8);

	/*
		mov     edx, dword ptr [edi]                    ; 00529CAA _ 8B. 17
		add     edx, 24                                 ; 00529CAC _ 83. C2, 18
		nop                                             ; 00529CAF _ 90
	*/
	*(LPWORD )0x00529CAA = BSWAP16(0x8B17);
	*(LPDWORD)0x00529CAC = BSWAP32(0x83C21890);

	SET_PROC (0x00529D62, TSSList_Setting_GetName);

	CALL     (0x00529ED6, TSSList_Setting_SetSubjectName);
	JMP_REL8 (0x00529EDB, 0x00529F31);
	NPAD5    (0x00529EDD);

	/*
		mov     edx, dword ptr [edi]                    ; 00529F31 _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 00529F33 _ 8D. 4B, 60
		add     edx, 24                                 ; 00529F36 _ 83. C2, 18
	*/
	*(LPBYTE )0x00529F32 = 0x17;
	*(LPDWORD)0x00529F34 = BSWAP32(0x4B6083C2);
	CALL     (0x00529F39, TSSList_Setting_SetLockStr);
	JMP_REL8 (0x00529F3E, 0x00529F94);
	NPAD5    (0x00529F40);

	// TSSList::Read
	SET_PROC (0x0052A10B, TSSList_Read_GetAddressStr);

	// TSSList::Write
	SET_PROC (0x0052A2AF, TSSList_Write_GetAddressStr);

	// TSSString::Setting
	SET_PROC (0x0052A6F3, TSSString_Setting_GetCode);

	CALL     (0x0052A93E, TSSString_Setting_SetAddressStr);
	JMP_REL8 (0x0052A943, 0x0052A999);
	NPAD5    (0x0052A945);

	/*
		mov     edx, eax                                ; 0052AA61 _ 8B. D0
        mov     ecx, ebx                                ; 0052AA63 _ 8B. CB
	*/
	*(LPDWORD)0x0052AA61 = BSWAP32(0x8BD08BCB);
	CALL     (0x0052AA65, TSSString_Setting_SetEndWord);
	JMP_REL8 (0x0052AA6A, 0x0052AAC2);
	NPAD1    (0x0052AA6C);

	SET_PROC (0x0052AB89, TSSString_Setting_GetName);

	CALL     (0x0052ACFD, TSSString_Setting_SetSubjectName);
	JMP_REL8 (0x0052AD02, 0x0052AD58);
	NPAD5    (0x0052AD04);

	/*
		mov     edx, dword ptr [edi]                    ; 0052AD58 _ 8B. 17
		lea     ecx, [ebx + 0B0H]                       ; 0052AD5A _ 8D. 8B, 000000B0
		add     edx, 24                                 ; 0052AD60 _ 83. C2, 18
	*/
	*(LPDWORD)0x0052AD58 = BSWAP32(0x8B178D8B);
	*(LPBYTE )0x0052AD61 = 0xC2;
	CALL     (0x0052AD63, TSSString_Setting_SetNowValHeadStr);
	JMP_REL8 (0x0052AD68, 0x0052ADBE);
	NPAD5    (0x0052AD6A);

	/*
		mov     edx, dword ptr [edi]                    ; 0052ADBE _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 0052ADC0 _ 8D. 4B, 60
		add     edx, 48                                 ; 0052ADC3 _ 83. C2, 30
	*/
	*(LPBYTE )0x0052ADBF = 0x17;
	*(LPBYTE )0x0052ADC1 = 0x4B;
	*(LPBYTE )0x0052ADC4 = 0xC2;
	CALL     (0x0052ADC6, TSSString_Setting_SetLockStr);
	JMP_REL8 (0x0052ADCB, 0x0052AE21);
	NPAD5    (0x0052ADCD);

	// TSSString::Read
	SET_PROC (0x0052AFA3, TSSString_Read_GetAddressStr);

	CALL     (0x0052B03A, TSSString_Read_GetEndWord);
	NPAD1    (0x0052B03F);

	/*
		mov     edx, dword ptr [edi]                    ; 0052B040 _ 8B. 17
		cmp     edx, dword ptr [edi+4H]                 ; 0052B042 _ 3B. 57, 04
	*/
	*(LPDWORD)0x0052B040 = BSWAP32(0x8B173B57);

	JMP_REL8 (0x0052B053, 0x0052B059);
	NPAD4    (0x0052B055);

	JMP_REL8 (0x0052B0C5, 0x0052B0CB);
	NPAD4    (0x0052B0C7);
	PUSH_EDI (0x0052B0CB);

	// TSSString::Write
	SET_PROC (0x0052B2A8, TSSString_Write_GetAddressStr);

	/*
		call    TSSString_Write_GetEndWord              ; 0052B33C _ E8, ????????
		mov     ecx, dword ptr [ebp - 18H]              ; 0052B341 _ 8B. 4D, E8
		jmp     0052B347H                               ; 0052B344 _ EB, 01
		nop                                             ; 0052B346 _ 90
	*/
	CALL     (0x0052B33C, TSSString_Write_GetEndWord);
	*(LPDWORD)0x0052B341 = BSWAP32(0x8B4DE8EB);
	*(LPWORD )0x0052B345 = BSWAP16(0x0190);

	/*
		call    TSSString_Write_GetEndWord              ; 0052B40D _ E8, ????????
        lea     ecx, [ebp - 0DCH]                       ; 0052B412 _ 8D. 8D, FFFFFF24
		jmp     0052B41BH                               ; 0052B418 _ EB, 01
		nop                                             ; 0052B41A _ 90
	*/
	CALL     (0x0052B40D, TSSString_Write_GetEndWord);
	*(LPDWORD)0x0052B412 = BSWAP32(0x8D8D24FF);
	*(LPDWORD)0x0052B416 = BSWAP32(0xFFFFEB01);
	*(LPBYTE )0x0052B41A = 0x90;

	// TSSToggle::Setting
	SET_PROC (0x0052BAF7, TSSToggle_Setting_GetCode);

	CALL     (0x0052BD48, TSSToggle_Setting_SetAddressStr);
	JMP_REL8 (0x0052BD4D, 0x0052BDA3);
	NPAD5    (0x0052BD4F);

	/*
		mov     edx, dword ptr [edi]                    ; 0052BDA3 _ 8B. 17
		lea     ecx, [ebx + 90H]                        ; 0052BDA5 _ 8D. 8B, 00000090
		add     edx, 24                                 ; 0052BDAB _ 83. C2, 18
	*/
	*(LPDWORD)0x0052BDA4 = BSWAP32(0x178D8B90);
	*(LPBYTE )0x0052BDAC = 0xC2;
	CALL     (0x0052BDAE, TSSToggle_Setting_SetOnCode);
	JMP_REL8 (0x0052BDB3, 0x0052BE09);
	NPAD5    (0x0052BDB5);

	/*
		mov     edx, dword ptr [edi]                    ; 0052BE09 _ 8B. 17
		lea     ecx, [ebx + 0A8H]                       ; 0052BE0B _ 8D. 8B, 000000A8
		add     edx, 48                                 ; 0052BE11 _ 83. C2, 30
	*/
	*(LPBYTE )0x0052BE0A = 0x17;
	*(LPBYTE )0x0052BE0C = 0x8B;
	*(LPBYTE )0x0052BE12 = 0xC2;
	CALL     (0x0052BE14, TSSToggle_Setting_SetOffCode);
	JMP_REL8 (0x0052BE19, 0x0052BE6F);
	NPAD5    (0x0052BE1B);

	/*
        push    15                                      ; 0052BE77 _ 6A, 0F
        mov     edx, dword ptr [edi]                    ; 0052BE79 _ 8B. 17
		nop                                             ; 0052BE7B _ 90
	*/
	*(LPDWORD)0x0052BE78 = BSWAP32(0x0F8B1790);

	/*
		mov     eax, dword ptr [edi]                    ; 0052BE8D _ 8B. 07
		add     eax, 48                                 ; 0052BE8F _ 83. C0, 30
		nop                                             ; 0052BE92 _ 90
	*/
	*(LPDWORD)0x0052BE8D = BSWAP32(0x8B0783C0);
	*(LPWORD )0x0052BE91 = BSWAP16(0x3090);

	/*
		mov     edx, dword ptr [edi]                    ; 0052BE9D _ 8B. 17
		add     edx, 24                                 ; 0052BE9F _ 83. C2, 18
		nop                                             ; 0052BEA2 _ 90
	*/
	*(LPDWORD)0x0052BE9D = BSWAP32(0x8B1783C2);
	*(LPWORD )0x0052BEA1 = BSWAP16(0x1890);

	SET_PROC (0x0052C00F, TSSToggle_Setting_GetName);

	CALL     (0x0052C183, TSSToggle_Setting_SetSubjectName);
	JMP_REL8 (0x0052C188, 0x0052C1DE);
	NPAD5    (0x0052C18A);

	/*
		mov     edx, dword ptr [edi]                    ; 0052C1DE _ 8B. 17
		lea     ecx, [ebx + 0C0H]                       ; 0052C1E0 _ 8D. 8B, 000000C0
		add     edx, 24                                 ; 0052C1E6 _ 83. C2, 18
	*/
	*(LPBYTE )0x0052C1DF = 0x17;
	*(LPBYTE )0x0052C1E1 = 0x8B;
	*(LPBYTE )0x0052C1E7 = 0xC2;
	CALL     (0x0052C1E9, TSSToggle_Setting_SetNowValHeadStr);
	JMP_REL8 (0x0052C1EE, 0x0052C244);
	NPAD5    (0x0052C1F0);

	/*
		mov     edx, dword ptr [edi]                    ; 0052C244 _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 0052C246 _ 8D. 4B, 60
		add     edx, 48                                 ; 0052C249 _ 83. C2, 30
	*/
	*(LPDWORD)0x0052C244 = BSWAP32(0x8B178D4B);
	*(LPBYTE )0x0052C24A = 0xC2;
	CALL     (0x0052C24C, TSSToggle_Setting_SetLockStr);
	JMP_REL8 (0x0052C251, 0x0052C2A7);
	NPAD5    (0x0052C253);

	// TSSToggle::Read
	SET_PROC (0x0052C4B5, TSSToggle_Read_GetAddressStr);

	CALL     (0x0052C4BD, TSSToggle_Read_GetOnCode1);
	NPAD1    (0x0052C4C2);

	CALL     (0x0052C504, TSSToggle_Read_GetOffCode);
	NPAD1    (0x0052C509);

	SET_PROC (0x0052C526, TSSToggle_Read_GetOnCode2);

	// TSSToggle::Write
	SET_PROC (0x0052C795, TSSToggle_Write_GetAddressStr);

	CALL     (0x0052C7B3, TSSToggle_Write_GetOffCode);
	NPAD1    (0x0052C7B8);

	SET_PROC (0x0052C7CF, TSSToggle_Write_GetOnCode);

	// TSSToggle::ToByteCode
	SET_PROC (0x0052CB06, TSSToggle_ToByteCode_GetOnOffCode);

	// TSSTrace::Setting
	/*
		mov     eax, dword ptr [ebx + 34H]              ; 0052CB5E _ 8B. 43, 34
        mov     dword ptr [ebx + 80H], eax              ; 0052CB61 _ 89. 83, 00000080
		jmp     0052CB92H                               ; 0052CB67 _ EB, 29
		nop                                             ; 0052CB69 _ 90
	*/
	*(LPDWORD)0x0052CB5E = BSWAP32(0x8B433489);
	*(LPDWORD)0x0052CB62 = BSWAP32(0x83800000);
	*(LPDWORD)0x0052CB66 = BSWAP32(0x00EB2990);

	SET_PROC (0x0052CC08, TSSTrace_Setting_GetName);

	/*
		lea     ecx, [ebx + 44H]                        ; 0052CD51 _ 8D. 4B, 44
	*/
	*(LPDWORD)0x0052CD51 = BSWAP32(0x8D4B44E8);
	SET_PROC (0x0052CD54, TSSTrace_Setting_SetSubjectName);
	JMP_REL8 (0x0052CD59, 0x0052CD9D);
	NPAD5    (0x0052CD5B);

	/*
		lea     ecx, [ebx + 60H]                        ; 0052CDA3 _ 8D. 4B, 60
	*/
	*(LPDWORD)0x0052CDA3 = BSWAP32(0x8D4B60E8);
	SET_PROC (0x0052CDA6, TSSTrace_Setting_SetLockStr);
	JMP_REL8 (0x0052CDAB, 0x0052CDEF);
	NPAD5    (0x0052CDAD);

	// TSSTrace::Write
	CALL     (0x0052CF28, TSSTrace_Write_GetFileName);

	// TSSGSubject::GetSubjectName
	CALL     (0x0052CF86, TSSGSubject_GetSubjectName_GetSubjectName);

	// TSSBundleFloatCalc::Setting
	SET_PROC (0x0052D373, TSSBundleFloatCalc_Setting_GetCode);

	CALL     (0x0052D5C1, TSSBundleFloatCalc_Setting_SetAddressStr);
	JMP_REL8 (0x0052D5C6, 0x0052D61C);
	NPAD5    (0x0052D5C8);

	/*
		mov     edx, dword ptr [edi]                    ; 0052D61C _ 8B. 17
		lea     ecx, [ebx + 0E8H]                       ; 0052D61E _ 8D. 8B, 000000E8
		add     edx, 24                                 ; 0052D624 _ 83. C2, 18
	*/
	*(LPDWORD)0x0052D61C = BSWAP32(0x8B178D8B);
	*(LPBYTE )0x0052D625 = 0xC2;
	CALL     (0x0052D627, TSSBundleFloatCalc_Setting_SetFileName);
	JMP_REL8 (0x0052D62C, 0x0052D682);
	NPAD5    (0x0052D62E);

	SET_PROC (0x0052D8CB, TSSBundleFloatCalc_Setting_GetName);

	CALL     (0x0052DA3C, TSSBundleFloatCalc_Setting_SetSubjectName);
	JMP_REL8 (0x0052DA41, 0x0052DA97);
	NPAD5    (0x0052DA43);

	/*
		mov     edx, dword ptr [edi]                    ; 0052DA97 _ 8B. 17
		lea     ecx, [ebx + 0B0H]                       ; 0052DA99 _ 8D. 8B, 000000B0
		add     edx, 24                                 ; 0052DA9F _ 83. C2, 18
	*/
	*(LPDWORD)0x0052DA98 = BSWAP32(0x178D8BB0);
	*(LPBYTE )0x0052DAA0 = 0xC2;
	CALL     (0x0052DAA2, TSSBundleFloatCalc_Setting_SetNowValHeadStr);
	JMP_REL8 (0x0052DAA7, 0x0052DAFD);
	NPAD5    (0x0052DAA9);

	/*
		mov     edx, dword ptr [edi]                    ; 0052DAFD _ 8B. 17
		lea     ecx, [ebx + 60H]                        ; 0052DAFF _ 8D. 4B, 60
		add     edx, 48                                 ; 0052DB02 _ 83. C2, 30
	*/
	*(LPBYTE )0x0052DAFE = 0x17;
	*(LPDWORD)0x0052DB00 = BSWAP32(0x4B6083C2);
	CALL     (0x0052DB05, TSSBundleFloatCalc_Setting_SetLockStr);
	JMP_REL8 (0x0052DB0A, 0x0052DB60);
	NPAD5    (0x0052DB0C);

	CALL     (0x0052DB6B, TSSBundleFloatCalc_Setting_SetNowValFootStr);
	JMP_REL8 (0x0052DB70, 0x0052DBC6);
	NPAD5    (0x0052DB72);

	// TSSBundleFloatCalc::Read
	SET_PROC (0x0052DD6B, TSSBundleFloatCalc_Read_GetAddressStr);
	SET_PROC (0x0052DDE1, TSSBundleFloatCalc_Read_GetFileName);

	// TSSBundleFloatCalc::Write
	SET_PROC (0x0052E71F, TSSBundleFloatCalc_Write_GetAddressStr);
	SET_PROC (0x0052E765, TSSBundleFloatCalc_Write_GetFileName);

	// TSSSplit::Setting
	SET_PROC (0x0052FF77, TSSSplit_Setting_GetCode);

	/*
		mov     edx, dword ptr [edi]                    ; 0053040A _ 8B. 17
		lea     ecx, [ebx + 70H]                        ; 0053040C _ 8D. 4B, 70
		add     edx, 48                                 ; 0053040F _ 83. C2, 30
	*/
	*(LPBYTE )0x0053040B = 0x17;
	*(LPBYTE )0x0053040D = 0x4B;
	*(LPBYTE )0x00530410 = 0xC2;
	CALL     (0x00530412, TSSSplit_Setting_SetStrParam);
	JMP_REL8 (0x00530417, 0x0053046D);
	NPAD5    (0x00530419);

	/*
		mov     edx, dword ptr [edi]                    ; 0052C244 _ 8B. 17
		lea     ecx, [ebx + 70H]                        ; 0052C246 _ 8D. 4B, 70
		add     edx, 48                                 ; 0052C249 _ 83. C2, 30
	*/
	CALL     (0x0053050C, TSSSplit_Setting_SetStrParam);
	JMP_REL8 (0x00530511, 0x0053056E);
	NPAD5    (0x00530513);

	/*
		mov     eax, dword ptr [ebx + 1CH]              ; 0053056E _ 8B. 43, 1C
		mov     dword ptr [ebx + 4CH], eax              ; 00530571 _ 89. 43, 4C
		jmp     005305CFH                               ; 00530574 _ EB, 59
	*/
	*(LPDWORD)0x0053056E = BSWAP32(0x8B431C89);
	*(LPDWORD)0x00530572 = BSWAP32(0x434CEB59);
	NPAD4    (0x00530576);
}

void __cdecl Attach_SubjectStringTable()
{
	AttachConstructor();
	AttachVirtualFunction();
	AttachOperator();
}
