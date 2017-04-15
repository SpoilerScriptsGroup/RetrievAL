#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <math.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"
#include "IsBadXxxPtr.h"

#if defined(__BORLANDC__)
EXTERN_C unsigned __int64 __cdecl _strtoui64(const char *nptr, char **endptr, int base);
#pragma warn -8004
#pragma warn -8060
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define _this                                                                          this
#define _ultoa                                                                         ultoa
#define fmodf                                                                          fmod
#define bcb6__isnan                                                                    _isnan
#define bcb6_std_string                                                                string
#define bcb6_std_string_c_str(s)                                                       (s)->c_str()
#define bcb6_std_string_begin(s)                                                       (s)->begin()
#define bcb6_std_string_end(s)                                                         (s)->end()
#define bcb6_std_string_length(s)                                                      (s)->length()
#define bcb6_std_vector_TSSGAttributeElement                                           vector<TSSGAttributeElement *>
#define TSSGCtrl_GetAttribute(SSGCtrl, SSGS, Type)                                     (SSGCtrl)->GetAttribute(SSGS, Type)
#define TSSGCtrl_GetSSGActionListner(SSGCtrl)                                          (SSGCtrl)->GetSSGActionListner()
#define TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, Address, Mode)                  (SSGCtrl)->AddressAttributeFilter(SSGS, Address, Mode)
#define TSSGSubject_GetAttribute(SSGS)                                                 (SSGS)->GetAttribute()
#define TSSGSubject_GetSize(SSGS)                                                      (SSGS)->GetSize()
#define TSSGActionListner_OnSubjectWriteError(SSGActionListner, SSGS, Address)         (SSGActionListner)->OnSubjectWriteError(SSGS, Address)
#define TSSGActionListner_OnParsingError(SSGActionListner, SSGS, Code)                 (SSGActionListner)->OnParsingError(SSGS, Code)
#define TSSGActionListner_OnParsingProcess(SSGActionListner, SSGS, Code, TopVal)       (SSGActionListner)->OnParsingProcess(SSGS, Code, TopVal)
#define TSSGActionListner_OnParsingDoubleProcess(SSGActionListner, SSGS, Code, TopVal) (SSGActionListner)->OnParsingDoubleProcess(SSGS, Code, TopVal)
#define TSSGAttributeElement_GetType(SSGAttributeElement)                              (SSGAttributeElement)->GetType()
#define TEndWithAttribute_GetCode(EndWithAttribute)                                    (string *)((LPBYTE)(EndWithAttribute) + 8)
#define TIO_FEPAttribute_GetInputCode(IO_FEPAttribute)                                 (string *)((LPBYTE)(IO_FEPAttribute) + 8)
#define TIO_FEPAttribute_GetOutputCode(IO_FEPAttribute)                                ((string *)((LPBYTE)(IO_FEPAttribute) + 8) + 1)
#define TProcessCtrl_Open(ProcessCtrl, Mode)                                           (ProcessCtrl)->Open(Mode)
#define TProcessCtrl_GetModuleFromName(ProcessCtrl, Name)                              (ProcessCtrl)->GetModuleFromName(Name)
#define TProcessCtrl_GetHeapList(ProcessCtrl, ListNo)                                  (ProcessCtrl)->GetHeapList(ListNo)
#else
#define ADDITIONAL_TAGS      1
#define ALLOCATE_SUPPORT     1
#define LOCAL_MEMORY_SUPPORT 1
#define REPEAT_INDEX         1
#define SUBJECT_STATUS       1
#include "bcb6_float.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGActionListner.h"
#include "THeapListData.h"
#include "TProcessCtrl.h"
#include "TEndWithAttribute.h"
#include "TIO_FEPAttribute.h"
#define bcb6_std_vector_TSSGAttributeElement bcb6_std_vector
EXTERN_C size_t __stdcall ReplaceDefineByHeap(bcb6_std_vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity);
#endif

EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName);
EXTERN_C FARPROC * __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName);
EXTERN_C LPVOID __stdcall GetSectionAddress(HANDLE hProcess, HMODULE hModule, LPCSTR lpSectionName, LPDWORD lpdwSectionSize);
EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString);
EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString);
EXTERN_C BOOLEAN __stdcall MoveProcessMemory(HANDLE hDestProcess, void *lpDest, HANDLE hSrcProcess, const void *lpSrc, size_t nSize);

#if defined(_MSC_VER)
#if defined(REPEAT_INDEX) && REPEAT_INDEX
#include "SubjectProperty\SSGSubjectProperty.h"
#endif
#pragma function(memcmp)
#pragma function(strlen)
extern HANDLE hHeap;
#endif

#define AT_ADJUST   0x0002
#define AT_REPLACE  0x0004
#define AT_VARIABLE 0x0800
#define AT_DEFINE   0x1000

#define OS_SPLIT         0x0001
#define OS_PUSH          0x0002
#define OS_OPEN          0x0004
#define OS_CLOSE         0x0008
#define OS_MONADIC       0x0010
#define OS_POST          0x0020
#define OS_SHORT_CIRCUIT 0x0040
#define OS_LEFT_ASSIGN   0x0080
#define OS_RIGHT_ASSIGN  0x0100
#define OS_TERNARY       0x0200
#define OS_TERNARY_END   0x0400
#define OS_WHILE_BEGIN   0x0800
#define OS_WHILE_END     0x1000
#define OS_MEMMOVE_BEGIN 0x2000
#define OS_MEMMOVE_END   0x4000
#define OS_RET_OPERAND   0x8000

/*
 [Wikipedia] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/%E6%BC%94%E7%AE%97%E5%AD%90%E3%81%AE%E5%84%AA%E5%85%88%E9%A0%86%E4%BD%8D#.E3.83.97.E3.83.AD.E3.82.B0.E3.83.A9.E3.83.9F.E3.83.B3.E3.82.B0.E8.A8.80.E8.AA.9E

 [Microsoft Developer Network] - [優先順位と評価順序] - [C 演算子の優先順位と結合規則]
 https://msdn.microsoft.com/ja-jp/library/2bxt6kc4.aspx

 [Wikipedia] - [CとC++の演算子] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/C%E3%81%A8C%2B%2B%E3%81%AE%E6%BC%94%E7%AE%97%E5%AD%90#.E6.BC.94.E7.AE.97.E5.AD.90.E3.81.AE.E5.84.AA.E5.85.88.E9.A0.86.E4.BD.8D

 100 while                              OS_PUSH
 100 memmove                            OS_PUSH
 100 (                                  OS_OPEN
 100 ++ --                              OS_PUSH | OS_MONADIC | OS_POST 後置インクリメント 後置デクリメント
  90 [_                                 OS_OPEN
  85 [.                                 OS_OPEN
  83 [~                                 OS_OPEN
  80 [:                                 OS_OPEN
  75 MName:: HNumber::
     Cast32:: Cast64::
     I1toI4:: I2toI4:: I4toI8::
     Memory::
     strlen:: wcslen::
     BitScanForward:: BitScanReverse::  OS_PUSH
  52 ++ -- - ! ~ *                      OS_PUSH | OS_MONADIC           前置インクリメント 前置デクリメント 単項マイナス 論理否定 ビットごとの論理否定 間接演算子
  51 * / % idiv imod                    OS_PUSH                        乗算 除算 剰余算 符号付除算 符号付剰余算
  50 + -                                OS_PUSH                        加算 減算
  49 << >> sar rol ror                  OS_PUSH                        左論理シフト 右論理シフト 右算術シフト 左ローテート 右ローテート
  40 < > <= >= lt gt le ge              OS_PUSH                        小なり 大なり 以下 以上 符号付小なり 符号付大なり 符号付以下 符号付以上
  35 == !=                              OS_PUSH                        等価 非等価
  34 &                                  OS_PUSH                        ビットごとの論理積
  33 ^                                  OS_PUSH                        ビットごとの排他的論理和
  32 |                                  OS_PUSH                        ビットごとの論理和
  31 &&                                 OS_PUSH | OS_SHORT_CIRCUIT     論理積
  30 ||                                 OS_PUSH | OS_SHORT_CIRCUIT     論理和
  29 ? :                                OS_PUSH                        条件演算子
  25 =>                                 OS_PUSH                        右辺代入
  25 = += -= *= /= %= &= |= ^= <<= >>=  OS_PUSH | OS_LEFT_ASSIGN       左辺代入 加算代入 減算代入 乗算代入 除算代入 剰余代入 ビット積代入 ビット排他的論理和代入 ビット和代入 左論理シフト代入 右論理シフト代入
  20 :] :8] :7] :6] :5] :4] :3] :2] :1] OS_PUSH | OS_CLOSE
  20 :L] :L8] :L7] :L6] :L5]
     :L4] :L3] :L2] :L1]                OS_PUSH | OS_CLOSE
  18 ~] ~8] ~7] ~6] ~5] ~4] ~3] ~2]     OS_PUSH | OS_CLOSE
  15 .]                                 OS_PUSH | OS_CLOSE
  10 _]                                 OS_PUSH | OS_CLOSE
   0 )                                  OS_CLOSE
   0 ;                                  OS_SPLIT
*/

typedef enum {
	TAG_NOT_OPERATOR     ,  // 127                  OS_PUSH
	TAG_MEMMOVE_LOCAL    ,  // 127 L                OS_PUSH
	TAG_WHILE            ,  // 100 while            OS_PUSH | OS_WHILE_BEGIN
	TAG_MEMMOVE          ,  // 100 memmove          OS_PUSH | OS_MEMMOVE_BEGIN
	TAG_PARENTHESIS_OPEN ,  // 100 (                OS_OPEN
	TAG_INC              ,  // 100 N++     (52 ++N) OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_DEC              ,  // 100 N--     (52 --N) OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_ADDR_ADJUST_OPEN ,  //  90 [_               OS_OPEN
	TAG_ADDR_REPLACE_OPEN,  //  85 [.               OS_OPEN
	TAG_REV_ENDIAN_OPEN  ,  //  83 [~               OS_OPEN
	TAG_REMOTE_OPEN      ,  //  80 [:               OS_OPEN
	TAG_MNAME            ,  //  75 MName::          OS_PUSH
	TAG_PROCEDURE        ,  //  75 ::               OS_PUSH
	TAG_IMPORT_FUNCTION  ,  //  75 :!               OS_PUSH
	TAG_IMPORT_REFERENCE ,  //  75 :&               OS_PUSH
	TAG_MODULENAME       ,  //  75                  OS_PUSH
	TAG_SECTION          ,  //  75 := :+            OS_PUSH
	TAG_HNUMBER          ,  //  75 HNumber::        OS_PUSH
	TAG_CAST32           ,  //  75 Cast32::         OS_PUSH
	TAG_CAST64           ,  //  75 Cast64::         OS_PUSH
	TAG_I1TOI4           ,  //  75 I1toI4::         OS_PUSH
	TAG_I2TOI4           ,  //  75 I2toI4::         OS_PUSH
	TAG_I4TOI8           ,  //  75 I4toI8::         OS_PUSH
	TAG_MEMORY           ,  //  75 Memory::         OS_PUSH
	TAG_STRLEN           ,  //  75 strlen::         OS_PUSH
	TAG_WCSLEN           ,  //  75 wcslen::         OS_PUSH
	TAG_BSF              ,  //  75 BitScanForward:: OS_PUSH
	TAG_BSR              ,  //  75 BitScanReverse:: OS_PUSH
	TAG_NEG              ,  //  52 -                OS_PUSH | OS_MONADIC
	TAG_NOT              ,  //  52 !                OS_PUSH | OS_MONADIC
	TAG_BIT_NOT          ,  //  52 ~                OS_PUSH | OS_MONADIC
	TAG_INDIRECTION      ,  //  52 *                OS_PUSH | OS_MONADIC
	TAG_MUL              ,  //  51 *       (25 *= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_DIV              ,  //  51 /       (25 /= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_MOD              ,  //  51 %       (25 %= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_IDIV             ,  //  51 idiv             OS_PUSH
	TAG_IMOD             ,  //  51 imod             OS_PUSH
	TAG_ADD              ,  //  50 +       (25 += ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SUB              ,  //  50 -       (25 -= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHL              ,  //  49 <<      (25 <<=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHR              ,  //  49 >>      (25 >>=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SAR              ,  //  49 sar              OS_PUSH
	TAG_ROL              ,  //  49 rol              OS_PUSH
	TAG_ROR              ,  //  49 ror              OS_PUSH
	TAG_BT               ,  //  40 <                OS_PUSH
	TAG_AT               ,  //  40 >                OS_PUSH
	TAG_BE               ,  //  40 <=               OS_PUSH
	TAG_AE               ,  //  40 >=               OS_PUSH
	TAG_LT               ,  //  40 lt               OS_PUSH
	TAG_GT               ,  //  40 gt               OS_PUSH
	TAG_LE               ,  //  40 le               OS_PUSH
	TAG_GE               ,  //  40 ge               OS_PUSH
	TAG_EQ               ,  //  35 ==               OS_PUSH
	TAG_NE               ,  //  35 !=               OS_PUSH
	TAG_BIT_AND          ,  //  34 &       (25 &= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_XOR              ,  //  33 ^       (25 ^= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_BIT_OR           ,  //  32 |       (25 |= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_AND              ,  //  31 &&               OS_PUSH | OS_SHORT_CIRCUIT
	TAG_OR               ,  //  30 ||               OS_PUSH | OS_SHORT_CIRCUIT
	TAG_TERNARY          ,  //  29 ?                OS_PUSH | OS_TERNARY
	TAG_TERNARY_SPLIT    ,  //  29 :                OS_PUSH | OS_TERNARY
	TAG_RIGHT_ASSIGN     ,  //  25 =>               OS_PUSH | OS_RIGHT_ASSIGN
	TAG_LEFT_ASSIGN      ,  //  25 =                OS_PUSH | OS_LEFT_ASSIGN
	TAG_REMOTE1          ,  //  20 :1]              OS_PUSH | OS_CLOSE
	TAG_REMOTE2          ,  //  20 :2]              OS_PUSH | OS_CLOSE
	TAG_REMOTE3          ,  //  20 :3]              OS_PUSH | OS_CLOSE
	TAG_REMOTE4          ,  //  20 :] :4]           OS_PUSH | OS_CLOSE
	TAG_REMOTE5          ,  //  20 :5]              OS_PUSH | OS_CLOSE
	TAG_REMOTE6          ,  //  20 :6]              OS_PUSH | OS_CLOSE
	TAG_REMOTE7          ,  //  20 :7]              OS_PUSH | OS_CLOSE
	TAG_REMOTE8          ,  //  20 :8]              OS_PUSH | OS_CLOSE
	TAG_LOCAL1           ,  //  20 :L1]             OS_PUSH | OS_CLOSE
	TAG_LOCAL2           ,  //  20 :L2]             OS_PUSH | OS_CLOSE
	TAG_LOCAL3           ,  //  20 :L3]             OS_PUSH | OS_CLOSE
	TAG_LOCAL4           ,  //  20 :L] :L4]         OS_PUSH | OS_CLOSE
	TAG_LOCAL5           ,  //  20 :L5]             OS_PUSH | OS_CLOSE
	TAG_LOCAL6           ,  //  20 :L6]             OS_PUSH | OS_CLOSE
	TAG_LOCAL7           ,  //  20 :L7]             OS_PUSH | OS_CLOSE
	TAG_LOCAL8           ,  //  20 :L8]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN2      ,  //  18 ~2]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN3      ,  //  18 ~3]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN4      ,  //  18 ~] ~4]           OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN5      ,  //  18 ~5]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN6      ,  //  18 ~6]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN7      ,  //  18 ~7]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN8      ,  //  18 ~8]              OS_PUSH | OS_CLOSE
	TAG_ADDR_REPLACE     ,  //  15 .]               OS_PUSH | OS_CLOSE
	TAG_ADDR_ADJUST      ,  //  10 _]               OS_PUSH | OS_CLOSE
	TAG_PARAM_SPLIT      ,  //   0 ,                OS_PUSH | OS_SPLIT
	TAG_WHILE_EXPR       ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_PARENTHESIS_CLOSE,  //   0 )                OS_CLOSE
	TAG_SPLIT            ,  //   0 ;                OS_SPLIT
} TAG;

typedef enum {
	PRIORITY_NOT_OPERATOR      = 127,   //                   OS_PUSH
	PRIORITY_MEMMOVE_LOCAL     = 127,   //  L                OS_PUSH
	PRIORITY_WHILE             = 100,   //  while            OS_PUSH | OS_WHILE_BEGIN
	PRIORITY_MEMMOVE           = 100,   //  memmove          OS_PUSH | OS_MEMMOVE_BEGIN
	PRIORITY_PARENTHESIS_OPEN  = 100,   //  (                OS_OPEN
	PRIORITY_POST_INC          = 100,   //  N++              OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_POST_DEC          = 100,   //  N--              OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_ADDR_ADJUST_OPEN  =  90,   //  [_               OS_OPEN
	PRIORITY_ADDR_REPLACE_OPEN =  85,   //  [.               OS_OPEN
	PRIORITY_REV_ENDIAN_OPEN   =  83,   //  [~               OS_OPEN
	PRIORITY_REMOTE_OPEN       =  80,   //  [:               OS_OPEN
	PRIORITY_FUNCTION          =  75,   //  MName::          OS_PUSH
	                                    //  ::               OS_PUSH
	                                    //  :!               OS_PUSH
	                                    //  :&               OS_PUSH
	                                    //  := :+            OS_PUSH
	                                    //  HNumber::        OS_PUSH
	                                    //  Cast32::         OS_PUSH
	                                    //  Cast64::         OS_PUSH
	                                    //  I1toI4::         OS_PUSH
	                                    //  I2toI4::         OS_PUSH
	                                    //  I4toI8::         OS_PUSH
	                                    //  Memory::         OS_PUSH
	                                    //  strlen::         OS_PUSH
	                                    //  wcslen::         OS_PUSH
	                                    //  BitScanForward:: OS_PUSH
	                                    //  BitScanReverse:: OS_PUSH
	PRIORITY_NEG               =  52,   //  -                OS_PUSH | OS_MONADIC
	PRIORITY_NOT               =  52,   //  !                OS_PUSH | OS_MONADIC
	PRIORITY_BIT_NOT           =  52,   //  ~                OS_PUSH | OS_MONADIC
	PRIORITY_INDIRECTION       =  52,   //  *                OS_PUSH | OS_MONADIC
	PRIORITY_PRE_INC           =  52,   //  ++N              OS_PUSH | OS_MONADIC
	PRIORITY_PRE_DEC           =  52,   //  --N              OS_PUSH | OS_MONADIC
	PRIORITY_MUL               =  51,   //  *       (25 *= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_DIV               =  51,   //  /       (25 /= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_MOD               =  51,   //  %       (25 %= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_IDIV              =  51,   //  idiv             OS_PUSH
	PRIORITY_IMOD              =  51,   //  imod             OS_PUSH
	PRIORITY_ADD               =  50,   //  +       (25 += ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SUB               =  50,   //  -       (25 -= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHL               =  49,   //  <<      (25 <<=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHR               =  49,   //  >>      (25 >>=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SAR               =  49,   //  sar              OS_PUSH
	PRIORITY_ROL               =  49,   //  rol              OS_PUSH
	PRIORITY_ROR               =  49,   //  ror              OS_PUSH
	PRIORITY_BT                =  40,   //  <                OS_PUSH
	PRIORITY_AT                =  40,   //  >                OS_PUSH
	PRIORITY_BE                =  40,   //  <=               OS_PUSH
	PRIORITY_AE                =  40,   //  >=               OS_PUSH
	PRIORITY_LT                =  40,   //  lt               OS_PUSH
	PRIORITY_GT                =  40,   //  gt               OS_PUSH
	PRIORITY_LE                =  40,   //  le               OS_PUSH
	PRIORITY_GE                =  40,   //  ge               OS_PUSH
	PRIORITY_EQ                =  35,   //  ==               OS_PUSH
	PRIORITY_NE                =  35,   //  !=               OS_PUSH
	PRIORITY_BIT_AND           =  34,   //  &       (25 &= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_XOR               =  33,   //  ^       (25 ^= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_BIT_OR            =  32,   //  |       (25 |= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_AND               =  31,   //  &&               OS_PUSH | OS_SHORT_CIRCUIT
	PRIORITY_OR                =  30,   //  ||               OS_PUSH | OS_SHORT_CIRCUIT
	PRIORITY_TERNARY           =  29,   //  ? :              OS_PUSH | OS_TERNARY
	PRIORITY_RIGHT_ASSIGN      =  25,   //  =>               OS_PUSH | OS_RIGHT_ASSIGN
	PRIORITY_LEFT_ASSIGN       =  25,   //  =                OS_PUSH | OS_LEFT_ASSIGN
	PRIORITY_REMOTE            =  20,   //  :]  :1] :2] :3]  OS_PUSH | OS_CLOSE
	                                    //  :4] :5] :6] :7]
	                                    //  :8]
	                                    //  :L]  :L1] :L2]   OS_PUSH | OS_CLOSE
	                                    //  :L3] :L4] :L5]
	                                    //  :L6] :L7] :L8]
	PRIORITY_REV_ENDIAN        =  18,   //  ~]  ~2] ~3] ~4]  OS_PUSH | OS_CLOSE
	                                    //  ~5] ~6] ~7] ~8]
	PRIORITY_ADDR_REPLACE      =  15,   //  .]               OS_PUSH | OS_CLOSE
	PRIORITY_ADDR_ADJUST       =  10,   //  _]               OS_PUSH | OS_CLOSE
	PRIORITY_PARAM_SPLIT       =   0,   //  ,                OS_PUSH | OS_SPLIT
	PRIORITY_PARENTHESIS_CLOSE =   0,   //  )                OS_CLOSE
	PRIORITY_SPLIT             =   0,   //  ;                OS_SPLIT
} PRIORITY;

typedef struct {
	TAG    Tag;
	size_t Length;
	LPSTR  String;
	BYTE   Priority;
	WORD   Type;
	size_t LoopDepth;
} MARKUP, *PMARKUP;

typedef union {
	struct {
		DWORD Low;
		DWORD High;
	};
	ULONGLONG Quad;
	float     Float;
	double    Double;
} VARIABLE, *PVARIABLE;

typedef struct {
	VARIABLE Value;
	BOOL     IsQuad;
} OPERAND, *POPERAND;

typedef struct {
	size_t   Length;
	LPCSTR   String;
	VARIABLE Value;
	BOOL     IsQuad;
} MARKUP_VARIABLE, *PMARKUP_VARIABLE;

#ifndef TYPEDEF_PROCESSMEMORYBLOCK
#define TYPEDEF_PROCESSMEMORYBLOCK
typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;
#endif

size_t             nNumberOfProcessMemory = 0;
PROCESSMEMORYBLOCK *lpProcessMemory = NULL;

//---------------------------------------------------------------------
#define AllocMarkup() \
	(MARKUP *)HeapAlloc(hHeap, 0, 0x10 * sizeof(MARKUP))
//---------------------------------------------------------------------
MARKUP *ReAllocMarkup(MARKUP **lppMarkupArray, size_t *lpnNumberOfMarkup)
{
	if (*lpnNumberOfMarkup && !(*lpnNumberOfMarkup & 0x0F))
	{
		size_t nBytes;
		LPVOID lpMem;

		nBytes = (*lpnNumberOfMarkup + 0x10) * sizeof(MARKUP);
		lpMem = HeapReAlloc(hHeap, 0, *lppMarkupArray, nBytes);
		if (!lpMem)
			return NULL;
		*lppMarkupArray = (MARKUP *)lpMem;
	}
	return *lppMarkupArray + (*lpnNumberOfMarkup)++;
}
//---------------------------------------------------------------------
void TrimMarkupString(MARKUP *lpMarkup)
{
	if (lpMarkup->Length)
	{
		char *begin, *end;

		// it do not checking multibyte,
		// because space is not the lead and tail byte of codepage 932.

		// load from memory
		begin = lpMarkup->String;
		end = begin + lpMarkup->Length;

		// trim left
		while (__intrinsic_isspace(*begin))
			begin++;

		// trim right
		while (--end > begin && __intrinsic_isspace(*end));

		// store to memory
		lpMarkup->String = begin;
		lpMarkup->Length = ++end - begin;
	}
}
//---------------------------------------------------------------------
MARKUP *Markup(IN LPCSTR lpSrc, IN size_t nSrcLength, OUT LPSTR *lppMarkupStringBuffer, OUT size_t *lpnNumberOfMarkup)
{
	size_t  nStringLength;
	LPSTR   lpMarkupStringBuffer;
	LPSTR   lpDest;
	char    c;
	MARKUP  *lpTagArray;
	size_t  nNumberOfTag;
	BOOLEAN bIsLaedByte, bPrevIsTailByte;
	MARKUP  *lpMarkupArray;
	MARKUP  *lpMarkup, *lpEndOfMarkup;
	size_t  nMarkupIndex;
	size_t  nMarkupLength;
	size_t  nFirstTernary;
	size_t  nFirstWhile;
	size_t  nFirstMemmove;
	size_t  nDepth;

	// check parameters
	if (!lpSrc || !nSrcLength)
		return NULL;

	// allocate buffer
	lpMarkupStringBuffer = (char *)HeapAlloc(hHeap, 0, nSrcLength + sizeof(DWORD));
	if (!lpMarkupStringBuffer)
		return NULL;

	// copy
	for (lpDest = lpMarkupStringBuffer; *lpDest = c = *(lpSrc++); lpDest++)
	{
		// check lead byte of multibyte string
		if (!__intrinsic_isleadbyte(c))
		{
			if (c != '\\')
			{
				// split by semicolon
				if (c == ';')
					*lpDest = '\0';
				continue;
			}
			// skip next character of escape
			if (!(*(++lpDest) = c = *(lpSrc++)))
				break;
			if (!__intrinsic_isleadbyte(c))
				continue;
		}
		// skip tail byte
		if (!(*(++lpDest) = *(lpSrc++)))
			break;
	}
	*(LPDWORD)lpDest = 0;
	nStringLength = lpDest - lpMarkupStringBuffer;

	// mark up the tags and operators
	lpTagArray = AllocMarkup();
	if (!lpTagArray)
		goto FAILED1;
	nFirstTernary = SIZE_MAX;
	nFirstWhile = SIZE_MAX;
	nFirstMemmove = SIZE_MAX;
	nNumberOfTag = 0;
	bPrevIsTailByte = FALSE;
	for (LPBYTE p = lpMarkupStringBuffer, end = lpMarkupStringBuffer + nStringLength; p < end; bPrevIsTailByte = bIsLaedByte)
	{
		if (!__intrinsic_isleadbyte(*p))
		{
			TAG    iTag;
			size_t nLength;
			BYTE   bPriority;

			#define APPEND_TAG(tag, length, priority, type)                  \
			do                                                               \
			{                                                                \
			    if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag))) \
			        goto FAILED2;                                            \
			    lpMarkup->Tag      = tag;                                    \
			    lpMarkup->Length   = length;                                 \
			    lpMarkup->String   = p;                                      \
			    lpMarkup->Priority = priority;                               \
			    lpMarkup->Type     = type;                                   \
			} while (0)

			#define APPEND_TAG_WITH_CONTINUE(tag, length, priority, type)    \
			do                                                               \
			{                                                                \
			    APPEND_TAG(tag, length, priority, type);                     \
			    p += length;                                                 \
			    goto CONTINUE;                                               \
			} while (0)

			bIsLaedByte = FALSE;
			switch (*p)
			{
			case '(':
				APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_OPEN, 1, PRIORITY_PARENTHESIS_OPEN, OS_OPEN);
			case '[':
				switch (*(p + 1))
				{
				case '_':
					APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST_OPEN, 2, PRIORITY_ADDR_ADJUST_OPEN, OS_OPEN);
				case '.':
					APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE_OPEN, 2, PRIORITY_ADDR_REPLACE_OPEN, OS_OPEN);
				case '~':
					APPEND_TAG_WITH_CONTINUE(TAG_REV_ENDIAN_OPEN, 2, PRIORITY_REV_ENDIAN_OPEN, OS_OPEN);
				case ':':
					APPEND_TAG_WITH_CONTINUE(TAG_REMOTE_OPEN, 2, PRIORITY_REMOTE_OPEN, OS_OPEN);
				}
				break;
			case 'M':
				if (*(LPDWORD)(p + 1) == BSWAP32('Name'))
				{
					if (*(LPWORD)(p + 5) == BSWAP16('::'))
					{
						iTag = TAG_MNAME;
						nLength = 7;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
				}
				else if (*(LPDWORD)(p + 1) == BSWAP32('emor'))
				{
					if (*(LPDWORD)(p + 4) == BSWAP32('ry::'))
					{
						iTag = TAG_MEMORY;
						nLength = 8;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
				}
				else
				{
					break;
				}
				p += 5;
				continue;
			case 'H':
				if (*(LPDWORD)(p + 1) != BSWAP32('Numb'))
					break;
				if (*(LPDWORD)(p + 5) == BSWAP32('er::'))
				{
					iTag = TAG_HNUMBER;
					nLength = 9;
					goto APPEND_FUNCTIONAL_OPERATOR;
				}
				else
				{
					p += 5;
					continue;
				}
			case 'B':
				if (*(LPDWORD)p != BSWAP32('BitS'))
					break;
				if (*(LPDWORD)(p + 4) == BSWAP32('canF'))
				{
					if (*(LPDWORD)(p + 8) == BSWAP32('orwa'))
					{
						if (*(LPDWORD)(p + 12) == BSWAP32('rd::'))
						{
							iTag = TAG_BSF;
							nLength = 16;
							goto APPEND_FUNCTIONAL_OPERATOR;
						}
						else
						{
							p += 12;
							continue;
						}
					}
					else
					{
						p += 8;
						continue;
					}
				}
				else if (*(LPDWORD)(p + 4) == BSWAP32('canR'))
				{
					if (*(LPDWORD)(p + 8) == BSWAP32('ever'))
					{
						if (*(LPDWORD)(p + 12) == BSWAP32('se::'))
						{
							iTag = TAG_BSR;
							nLength = 16;
							goto APPEND_FUNCTIONAL_OPERATOR;
						}
						else
						{
							p += 12;
							continue;
						}
					}
					else
					{
						p += 8;
						continue;
					}
				}
				else
				{
					p += 4;
					continue;
				}
			case 'C':
				if (*(LPDWORD)p != BSWAP32('Cast'))
					break;
				if (*(LPDWORD)(p + 4) == BSWAP32('32::'))
				{
					iTag = TAG_CAST32;
					nLength = 8;
					goto APPEND_FUNCTIONAL_OPERATOR;
				}
				else if (*(LPDWORD)(p + 4) == BSWAP32('64::'))
				{
					iTag = TAG_CAST64;
					nLength = 8;
					goto APPEND_FUNCTIONAL_OPERATOR;
				}
				else
				{
					p += 4;
					continue;
				}
			case 'I':
				if (*(LPDWORD)p == BSWAP32('I1to'))
				{
					if (*(LPDWORD)(p + 4) == BSWAP32('I4::'))
					{
						iTag = TAG_I1TOI4;
						nLength = 8;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
				}
				else if (*(LPDWORD)p == BSWAP32('I2to'))
				{
					if (*(LPDWORD)(p + 4) == BSWAP32('I4::'))
					{
						iTag = TAG_I2TOI4;
						nLength = 8;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
				}
				else if (*(LPDWORD)p == BSWAP32('I4to'))
				{
					if (*(LPDWORD)(p + 4) == BSWAP32('I8::'))
					{
						iTag = TAG_I4TOI8;
						nLength = 8;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
				}
				else
				{
					break;
				}
				p += 4;
				continue;
			APPEND_FUNCTIONAL_OPERATOR:
				if (p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					__intrinsic_isascii(*(p - 1)) &&
					!__intrinsic_isdigit(*(p - 1)) &&
					!__intrinsic_isalpha(*(p - 1)) &&
					*(p - 1) != '$'))
				{
					APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_FUNCTION, OS_PUSH);
				}
				p += nLength;
				continue;
			case '~':
				switch (*(p + 1))
				{
				case ']':
					iTag = TAG_REV_ENDIAN4;
					nLength = 2;
					goto APPEND_REV_ENDIAN;
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (*(p + 2) == ']')
					{
						iTag = (TAG)(TAG_REV_ENDIAN2 + *(p + 1) - '2');
						nLength = 3;
					APPEND_REV_ENDIAN:
						APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_REV_ENDIAN, OS_PUSH | OS_CLOSE);
					}
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_BIT_NOT, 1, PRIORITY_BIT_NOT, OS_PUSH | OS_MONADIC);
				}
				break;
			case '*':
				if (*(p + 1) != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_MUL, 1, PRIORITY_MUL, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_MUL, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case '/':
				if (*(p + 1) != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_DIV, 1, PRIORITY_DIV, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_DIV, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case '%':
				if (*(p + 1) != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_MOD, 1, PRIORITY_MOD, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_MOD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case 'i':
				if (*(LPDWORD)p == BSWAP32('idiv'))
				{
					iTag = TAG_IDIV;
					nLength = 4;
					bPriority = PRIORITY_IDIV;
					goto APPEND_WORD_OPERATOR;
				}
				else if (*(LPDWORD)p == BSWAP32('imod'))
				{
					iTag = TAG_IMOD;
					nLength = 4;
					bPriority = PRIORITY_IMOD;
					goto APPEND_WORD_OPERATOR;
				}
				break;
			case '+':
				switch (*(p + 1))
				{
				case '+':
					APPEND_TAG_WITH_CONTINUE(TAG_INC, 2, PRIORITY_PRE_INC, OS_PUSH | OS_MONADIC);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_ADD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_ADD, 1, PRIORITY_ADD, OS_PUSH);
				}
			case '-':
				switch (*(p + 1))
				{
				case '-':
					APPEND_TAG_WITH_CONTINUE(TAG_DEC, 2, PRIORITY_PRE_DEC, OS_PUSH | OS_MONADIC);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_SUB, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_SUB, 1, PRIORITY_SUB, OS_PUSH);
				}
			case '<':
				switch (*(p + 1))
				{
				case '<':
					if (*(p + 2) != '=')
						APPEND_TAG_WITH_CONTINUE(TAG_SHL, 2, PRIORITY_SHL, OS_PUSH);
					else
						APPEND_TAG_WITH_CONTINUE(TAG_SHL, 3, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_BE, 2, PRIORITY_BE, OS_PUSH);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_BT, 1, PRIORITY_BT, OS_PUSH);
				}
			case '>':
				switch (*(p + 1))
				{
				case '>':
					if (*(p + 2) != '=')
						APPEND_TAG_WITH_CONTINUE(TAG_SHR, 2, PRIORITY_SHR, OS_PUSH);
					else
						APPEND_TAG_WITH_CONTINUE(TAG_SHR, 3, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_AE, 2, PRIORITY_AE, OS_PUSH);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_AT, 1, PRIORITY_AT, OS_PUSH);
				}
			case 'l':
				if (*(p + 1) == 't')
				{
					iTag = TAG_LT;
					nLength = 2;
					bPriority = PRIORITY_LT;
					goto APPEND_WORD_OPERATOR;
				}
				else if (*(p + 1) == 'e')
				{
					iTag = TAG_LE;
					nLength = 2;
					bPriority = PRIORITY_LE;
					goto APPEND_WORD_OPERATOR;
				}
				break;
			case 'g':
				if (*(p + 1) == 't')
				{
					iTag = TAG_GT;
					nLength = 2;
					bPriority = PRIORITY_GT;
					goto APPEND_WORD_OPERATOR;
				}
				else if (*(p + 1) == 'e')
				{
					iTag = TAG_GE;
					nLength = 2;
					bPriority = PRIORITY_GE;
					goto APPEND_WORD_OPERATOR;
				}
				break;
			case 's':
				if (*(LPWORD)(p + 1) == BSWAP16('ar'))
				{
					iTag = TAG_SAR;
					nLength = 3;
					bPriority = PRIORITY_SAR;
					goto APPEND_WORD_OPERATOR;
				}
				else if (*(LPDWORD)p == BSWAP32('strl'))
				{
					if (*(LPDWORD)(p + 4) != BSWAP32('en::'))
					{
						iTag = TAG_STRLEN;
						nLength = 8;
						goto APPEND_FUNCTIONAL_OPERATOR;
					}
					p += 4;
					continue;
				}
				break;
			case 'r':
				if (*(p + 1) != 'o')
					break;
				if (*(p + 2) == 'l')
				{
					iTag = TAG_ROL;
					nLength = 3;
					bPriority = PRIORITY_ROL;
					goto APPEND_WORD_OPERATOR;
				}
				else if (*(p + 2) == 'r')
				{
					iTag = TAG_ROR;
					nLength = 3;
					bPriority = PRIORITY_ROR;
					goto APPEND_WORD_OPERATOR;
				}
				p++;
				break;
			APPEND_WORD_OPERATOR:
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					__intrinsic_isascii(*(p - 1)) &&
					!__intrinsic_isalpha(*(p - 1)) &&
					*(p - 1) != '_' &&
					*(p - 1) != '$')) &&
					__intrinsic_isascii(*(p + nLength)) &&
					!__intrinsic_isalpha(*(p + nLength)) &&
					*(p + nLength) != '_')
				{
					APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH);
				}
				p += nLength;
				continue;
			case 'w':
				switch (*(p + 1))
				{
				case 'h':
					if (*(LPWORD)(p + 2) == BSWAP16('il'))
					{
						if (*(p + 4) == 'e')
						{
							if ((p == lpMarkupStringBuffer || (
								!bPrevIsTailByte &&
								__intrinsic_isascii(*(p - 1)) &&
								!__intrinsic_isdigit(*(p - 1)) &&
								!__intrinsic_isalpha(*(p - 1)) &&
								*(p - 1) != '$')) &&
								(__intrinsic_isspace(*(p + 5)) || *(p + 5) == '('))
							{
								if (nFirstWhile == SIZE_MAX)
									nFirstWhile = nNumberOfTag;
								APPEND_TAG_WITH_CONTINUE(TAG_WHILE, 5, PRIORITY_WHILE, OS_PUSH | OS_WHILE_BEGIN);
							}
							p += 5;
							continue;
						}
						p += 4;
						continue;
					}
					p += 2;
					continue;
				case 'c':
					if (*(LPDWORD)(p + 2) == BSWAP32('slen'))
					{
						if (*(LPWORD)(p + 6) == BSWAP16('::'))
						{
							iTag = TAG_WCSLEN;
							nLength = 8;
							goto APPEND_FUNCTIONAL_OPERATOR;
						}
						p += 6;
						continue;
					}
					p += 2;
					continue;
				}
				break;
			case 'm':
				if (*(LPDWORD)(p + 1) != BSWAP32('emmo'))
					break;
				if (*(LPWORD)(p + 5) == BSWAP16('ve'))
				{
					if ((p == lpMarkupStringBuffer || (
						!bPrevIsTailByte &&
						__intrinsic_isascii(*(p - 1)) &&
						!__intrinsic_isdigit(*(p - 1)) &&
						!__intrinsic_isalpha(*(p - 1)) &&
						*(p - 1) != '$')) &&
						(__intrinsic_isspace(*(p + 7)) || *(p + 7) == '('))
					{
						if (nFirstMemmove == SIZE_MAX)
							nFirstMemmove = nNumberOfTag;
						APPEND_TAG_WITH_CONTINUE(TAG_MEMMOVE, 7, PRIORITY_MEMMOVE, OS_PUSH | OS_MEMMOVE_BEGIN);
					}
					p += 7;
					continue;
				}
				p += 5;
				continue;
			case 'L':
				if (nNumberOfTag > 1 && !bPrevIsTailByte &&
					(__intrinsic_isspace(*(p - 1)) || *(p - 1) == ',' || *(p - 1) == '(') &&
					(__intrinsic_isspace(*(p + 1)) || *(p + 1) == '(' || *(p + 1) == '['))
				{
					MARKUP *lpPrev;

					lpPrev = lpTagArray + nNumberOfTag - 1;
					if (lpPrev->Tag != TAG_PARENTHESIS_OPEN)
					{
						size_t nDepth;

						if (lpPrev->Tag != TAG_PARAM_SPLIT)
							break;
						nDepth = 1;
						while (--lpPrev != lpTagArray)
						{
							if (!(lpPrev->Type & (OS_OPEN | OS_CLOSE)))
								continue;
							if (lpPrev->Type & OS_CLOSE)
								nDepth++;
							else if (!--nDepth)
								break;
						}
						if (lpPrev == lpTagArray)
							break;
					}
					if ((--lpPrev)->Tag != TAG_MEMMOVE)
						break;
					APPEND_TAG_WITH_CONTINUE(TAG_MEMMOVE_LOCAL, 1, PRIORITY_MEMMOVE_LOCAL, OS_PUSH);
				}
				break;
			case '?':
				if (nFirstTernary == SIZE_MAX)
					nFirstTernary = nNumberOfTag;
				APPEND_TAG_WITH_CONTINUE(TAG_TERNARY, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARY);
			case '=':
				switch (*(p + 1))
				{
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_EQ, 2, PRIORITY_EQ, OS_PUSH);
				case '>':
					APPEND_TAG_WITH_CONTINUE(TAG_RIGHT_ASSIGN, 2, PRIORITY_RIGHT_ASSIGN, OS_PUSH | OS_RIGHT_ASSIGN);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_LEFT_ASSIGN, 1, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				}
			case '!':
				if (*(p + 1) == '=')
					APPEND_TAG_WITH_CONTINUE(TAG_NE, 2, PRIORITY_NE, OS_PUSH);
				else if (!nNumberOfTag || (lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_FUNCTION && lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_REFERENCE))
					APPEND_TAG_WITH_CONTINUE(TAG_NOT, 1, PRIORITY_NOT, OS_PUSH | OS_MONADIC);
				if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
					goto FAILED2;
				lpMarkup->Tag      = TAG_MODULENAME;
				lpMarkup->Length   = p - lpTagArray[nNumberOfTag - 2].String - 1;
				lpMarkup->String   = lpTagArray[nNumberOfTag - 2].String + 2;
				lpMarkup->Priority = PRIORITY_FUNCTION;
				lpMarkup->Type     = OS_PUSH;
				break;
			case 'a':
				if (*(LPWORD)(p + 1) != BSWAP16('nd'))
					break;
				iTag = TAG_AND;
				nLength = 3;
				bPriority = PRIORITY_AND;
				goto APPEND_RET_OPERAND_OPERATOR;
			case '&':
				switch (*(p + 1))
				{
				case '&':
					APPEND_TAG(TAG_AND, 2, PRIORITY_AND, OS_PUSH | OS_SHORT_CIRCUIT);
					APPEND_TAG_WITH_CONTINUE(TAG_AND, 2, PRIORITY_AND, OS_PUSH);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_BIT_AND, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_BIT_AND, 1, PRIORITY_BIT_AND, OS_PUSH);
				}
			case '^':
				if (*(p + 1) != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_XOR, 1, PRIORITY_XOR, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_XOR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case 'o':
				if (*(p + 1) != 'r')
					break;
				iTag = TAG_OR;
				nLength = 2;
				bPriority = PRIORITY_OR;
			APPEND_RET_OPERAND_OPERATOR:
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					__intrinsic_isascii(*(p - 1)) &&
					!__intrinsic_isalpha(*(p - 1)) &&
					*(p - 1) != '_' &&
					*(p - 1) != '$')) &&
					__intrinsic_isascii(*(p + nLength)) &&
					!__intrinsic_isalpha(*(p + nLength)) &&
					*(p + nLength) != '_')
				{
					APPEND_TAG(iTag, nLength, bPriority, OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND);
					APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH | OS_RET_OPERAND);
				}
				p += nLength;
				continue;
			case '|':
				switch (*(p + 1))
				{
				case '|':
					APPEND_TAG(TAG_OR, 2, PRIORITY_OR, OS_PUSH | OS_SHORT_CIRCUIT);
					APPEND_TAG_WITH_CONTINUE(TAG_OR, 2, PRIORITY_OR, OS_PUSH);
				case '=':
					APPEND_TAG_WITH_CONTINUE(TAG_BIT_OR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
				default:
					APPEND_TAG_WITH_CONTINUE(TAG_BIT_OR, 1, PRIORITY_BIT_OR, OS_PUSH);
				}
			case ':':
				switch (*(p + 1))
				{
				case ':':
					APPEND_TAG_WITH_CONTINUE(TAG_PROCEDURE, 2, PRIORITY_FUNCTION, OS_PUSH);
				case '!':
					APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_FUNCTION, 2, PRIORITY_FUNCTION, OS_PUSH);
				case '&':
					APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_REFERENCE, 2, PRIORITY_FUNCTION, OS_PUSH);
				case '=':
				case '+':
					APPEND_TAG_WITH_CONTINUE(TAG_SECTION, 2, PRIORITY_FUNCTION, OS_PUSH);
				case ']':
					iTag = TAG_REMOTE4;
					nLength = 2;
					goto APPEND_REMOTE;
				case 'L':
					switch (*(p + 2))
					{
					case ']':
						iTag = TAG_LOCAL4;
						nLength = 3;
						goto APPEND_REMOTE;
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
						if (*(p + 3) != ']')
							break;
						iTag = (TAG)(TAG_LOCAL1 + *(p + 2) - '1');
						nLength = 4;
						goto APPEND_REMOTE;
					}
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (*(p + 2) != ']')
						break;
					iTag = (TAG)(TAG_REMOTE1 + *(p + 1) - '1');
					nLength = 3;
				APPEND_REMOTE:
					APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_REMOTE, OS_PUSH | OS_CLOSE);
				}
				APPEND_TAG_WITH_CONTINUE(TAG_TERNARY_SPLIT, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARY);
			case '.':
				if (*(p + 1) != ']')
					break;
				APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE, 2, PRIORITY_ADDR_REPLACE, OS_PUSH | OS_CLOSE);
			case '_':
				if (*(p + 1) != ']')
					break;
				APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST, 2, PRIORITY_ADDR_ADJUST, OS_PUSH | OS_CLOSE);
			case ')':
				APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_CLOSE, 1, PRIORITY_PARENTHESIS_CLOSE, OS_CLOSE);
			case ',':
				APPEND_TAG_WITH_CONTINUE(TAG_PARAM_SPLIT, 1, PRIORITY_PARAM_SPLIT, OS_PUSH | OS_SPLIT);
			case '\0':
				APPEND_TAG_WITH_CONTINUE(TAG_SPLIT, 1, PRIORITY_SPLIT, OS_SPLIT);
			}
			p++;

		CONTINUE:;
			#undef APPEND_TAG
			#undef APPEND_TAG_WITH_CONTINUE
		}
		else
		{
			bIsLaedByte = TRUE;
			p += 2;
		}
	}

	// correct memmove
	if (nFirstMemmove != SIZE_MAX)
	{
		for (MARKUP *lpTag = lpTagArray + nFirstMemmove, *lpEndOfTag = lpTagArray + nNumberOfTag; lpTag < lpEndOfTag; lpTag++)
		{
			if (lpTag->Tag != TAG_MEMMOVE)
				continue;
			if (++lpTag >= lpEndOfTag)
				break;
			if (lpTag->Tag != TAG_PARENTHESIS_OPEN)
				continue;
			while (++lpTag < lpEndOfTag && lpTag->Tag != TAG_PARAM_SPLIT);
			if (lpTag >= lpEndOfTag)
				break;
			while (++lpTag < lpEndOfTag && lpTag->Tag != TAG_PARAM_SPLIT);
			if (lpTag >= lpEndOfTag)
				break;
			nDepth = 0;
			while (++lpTag < lpEndOfTag)
			{
				if (lpTag->Tag == TAG_PARENTHESIS_CLOSE)
				{
					if (!nDepth)
						break;
					nDepth--;
				}
				else if (lpTag->Tag == TAG_PARENTHESIS_OPEN)
				{
					nDepth++;
				}
			}
			if (lpTag >= lpEndOfTag)
				break;
			lpTag->Type |= OS_SPLIT | OS_PUSH | OS_MEMMOVE_END;
		}
	}

	// add ternary block
	if (nFirstTernary != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstTernary, *lpEndOfTag = lpTagArray + nNumberOfTag; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpBegin, *lpEnd;

			if (lpTag1->Tag != TAG_TERNARY)
				continue;
			lpBegin = NULL;
			nDepth = 0;
			for (MARKUP *lpTag2 = lpTag1 - 1; lpTag2 >= lpTagArray; lpTag2--)
			{
				if (lpTag2->Type & OS_CLOSE)
				{
					nDepth++;
					continue;
				}
				else if (lpTag2->Type & OS_OPEN)
				{
					if (nDepth)
					{
						nDepth--;
						continue;
					}
				}
				else if (nDepth || lpTag2->Priority > PRIORITY_TERNARY)
				{
					continue;
				}
				lpBegin = lpTag2;
				break;
			}
			lpEnd = NULL;
			nDepth = 0;
			for (MARKUP *lpTag2 = lpTag1 + 1; lpTag2 < lpEndOfTag; lpTag2++)
			{
				if (lpTag2->Type & OS_OPEN)
				{
					nDepth++;
					continue;
				}
				else if (lpTag2->Type & OS_CLOSE)
				{
					if (!nDepth)
						break;
					nDepth--;
					continue;
				}
				else if (nDepth || lpTag2->Tag != TAG_TERNARY_SPLIT)
				{
					continue;
				}
				nDepth = 0;
				for (lpTag2++; lpTag2 < lpEndOfTag; lpTag2++)
				{
					if (!(lpTag2->Type & OS_SPLIT) || nDepth)
					{
						if (!(lpTag2->Type & (OS_OPEN | OS_CLOSE)))
						{
							if (nDepth || lpTag2->Priority >= PRIORITY_TERNARY)
								continue;
						}
						else if (lpTag2->Type & OS_OPEN)
						{
							nDepth++;
							continue;
						}
						else if (nDepth)
						{
							nDepth--;
							continue;
						}
					}
					lpEnd = lpTag2;
					break;
				}
				break;
			}
			if (lpBegin && (lpBegin->Type & OS_OPEN) && lpEnd && (lpEnd->Type & OS_CLOSE))
			{
				lpEnd->Type |= OS_PUSH | OS_TERNARY_END;
				continue;
			}
			if (lpEnd)
				(LPBYTE)lpEnd -= (size_t)lpTagArray;
			(LPBYTE)lpTag1     -= (size_t)lpTagArray;
			(LPBYTE)lpEndOfTag -= (size_t)lpTagArray;
			if (lpBegin)
			{
				size_t  size;
				LPCVOID src;

				(LPBYTE)lpBegin -= (size_t)lpTagArray;
				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED2;
				size = (size_t)lpEndOfTag - (size_t)lpBegin;
				(LPBYTE)lpBegin += (size_t)lpTagArray;
				src = lpBegin++;
				memmove(lpBegin, src, size);
				lpBegin->String += lpBegin->Length;
			}
			else
			{
				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED2;
				memmove(lpTagArray + 1, lpTagArray, (size_t)lpEndOfTag);
				lpBegin = lpTagArray;
			}
			lpBegin->Tag      = TAG_SPLIT;
			lpBegin->Length   = 0;
			lpBegin->Priority = PRIORITY_PARENTHESIS_OPEN;
			lpBegin->Type     = OS_OPEN;
			if (lpEnd)
			{
				size_t size;

				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED2;
				size = (size_t)lpEndOfTag - (size_t)lpEnd;
				(LPBYTE)lpEnd += (size_t)(lpTagArray + 1);
				memmove(lpEnd + 1, lpEnd, size);
			}
			else
			{
				if (!(lpEnd = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
					goto FAILED2;
				lpEnd->String = lpMarkupStringBuffer + nStringLength;
			}
			(LPBYTE)lpTag1     += (size_t)(lpTagArray + 1);
			(LPBYTE)lpEndOfTag += (size_t)(lpTagArray + 2);
			lpEnd->Tag      = TAG_SPLIT;
			lpEnd->Length   = 0;
			lpEnd->Priority = PRIORITY_PARENTHESIS_CLOSE;
			lpEnd->Type     = OS_PUSH | OS_CLOSE | OS_TERNARY_END;
		}
	}

	// correct while loop
	if (nFirstWhile != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstWhile, *lpEndOfTag = lpTagArray + nNumberOfTag; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2;
			char   *p;

			if (lpTag1->Tag != TAG_WHILE)
				continue;
			lpTag2 = lpTag1;
			while (++lpTag2 < lpEndOfTag && lpTag2->Tag != TAG_PARENTHESIS_OPEN && lpTag2->Tag != TAG_SPLIT);
			if (lpTag2 >= lpEndOfTag || lpTag2->Tag == TAG_SPLIT)
				break;
			nDepth = 0;
			while (++lpTag2 < lpEndOfTag)
			{
				if (lpTag2->Tag == TAG_PARENTHESIS_CLOSE)
				{
					if (!nDepth)
						break;
					nDepth--;
				}
				else if (lpTag2->Tag == TAG_PARENTHESIS_OPEN)
				{
					nDepth++;
				}
			}
			if (lpTag2 >= lpEndOfTag || lpTag2->Tag != TAG_PARENTHESIS_CLOSE)
				break;
			lpTag2->Tag  = TAG_WHILE_EXPR;
			lpTag2->Type = OS_CLOSE | OS_PUSH;
			if (++lpTag2 >= lpEndOfTag)
				break;
			if (lpTag2->Tag == TAG_PARENTHESIS_OPEN)
			{
				nDepth = 0;
				while (++lpTag2 < lpEndOfTag)
				{
					if (lpTag2->Tag == TAG_PARENTHESIS_CLOSE)
					{
						if (!nDepth)
							break;
						nDepth--;
					}
					else if (lpTag2->Tag == TAG_PARENTHESIS_OPEN)
					{
						nDepth++;
					}
				}
				p = lpTag2 < lpEndOfTag ?
					lpTag2->String + lpTag2->Length :
					lpMarkupStringBuffer + nStringLength;
			}
			else
			{
				while (lpTag2->Tag != TAG_SPLIT && ++lpTag2 < lpEndOfTag);
				p = lpTag2 < lpEndOfTag ?
					lpTag2->String :
					lpMarkupStringBuffer + nStringLength;
			}
			if (lpTag2 < lpEndOfTag && lpTag2->Tag == TAG_SPLIT)
			{
				lpTag2->Type = OS_SPLIT | OS_PUSH | OS_WHILE_END;
				continue;
			}
			(LPBYTE)lpTag1     -= (size_t)lpTagArray;
			(LPBYTE)lpTag2     -= (size_t)lpTagArray;
			(LPBYTE)lpEndOfTag -= (size_t)lpTagArray;
			if (lpTag2 < lpEndOfTag)
			{
				size_t  size;
				LPCVOID src;

				size = (size_t)lpEndOfTag - (size_t)lpTag2;
				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED2;
				(LPBYTE)lpTag2 += (size_t)lpTagArray;
				src = lpTag2++;
				memmove(lpTag2, src, size);
			}
			else
			{
				if (!(lpTag2 = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
					goto FAILED2;
			}
			(LPBYTE)lpTag1     += (size_t)lpTagArray;
			(LPBYTE)lpEndOfTag += (size_t)(lpTagArray + 1);
			lpTag2->Tag      = TAG_SPLIT;
			lpTag2->Length   = 0;
			lpTag2->String   = p;
			lpTag2->Priority = PRIORITY_SPLIT;
			lpTag2->Type     = OS_SPLIT | OS_PUSH | OS_WHILE_END;
		}
	}

	// add last value
	if (nNumberOfTag)
	{
		LPSTR lpString;
		LPSTR lpEnd;

		lpString = lpTagArray[nNumberOfTag - 1].String + lpTagArray[nNumberOfTag - 1].Length;
		lpEnd = lpMarkupStringBuffer + nStringLength;
		if (lpEnd > lpString)
		{
			if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
				goto FAILED2;
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpEnd - lpString;
			lpMarkup->String   = lpString;
			lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type     = OS_PUSH;
			TrimMarkupString(lpMarkup);
			if (!lpMarkup->Length)
				nNumberOfTag--;
		}
	}
	else
	{
		lpMarkup = lpTagArray;
		lpMarkup->Tag      = TAG_NOT_OPERATOR;
		lpMarkup->Length   = nStringLength;
		lpMarkup->String   = lpMarkupStringBuffer;
		lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
		lpMarkup->Type     = OS_PUSH;
		TrimMarkupString(lpMarkup);
		if (!lpMarkup->Length)
			goto FAILED2;
		nNumberOfTag++;
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (!lpMarkupArray)
		goto FAILED2;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	nMarkupIndex = 0;
	nMarkupLength = 0;
	for (MARKUP *lpTag = lpTagArray, *lpEndOfTag = lpTagArray + nNumberOfTag; lpTag < lpEndOfTag; lpTag++)
	{
		nMarkupIndex += nMarkupLength;
		if ((size_t)(lpTag->String - lpMarkupStringBuffer) > nMarkupIndex)
		{
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpTag->String - lpMarkupStringBuffer - nMarkupIndex;
			lpMarkup->String   = lpMarkupStringBuffer + nMarkupIndex;
			lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type     = OS_PUSH;
			nMarkupIndex += lpMarkup->Length;
			TrimMarkupString(lpMarkup);
			if (lpMarkup->Length)
				lpMarkup++;
		}
		*lpMarkup = *lpTag;
		nMarkupLength = lpMarkup->Length;
		nMarkupIndex = lpMarkup->String - lpMarkupStringBuffer;
		lpMarkup++;
	}
	lpEndOfMarkup = lpMarkup;

	// release
	HeapFree(hHeap, 0, lpTagArray);

	nDepth = 0;
	for (lpMarkup = lpMarkupArray; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		// get depth of nested loop
		if (lpMarkup->Type & OS_WHILE_BEGIN)
		{
			lpMarkup->LoopDepth = nDepth++;
		}
		else
		{
			if ((lpMarkup->Type & OS_WHILE_END) && nDepth)
			{
				nDepth--;
			}
			lpMarkup->LoopDepth = nDepth;
		}

		// correct operators
		switch (lpMarkup->Tag)
		{
		case TAG_INC:
			if (lpMarkup + 1 < lpEndOfMarkup)
				if ((lpMarkup + 1)->Tag == TAG_NOT_OPERATOR)
					break;
			// post increment operator
			if (lpMarkup == lpMarkupArray)
				break;
			if ((lpMarkup - 1)->Tag != TAG_NOT_OPERATOR)
				break;
			lpMarkup->Type = OS_PUSH | OS_MONADIC | OS_POST;
			lpMarkup->Priority = PRIORITY_POST_INC;
			break;
		case TAG_DEC:
			if (lpMarkup + 1 < lpEndOfMarkup)
				if ((lpMarkup + 1)->Tag == TAG_NOT_OPERATOR)
					break;
			// post decrement operator
			if (lpMarkup == lpMarkupArray)
				break;
			if ((lpMarkup - 1)->Tag != TAG_NOT_OPERATOR)
				break;
			lpMarkup->Type = OS_PUSH | OS_MONADIC | OS_POST;
			lpMarkup->Priority = PRIORITY_POST_DEC;
			break;
		case TAG_ADD:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if ((lpMarkup - 1)->Tag == TAG_NOT_OPERATOR || ((lpMarkup - 1)->Type & (OS_CLOSE | OS_POST)))
					break;
			// plus-sign operator (remove)
			lpMarkup->Type = 0;
			break;
		case TAG_SUB:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if ((lpMarkup - 1)->Tag == TAG_NOT_OPERATOR || ((lpMarkup - 1)->Type & (OS_CLOSE | OS_POST)))
					break;
			// negative operator
			lpMarkup->Tag = TAG_NEG;
			lpMarkup->Priority = PRIORITY_NEG;
			lpMarkup->Type = OS_PUSH | OS_MONADIC;
			break;
		case TAG_MUL:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if ((lpMarkup - 1)->Tag == TAG_NOT_OPERATOR || ((lpMarkup - 1)->Type & (OS_CLOSE | OS_POST)))
					break;
			// indirection operator
			lpMarkup->Tag = TAG_INDIRECTION;
			lpMarkup->Priority = PRIORITY_INDIRECTION;
			lpMarkup->Type = OS_PUSH | OS_MONADIC;
			break;
		case TAG_MODULENAME:
			lpMarkup->Length--;
			break;
		}
	}

	// return
	*lppMarkupStringBuffer = lpMarkupStringBuffer;
	*lpnNumberOfMarkup = lpEndOfMarkup - lpMarkupArray;
	return lpMarkupArray;

FAILED2:
	HeapFree(hHeap, 0, lpTagArray);
FAILED1:
	HeapFree(hHeap, 0, lpMarkupStringBuffer);
	return NULL;
}
//---------------------------------------------------------------------
//「中置記法の文字列を、後置記法（逆ポーランド記法）に変換し
//	因子単位で格納したベクタを返す関数」
//---------------------------------------------------------------------
size_t Postfix(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT MARKUP **lpPostfixBuffer, IN MARKUP **lpFactorBuffer, IN size_t *lpnNestBuffer)
{
	MARKUP **lpPostfixTop, **lpEndOfPostfix;
	MARKUP *lpFactor, **lpFactorTop, **lpEndOfFactor;
	size_t *lpnNestTop, *lpnEndOfNest;
	MARKUP *lpMarkup, *lpEndOfMarkup;

	lpPostfixTop = lpEndOfPostfix = lpPostfixBuffer;
	lpFactorTop = lpEndOfFactor = lpFactorBuffer;
	lpnNestTop = lpnEndOfNest = lpnNestBuffer;

	#define POSTFIX_IS_EMPTY()  (lpEndOfPostfix == lpPostfixBuffer)
	#define POSTFIX_PUSH(value) (*(lpPostfixTop = lpEndOfPostfix++) = (value))
	#define POSTFIX_POP()       (!POSTFIX_IS_EMPTY() ? *(lpEndOfPostfix = lpPostfixTop != lpPostfixBuffer ? lpPostfixTop-- : lpPostfixTop) : NULL)
	#define FACTOR_IS_EMPTY()   (lpEndOfFactor == lpFactorBuffer)
	#define FACTOR_PUSH(value)  (*(lpFactorTop = lpEndOfFactor++) = (value))
	#define FACTOR_POP()        (!FACTOR_IS_EMPTY() ? *(lpEndOfFactor = lpFactorTop != lpFactorBuffer ? lpFactorTop-- : lpFactorTop) : NULL)
	#define NEST_IS_EMPTY()     (lpnEndOfNest == lpnNestBuffer)
	#define NEST_PUSH(value)    (*(lpnNestTop = lpnEndOfNest++) = (value))
	#define NEST_POP()          (!NEST_IS_EMPTY() ? *(lpnEndOfNest = lpnNestTop != lpnNestBuffer ? lpnNestTop-- : lpnNestTop) : 0)

	NEST_PUSH(0);
	for (lpMarkup = lpMarkupArray, lpEndOfMarkup = lpMarkupArray + nNumberOfMarkup; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		if (lpMarkup->Type & (OS_CLOSE | OS_LEFT_ASSIGN | OS_TERNARY))
		{
			if (lpMarkup->Type & (OS_CLOSE | OS_TERNARY))
			{
				for (; *lpnNestTop; (*lpnNestTop)--)
					POSTFIX_PUSH(FACTOR_POP());
				NEST_POP();
				if (*lpnNestTop && (lpFactorTop[1 - *lpnNestTop]->Type & OS_LEFT_ASSIGN))
				{
					for (; *lpnNestTop; (*lpnNestTop)--)
						POSTFIX_PUSH(FACTOR_POP());
					NEST_POP();
				}
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
				if (!(lpMarkup->Type & OS_CLOSE))
					NEST_PUSH(0);
			}
			else	// OS_LEFT_ASSIGN
			{
				if (lpMarkup != lpMarkupArray && ((lpMarkup - 1)->Type & OS_CLOSE))
				{
					MARKUP *lpPrev;
					size_t nDepth;

					lpPrev = lpMarkup;
					nDepth = 0;
					do
					{
						lpPrev--;
						if (!(lpPrev->Type & (OS_OPEN | OS_CLOSE)))
							continue;
						if (lpPrev->Type & OS_CLOSE)
							nDepth++;
						else if (!--nDepth)
							break;
					} while (lpPrev != lpMarkupArray);
					while (!POSTFIX_IS_EMPTY() && (*lpPostfixTop)->String >= lpPrev->String)
					{
						FACTOR_PUSH(POSTFIX_POP());
						(*lpnNestTop)++;
					}
				}
				lpFactorTop = lpEndOfFactor++;
				if (*lpnNestTop)
				{
					MARKUP **lpFactor;

					lpFactor = lpFactorTop - *lpnNestTop;
					memmove(lpFactor + 1, lpFactor, (size_t)lpFactorTop - (size_t)lpFactor);
					*lpFactor = lpMarkup;
				}
				else
				{
					*lpFactorTop = lpMarkup;
				}
				(*lpnNestTop)++;
				NEST_PUSH(0);
			}
			continue;
		}
		while (*lpnNestTop && lpMarkup->Priority <= (*lpFactorTop)->Priority)
		{
			POSTFIX_PUSH(FACTOR_POP());
			(*lpnNestTop)--;
		}
		if (lpMarkup->Type & (OS_OPEN | OS_PUSH))
		{
			if (lpMarkup->Type & OS_OPEN)
			{
				// 開始括弧ならスタックプッシュ
				NEST_PUSH(0);
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
			}
			else	// OS_PUSH
			{
				// 除去不可フラグが立っているならスタックへ
				FACTOR_PUSH(lpMarkup);
				(*lpnNestTop)++;
				if (lpMarkup->Type & OS_MONADIC)
				{
					MARKUP *lpNext;

					while ((lpNext = lpMarkup + 1) < lpEndOfMarkup && (lpNext->Type & OS_MONADIC))
					{
						FACTOR_PUSH(lpMarkup = lpNext);
						(*lpnNestTop)++;
					}
				}
			}
		}
	}
	while (lpFactor = FACTOR_POP())
		POSTFIX_PUSH(lpFactor);
	return lpEndOfPostfix - lpPostfixBuffer;

	#undef POSTFIX_PUSH
	#undef FACTOR_IS_EMPTY
	#undef FACTOR_PUSH
	#undef FACTOR_POP
	#undef NEST_IS_EMPTY
	#undef NEST_PUSH
	#undef NEST_POP
}
//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数」
//---------------------------------------------------------------------
QWORD __cdecl _Parsing(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, const bcb6_std_string *Src, BOOL IsInteger, BOOL IsQuad, va_list ArgPtr)
{
	#define PROCESS_DESIRED_ACCESS (PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION)

	const OPERAND                        OperandZero = { { 0, 0 }, FALSE };
	QWORD                                qwResult;
	LPSTR                                lpszSrc;
	size_t                               nSrcLength;
#if defined(ADDITIONAL_TAGS) && ADDITIONAL_TAGS
	size_t                               capacity;
	TEndWithAttribute                    *variable;
	bcb6_std_string                      *code;
	size_t                               nVariableLength;
	LPSTR                                p;
#if defined(__BORLANDC__)
	vector<TSSGAttributeElement *>       *attributes;
#else
	bcb6_std_vector_TSSGAttributeElement *attributes;
#endif
#endif
	LPSTR                                lpMarkupStringBuffer;
	size_t                               nNumberOfMarkup;
	MARKUP                               *lpMarkupArray;
	MARKUP                               **lpPostfix;
	MARKUP                               **lpFactorBuffer;
	size_t                               *lpnNestBuffer;
	OPERAND                              *lpOperandBuffer, *lpEndOfOperand, *lpOperandTop;
	MARKUP_VARIABLE                      *lpVariable;
	size_t                               nNumberOfVariable;
	size_t                               nNumberOfPostfix;
	size_t                               length;
	HANDLE                               hProcess;
	BOOLEAN                              bCompoundAssign;
	OPERAND                              operand;
#if defined(REPEAT_INDEX) && REPEAT_INDEX
	LPSTR                                lpVariableStringBuffer;
#endif

	qwResult = 0;
	lpszSrc = (LPSTR)bcb6_std_string_begin(Src);
#if defined(LOCAL_MEMORY_SUPPORT) && LOCAL_MEMORY_SUPPORT
	while (__intrinsic_isspace(*lpszSrc))
		lpszSrc++;
	if (*lpszSrc == 'L')
	{
		if (__intrinsic_isspace(lpszSrc[1]))
		{
			lpszSrc += 2;
			while (__intrinsic_isspace(*lpszSrc))
				lpszSrc++;
		}
		else if (lpszSrc[1] == '{')
		{
			lpszSrc++;
		}
	}
#endif
	nSrcLength = bcb6_std_string_end(Src) - lpszSrc;
#if defined(ADDITIONAL_TAGS) && ADDITIONAL_TAGS
	variable = (TEndWithAttribute *)TSSGCtrl_GetAttribute(SSGCtrl, SSGS, AT_VARIABLE);
	if (variable && (nVariableLength = bcb6_std_string_length(code = TEndWithAttribute_GetCode(variable))))
	{
		unsigned long bits;

#ifndef _WIN64
		if (_BitScanReverse(&bits, nVariableLength + nSrcLength))
#else
		if (_BitScanReverse64(&bits, nVariableLength + nSrcLength))
#endif
			capacity = (size_t)1 << (bits + 1);
		else
			capacity = 1;
		p = (LPSTR)HeapAlloc(hHeap, 0, capacity);
		if (!p)
			goto FAILED1;
		memcpy(p, bcb6_std_string_c_str(code), nVariableLength);
		memcpy(p + nVariableLength, lpszSrc, nSrcLength + 1);
		lpszSrc = p;
		nSrcLength += nVariableLength;
	}
	else
	{
		unsigned long bits;

#ifndef _WIN64
		if (_BitScanReverse(&bits, nSrcLength))
#else
		if (_BitScanReverse64(&bits, nSrcLength))
#endif
			capacity = (size_t)1 << (bits + 1);
		else
			capacity = 1;
		p = (LPSTR)HeapAlloc(hHeap, 0, capacity);
		if (!p)
			goto FAILED1;
		memcpy(p, lpszSrc, nSrcLength + 1);
		lpszSrc = p;
	}
	attributes = TSSGSubject_GetAttribute(SSGS);
	if (attributes)
	{
		nSrcLength = ReplaceDefineByHeap(attributes, &lpszSrc, nSrcLength, capacity);
		if (nSrcLength == SIZE_MAX)
			goto FAILED2;
	}
#if defined(LOCAL_MEMORY_SUPPORT) && LOCAL_MEMORY_SUPPORT
	p = lpszSrc;
	while (__intrinsic_isspace(*p))
		p++;
	if (p[0] == 'L' && __intrinsic_isspace(p[1]))
	{
		p += 2;
		while (__intrinsic_isspace(*p))
			p++;
		nSrcLength -= p - lpszSrc;
		memcpy(lpszSrc, p, nSrcLength + 1);
	}
#endif
#endif
	lpMarkupArray = Markup(lpszSrc, nSrcLength, &lpMarkupStringBuffer, &nNumberOfMarkup);
	if (!lpMarkupArray)
		goto FAILED2;
	lpPostfix = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpPostfix)
		goto FAILED3;
	lpFactorBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpFactorBuffer)
		goto FAILED4;
	lpnNestBuffer = (size_t *)HeapAlloc(hHeap, 0, sizeof(size_t) * (nNumberOfMarkup + 1));
	if (!lpnNestBuffer)
		goto FAILED5;
	lpOperandBuffer = (OPERAND *)HeapAlloc(hHeap, 0, sizeof(OPERAND) * (nNumberOfMarkup + 1));
	if (!lpOperandBuffer)
		goto FAILED6;
	lpVariable = (MARKUP_VARIABLE *)HeapAlloc(hHeap, 0, sizeof(MARKUP_VARIABLE) * 0x10);
	if (!lpVariable)
		goto FAILED7;

	nNumberOfPostfix = 0;
	for (size_t i = 0; i < nNumberOfMarkup; i++)
	{
		size_t j, nRange;

		j = i;
		while (!(lpMarkupArray[i].Type & OS_SPLIT) && ++i < nNumberOfMarkup);
		nRange = i - j;
		if (nRange)
			nNumberOfPostfix += Postfix(lpMarkupArray + j, nRange, lpPostfix + nNumberOfPostfix, lpFactorBuffer, lpnNestBuffer);
		if (i < nNumberOfMarkup && (lpMarkupArray[i].Type & OS_PUSH))
			lpPostfix[nNumberOfPostfix++] = lpMarkupArray + i;
	}

	#define OPERAND_IS_EMPTY()  (lpEndOfOperand == lpOperandBuffer)
	#define OPERAND_PUSH(value) (*(lpOperandTop = lpEndOfOperand++) = (value))
	#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpEndOfOperand = lpOperandTop != lpOperandBuffer ? lpOperandTop-- : lpOperandTop) : OperandZero)
	#define OPERAND_CLEAR()     (*(lpOperandTop = lpEndOfOperand = lpOperandBuffer) = OperandZero)

	hProcess = NULL;
	OPERAND_CLEAR();
	lpOperandTop->IsQuad = IsQuad;
#if !defined(SUBJECT_STATUS) || !SUBJECT_STATUS
	nNumberOfVariable = 0;
#else
	lpVariable[0].Length = 4;
	lpVariable[0].String = "Addr";
	lpVariable[0].Value.Quad = (QWORD)SSGS->address;
	lpVariable[0].IsQuad = sizeof(SSGS->address) > sizeof(DWORD);
	lpVariable[1].Length = 4;
	lpVariable[1].String = "Read";
	lpVariable[1].Value.Quad = (QWORD)SSGS->evaluateAtRead;
	lpVariable[1].IsQuad = sizeof(SSGS->evaluateAtRead) > sizeof(DWORD);
	lpVariable[2].Length = 4;
	lpVariable[2].String = "Size";
	lpVariable[2].Value.Quad = (QWORD)TSSGSubject_GetSize(SSGS);
	lpVariable[2].IsQuad = FALSE;
	nNumberOfVariable = 3;
#endif
	while (length = va_arg(ArgPtr, size_t))
	{
		lpVariable[nNumberOfVariable].Length = length;
		lpVariable[nNumberOfVariable].String = va_arg(ArgPtr, LPCSTR);
		lpVariable[nNumberOfVariable].Value.Quad = va_arg(ArgPtr, QWORD);
		lpVariable[nNumberOfVariable].IsQuad = IsQuad;
		nNumberOfVariable++;
	}
#if defined(REPEAT_INDEX) && REPEAT_INDEX
	do	/* do { ... } while (0); */
	{
		TSSGSubjectProperty *lpProperty;
		size_t              nPrevNumberOfVariable;
		size_t              nSize, nCapacity, nForward;

		lpVariableStringBuffer = (LPSTR)HeapAlloc(hHeap, 0, 32);
		if (!lpVariableStringBuffer)
			goto FAILED8;
		lpProperty = GetSubjectProperty(SSGS);
		if (!lpProperty)
			break;
		if (!lpProperty->RepeatDepth)
			break;
		nPrevNumberOfVariable = nNumberOfVariable;
		p = lpVariableStringBuffer;
		nSize = 0;
		nCapacity = 32;
		nForward = SIZE_MAX;
		do
		{
			if (nCapacity < nSize + (3 + 10 + 1) + (6 + 10 + 1))
			{
				LPVOID lpMem;

				lpMem = HeapReAlloc(hHeap, 0, lpVariableStringBuffer, nCapacity <<= 1);
				if (!lpMem)
					goto FAILED9;
				p += (size_t)lpMem - (size_t)lpVariableStringBuffer;
				lpVariableStringBuffer = (LPSTR)lpMem;
			}
			*(LPDWORD)p = BSWAP32('Idx\0');
			_ultoa(lpProperty->RepeatDepth, p + 3, 10);
			if (nNumberOfVariable && !(nNumberOfVariable & 0x0F))
			{
				LPVOID lpMem;
				size_t nBytes;

				nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
				lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
				if (!lpMem)
					goto FAILED9;
				lpVariable = (MARKUP_VARIABLE *)lpMem;
			}
			lpVariable[nNumberOfVariable].Length = nVariableLength = strlen(p + 3) + 3;
			nSize += ++nVariableLength;
			lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
			p += nVariableLength;
			lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
			lpVariable[nNumberOfVariable].IsQuad = FALSE;
			nNumberOfVariable++;
			if (++nForward)
			{
				*(LPDWORD) p      = BSWAP32('FwdI');
				*(LPWORD )(p + 4) = BSWAP16('dx'  );
				_ultoa(nForward, p + 6, 10);
				nVariableLength = strlen(p + 6) + 6;
			}
			else
			{
				*(LPDWORD)p = BSWAP32('Idx\0');
				nVariableLength = 3;
			}
			if (nNumberOfVariable && !(nNumberOfVariable & 0x0F))
			{
				LPVOID lpMem;
				size_t nBytes;

				nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
				lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
				if (!lpMem)
					goto FAILED9;
				lpVariable = (MARKUP_VARIABLE *)lpMem;
			}
			lpVariable[nNumberOfVariable].Length = nVariableLength;
			nSize += ++nVariableLength;
			lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
			p += nVariableLength;
			lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
			lpVariable[nNumberOfVariable].IsQuad = FALSE;
			nNumberOfVariable++;
		} while ((lpProperty = GetParentRepeat(lpProperty)) && lpProperty->RepeatDepth);
		for (size_t i = nPrevNumberOfVariable; i < nNumberOfVariable; i++)
		{
			lpVariable[i].String += (size_t)lpVariableStringBuffer;
		}
	} while (0);
#endif

	bCompoundAssign = FALSE;
	for (size_t i = 0; i < nNumberOfPostfix; i++)
	{
		MARKUP  *lpMarkup;
		size_t  nDepth;
		size_t  nSize;
		BOOLEAN boolValue;
		LPVOID  lpAddress;
		LPCSTR  lpGuideText;
#if !defined(__BORLANDC__)
		size_t  nGuideTextLength;
#endif

		lpMarkup = lpPostfix[i];
		switch (lpMarkup->Tag)
		{
		case TAG_WHILE:
			OPERAND_CLEAR();
		case TAG_MEMMOVE:
			break;
		case TAG_WHILE_EXPR:
			operand = OPERAND_POP();
			OPERAND_CLEAR();
			if (IsInteger)
			{
				if (operand.Value.Low || (!operand.IsQuad && operand.Value.High))
					continue;
			}
			else if (!operand.IsQuad)
			{
				if (operand.Value.Float)
					continue;
			}
			else
			{
				if (operand.Value.Double)
					continue;
			}
			nDepth = lpMarkup->LoopDepth;
			while (++i < nNumberOfPostfix && lpPostfix[i]->LoopDepth >= nDepth);
			continue;
		case TAG_SPLIT:
			if (lpMarkup->Type & OS_WHILE_END)
			{
				if (i && --i)
				{
					nDepth = lpMarkup->LoopDepth;
					while (lpPostfix[i]->LoopDepth != nDepth && --i);
				}
				OPERAND_CLEAR();
			}
		case TAG_MEMMOVE_LOCAL:
		case TAG_PARAM_SPLIT:
		case TAG_IMPORT_FUNCTION:
		case TAG_IMPORT_REFERENCE:
			continue;
		case TAG_PARENTHESIS_CLOSE:
			if (lpMarkup->Type & OS_MEMMOVE_END)
			{
				HANDLE     hDestProcess;
				void       *lpDest;
				HANDLE     hSrcProcess;
				const void *lpSrc;

				operand = OPERAND_POP();
				nSize = IsInteger ?
					(size_t)operand.Value.Quad :
					operand.IsQuad ?
						(size_t)operand.Value.Double :
						(size_t)operand.Value.Float;
				operand = OPERAND_POP();
				lpSrc = IsInteger ?
					(const void *)(INT_PTR)operand.Value.Quad :
					operand.IsQuad ?
						(const void *)(INT_PTR)operand.Value.Double :
						(const void *)(INT_PTR)operand.Value.Float;
				lpDest = IsInteger ?
					(void *)(INT_PTR)lpOperandTop->Value.Quad :
					lpOperandTop->IsQuad ?
						(void *)(INT_PTR)lpOperandTop->Value.Double :
						(void *)(INT_PTR)lpOperandTop->Value.Float;
				hDestProcess = (HANDLE)TRUE;
				hSrcProcess = (HANDLE)TRUE;
				do	/* do { ... } while (0); */
				{
					size_t j;

					j = i;
					while (lpPostfix[--j]->Tag != TAG_PARAM_SPLIT && j);
					if (lpPostfix[j]->Tag != TAG_PARAM_SPLIT)
						break;
					while (lpPostfix[--j]->Tag != TAG_PARAM_SPLIT && j);
					if (lpPostfix[j]->Tag != TAG_PARAM_SPLIT)
						break;
					if (lpPostfix[j + 1]->Tag == TAG_MEMMOVE_LOCAL)
						hSrcProcess = NULL;
					while (lpPostfix[--j]->Tag != TAG_MEMMOVE && j);
					if (lpPostfix[j]->Tag != TAG_MEMMOVE)
						break;
					if (lpPostfix[j + 1]->Tag == TAG_MEMMOVE_LOCAL)
						hDestProcess = NULL;
				} while (0);
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto FAILED9;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				if (!MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize))
					goto PARSING_ERROR;
			}
			continue;
		case TAG_ADD:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Value.Low += operand.Value.Low;
				else
					lpOperandTop->Value.Quad += operand.Value.Quad;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
				{
					if (!operand.IsQuad)
					{
						lpOperandTop->Value.Float += operand.Value.Float;
					}
					else
					{
						lpOperandTop->Value.Double = lpOperandTop->Value.Float + operand.Value.Double;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (operand.IsQuad)
						lpOperandTop->Value.Double += operand.Value.Double;
					else
						lpOperandTop->Value.Double += operand.Value.Float;
				}
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_SUB:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				OPERAND swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Value.Low -= operand.Value.Low;
				else
					lpOperandTop->Value.Quad -= operand.Value.Quad;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
				{
					if (!operand.IsQuad)
					{
						lpOperandTop->Value.Float -= operand.Value.Float;
					}
					else
					{
						lpOperandTop->Value.Double = lpOperandTop->Value.Float - operand.Value.Double;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (operand.IsQuad)
						lpOperandTop->Value.Double -= operand.Value.Double;
					else
						lpOperandTop->Value.Double -= operand.Value.Float;
				}
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_MUL:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Value.Low *= operand.Value.Low;
				else
					lpOperandTop->Value.Quad *= operand.Value.Quad;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
				{
					if (!operand.IsQuad)
					{
						lpOperandTop->Value.Float *= operand.Value.Float;
					}
					else
					{
						lpOperandTop->Value.Double = lpOperandTop->Value.Float * operand.Value.Double;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (operand.IsQuad)
						lpOperandTop->Value.Double *= operand.Value.Double;
					else
						lpOperandTop->Value.Double *= operand.Value.Float;
				}
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_IDIV:
			if (IsInteger)
			{
				BOOL IsQuad;

				operand = OPERAND_POP();
				if (!operand.Value.Low && (!operand.IsQuad || !operand.Value.High))
					goto FAILED10;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Value.Low = (long)lpOperandTop->Value.Low / (long)operand.Value.Low;
				else
					lpOperandTop->Value.Quad = (lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) / (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = IsQuad;
				break;
			}
		case TAG_DIV:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				OPERAND swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!operand.Value.Low && (!operand.IsQuad || !operand.Value.High))
					goto FAILED10;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Value.Low /= operand.Value.Low;
				else
					lpOperandTop->Value.Quad /= operand.Value.Quad;
			}
			else
			{
				if (!operand.IsQuad)
				{
					if (!operand.Value.Float)
						goto FAILED10;
					if (!lpOperandTop->IsQuad)
						lpOperandTop->Value.Float /= operand.Value.Float;
					else
						lpOperandTop->Value.Double /= operand.Value.Float;
				}
				else
				{
					if (!operand.Value.Double)
						goto FAILED10;
					if (lpOperandTop->IsQuad)
						lpOperandTop->Value.Double /= operand.Value.Double;
					else
					{
						lpOperandTop->Value.Double = lpOperandTop->Value.Float / operand.Value.Double;
						lpOperandTop->IsQuad = TRUE;
					}
				}
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_IMOD:
			if (IsInteger)
			{
				BOOL IsQuad;

				operand = OPERAND_POP();
				if (!operand.Value.Low && (!operand.IsQuad || !operand.Value.High))
					goto FAILED10;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Value.Low = (long)lpOperandTop->Value.Low % (long)operand.Value.Low;
				else
					lpOperandTop->Value.Quad = (lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) % (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = IsQuad;
				break;
			}
		case TAG_MOD:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				OPERAND swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!operand.Value.Low && (!operand.IsQuad || !operand.Value.High))
					goto FAILED10;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Value.Low %= operand.Value.Low;
				else
					lpOperandTop->Value.Quad %= operand.Value.Quad;
			}
			else
			{
				if (!operand.IsQuad)
				{
					if (!operand.Value.Float)
						goto FAILED10;
					if (!lpOperandTop->IsQuad)
						lpOperandTop->Value.Float = fmodf(lpOperandTop->Value.Float, operand.Value.Float);
					else
						lpOperandTop->Value.Double = fmod(lpOperandTop->Value.Double, operand.Value.Float);
				}
				else
				{
					if (!operand.Value.Double)
						goto FAILED10;
					if (lpOperandTop->IsQuad)
						lpOperandTop->Value.Double = fmod(lpOperandTop->Value.Double, operand.Value.Double);
					else
					{
						lpOperandTop->Value.Double = fmod(lpOperandTop->Value.Float, operand.Value.Double);
						lpOperandTop->IsQuad = TRUE;
					}
				}
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_NEG:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Low = -(long)lpOperandTop->Value.Low;
				else
					lpOperandTop->Value.Quad = -(LONG64)lpOperandTop->Value.Quad;
			}
			else
			{
				if (lpOperandTop->IsQuad)
					lpOperandTop->Value.Float -= operand.Value.Float;
				else
					lpOperandTop->Value.Double -= operand.Value.Double;
			}
			break;
		case TAG_SHL:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				OPERAND swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Low = operand.Value.Low < sizeof(DWORD) * 8 ? lpOperandTop->Value.Low << operand.Value.Low : 0;
				else
					lpOperandTop->Value.Quad = operand.Value.Quad < sizeof(QWORD) * 8 ? lpOperandTop->Value.Quad << operand.Value.Low : 0;
			}
			else
			{
				if (!operand.IsQuad)
					operand.Value.Low = (DWORD)operand.Value.Float;
				else
					operand.Value.Low = (DWORD)operand.Value.Double;
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Low = operand.Value.Low < sizeof(DWORD) * 8 ? lpOperandTop->Value.Low << operand.Value.Low : 0;
				else
					lpOperandTop->Value.Quad = operand.Value.Low < sizeof(QWORD) * 8 ? lpOperandTop->Value.Quad << operand.Value.Low : 0;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_SHR:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				OPERAND swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Low = operand.Value.Low < sizeof(DWORD) * 8 ? lpOperandTop->Value.Low >> operand.Value.Low : 0;
				else
					lpOperandTop->Value.Quad = operand.Value.Quad < sizeof(QWORD) * 8 ? lpOperandTop->Value.Quad >> operand.Value.Low : 0;
			}
			else
			{
				if (!operand.IsQuad)
					operand.Value.Low = (DWORD)operand.Value.Float;
				else
					operand.Value.Low = (DWORD)operand.Value.Double;
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Low = operand.Value.Low < sizeof(DWORD) * 8 ? lpOperandTop->Value.Low >> operand.Value.Low : 0;
				else
					lpOperandTop->Value.Quad = operand.Value.Low < sizeof(QWORD) * 8 ? lpOperandTop->Value.Quad >> operand.Value.Low : 0;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_SAR:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
				{
					if (operand.Value.Low > sizeof(DWORD) * 8)
						operand.Value.Low = sizeof(DWORD) * 8;
					lpOperandTop->Value.Low = (long)lpOperandTop->Value.Low >> operand.Value.Low;
				}
				else
				{
					if (operand.Value.Quad > sizeof(QWORD) * 8)
						operand.Value.Low = sizeof(QWORD) * 8;
					lpOperandTop->Value.Quad = (LONG64)lpOperandTop->Value.Quad >> operand.Value.Low;
				}
			}
			else
			{
				if (!operand.IsQuad)
					operand.Value.Low = (DWORD)operand.Value.Float;
				else
					operand.Value.Low = (DWORD)operand.Value.Double;
				if (!lpOperandTop->IsQuad)
				{
					if (operand.Value.Low > sizeof(DWORD) * 8)
						operand.Value.Low = sizeof(DWORD) * 8;
					lpOperandTop->Value.Low = (long)lpOperandTop->Value.Low >> operand.Value.Low;
				}
				else
				{
					if (operand.Value.Low > sizeof(QWORD) * 8)
						operand.Value.Low = sizeof(QWORD) * 8;
					lpOperandTop->Value.Quad = (LONG64)lpOperandTop->Value.Quad >> operand.Value.Low;
				}
			}
			break;
		case TAG_ROL:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
				{
					operand.Value.Low &= sizeof(DWORD) * 8 - 1;
					lpOperandTop->Value.Low = _lrotl(lpOperandTop->Value.Low, operand.Value.Low);
				}
				else
				{
					operand.Value.Low &= sizeof(QWORD) * 8 - 1;
					lpOperandTop->Value.Quad = _lrotl64(lpOperandTop->Value.Quad, operand.Value.Low);
				}
			}
			else
			{
				if (!operand.IsQuad)
					operand.Value.Low = (DWORD)operand.Value.Float;
				else
					operand.Value.Low = (DWORD)operand.Value.Double;
				if (!lpOperandTop->IsQuad)
				{
					operand.Value.Low &= sizeof(DWORD) * 8 - 1;
					lpOperandTop->Value.Low = _lrotl(lpOperandTop->Value.Low, operand.Value.Low);
				}
				else
				{
					operand.Value.Low &= sizeof(QWORD) * 8 - 1;
					lpOperandTop->Value.Quad = _lrotl64(lpOperandTop->Value.Quad, operand.Value.Low);
				}
			}
			break;
		case TAG_ROR:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
				{
					operand.Value.Low &= sizeof(DWORD) * 8 - 1;
					lpOperandTop->Value.Low = _lrotr(lpOperandTop->Value.Low, operand.Value.Low);
				}
				else
				{
					operand.Value.Low &= sizeof(QWORD) * 8 - 1;
					lpOperandTop->Value.Quad = _lrotr64(lpOperandTop->Value.Quad, operand.Value.Low);
				}
			}
			else
			{
				if (!operand.IsQuad)
					operand.Value.Low = (DWORD)operand.Value.Float;
				else
					operand.Value.Low = (DWORD)operand.Value.Double;
				if (!lpOperandTop->IsQuad)
				{
					operand.Value.Low &= sizeof(DWORD) * 8 - 1;
					lpOperandTop->Value.Low = _lrotr(lpOperandTop->Value.Low, operand.Value.Low);
				}
				else
				{
					operand.Value.Low &= sizeof(QWORD) * 8 - 1;
					lpOperandTop->Value.Quad = _lrotr64(lpOperandTop->Value.Quad, operand.Value.Low);
				}
			}
			break;
		case TAG_BIT_AND:
			operand = OPERAND_POP();
			lpOperandTop->Value.Quad &= operand.Value.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_OR:
			operand = OPERAND_POP();
			lpOperandTop->Value.Quad |= operand.Value.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_XOR:
			operand = OPERAND_POP();
			lpOperandTop->Value.Quad ^= operand.Value.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_NOT:
			lpOperandTop->Value.Low = ~lpOperandTop->Value.Low;
			if (lpOperandTop->IsQuad)
				lpOperandTop->Value.High = ~lpOperandTop->Value.High;
			break;
		case TAG_AND:
			if (IsInteger)
			{
				boolValue = lpOperandTop->Value.Quad ? TRUE : FALSE;
				if(!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Value.Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = boolValue = lpOperandTop->Value.Float ? TRUE : FALSE;
				else
					lpOperandTop->Value.Double = boolValue = lpOperandTop->Value.Double ? TRUE : FALSE;
			}
			if (lpMarkup->Type & OS_SHORT_CIRCUIT)
			{
				if (!boolValue)
				{
					while (++i < nNumberOfPostfix)
					{
						if (lpPostfix[i]->Tag != TAG_AND)
							continue;
						if (lpPostfix[i]->String != lpMarkup->String)
							continue;
						break;
					}
				}
				else
				{
					OPERAND_POP();
					continue;
				}
			}
			break;
		case TAG_OR:
			if (IsInteger)
			{
				boolValue = lpOperandTop->Value.Quad ? TRUE : FALSE;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Value.Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = boolValue = lpOperandTop->Value.Float ? TRUE : FALSE;
				else
					lpOperandTop->Value.Double = boolValue = lpOperandTop->Value.Double ? TRUE : FALSE;
			}
			if (lpMarkup->Type & OS_SHORT_CIRCUIT)
			{
				if (boolValue)
				{
					while (++i < nNumberOfPostfix)
					{
						if (lpPostfix[i]->Tag != TAG_OR)
							continue;
						if (lpPostfix[i]->String != lpMarkup->String)
							continue;
						break;
					}
				}
				else
				{
					OPERAND_POP();
					continue;
				}
			}
			break;
		case TAG_NOT:
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = !lpOperandTop->Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = !lpOperandTop->Value.Float;
				else
					lpOperandTop->Value.Double = !lpOperandTop->Value.Double;
			}
			break;
		case TAG_EQ:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad == operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float == (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double == (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_NE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad != operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float != (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double != (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_LT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Value.Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Value.Low < (long)operand.Value.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) < (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_BT:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad < operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float < (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double < (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_GT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Value.Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Value.Low >(long)operand.Value.Low :
				(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) > (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_AT:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad > operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float > (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double > (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_LE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Value.Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Value.Low <= (long)operand.Value.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) <= (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_BE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad <= operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float <= (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double <= (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_GE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Value.Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Value.Low >= (long)operand.Value.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Quad : (long)lpOperandTop->Value.Low) >= (operand.IsQuad ? (__int64)operand.Value.Quad : (long)operand.Value.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_AE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = lpOperandTop->Value.Quad >= operand.Value.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(lpOperandTop->Value.Float >= (!operand.IsQuad ? operand.Value.Float : operand.Value.Double));
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Double >= (!operand.IsQuad ? operand.Value.Float : operand.Value.Double);
			}
			break;
		case TAG_TERNARY:
			operand = OPERAND_POP();
			if (IsInteger ? !operand.Value.Quad : !operand.IsQuad ? !operand.Value.Float : !operand.Value.Double)
			{
				nDepth = 0;
				while (++i < nNumberOfPostfix)
					if (lpPostfix[i]->Tag == TAG_TERNARY)
					{
						nDepth++;
					}
					else if (lpPostfix[i]->Tag == TAG_TERNARY_SPLIT)
					{
						if (!nDepth)
							break;
						nDepth--;
					}
			}
			break;
		case TAG_TERNARY_SPLIT:
			for (; i + 1 < nNumberOfPostfix; i++)
				if (lpPostfix[i + 1]->Type & (OS_LEFT_ASSIGN | OS_TERNARY_END))
				{
					if (lpPostfix[i + 1]->Type & OS_RIGHT_ASSIGN)
						i--;
					break;
				}
			continue;
		case TAG_INDIRECTION:
			nSize = sizeof(LPVOID);
			goto PROCESS_MEMORY;
		case TAG_REMOTE1:
		case TAG_REMOTE2:
		case TAG_REMOTE3:
		case TAG_REMOTE4:
		case TAG_REMOTE5:
		case TAG_REMOTE6:
		case TAG_REMOTE7:
		case TAG_REMOTE8:
			nSize = lpMarkup->Tag - TAG_REMOTE1 + 1;
		PROCESS_MEMORY:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			{
				MARKUP *lpNext;

				if (bCompoundAssign)
				{
					OPERAND op1 = OPERAND_POP();
					OPERAND op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
				}
				if (IsInteger)
					lpAddress = (LPVOID)lpOperandTop->Value.Quad;
				else
					lpAddress = (LPVOID)(!lpOperandTop->IsQuad ? (INT_PTR)lpOperandTop->Value.Float : (INT_PTR)lpOperandTop->Value.Double);
				if (!bCompoundAssign && (!(lpNext = i + 1 < nNumberOfPostfix ? lpPostfix[i + 1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
				{
					bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
					if (bCompoundAssign)
					{
						OPERAND op1 = OPERAND_POP();
						OPERAND op2 = *lpOperandTop;
						*lpOperandTop = op1;
						OPERAND_PUSH(op2);
						OPERAND_PUSH(op2);
					}
					lpOperandTop->Value.Quad = 0;
					if (!ReadProcessMemory(hProcess, lpAddress, &lpOperandTop->Value.Quad, nSize, NULL))
						lpOperandTop->Value.Quad = 0;
					lpOperandTop->IsQuad = nSize > sizeof(DWORD);
				}
				else
				{
					OPERAND_POP();
					if (!WriteProcessMemory(hProcess, lpAddress, &lpOperandTop->Value.Quad, nSize, NULL))
						goto WRITE_ERROR;
					i++;
					if (bCompoundAssign)
					{
						bCompoundAssign = FALSE;
						continue;
					}
				}
			}
			if (lpMarkup->Tag != TAG_INDIRECTION)
				break;
			if (!TSSGCtrl_GetSSGActionListner(SSGCtrl))
				continue;
			lpGuideText = "* 間接参照";
#if !defined(__BORLANDC__)
			nGuideTextLength = 10;
#endif
			goto OUTPUT_GUIDE;
#if defined(LOCAL_MEMORY_SUPPORT) && LOCAL_MEMORY_SUPPORT
		case TAG_LOCAL1:
		case TAG_LOCAL2:
		case TAG_LOCAL3:
		case TAG_LOCAL4:
		case TAG_LOCAL5:
		case TAG_LOCAL6:
		case TAG_LOCAL7:
		case TAG_LOCAL8:
			{
				size_t nSize;
				MARKUP *lpNext;

				nSize = lpMarkup->Tag - TAG_LOCAL1 + 1;
				if (bCompoundAssign)
				{
					OPERAND op1 = OPERAND_POP();
					OPERAND op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
				}
				if (IsInteger)
					lpAddress = (LPVOID)lpOperandTop->Value.Quad;
				else
					lpAddress = (LPVOID)(!lpOperandTop->IsQuad ? (INT_PTR)lpOperandTop->Value.Float : (INT_PTR)lpOperandTop->Value.Double);
				if (!bCompoundAssign && (!(lpNext = i + 1 < nNumberOfPostfix ? lpPostfix[i + 1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
				{
					bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
					if (bCompoundAssign)
					{
						OPERAND op1 = OPERAND_POP();
						OPERAND op2 = *lpOperandTop;
						*lpOperandTop = op1;
						OPERAND_PUSH(op2);
						OPERAND_PUSH(op2);
					}
					lpOperandTop->IsQuad = nSize > sizeof(DWORD);
					if (!IsBadReadPtr(lpAddress, nSize))
					{
						switch (nSize)
						{
						case 1:
							lpOperandTop->Value.Quad = *(LPBYTE)lpAddress;
							break;
						case 2:
							lpOperandTop->Value.Quad = *(LPWORD)lpAddress;
							break;
						case 3:
							lpOperandTop->Value.Quad = *(LPWORD)lpAddress | ((DWORD)*((LPBYTE)lpAddress + 2) << 16);
							break;
						case 4:
							lpOperandTop->Value.Quad = *(LPDWORD)lpAddress;
							break;
						case 5:
							lpOperandTop->Value.Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPBYTE)lpAddress + 4) << 32);
							break;
						case 6:
							lpOperandTop->Value.Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPWORD)lpAddress + 2) << 32);
							break;
						case 7:
							lpOperandTop->Value.Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPWORD)lpAddress + 2) << 32) | ((QWORD)*((LPBYTE)lpAddress + 6) << 48);
							break;
						case 8:
							lpOperandTop->Value.Quad = *(LPQWORD)lpAddress;
							break;
						}
					}
					else
					{
						lpOperandTop->Value.Quad = 0;
					}
				}
				else
				{
					if (IsBadWritePtr(lpAddress, nSize))
						goto WRITE_ERROR;
					OPERAND_POP();
					switch (nSize)
					{
					case 1:
						*(LPBYTE)lpAddress = (BYTE)lpOperandTop->Value.Low;
						break;
					case 2:
						*(LPWORD)lpAddress = (WORD)lpOperandTop->Value.Low;
						break;
					case 3:
						*(LPWORD)lpAddress = (WORD)lpOperandTop->Value.Low;
						*((LPBYTE)lpAddress + 2) = (BYTE)(lpOperandTop->Value.Low >> 16);
						break;
					case 4:
						*(LPDWORD)lpAddress = lpOperandTop->Value.Low;
						break;
					case 5:
						*(LPDWORD)lpAddress = lpOperandTop->Value.Low;
						*((LPBYTE)lpAddress + 4) = (BYTE)lpOperandTop->Value.High;
						break;
					case 6:
						*(LPDWORD)lpAddress = lpOperandTop->Value.Low;
						*((LPWORD)lpAddress + 2) = (WORD)lpOperandTop->Value.High;
						break;
					case 7:
						*(LPDWORD)lpAddress = lpOperandTop->Value.Low;
						*((LPWORD)lpAddress + 2) = (WORD)lpOperandTop->Value.High;
						*((LPBYTE)lpAddress + 6) = (BYTE)(lpOperandTop->Value.High >> 16);
						break;
					case 8:
						*(LPQWORD)lpAddress = lpOperandTop->Value.Quad;
						break;
					}
					i++;
					if (bCompoundAssign)
					{
						bCompoundAssign = FALSE;
						continue;
					}
				}
			}
			break;
#endif
		WRITE_ERROR:
			TSSGActionListner_OnSubjectWriteError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (unsigned long)lpAddress);
			goto FAILED10;
		case TAG_REV_ENDIAN2:
			lpOperandTop->Value.Quad = __intrinsic_bswap16((WORD)lpOperandTop->Value.Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN3:
			lpOperandTop->Value.Quad = __intrinsic_bswap24(lpOperandTop->Value.Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN4:
			lpOperandTop->Value.Quad = __intrinsic_bswap32(lpOperandTop->Value.Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN5:
			lpOperandTop->Value.Quad = __intrinsic_bswap40(lpOperandTop->Value.Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.High = 0;
			break;
		case TAG_REV_ENDIAN6:
			lpOperandTop->Value.Quad = __intrinsic_bswap48(lpOperandTop->Value.Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.High = 0;
			break;
		case TAG_REV_ENDIAN7:
			lpOperandTop->Value.Quad = __intrinsic_bswap56(lpOperandTop->Value.Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.High = 0;
			break;
		case TAG_REV_ENDIAN8:
			lpOperandTop->Value.Quad = __intrinsic_bswap64(lpOperandTop->Value.Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.High = 0;
			break;
		case TAG_ADDR_REPLACE:
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, &lpOperandTop->Value.Low, AT_REPLACE) != 0)
				goto FAILED10;
			if (IsInteger)
			{
				lpOperandTop->Value.High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)lpOperandTop->Value.Low;
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Low;
			}
			break;
		case TAG_ADDR_ADJUST:
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, &lpOperandTop->Value.Low, AT_ADJUST) != 0)
				goto FAILED10;
			if (IsInteger)
			{
				lpOperandTop->Value.High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)lpOperandTop->Value.Low;
				else
					lpOperandTop->Value.Double = lpOperandTop->Value.Low;
			}
			break;
		case TAG_LEFT_ASSIGN:
			if (i + 1 < nNumberOfPostfix)
			{
				MARKUP *lpNext;
				size_t length;
				LPSTR  p;
				size_t j;

				lpNext = lpPostfix[i + 1];
				if (lpNext->Tag != TAG_NOT_OPERATOR)
					goto PARSING_ERROR;
				if (*lpNext->String == '$')
				{
					length = lpNext->Length - 1;
					if (!length)
						goto PARSING_ERROR;
					p = lpNext->String + 1;
				}
				else
				{
					length = lpNext->Length;
					p = lpNext->String;
				}
				for (j = 0; j < nNumberOfVariable; j++)
					if (lpVariable[j].Length == length)
						if (memcmp(lpVariable[j].String, p, length) == 0)
							break;
				if (j < nNumberOfVariable)
				{
					lpVariable[j].Value = lpOperandTop->Value;
					lpVariable[j].IsQuad = lpOperandTop->IsQuad;
				}
				else
				{
					if (!(nNumberOfVariable & 0x0F))
					{
						LPVOID lpMem;
						size_t nBytes;

						nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
						lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
						if (!lpMem)
							goto FAILED10;
						lpVariable = (MARKUP_VARIABLE *)lpMem;
					}
					lpVariable[nNumberOfVariable].Length = length;
					lpVariable[nNumberOfVariable].String = p;
					lpVariable[nNumberOfVariable].Value = lpOperandTop->Value;
					lpVariable[nNumberOfVariable].IsQuad = lpOperandTop->IsQuad;
					nNumberOfVariable++;
				}
				i++;
				break;
			}
			goto PARSING_ERROR;
		case TAG_STRLEN:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = StringLengthA(hProcess, (LPCSTR)lpOperandTop->Value.Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(DWORD);
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)StringLengthA(hProcess, (LPCSTR)(size_t)lpOperandTop->Value.Float);
				else
					lpOperandTop->Value.Double = StringLengthA(hProcess, (LPCSTR)(size_t)lpOperandTop->Value.Double);
			}
			break;
		case TAG_WCSLEN:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			if (IsInteger)
			{
				lpOperandTop->Value.Quad = StringLengthW(hProcess, (LPCWSTR)lpOperandTop->Value.Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(DWORD);
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)StringLengthW(hProcess, (LPCWSTR)(size_t)lpOperandTop->Value.Float);
				else
					lpOperandTop->Value.Double = StringLengthW(hProcess, (LPCWSTR)(size_t)lpOperandTop->Value.Double);
			}
			break;
		case TAG_BSF:
			if (!lpOperandTop->IsQuad)
			{
				long Mask;

				Mask = lpOperandTop->Value.Low;
				if (!_BitScanForward(&lpOperandTop->Value.Low, Mask))
					lpOperandTop->Value.Low = 0xFFFFFFFF;
			}
			else
			{
				unsigned __int64 Mask;

				Mask = lpOperandTop->Value.Quad;
				if (!_BitScanForward64(&lpOperandTop->Value.Low, Mask))
					lpOperandTop->Value.Low = 0xFFFFFFFF;
			}
			if (IsInteger)
			{
				lpOperandTop->Value.High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(long)lpOperandTop->Value.Low;
				else
					lpOperandTop->Value.Double = (long)lpOperandTop->Value.Low;
			}
			break;
		case TAG_BSR:
			if (!lpOperandTop->IsQuad)
			{
				long Mask;

				Mask = lpOperandTop->Value.Low;
				if (!_BitScanReverse(&lpOperandTop->Value.Low, Mask))
					lpOperandTop->Value.Low = 0xFFFFFFFF;
			}
			else
			{
				unsigned __int64 Mask;

				Mask = lpOperandTop->Value.Quad;
				if (!_BitScanReverse64(&lpOperandTop->Value.Low, Mask))
					lpOperandTop->Value.Low = 0xFFFFFFFF;
			}
			if (IsInteger)
			{
				lpOperandTop->Value.High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(long)lpOperandTop->Value.Low;
				else
					lpOperandTop->Value.Double = (long)lpOperandTop->Value.Low;
			}
			break;
		case TAG_CAST32:
			if (IsInteger)
			{
				lpOperandTop->Value.High = 0;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Value.Float = (float)lpOperandTop->Value.Double;
				lpOperandTop->Value.High = 0;
			}
			lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_CAST64:
			if (!IsInteger && !lpOperandTop->IsQuad)
				lpOperandTop->Value.Double = lpOperandTop->Value.Float;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_I1TOI4:
			lpOperandTop->Value.Quad = (long)(char)lpOperandTop->Value.Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I2TOI4:
			lpOperandTop->Value.Quad = (long)(short)lpOperandTop->Value.Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I4TOI8:
			lpOperandTop->Value.Quad = (__int64)(long)lpOperandTop->Value.Low;
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.High = 0;
			break;
#if defined(ALLOCATE_SUPPORT) && ALLOCATE_SUPPORT
		case TAG_MEMORY:
			{
				lpAddress = NULL;
				if (!IsInteger)
					lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
				if (!lpOperandTop->Value.High)
				{
					for (size_t j = 0; j < nNumberOfProcessMemory; j++)
					{
						if (lpOperandTop->Value.Low != lpProcessMemory[j].Id)
							continue;
						if (!lpProcessMemory[j].Address)
						{
							if (lpProcessMemory[j].Protect)
							{
								if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
									goto FAILED9;
								lpProcessMemory[j].Address = VirtualAllocEx(hProcess, NULL, lpProcessMemory[j].Size, MEM_COMMIT, lpProcessMemory[j].Protect);
							}
							else
							{
								lpProcessMemory[j].Address = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, lpProcessMemory[j].Size);
							}
						}
						lpAddress = lpProcessMemory[j].Address;
						break;
					}
				}
				lpOperandTop->Value.Quad = (QWORD)lpAddress;
			}
			if (IsInteger)
				lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(DWORD);
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
			else
				lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
			break;
#endif
		case TAG_PROCEDURE:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if ((lpMarkup + 1)->Priority <= lpMarkup->Priority)
				break;
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			operand = OPERAND_POP();
			if (!IsInteger)
				lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
			lpOperandTop->Value.Quad =
				!operand.Value.High && IS_INTRESOURCE(operand.Value.Low) ?
				(QWORD)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Value.Quad, (LPSTR)operand.Value.Quad) :
				0;
			if (IsInteger)
				lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
			else
				lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
			break;
		case TAG_MODULENAME:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if ((lpMarkup + 1)->Priority <= lpMarkup->Priority)
				break;
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			operand = OPERAND_POP();
			if (!IsInteger)
				lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
			if (!operand.Value.High && IS_INTRESOURCE(operand.Value.Low))
			{
				char   c;
				LPVOID lpFunction;

				c = lpMarkup->String[lpMarkup->Length];
				lpMarkup->String[lpMarkup->Length] = '\0';
				lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Value.Quad, lpMarkup->String, (LPSTR)operand.Value.Quad);
				if (lpFunction && (lpMarkup - 1)->Tag == TAG_IMPORT_FUNCTION)
					if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
						lpFunction = NULL;
				lpOperandTop->Value.Quad = (QWORD)lpFunction;
				lpMarkup->String[lpMarkup->Length] = c;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
				else if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
				else
					lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
			}
			else
			{
				lpOperandTop->Value.Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
				else if (!lpOperandTop->IsQuad)
					lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
				else
					lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
			}
			break;
		case TAG_HNUMBER:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if ((lpMarkup + 1)->Priority <= lpMarkup->Priority)
				break;
			if (!IsInteger)
				lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
			if (!lpOperandTop->Value.High)
			{
				THeapListData *HeapL;

				HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, lpOperandTop->Value.Low - 1);
				lpOperandTop->Value.Low = HeapL ? HeapL->heapListAddress : 0;
			}
			else
			{
				lpOperandTop->Value.Quad = 0;
			}
			if (IsInteger)
				lpOperandTop->IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(DWORD);
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
			else
				lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
			break;
		case TAG_NOT_OPERATOR:
			{
				size_t          length;
				LPSTR           p, end;
				MARKUP_VARIABLE *element;
				char            *endptr;
				MARKUP          *lpNext;
				LPSTR           lpEndOfModuleName;
				LPSTR           lpModuleName;
				char            c;

				length = lpMarkup->Length;
				p = lpMarkup->String;
				end = p + length;
				element = NULL;
				endptr = NULL;
				do	/* do { ... } while (0); */
				{
					size_t j;

					if (!length)
						break;
					if (*p == '$')
					{
						length--;
						p++;
					}
					if (!length)
						break;
					if (IsInteger)
					{
						operand.Value.Quad = _strtoui64(p, &endptr, 0);
					}
					else
					{
						if (!IsQuad)
							operand.Value.Float = (float)strtod(p, &endptr);
						else
							operand.Value.Double = strtod(p, &endptr);
						if (endptr == end)
							break;
						endptr = p;
						if (*endptr != '0')
							break;
						if (lpMarkup->Length > 18)
						{
							c = *(endptr + 1);
							if (c == 'x' || c == 'X')
								break;
							if (lpMarkup->Length >= 23)
							{
								if (lpMarkup->Length != 23)
									break;
								if (c != '1')
									break;
								endptr++;
								while ((BYTE)(c = *(++endptr)) >= (BYTE)'0' && (BYTE)c <= (BYTE)'7');
								if (endptr != end)
									break;
							}
						}
						operand.Value.Quad = _strtoui64(p, &endptr, 0);
						if (endptr == end)
						{
							if (!IsQuad)
							{
								operand.Value.Float = (float)(QWORD)operand.Value.Quad;
								if (bcb6__isnan(operand.Value.Float))
									endptr = p;
							}
							else
							{
								operand.Value.Double = (double)(QWORD)operand.Value.Quad;
								if (bcb6__isnan(operand.Value.Double))
									endptr = p;
							}
						}
					}
					if (endptr == end)
						break;
					for (j = 0; j < nNumberOfVariable; j++)
					{
						if (lpVariable[j].Length != length)
							continue;
						if (memcmp(lpVariable[j].String, p, length) != 0)
							continue;
						element = lpVariable + j;
						break;
					}
				} while (0);
				lpNext = i + 1 < nNumberOfPostfix ? lpPostfix[i + 1] : NULL;
				switch (lpNext ? lpNext->Tag : ~0)
				{
				case TAG_INC:
					if (!element)
						break;
					operand.Value = element->Value;
					operand.IsQuad = element->IsQuad;
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Value.Low = ++element->Value.Low;
							else
								operand.Value.Quad = ++element->Value.Quad;
						}
						else
						{
							if (!lpOperandTop->IsQuad)
								operand.Value.Float = element->Value.Float += 1;
							else
								operand.Value.Double = element->Value.Double += 1;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Value.Low = element->Value.Low++;
							else
								operand.Value.Quad = element->Value.Quad++;
						}
						else
						{
							if (!lpOperandTop->IsQuad)
							{
								operand.Value.Float = element->Value.Float;
								element->Value.Float += 1;
							}
							else
							{
								operand.Value.Double = element->Value.Double;
								element->Value.Double += 1;
							}
						}
					}
					OPERAND_PUSH(operand);
					i++;
					if (!TSSGCtrl_GetSSGActionListner(SSGCtrl))
						continue;
					lpGuideText = lpNext->Type & OS_POST ? "++ 後置" : "++ 前置";
#if !defined(__BORLANDC__)
					nGuideTextLength = 7;
#endif
					goto OUTPUT_GUIDE;
				case TAG_DEC:
					if (!element)
						break;
					operand.Value = element->Value;
					operand.IsQuad = element->IsQuad;
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Value.Low = --element->Value.Low;
							else
								operand.Value.Quad = --element->Value.Quad;
						}
						else
						{
							if (!lpOperandTop->IsQuad)
								operand.Value.Float = element->Value.Float -= 1;
							else
								operand.Value.Double = element->Value.Double -= 1;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Value.Low = element->Value.Low--;
							else
								operand.Value.Quad = element->Value.Quad--;
						}
						else
						{
							if (!lpOperandTop->IsQuad)
							{
								operand.Value.Float = element->Value.Float;
								element->Value.Float -= 1;
							}
							else
							{
								operand.Value.Double = element->Value.Double;
								element->Value.Double -= 1;
							}
						}
					}
					OPERAND_PUSH(operand);
					i++;
					if (!TSSGCtrl_GetSSGActionListner(SSGCtrl))
						continue;
					lpGuideText = lpNext->Type & OS_POST ? "-- 後置" : "-- 前置";
#if !defined(__BORLANDC__)
					nGuideTextLength = 7;
#endif
					goto OUTPUT_GUIDE;
				case TAG_RIGHT_ASSIGN:
				case TAG_LEFT_ASSIGN:
				ASSIGN:
					if (!length)
						break;
					if (!element)
					{
						if (!(nNumberOfVariable & 0x0F))
						{
							LPVOID lpMem;
							size_t nBytes;

							nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
							lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
							if (!lpMem)
								goto FAILED10;
							lpVariable = (MARKUP_VARIABLE *)lpMem;
						}
						element = lpVariable + nNumberOfVariable++;
						element->Length = length;
						element->String = p;
					}
					element->Value = lpOperandTop->Value;
					element->IsQuad = lpOperandTop->IsQuad;
					i++;
					if (bCompoundAssign)
					{
						bCompoundAssign = FALSE;
						continue;
					}
					break;
				case TAG_MNAME:
					{
						LPMODULEENTRY32A lpme;

						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpme = TProcessCtrl_GetModuleFromName(&SSGCtrl->processCtrl, lpMarkup->String);
						lpMarkup->String[lpMarkup->Length] = c;
						operand.Value.Quad = lpme ? (QWORD)lpme->hModule : 0;
						operand.IsQuad = sizeof(HMODULE) > sizeof(DWORD);
						OPERAND_PUSH(operand);
					}
					i++;
					break;
				case TAG_PROCEDURE:
					if ((HMODULE)lpOperandTop->Value.Quad)
					{
						LPSTR lpProcName;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						if (endptr != end && element)
						{
							endptr = end;
							operand.Value = element->Value;
							operand.IsQuad = element->IsQuad;
						}
						if (!IsInteger && endptr == end)
						{
							if (!lpOperandTop->IsQuad)
								operand.Value.Quad = (QWORD)operand.Value.Float;
							else
								operand.Value.Quad = (QWORD)operand.Value.Double;
						}
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.Value.High || !IS_INTRESOURCE(operand.Value.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Value.Quad;
						if (!IsInteger)
							lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpOperandTop->Value.Quad = (QWORD)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Value.Quad, lpProcName);
						lpMarkup->String[lpMarkup->Length] = c;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
						else if (!lpOperandTop->IsQuad)
							lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
						else
							lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
					}
					i++;
					break;
				case TAG_MODULENAME:
					lpEndOfModuleName = (lpModuleName = lpNext->String) + lpNext->Length;
					c = *lpEndOfModuleName;
					*lpEndOfModuleName = '\0';
					if (TSSGCtrl_GetSSGActionListner(SSGCtrl))
					{
#if defined(__BORLANDC__)
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpModuleName, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpModuleName, 0);
#else
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(lpModuleName, lpNext->Length, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(lpModuleName, lpNext->Length, 0);
#endif
					}
					i++;
					lpNext--;
					goto GET_IMPORT_FUNCTION;
				case TAG_IMPORT_FUNCTION:
				case TAG_IMPORT_REFERENCE:
					lpEndOfModuleName = lpModuleName = NULL;
				GET_IMPORT_FUNCTION:
					if ((HMODULE)lpOperandTop->Value.Quad)
					{
						LPSTR  lpProcName;
						char   c2;
						LPVOID lpFunction;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						if (endptr != end && element)
						{
							endptr = end;
							operand.Value = element->Value;
							operand.IsQuad = element->IsQuad;
						}
						if (!IsInteger && endptr == end)
						{
							if (!lpOperandTop->IsQuad)
								operand.Value.Quad = (QWORD)operand.Value.Float;
							else
								operand.Value.Quad = (QWORD)operand.Value.Double;
						}
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.Value.High || !IS_INTRESOURCE(operand.Value.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Value.Quad;
						if (!IsInteger)
							lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
						c2 = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Value.Quad, lpModuleName, lpProcName);
						if (lpFunction && lpNext->Tag == TAG_IMPORT_FUNCTION)
							if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
								lpFunction = NULL;
						lpOperandTop->Value.Quad = (QWORD)lpFunction;
						lpMarkup->String[lpMarkup->Length] = c2;
						if (lpEndOfModuleName)
							*lpEndOfModuleName = c;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
						else if (!lpOperandTop->IsQuad)
							lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
						else
							lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
					}
					i++;
					break;
				case TAG_SECTION:
					if ((HMODULE)lpOperandTop->Value.Quad)
					{
						BOOL  IsEndOfSection;
						DWORD dwSectionSize;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						IsEndOfSection = *(LPWORD)lpNext->String == BSWAP16(':+');
						if (!IsInteger)
							lpOperandTop->Value.Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Value.Float : (__int64)lpOperandTop->Value.Double;
						lpOperandTop->Value.Quad = (QWORD)GetSectionAddress(hProcess, (HMODULE)lpOperandTop->Value.Quad, lpMarkup->String, IsEndOfSection ? &dwSectionSize : NULL);
						if (IsEndOfSection)
							lpOperandTop->Value.Quad += dwSectionSize;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(DWORD);
						else if (!lpOperandTop->IsQuad)
							lpOperandTop->Value.Float = (float)(size_t)lpOperandTop->Value.Quad;
						else
							lpOperandTop->Value.Double = (size_t)lpOperandTop->Value.Quad;
						lpMarkup->String[lpMarkup->Length] = c;
					}
					i++;
					break;
				case TAG_HNUMBER:
					if (endptr != end)
						goto PARSING_ERROR;
					if (!IsInteger)
					{
						if (!lpOperandTop->IsQuad)
							operand.Value.Quad = (QWORD)operand.Value.Float;
						else
							operand.Value.Quad = (QWORD)operand.Value.Double;
					}
					if (operand.Value.High)
						goto PARSING_ERROR;
					{
						THeapListData *HeapL;

						HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, operand.Value.Low - 1);
						if (HeapL)
							operand.Value.Quad = HeapL->heapListAddress;
					}
					if (IsInteger)
						operand.IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(DWORD);
					else if (!lpOperandTop->IsQuad)
						operand.Value.Float = (float)(size_t)operand.Value.Quad;
					else
						operand.Value.Double = (size_t)operand.Value.Quad;
					OPERAND_PUSH(operand);
					i++;
					break;
				default:
					if (bCompoundAssign)
						goto ASSIGN;
					if (lpNext && lpNext->Tag != TAG_LEFT_ASSIGN && (lpNext->Type & OS_LEFT_ASSIGN))
						bCompoundAssign = TRUE;
					if (element)
					{
						operand.Value = element->Value;
						operand.IsQuad = element->IsQuad;
						OPERAND_PUSH(operand);
					}
					else if (endptr == end)
					{
						operand.IsQuad = IsInteger ? operand.Value.High != 0 : IsQuad;
						OPERAND_PUSH(operand);
					}
					else
					{
						OPERAND_PUSH(OperandZero);
					}
					break;
				}
			}
			break;
		default:
			if (lpMarkup->Type & (OS_TERNARY_END | OS_WHILE_END))
				continue;
		PARSING_ERROR:
			if (TSSGCtrl_GetSSGActionListner(SSGCtrl))
			{
				lpMarkup->String[lpMarkup->Length] = '\0';
				TSSGActionListner_OnParsingError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpMarkup->String);
			}
			goto FAILED10;
		}
		if (TSSGCtrl_GetSSGActionListner(SSGCtrl))
		{
#if defined(__BORLANDC__)
			char c;

			c = lpMarkup->String[lpMarkup->Length];
			lpMarkup->String[lpMarkup->Length] = '\0';
			lpGuideText = lpMarkup->String;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpGuideText, lpOperandTop->Value.Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpGuideText, !lpOperandTop->IsQuad ? lpOperandTop->Value.Float : lpOperandTop->Value.Double);
			if (lpGuideText == lpMarkup->String)
				lpMarkup->String[lpMarkup->Length] = c;
#else
			lpGuideText = lpMarkup->String;
			nGuideTextLength = lpMarkup->Length;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(lpGuideText, nGuideTextLength, lpOperandTop->Value.Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(lpGuideText, nGuideTextLength, !lpOperandTop->IsQuad ? lpOperandTop->Value.Float : lpOperandTop->Value.Double);
#endif
		}
	}
	qwResult = lpOperandTop->Value.Quad;
FAILED10:
	if (hProcess)
		CloseHandle(hProcess);
FAILED9:
#if defined(REPEAT_INDEX) && REPEAT_INDEX
	if (lpVariableStringBuffer)
		HeapFree(hHeap, 0, lpVariableStringBuffer);
FAILED8:
#endif
	HeapFree(hHeap, 0, lpVariable);
FAILED7:
	HeapFree(hHeap, 0, lpOperandBuffer);
FAILED6:
	HeapFree(hHeap, 0, lpnNestBuffer);
FAILED5:
	HeapFree(hHeap, 0, lpFactorBuffer);
FAILED4:
	HeapFree(hHeap, 0, lpPostfix);
FAILED3:
	HeapFree(hHeap, 0, lpMarkupArray);
	HeapFree(hHeap, 0, lpMarkupStringBuffer);
FAILED2:
#if defined(ADDITIONAL_TAGS) && ADDITIONAL_TAGS
	HeapFree(hHeap, 0, lpszSrc);
#endif
#if (defined(ADDITIONAL_TAGS) && ADDITIONAL_TAGS)
FAILED1:
#endif
	return qwResult;

	#undef OPERAND_CLEAR
	#undef OPERAND_IS_EMPTY
	#undef OPERAND_PUSH
	#undef OPERAND_POP
	#undef PROCESS_DESIRED_ACCESS
}
//---------------------------------------------------------------------
#if defined(__BORLANDC__)
unsigned long TSSGCtrl::Parsing(IN TSSGSubject *SSGS, IN const string &_Src, ...)
{
	#define Src (&_Src)
#else
unsigned long __cdecl Parsing(IN TSSGCtrl *_this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...)
{
#endif
	QWORD   Result;
	va_list ArgPtr;

#if defined(__BORLANDC__)
	va_start(ArgPtr, SSGS);
	va_arg(ArgPtr, LPCVOID);
#else
	va_start(ArgPtr, Src);
#endif
	Result = _Parsing(_this, SSGS, Src, TRUE, FALSE, ArgPtr);
	va_end(ArgPtr);

	return (unsigned long)Result;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数（double版）」
//	・四則演算や比較演算のみ(剰余演算子[%]は不可)
//---------------------------------------------------------------------
#if defined(__BORLANDC__)
double TSSGCtrl::ParsingDouble(IN TSSGSubject *SSGS, IN const string &_Src, IN double Val)
{
	#define Src (&_Src)
#else
double __cdecl ParsingDouble(IN TSSGCtrl *_this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, IN double Val)
{
#endif
	union {
		QWORD  Quad;
		double Double;
	} Result;
	struct {
		struct {
			size_t Length;
			LPCSTR String;
			double Value;
		} Data;
		size_t Terminator;
	} Param;

	Param.Data.Length = 3;
	Param.Data.String = "Val";
	Param.Data.Value = Val;
	Param.Terminator = 0;
	Result.Quad = _Parsing(_this, SSGS, Src, FALSE, TRUE, (va_list)&Param);

	return Result.Double;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------

#if defined(__BORLANDC__)
#undef _this
#undef fmodf
#undef _ultoa
#undef bcb6__isnan
#undef bcb6_std_string
#undef bcb6_std_string_c_str
#undef bcb6_std_string_length
#undef TSSGCtrl_GetAttribute
#undef TSSGCtrl_GetSSGActionListner
#undef TSSGCtrl_AddressAttributeFilter
#undef TSSGSubject_GetAttribute
#undef TSSGActionListner_OnParsingError
#undef TSSGActionListner_OnParsingProcess
#undef TSSGActionListner_OnParsingDoubleProcess
#undef TSSGAttributeElement_GetType
#undef TEndWithAttribute_GetCode
#undef TIO_FEPAttribute_GetInputCode
#undef TIO_FEPAttribute_GetOutputCode
#undef TProcessCtrl_Open
#undef TProcessCtrl_GetModuleFromName
#undef TProcessCtrl_GetHeapList
#endif

#undef AT_ADJUST
#undef AT_REPLACE
#undef AT_VARIABLE
#undef AT_DEFINE

#undef OS_SPLIT
#undef OS_PUSH
#undef OS_OPEN
#undef OS_CLOSE
#undef OS_MONADIC
#undef OS_POST
#undef OS_SHORT_CIRCUIT
#undef OS_LEFT_ASSIGN
#undef OS_RIGHT_ASSIGN
#undef OS_TERNARY
#undef OS_TERNARY_END
#undef OS_WHILE_BEGIN
#undef OS_WHILE_END
#undef OS_MEMMOVE_BEGIN
#undef OS_MEMMOVE_END

