#include <windows.h>

EXTERN_C void __cdecl Caller_CheckSSGVersion();
EXTERN_C void __cdecl Caller_Parsing();
EXTERN_C void __cdecl Caller_ParsingWithVal();
EXTERN_C void __cdecl Caller_ParsingDouble();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_size_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_adjustment_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_enabled_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_involve_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_note_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_process_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_title_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_maker_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_creator_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_distinction_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_ADJSubjectSet_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_val_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine_Release();
EXTERN_C void __cdecl TSSGCtrl_AddressNaming_ReplaceDefine1();
EXTERN_C void __cdecl TSSGCtrl_AddressNaming_ReplaceDefine2();
EXTERN_C void __cdecl TSSGCtrl_AddressNaming_ReplaceDefine3();
EXTERN_C void __cdecl TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine();
EXTERN_C void __cdecl TSSGCtrl_Funneling_ReplaceDefine();
EXTERN_C void __cdecl Caller_TSSGCtrl_Open();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_OpenProcess();
EXTERN_C void __cdecl TSSGCtrl_AddressNaming_OpenProcess();
EXTERN_C void __cdecl TSSGCtrl_Funneling_Write();
EXTERN_C void __cdecl Caller_TSSGCtrl_IsEnabled_except1();
EXTERN_C void __cdecl TSSBundleCalc_Read_Read();
EXTERN_C void __cdecl TSSBundleCalc_Write_Write();
EXTERN_C void __cdecl TSSBundleList_Read_OneRead();
EXTERN_C void __cdecl TSSBundleList_Write_OneWrite();
EXTERN_C void __cdecl TSSBundleToggle_Read_Compare();
EXTERN_C void __cdecl TSSBundleToggle_Write_Write();
EXTERN_C void __cdecl TSSCopy_Write_OneRead();
EXTERN_C void __cdecl TSSCopy_Write_OneWrite();
EXTERN_C void __cdecl TSSDoubleList_Read_OneRead();
EXTERN_C void __cdecl TSSDoubleList_Read_Compare1();
EXTERN_C void __cdecl TSSDoubleList_Read_Compare2();
EXTERN_C void __cdecl TSSDoubleList_Write_OneWrite();
EXTERN_C void __cdecl TSSDoubleList_Write_Write1();
EXTERN_C void __cdecl TSSDoubleList_Write_Write2();
EXTERN_C void __cdecl TSSDoubleToggle_Read_Compare1();
EXTERN_C void __cdecl TSSDoubleToggle_Read_Compare2();
EXTERN_C void __cdecl TSSDoubleToggle_Write_Write1();
EXTERN_C void __cdecl TSSDoubleToggle_Write_Write2();
EXTERN_C void __cdecl TSSBundleCalc_Read_Read();
#define TSSBundleFloatCalc_Read_Read TSSBundleCalc_Read_Read
EXTERN_C void __cdecl TSSBundleFloatCalc_Write_OneWrite1();
EXTERN_C void __cdecl TSSBundleFloatCalc_Write_OneWrite2();
EXTERN_C void __cdecl Caller_TSSGCtrl_GetAddress_SaveAddress();

#define ADD_EAX_DWORD_PTR_EBX         (WORD )0x0303
#define CMP_AL_IMM8                   (WORD )0x3C65
#define PUSH_IMM8                     (BYTE )0x6A
#define MOV_EAX_TO_DWORD_PTR_EBP_IMM8 (WORD )0x4589
#define LEA_EAX_EDI_MUL3              (WORD )0x048D
#define LEA_EAX_EDI_MUL3_3RD_BYTE     (BYTE )0x7F
#define NOP                           (BYTE )0x90
#define NOP_X2                        (WORD )0x9090
#define NOP_X4                        (DWORD)0x90909090
#define SHL_EAX_IMM8                  (WORD )0xE0C1
#define CALL_REL32                    (BYTE )0xE8
#define JMP_REL32                     (BYTE )0xE9
#define JMP_REL8                      (BYTE )0xEB

extern LPSTR lpSSGVersion;

EXTERN_C void Attach_Parsing()
{
	// TSSGCtrl::GetVersion
	*(LPDWORD)(0x0041583D + 1) = (DWORD)lpSSGVersion;

	// TSSBitList::Read
	*(LPBYTE )0x004B905C = PUSH_IMM8;
	*(LPBYTE )0x004B905D = 0x80;
	*(LPBYTE )0x004B905E = CALL_REL32;
	*(LPDWORD)0x004B905F = (DWORD)Caller_TSSGCtrl_Open - (0x004B905F + sizeof(DWORD));
	*(LPBYTE )0x004B9063 = NOP;

	// TSSBitList::Write
	*(LPBYTE )0x004BAC5D = PUSH_IMM8;
	*(LPBYTE )0x004BAC5E = 0x80;
	*(LPBYTE )0x004BAC5F = CALL_REL32;
	*(LPDWORD)0x004BAC60 = (DWORD)Caller_TSSGCtrl_Open - (0x004BAC60 + sizeof(DWORD));
	*(LPBYTE )0x004BAC64 = NOP;

	// TSSBundleCalc::Read
	*(LPBYTE )0x004BD6BF = JMP_REL8;

	*(LPBYTE )0x004BDC69 = JMP_REL8;
	*(LPBYTE )0x004BDC6A = 0x4C;
	*(LPBYTE )0x004BDC6B = NOP;

	*(LPDWORD)(0x004BDCD4 + 1) = (DWORD)TSSBundleCalc_Read_Read - (0x004BDCD4 + 1 + sizeof(DWORD));

	// TSSBundleCalc::Write
	*(LPBYTE )0x004BE004 = JMP_REL8;

	*(LPBYTE )0x004BE50D = JMP_REL8;
	*(LPBYTE )0x004BE50E = 0x4C;
	*(LPBYTE )0x004BE50F = NOP;

	*(LPDWORD)(0x004BE578 + 1) = (DWORD)TSSBundleCalc_Write_Write - (0x004BE578 + 1 + sizeof(DWORD));

	// TSSBundleList::Read
	*(LPBYTE )0x004BF39E = JMP_REL8;

	*(LPWORD )0x004BF572 = LEA_EAX_EDI_MUL3;
	*(LPBYTE )0x004BF574 = LEA_EAX_EDI_MUL3_3RD_BYTE;
	*(LPWORD )0x004BF575 = SHL_EAX_IMM8;
	*(LPBYTE )0x004BF577 = 0x03;
	*(LPWORD )0x004BF578 = ADD_EAX_DWORD_PTR_EBX;
	*(LPWORD )0x004BF57A = MOV_EAX_TO_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004BF57C = (char)-0x50;

	*(LPDWORD)(0x004BF5A8 + 1) = (DWORD)TSSBundleList_Read_OneRead - (0x004BF5A8 + 1 + sizeof(DWORD));

	// TSSBundleList::Write
	*(LPBYTE )0x004BF6B5 = JMP_REL8;

	*(LPWORD )0x004BF863 = LEA_EAX_EDI_MUL3;
	*(LPBYTE )0x004BF865 = LEA_EAX_EDI_MUL3_3RD_BYTE;
	*(LPWORD )0x004BF866 = SHL_EAX_IMM8;
	*(LPBYTE )0x004BF868 = 0x03;
	*(LPWORD )0x004BF869 = ADD_EAX_DWORD_PTR_EBX;
	*(LPWORD )0x004BF86B = MOV_EAX_TO_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004BF86D = (char)-0x4C;

	*(LPDWORD)(0x004BF899 + 1) = (DWORD)TSSBundleList_Write_OneWrite - (0x004BF899 + 1 + sizeof(DWORD));

	// TSSBundleToggle::Read
	*(LPBYTE )0x004C018A = JMP_REL8;

	*(LPDWORD)(0x004C0745 + 1) = (DWORD)TSSBundleToggle_Read_Compare - (0x004C0745 + 1 + sizeof(DWORD));

	// TSSBundleToggle::Write
	*(LPBYTE )0x004C0B86 = JMP_REL8;

	*(LPDWORD)(0x004C10D3 + 1) = (DWORD)TSSBundleToggle_Write_Write - (0x004C10D3 + 1 + sizeof(DWORD));

	// TSSCalc::Read
	*(LPBYTE )0x004C1E18 = PUSH_IMM8;
	*(LPBYTE )0x004C1E19 = 0x88;
	*(LPBYTE )0x004C1E1A = CALL_REL32;
	*(LPDWORD)0x004C1E1B = (DWORD)Caller_TSSGCtrl_Open - (0x004C1E1B + sizeof(DWORD));
	*(LPBYTE )0x004C1E1F = NOP;

	// TSSCalc::Write
	*(LPBYTE )0x004C1FD8 = PUSH_IMM8;
	*(LPBYTE )0x004C1FD9 = 0x88;
	*(LPBYTE )0x004C1FDA = CALL_REL32;
	*(LPDWORD)0x004C1FDB = (DWORD)Caller_TSSGCtrl_Open - (0x004C1FDB + sizeof(DWORD));
	*(LPBYTE )0x004C1FDF = NOP;

	// TSSCopy::Write
	*(LPBYTE )0x004C2AE1 = JMP_REL8;

	*(LPDWORD)(0x004C2B8C + 1) = (DWORD)TSSCopy_Write_OneRead - (0x004C2B8C + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004C2C13 + 1) = (DWORD)TSSCopy_Write_OneWrite - (0x004C2C13 + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004C2C63 + 1) = (DWORD)TSSCopy_Write_OneWrite - (0x004C2C63 + 1 + sizeof(DWORD));

	// TSSDoubleList::Read
	*(LPBYTE )0x004C41A3 = JMP_REL8;

	*(LPDWORD)(0x004C42F5 + 1) = (DWORD)TSSDoubleList_Read_OneRead - (0x004C42F5 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C4617 = JMP_REL8;
	*(LPBYTE )0x004C4618 = 0x4C;
	*(LPBYTE )0x004C4619 = NOP;

	*(LPDWORD)(0x004C482D + 1) = (DWORD)TSSDoubleList_Read_Compare1 - (0x004C482D + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C4E08 = JMP_REL8;
	*(LPBYTE )0x004C4E09 = 0x56;
	*(LPDWORD)0x004C4E0A = NOP_X4;

	*(LPDWORD)(0x004C5034 + 1) = (DWORD)TSSDoubleList_Read_Compare2 - (0x004C5034 + 1 + sizeof(DWORD));

	// TSSDoubleList::Write
	*(LPBYTE )0x004C52EB = JMP_REL8;

	*(LPDWORD)(0x004C54FD + 1) = (DWORD)TSSDoubleList_Write_OneWrite - (0x004C54FD + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C5717 = JMP_REL8;
	*(LPBYTE )0x004C5718 = 0x4C;
	*(LPBYTE )0x004C5719 = NOP;

	*(LPDWORD)(0x004C592D + 1) = (DWORD)TSSDoubleList_Write_Write1 - (0x004C592D + 1 + sizeof(DWORD));

	*(LPBYTE )0x004C5EAF = JMP_REL8;
	*(LPBYTE )0x004C5EB0 = 0x56;
	*(LPDWORD)0x004C5EB1 = NOP_X4;

	*(LPDWORD)(0x004C60D1 + 1) = (DWORD)TSSDoubleList_Write_Write2 - (0x004C60D1 + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Read
	*(LPBYTE )0x004C85FB = JMP_REL8;

	*(LPBYTE )0x004C9AF3 = JMP_REL8;
	*(LPBYTE )0x004C9AF4 = 0x56;
	*(LPDWORD)0x004C9AF5 = NOP_X4;

	*(LPDWORD)(0x004C9D8E + 1) = (DWORD)TSSDoubleToggle_Read_Compare1 - (0x004C9D8E + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004CA540 + 1) = (DWORD)TSSDoubleToggle_Read_Compare2 - (0x004CA540 + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Write
	*(LPBYTE )0x004CAE89 = JMP_REL8;

	*(LPBYTE )0x004CC49C = JMP_REL8;
	*(LPBYTE )0x004CC49D = 0x3F;
	*(LPDWORD)0x004CC49E = NOP_X4;

	*(LPDWORD)(0x004CC708 + 1) = (DWORD)TSSDoubleToggle_Write_Write1 - (0x004CC708 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004CCA6A = JMP_REL8;
	*(LPBYTE )0x004CCA6B = 0x3F;
	*(LPDWORD)0x004CCA6C = NOP_X4;

	*(LPDWORD)(0x004CD631 + 1) = (DWORD)TSSDoubleToggle_Write_Write2 - (0x004CD631 + 1 + sizeof(DWORD));

	// TSSFloatCalc::Read
	*(LPBYTE )0x004CE32C = PUSH_IMM8;
	*(LPBYTE )0x004CE32D = 0x98;
	*(LPBYTE )0x004CE32E = CALL_REL32;
	*(LPDWORD)0x004CE32F = (DWORD)Caller_TSSGCtrl_Open - (0x004CE32F + sizeof(DWORD));
	*(LPBYTE )0x004CE333 = NOP;

	// TSSFloatCalc::Write
	*(LPBYTE )0x004CE504 = PUSH_IMM8;
	*(LPBYTE )0x004CE505 = 0x98;
	*(LPBYTE )0x004CE506 = CALL_REL32;
	*(LPDWORD)0x004CE507 = (DWORD)Caller_TSSGCtrl_Open - (0x004CE507 + sizeof(DWORD));
	*(LPBYTE )0x004CE50B = NOP;

	// TSSGCtrl::EnumReadSSG/replace
	*(LPBYTE )0x004E65D7 = CALL_REL32;
	*(LPDWORD)0x004E65D8 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E65D8 + sizeof(DWORD));
	*(LPBYTE )0x004E65DC = NOP;

	// TSSGCtrl::EnumReadSSG/size
	*(LPBYTE )0x004E687B = JMP_REL32;
	*(LPDWORD)0x004E687C = (DWORD)TSSGCtrl_EnumReadSSG_size_ReplaceDefine - (0x004E687C + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG/adjustment
	*(LPBYTE )0x004E69B1 = JMP_REL32;
	*(LPDWORD)0x004E69B2 = (DWORD)TSSGCtrl_EnumReadSSG_adjustment_ReplaceDefine - (0x004E69B2 + sizeof(DWORD));
	*(LPBYTE )0x004E69B6 = NOP;

	// TSSGCtrl::EnumReadSSG/funnel
	*(LPBYTE )0x004E6EAB = CALL_REL32;
	*(LPDWORD)0x004E6EAC = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E6EAC + sizeof(DWORD));
	*(LPBYTE )0x004E6EB0 = NOP;

	// TSSGCtrl::EnumReadSSG/repeat
	*(LPBYTE )0x004E70B3 = JMP_REL32;
	*(LPDWORD)0x004E70B4 = (DWORD)TSSGCtrl_EnumReadSSG_repeat_ReplaceDefine - (0x004E70B4 + sizeof(DWORD));
	*(LPBYTE )0x004E70B8 = NOP;

	// TSSGCtrl::EnumReadSSG/io_fep
	*(LPBYTE )0x004E746F = CALL_REL32;
	*(LPDWORD)0x004E7470 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E7470 + sizeof(DWORD));
	*(LPBYTE )0x004E7474 = NOP;

	// TSSGCtrl::EnumReadSSG/e_with
	*(LPBYTE )0x004E773F = CALL_REL32;
	*(LPDWORD)0x004E7740 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E7740 + sizeof(DWORD));
	*(LPBYTE )0x004E7744 = NOP;

	// TSSGCtrl::EnumReadSSG/enabled
	*(LPBYTE )0x004E79C6 = CALL_REL32;
	*(LPDWORD)0x004E79C7 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E79C7 + sizeof(DWORD));
	*(LPBYTE )0x004E79CB = NOP;

	// TSSGCtrl::EnumReadSSG/adjust_check
	*(LPBYTE )0x004E7C4C = CALL_REL32;
	*(LPDWORD)0x004E7C4D = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E7C4D + sizeof(DWORD));
	*(LPBYTE )0x004E7C51 = NOP;

	// TSSGCtrl::EnumReadSSG/child_rw
	*(LPBYTE )0x004E7E31 = CALL_REL32;
	*(LPDWORD)0x004E7E32 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E7E32 + sizeof(DWORD));
	*(LPBYTE )0x004E7E36 = NOP;

	// TSSGCtrl::EnumReadSSG/caution
	*(LPBYTE )0x004E8133 = CALL_REL32;
	*(LPDWORD)0x004E8134 = (DWORD)TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 - (0x004E8134 + sizeof(DWORD));
	*(LPBYTE )0x004E8138 = NOP;

	// TSSGCtrl::EnumReadSSG/involve
	*(LPBYTE )0x004E84D5 = JMP_REL32;
	*(LPDWORD)0x004E84D6 = (DWORD)TSSGCtrl_EnumReadSSG_involve_ReplaceDefine - (0x004E84D6 + sizeof(DWORD));
	*(LPBYTE )0x004E84DA = NOP;

	// TSSGCtrl::EnumReadSSG/note
	*(LPBYTE )0x004E89FD = JMP_REL32;
	*(LPDWORD)0x004E89FE = (DWORD)TSSGCtrl_EnumReadSSG_note_ReplaceDefine - (0x004E89FE + sizeof(DWORD));
	*(LPBYTE )0x004E8A02 = NOP;

	// TSSGCtrl::EnumReadSSG/process
	*(LPBYTE )0x004E913E = JMP_REL32;
	*(LPDWORD)0x004E913F = (DWORD)TSSGCtrl_EnumReadSSG_process_ReplaceDefine - (0x004E913F + sizeof(DWORD));
	*(LPBYTE )0x004E9143 = NOP;

	// TSSGCtrl::EnumReadSSG/title
	*(LPBYTE )0x004E91FD = JMP_REL32;
	*(LPDWORD)0x004E91FE = (DWORD)TSSGCtrl_EnumReadSSG_title_ReplaceDefine - (0x004E91FE + sizeof(DWORD));
	*(LPDWORD)0x004E9202 = NOP_X4;

	// TSSGCtrl::EnumReadSSG/maker
	*(LPBYTE )0x004E969B = JMP_REL32;
	*(LPDWORD)0x004E969C = (DWORD)TSSGCtrl_EnumReadSSG_maker_ReplaceDefine - (0x004E969C + sizeof(DWORD));
	*(LPDWORD)0x004E96A0 = NOP_X4;

	// TSSGCtrl::EnumReadSSG/creator
	*(LPBYTE )0x004E9B3A = JMP_REL32;
	*(LPDWORD)0x004E9B3B = (DWORD)TSSGCtrl_EnumReadSSG_creator_ReplaceDefine - (0x004E9B3B + sizeof(DWORD));
	*(LPDWORD)0x004E9B3F = NOP_X4;

	// TSSGCtrl::EnumReadSSG/distinction
	*(LPBYTE )0x004E9FD9 = JMP_REL32;
	*(LPDWORD)0x004E9FDA = (DWORD)TSSGCtrl_EnumReadSSG_distinction_ReplaceDefine - (0x004E9FDA + sizeof(DWORD));
	*(LPBYTE )0x004E9FDE = NOP;

	// TSSGCtrl::EnumReadSSG/ADJSubjectSet
	*(LPBYTE )0x004EA5EA = JMP_REL32;
	*(LPDWORD)0x004EA5EB = (DWORD)TSSGCtrl_EnumReadSSG_ADJSubjectSet_ReplaceDefine - (0x004EA5EB + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG/val
	*(LPBYTE )0x004EB45E = JMP_REL32;
	*(LPDWORD)0x004EB45F = (DWORD)TSSGCtrl_EnumReadSSG_val_ReplaceDefine - (0x004EB45F + sizeof(DWORD));
	*(LPDWORD)0x004EB463 = NOP_X4;

	// TSSGCtrl::GetSSGDataFile
	*(LPBYTE )0x004EE3C1 = CALL_REL32;
	*(LPDWORD)0x004EE3C2 = (DWORD)TSSGCtrl_GetSSGDataFile_OpenProcess - (0x004EE3C2 + sizeof(DWORD));
	*(LPBYTE )0x004EE3C6 = NOP;

	*(LPBYTE )0x004EE58A = JMP_REL8;
	*(LPBYTE )0x004EE58B = 0x004EE5C6 - (0x004EE58B + sizeof(BYTE));
	*(LPDWORD)0x004EE58C = NOP_X4;

	// TSSGCtrl::ReadSSRFile
	*(LPDWORD)(0x004FF37F + 1) = (DWORD)TSSGCtrl_ReadSSRFile_ReplaceDefine - (0x004FF37F + 1 + sizeof(DWORD));

	// TSSGCtrl::SeekScriptIndex
	*(LPDWORD)(0x004FD1FB + 1) = (DWORD)Caller_CheckSSGVersion - (0x004FD1FB + 1 + sizeof(DWORD));

	// TSSGCtrl::LoopSSRFile
	*(LPDWORD)(0x00501DC4 + 1) = (DWORD)Caller_ParsingWithVal - (0x00501DC4 + 1 + sizeof(DWORD));

	// TSSGCtrl::LoopSSRFile
	*(LPBYTE )0x005021B5 = CALL_REL32;
	*(LPDWORD)0x005021B6 = (DWORD)TSSGCtrl_LoopSSRFile_ReplaceDefine - (0x005021B6 + sizeof(DWORD));
	*(LPBYTE )0x005021BA = NOP;

	*(LPBYTE )0x005021CC = CALL_REL32;
	*(LPDWORD)0x005021CD = (DWORD)TSSGCtrl_LoopSSRFile_ReplaceDefine_Release - (0x005021CD + sizeof(DWORD));
	*(LPBYTE )0x005021D1 = NOP;

	// TSSGCtrl::LoopSSRFile
	*(LPDWORD)(0x0050212C + 1) = (DWORD)Caller_ParsingWithVal - (0x0050212C + 1 + sizeof(DWORD));

	// TSSGCtrl::LoopSSRFile
	*(LPDWORD)(0x005021F9 + 1) = (DWORD)Caller_ParsingWithVal - (0x005021F9 + 1 + sizeof(DWORD));

	// TSSGCtrl::GetAddress
	*(LPDWORD)(0x00503A87 + 1) = (DWORD)Caller_Parsing - (0x00503A87 + 1 + sizeof(DWORD));

	// TSSGCtrl::GetAddress
	*(LPDWORD)(0x00503CC7 + 1) = (DWORD)Caller_Parsing - (0x00503CC7 + 1 + sizeof(DWORD));

	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x0050490D + 1) = (DWORD)TSSGCtrl_AddressNaming_ReplaceDefine1 - (0x0050490D + 1 + sizeof(DWORD));

	// TSSGCtrl::AddressNaming
	*(LPBYTE )0x00504F65 = JMP_REL32;
	*(LPDWORD)0x00504F66 = (DWORD)TSSGCtrl_AddressNaming_OpenProcess - (0x00504F66 + sizeof(DWORD));
	*(LPWORD )0x00504F6A = NOP_X2;
	*(LPBYTE )0x00504F6C = NOP;

	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x00505703 + 1) = (DWORD)TSSGCtrl_AddressNaming_ReplaceDefine2 - (0x00505703 + 1 + sizeof(DWORD));

	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x005059A8 + 1) = (DWORD)TSSGCtrl_AddressNaming_ReplaceDefine3 - (0x005059A8 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )0x00507170 = CALL_REL32;
	*(LPDWORD)0x00507171 = (DWORD)TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine - (0x00507171 + sizeof(DWORD));
	*(LPBYTE )0x00507175 = NOP;
	*(LPWORD )0x00507176 = CMP_AL_IMM8;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050B512 + 1) = (DWORD)Caller_Parsing - (0x0050B512 + 1 + sizeof(DWORD));

	// TSSGCtrl::Funneling
	*(LPBYTE )0x00510429 = JMP_REL32;
	*(LPDWORD)0x0051042A = (DWORD)TSSGCtrl_Funneling_ReplaceDefine - (0x0051042A + sizeof(DWORD));

	// TSSGCtrl::Funneling
	*(LPDWORD)(0x00510A8D + 1) = (DWORD)Caller_ParsingWithVal - (0x00510A8D + 1 + sizeof(DWORD));

	// TSSGCtrl::Funneling
	*(LPDWORD)(0x00510C69 + 1) = (DWORD)TSSGCtrl_Funneling_Write - (0x00510C69 + 1 + sizeof(DWORD));

	// TSSGCtrl::CheckIO_FEP
	*(LPDWORD)(0x00510FE6 + 1) = (DWORD)Caller_ParsingWithVal - (0x00510FE6 + 1 + sizeof(DWORD));

	// TSSGCtrl::IsEnabled
	*(LPBYTE )0x005111E0 = JMP_REL32;
	*(LPDWORD)0x005111E1 = (DWORD)Caller_TSSGCtrl_IsEnabled_except1 - (0x005111E1 + sizeof(DWORD));
	*(LPBYTE )0x005111E5 = NOP;

	// TSSGCtrl::IsEnabled
	*(LPDWORD)(0x0051125C + 1) = (DWORD)Caller_Parsing - (0x0051125C + 1 + sizeof(DWORD));

	// TSSGCtrl::ParsingDouble
	*(LPBYTE )0x005174C0 = JMP_REL32;
	*(LPDWORD)0x005174C1 = (DWORD)Caller_ParsingDouble - (0x005174C1 + sizeof(DWORD));
	*(LPDWORD)0x005174C5 = NOP_X4;

	// TSSList::Read
	*(LPBYTE )0x0052A07C = PUSH_IMM8;
	*(LPBYTE )0x0052A07D = 0x88;
	*(LPBYTE )0x0052A07E = CALL_REL32;
	*(LPDWORD)0x0052A07F = (DWORD)Caller_TSSGCtrl_Open - (0x0052A07F + sizeof(DWORD));
	*(LPBYTE )0x0052A083 = NOP;

	// TSSList::Write
	*(LPBYTE )0x0052A220 = PUSH_IMM8;
	*(LPBYTE )0x0052A221 = 0x88;
	*(LPBYTE )0x0052A222 = CALL_REL32;
	*(LPDWORD)0x0052A223 = (DWORD)Caller_TSSGCtrl_Open - (0x0052A223 + sizeof(DWORD));
	*(LPBYTE )0x0052A227 = NOP;

	// TSSString::Read
	*(LPBYTE )0x0052AF1A = PUSH_IMM8;
	*(LPBYTE )0x0052AF1B = 0x80;
	*(LPBYTE )0x0052AF1C = CALL_REL32;
	*(LPDWORD)0x0052AF1D = (DWORD)Caller_TSSGCtrl_Open - (0x0052AF1D + sizeof(DWORD));
	*(LPBYTE )0x0052AF21 = NOP;

	// TSSString::Write
	*(LPBYTE )0x0052B212 = PUSH_IMM8;
	*(LPBYTE )0x0052B213 = 0x80;
	*(LPBYTE )0x0052B214 = CALL_REL32;
	*(LPDWORD)0x0052B215 = (DWORD)Caller_TSSGCtrl_Open - (0x0052B215 + sizeof(DWORD));
	*(LPBYTE )0x0052B219 = NOP;

	// TSSToggle::Read
	*(LPBYTE )0x0052C42B = PUSH_IMM8;
	*(LPBYTE )0x0052C42C = 0x78;
	*(LPBYTE )0x0052C42D = CALL_REL32;
	*(LPDWORD)0x0052C42E = (DWORD)Caller_TSSGCtrl_Open - (0x0052C42E + sizeof(DWORD));
	*(LPBYTE )0x0052C432 = NOP;

	// TSSToggle::Write
	*(LPBYTE )0x0052C70B = PUSH_IMM8;
	*(LPBYTE )0x0052C70C = 0x78;
	*(LPBYTE )0x0052C70D = CALL_REL32;
	*(LPDWORD)0x0052C70E = (DWORD)Caller_TSSGCtrl_Open - (0x0052C70E + sizeof(DWORD));
	*(LPBYTE )0x0052C712 = NOP;

	// TSSBundleFloatCalc::Read
	*(LPBYTE )0x0052DCF3 = JMP_REL8;

	*(LPBYTE )0x0052E2A9 = JMP_REL8;
	*(LPBYTE )0x0052E2AA = 0x4C;
	*(LPBYTE )0x0052E2AB = NOP;

	*(LPDWORD)(0x0052E314 + 1) = (DWORD)TSSBundleFloatCalc_Read_Read - (0x0052E314 + 1 + sizeof(DWORD));

	// TSSBundleFloatCalc::Write
	*(LPBYTE )0x0052E6AB = JMP_REL8;

	*(LPBYTE )0x0052E8C4 = JMP_REL8;
	*(LPBYTE )0x0052E8C5 = 0x4C;
	*(LPBYTE )0x0052E8C6 = NOP;

	*(LPDWORD)(0x0052E935 + 1) = (DWORD)TSSBundleFloatCalc_Write_OneWrite1 - (0x0052E935 + 1 + sizeof(DWORD));

	*(LPBYTE )0x0052EA46 = JMP_REL8;
	*(LPBYTE )0x0052EA47 = 0x4C;
	*(LPBYTE )0x0052EA48 = NOP;

	*(LPDWORD)(0x0052EAB7 + 1) = (DWORD)TSSBundleFloatCalc_Write_OneWrite2 - (0x0052EAB7 + 1 + sizeof(DWORD));

	// TSSGCtrl::GetAddress
	*(LPBYTE )0x00503F9F = JMP_REL32;
	*(LPDWORD)0x00503FA0 = (DWORD)Caller_TSSGCtrl_GetAddress_SaveAddress - (0x00503FA0 + sizeof(DWORD));
}
