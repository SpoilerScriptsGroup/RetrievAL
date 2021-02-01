#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "ToolTip/ToolTip.h"

EXTERN_C DWORD_DWORD __fastcall TSSGCtrl_ReadSSRFile_CheckSignedParam(void *, void *, void *, void *, void *);
EXTERN_C long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(BOOL);
EXTERN_C void __fastcall TSSGCtrl_ReadSSRFile_CompareLoopCounter(BOOL);
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0();
EXTERN_C void*__fastcall TSSGCtrl_LoopSSRFile_FixWordRepeat(void *, void *, unsigned, unsigned);
EXTERN_C void*__stdcall TSSGCtrl_LoopSSRFile_switch_Type(signed, void *, unsigned, void *);
EXTERN_C unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);

#if USE_TOOLTIP
extern BOOL bActive;
#else
#define bActive FALSE
#endif

static unsigned long __fastcall TSSGCtrl_ReadSSRFile_Parsing(
	unsigned long *const End,
	unsigned long *const Begin,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGC,
	string  const *const Code,
	TSSGSubject   *const SSGS)
{
	unsigned long   Step;
	bcb6_std_string *end = &vector_at(tmpV, 2);
	if (!SSGC->script.ePos)
	{
		*Begin = TStringDivision_ToULongDef(&vector_at(tmpV, 1), 0);
		*End   = TStringDivision_ToULongDef(&vector_at(tmpV, 2), 0);
		Step   = TStringDivision_ToULongDef(&vector_at(tmpV, 3), 1);
	}
	else
	{
		DWORD dwErrCode;
		SetLastError(NO_ERROR);
		*Begin = Parsing(SSGC, SSGS, &vector_at(tmpV, 1), 0);
		*End   = Parsing(SSGC, SSGS, &vector_at(tmpV, 2), 0);
		Step   = Parsing(SSGC, SSGS, &vector_at(tmpV, 3), 0);
		if (!Step) Step = 1;
		if ((dwErrCode = GetLastError()) && dwErrCode != ERROR_NO_MORE_FILES)
		{
			LPSTR lpBuffer;
			*Begin = 0;
			*End   = 0;
			if (!bActive && FormatMessageA(
				FORMAT_MESSAGE_MAX_WIDTH_MASK |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dwErrCode,
				0,
				(LPSTR)&lpBuffer,
				sizeof(double),
				NULL))
			{
#if USE_TOOLTIP
				ShowToolTip(string_c_str(Code), (HICON)lpBuffer);
#else
				if (TMainForm_GetUserMode(MainForm) != 1)
					TMainForm_Guide(lpBuffer, 0);
#endif
				LocalFree(lpBuffer);
			}
		} 
	}
	if (string_empty (end) ||
		string_length(end) == 1 && string_at(end, 0) == '_')
	{
		*End   = *Begin;
		*Begin = 0;
	}
	return Step;
}

static DWORD_DWORD __stdcall TSSGCtrl_ReadSSRFile_GetSSGDataFile_StepFile(
	TSSGCtrl      *const this,
	vector_string *const tmpV,
	vector_string *const StepFile
)
{
	LPSTR lpBuffer;
	DWORD const dwErrCode = GetLastError();
	if (!bActive && this->script.ePos
		&& (!StepFile || dwErrCode && dwErrCode != ERROR_NO_MORE_FILES)
		&& FormatMessageA(
			FORMAT_MESSAGE_MAX_WIDTH_MASK |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwErrCode ? dwErrCode : ERROR_INVALID_TARGET_HANDLE,
			0,
			(LPSTR)&lpBuffer,
			sizeof(double),
			NULL))
	{
#if USE_TOOLTIP
		ShowToolTip(string_c_str(&vector_at(tmpV, 1)), (HICON)lpBuffer);
#else
		if (TMainForm_GetUserMode(MainForm) != 1)
			TMainForm_Guide(lpBuffer, 0);
#endif
		LocalFree(lpBuffer);
	}
	if (!StepFile) string_clear(&vector_at(tmpV, 1));// prevent Parsing
	return (DWORD_DWORD) { (DWORD)StepFile, !StepFile };// edx isn't 0 means set Begin, End, Step to 0, 0, 1
}

static vector_string* __fastcall TSSGCtrl_ReadSSRFile_return_Data(vector_string *const retVal, vector_string *const Data)
{
	*retVal = *Data;
	vector_ctor(Data);
	return retVal;
}

static char __fastcall TSSGCtrl_EnumReadSSR_switch_tmpS(
	list     *const LineList,
	string   *const tmpS,
	TSSGCtrl *const SSGC,
	vector   *const FormatVec)
{
	string     Half, Trim;
	if (string_length(tmpS) == 2 && *(LPWORD)string_begin(tmpS) == BSWAP16('!]')) return '\0';
	char const Char = string_at(TStringDivision_Half_WithoutTokenDtor(&Half, &SSGC->strD, tmpS, ",", 1u, 0, dtNEST | etTRIM), 0);
	if (Char != ',')
	{
		Trim._M_start = string_begin(&Half) + 1;
		while (__intrinsic_isblank(*Trim._M_start)) ++Trim._M_start;
		Trim._M_finish = string_end(&Half);
		Trim._M_end_of_storage = Trim._M_start;// Non-allocated mark.
	}
	switch (Char)
	{
	default:
		list_push_back(LineList, &((struct pair_byte_string) { rtSTRING,  Half }));
		/* FALLTHROUGH */
	case ',':
		string_dtor(&Half);
		return string_at(tmpS, 0);
	case '*':
	case '+':
	case '@':
		list_push_back(LineList, &((struct pair_byte_string) { rtFILE  ,  Char != '@' ? Half : Trim }));
		list_push_back(LineList, &((struct pair_byte_string) { rtNUMBER, *tmpS }));
		break;
	case 'F':
		list_push_back(LineList, &((struct pair_byte_string) { rtFORMAT,  Trim }));
		list_push_back(LineList, &((struct pair_byte_string) { rtSTRING, *tmpS }));
		break;
	case 'R':
		list_push_back(LineList, &((struct pair_byte_string) { rtWORD_REPEAT,  Trim }));
		list_push_back(LineList, &((struct pair_byte_string) { rtSTRING     , *tmpS }));
		break;
	}
	string_dtor(&Half);
	return '\0';// means processed
}

static string* __cdecl TSSGCtrl_EnumReadSSR_tmpS_L_substr(
	string       *const substr,
	string const *const self,
	size_t        const __pos,
	size_t        const __n)
{
	*substr = *self;
	string_end_of_storage(substr) = string_begin(substr) += __pos;// Non-allocated mark.
	return substr;
}

#define CMP_AL_IMM8 (BYTE)0x3C
#define PUSH_EAX    (BYTE)0x50
#define PUSH_EDX    (BYTE)0x52
#define PUSH_ESI    (BYTE)0x56
#define PUSH_EDI    (BYTE)0x57
#define POP_EDX     (BYTE)0x5A
#define POP_EBX     (BYTE)0x5B
#define POP_EBP     (BYTE)0x5D
#define POP_ESI     (BYTE)0x5E
#define POP_EDI     (BYTE)0x5F
#define PUSH_IMM8   (BYTE)0x6A
#define MOV_ECX_ESI (WORD)0xCE8B
#define MOV_ESP_EBP (WORD)0xE58B
#define NOP         (BYTE)0x90
#define NOP_X2      (WORD)0x9066
#define NOP_X4     (DWORD)0x00401F0F 
#define NOP_X8            0x0000000000841F0Full
#define RET         (BYTE)0xC3
#define SS_LEAVE    (WORD)0xC936
#define JE_SHORT    (BYTE)0x74
#define JECXZ       (BYTE)0xE3
#define CALL_REL32  (BYTE)0xE8
#define JMP_REL32   (BYTE)0xE9
#define JMP_REL8    (BYTE)0xEB

EXTERN_C void __cdecl Attach_FixRepeat()
{
	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FEB85 = dtNEST | etTRIM;

	// TSSGCtrl::ReadSSRFile
	//   tmpV.resize(4,"_");
	*(LPWORD )0x004FEBEC = BSWAP16(0x8B55);// mov edx,
	*(LPBYTE )0x004FEBEE =         0x18   ;// dword ptr [SSGS]
	*(LPWORD )0x004FEBF4 = MOV_ECX_ESI;
	*(LPBYTE )0x004FEBF6 = CALL_REL32;
	*(LPDWORD)0x004FEBF7 = (DWORD)TSSGCtrl_ReadSSRFile_CheckSignedParam - (0x004FEBF7 + sizeof(DWORD));
	*(LPBYTE )0x004FEBFB = PUSH_EDX;

	// TSSGCtrl::ReadSSRFile
	//   refer to extra argument "SSGS"
	*(LPBYTE )(0x004FED74 + 0) = 0x81;
	*(LPBYTE )(0x004FED74 + 3) = 0x00;
	*(LPWORD )(0x004FED74 + 4) = 0x0000;
	*(LPWORD )(0x004FED7A + 0) = BSWAP16(0xFF75);
	*(LPBYTE )(0x004FED7A + 2) = 0x18;// push dword ptr [ebp + ...]

	*(LPBYTE )(0x004FED84 + 1) = 0x6B;
	*(LPBYTE )(0x004FED84 + 3) = 0x04;

	// TSSGCtrl::ReadSSRFile
	//   SetLastError(0) before GetSSGDataFile
	*(LPBYTE )(0x004FF0A5 + 1) = 0x004FF123 - (0x004FF0A5 + 1 + sizeof(BYTE));
	*(LPBYTE )(0x004FF0A9 + 0) = PUSH_IMM8;
	*(LPBYTE )(0x004FF0A9 + 1) = 0;
	*(LPBYTE )(0x004FF0AB + 0) = CALL_REL32;
	*(LPDWORD)(0x004FF0AB + 1) = (DWORD)SetLastError - (0x004FF0AB + 1 + sizeof(DWORD));
	*(LPDWORD) 0x004FF0B0      = NOP_X4;

	// TSSGCtrl::ReadSSRFile
	//   refer to extra argument "SSGS"
	*(LPBYTE )(0x004FF0F2 + 0) = 0x81;
	*(LPBYTE )(0x004FF0F2 + 3) = 0x00;
	*(LPWORD )(0x004FF0F2 + 4) = 0x0000;
	*(LPWORD )(0x004FF0F8 + 0) = BSWAP16(0xFF75);
	*(LPBYTE )(0x004FF0F8 + 2) = 0x18;// push dword ptr [ebp + ...]

	*(LPBYTE )(0x004FF102 + 1) = 0x6B;
	*(LPBYTE )(0x004FF102 + 3) = 0x04;

	// TSSGCtrl::ReadSSRFile
	//   after GetSSGDataFile
	*(LPBYTE )0x004FF114 = PUSH_EAX;
	*(LPBYTE )0x004FF115 = PUSH_ESI;
	*(LPBYTE )0x004FF116 = PUSH_EDI;
	*(LPBYTE )0x004FF117 = CALL_REL32;
	*(LPDWORD)0x004FF118 = (DWORD)TSSGCtrl_ReadSSRFile_GetSSGDataFile_StepFile -(0x004FF118 + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF123 = JMP_REL8;
	*(LPBYTE )0x004FF124 = 0x004FF149 - (0x004FF124 + sizeof(BYTE));

	*(LPBYTE )0x004FF149 =         0xFF       ;// push dword ptr [SSGS]
	*(LPWORD )0x004FF14A = BSWAP16(0x7518    );// push dword ptr [Code]
	*(LPDWORD)0x004FF14C = BSWAP32(0xFF751057);// push this
	*(LPDWORD)0x004FF150 = BSWAP32(0x568D9510);// push tmpV
	*(LPDWORD)0x004FF154 = BSWAP32(0xFFFFFF8D);// lea  edx, [Begin]
	*(LPWORD )0x004FF158 = BSWAP16(0x4AFC    );// lea  ecx, [Begin - 4]
	*(LPBYTE )0x004FF15A = CALL_REL32;
	*(LPDWORD)0x004FF15B = (DWORD)TSSGCtrl_ReadSSRFile_Parsing - (0x004FF15B + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPDWORD)0x004FF2B8 = BSWAP32(0x8B0C24 << 8 | CALL_REL32);// mov ecx, [esp]
	*(LPDWORD)0x004FF2BC = (DWORD)TSSGCtrl_ReadSSRFile_DestReserve - (0x004FF2BC + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF2E5 = JMP_REL32;
	*(LPDWORD)0x004FF2E6 = 0x004FF309 - (0x004FF2E6 + sizeof(DWORD));
	*(LPWORD )0x004FF2EA = NOP_X2;
	*(LPBYTE )0x004FF2EC = JMP_REL32;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF309 =         0x8B   ;// mov
	*(LPWORD )0x004FF30A = BSWAP16(0x4C24);// ecx,
	*(LPBYTE )0x004FF30C =         0x00   ;// dword ptr [esp + 0x00]  
	*(LPBYTE )0x004FF30D = CALL_REL32;
	*(LPDWORD)0x004FF30E = (DWORD)TSSGCtrl_ReadSSRFile_CompareLoopCounter - (0x004FF30E + sizeof(DWORD));
	*(LPBYTE )0x004FF312 = JECXZ;
	*(LPBYTE )0x004FF313 = (BYTE)(0x004FF2F1 - (0x004FF313 + sizeof(BYTE)));
	*(LPBYTE )0x004FF314 = JMP_REL8;
	*(LPBYTE )0x004FF315 = (BYTE)(0x004FF2EC - (0x004FF315 + sizeof(BYTE)));

	// TSSGCtrl::ReadSSRFile
	*(LPWORD )(0x004FF3EC + 0) = BSWAP16(0x8D55);// lea edx, [ebp - 18h]
	*(LPWORD )(0x004FF3EC + 2) = BSWAP16(0xE8 << 8 | CALL_REL32);
	*(LPDWORD)(0x004FF3EF + 1) = (DWORD)TSSGCtrl_ReadSSRFile_return_Data - (0x004FF3EF + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004FF3F4 + 0) = JMP_REL32;
	*(LPDWORD)(0x004FF3F4 + 1) = 0x004FF478 - (0x004FF3F4 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004FF3F9 + 0) =         0x0F   ;// nop
	*(LPWORD )(0x004FF3F9 + 1) = BSWAP16(0x1F44);// dword ptr
	*(LPWORD )(0x004FF3F9 + 3) = BSWAP16(0x0000);// [eax + eax]

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF5E0 = POP_EDX;
	*(LPBYTE )0x004FF5E1 = POP_EDI;
	*(LPBYTE )0x004FF5E2 = POP_ESI;
	*(LPBYTE )0x004FF5E3 = POP_EBX;
	*(LPWORD )0x004FF5E4 = SS_LEAVE;

	// TSSGCtrl::EnumReadSSR
	//   File.size()*2 => File.size()
	*(LPWORD )0x004FF9F7 = NOP_X2;

	// TSSGCtrl::EnumReadSSR
#if 1
	*(LPWORD )0x004FFD9C = BSWAP16(0x836B);// sub dword ptr [ebx + ...]
	*(LPBYTE )0x004FFD9F = 3;
	*(LPDWORD)0x004FFDA0 = BSWAP32(0x8B45EC3B);// mov  eax, [ebp - 14h]
	*(LPWORD )0x004FFDA4 = BSWAP16(0x45E8    );// cmp  eax, [ebp - 18h]
	*(LPBYTE )0x004FFDA6 = JE_SHORT;
	*(LPBYTE )0x004FFDA7 = 0x004FFDD5 - (0x004FFDA7 + sizeof(BYTE));
	*(LPDWORD)0x004FFDA8 = BSWAP32(0xFF7510FF);// push dword ptr [FormatVec]  
	*(LPDWORD)0x004FFDAC = BSWAP32(0x75088D55);// push dword ptr [this]  
	*(LPDWORD)0x004FFDB0 = BSWAP32(0xE88D4DC0);// lea  edx,      [tmpS]
	*(LPBYTE )0x004FFDB4 = CALL_REL32         ;// lea  ecx,      [LineList]
	*(LPDWORD)0x004FFDB5 = (DWORD)TSSGCtrl_EnumReadSSR_switch_tmpS - (0x004FFDB5 + sizeof(DWORD));
	*(LPDWORD)0x004FFDC1 = 0x00500C13 - (0x004FFDC1 + sizeof(DWORD));
	*(LPBYTE )0x004FFDCD = CMP_AL_IMM8;
	*(LPBYTE )0x004FFDCE = -0x23 -0x1D -0x0C;// means '\0'
	*(LPBYTE )0x004FFDD0 = 0x85;// je => jne
	*(LPDWORD)0x004FFDD1 = 0x00500C13 - (0x004FFDD1 + sizeof(DWORD));
	*(LPDWORD)0x004FFDD6 = 0x00500DFF - (0x004FFDD6 + sizeof(DWORD));
#else
	*(LPBYTE )0x004FFDBD = JMP_REL32;
	*(LPDWORD)0x004FFDBE = (DWORD)TSSGCtrl_EnumReadSSR_SwitchTmpS_0 - (0x004FFDBE + sizeof(DWORD));
	*(LPWORD )0x004FFDC2 = NOP_X2;
	*(LPBYTE )0x004FFDC4 = NOP;
#endif

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)0x00500308 = (DWORD)TSSGCtrl_EnumReadSSR_tmpS_L_substr - (0x00500308 + sizeof(DWORD));
	*(LPBYTE )0x0050044A = JMP_REL8;// omit dtor substr

	// TSSGCtrl::LoopSSRFile
#if 1
	*(LPBYTE )0x00502211 =         0x8D   ;
	*(LPWORD )0x00502212 = BSWAP16(0x4D88);// lea  ecx,      [tmpS]
	*(LPWORD )0x00502214 = BSWAP16(0xFF75);// push dword ptr [LoopVal]
	*(LPBYTE )0x00502216 =         0x14   ;
	*(LPBYTE )0x00502217 = PUSH_ESI;
	*(LPBYTE )0x00502218 = CALL_REL32;
	*(LPDWORD)0x00502219 = (DWORD)TSSGCtrl_LoopSSRFile_FixWordRepeat - (0x00502219 + sizeof(DWORD));
	*(LPBYTE )0x0050221E = 0xC0;// esi => eax
	*(LPBYTE )0x00502220 = 0x85;// ecx => eax
#else
	*(LPBYTE )0x0050221D = JMP_REL32;
	*(LPDWORD)0x0050221E = (DWORD)TSSGCtrl_LoopSSRFile_FixWordRepeat - (0x0050221E + sizeof(DWORD));
	*(LPBYTE )0x00502222 = NOP;
#endif

	// TSSGCtrl::LoopSSRFile
#if 0
	*(LPBYTE )0x00502676 = JMP_REL32;
	*(LPDWORD)0x00502677 = (DWORD)TSSGCtrl_LoopSSRFile_Format - (0x00502677 + sizeof(DWORD));
	*(LPBYTE )0x0050267B = NOP;
#else
	*(LPWORD )0x00502676 = BSWAP16(0xFFB5    );// push dword ptr [ebp + ...]
	*(LPDWORD)0x0050267C = BSWAP32(0xFF7514FF);// push dword ptr [LoopVal]  
	*(LPWORD )0x00502680 = BSWAP16(0x7508    );// push dword ptr [this]
	*(LPBYTE )0x00502682 = PUSH_EDI;
	*(LPBYTE )0x00502683 = CALL_REL32;
	*(LPDWORD)0x00502684 = (DWORD)TSSGCtrl_LoopSSRFile_switch_Type - (0x00502684 + sizeof(DWORD));
	*(LPWORD )0x00502690 = BSWAP16(0x8985);// mov [ebp + ...], eax
	*(LPBYTE )0x00502697 =         0x85   ;// edx => eax
#endif
	*(UINT64*)0x005026A8 = NOP_X8;

	// TSSGCtrl::LoopSSRFile
	//   tmpS=""; => tmpS.clear();
	*(LPWORD )0x0050272A = BSWAP16(0x8B45);
	*(LPDWORD)0x0050272C = BSWAP32(0x88C60000);
	*(LPDWORD)0x00502730 = BSWAP32(0x89458C << 8 | JMP_REL8);
	*(LPBYTE )0x00502734 = 0x00502760 - (0x00502734 + sizeof(BYTE));
}
