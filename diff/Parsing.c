#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <math.h>
#include <float.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "MoveProcessMemory.h"
#include "FillProcessMemory.h"

#define IMPLEMENTED 0

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
#ifndef PROCESS_QUERY_LIMITED_INFORMATION
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif
#define _ultoa                                                                         ultoa
#define fmodf                                                                          fmod
#define string                                                                         string
#define string_c_str(s)                                                                (s)->c_str()
#define string_begin(s)                                                                (s)->begin()
#define string_end(s)                                                                  (s)->end()
#define string_length(s)                                                               (s)->length()
#define vector_TSSGAttributeElement                                                    vector<TSSGAttributeElement *>
#define TMainForm_GetUserMode(MainForm)                                                (MainForm)->GetUserMode()
#define TSSGCtrl_GetAttribute(SSGCtrl, SSGS, Type)                                     (SSGCtrl)->GetAttribute(SSGS, Type)
#define TSSGCtrl_GetSSGActionListner(SSGCtrl)                                          (SSGCtrl)->GetSSGActionListner()
#define TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, Address, Mode)                  (SSGCtrl)->AddressAttributeFilter(SSGS, Address, Mode)
#define TSSGSubject_GetAttribute(SSGS)                                                 (SSGS)->GetAttribute()
#define TSSGSubject_GetSize(SSGS)                                                      (SSGS)->GetSize()
#define TSSGActionListner_OnSubjectReadError(SSGActionListner, SSGS, Address)          (SSGActionListner)->OnSubjectReadError(SSGS, Address)
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
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGActionListner.h"
#include "THeapListData.h"
#include "TProcessCtrl.h"
#include "TEndWithAttribute.h"
#include "TIO_FEPAttribute.h"
#include "TMainForm.h"
#define vector_TSSGAttributeElement vector
EXTERN_C size_t __stdcall ReplaceDefineByHeap(vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity);
#endif

int __cdecl GuidePrint(const char *format, ...);
int __fastcall GuidePrintV(const char *format, va_list argptr);
EXTERN_C int __cdecl DebugPrint(const char *format, ...);
EXTERN_C int __fastcall DebugPrintV(const char *format, va_list argptr);
EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName);
EXTERN_C FARPROC * __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName);
EXTERN_C LPVOID __stdcall GetSectionAddress(HANDLE hProcess, HMODULE hModule, LPCSTR lpSectionName, LPDWORD lpdwSectionSize);
EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString);
EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString);

#if defined(_MSC_VER)
#if REPEAT_INDEX
#include "SubjectProperty\SSGSubjectProperty.h"
#endif
#pragma function(strlen)
extern HANDLE hHeap;
#endif

#define AT_ADJUST   0x0002
#define AT_REPLACE  0x0004
#define AT_VARIABLE 0x0800
#define AT_DEFINE   0x1000

#define OS_PUSH          0x0001
#define OS_OPEN          0x0002
#define OS_CLOSE         0x0004
#define OS_SPLIT         0x0008
#define OS_MONADIC       0x0010
#define OS_POST          0x0020
#define OS_SHORT_CIRCUIT 0x0040
#define OS_LEFT_ASSIGN   0x0080
#define OS_PARENTHESIS   0x0100
#define OS_HAS_EXPR      0x0200
#define OS_TERNARY       0x0400
#define OS_TERNARY_END   0x0800
#define OS_LOOP_BEGIN    0x1000
#define OS_LOOP_END      0x2000
#define OS_RET_OPERAND   0x4000

/*
 [Wikipedia] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/%E6%BC%94%E7%AE%97%E5%AD%90%E3%81%AE%E5%84%AA%E5%85%88%E9%A0%86%E4%BD%8D#.E3.83.97.E3.83.AD.E3.82.B0.E3.83.A9.E3.83.9F.E3.83.B3.E3.82.B0.E8.A8.80.E8.AA.9E

 [Microsoft Developer Network] - [優先順位と評価順序] - [C 演算子の優先順位と結合規則]
 https://msdn.microsoft.com/ja-jp/library/2bxt6kc4.aspx

 [Wikipedia] - [CとC++の演算子] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/C%E3%81%A8C%2B%2B%E3%81%AE%E6%BC%94%E7%AE%97%E5%AD%90#.E6.BC.94.E7.AE.97.E5.AD.90.E3.81.AE.E5.84.AA.E5.85.88.E9.A0.86.E4.BD.8D

 127 if                                 OS_PUSH | OS_HAS_EXPR
 127 else                               OS_PUSH
 127 do                                 OS_PUSH | OS_LOOP_BEGIN
 127 while                              OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN
 127 for                                OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN
 127 break                              OS_PUSH
 127 continue                           OS_PUSH
 127 printf                             OS_PUSH
 127 dprintf                            OS_PUSH
 127 memmove                            OS_PUSH
 127 memset                             OS_PUSH
 127 memset16                           OS_PUSH
 127 memset32                           OS_PUSH
 127 memset64                           OS_PUSH
#if IMPLEMENTED
 127 call                               OS_PUSH
 127 co_await                           OS_PUSH
 127 co_yield                           OS_PUSH
 127 co_return                          OS_PUSH
 127 switch                             OS_PUSH | OS_HAS_EXPR
 127 case                               OS_PUSH
 127 default                            OS_PUSH
 127 goto                               OS_PUSH
#endif
 100 (                                  OS_OPEN | OS_PARENTHESIS
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
  31 && and                             OS_PUSH | OS_SHORT_CIRCUIT     論理積
  30 || or                              OS_PUSH | OS_SHORT_CIRCUIT     論理和
  29 ? :                                OS_PUSH                        条件演算子
  25 =>                                 OS_PUSH                        右辺代入
  25 = += -= *= /= %= &= |= ^= <<= >>=  OS_PUSH | OS_LEFT_ASSIGN       左辺代入 加算代入 減算代入 乗算代入 除算代入 剰余代入 ビット積代入 ビット排他的論理和代入 ビット和代入 左論理シフト代入 右論理シフト代入
  20 :] :8] :7] :6] :5] :4] :3] :2] :1] OS_PUSH | OS_CLOSE
  20 :L] :L8] :L7] :L6] :L5]
     :L4] :L3] :L2] :L1]                OS_PUSH | OS_CLOSE
  18 ~] ~8] ~7] ~6] ~5] ~4] ~3] ~2]     OS_PUSH | OS_CLOSE
  15 .]                                 OS_PUSH | OS_CLOSE
  10 _]                                 OS_PUSH | OS_CLOSE
   0 )                                  OS_CLOSE | OS_PARENTHESIS
   0 ;                                  OS_SPLIT
   0 return                             OS_PUSH
*/

typedef enum {
	TAG_NOT_OPERATOR     ,  // 127                  OS_PUSH
	TAG_IF               ,  // 127 if               OS_PUSH | OS_HAS_EXPR
	TAG_ELSE             ,  // 127 else             OS_PUSH
	TAG_DO               ,  // 127 do               OS_PUSH | OS_LOOP_BEGIN
	TAG_WHILE            ,  // 127 while            OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN
	TAG_FOR              ,  // 127 for              OS_PUSH | OS_HAS_EXPR
	TAG_BREAK            ,  // 127 break            OS_PUSH
	TAG_CONTINUE         ,  // 127 continue         OS_PUSH
	TAG_PRINTF           ,  // 127 printf           OS_PUSH
	TAG_DPRINTF          ,  // 127 dprintf          OS_PUSH
	TAG_MEMMOVE          ,  // 127 memmove          OS_PUSH
	TAG_MEMSET           ,  // 127 memset           OS_PUSH
	TAG_MEMSET16         ,  // 127 memset16         OS_PUSH
	TAG_MEMSET32         ,  // 127 memset32         OS_PUSH
	TAG_MEMSET64         ,  // 127 memset64         OS_PUSH
	TAG_PARAM_LOCAL      ,  // 127 L                OS_PUSH
#if IMPLEMENTED
	TAG_CALL             ,  // 127 call             OS_PUSH
	TAG_CO_AWAIT         ,  // 127 co_await         OS_PUSH
	TAG_CO_YIELD         ,  // 127 co_yield         OS_PUSH
	TAG_CO_RETURN        ,  // 127 co_return        OS_PUSH
	TAG_SWITCH           ,  // 127 switch           OS_PUSH | OS_HAS_EXPR
	TAG_CASE             ,  // 127 case             OS_PUSH
	TAG_DEFAULT          ,  // 127 default          OS_PUSH
	TAG_GOTO             ,  // 127 goto             OS_PUSH
	TAG_LABEL            ,  // 127                  OS_PUSH
#endif
	TAG_PARENTHESIS_OPEN ,  // 100 (                OS_OPEN | OS_PARENTHESIS
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
	TAG_AND              ,  //  31 &&      (31 and) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_OR               ,  //  30 ||      (30 or ) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_TERNARY          ,  //  29 ?                OS_PUSH | OS_TERNARY
	TAG_TERNARY_SPLIT    ,  //  29 :                OS_PUSH | OS_TERNARY
	TAG_RIGHT_ASSIGN     ,  //  25 =>               OS_PUSH
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
	TAG_DELIMITER        ,  //   0 ,                OS_PUSH | OS_SPLIT
	TAG_IF_EXPR          ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_WHILE_EXPR       ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_FOR_INITIALIZE   ,  //   0 ;                OS_PUSH | OS_SPLIT
	TAG_FOR_CONDITION    ,  //   0 ;                OS_PUSH | OS_SPLIT | OS_LOOP_BEGIN
	TAG_FOR_UPDATE       ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_PRINTF_END       ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_DPRINTF_END      ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_MEMMOVE_END      ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET_END       ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET16_END     ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET32_END     ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET64_END     ,  //   0 )                OS_PUSH | OS_CLOSE
	TAG_PARENTHESIS_CLOSE,  //   0 )                OS_CLOSE | OS_PARENTHESIS
	TAG_SPLIT            ,  //   0 ;                OS_SPLIT
	TAG_RETURN           ,  //   0 return           OS_PUSH
	TAG_PARSE_ERROR      ,
#if IMPLEMENTED
//	TAG_PARSE_ERROR_PARENTHESES_NOT_FOUND     , // 構文に必要な括弧がありません
//	TAG_PARSE_ERROR_PARENTHESES_INVALID_PAIRS , // 括弧の対応が正しくありません
//	TAG_PARSE_ERROR_ARGUMENTS_NOT_ENOUGH      , // 引数が見つかりません
//	TAG_PARSE_ERROR_STATEMENT_NOT_FOUND       , // 式に続く文が見つかりません
//	TAG_PARSE_ERROR_END_OF_STATEMENT_NOT_FOUND, // 文の終端が見つかりません
//	TAG_PARSE_ERROR_WHILE_NOT_FOUND           , // do に対応する while がありません
#endif
} TAG;

typedef enum {
	PRIORITY_NOT_OPERATOR      = 127,   //                  OS_PUSH
	PRIORITY_IF                = 127,   // if               OS_PUSH | OS_HAS_EXPR
	PRIORITY_ELSE              = 127,   // else             OS_PUSH
	PRIORITY_DO                = 127,   // do               OS_PUSH
	PRIORITY_WHILE             = 127,   // while            OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN
	PRIORITY_FOR               = 127,   // for              OS_PUSH | OS_HAS_EXPR
	PRIORITY_BREAK             = 127,   // break            OS_PUSH
	PRIORITY_CONTINUE          = 127,   // continue         OS_PUSH
	PRIORITY_PRINTF            = 127,   // printf           OS_PUSH
	PRIORITY_DPRINTF           = 127,   // dprintf          OS_PUSH
	PRIORITY_MEMMOVE           = 127,   // memmove          OS_PUSH
	PRIORITY_MEMSET            = 127,   // memset           OS_PUSH
	PRIORITY_MEMSET16          = 127,   // memset16         OS_PUSH
	PRIORITY_MEMSET32          = 127,   // memset32         OS_PUSH
	PRIORITY_MEMSET64          = 127,   // memset64         OS_PUSH
	PRIORITY_PARAM_LOCAL       = 127,   // L                OS_PUSH
#if IMPLEMENTED
	PRIORITY_CALL              = 127,   // call             OS_PUSH
	PRIORITY_CO_AWAIT          = 127,   // co_await         OS_PUSH
	PRIORITY_CO_YIELD          = 127,   // co_yield         OS_PUSH
	PRIORITY_CO_RETURN         = 127,   // co_return        OS_PUSH
	PRIORITY_SWITCH            = 127,   // switch           OS_PUSH | OS_HAS_EXPR
	PRIORITY_CASE              = 127,   // case             OS_PUSH
	PRIORITY_DEFAULT           = 127,   // default          OS_PUSH
	PRIORITY_GOTO              = 127,   // goto             OS_PUSH
	PRIORITY_LABEL             = 127,   //                  OS_PUSH
#endif
	PRIORITY_PARENTHESIS_OPEN  = 100,   // (                OS_OPEN | OS_PARENTHESIS
	PRIORITY_POST_INC          = 100,   // N++              OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_POST_DEC          = 100,   // N--              OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_ADDR_ADJUST_OPEN  =  90,   // [_               OS_OPEN
	PRIORITY_ADDR_REPLACE_OPEN =  85,   // [.               OS_OPEN
	PRIORITY_REV_ENDIAN_OPEN   =  83,   // [~               OS_OPEN
	PRIORITY_REMOTE_OPEN       =  80,   // [:               OS_OPEN
	PRIORITY_FUNCTION          =  75,   // MName::          OS_PUSH
	                                    // ::               OS_PUSH
	                                    // :!               OS_PUSH
	                                    // :&               OS_PUSH
	                                    // := :+            OS_PUSH
	                                    // HNumber::        OS_PUSH
	                                    // Cast32::         OS_PUSH
	                                    // Cast64::         OS_PUSH
	                                    // I1toI4::         OS_PUSH
	                                    // I2toI4::         OS_PUSH
	                                    // I4toI8::         OS_PUSH
	                                    // Memory::         OS_PUSH
	                                    // strlen::         OS_PUSH
	                                    // wcslen::         OS_PUSH
	                                    // BitScanForward:: OS_PUSH
	                                    // BitScanReverse:: OS_PUSH
	PRIORITY_NEG               =  52,   // -                OS_PUSH | OS_MONADIC
	PRIORITY_NOT               =  52,   // !                OS_PUSH | OS_MONADIC
	PRIORITY_BIT_NOT           =  52,   // ~                OS_PUSH | OS_MONADIC
	PRIORITY_INDIRECTION       =  52,   // *                OS_PUSH | OS_MONADIC
	PRIORITY_PRE_INC           =  52,   // ++N              OS_PUSH | OS_MONADIC
	PRIORITY_PRE_DEC           =  52,   // --N              OS_PUSH | OS_MONADIC
	PRIORITY_MUL               =  51,   // *       (25 *= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_DIV               =  51,   // /       (25 /= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_MOD               =  51,   // %       (25 %= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_IDIV              =  51,   // idiv             OS_PUSH
	PRIORITY_IMOD              =  51,   // imod             OS_PUSH
	PRIORITY_ADD               =  50,   // +       (25 += ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SUB               =  50,   // -       (25 -= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHL               =  49,   // <<      (25 <<=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHR               =  49,   // >>      (25 >>=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SAR               =  49,   // sar              OS_PUSH
	PRIORITY_ROL               =  49,   // rol              OS_PUSH
	PRIORITY_ROR               =  49,   // ror              OS_PUSH
	PRIORITY_BT                =  40,   // <                OS_PUSH
	PRIORITY_AT                =  40,   // >                OS_PUSH
	PRIORITY_BE                =  40,   // <=               OS_PUSH
	PRIORITY_AE                =  40,   // >=               OS_PUSH
	PRIORITY_LT                =  40,   // lt               OS_PUSH
	PRIORITY_GT                =  40,   // gt               OS_PUSH
	PRIORITY_LE                =  40,   // le               OS_PUSH
	PRIORITY_GE                =  40,   // ge               OS_PUSH
	PRIORITY_EQ                =  35,   // ==               OS_PUSH
	PRIORITY_NE                =  35,   // !=               OS_PUSH
	PRIORITY_BIT_AND           =  34,   // &       (25 &= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_XOR               =  33,   // ^       (25 ^= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_BIT_OR            =  32,   // |       (25 |= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_AND               =  31,   // &&      (31 and) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_OR                =  30,   // ||      (30 or ) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_TERNARY           =  29,   // ? :              OS_PUSH | OS_TERNARY
	PRIORITY_RIGHT_ASSIGN      =  25,   // =>               OS_PUSH
	PRIORITY_LEFT_ASSIGN       =  25,   // =                OS_PUSH | OS_LEFT_ASSIGN
	PRIORITY_REMOTE            =  20,   // :]  :1] :2] :3]  OS_PUSH | OS_CLOSE
	                                    // :4] :5] :6] :7]
	                                    // :8]
	                                    // :L]  :L1] :L2]   OS_PUSH | OS_CLOSE
	                                    // :L3] :L4] :L5]
	                                    // :L6] :L7] :L8]
	PRIORITY_REV_ENDIAN        =  18,   // ~]  ~2] ~3] ~4]  OS_PUSH | OS_CLOSE
	                                    // ~5] ~6] ~7] ~8]
	PRIORITY_ADDR_REPLACE      =  15,   // .]               OS_PUSH | OS_CLOSE
	PRIORITY_ADDR_ADJUST       =  10,   // _]               OS_PUSH | OS_CLOSE
	PRIORITY_DELIMITER         =   0,   // ,                OS_PUSH | OS_SPLIT
	PRIORITY_PARENTHESIS_CLOSE =   0,   // )                OS_CLOSE | OS_PARENTHESIS
	PRIORITY_SPLIT             =   0,   // ;                OS_SPLIT
	PRIORITY_RETURN            =   0,   // return           OS_PUSH
} PRIORITY;

typedef struct {
	TAG    Tag;
	size_t Length;
	LPSTR  String;
	BYTE   Priority;
	WORD   Type;
	size_t Depth;
	size_t LoopDepth;
} MARKUP, *PMARKUP;

typedef struct {
	union {
		struct {
			DWORD Low;
			DWORD High;
		};
		ULONGLONG Quad;
		float     Float;
		double    Double;
	};
	BOOL IsQuad;
} VARIABLE, *PVARIABLE;

typedef VARIABLE OPERAND;
typedef OPERAND *POPERAND;

typedef struct {
	size_t   Length;
	LPCSTR   String;
	VARIABLE Value;
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
static MARKUP * __fastcall ReAllocMarkup(MARKUP **lppMarkupArray, size_t *lpnNumberOfMarkup)
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
static void __fastcall TrimMarkupString(MARKUP *lpMarkup)
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
static MARKUP * __fastcall FindParenthesisClose(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	size_t nDepth;

	nDepth = 1;
	while (++lpMarkup < lpEndOfMarkup)
	{
		if (!(lpMarkup->Type & OS_PARENTHESIS))
			continue;
		if (lpMarkup->Type & OS_OPEN)
			nDepth++;
		else if (!--nDepth)
			break;
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindSplit(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	size_t nDepth;

	nDepth = 0;
	do
	{
		if (lpMarkup->Type & OS_SPLIT)
		{
			if (!nDepth)
				break;
		}
		else if (lpMarkup->Type & OS_PARENTHESIS)
		{
			if (lpMarkup->Type & OS_OPEN)
				nDepth++;
			else if (nDepth)
				nDepth--;
		}
	} while (++lpMarkup < lpEndOfMarkup);
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindEndOfStructuredStatement(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup->Type & OS_HAS_EXPR)
	{
		size_t nNest;

		nNest = 0;
		for (; ; )
		{
			if (lpMarkup->Tag == TAG_IF)
				nNest++;
			if (++lpMarkup >= lpEndOfMarkup)
				break;
			if (lpMarkup->Tag != TAG_PARENTHESIS_OPEN)
				break;
			lpMarkup = FindParenthesisClose(lpMarkup, lpEndOfMarkup);
			if (++lpMarkup >= lpEndOfMarkup)
				break;
			if (lpMarkup->Type & OS_HAS_EXPR)
				continue;
			while (nNest--)
			{
				if (lpMarkup->Tag == TAG_PARENTHESIS_OPEN)
					lpMarkup = FindParenthesisClose(lpMarkup, lpEndOfMarkup);
				else
					lpMarkup = FindSplit(lpMarkup, lpEndOfMarkup);
				if (lpMarkup + 1 >= lpEndOfMarkup || (lpMarkup + 1)->Tag != TAG_ELSE)
					break;
				if ((lpMarkup += 2) >= lpEndOfMarkup)
					break;
				lpMarkup = FindEndOfStructuredStatement(lpMarkup, lpEndOfMarkup);
			}
			return (MARKUP *)lpMarkup;
		}
		return (MARKUP *)lpEndOfMarkup;
	}
	else if (lpMarkup->Tag == TAG_PARENTHESIS_OPEN)
	{
		return FindParenthesisClose(lpMarkup, lpEndOfMarkup);
	}
	else
	{
		return FindSplit(lpMarkup, lpEndOfMarkup);
	}
}
//---------------------------------------------------------------------
static MARKUP * __stdcall Markup(IN LPCSTR lpSrc, IN size_t nSrcLength, OUT LPSTR *lppMarkupStringBuffer, OUT size_t *lpnNumberOfMarkup)
{
	size_t  nStringLength;
	LPSTR   lpMarkupStringBuffer;
	LPSTR   lpDest;
	char    c;
	MARKUP  *lpTagArray, *lpEndOfTag;
	size_t  nNumberOfTag;
	BOOLEAN bIsLaedByte, bPrevIsTailByte;
	MARKUP  *lpMarkupArray;
	MARKUP  *lpMarkup, *lpEndOfMarkup;
	size_t  nMarkupIndex;
	size_t  nFirstIf;
	size_t  nFirstTernary;
	size_t  nFirstDo;
	size_t  nFirstWhile;
	size_t  nFirstFor;
	size_t  nFirstPrintf;
	size_t  nFirstDPrintf;
	size_t  nFirstMemmove;
	size_t  nFirstMemset;
	size_t  nFirstMemset16;
	size_t  nFirstMemset32;
	size_t  nFirstMemset64;
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
				{
					*lpDest = '\0';
				}
				else if (c == '"')
				{
					// skip string block
					while ((*(++lpDest) = c = *(lpSrc++)) && c != '"')
					{
						if (!__intrinsic_isleadbyte(c))
						{
							if (c != '\\')
								continue;
							// skip next character of escape
							if (!(*(++lpDest) = c = *(lpSrc++)))
								break;
							if (!__intrinsic_isleadbyte(c))
								continue;
						}
						// skip tail byte
						if (!(*(++lpDest) = c = *(lpSrc++)))
							break;
					}
					if (!c)
						break;
				}
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
	nFirstIf       = SIZE_MAX;
	nFirstTernary  = SIZE_MAX;
	nFirstDo       = SIZE_MAX;
	nFirstWhile    = SIZE_MAX;
	nFirstFor      = SIZE_MAX;
	nFirstPrintf   = SIZE_MAX;
	nFirstDPrintf  = SIZE_MAX;
	nFirstMemmove  = SIZE_MAX;
	nFirstMemset   = SIZE_MAX;
	nFirstMemset16 = SIZE_MAX;
	nFirstMemset32 = SIZE_MAX;
	nFirstMemset64 = SIZE_MAX;
	nNumberOfTag = 0;
	bPrevIsTailByte = FALSE;
	for (LPBYTE p = lpMarkupStringBuffer, end = lpMarkupStringBuffer + nStringLength; p < end; bPrevIsTailByte = bIsLaedByte)
	{
		TAG    iTag;
		size_t nLength;
		BYTE   bPriority;

		#define APPEND_TAG(tag, length, priority, type)                  \
		do                                                               \
		{                                                                \
		    if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag))) \
		        goto FAILED2;                                            \
		    lpMarkup->Tag       = tag;                                   \
		    lpMarkup->Length    = length;                                \
		    lpMarkup->String    = p;                                     \
		    lpMarkup->Priority  = priority;                              \
		    lpMarkup->Type      = type;                                  \
		    lpMarkup->Depth     = 0;                                     \
		} while (0)

		#define APPEND_TAG_WITH_CONTINUE(tag, length, priority, type)    \
		do                                                               \
		{                                                                \
		    APPEND_TAG(tag, length, priority, type);                     \
		    p += length;                                                 \
		    goto CONTINUE;                                               \
		} while (0)

		#define is_separated_left(p) ( \
			__intrinsic_isascii(*(p)) && \
			!__intrinsic_isalnum(*(p)) && \
			(*(p) != '_' || ((p) != lpMarkupStringBuffer && *(p - 1) == '[')) && \
			*(p) != '$')

		#define is_separated_right(p) ( \
			__intrinsic_isascii(*(p)) && \
			!__intrinsic_isalnum(*(p)) && \
			(*(p) != '_' || (p < end && *(p + 1) == ']')))

		bIsLaedByte = FALSE;
		switch (*p)
		{
		case '\0':
			// ";"
			APPEND_TAG_WITH_CONTINUE(TAG_SPLIT, 1, PRIORITY_SPLIT, OS_SPLIT);
		case '!':
			// "!", "!="
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
		case '"':
			// double-quoted string
			while (++p < end && *p != '"')
			{
				if (!__intrinsic_isleadbyte(*p))
				{
					if (*p != '\\')
						continue;
					p++;
					if (!__intrinsic_isleadbyte(*p))
						continue;
				}
				p++;
			}
			break;
		case '%':
			// "%", "%="
			if (*(p + 1) != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 1, PRIORITY_MOD, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '&':
			// "&", "&&", "&="
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
		case '(':
			// "("
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_OPEN, 1, PRIORITY_PARENTHESIS_OPEN, OS_OPEN | OS_PARENTHESIS);
		case ')':
			// ")"
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_CLOSE, 1, PRIORITY_PARENTHESIS_CLOSE, OS_CLOSE | OS_PARENTHESIS);
		case '*':
			// "*", "*="
			if (*(p + 1) != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 1, PRIORITY_MUL, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '+':
			// "+", "++", "+="
			switch (*(p + 1))
			{
			case '+':
				APPEND_TAG_WITH_CONTINUE(TAG_INC, 2, PRIORITY_PRE_INC, OS_PUSH | OS_MONADIC);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_ADD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_ADD, 1, PRIORITY_ADD, OS_PUSH);
			}
		case ',':
			// ","
			APPEND_TAG_WITH_CONTINUE(TAG_DELIMITER, 1, PRIORITY_DELIMITER, OS_PUSH | OS_SPLIT);
		case '-':
			// "-", "--", "-="
			switch (*(p + 1))
			{
			case '-':
				APPEND_TAG_WITH_CONTINUE(TAG_DEC, 2, PRIORITY_PRE_DEC, OS_PUSH | OS_MONADIC);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_SUB, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_SUB, 1, PRIORITY_SUB, OS_PUSH);
			}
		case '.':
			// ".]"
			if (*(p + 1) != ']')
				break;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE, 2, PRIORITY_ADDR_REPLACE, OS_PUSH | OS_CLOSE);
		case '/':
			// "/", "/="
			if (*(p + 1) != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 1, PRIORITY_DIV, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case ':':
			// ":", "::", ":!", ":&", ":=", ":+"
			// ":]", ":]", ":1]", ":2]", ":3]", ":4]", ":5]", ":6]", ":7]", ":8]"
			// ":L]", ":L1]", ":L2]", ":L3]", ":L4]", ":L5]", ":L6]", ":L7]", ":L8]"
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
		case '<':
			// "<", "<<", "<<=", "<="
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
		case '=':
			// "=", "==", "=>"
			switch (*(p + 1))
			{
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_EQ, 2, PRIORITY_EQ, OS_PUSH);
			case '>':
				APPEND_TAG_WITH_CONTINUE(TAG_RIGHT_ASSIGN, 2, PRIORITY_RIGHT_ASSIGN, OS_PUSH);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_LEFT_ASSIGN, 1, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			}
		case '>':
			// ">", ">>", ">>=", ">="
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
		case '?':
			// "?"
			if (nFirstTernary == SIZE_MAX)
				nFirstTernary = nNumberOfTag;
			APPEND_TAG_WITH_CONTINUE(TAG_TERNARY, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARY);
		case 'B':
			// "BitScanForward::", "BitScanReverse::"
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
				p += 8;
				continue;
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
				p += 8;
				continue;
			}
			p += 4;
			continue;
		case 'C':
			// "Cast32::", "Cast64::"
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
			p += 4;
			continue;
		case 'H':
			// "HNumber::"
			if (*(LPDWORD)(p + 1) != BSWAP32('Numb'))
				break;
			if (*(LPDWORD)(p + 5) == BSWAP32('er::'))
			{
				iTag = TAG_HNUMBER;
				nLength = 9;
				goto APPEND_FUNCTIONAL_OPERATOR;
			}
			p += 5;
			continue;
		case 'I':
			// "I1toI4::", "I2toI4::", "I4toI8::"
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
		case 'L':
			// "L"
			if (nNumberOfTag > 1 && !bPrevIsTailByte &&
				(__intrinsic_isspace(*(p - 1)) || *(p - 1) == ',' || *(p - 1) == '(') &&
				(__intrinsic_isspace(*(p + 1)) || *(p + 1) == '(' || *(p + 1) == '[' || *(p + 1) == '{'))
			{
				MARKUP *lpPrev;

				lpPrev = lpTagArray + nNumberOfTag - 1;
				if (lpPrev->Tag != TAG_PARENTHESIS_OPEN)
				{
					size_t nDepth;

					if (lpPrev->Tag != TAG_DELIMITER)
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
				switch ((--lpPrev)->Tag)
				{
				case TAG_MEMMOVE:
				case TAG_MEMSET:
				case TAG_MEMSET16:
				case TAG_MEMSET32:
				case TAG_MEMSET64:
					APPEND_TAG_WITH_CONTINUE(TAG_PARAM_LOCAL, 1, PRIORITY_PARAM_LOCAL, OS_PUSH);
				}
			}
			break;
		case 'M':
			// "MName::", "Memory::"
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
		case '[':
			// "[_", "[.", "[~", "[:"
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
		case '^':
			// "^", "^="
			if (*(p + 1) != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 1, PRIORITY_XOR, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '_':
			// "_]"
			if (*(p + 1) != ']')
				break;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST, 2, PRIORITY_ADDR_ADJUST, OS_PUSH | OS_CLOSE);
		case 'a':
			// "and"
			if (*(LPWORD)(p + 1) != BSWAP16('nd'))
				break;
			iTag = TAG_AND;
			nLength = 3;
			bPriority = PRIORITY_AND;
			goto APPEND_RET_OPERAND_OPERATOR;
		case 'b':
			// "break"
			if (*(LPDWORD)(p + 1) != BSWAP32('reak'))
				break;
			if ((p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				is_separated_left(p - 1))) &&
				(__intrinsic_isspace(*(p + 5)) || *(p + 5) == '\0'))
			{
				APPEND_TAG_WITH_CONTINUE(TAG_BREAK, 5, PRIORITY_BREAK, OS_PUSH);
			}
			p += 5;
			continue;
		case 'c':
			// "continue"
			// not implemented: "call", "case", "co_await", "co_return", "co_yield"
			switch (*(p + 1))
			{
#if IMPLEMENTED
			case 'a':
				switch (*(p + 2))
				{
				case 'l':
					if (*(p + 3) == 'l')
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 4)) || *(p + 4) == '('))
						{
							APPEND_TAG_WITH_CONTINUE(TAG_CALL, 4, PRIORITY_CALL, OS_PUSH);
						}
						p += 4;
						continue;
					}
					p += 3;
					continue;
				case 's':
					if (*(p + 3) == 'e')
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 4)) || *(p + 4) == '('))
						{
							APPEND_TAG_WITH_CONTINUE(TAG_CASE, 4, PRIORITY_CASE, OS_PUSH);
						}
						p += 4;
						continue;
					}
					p += 3;
					continue;
				}
				p += 2;
				continue;
#endif
			case 'o':
				switch (*(p + 2))
				{
#if IMPLEMENTED
				case '_':
					switch (*(p + 3))
					{
					case 'a':
						if (*(LPDWORD)(p + 4) == BSWAP32('wait'))
						{
							if ((p == lpMarkupStringBuffer || (
								!bPrevIsTailByte &&
								is_separated_left(p - 1))) &&
								(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '\0'))
							{
								APPEND_TAG_WITH_CONTINUE(TAG_CO_AWAIT, 8, PRIORITY_CO_AWAIT, OS_PUSH);
							}
							p += 8;
							continue;
						}
						p += 4;
						continue;
					case 'r':
						if (*(LPDWORD)(p + 4) == BSWAP32('etur'))
						{
							if (*(p + 8) == 'n')
							{
								if ((p == lpMarkupStringBuffer || (
									!bPrevIsTailByte &&
									is_separated_left(p - 1))) &&
									(__intrinsic_isspace(*(p + 9)) || *(p + 9) == '\0'))
								{
									APPEND_TAG_WITH_CONTINUE(TAG_CO_RETURN, 9, PRIORITY_CO_RETURN, OS_PUSH);
								}
								p += 9;
								continue;
							}
							p += 8;
							continue;
						}
						p += 4;
						continue;
					case 'y':
						if (*(LPDWORD)(p + 4) == BSWAP32('ield'))
						{
							if ((p == lpMarkupStringBuffer || (
								!bPrevIsTailByte &&
								is_separated_left(p - 1))) &&
								(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '\0'))
							{
								APPEND_TAG_WITH_CONTINUE(TAG_CO_YIELD, 8, PRIORITY_CO_YIELD, OS_PUSH);
							}
							p += 8;
							continue;
						}
						p += 4;
						continue;
					}
					p += 3;
					continue;
#endif
				case 'n':
					if (*(LPDWORD)(p + 3) == BSWAP32('tinu'))
					{
						if (*(p + 7) == 'e')
						{
							if ((p == lpMarkupStringBuffer || (
								!bPrevIsTailByte &&
								is_separated_left(p - 1))) &&
								(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '\0'))
							{
								APPEND_TAG_WITH_CONTINUE(TAG_CONTINUE, 8, PRIORITY_CONTINUE, OS_PUSH);
							}
							p += 8;
							continue;
						}
						p += 7;
						continue;
					}
					p += 3;
					continue;
				}
				p += 2;
				continue;
			}
			break;
		case 'd':
			// "do", "dprintf"
			// not implemented: "default"
			switch (*(p + 1))
			{
#if IMPLEMENTED
			case 'e':
				if (*(LPDWORD)(p + 2) == BSWAP32('faul'))
				{
					if (*(p + 6) == 't')
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 7)) || *(p + 7) == ':'))
						{
							APPEND_TAG_WITH_CONTINUE(TAG_DEFAULT, 7, PRIORITY_DEFAULT, OS_PUSH);
						}
						p += 7;
						continue;
					}
					p += 6;
					continue;
				}
				p += 2;
				continue;
#endif
			case 'o':
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					is_separated_left(p - 1))) &&
					(__intrinsic_isspace(*(p + 2)) || *(p + 2) == '('))
				{
					if (nFirstDo == SIZE_MAX)
						nFirstDo = nNumberOfTag;
					APPEND_TAG_WITH_CONTINUE(TAG_DO, 2, PRIORITY_DO, OS_PUSH | OS_LOOP_BEGIN);
				}
				p += 2;
				continue;
			case 'p':
				if (*(LPDWORD)(p + 2) == BSWAP32('rint'))
				{
					if (*(p + 6) == 'f')
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 7)) || *(p + 7) == '('))
						{
							if (nFirstDPrintf == SIZE_MAX)
								nFirstDPrintf = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_DPRINTF, 7, PRIORITY_DPRINTF, OS_PUSH);
						}
						p += 7;
						continue;
					}
					p += 6;
					continue;
				}
				p += 2;
				continue;
			}
			break;
		case 'e':
			// "else"
			if (*(LPDWORD)p != BSWAP32('else'))
				break;
			if ((p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				(__intrinsic_isspace(*(p - 1)) || *(p - 1) == ')' || *(p - 1) == '\0'))) &&
				(__intrinsic_isspace(*(p + 4)) || *(p + 4) == '('))
			{
				APPEND_TAG_WITH_CONTINUE(TAG_ELSE, 4, PRIORITY_ELSE, OS_PUSH);
			}
			p += 4;
			continue;
		case 'f':
			// "for"
			if (*(LPWORD)(p + 1) != BSWAP16('or'))
				break;
			if ((p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				is_separated_left(p - 1))) &&
				(__intrinsic_isspace(*(p + 3)) || *(p + 3) == '('))
			{
				if (nFirstFor == SIZE_MAX)
					nFirstFor = nNumberOfTag;
				APPEND_TAG_WITH_CONTINUE(TAG_FOR, 3, PRIORITY_FOR, OS_PUSH | OS_HAS_EXPR);
			}
			p += 3;
			continue;
		case 'g':
			// "gt", "ge"
			// not implemented: "goto"
			switch (*(p + 1))
			{
			case 't':
				iTag = TAG_GT;
				nLength = 2;
				bPriority = PRIORITY_GT;
				goto APPEND_WORD_OPERATOR;
			case 'e':
				iTag = TAG_GE;
				nLength = 2;
				bPriority = PRIORITY_GE;
				goto APPEND_WORD_OPERATOR;
#if IMPLEMENTED
			case 'o':
				if (*(LPWORD)(p + 2) != BSWAP16('to'))
					break;
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					is_separated_left(p - 1))) &&
					(__intrinsic_isspace(*(p + 4)) || *(p + 4) == '('))
				{
					APPEND_TAG_WITH_CONTINUE(TAG_GOTO, 4, PRIORITY_GOTO, OS_PUSH | OS_HAS_EXPR);
				}
				break;
#endif
			}
			break;
		case 'i':
			// "idiv", "imod", "if"
			switch (*(p + 1))
			{
			case 'd':
				if (*(LPWORD)(p + 2) != BSWAP16('iv'))
					break;
				iTag = TAG_IDIV;
				nLength = 4;
				bPriority = PRIORITY_IDIV;
				goto APPEND_WORD_OPERATOR;
			case 'm':
				if (*(LPWORD)(p + 2) != BSWAP16('od'))
					break;
				iTag = TAG_IMOD;
				nLength = 4;
				bPriority = PRIORITY_IMOD;
				goto APPEND_WORD_OPERATOR;
			case 'f':
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					is_separated_left(p - 1))) &&
					(__intrinsic_isspace(*(p + 2)) || *(p + 2) == '('))
				{
					if (nFirstIf == SIZE_MAX)
						nFirstIf = nNumberOfTag;
					APPEND_TAG_WITH_CONTINUE(TAG_IF, 2, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
				}
				break;
			}
			break;
		case 'l':
			// "lt", "le"
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
		case 'm':
			// "memmove", "memset", "memset16", "memset32", "memset64"
			if (*(LPWORD)(p + 1) != BSWAP16('em'))
				break;
			if (*(LPWORD)(p + 3) == BSWAP16('mo'))
			{
				if (*(LPWORD)(p + 5) == BSWAP16('ve'))
				{
					if ((p == lpMarkupStringBuffer || (
						!bPrevIsTailByte &&
						is_separated_left(p - 1))) &&
						(__intrinsic_isspace(*(p + 7)) || *(p + 7) == '('))
					{
						if (nFirstMemmove == SIZE_MAX)
							nFirstMemmove = nNumberOfTag;
						APPEND_TAG_WITH_CONTINUE(TAG_MEMMOVE, 7, PRIORITY_MEMMOVE, OS_PUSH);
					}
					p += 7;
					continue;
				}
				p += 5;
				continue;
			}
			else if (*(LPWORD)(p + 3) == BSWAP16('se'))
			{
				if (*(p + 5) == 't')
				{
					if (*(LPWORD)(p + 6) == BSWAP16('16'))
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '('))
						{
							if (nFirstMemset16 == SIZE_MAX)
								nFirstMemset16 = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_MEMSET16, 8, PRIORITY_MEMSET16, OS_PUSH);
						}
					}
					else if (*(LPWORD)(p + 6) == BSWAP16('32'))
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '('))
						{
							if (nFirstMemset32 == SIZE_MAX)
								nFirstMemset32 = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_MEMSET32, 8, PRIORITY_MEMSET32, OS_PUSH);
						}
					}
					else if (*(LPWORD)(p + 6) == BSWAP16('64'))
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 8)) || *(p + 8) == '('))
						{
							if (nFirstMemset64 == SIZE_MAX)
								nFirstMemset64 = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_MEMSET64, 8, PRIORITY_MEMSET64, OS_PUSH);
						}
					}
					else
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 6)) || *(p + 6) == '('))
						{
							if (nFirstMemset == SIZE_MAX)
								nFirstMemset = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_MEMSET, 6, PRIORITY_MEMSET, OS_PUSH);
						}
						p += 6;
						continue;
					}
					p += 8;
					continue;
				}
				p += 5;
				continue;
			}
			p += 3;
			continue;
		case 'o':
			// "or"
			if (*(p + 1) != 'r')
				break;
			iTag = TAG_OR;
			nLength = 2;
			bPriority = PRIORITY_OR;
		APPEND_RET_OPERAND_OPERATOR:
			if ((p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				is_separated_left(p - 1))) &&
				is_separated_right(p + nLength))
			{
				APPEND_TAG(iTag, nLength, bPriority, OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND);
				APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH | OS_RET_OPERAND);
			}
			p += nLength;
			continue;
		case 'p':
			// "printf"
			if (*(LPDWORD)(p + 1) == BSWAP32('rint'))
			{
				if (*(p + 5) == 'f')
				{
					if ((p == lpMarkupStringBuffer || (
						!bPrevIsTailByte &&
						is_separated_left(p - 1))) &&
						(__intrinsic_isspace(*(p + 6)) || *(p + 6) == '('))
					{
						if (nFirstPrintf == SIZE_MAX)
							nFirstPrintf = nNumberOfTag;
						APPEND_TAG_WITH_CONTINUE(TAG_PRINTF, 6, PRIORITY_PRINTF, OS_PUSH);
					}
					p += 6;
					continue;
				}
				p += 5;
				continue;
			}
			break;
		case 'r':
			// "return", "rol", "ror"
			switch (*(p + 1))
			{
			case 'e':
				if (*(LPDWORD)(p + 2) == BSWAP32('turn'))
				{
					if ((p == lpMarkupStringBuffer || (
						!bPrevIsTailByte &&
						is_separated_left(p - 1))) &&
						(__intrinsic_isspace(*(p + 6)) || *(p + 6) == '('))
					{
						APPEND_TAG_WITH_CONTINUE(TAG_RETURN, 6, PRIORITY_RETURN, OS_PUSH);
					}
					p += 6;
					continue;
				}
				p += 2;
				continue;
			case 'o':
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
				p += 2;
				continue;
			}
			break;
		case 's':
			// "sar", "strlen::"
			// not implemented: "switch"
			switch (*(p + 1))
			{
			case 'a':
				if (*(p + 2) != 'r')
					break;
				iTag = TAG_SAR;
				nLength = 3;
				bPriority = PRIORITY_SAR;
				goto APPEND_WORD_OPERATOR;
			case 't':
				if (*(LPDWORD)(p + 2) != BSWAP32('rlen'))
					break;
				if (*(LPWORD)(p + 6) == BSWAP16('::'))
				{
					iTag = TAG_STRLEN;
					nLength = 8;
					goto APPEND_FUNCTIONAL_OPERATOR;
				}
				p += 6;
				continue;
#if IMPLEMENTED
			case 'w':
				if (*(LPDWORD)(p + 2) != BSWAP32('itch'))
					break;
				if ((p == lpMarkupStringBuffer || (
					!bPrevIsTailByte &&
					is_separated_left(p - 1))) &&
					(__intrinsic_isspace(*(p + 6)) || *(p + 6) == '('))
				{
					APPEND_TAG_WITH_CONTINUE(TAG_SWITCH, 6, PRIORITY_SWITCH, OS_PUSH);
				}
				p += 6;
				continue;
#endif
			}
			break;
		APPEND_WORD_OPERATOR:
			if ((p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				is_separated_left(p - 1))) &&
				is_separated_right(p + nLength))
			{
				APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH);
			}
			p += nLength;
			continue;
		case 'w':
			// "wcslen::", "while"
			switch (*(p + 1))
			{
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
			case 'h':
				if (*(LPWORD)(p + 2) == BSWAP16('il'))
				{
					if (*(p + 4) == 'e')
					{
						if ((p == lpMarkupStringBuffer || (
							!bPrevIsTailByte &&
							is_separated_left(p - 1))) &&
							(__intrinsic_isspace(*(p + 5)) || *(p + 5) == '('))
						{
							if (nFirstWhile == SIZE_MAX)
								nFirstWhile = nNumberOfTag;
							APPEND_TAG_WITH_CONTINUE(TAG_WHILE, 5, PRIORITY_WHILE, OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN);
						}
						p += 5;
						continue;
					}
					p += 4;
					continue;
				}
				p += 2;
				continue;
			}
			break;
		APPEND_FUNCTIONAL_OPERATOR:
			if (p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				is_separated_left(p - 1)))
			{
				APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_FUNCTION, OS_PUSH);
			}
			p += nLength;
			continue;
		case '|':
			// "||", "|="
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
		case '~':
			// "~", "~]", "~2]", "~3]", "~4]", "~5]", "~6]", "~7]", "~8]"
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
#if CODEPAGE_SUPPORT
		default:
			if (!__intrinsic_isleadbyte(*p))
				break;
#else
		case_unsigned_leadbyte:
#endif
			bIsLaedByte = TRUE;
			p += 2;
			continue;
		}
		p++;

	CONTINUE:;
		#undef APPEND_TAG
		#undef APPEND_TAG_WITH_CONTINUE
	}

	lpEndOfTag = lpTagArray + nNumberOfTag;

	// add ternary block
	if (nFirstTernary != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstTernary; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpBegin, *lpEnd;
			size_t nDepth;
			size_t nIndex;
			size_t n;

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
				else if (nDepth || lpTag2->Priority >= PRIORITY_TERNARY)
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
			lpBegin->Tag      = TAG_PARENTHESIS_OPEN;
			lpBegin->Length   = 0;
			lpBegin->Priority = PRIORITY_PARENTHESIS_OPEN;
			lpBegin->Type     = OS_OPEN | OS_PARENTHESIS;
			lpBegin->Depth    = 0;
			nIndex = (size_t)(lpBegin - lpTagArray);
			if (nFirstDo       >= nIndex && (n = nFirstDo       + 1)) nFirstDo       = n;
			if (nFirstIf       >= nIndex && (n = nFirstIf       + 1)) nFirstIf       = n;
			if (nFirstWhile    >= nIndex && (n = nFirstWhile    + 1)) nFirstWhile    = n;
			if (nFirstFor      >= nIndex && (n = nFirstFor      + 1)) nFirstFor      = n;
			if (nFirstMemmove  >= nIndex && (n = nFirstMemmove  + 1)) nFirstMemmove  = n;
			if (nFirstMemset   >= nIndex && (n = nFirstMemset   + 1)) nFirstMemset   = n;
			if (nFirstMemset16 >= nIndex && (n = nFirstMemset16 + 1)) nFirstMemset16 = n;
			if (nFirstMemset32 >= nIndex && (n = nFirstMemset32 + 1)) nFirstMemset32 = n;
			if (nFirstMemset64 >= nIndex && (n = nFirstMemset64 + 1)) nFirstMemset64 = n;
			if (nFirstPrintf   >= nIndex && (n = nFirstPrintf   + 1)) nFirstPrintf   = n;
			if (nFirstDPrintf  >= nIndex && (n = nFirstDPrintf  + 1)) nFirstDPrintf  = n;
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
			lpEnd->Tag      = TAG_PARENTHESIS_CLOSE;
			lpEnd->Length   = 0;
			lpEnd->Priority = PRIORITY_PARENTHESIS_CLOSE;
			lpEnd->Type     = OS_PUSH | OS_CLOSE | OS_PARENTHESIS | OS_TERNARY_END;
			lpEnd->Depth    = 0;
			nIndex = (size_t)(lpEnd - lpTagArray);
			if (nFirstDo       >= nIndex && (n = nFirstDo       + 1)) nFirstDo       = n;
			if (nFirstIf       >= nIndex && (n = nFirstIf       + 1)) nFirstIf       = n;
			if (nFirstWhile    >= nIndex && (n = nFirstWhile    + 1)) nFirstWhile    = n;
			if (nFirstFor      >= nIndex && (n = nFirstFor      + 1)) nFirstFor      = n;
			if (nFirstMemmove  >= nIndex && (n = nFirstMemmove  + 1)) nFirstMemmove  = n;
			if (nFirstMemset   >= nIndex && (n = nFirstMemset   + 1)) nFirstMemset   = n;
			if (nFirstMemset16 >= nIndex && (n = nFirstMemset16 + 1)) nFirstMemset16 = n;
			if (nFirstMemset32 >= nIndex && (n = nFirstMemset32 + 1)) nFirstMemset32 = n;
			if (nFirstMemset64 >= nIndex && (n = nFirstMemset64 + 1)) nFirstMemset64 = n;
			if (nFirstPrintf   >= nIndex && (n = nFirstPrintf   + 1)) nFirstPrintf   = n;
			if (nFirstDPrintf  >= nIndex && (n = nFirstDPrintf  + 1)) nFirstDPrintf  = n;
		}
	}

	// correct printf
	if (nFirstPrintf != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstPrintf; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2;

			if (lpTag1->Tag != TAG_PRINTF)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto PRINTF_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto PRINTF_PARENTHESES_NOT_FOUND;
			if ((lpTag2 = FindParenthesisClose(lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto PRINTF_PARENTHESES_INVALID_PAIRS;
			lpTag1 = lpTag2;
			lpTag1->Tag = TAG_PRINTF_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		PRINTF_PARENTHESES_NOT_FOUND:
		PRINTF_PARENTHESES_INVALID_PAIRS:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct dprintf
	if (nFirstDPrintf != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstDPrintf; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2;

			if (lpTag1->Tag != TAG_DPRINTF)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto DPRINTF_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto DPRINTF_PARENTHESES_NOT_FOUND;
			if ((lpTag2 = FindParenthesisClose(lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto DPRINTF_PARENTHESES_INVALID_PAIRS;
			lpTag1 = lpTag2;
			lpTag1->Tag = TAG_DPRINTF_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		DPRINTF_PARENTHESES_NOT_FOUND:
		DPRINTF_PARENTHESES_INVALID_PAIRS:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct memmove
	if (nFirstMemmove != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstMemmove; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_MEMMOVE)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto MEMMOVE_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto MEMMOVE_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto MEMMOVE_PARENTHESES_INVALID_PAIRS;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMMOVE_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMMOVE_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag3;
			lpTag1->Tag = TAG_MEMMOVE_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		MEMMOVE_PARENTHESES_NOT_FOUND:
		MEMMOVE_PARENTHESES_INVALID_PAIRS:
		MEMMOVE_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct memset
	if (nFirstMemset != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstMemset; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_MEMSET)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto MEMSET_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto MEMSET_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto MEMSET_PARENTHESES_INVALID_PAIRS;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag3;
			lpTag1->Tag = TAG_MEMSET_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		MEMSET_PARENTHESES_NOT_FOUND:
		MEMSET_PARENTHESES_INVALID_PAIRS:
		MEMSET_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct memset16
	if (nFirstMemset16 != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstMemset16; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_MEMSET16)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto MEMSET16_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto MEMSET16_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto MEMSET16_PARENTHESES_INVALID_PAIRS;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET16_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET16_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag3;
			lpTag1->Tag = TAG_MEMSET16_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		MEMSET16_PARENTHESES_NOT_FOUND:
		MEMSET16_PARENTHESES_INVALID_PAIRS:
		MEMSET16_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct memset32
	if (nFirstMemset32 != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstMemset32; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_MEMSET32)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto MEMSET32_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto MEMSET32_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto MEMSET32_PARENTHESES_INVALID_PAIRS;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET32_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET32_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag3;
			lpTag1->Tag = TAG_MEMSET32_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		MEMSET32_PARENTHESES_NOT_FOUND:
		MEMSET32_PARENTHESES_INVALID_PAIRS:
		MEMSET32_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct memset64
	if (nFirstMemset64 != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstMemset64; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_MEMSET64)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto MEMSET64_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto MEMSET64_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto MEMSET64_PARENTHESES_INVALID_PAIRS;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET64_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			while (++lpTag2 < lpTag3 && lpTag2->Tag != TAG_DELIMITER);
			if (lpTag2 >= lpTag3)
				goto MEMSET64_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag3;
			lpTag1->Tag = TAG_MEMSET64_END;
			lpTag1->Type |= OS_PUSH;
			continue;

		MEMSET64_PARENTHESES_NOT_FOUND:
		MEMSET64_PARENTHESES_INVALID_PAIRS:
		MEMSET64_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;
		}
	}

	// correct if block
	if (nFirstIf != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstIf; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;
			MARKUP *lpElse;

			if (lpTag1->Tag != TAG_IF)
				continue;
			if ((lpTag1 + 1) >= lpEndOfTag)
				goto IF_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto IF_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((lpTag2 = FindParenthesisClose(lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto IF_EXPRESSION_PARENTHESES_INVALID_PAIRS;
			if ((lpTag1 = lpTag2) + 1 >= lpEndOfTag)
				goto IF_STATEMENT_NOT_FOUND;
			lpTag2->Tag = TAG_IF_EXPR;
			lpTag2->Type |= OS_PUSH;
			if ((lpTag2 + 1)->Tag != TAG_ELSE)
			{
				if ((lpTag3 = FindEndOfStructuredStatement(++lpTag2, lpEndOfTag)) >= lpEndOfTag)
					goto IF_END_OF_STATEMENT_NOT_FOUND;
				lpTag2 = lpTag3;
			}
			if (lpTag2 + 1 < lpEndOfTag && (lpTag2 + 1)->Tag == TAG_ELSE)
			{
				lpElse = ++lpTag2;
				if (lpTag2 + 1 >= lpEndOfTag)
					goto IF_ELSE_STATEMENT_NOT_FOUND;
				if ((lpTag3 = FindEndOfStructuredStatement(++lpTag2, lpEndOfTag)) >= lpEndOfTag)
					goto IF_END_OF_STATEMENT_NOT_FOUND;
				lpTag2 = lpTag3;
			}
			else
			{
				lpElse = NULL;
			}
			while (--lpTag2 > lpTag1)
				if (lpTag2 != lpElse)
					lpTag2->Depth++;
			continue;

		IF_EXPRESSION_PARENTHESES_NOT_FOUND:
		IF_EXPRESSION_PARENTHESES_INVALID_PAIRS:
		IF_STATEMENT_NOT_FOUND:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;

		IF_END_OF_STATEMENT_NOT_FOUND:
		IF_ELSE_STATEMENT_NOT_FOUND:
			lpTag2->Tag = TAG_PARSE_ERROR;
			lpTag2->Type |= OS_PUSH;
			break;
		}
	}

	// correct do while loop
	if (nFirstDo != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstDo; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_DO)
				continue;
			if ((lpTag1 + 1) >= lpEndOfTag)
				goto DO_WHILE_STATEMENT_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto DO_WHILE_STATEMENT_PARENTHESES_NOT_FOUND;
			if ((lpTag2 = FindParenthesisClose(lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto DO_WHILE_STATEMENT_PARENTHESES_INVALID_PAIRS;
			if (lpTag2 + 1 >= lpEndOfTag)
				goto DO_WHILE_WHILE_NOT_FOUND;
			if ((++lpTag2)->Tag != TAG_WHILE)
				goto DO_WHILE_WHILE_NOT_FOUND;
			lpTag2->Type = OS_PUSH | OS_POST;
			if (lpTag2 + 1 >= lpEndOfTag)
				goto DO_WHILE_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((++lpTag2)->Tag != TAG_PARENTHESIS_OPEN)
				goto DO_WHILE_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2, lpEndOfTag)) >= lpEndOfTag)
				goto DO_WHILE_EXPRESSION_PARENTHESES_INVALID_PAIRS;
			lpTag3->Tag = TAG_WHILE_EXPR;
			lpTag3->Type |= OS_PUSH | OS_POST | OS_LOOP_END;
			continue;

		DO_WHILE_STATEMENT_PARENTHESES_NOT_FOUND:
		DO_WHILE_STATEMENT_PARENTHESES_INVALID_PAIRS:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;

		DO_WHILE_WHILE_NOT_FOUND:
		DO_WHILE_EXPRESSION_PARENTHESES_NOT_FOUND:
		DO_WHILE_EXPRESSION_PARENTHESES_INVALID_PAIRS:
			lpTag2->Tag = TAG_PARSE_ERROR;
			lpTag2->Type |= OS_PUSH;
			break;
		}
	}

	// correct while loop
	if (nFirstWhile != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstWhile; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_WHILE || (lpTag1->Type & OS_POST))
				continue;
			if ((lpTag1 + 1) >= lpEndOfTag)
				goto WHILE_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto WHILE_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((lpTag2 = FindParenthesisClose(lpTag1, lpEndOfTag)) >= lpEndOfTag)
				goto WHILE_EXPRESSION_PARENTHESES_INVALID_PAIRS;
			if ((lpTag1 = lpTag2) + 1 >= lpEndOfTag)
				goto WHILE_STATEMENT_NOT_FOUND;
			lpTag2->Tag = TAG_WHILE_EXPR;
			lpTag2->Type |= OS_PUSH;
			lpTag3 = FindEndOfStructuredStatement(++lpTag2, lpEndOfTag);
			if (lpTag3 >= lpEndOfTag)
				goto WHILE_END_OF_STATEMENT_NOT_FOUND;
			lpTag3->Type |= OS_PUSH | OS_LOOP_END;
			continue;

		WHILE_EXPRESSION_PARENTHESES_NOT_FOUND:
		WHILE_EXPRESSION_PARENTHESES_INVALID_PAIRS:
		WHILE_STATEMENT_NOT_FOUND:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;

		WHILE_END_OF_STATEMENT_NOT_FOUND:
			lpTag2->Tag = TAG_PARSE_ERROR;
			lpTag2->Type |= OS_PUSH;
			break;
		}
	}

	// correct for loop
	if (nFirstFor != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstFor; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpTag2, *lpTag3;

			if (lpTag1->Tag != TAG_FOR)
				continue;
			if (lpTag1 + 1 >= lpEndOfTag)
				goto FOR_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((++lpTag1)->Tag != TAG_PARENTHESIS_OPEN)
				goto FOR_EXPRESSION_PARENTHESES_NOT_FOUND;
			if ((lpTag3 = FindParenthesisClose(lpTag2 = lpTag1, lpEndOfTag)) + 1 >= lpEndOfTag)
				goto FOR_EXPRESSION_PARENTHESES_INVALID_PAIRS;
			lpTag2 = FindSplit(lpTag2 + 1, lpTag3);
			if (lpTag2 >= lpTag3)
				goto FOR_ARGUMENTS_NOT_ENOUGH;
			lpTag1 = lpTag2;
			lpTag2->Tag = TAG_FOR_INITIALIZE;
			lpTag2->Type |= OS_PUSH;
			lpTag2 = FindSplit(lpTag2 + 1, lpTag3);
			if (lpTag2 >= lpTag3)
				goto FOR_ARGUMENTS_NOT_ENOUGH;
			lpTag2->Tag = TAG_FOR_CONDITION;
			lpTag2->Type |= OS_PUSH | OS_LOOP_BEGIN;
			lpTag3->Tag = TAG_FOR_UPDATE;
			lpTag3->Type |= OS_PUSH;
			lpTag3 = FindEndOfStructuredStatement(lpTag2 = (lpTag1 = lpTag3) + 1, lpEndOfTag);
			if (lpTag3 >= lpEndOfTag)
				goto FOR_END_OF_STATEMENT_NOT_FOUND;
			lpTag3->Type |= OS_PUSH | OS_LOOP_END;
			continue;

		FOR_EXPRESSION_PARENTHESES_NOT_FOUND:
		FOR_EXPRESSION_PARENTHESES_INVALID_PAIRS:
		FOR_ARGUMENTS_NOT_ENOUGH:
			lpTag1->Tag = TAG_PARSE_ERROR;
			lpTag1->Type |= OS_PUSH;
			break;

		FOR_END_OF_STATEMENT_NOT_FOUND:
			lpTag2->Tag = TAG_PARSE_ERROR;
			lpTag2->Type |= OS_PUSH;
			break;
		}
	}

	// correct continue of do while loop
	if (nFirstDo != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstDo; lpTag1 < lpEndOfTag; lpTag1++)
		{
			size_t nDepth;

			if (lpTag1->Tag != TAG_DO)
				continue;
			nDepth = 0;
			for (MARKUP *lpTag2 = lpTag1 + 1; lpTag2 < lpEndOfTag; lpTag2++)
			{
				if (lpTag2->Type & OS_LOOP_BEGIN)
				{
					nDepth++;
				}
				else if (lpTag2->Type & OS_LOOP_END)
				{
					if (!nDepth)
						break;
					nDepth--;
				}
				else if (!nDepth && lpTag2->Tag == TAG_CONTINUE)
				{
					lpTag2->Type |= OS_POST;
				}
			}
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
			lpMarkup->Depth    = lpTagArray[nNumberOfTag - 2].Depth + (lpTagArray[nNumberOfTag - 2].Tag == TAG_IF_EXPR || lpTagArray[nNumberOfTag - 2].Tag == TAG_ELSE);
			TrimMarkupString(lpMarkup);
			if (!lpMarkup->Length)
				nNumberOfTag--;
			lpEndOfTag = lpTagArray + nNumberOfTag;
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
		lpMarkup->Depth    = 0;
		TrimMarkupString(lpMarkup);
		if (!lpMarkup->Length)
			goto FAILED2;
		nNumberOfTag++;
		lpEndOfTag++;
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (!lpMarkupArray)
		goto FAILED2;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	nMarkupIndex = 0;
	for (MARKUP *lpTag = lpTagArray; lpTag < lpEndOfTag; lpTag++)
	{
		if ((size_t)(lpTag->String - lpMarkupStringBuffer) > nMarkupIndex)
		{
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpTag->String - lpMarkupStringBuffer - nMarkupIndex;
			lpMarkup->String   = lpMarkupStringBuffer + nMarkupIndex;
			lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type     = OS_PUSH;
			lpMarkup->Depth    = lpTag != lpTagArray ? (lpTag - 1)->Depth + ((lpTag - 1)->Tag == TAG_IF_EXPR || (lpTag - 1)->Tag == TAG_ELSE) : 0;
			nMarkupIndex += lpMarkup->Length;
			TrimMarkupString(lpMarkup);
			if (lpMarkup->Length)
			{
				if (lpMarkup != lpMarkupArray)
				{
					BOOLEAN inDoubleQuote;
					LPBYTE  p, end;

					switch ((lpMarkup - 1)->Tag)
					{
					case TAG_PROCEDURE:
					case TAG_IMPORT_FUNCTION:
					case TAG_IMPORT_REFERENCE:
					case TAG_SECTION:
						inDoubleQuote = FALSE;
						p = lpMarkup->String;
						end = p + lpMarkup->Length;
						do
						{
							BYTE c;

							c = *p;
							if (!__intrinsic_isleadbyte(c))
							{
								size_t length;

								if (c != '\"')
								{
									if (c == '\\' && inDoubleQuote)
									{
										LPBYTE src;
										char   c1, c2;

										if (!(length = --end - p))
											break;
										memcpy(p, p + 1, length);
										switch (c = *p)
										{
										case 'a':
											*p = '\a';
											break;
										case 'b':
											*p = '\b';
											break;
										case 'f':
											*p = '\f';
											break;
										case 'n':
											*p = '\n';
											break;
										case 'r':
											*p = '\r';
											break;
										case 't':
											*p = '\t';
											break;
										case 'v':
											*p = '\v';
											break;
										case 'x':
											c1 = *(src = p + 1);
											if (c1 >= '0' && c1 <= '9')
												c1 -= '0';
											else if (c1 >= 'A' && c1 <= 'F')
												c1 -= 'A' - 0x0A;
											else if (c1 >= 'a' && c1 <= 'a')
												c1 -= 'a' - 0x0A;
											else
												break;
											c2 = *(++src);
											if (c2 >= '0' && c2 <= '9')
												c1 = c1 * 0x10 + c2 - '0';
											else if (c2 >= 'A' && c2 <= 'F')
												c1 = c1 * 0x10 + c2 - ('A' - 0x0A);
											else if (c2 >= 'a' && c2 <= 'a')
												c1 = c1 * 0x10 + c2 - ('a' - 0x0A);
											else
												src--;
											end -= src++ - p;
											*p = c1;
											memcpy(p + 1, src, end - p);
											break;
										default:
											if (__intrinsic_isleadbyte(c))
												p++;
											break;
										}
									}
									p++;
								}
								else
								{
									if (!(length = --end - p))
										break;
									memcpy(p, p + 1, length);
									inDoubleQuote = !inDoubleQuote;
								}
							}
							else
							{
								p += 2;
							}
						} while (p < end);
						lpMarkup->Length = end - lpMarkup->String;
						break;
					}
				}
				if (lpMarkup->Length)
					lpMarkup++;
			}
		}
		lpMarkup->Tag      = lpTag->Tag     ;
		lpMarkup->Length   = lpTag->Length  ;
		lpMarkup->String   = lpTag->String  ;
		lpMarkup->Priority = lpTag->Priority;
		lpMarkup->Type     = lpTag->Type    ;
		lpMarkup->Depth    = lpTag->Depth   ;
		nMarkupIndex = lpMarkup->String + lpMarkup->Length - lpMarkupStringBuffer;
		lpMarkup++;
	}
	lpEndOfMarkup = lpMarkup;

	// release
	HeapFree(hHeap, 0, lpTagArray);

	nDepth = 0;
	for (lpMarkup = lpMarkupArray; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		// get depth of nested loop
		lpMarkup->LoopDepth =
			(lpMarkup->Type & OS_LOOP_BEGIN) ? nDepth++ :
			((lpMarkup->Type & OS_LOOP_END) && nDepth) ? --nDepth :
			nDepth;
		lpMarkup->Depth += lpMarkup->LoopDepth;

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
static size_t __stdcall Postfix(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT MARKUP **lpPostfixBuffer, IN MARKUP **lpFactorBuffer, IN size_t *lpnNestBuffer)
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
static QWORD __cdecl InternalParsing(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, const string *Src, BOOL IsInteger, BOOL IsQuad, va_list ArgPtr)
{
	#define PROCESS_DESIRED_ACCESS (PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION)

	const OPERAND                  OperandZero = { 0, 0, FALSE };
	QWORD                          qwResult;
	LPSTR                          lpszSrc;
	size_t                         nSrcLength;
#if ADDITIONAL_TAGS
	size_t                         capacity;
	TEndWithAttribute              *variable;
	string                         *code;
	size_t                         nVariableLength;
	LPSTR                          p;
#if defined(__BORLANDC__)
	vector<TSSGAttributeElement *> *attributes;
#else
	vector_TSSGAttributeElement    *attributes;
#endif
#endif
	LPSTR                          lpMarkupStringBuffer;
	size_t                         nNumberOfMarkup;
	MARKUP                         *lpMarkupArray;
	MARKUP                         **lpPostfix;
	MARKUP                         **lpFactorBuffer;
	size_t                         *lpnNestBuffer;
	OPERAND                        *lpOperandBuffer, *lpEndOfOperand, *lpOperandTop;
	MARKUP_VARIABLE                *lpVariable;
	size_t                         nNumberOfVariable;
	size_t                         nNumberOfPostfix;
	size_t                         length;
	HANDLE                         hProcess;
	BOOLEAN                        bCompoundAssign;
	OPERAND                        operand;
#if REPEAT_INDEX
	LPSTR                          lpVariableStringBuffer;
#endif

	qwResult = 0;
	lpszSrc = (LPSTR)string_begin(Src);
#if LOCAL_MEMORY_SUPPORT
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
	nSrcLength = string_end(Src) - lpszSrc;
#if ADDITIONAL_TAGS
	variable = (TEndWithAttribute *)TSSGCtrl_GetAttribute(SSGCtrl, SSGS, AT_VARIABLE);
	if (variable && (nVariableLength = string_length(code = TEndWithAttribute_GetCode(variable))))
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
		memcpy(p, string_c_str(code), nVariableLength);
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
#if LOCAL_MEMORY_SUPPORT
	p = lpszSrc;
	while (__intrinsic_isspace(*p))
		p++;
	if (p[0] == 'L' && p[1] && __intrinsic_isascii(p[1]) && !__intrinsic_iscsym(p[1]) && p[1] != '=')
	{
		p++;
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
#if !SUBJECT_STATUS
	nNumberOfVariable = 0;
#else
	lpVariable[0].Length = 4;
	lpVariable[0].String = "Addr";
	lpVariable[0].Value.Quad = (QWORD)SSGS->address;
	lpVariable[0].Value.IsQuad = sizeof(SSGS->address) > sizeof(DWORD);
	lpVariable[1].Length = 4;
	lpVariable[1].String = "Read";
	lpVariable[1].Value.Quad = (QWORD)SSGS->evaluateAtRead;
	lpVariable[1].Value.IsQuad = sizeof(SSGS->evaluateAtRead) > sizeof(DWORD);
	lpVariable[2].Length = 4;
	lpVariable[2].String = "Size";
	lpVariable[2].Value.Quad = (QWORD)TSSGSubject_GetSize(SSGS);
	lpVariable[2].Value.IsQuad = FALSE;
	nNumberOfVariable = 3;
#endif
	while (length = va_arg(ArgPtr, size_t))
	{
		lpVariable[nNumberOfVariable].Length = length;
		lpVariable[nNumberOfVariable].String = va_arg(ArgPtr, LPCSTR);
		lpVariable[nNumberOfVariable].Value.Quad = va_arg(ArgPtr, QWORD);
		lpVariable[nNumberOfVariable].Value.IsQuad = IsQuad;
		nNumberOfVariable++;
	}
#if REPEAT_INDEX
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
			lpVariable[nNumberOfVariable].Value.IsQuad = FALSE;
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
			lpVariable[nNumberOfVariable].Value.IsQuad = FALSE;
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
		case TAG_IF:
		case TAG_DO:
		case TAG_WHILE:
		case TAG_FOR:
			OPERAND_CLEAR();
			break;
		case TAG_IF_EXPR:
			operand = OPERAND_POP();
			OPERAND_CLEAR();
			if (IsInteger)
			{
				if (operand.Quad)
					continue;
			}
			else if (operand.IsQuad)
			{
				if (operand.Double)
					continue;
			}
			else
			{
				if (operand.Float)
					continue;
			}
			while (++i < nNumberOfPostfix && lpPostfix[i]->Depth > lpMarkup->Depth);
			if (i >= nNumberOfPostfix)
				continue;
			if ((lpMarkup = lpPostfix[i])->Tag == TAG_ELSE)
				break;
			i--;
			continue;
		case TAG_ELSE:
			while (++i < nNumberOfPostfix && lpPostfix[i]->Depth > lpMarkup->Depth);
			i--;
			continue;
		case TAG_WHILE_EXPR:
			operand = OPERAND_POP();
			OPERAND_CLEAR();
			if (IsInteger)
				boolValue = !!operand.Quad;
			else if (operand.IsQuad)
				boolValue = !!operand.Double;
			else
				boolValue = !!operand.Float;
			if (!(lpMarkup->Type & OS_POST))
			{
				if (!boolValue)
					while (++i < nNumberOfPostfix && lpPostfix[i]->LoopDepth >= lpMarkup->LoopDepth);
			}
			else
			{
				if (boolValue && i)
					while (--i && lpPostfix[i]->LoopDepth > lpMarkup->LoopDepth);
			}
			continue;
		case TAG_FOR_INITIALIZE:
			OPERAND_CLEAR();
			continue;
		case TAG_FOR_CONDITION:
			operand = OPERAND_POP();
			OPERAND_CLEAR();
			if (IsInteger)
				boolValue = !!operand.Quad;
			else if (operand.IsQuad)
				boolValue = !!operand.Double;
			else
				boolValue = !!operand.Float;
			if (boolValue)
				while (++i < nNumberOfPostfix && lpPostfix[i]->Tag != TAG_FOR_UPDATE);
			else
				while (++i < nNumberOfPostfix && lpPostfix[i]->LoopDepth > lpMarkup->LoopDepth);
			continue;
		case TAG_FOR_UPDATE:
			OPERAND_CLEAR();
			if (i)
				while (--i && lpPostfix[i]->Tag != TAG_FOR_INITIALIZE);
			continue;
		case TAG_PARENTHESIS_CLOSE:
		case TAG_SPLIT:
			if (!(lpMarkup->Type & OS_LOOP_END))
				continue;
			OPERAND_CLEAR();
			if (i)
				while (--i && lpPostfix[i]->LoopDepth > lpMarkup->LoopDepth);
			continue;
		case TAG_BREAK:
			OPERAND_CLEAR();
			while (++i < nNumberOfPostfix && lpPostfix[i]->LoopDepth >= lpMarkup->LoopDepth);
			break;
		case TAG_CONTINUE:
			OPERAND_CLEAR();
			if (lpMarkup->Type & OS_POST)
			{
				while (++i < nNumberOfPostfix && lpPostfix[i]->LoopDepth >= lpMarkup->LoopDepth)
					if (lpPostfix[i]->LoopDepth == lpMarkup->LoopDepth && lpPostfix[i]->Tag == TAG_WHILE)
						break;
				i--;
			}
			else
			{
				if (i)
					while (--i && lpPostfix[i]->LoopDepth >= lpMarkup->LoopDepth);
			}
			break;
		case TAG_PARAM_LOCAL:
		case TAG_DELIMITER:
		case TAG_IMPORT_FUNCTION:
		case TAG_IMPORT_REFERENCE:
			continue;
		case TAG_PRINTF:
		case TAG_DPRINTF:
		case TAG_MEMMOVE:
		case TAG_MEMSET:
		case TAG_MEMSET16:
		case TAG_MEMSET32:
		case TAG_MEMSET64:
		case TAG_RETURN:
			break;
		case TAG_PRINTF_END:
		case TAG_DPRINTF_END:
			if (TSSGCtrl_GetSSGActionListner(SSGCtrl) && i && (lpMarkup->Tag != TAG_PRINTF_END || TMainForm_GetUserMode(MainForm) >= 3))
			{
				TAG        functionTag;
				size_t     j, size;
				LPVOID     buffer;
				PULONG_PTR stack;
				LPSTR      psz;

				functionTag = lpMarkup->Tag - (TAG_PRINTF_END - TAG_PRINTF);
				j = i;
				while ((lpMarkup = lpPostfix[--j])->Tag != functionTag && j);
				if (lpMarkup->Tag != functionTag)
					continue;
				size = 0;
				while (++j < i)
				{
					size += sizeof(QWORD);
					lpMarkup = lpPostfix[j];
					if (lpMarkup->Tag == TAG_NOT_OPERATOR && *lpMarkup->String == '"')
						size += lpMarkup->Length;
					while (++j < i && lpPostfix[j]->Tag != TAG_DELIMITER);
				}
				if (!size)
					continue;
				buffer = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
				if (!buffer)
					continue;
				stack = (PULONG_PTR)((LPBYTE)buffer + size);
				psz = (LPSTR)buffer;
				for (j = i - 1; ; j--)
				{
					if (lpPostfix[j]->Tag != TAG_DELIMITER && lpPostfix[j]->Tag != functionTag)
						continue;
					lpMarkup = lpPostfix[j + 1];
					if (lpMarkup->Tag != TAG_NOT_OPERATOR || *lpMarkup->String != '"')
					{
						operand = OPERAND_POP();
#ifndef _WIN64
						if (!operand.IsQuad)
							*(--stack) = operand.Low;
						else
							*(--(PULONG64)stack) = operand.Quad;
#else
						*(--stack) = operand.Quad;
#endif
					}
					else
					{
						size_t length;
						LPSTR  string, end;

						length = lpMarkup->Length;
						string = lpMarkup->String;
						if (--length && string[length] == '"')
							length--;
						memcpy(psz, ++string, length);
						*(--stack) = (ULONG_PTR)psz;
						end = psz + length;
						while (*psz)
						{
							if (!__intrinsic_isleadbyte(*psz))
							{
								if (*psz == '"')
								{
									LPSTR p = psz + 1;
									while (__intrinsic_isspace(*p))
										p++;
									if (*p == '"')
									{
										end -= ++p - psz;
										memcpy(psz, p, end - psz);
									}
									else
									{
										*psz = '\0';
										break;
									}
								}
								else if (*(psz++) == '\\')
								{
									if (__intrinsic_isleadbyte(*psz) && !*(++psz))
										break;
									psz++;
								}
							}
							else
							{
								if (!*(++psz))
									break;
								psz++;
							}
						}
						psz++;
					}
					if (lpPostfix[j]->Tag == functionTag)
						break;
				}
				if (functionTag == TAG_PRINTF)
					GuidePrintV((const char *)*stack, (va_list)(stack + 1));
				else
					DebugPrintV((const char *)*stack, (va_list)(stack + 1));
				HeapFree(hHeap, 0, buffer);
			}
			continue;
		case TAG_MEMMOVE_END:
			{
				HANDLE     hDestProcess;
				void       *lpDest;
				HANDLE     hSrcProcess;
				const void *lpSrc;
				NTSTATUS   Status;

				operand = OPERAND_POP();
				nSize = IsInteger ?
					(size_t)operand.Quad :
					operand.IsQuad ?
						(size_t)operand.Double :
						(size_t)operand.Float;
				operand = OPERAND_POP();
				lpSrc = IsInteger ?
					(const void *)(INT_PTR)operand.Quad :
					operand.IsQuad ?
						(const void *)(INT_PTR)operand.Double :
						(const void *)(INT_PTR)operand.Float;
				lpDest = IsInteger ?
					(void *)(INT_PTR)lpOperandTop->Quad :
					lpOperandTop->IsQuad ?
						(void *)(INT_PTR)lpOperandTop->Double :
						(void *)(INT_PTR)lpOperandTop->Float;
				hDestProcess = (HANDLE)TRUE;
				hSrcProcess = (HANDLE)TRUE;
				do	/* do { ... } while (0); */
				{
					size_t j;

					j = i;
					while (lpPostfix[--j]->Tag != TAG_DELIMITER && j);
					if (lpPostfix[j]->Tag != TAG_DELIMITER)
						break;
					while (lpPostfix[--j]->Tag != TAG_DELIMITER && j);
					if (lpPostfix[j]->Tag != TAG_DELIMITER)
						break;
					if (lpPostfix[j + 1]->Tag == TAG_PARAM_LOCAL)
						hSrcProcess = NULL;
					while (lpPostfix[--j]->Tag != TAG_MEMMOVE && j);
					if (lpPostfix[j]->Tag != TAG_MEMMOVE)
						break;
					if (lpPostfix[j + 1]->Tag == TAG_PARAM_LOCAL)
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
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (unsigned long)lpAddress);
						goto FAILED10;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
					WRITE_ERROR:
						TSSGActionListner_OnSubjectWriteError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (unsigned long)lpAddress);
						goto FAILED10;
					}
					goto PARSING_ERROR;
				}
			}
			continue;
		case TAG_MEMSET_END:
		case TAG_MEMSET16_END:
		case TAG_MEMSET32_END:
		case TAG_MEMSET64_END:
			{
				TAG    functionTag;
				HANDLE hDestProcess;
				PVOID  lpDest;
				QWORD  qwFill;
				size_t nCount;
				BOOL   bSuccess;

				functionTag = lpMarkup->Tag - (TAG_MEMSET_END - TAG_MEMSET);
				operand = OPERAND_POP();
				nCount = IsInteger ?
					(size_t)operand.Quad :
					operand.IsQuad ?
						(size_t)operand.Double :
						(size_t)operand.Float;
				operand = OPERAND_POP();
				qwFill = operand.Quad;
				lpDest = IsInteger ?
					(PVOID)(INT_PTR)lpOperandTop->Quad :
					lpOperandTop->IsQuad ?
						(PVOID)(INT_PTR)lpOperandTop->Double :
						(PVOID)(INT_PTR)lpOperandTop->Float;
				hDestProcess = (HANDLE)TRUE;
				do	/* do { ... } while (0); */
				{
					size_t j;

					j = i;
					while (lpPostfix[--j]->Tag != TAG_DELIMITER && j);
					if (lpPostfix[j]->Tag != TAG_DELIMITER)
						break;
					while (lpPostfix[--j]->Tag != TAG_DELIMITER && j);
					if (lpPostfix[j]->Tag != TAG_DELIMITER)
						break;
					while (lpPostfix[--j]->Tag != functionTag && j);
					if (lpPostfix[j]->Tag != functionTag)
						break;
					if (lpPostfix[j + 1]->Tag == TAG_PARAM_LOCAL)
						hDestProcess = NULL;
				} while (0);
				if (hDestProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto FAILED9;
					hDestProcess = hProcess;
				}
				switch (functionTag)
				{
				case TAG_MEMSET:
					bSuccess = FillProcessMemory(hDestProcess, lpDest, nCount, (BYTE)qwFill);
					break;
				case TAG_MEMSET16:
					bSuccess = FillProcessMemory16(hDestProcess, lpDest, nCount, (WORD)qwFill);
					break;
				case TAG_MEMSET32:
					bSuccess = FillProcessMemory32(hDestProcess, lpDest, nCount, (DWORD)qwFill);
					break;
				default:
					bSuccess = FillProcessMemory64(hDestProcess, lpDest, nCount, qwFill);
					break;
				}
				if (!bSuccess)
					goto WRITE_ERROR;
			}
			continue;
		case TAG_ADD:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low += operand.Low;
				else
					lpOperandTop->Quad += operand.Quad;
			}
			else if (lpOperandTop->IsQuad)
			{
				if (operand.IsQuad)
					lpOperandTop->Double += operand.Double;
				else
					lpOperandTop->Double += operand.Float;
			}
			else
			{
				if (!operand.IsQuad)
				{
					lpOperandTop->Float += operand.Float;
				}
				else
				{
					lpOperandTop->Double = lpOperandTop->Float + operand.Double;
					lpOperandTop->IsQuad = TRUE;
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
					lpOperandTop->Low -= operand.Low;
				else
					lpOperandTop->Quad -= operand.Quad;
			}
			else if (lpOperandTop->IsQuad)
			{
				if (operand.IsQuad)
					lpOperandTop->Double -= operand.Double;
				else
					lpOperandTop->Double -= operand.Float;
			}
			else
			{
				if (!operand.IsQuad)
				{
					lpOperandTop->Float -= operand.Float;
				}
				else
				{
					lpOperandTop->Double = lpOperandTop->Float - operand.Double;
					lpOperandTop->IsQuad = TRUE;
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
					lpOperandTop->Low *= operand.Low;
				else
					lpOperandTop->Quad *= operand.Quad;
			}
			else if (lpOperandTop->IsQuad)
			{
				if (operand.IsQuad)
					lpOperandTop->Double *= operand.Double;
				else
					lpOperandTop->Double *= operand.Float;
			}
			else
			{
				if (!operand.IsQuad)
				{
					lpOperandTop->Float *= operand.Float;
				}
				else
				{
					lpOperandTop->Double = lpOperandTop->Float * operand.Double;
					lpOperandTop->IsQuad = TRUE;
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
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED10;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (long)lpOperandTop->Low / (long)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) / (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
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
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED10;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low /= operand.Low;
				else
					lpOperandTop->Quad /= operand.Quad;
			}
			else if (operand.IsQuad)
			{
				if (!operand.Double)
					goto FAILED10;
				if (lpOperandTop->IsQuad)
					lpOperandTop->Double /= operand.Double;
				else
				{
					lpOperandTop->Double = lpOperandTop->Float / operand.Double;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			else
			{
				if (!operand.Float)
					goto FAILED10;
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Float /= operand.Float;
				else
					lpOperandTop->Double /= operand.Float;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_IMOD:
			if (IsInteger)
			{
				BOOL IsQuad;

				operand = OPERAND_POP();
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED10;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (long)lpOperandTop->Low % (long)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) % (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
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
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED10;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low %= operand.Low;
				else
					lpOperandTop->Quad %= operand.Quad;
			}
			else if (operand.IsQuad)
			{
				if (!operand.Double)
					goto FAILED10;
				if (lpOperandTop->IsQuad)
					lpOperandTop->Double = fmod(lpOperandTop->Double, operand.Double);
				else
				{
					lpOperandTop->Double = fmod(lpOperandTop->Float, operand.Double);
					lpOperandTop->IsQuad = TRUE;
				}
			}
			else
			{
				if (!operand.Float)
					goto FAILED10;
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Float = fmodf(lpOperandTop->Float, operand.Float);
				else
					lpOperandTop->Double = fmod(lpOperandTop->Double, operand.Float);
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_NEG:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = -(long)lpOperandTop->Low;
				else
					lpOperandTop->Quad = -(LONG64)lpOperandTop->Quad;
			}
			else
			{
				if (lpOperandTop->IsQuad)
					lpOperandTop->Float = -lpOperandTop->Float;
				else
					lpOperandTop->Double = -lpOperandTop->Double;
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
			if (!IsInteger)
				operand.Quad = operand.IsQuad ? (__int64)operand.Double : (__int64)operand.Float;
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = operand.Quad < sizeof(DWORD) * 8 ? lpOperandTop->Low << operand.Low : 0;
			else
				lpOperandTop->Quad = operand.Quad < sizeof(QWORD) * 8 ? lpOperandTop->Quad << operand.Low : 0;
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
			if (!IsInteger)
				operand.Quad = operand.IsQuad ? (__int64)operand.Double : (__int64)operand.Float;
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = operand.Quad < sizeof(DWORD) * 8 ? lpOperandTop->Low >> operand.Low : 0;
			else
				lpOperandTop->Quad = operand.Quad < sizeof(QWORD) * 8 ? lpOperandTop->Quad >> operand.Low : 0;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_SAR:
			operand = OPERAND_POP();
			if (!IsInteger)
				operand.Quad = operand.IsQuad ? (__int64)operand.Double : (__int64)operand.Float;
			if (!lpOperandTop->IsQuad)
			{
				if (operand.Quad > sizeof(DWORD) * 8)
					operand.Low = sizeof(DWORD) * 8;
				lpOperandTop->Low = (long)lpOperandTop->Low >> operand.Low;
			}
			else
			{
				if (operand.Quad > sizeof(QWORD) * 8)
					operand.Low = sizeof(QWORD) * 8;
				lpOperandTop->Quad = (LONG64)lpOperandTop->Quad >> operand.Low;
			}
			break;
		case TAG_ROL:
			operand = OPERAND_POP();
			if (!IsInteger)
				operand.Quad = operand.IsQuad ? (__int64)operand.Double : (__int64)operand.Float;
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = _rotl(lpOperandTop->Low, operand.Low);
			else
				lpOperandTop->Quad = _rotl64(lpOperandTop->Quad, operand.Low);
			break;
		case TAG_ROR:
			operand = OPERAND_POP();
			if (!IsInteger)
				operand.Quad = operand.IsQuad ? (__int64)operand.Double : (__int64)operand.Float;
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = _rotr(lpOperandTop->Low, operand.Low);
			else
				lpOperandTop->Quad = _rotr64(lpOperandTop->Quad, operand.Low);
			break;
		case TAG_BIT_AND:
			operand = OPERAND_POP();
			lpOperandTop->Quad &= operand.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_OR:
			operand = OPERAND_POP();
			lpOperandTop->Quad |= operand.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_XOR:
			operand = OPERAND_POP();
			lpOperandTop->Quad ^= operand.Quad;
			lpOperandTop->IsQuad |= operand.IsQuad;
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_NOT:
			lpOperandTop->Low = ~lpOperandTop->Low;
			if (lpOperandTop->IsQuad)
				lpOperandTop->High = ~lpOperandTop->High;
			break;
		case TAG_AND:
			if (IsInteger)
			{
				boolValue = !!lpOperandTop->Quad;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else if (lpOperandTop->IsQuad)
			{
				boolValue = !!lpOperandTop->Double;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
					lpOperandTop->Double = boolValue;
			}
			else
			{
				boolValue = !!lpOperandTop->Float;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
					lpOperandTop->Float = boolValue;
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
				boolValue = !!lpOperandTop->Quad;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else if (lpOperandTop->IsQuad)
			{
				boolValue = !!lpOperandTop->Double;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
					lpOperandTop->Double = boolValue;
			}
			else
			{
				boolValue = !!lpOperandTop->Float;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
					lpOperandTop->Float = boolValue;
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
				lpOperandTop->Quad = !lpOperandTop->Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = !lpOperandTop->Double;
			}
			else
			{
				lpOperandTop->Float = !lpOperandTop->Float;
			}
			break;
		case TAG_EQ:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad == operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double == (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float == (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_NE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad != operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double != (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float != (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_LT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Low < (long)operand.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) < (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_BT:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad < operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double < (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float < (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_GT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Low > (long)operand.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) > (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_AT:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad > operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double > (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float > (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_LE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Low <= (long)operand.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) <= (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_BE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad <= operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double <= (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float <= (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_GE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(long)lpOperandTop->Low >= (long)operand.Low :
					(lpOperandTop->IsQuad ? (__int64)lpOperandTop->Quad : (long)lpOperandTop->Low) >= (operand.IsQuad ? (__int64)operand.Quad : (long)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
		case TAG_AE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad >= operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Double >= (operand.IsQuad ? operand.Double : operand.Float);
			}
			else
			{
				lpOperandTop->Float = (float)(lpOperandTop->Float >= (!operand.IsQuad ? operand.Float : operand.Double));
			}
			break;
		case TAG_TERNARY:
			operand = OPERAND_POP();
			if (IsInteger ? !operand.Quad : !operand.IsQuad ? !operand.Float : !operand.Double)
			{
				size_t nDepth;

				nDepth = 0;
				while (++i < nNumberOfPostfix)
				{
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
			}
			break;
		case TAG_TERNARY_SPLIT:
			for (; i + 1 < nNumberOfPostfix; i++)
				if (lpPostfix[i + 1]->Type & OS_TERNARY_END)
					break;
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
					lpAddress = (LPVOID)lpOperandTop->Quad;
				else
					lpAddress = (LPVOID)(!lpOperandTop->IsQuad ? (INT_PTR)lpOperandTop->Float : (INT_PTR)lpOperandTop->Double);
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
					lpOperandTop->Quad = 0;
					if (!ReadProcessMemory(hProcess, lpAddress, &lpOperandTop->Quad, nSize, NULL))
						lpOperandTop->Quad = 0;
					lpOperandTop->IsQuad = nSize > sizeof(DWORD);
				}
				else
				{
					OPERAND_POP();
					if (!WriteProcessMemory(hProcess, lpAddress, &lpOperandTop->Quad, nSize, NULL))
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
#if LOCAL_MEMORY_SUPPORT
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
					lpAddress = (LPVOID)lpOperandTop->Quad;
				else
					lpAddress = (LPVOID)(!lpOperandTop->IsQuad ? (INT_PTR)lpOperandTop->Float : (INT_PTR)lpOperandTop->Double);
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
							lpOperandTop->Quad = *(LPBYTE)lpAddress;
							break;
						case 2:
							lpOperandTop->Quad = *(LPWORD)lpAddress;
							break;
						case 3:
							lpOperandTop->Quad = *(LPWORD)lpAddress | ((DWORD)*((LPBYTE)lpAddress + 2) << 16);
							break;
						case 4:
							lpOperandTop->Quad = *(LPDWORD)lpAddress;
							break;
						case 5:
							lpOperandTop->Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPBYTE)lpAddress + 4) << 32);
							break;
						case 6:
							lpOperandTop->Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPWORD)lpAddress + 2) << 32);
							break;
						case 7:
							lpOperandTop->Quad = *(LPDWORD)lpAddress | ((QWORD)*((LPWORD)lpAddress + 2) << 32) | ((QWORD)*((LPBYTE)lpAddress + 6) << 48);
							break;
						case 8:
							lpOperandTop->Quad = *(LPQWORD)lpAddress;
							break;
						}
					}
					else
					{
						lpOperandTop->Quad = 0;
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
						*(LPBYTE)lpAddress = (BYTE)lpOperandTop->Low;
						break;
					case 2:
						*(LPWORD)lpAddress = (WORD)lpOperandTop->Low;
						break;
					case 3:
						*(LPWORD)lpAddress = (WORD)lpOperandTop->Low;
						*((LPBYTE)lpAddress + 2) = (BYTE)(lpOperandTop->Low >> 16);
						break;
					case 4:
						*(LPDWORD)lpAddress = lpOperandTop->Low;
						break;
					case 5:
						*(LPDWORD)lpAddress = lpOperandTop->Low;
						*((LPBYTE)lpAddress + 4) = (BYTE)lpOperandTop->High;
						break;
					case 6:
						*(LPDWORD)lpAddress = lpOperandTop->Low;
						*((LPWORD)lpAddress + 2) = (WORD)lpOperandTop->High;
						break;
					case 7:
						*(LPDWORD)lpAddress = lpOperandTop->Low;
						*((LPWORD)lpAddress + 2) = (WORD)lpOperandTop->High;
						*((LPBYTE)lpAddress + 6) = (BYTE)(lpOperandTop->High >> 16);
						break;
					case 8:
						*(LPQWORD)lpAddress = lpOperandTop->Quad;
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
		case TAG_REV_ENDIAN2:
			lpOperandTop->Quad = __intrinsic_bswap16((WORD)lpOperandTop->Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN3:
			lpOperandTop->Quad = __intrinsic_bswap24(lpOperandTop->Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN4:
			lpOperandTop->Quad = __intrinsic_bswap32(lpOperandTop->Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN5:
			lpOperandTop->Quad = __intrinsic_bswap40(lpOperandTop->Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_REV_ENDIAN6:
			lpOperandTop->Quad = __intrinsic_bswap48(lpOperandTop->Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_REV_ENDIAN7:
			lpOperandTop->Quad = __intrinsic_bswap56(lpOperandTop->Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_REV_ENDIAN8:
			lpOperandTop->Quad = __intrinsic_bswap64(lpOperandTop->Quad);
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_ADDR_REPLACE:
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, &lpOperandTop->Low, AT_REPLACE) != 0)
				goto FAILED10;
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Low;
			}
			else
			{
				lpOperandTop->Float = (float)lpOperandTop->Low;
			}
			break;
		case TAG_ADDR_ADJUST:
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, &lpOperandTop->Low, AT_ADJUST) != 0)
				goto FAILED10;
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = lpOperandTop->Low;
			}
			else
			{
				lpOperandTop->Float = (float)lpOperandTop->Low;
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
					lpVariable[j].Value = *lpOperandTop;
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
					lpVariable[nNumberOfVariable].Value = *lpOperandTop;
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
				lpOperandTop->Quad = StringLengthA(hProcess, (LPCSTR)lpOperandTop->Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(DWORD);
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = StringLengthA(hProcess, (LPCSTR)(size_t)lpOperandTop->Double);
			}
			else
			{
				lpOperandTop->Float = (float)StringLengthA(hProcess, (LPCSTR)(size_t)lpOperandTop->Float);
			}
			break;
		case TAG_WCSLEN:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED9;
			if (IsInteger)
			{
				lpOperandTop->Quad = StringLengthW(hProcess, (LPCWSTR)lpOperandTop->Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(DWORD);
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = StringLengthW(hProcess, (LPCWSTR)(size_t)lpOperandTop->Double);
			}
			else
			{
				lpOperandTop->Float = (float)StringLengthW(hProcess, (LPCWSTR)(size_t)lpOperandTop->Float);
			}
			break;
		case TAG_BSF:
			if (!lpOperandTop->IsQuad)
			{
				long Mask;

				Mask = lpOperandTop->Low;
				if (!_BitScanForward(&lpOperandTop->Low, Mask))
					lpOperandTop->Low = 0xFFFFFFFF;
			}
			else
			{
				unsigned __int64 Mask;

				Mask = lpOperandTop->Quad;
				if (!_BitScanForward64(&lpOperandTop->Low, Mask))
					lpOperandTop->Low = 0xFFFFFFFF;
			}
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = (long)lpOperandTop->Low;
			}
			else
			{
				lpOperandTop->Float = (float)(long)lpOperandTop->Low;
			}
			break;
		case TAG_BSR:
			if (!lpOperandTop->IsQuad)
			{
				long Mask;

				Mask = lpOperandTop->Low;
				if (!_BitScanReverse(&lpOperandTop->Low, Mask))
					lpOperandTop->Low = 0xFFFFFFFF;
			}
			else
			{
				unsigned __int64 Mask;

				Mask = lpOperandTop->Quad;
				if (!_BitScanReverse64(&lpOperandTop->Low, Mask))
					lpOperandTop->Low = 0xFFFFFFFF;
			}
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Double = (long)lpOperandTop->Low;
			}
			else
			{
				lpOperandTop->Float = (float)(long)lpOperandTop->Low;
			}
			break;
		case TAG_CAST32:
			if (IsInteger)
			{
				lpOperandTop->High = 0;
			}
			else if (lpOperandTop->IsQuad)
			{
				lpOperandTop->Float = (float)lpOperandTop->Double;
				lpOperandTop->High = 0;
			}
			lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_CAST64:
			if (!IsInteger && !lpOperandTop->IsQuad)
				lpOperandTop->Double = lpOperandTop->Float;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_I1TOI4:
			lpOperandTop->Quad = (long)(char)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I2TOI4:
			lpOperandTop->Quad = (long)(short)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I4TOI8:
			lpOperandTop->Quad = (__int64)(long)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
#if ALLOCATE_SUPPORT
		case TAG_MEMORY:
			{
				lpAddress = NULL;
				if (!IsInteger)
					lpOperandTop->Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Float : (__int64)lpOperandTop->Double;
				if (!lpOperandTop->High)
				{
					for (size_t j = 0; j < nNumberOfProcessMemory; j++)
					{
						if (lpOperandTop->Low != lpProcessMemory[j].Id)
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
				lpOperandTop->Quad = (QWORD)lpAddress;
			}
			if (IsInteger)
				lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(DWORD);
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
			else
				lpOperandTop->Double = (size_t)lpOperandTop->Quad;
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
				lpOperandTop->Quad = lpOperandTop->IsQuad ? (__int64)lpOperandTop->Double : (__int64)lpOperandTop->Float;
			if (!operand.High && IS_INTRESOURCE(operand.Low))
			{
				lpOperandTop->Quad = (QWORD)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, (LPSTR)operand.Quad);
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
				else if (lpOperandTop->IsQuad)
					lpOperandTop->Double = (size_t)lpOperandTop->Quad;
				else
					lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
			}
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
				lpOperandTop->Quad = lpOperandTop->IsQuad ? (__int64)lpOperandTop->Double : (__int64)lpOperandTop->Float;
			if (!operand.High && IS_INTRESOURCE(operand.Low))
			{
				char   c;
				LPVOID lpFunction;

				c = lpMarkup->String[lpMarkup->Length];
				lpMarkup->String[lpMarkup->Length] = '\0';
				lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, (LPSTR)operand.Quad);
				if (lpFunction && (lpMarkup - 1)->Tag == TAG_IMPORT_FUNCTION)
					if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
						lpFunction = NULL;
				lpOperandTop->Quad = (QWORD)lpFunction;
				lpMarkup->String[lpMarkup->Length] = c;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
				else if (lpOperandTop->IsQuad)
					lpOperandTop->Double = (size_t)lpOperandTop->Quad;
				else
					lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
			}
			break;
		case TAG_HNUMBER:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if ((lpMarkup + 1)->Priority <= lpMarkup->Priority)
				break;
			if (!IsInteger)
				lpOperandTop->Quad = lpOperandTop->IsQuad ? (__int64)lpOperandTop->Double : (__int64)lpOperandTop->Float;
			if (!lpOperandTop->High)
			{
				THeapListData *HeapL;

				HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, lpOperandTop->Low - 1);
				lpOperandTop->Low = HeapL ? HeapL->heapListAddress : 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(DWORD);
				else if (lpOperandTop->IsQuad)
					lpOperandTop->Double = (size_t)lpOperandTop->Quad;
				else
					lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(DWORD);
			}
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

				p = lpMarkup->String;
				if (*p == '"' || *p == '\'')
					continue;
				length = lpMarkup->Length;
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
						operand.Quad = _strtoui64(p, &endptr, 0);
					}
					else
					{
						if (IsQuad)
							operand.Double = strtod(p, &endptr);
						else
							operand.Float = (float)strtod(p, &endptr);
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
						operand.Quad = _strtoui64(p, &endptr, 0);
						if (endptr == end)
						{
							if (IsQuad)
							{
								operand.Double = (double)(QWORD)operand.Quad;
								if (_isnan(operand.Double))
									endptr = p;
							}
							else
							{
								operand.Float = (float)(QWORD)operand.Quad;
								if (_isnan(operand.Float))
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
				if (!element && lpNext && (lpNext->Tag == TAG_INC || lpNext->Tag == TAG_DEC) && length)
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
					element->Value.Quad = 0;
					element->Value.IsQuad = IsQuad;
				}
				switch (lpNext ? lpNext->Tag : ~0)
				{
				case TAG_INC:
					if (!element)
						break;
					operand = element->Value;
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = ++element->Value.Low;
							else
								operand.Quad = ++element->Value.Quad;
						}
						else
						{
							if (lpOperandTop->IsQuad)
								operand.Double = element->Value.Double += 1;
							else
								operand.Float = element->Value.Float += 1;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = element->Value.Low++;
							else
								operand.Quad = element->Value.Quad++;
						}
						else
						{
							if (lpOperandTop->IsQuad)
							{
								operand.Double = element->Value.Double;
								element->Value.Double += 1;
							}
							else
							{
								operand.Float = element->Value.Float;
								element->Value.Float += 1;
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
					operand = element->Value;
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = --element->Value.Low;
							else
								operand.Quad = --element->Value.Quad;
						}
						else
						{
							if (lpOperandTop->IsQuad)
								operand.Double = element->Value.Double -= 1;
							else
								operand.Float = element->Value.Float -= 1;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = element->Value.Low--;
							else
								operand.Quad = element->Value.Quad--;
						}
						else
						{
							if (lpOperandTop->IsQuad)
							{
								operand.Double = element->Value.Double;
								element->Value.Double -= 1;
							}
							else
							{
								operand.Float = element->Value.Float;
								element->Value.Float -= 1;
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
					element->Value = *lpOperandTop;
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
						operand.Quad = lpme ? (QWORD)lpme->hModule : 0;
						operand.IsQuad = sizeof(HMODULE) > sizeof(DWORD);
						OPERAND_PUSH(operand);
					}
					i++;
					break;
				case TAG_PROCEDURE:
					if ((HMODULE)lpOperandTop->Quad)
					{
						LPSTR lpProcName;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
						{
							if (!lpOperandTop->IsQuad)
								operand.Quad = (__int64)operand.Float;
							else
								operand.Quad = (__int64)operand.Double;
						}
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = !lpOperandTop->IsQuad ? (__int64)lpOperandTop->Float : (__int64)lpOperandTop->Double;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpOperandTop->Quad = (QWORD)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpProcName);
						lpMarkup->String[lpMarkup->Length] = c;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
						else if (lpOperandTop->IsQuad)
							lpOperandTop->Double = (size_t)lpOperandTop->Quad;
						else
							lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
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
					if ((HMODULE)lpOperandTop->Quad)
					{
						LPSTR  lpProcName;
						char   c2;
						LPVOID lpFunction;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
						{
							if (!lpOperandTop->IsQuad)
								operand.Quad = (__int64)operand.Float;
							else
								operand.Quad = (__int64)operand.Double;
						}
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = lpOperandTop->IsQuad ? (__int64)lpOperandTop->Double : (__int64)lpOperandTop->Float;
						c2 = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpModuleName, lpProcName);
						if (lpFunction && lpNext->Tag == TAG_IMPORT_FUNCTION)
							if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
								lpFunction = NULL;
						lpOperandTop->Quad = (QWORD)lpFunction;
						lpMarkup->String[lpMarkup->Length] = c2;
						if (lpEndOfModuleName)
							*lpEndOfModuleName = c;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(DWORD);
						else if (lpOperandTop->IsQuad)
							lpOperandTop->Double = (size_t)lpOperandTop->Quad;
						else
							lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
					}
					i++;
					break;
				case TAG_SECTION:
					if ((HMODULE)lpOperandTop->Quad)
					{
						BOOL  IsEndOfSection;
						DWORD dwSectionSize;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED9;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						IsEndOfSection = *(LPWORD)lpNext->String == BSWAP16(':+');
						if (!IsInteger)
							lpOperandTop->Quad = lpOperandTop->IsQuad ? (__int64)lpOperandTop->Double : (__int64)lpOperandTop->Float;
						lpOperandTop->Quad = (QWORD)GetSectionAddress(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, IsEndOfSection ? &dwSectionSize : NULL);
						if (IsEndOfSection)
							lpOperandTop->Quad += dwSectionSize;
						if (IsInteger)
							lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(DWORD);
						else if (lpOperandTop->IsQuad)
							lpOperandTop->Double = (size_t)lpOperandTop->Quad;
						else
							lpOperandTop->Float = (float)(size_t)lpOperandTop->Quad;
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
							operand.Quad = (__int64)operand.Float;
						else
							operand.Quad = (__int64)operand.Double;
					}
					if (operand.High)
						goto PARSING_ERROR;
					{
						THeapListData *HeapL;

						HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, operand.Low - 1);
						if (HeapL)
							operand.Quad = HeapL->heapListAddress;
					}
					if (IsInteger)
						operand.IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(DWORD);
					else if (lpOperandTop->IsQuad)
						operand.Double = (size_t)operand.Quad;
					else
						operand.Float = (float)(size_t)operand.Quad;
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
						operand = element->Value;
						OPERAND_PUSH(operand);
					}
					else if (endptr == end)
					{
						operand.IsQuad = IsInteger ? operand.High != 0 : IsQuad;
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
			if (lpMarkup->Type & (OS_TERNARY_END | OS_LOOP_END))
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
				TSSGActionListner_OnParsingProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpGuideText, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpGuideText, lpOperandTop->IsQuad ? lpOperandTop->Double : lpOperandTop->Float);
			if (lpGuideText == lpMarkup->String)
				lpMarkup->String[lpMarkup->Length] = c;
#else
			lpGuideText = lpMarkup->String;
			nGuideTextLength = lpMarkup->Length;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(lpGuideText, nGuideTextLength, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(lpGuideText, nGuideTextLength, lpOperandTop->IsQuad ? lpOperandTop->Double : lpOperandTop->Float);
#endif
		}
		if (lpMarkup->Tag == TAG_RETURN)
			break;
	}
	qwResult = lpOperandTop->Quad;
FAILED10:
	if (hProcess)
		CloseHandle(hProcess);
FAILED9:
#if REPEAT_INDEX
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
#if ADDITIONAL_TAGS
	HeapFree(hHeap, 0, lpszSrc);
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
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...)
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
	Result = InternalParsing(this, SSGS, Src, TRUE, FALSE, ArgPtr);
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
double __cdecl ParsingDouble(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, IN double Val)
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
	Result.Quad = InternalParsing(this, SSGS, Src, FALSE, TRUE, (va_list)&Param);

	return Result.Double;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------

#undef IMPLEMENTED

#if defined(__BORLANDC__)
#undef _ultoa
#undef fmodf
#undef string
#undef string_c_str
#undef string_begin
#undef string_end
#undef string_length
#undef vector_TSSGAttributeElement
#undef TMainForm_GetUserMode
#undef TSSGCtrl_GetAttribute
#undef TSSGCtrl_GetSSGActionListner
#undef TSSGCtrl_AddressAttributeFilter
#undef TSSGSubject_GetAttribute
#undef TSSGSubject_GetSize
#undef TSSGActionListner_OnSubjectReadError
#undef TSSGActionListner_OnSubjectWriteError
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

#undef OS_PUSH
#undef OS_OPEN
#undef OS_CLOSE
#undef OS_SPLIT
#undef OS_MONADIC
#undef OS_POST
#undef OS_SHORT_CIRCUIT
#undef OS_LEFT_ASSIGN
#undef OS_PARENTHESIS
#undef OS_HAS_EXPR
#undef OS_TERNARY
#undef OS_TERNARY_END
#undef OS_LOOP_BEGIN
#undef OS_LOOP_END
#undef OS_RET_OPERAND

