#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#if !defined(__BORLANDC__)
#include <stdint.h>
#include <float.h>
#include <math.h>
#else
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#include <stdint.h>
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#ifdef _WIN64
#define PTRDIFF_MIN _I64_MIN
#define PTRDIFF_MAX _I64_MAX
#define SIZE_MAX    _UI64_MAX
#else
#define PTRDIFF_MIN INT_MIN
#define PTRDIFF_MAX INT_MAX
#define SIZE_MAX    UINT_MAX
#endif
#include <float.h>
#include <math.h>
#define trunc(x) ((x) >= 0 ? floor(x) : ceil(x))
#define round(x) floor((x) + 0.5)
#endif
#include <fbit.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "MoveProcessMemory.h"
#include "FillProcessMemory.h"
#include "PageSize.h"
#include "TranscodeMultiByte.h"
#include "atoitbl.h"
#include <assert.h>
#include "ToolTip/ToolTip.h"
#include "ToolTip/commctrl.h"

#ifdef __BORLANDC__
#define USE_PLUGIN 0
#else
#define USE_PLUGIN 1
#endif

#if USE_PLUGIN
#include "Plugin.h"
#endif

#define IMPLEMENTED 0

#if defined(__BORLANDC__)
EXTERN_C uint64_t __msreturn __cdecl _strtoui64(const char *nptr, char **endptr, int base);
#pragma warn -8004
#pragma warn -8008
#pragma warn -8060
#pragma warn -8066
#ifndef PROCESS_QUERY_LIMITED_INFORMATION
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif
#define static_assert(expression, string)                                              assert(expression)
#define _ultoa                                                                         ultoa
#define string                                                                         string
#define string_c_str(s)                                                                (s)->c_str()
#define string_begin(s)                                                                (s)->begin()
#define string_end(s)                                                                  (s)->end()
#define string_length(s)                                                               (s)->length()
#define vector_begin(v)                                                                (v)->begin()
#define vector_end(v)                                                                  (v)->end()
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
#define SCOPE_SUPPORT        1
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGActionListner.h"
#include "THeapListData.h"
#include "TProcessCtrl.h"
#include "TSSGAttributeElement.h"
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
extern HANDLE pHeap;
#endif

#include "HashBytes.h"

#define atADJUST   0x0002
#define atREPLACE  0x0004
#define atVARIABLE 0x0800
#define atDEFINE   0x1000

#define OS_PUSH          0x0001
#define OS_OPEN          0x0002
#define OS_CLOSE         0x0004
#define OS_SPLIT         0x0008
#define OS_DELIMITER     0x0010
#define OS_MONADIC       0x0020
#define OS_POST          0x0040
#define OS_SHORT_CIRCUIT 0x0080
#define OS_LEFT_ASSIGN   0x0100
#define OS_PARENTHESIS   0x0200
#define OS_HAS_EXPR      0x0400
#define OS_TERNARY       0x0800
#define OS_TERNARY_END   0x1000
#define OS_LOOP_BEGIN    0x2000
#define OS_LOOP_END      0x4000
#define OS_RET_OPERAND   0x8000

/*
 [Wikipedia] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/%E6%BC%94%E7%AE%97%E5%AD%90%E3%81%AE%E5%84%AA%E5%85%88%E9%A0%86%E4%BD%8D#.E3.83.97.E3.83.AD.E3.82.B0.E3.83.A9.E3.83.9F.E3.83.B3.E3.82.B0.E8.A8.80.E8.AA.9E

 [Microsoft Developer Network] - [優先順位と評価順序] - [C 演算子の優先順位と結合規則]
 https://msdn.microsoft.com/ja-jp/library/2bxt6kc4.aspx

 [Wikipedia] - [CとC++の演算子] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/C%E3%81%A8C%2B%2B%E3%81%AE%E6%BC%94%E7%AE%97%E5%AD%90#.E6.BC.94.E7.AE.97.E5.AD.90.E3.81.AE.E5.84.AA.E5.85.88.E9.A0.86.E4.BD.8D

 127 parse_int                          OS_PUSH
 127 parse_real                         OS_PUSH
 127 parse_reset                        OS_PUSH
 127 if                                 OS_PUSH | OS_HAS_EXPR
 127 else                               OS_PUSH
 127 do                                 OS_PUSH | OS_LOOP_BEGIN
 127 while                              OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_POST)
 127 for                                OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN
 127 break                              OS_PUSH
 127 continue                           OS_PUSH
 127 printf dprintf                     OS_PUSH
     memmove
     memset memset16 memset32 memset64
     strcpy wcscpy
#if ALLOCATE_SUPPORT
 127 realloc                            OS_PUSH
#endif
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
  64 (                                  OS_OPEN | OS_PARENTHESIS
  64 [_                                 OS_OPEN
  64 [.                                 OS_OPEN
  64 [~                                 OS_OPEN
  64 [:                                 OS_OPEN
  64 ++ --                              OS_PUSH | OS_MONADIC | OS_POST 後置インクリメント 後置デクリメント
  60 MName:: HNumber::                  OS_PUSH
     ProcessId::
     Cast32:: Cast64::
     I1toI4:: I2toI4:: I4toI8::
     Memory::
     strlen:: wcslen::
     utof:: itof:: ftoi::
     trunc:: round::
     BitScanForward:: BitScanReverse::
  56 ++ -- - ! ~ *                      OS_PUSH | OS_MONADIC           前置インクリメント 前置デクリメント 単項マイナス 論理否定 ビットごとの論理否定 間接演算子
  52 * / % idiv imod                    OS_PUSH                        乗算 除算 剰余算 符号付除算 符号付剰余算
  48 + -                                OS_PUSH                        加算 減算
  44 << >> sar rol ror                  OS_PUSH                        左論理シフト 右論理シフト 右算術シフト 左ローテート 右ローテート
  40 < > <= >= lt gt le ge              OS_PUSH                        小なり 大なり 以下 以上 符号付小なり 符号付大なり 符号付以下 符号付以上
  36 == !=                              OS_PUSH                        等価 非等価
  32 &                                  OS_PUSH                        ビットごとの論理積
  28 ^                                  OS_PUSH                        ビットごとの排他的論理和
  24 |                                  OS_PUSH                        ビットごとの論理和
  20 && and                             OS_PUSH | OS_SHORT_CIRCUIT     論理積
  16 || or                              OS_PUSH | OS_SHORT_CIRCUIT     論理和
  12 ? :                                OS_PUSH | OS_TERNARY           条件演算子
   8 =>                                 OS_PUSH                        右辺代入
   8 = += -= *= /= %= &= |= ^= <<= >>=  OS_PUSH | OS_LEFT_ASSIGN       左辺代入 加算代入 減算代入 乗算代入 除算代入 剰余代入 ビット積代入 ビット排他的論理和代入 ビット和代入 左論理シフト代入 右論理シフト代入
   4 ,                                  OS_DELIMITER
   4 :] :8] :7] :6] :5] :4] :3] :2] :1] OS_PUSH | OS_CLOSE
     :I] :I8] :I7] :I6] :I5]
     :I4] :I3] :I2] :I1]
     :R] :R4] :R8]
     :F] :F4] :F8]
     :L] :L8] :L7] :L6] :L5]
     :L4] :L3] :L2] :L1]
     :LI] :LI8] :LI7] :LI6] :LI5]
     :LI4] :LI3] :LI2] :LI1]
     :LR] :LR4] :LR8]
     :LF] :LF4] :LF8]
   4 ~] ~8] ~7] ~6] ~5] ~4] ~3] ~2]     OS_PUSH | OS_CLOSE
   4 .]                                 OS_PUSH | OS_CLOSE
   4 _]                                 OS_PUSH | OS_CLOSE
   4 )                                  OS_CLOSE | OS_PARENTHESIS
   4 ;                                  OS_SPLIT
   0 return                             OS_PUSH
*/

typedef enum {
	TAG_NOT_OPERATOR     ,  // 127                  OS_PUSH
	TAG_PARSE_INT        ,  // 127 parse_int        OS_PUSH
	TAG_PARSE_REAL       ,  // 127 parse_real       OS_PUSH
	TAG_PARSE_RESET      ,  // 127 parse_reset      OS_PUSH
	TAG_IF               ,  // 127 if               OS_PUSH | OS_HAS_EXPR
	TAG_ELSE             ,  // 127 else             OS_PUSH
	TAG_DO               ,  // 127 do               OS_PUSH | OS_LOOP_BEGIN
	TAG_WHILE            ,  // 127 while            OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_POST)
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
	TAG_STRCPY           ,  // 127 strcpy           OS_PUSH
	TAG_WCSCPY           ,  // 127 wcscpy           OS_PUSH
#if ALLOCATE_SUPPORT
	TAG_REALLOC          ,  // 127 realloc          OS_PUSH
#endif
#if USE_PLUGIN
	TAG_PLUGIN           ,  // 127                  OS_PUSH
#endif
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
	TAG_PARENTHESIS_OPEN ,  //  64 (                OS_OPEN | OS_PARENTHESIS
	TAG_ADDR_ADJUST_OPEN ,  //  64 [_               OS_OPEN
	TAG_ADDR_REPLACE_OPEN,  //  64 [.               OS_OPEN
	TAG_REV_ENDIAN_OPEN  ,  //  64 [~               OS_OPEN
	TAG_REMOTE_OPEN      ,  //  64 [:               OS_OPEN
	TAG_INC              ,  //  64 N++     (52 ++N) OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_DEC              ,  //  64 N--     (52 --N) OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_MNAME            ,  //  60 MName::          OS_PUSH
	TAG_PROCEDURE        ,  //  60 ::               OS_PUSH
	TAG_IMPORT_FUNCTION  ,  //  60 :!               OS_PUSH
	TAG_IMPORT_REFERENCE ,  //  60 :&               OS_PUSH
	TAG_MODULENAME       ,  //  60                  OS_PUSH
	TAG_SECTION          ,  //  60 := :+            OS_PUSH
	TAG_HNUMBER          ,  //  60 HNumber::        OS_PUSH
	TAG_PROCESSID        ,  //  60 ProcessId::      OS_PUSH
	TAG_CAST32           ,  //  60 Cast32::         OS_PUSH
	TAG_CAST64           ,  //  60 Cast64::         OS_PUSH
	TAG_I1TOI4           ,  //  60 I1toI4::         OS_PUSH
	TAG_I2TOI4           ,  //  60 I2toI4::         OS_PUSH
	TAG_I4TOI8           ,  //  60 I4toI8::         OS_PUSH
	TAG_MEMORY           ,  //  60 Memory::         OS_PUSH
	TAG_STRLEN           ,  //  60 strlen::         OS_PUSH
	TAG_WCSLEN           ,  //  60 wcslen::         OS_PUSH
	TAG_UTOF             ,  //  60 utof::           OS_PUSH
	TAG_ITOF             ,  //  60 itof::           OS_PUSH
	TAG_FTOI             ,  //  60 ftoi::           OS_PUSH
	TAG_TRUNC            ,  //  60 trunc::          OS_PUSH
	TAG_ROUND            ,  //  60 round::          OS_PUSH
	TAG_BSF              ,  //  60 BitScanForward:: OS_PUSH
	TAG_BSR              ,  //  60 BitScanReverse:: OS_PUSH
	TAG_NEG              ,  //  56 -                OS_PUSH | OS_MONADIC
	TAG_NOT              ,  //  56 !                OS_PUSH | OS_MONADIC
	TAG_BIT_NOT          ,  //  56 ~                OS_PUSH | OS_MONADIC
	TAG_INDIRECTION      ,  //  56 *                OS_PUSH | OS_MONADIC
	TAG_ADDRESS_OF       ,  //  56 &                OS_PUSH | OS_MONADIC
	TAG_MUL              ,  //  52 *        (8 *= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_DIV              ,  //  52 /        (8 /= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_MOD              ,  //  52 %        (8 %= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_IDIV             ,  //  52 idiv             OS_PUSH
	TAG_IMOD             ,  //  52 imod             OS_PUSH
	TAG_ADD              ,  //  48 +        (8 += ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SUB              ,  //  48 -        (8 -= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHL              ,  //  44 <<       (8 <<=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHR              ,  //  44 >>       (8 >>=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SAR              ,  //  44 sar              OS_PUSH
	TAG_ROL              ,  //  44 rol              OS_PUSH
	TAG_ROR              ,  //  44 ror              OS_PUSH
	TAG_BT               ,  //  40 <                OS_PUSH
	TAG_AT               ,  //  40 >                OS_PUSH
	TAG_BE               ,  //  40 <=               OS_PUSH
	TAG_AE               ,  //  40 >=               OS_PUSH
	TAG_LT               ,  //  40 lt               OS_PUSH
	TAG_GT               ,  //  40 gt               OS_PUSH
	TAG_LE               ,  //  40 le               OS_PUSH
	TAG_GE               ,  //  40 ge               OS_PUSH
	TAG_EQ               ,  //  36 ==               OS_PUSH
	TAG_NE               ,  //  36 !=               OS_PUSH
	TAG_BIT_AND          ,  //  32 &        (8 &= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_XOR              ,  //  28 ^        (8 ^= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_BIT_OR           ,  //  24 |        (8 |= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_AND              ,  //  20 &&      (20 and) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_OR               ,  //  16 ||      (16 or ) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_TERNARY          ,  //  12 ?                OS_PUSH | OS_TERNARY
	TAG_TERNARY_SPLIT    ,  //  12 :                OS_PUSH | OS_TERNARY
	TAG_RIGHT_ASSIGN     ,  //   8 =>               OS_PUSH
	TAG_LEFT_ASSIGN      ,  //   8 =                OS_PUSH | OS_LEFT_ASSIGN
	TAG_DELIMITER        ,  //   4 ,                OS_DELIMITER
	TAG_REMOTE1          ,  //   4 :1]              OS_PUSH | OS_CLOSE
	TAG_REMOTE2          ,  //   4 :2]              OS_PUSH | OS_CLOSE
	TAG_REMOTE3          ,  //   4 :3]              OS_PUSH | OS_CLOSE
	TAG_REMOTE4          ,  //   4 :] :4]           OS_PUSH | OS_CLOSE
	TAG_REMOTE5          ,  //   4 :5]              OS_PUSH | OS_CLOSE
	TAG_REMOTE6          ,  //   4 :6]              OS_PUSH | OS_CLOSE
	TAG_REMOTE7          ,  //   4 :7]              OS_PUSH | OS_CLOSE
	TAG_REMOTE8          ,  //   4 :8]              OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER1  ,  //   4 :I1]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER2  ,  //   4 :I2]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER3  ,  //   4 :I3]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER4  ,  //   4 :I] :I4]         OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER5  ,  //   4 :I5]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER6  ,  //   4 :I6]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER7  ,  //   4 :I7]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER8  ,  //   4 :I8] :F8]        OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL4     ,  //   4 :R] :R4]         OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL8     ,  //   4 :R8]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_FLOAT4    ,  //   4 :F] :F4]         OS_PUSH | OS_CLOSE
	TAG_LOCAL1           ,  //   4 :L1]             OS_PUSH | OS_CLOSE
	TAG_LOCAL2           ,  //   4 :L2]             OS_PUSH | OS_CLOSE
	TAG_LOCAL3           ,  //   4 :L3]             OS_PUSH | OS_CLOSE
	TAG_LOCAL4           ,  //   4 :L] :L4]         OS_PUSH | OS_CLOSE
	TAG_LOCAL5           ,  //   4 :L5]             OS_PUSH | OS_CLOSE
	TAG_LOCAL6           ,  //   4 :L6]             OS_PUSH | OS_CLOSE
	TAG_LOCAL7           ,  //   4 :L7]             OS_PUSH | OS_CLOSE
	TAG_LOCAL8           ,  //   4 :L8] :LF8]       OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER1   ,  //   4 :LI1]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER2   ,  //   4 :LI2]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER3   ,  //   4 :LI3]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER4   ,  //   4 :LI] :LI4]       OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER5   ,  //   4 :LI5]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER6   ,  //   4 :LI6]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER7   ,  //   4 :LI7]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER8   ,  //   4 :LI8]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL4      ,  //   4 :LR] :LR4]       OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL8      ,  //   4 :LR8]            OS_PUSH | OS_CLOSE
	TAG_LOCAL_FLOAT4     ,  //   4 :LF] :LF4]       OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN2      ,  //   4 ~2]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN3      ,  //   4 ~3]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN4      ,  //   4 ~] ~4]           OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN5      ,  //   4 ~5]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN6      ,  //   4 ~6]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN7      ,  //   4 ~7]              OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN8      ,  //   4 ~8]              OS_PUSH | OS_CLOSE
	TAG_ADDR_REPLACE     ,  //   4 .]               OS_PUSH | OS_CLOSE
	TAG_ADDR_ADJUST      ,  //   4 _]               OS_PUSH | OS_CLOSE
	TAG_IF_EXPR          ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_WHILE_EXPR       ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_FOR_INITIALIZE   ,  //   4 ;                OS_PUSH | OS_SPLIT
	TAG_FOR_CONDITION    ,  //   4 ;                OS_PUSH | OS_SPLIT | OS_LOOP_BEGIN
	TAG_FOR_UPDATE       ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_PRINTF_END       ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_DPRINTF_END      ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_MEMMOVE_END      ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET_END       ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET16_END     ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET32_END     ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_MEMSET64_END     ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_STRCPY_END       ,  //   4 )                OS_PUSH | OS_CLOSE
	TAG_WCSCPY_END       ,  //   4 )                OS_PUSH | OS_CLOSE
#if ALLOCATE_SUPPORT
	TAG_REALLOC_END      ,  //   4 )                OS_PUSH | OS_CLOSE
#endif
#if USE_PLUGIN
	TAG_PLUGIN_END       ,  //   4 )                OS_PUSH | OS_CLOSE
#endif
	TAG_PARENTHESIS_CLOSE,  //   4 )                OS_CLOSE | OS_PARENTHESIS
	TAG_SPLIT            ,  //   4 ;                OS_SPLIT
	TAG_RETURN           ,  //   0 return           OS_PUSH
	TAG_PARSE_ERROR      ,
} TAG;

typedef enum {
	PRIORITY_NOT_OPERATOR      = 127,   //                  OS_PUSH
	PRIORITY_PARSE_INT         = 127,   // parse_int        OS_PUSH
	PRIORITY_PARSE_REAL        = 127,   // parse_real       OS_PUSH
	PRIORITY_PARSE_RESET       = 127,   // parse_reset      OS_PUSH
	PRIORITY_IF                = 127,   // if               OS_PUSH | OS_HAS_EXPR
	PRIORITY_ELSE              = 127,   // else             OS_PUSH
	PRIORITY_DO                = 127,   // do               OS_PUSH
	PRIORITY_WHILE             = 127,   // while            OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_POST)
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
	PRIORITY_STRCPY            = 127,   // strcpy           OS_PUSH
	PRIORITY_WCSCPY            = 127,   // wcscpy           OS_PUSH
#if ALLOCATE_SUPPORT
	PRIORITY_REALLOC           = 127,   // realloc          OS_PUSH
#endif
#if USE_PLUGIN
	PRIORITY_PLUGIN            = 127,   //                  OS_PUSH
#endif
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
	PRIORITY_PARENTHESIS_OPEN  =  64,   // (                OS_OPEN | OS_PARENTHESIS
	PRIORITY_ADDR_ADJUST_OPEN  =  64,   // [_               OS_OPEN
	PRIORITY_ADDR_REPLACE_OPEN =  64,   // [.               OS_OPEN
	PRIORITY_REV_ENDIAN_OPEN   =  64,   // [~               OS_OPEN
	PRIORITY_REMOTE_OPEN       =  64,   // [:               OS_OPEN
	PRIORITY_POST_INC_DEC      =  64,   // N++, N--         OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_FUNCTION          =  60,   // MName::          OS_PUSH
	                                    // ::               OS_PUSH
	                                    // :!               OS_PUSH
	                                    // :&               OS_PUSH
	                                    // := :+            OS_PUSH
	                                    // HNumber::        OS_PUSH
	                                    // ProcessId::      OS_PUSH
	                                    // Cast32::         OS_PUSH
	                                    // Cast64::         OS_PUSH
	                                    // I1toI4::         OS_PUSH
	                                    // I2toI4::         OS_PUSH
	                                    // I4toI8::         OS_PUSH
	                                    // Memory::         OS_PUSH
	                                    // strlen::         OS_PUSH
	                                    // wcslen::         OS_PUSH
	                                    // utof::           OS_PUSH
	                                    // itof::           OS_PUSH
	                                    // ftoi::           OS_PUSH
	                                    // trunc::          OS_PUSH
	                                    // round::          OS_PUSH
	                                    // BitScanForward:: OS_PUSH
	                                    // BitScanReverse:: OS_PUSH
	PRIORITY_NEG               =  56,   // -                OS_PUSH | OS_MONADIC
	PRIORITY_NOT               =  56,   // !                OS_PUSH | OS_MONADIC
	PRIORITY_BIT_NOT           =  56,   // ~                OS_PUSH | OS_MONADIC
	PRIORITY_INDIRECTION       =  56,   // *                OS_PUSH | OS_MONADIC
	PRIORITY_ADDRESS_OF        =  56,   // &                OS_PUSH | OS_MONADIC
	PRIORITY_PRE_INC           =  56,   // ++N              OS_PUSH | OS_MONADIC
	PRIORITY_PRE_DEC           =  56,   // --N              OS_PUSH | OS_MONADIC
	PRIORITY_MUL               =  52,   // *        (8 *= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_DIV               =  52,   // /        (8 /= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_MOD               =  52,   // %        (8 %= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_IDIV              =  52,   // idiv             OS_PUSH
	PRIORITY_IMOD              =  52,   // imod             OS_PUSH
	PRIORITY_ADD               =  48,   // +        (8 += ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SUB               =  48,   // -        (8 -= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHL               =  44,   // <<       (8 <<=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHR               =  44,   // >>       (8 >>=) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SAR               =  44,   // sar              OS_PUSH
	PRIORITY_ROL               =  44,   // rol              OS_PUSH
	PRIORITY_ROR               =  44,   // ror              OS_PUSH
	PRIORITY_BT                =  40,   // <                OS_PUSH
	PRIORITY_AT                =  40,   // >                OS_PUSH
	PRIORITY_BE                =  40,   // <=               OS_PUSH
	PRIORITY_AE                =  40,   // >=               OS_PUSH
	PRIORITY_LT                =  40,   // lt               OS_PUSH
	PRIORITY_GT                =  40,   // gt               OS_PUSH
	PRIORITY_LE                =  40,   // le               OS_PUSH
	PRIORITY_GE                =  40,   // ge               OS_PUSH
	PRIORITY_EQ                =  36,   // ==               OS_PUSH
	PRIORITY_NE                =  36,   // !=               OS_PUSH
	PRIORITY_BIT_AND           =  32,   // &        (8 &= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_XOR               =  28,   // ^        (8 ^= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_BIT_OR            =  24,   // |        (8 |= ) OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_AND               =  20,   // &&      (20 and) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_OR                =  16,   // ||      (16 or ) OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_TERNARY           =  12,   // ? :              OS_PUSH | OS_TERNARY
	PRIORITY_RIGHT_ASSIGN      =   8,   // =>               OS_PUSH
	PRIORITY_LEFT_ASSIGN       =   8,   // =                OS_PUSH | OS_LEFT_ASSIGN
	PRIORITY_DELIMITER         =   4,   // ,                OS_DELIMITER
	PRIORITY_READ_WRITE        =   4,   // :]  :1] :2] :3]  OS_PUSH | OS_CLOSE
	                                    // :4] :5] :6] :7]
	                                    // :8]
	                                    // :I]  :I1] :I2]
	                                    // :I3] :I4] :I5]
	                                    // :I6] :I7] :I8]
	                                    // :R]  :R4] :R8]
	                                    // :F]  :F4] :F8]
	                                    // :L]  :L1] :L2]
	                                    // :L3] :L4] :L5]
	                                    // :L6] :L7] :L8]
	                                    // :LI]  :LI1]
	                                    // :LI2] :LI3]
	                                    // :LI4] :LI5]
	                                    // :LI6] :LI7]
	                                    // :LI8]
	                                    // :LR]
	                                    // :LR4] :LR8]
	                                    // :LF]
	                                    // :LF4] :LF8]
	PRIORITY_REV_ENDIAN        =   4,   // ~]  ~2] ~3] ~4]  OS_PUSH | OS_CLOSE
	                                    // ~5] ~6] ~7] ~8]
	PRIORITY_ADDR_REPLACE      =   4,   // .]               OS_PUSH | OS_CLOSE
	PRIORITY_ADDR_ADJUST       =   4,   // _]               OS_PUSH | OS_CLOSE
	PRIORITY_PARENTHESIS_CLOSE =   4,   // )                OS_CLOSE | OS_PARENTHESIS
	PRIORITY_SPLIT             =   4,   // ;                OS_SPLIT
	PRIORITY_RETURN            =   0,   // return           OS_PUSH
} PRIORITY;

typedef struct {
	TAG             Tag;
	size_t          Length;
	LPSTR           String;
	BYTE            Priority;
	WORD            Type;
	size_t          Depth;
	size_t          LoopDepth;
#if USE_PLUGIN
	PLUGIN_FUNCTION *Function;
#endif
} MARKUP, *PMARKUP;

typedef struct {
	union {
		struct {
			DWORD Low;
			DWORD High;
		};
		uint64_t  Quad;
		double    Real;
	};
	BOOL IsQuad;
} VARIABLE, *PVARIABLE;

typedef struct {
	size_t   Length;
	LPCSTR   String;
	VARIABLE Value;
#if SCOPE_SUPPORT
#if !defined(__BORLANDC__)
	map_iterator It;
#else
	map<unsigned long, pair<unsigned long, unsigned long> >::iterator It;
#endif
#endif
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
FILETIME           ftProcessCreationTime = { 0, 0 };

#if USE_PLUGIN
BOOL ParsingContinue;
#endif

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
#ifndef _M_IX86
static size_t __fastcall TrimMarkupString(MARKUP *lpMarkup)
{
	if (lpMarkup->Length)
	{
		char *begin, *end, c;

		// it do not checking multibyte,
		// because space is not the lead and trail byte of codepage 932.

		// load from memory
		begin = lpMarkup->String;
		end = begin + lpMarkup->Length;

		// trim left
		do
			c = *(begin++);
		while (__intrinsic_isspace(c));
		begin--;

		// trim right
		while (end > begin)
		{
			c = *(end - 1);
			end--;
			if (__intrinsic_isspace(c))
				continue;
			end++;
			break;
		}

		// store to memory
		lpMarkup->String = begin;
		lpMarkup->Length = end - begin;
	}
	return lpMarkup->Length;
}
#else
__declspec(naked) static size_t __fastcall TrimMarkupString(MARKUP *lpMarkup)
{
	#define offsetof_MARKUP_Length 4
	#define offsetof_MARKUP_String 8

	__asm
	{
		#define lpMarkup ecx

		mov     eax, dword ptr [ecx + offsetof_MARKUP_Length]
		mov     edx, dword ptr [ecx + offsetof_MARKUP_String]
		test    eax, eax
		jz      L5
		push    ecx
		add     eax, edx
	L1:
		mov     cl, byte ptr [edx]
		inc     edx
		cmp     cl, ' '
		je      L1
		cmp     cl, '\r'
		ja      L2
		cmp     cl, '\t'
		jae     L1
	L2:
		cmp     eax, edx
		jb      L4
		mov     cl, byte ptr [eax - 1]
		dec     eax
		cmp     cl, ' '
		je      L2
		cmp     cl, '\r'
		ja      L3
		cmp     cl, '\t'
		jae     L2
	L3:
		inc     eax
	L4:
		dec     edx
		pop     ecx
		sub     eax, edx
		mov     dword ptr [ecx + offsetof_MARKUP_Length], eax
		mov     dword ptr [ecx + offsetof_MARKUP_String], edx
	L5:
		ret

		#undef lpMarkup
	}

	#undef offsetof_MARKUP_Length
	#undef offsetof_MARKUP_String
}
#endif
//---------------------------------------------------------------------
static MARKUP * __fastcall FindParenthesisClose(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	size_t nDepth;

	nDepth = 1;
	while (++lpMarkup < lpEndOfMarkup)
		if (lpMarkup->Type & OS_PARENTHESIS)
			if (lpMarkup->Type & OS_OPEN)
				nDepth++;
			else if (!--nDepth)
				break;
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindDelimiter(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	size_t nDepth;

	nDepth = 0;
	while (++lpMarkup < lpEndOfMarkup)
		if (lpMarkup->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
			if (lpMarkup->Type & (OS_OPEN | OS_CLOSE))
				if (lpMarkup->Type & OS_OPEN)
					nDepth++;
				else if (nDepth)
					nDepth--;
				else
					return (MARKUP *)lpEndOfMarkup;
			else if (!nDepth)
				break;
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindSplit(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	size_t nDepth;
	size_t nSplit;

	nSplit = nDepth = 0;
	do
	{
		if (lpMarkup->Type & OS_SPLIT)
		{
			if (!nDepth && !nSplit--)
				break;
		}
		else if (lpMarkup->Type & (OS_OPEN | OS_CLOSE))
		{
			if (lpMarkup->Type & OS_OPEN)
				nDepth++;
			else if (nDepth)
				nDepth--;
		}
		else if (lpMarkup->Tag == TAG_DO)
		{
			if (lpMarkup + 1 < lpEndOfMarkup && lpMarkup[1].Tag != TAG_PARENTHESIS_OPEN)
				nSplit++;
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
				if (lpMarkup + 1 >= lpEndOfMarkup || lpMarkup[1].Tag != TAG_ELSE)
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
static MARKUP * __stdcall Markup(IN LPSTR lpSrc, IN size_t nSrcLength, OUT size_t *lpnNumberOfMarkup)
{
	MARKUP  *lpTagArray, *lpEndOfTag;
	size_t  nNumberOfTag;
	BOOLEAN bIsSeparatedLeft, bNextIsSeparatedLeft;
	LPBYTE  p, end;
	MARKUP  *lpMarkupArray;
	MARKUP  *lpMarkup, *lpEndOfMarkup;
	size_t  nFirstTernary;
	BOOL    bCorrectTag;
	size_t  nDepth;

	assert(lpSrc != NULL);
	assert(!__intrinsic_isspace(*lpSrc));

	// check parameters
	if (!nSrcLength)
		return NULL;

	// mark up the tags and operators
	lpTagArray = AllocMarkup();
	if (!lpTagArray)
		return NULL;

	nNumberOfTag = 0;
	nFirstTernary = SIZE_MAX;
	bCorrectTag = FALSE;
	bIsSeparatedLeft = TRUE;
	for (p = lpSrc, end = lpSrc + nSrcLength; p < end; bIsSeparatedLeft = bNextIsSeparatedLeft)
	{
		TAG    iTag;
		size_t nLength;
		BYTE   bPriority;

#if USE_PLUGIN
		#define SET_FUNCTION(markup, function) (markup)->Function = function
#else
		#define SET_FUNCTION(markup, function)
#endif
		#define APPEND_TAG(tag, length, priority, type)                     \
		do                                                                  \
		{                                                                   \
		    if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))    \
		        goto FAILED;                                                \
		    lpMarkup->Tag      = tag;                                       \
		    lpMarkup->Length   = length;                                    \
		    lpMarkup->String   = p;                                         \
		    lpMarkup->Priority = priority;                                  \
		    lpMarkup->Type     = type;                                      \
		    lpMarkup->Depth    = 0;                                         \
		    SET_FUNCTION(lpMarkup, NULL);                                   \
		} while (0)

		#define APPEND_TAG_WITH_CONTINUE(tag, length, priority, type)       \
		do                                                                  \
		{                                                                   \
		    APPEND_TAG(tag, length, priority, type);                        \
		    p += length;                                                    \
		    goto CONTINUE;                                                  \
		} while (0)

		/*
		#define IS_SEPARATED_LEFT(p) (                                      \
		    __intrinsic_isascii((p)[0]) &&                                  \
		    !__intrinsic_isalnum((p)[0]) &&                                 \
		    ((p)[0] != '_' || ((p) != lpSrc && (p)[-1] == '[')) &&          \
		    (p)[0] != '$')
		*/

		#define IS_SEPARATED_RIGHT(p) (                                     \
		    __intrinsic_isascii((p)[0]) &&                                  \
		    !__intrinsic_isalnum((p)[0]) &&                                 \
		    ((p)[0] != '_' || (p)[1] == ']'))

		bNextIsSeparatedLeft = FALSE;
		switch (*p)
		{
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
		case ' ':
		case '#':
		case '\'':
		case '@':
		case '\\':
		case '`':
			bNextIsSeparatedLeft = TRUE;
			break;
		case '!':
			// "!", "!="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] == '=')
				APPEND_TAG_WITH_CONTINUE(TAG_NE, 2, PRIORITY_NE, OS_PUSH);
			else if (!nNumberOfTag || (lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_FUNCTION && lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_REFERENCE))
				APPEND_TAG_WITH_CONTINUE(TAG_NOT, 1, PRIORITY_NOT, OS_PUSH | OS_MONADIC);
			if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
				goto FAILED;
			lpMarkup->Tag      = TAG_MODULENAME;
			lpMarkup->Length   = p - lpTagArray[nNumberOfTag - 2].String - 1;
			lpMarkup->String   = lpTagArray[nNumberOfTag - 2].String + 2;
			lpMarkup->Priority = PRIORITY_FUNCTION;
			lpMarkup->Type     = OS_PUSH;
			break;
		case '"':
		DOUBLE_QUOTED_STRING:
			// double-quoted string
			bNextIsSeparatedLeft = TRUE;
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
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 1, PRIORITY_MOD, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '&':
			// "&", "&&", "&="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_OPEN, 1, PRIORITY_PARENTHESIS_OPEN, OS_OPEN | OS_PARENTHESIS);
		case ')':
			// ")"
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_CLOSE, 1, PRIORITY_PARENTHESIS_CLOSE, OS_CLOSE | OS_PARENTHESIS);
		case '*':
			// "*", "*="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 1, PRIORITY_MUL, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '+':
			// "+", "++", "+="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_DELIMITER, 1, PRIORITY_DELIMITER, OS_DELIMITER);
		case '-':
			// "-", "--", "-="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != ']')
				break;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE, 2, PRIORITY_ADDR_REPLACE, OS_PUSH | OS_CLOSE);
		case '/':
			// "/", "/="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 1, PRIORITY_DIV, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case ':':
			// ":", "::", ":!", ":&", ":=", ":+"
			// ":]", ":1]", ":2]", ":3]", ":4]", ":5]", ":6]", ":7]", ":8]"
			// ":I]", ":I1]", ":I2]", ":I3]", ":I4]", ":I5]", ":I6]", ":I7]", ":I8]"
			// ":R]", ":R4]", ":R8]"
			// ":F]", ":F4]", ":F8]"
			// ":L]", ":L1]", ":L2]", ":L3]", ":L4]", ":L5]", ":L6]", ":L7]", ":L8]"
			// ":LI]", ":LI1]", ":LI2]", ":LI3]", ":LI4]", ":LI5]", ":LI6]", ":LI7]", ":LI8]"
			// ":LR]", ":LR4]", ":LR8]"
			// ":LF]", ":LF4]", ":LF8]"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '!':
				APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_FUNCTION, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '&':
				APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_REFERENCE, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '+':
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_SECTION, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				if (p[2] != ']')
					break;
				iTag = (TAG)(TAG_REMOTE1 + p[1] - '1');
				nLength = 3;
				goto APPEND_READ_WRITE;
			case ':':
				APPEND_TAG_WITH_CONTINUE(TAG_PROCEDURE, 2, PRIORITY_FUNCTION, OS_PUSH);
			case 'F':
				switch (p[2])
				{
				case '4':
					iTag = TAG_REMOTE_FLOAT4;
					goto REMOTE_FLOAT;
				case '8':
					iTag = TAG_REMOTE8;
				REMOTE_FLOAT:
					if (p[3] != ']')
						break;
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_FLOAT4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'I':
				switch (p[2])
				{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (p[3] != ']')
						break;
					iTag = (TAG)(TAG_REMOTE_INTEGER1 + p[2] - '1');
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_INTEGER4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'L':
				switch (p[2])
				{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (p[3] != ']')
						break;
					iTag = (TAG)(TAG_LOCAL1 + p[2] - '1');
					nLength = 4;
					goto APPEND_READ_WRITE;
				case 'F':
					switch (p[3])
					{
					case '4':
						iTag = TAG_LOCAL_FLOAT4;
						goto LOCAL_FLOAT;
					case '8':
						iTag = TAG_LOCAL8;
					LOCAL_FLOAT:
						if (p[4] != ']')
							break;
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_FLOAT4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case 'I':
					switch (p[3])
					{
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
						if (p[4] != ']')
							break;
						iTag = (TAG)(TAG_LOCAL_INTEGER1 + p[2] - '1');
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_INTEGER4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case 'R':
					switch (p[3])
					{
					case '4':
						iTag = TAG_LOCAL_REAL4;
						goto LOCAL_REAL;
					case '8':
						iTag = TAG_LOCAL_REAL8;
					LOCAL_REAL:
						if (p[4] != ']')
							break;
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_REAL4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case ']':
					iTag = TAG_LOCAL4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'R':
				switch (p[2])
				{
				case '4':
					iTag = TAG_REMOTE_REAL4;
					goto REMOTE_REAL;
				case '8':
					iTag = TAG_REMOTE_REAL8;
				REMOTE_REAL:
					if (p[3] != ']')
						break;
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_REAL4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case ']':
				iTag = TAG_REMOTE4;
				nLength = 2;
			APPEND_READ_WRITE:
				APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_READ_WRITE, OS_PUSH | OS_CLOSE);
			}
			APPEND_TAG_WITH_CONTINUE(TAG_TERNARY_SPLIT, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARY);
		case ';':
			// ";"
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_SPLIT, 1, PRIORITY_SPLIT, OS_SPLIT);
		case '<':
			// "<", "<<", "<<=", "<="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '<':
				if (p[2] != '=')
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
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '>':
				if (p[2] != '=')
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
			bNextIsSeparatedLeft = TRUE;
			if (nFirstTernary == SIZE_MAX)
				nFirstTernary = nNumberOfTag;
			APPEND_TAG_WITH_CONTINUE(TAG_TERNARY, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARY);
		case 'B':
			// "BitScanForward::", "BitScanReverse::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('BitS'))
				break;
			if (*(uint32_t *)(p + 4) == BSWAP32('canF'))
			{
				if (*(uint32_t *)(p + 8) != BSWAP32('orwa'))
					break;
				if (*(uint32_t *)(p + 12) != BSWAP32('rd::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_BSF, 16, PRIORITY_FUNCTION, OS_PUSH);
			}
			else if (*(uint32_t *)(p + 4) == BSWAP32('canR'))
			{
				if (*(uint32_t *)(p + 8) != BSWAP32('ever'))
					break;
				if (*(uint32_t *)(p + 12) != BSWAP32('se::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_BSR, 16, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'C':
			// "Cast32::", "Cast64::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('Cast'))
				break;
			if (*(uint32_t *)(p + 4) == BSWAP32('32::'))
			{
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_CAST32, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			else if (*(uint32_t *)(p + 4) == BSWAP32('64::'))
			{
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_CAST64, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'H':
			// "HNumber::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('Numb'))
				break;
			if (*(uint32_t *)(p + 5) != BSWAP32('er::'))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_HNUMBER, 9, PRIORITY_FUNCTION, OS_PUSH);
		case 'I':
			// "I1toI4::", "I2toI4::", "I4toI8::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p == BSWAP32('I1to'))
			{
				if (*(uint32_t *)(p + 4) != BSWAP32('I4::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_I1TOI4, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			else if (*(uint32_t *)p == BSWAP32('I2to'))
			{
				if (*(uint32_t *)(p + 4) != BSWAP32('I4::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_I2TOI4, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			else if (*(uint32_t *)p == BSWAP32('I4to'))
			{
				if (*(uint32_t *)(p + 4) != BSWAP32('I8::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_I4TOI8, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'L':
			// "L"
			if (bIsSeparatedLeft && nNumberOfTag > 1)
			{
				MARKUP *lpPrev;

				lpPrev = lpTagArray + nNumberOfTag - 1;
				if (lpPrev->Tag != TAG_PARENTHESIS_OPEN && lpPrev->Tag != TAG_DELIMITER)
					break;
				if (p[-1] != ',' && p[-1] != '(')
				{
					BYTE *first, c;

					if (!__intrinsic_isspace(p[-1]))
						break;
					first = lpPrev->String;
					do
						c = *(++first);
					while (__intrinsic_isspace(c));
					if (first != p)
						break;
				}
				if (!__intrinsic_isspace(p[1]) &&
					p[1] != '$' &&
					p[1] != '(' &&
					p[1] != '@' &&
					p[1] != '[' &&
					p[1] != '{' &&
					p[1] != '~' &&
					(p[1] != '!' || p[2] == '='))
					break;
				if (lpPrev->Tag == TAG_DELIMITER)
				{
					size_t nDepth;

					nDepth = 1;
					while (--lpPrev != lpTagArray)
						if (lpPrev->Type & (OS_OPEN | OS_CLOSE))
							if (lpPrev->Type & OS_CLOSE)
								nDepth++;
							else if (!--nDepth)
								break;
					if (lpPrev == lpTagArray)
						break;
				}
				switch ((lpPrev - 1)->Tag)
				{
				case TAG_MEMMOVE:
				case TAG_MEMSET:
				case TAG_MEMSET16:
				case TAG_MEMSET32:
				case TAG_MEMSET64:
				case TAG_STRCPY:
				case TAG_WCSCPY:
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARAM_LOCAL, 1, PRIORITY_PARAM_LOCAL, OS_PUSH);
				}
			}
			break;
		case 'M':
			// "MName::", "Memory::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) == BSWAP32('Name'))
			{
				if (*(uint16_t *)(p + 5) != BSWAP16('::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_MNAME, 7, PRIORITY_FUNCTION, OS_PUSH);
			}
			else if (*(uint32_t *)(p + 1) == BSWAP32('emor'))
			{
				if (*(uint32_t *)(p + 4) != BSWAP32('ry::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_MEMORY, 8, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'P':
			// "ProcessId::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('roce'))
				break;
			if (*(uint32_t *)(p + 5) != BSWAP32('ssId'))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_PROCESSID, 11, PRIORITY_FUNCTION, OS_PUSH);
		case '[':
			// "[_", "[.", "[~", "[:"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 1, PRIORITY_XOR, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '_':
			// "_]"
			if (p[1] != ']')
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST, 2, PRIORITY_ADDR_ADJUST, OS_PUSH | OS_CLOSE);
		case 'a':
			// "and"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint16_t *)(p + 1) != BSWAP16('nd'))
				break;
			iTag = TAG_AND;
			nLength = 3;
			bPriority = PRIORITY_AND;
			goto APPEND_RET_OPERAND_OPERATOR;
		case 'b':
			// "break"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('reak'))
				break;
			if (!__intrinsic_isspace(p[5]) && p[5] != ';')
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_BREAK, 5, PRIORITY_BREAK, OS_PUSH);
		case 'c':
			// "continue"
			// not implemented: "call", "case", "co_await", "co_return", "co_yield"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
#if IMPLEMENTED
			case 'a':
				switch (p[2])
				{
				case 'l':
					if (p[3] != 'l')
						break;
					if (p[4] != '(' && !__intrinsic_isspace(p[4]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CALL, 4, PRIORITY_CALL, OS_PUSH);
				case 's':
					if (p[3] != 'e')
						break;
					if (p[4] != ':' && !__intrinsic_isspace(p[4]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CASE, 4, PRIORITY_CASE, OS_PUSH);
				}
				break;
#endif
			case 'o':
				switch (p[2])
				{
#if IMPLEMENTED
				case '_':
					switch (p[3])
					{
					case 'a':
						if (*(uint32_t *)(p + 4) != BSWAP32('wait'))
							break;
						if (!__intrinsic_isspace(p[8]) && p[8] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_AWAIT, 8, PRIORITY_CO_AWAIT, OS_PUSH);
					case 'r':
						if (*(uint32_t *)(p + 4) != BSWAP32('etur'))
							break;
						if (p[8] != 'n')
							break;
						if (!__intrinsic_isspace(p[9]) && p[9] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_RETURN, 9, PRIORITY_CO_RETURN, OS_PUSH);
					case 'y':
						if (*(uint32_t *)(p + 4) != BSWAP32('ield'))
							break;
						if (!__intrinsic_isspace(p[8]) && p[8] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_YIELD, 8, PRIORITY_CO_YIELD, OS_PUSH);
					}
					break;
#endif
				case 'n':
					if (*(uint32_t *)(p + 3) != BSWAP32('tinu'))
						break;
					if (p[7] != 'e')
						break;
					if (!__intrinsic_isspace(p[8]) && p[8] != ';')
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CONTINUE, 8, PRIORITY_CONTINUE, OS_PUSH);
				}
				break;
			}
			break;
		case 'd':
			// "do", "dprintf"
			// not implemented: "default"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
#if IMPLEMENTED
			case 'e':
				if (*(uint32_t *)(p + 2) != BSWAP32('faul'))
					break;
				if (p[6] != 't')
					break;
				if (p[7] != ':' && !__intrinsic_isspace(p[7]))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_DEFAULT, 7, PRIORITY_DEFAULT, OS_PUSH);
#endif
			case 'o':
				if (p[2] != '(' && !__intrinsic_isspace(p[2]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_DO, 2, PRIORITY_DO, OS_PUSH | OS_LOOP_BEGIN);
			case 'p':
				if (*(uint32_t *)(p + 2) != BSWAP32('rint'))
					break;
				if (p[6] != 'f')
					break;
				if (p[7] != '(' && !__intrinsic_isspace(p[7]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_DPRINTF, 7, PRIORITY_DPRINTF, OS_PUSH);
			}
			break;
		case 'e':
			// "else"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('else'))
				break;
			if (p[4] != '(' && !__intrinsic_isspace(p[4]) && p[4] != ';')
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_ELSE, 4, PRIORITY_ELSE, OS_PUSH);
		case 'f':
			// "for", "ftoi::"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'o':
				if (p[2] != 'r')
					break;
				if (p[3] != '(' && !__intrinsic_isspace(p[3]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_FOR, 3, PRIORITY_FOR, OS_PUSH | OS_HAS_EXPR);
			case 't':
				if (*(uint32_t *)(p + 2) != BSWAP32('oi::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_FTOI, 6, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'g':
			// "gt", "ge"
			// not implemented: "goto"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
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
				if (*(uint16_t *)(p + 2) != BSWAP16('to'))
					break;
				if (p[4] != '(' && !__intrinsic_isspace(p[4]))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_GOTO, 4, PRIORITY_GOTO, OS_PUSH | OS_HAS_EXPR);
#endif
			}
			break;
		case 'i':
			// "idiv", "imod", "if", "itof::"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'd':
				if (*(uint16_t *)(p + 2) != BSWAP16('iv'))
					break;
				iTag = TAG_IDIV;
				nLength = 4;
				bPriority = PRIORITY_IDIV;
				goto APPEND_WORD_OPERATOR;
			case 'm':
				if (*(uint16_t *)(p + 2) != BSWAP16('od'))
					break;
				iTag = TAG_IMOD;
				nLength = 4;
				bPriority = PRIORITY_IMOD;
				goto APPEND_WORD_OPERATOR;
			case 'f':
				if (p[2] != '(' && !__intrinsic_isspace(p[2]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_IF, 2, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
			case 't':
				if (*(uint32_t *)(p + 2) != BSWAP32('of::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_ITOF, 6, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'l':
			// "lt", "le"
			if (!bIsSeparatedLeft)
				break;
			if (p[1] == 't')
			{
				iTag = TAG_LT;
				nLength = 2;
				bPriority = PRIORITY_LT;
				goto APPEND_WORD_OPERATOR;
			}
			else if (p[1] == 'e')
			{
				iTag = TAG_LE;
				nLength = 2;
				bPriority = PRIORITY_LE;
				goto APPEND_WORD_OPERATOR;
			}
			break;
		case 'm':
			// "memmove", "memset", "memset16", "memset32", "memset64"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint16_t *)(p + 1) != BSWAP16('em'))
				break;
			if (*(uint16_t *)(p + 3) == BSWAP16('mo'))
			{
				if (*(uint16_t *)(p + 5) != BSWAP16('ve'))
					break;
				if (p[7] != '(' && !__intrinsic_isspace(p[7]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_MEMMOVE, 7, PRIORITY_MEMMOVE, OS_PUSH);
			}
			else if (*(uint16_t *)(p + 3) == BSWAP16('se'))
			{
				if (p[5] != 't')
					break;
				if (*(uint16_t *)(p + 6) == BSWAP16('16'))
				{
					if (p[8] != '(' && !__intrinsic_isspace(p[8]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_MEMSET16, 8, PRIORITY_MEMSET16, OS_PUSH);
				}
				else if (*(uint16_t *)(p + 6) == BSWAP16('32'))
				{
					if (p[8] != '(' && !__intrinsic_isspace(p[8]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_MEMSET32, 8, PRIORITY_MEMSET32, OS_PUSH);
				}
				else if (*(uint16_t *)(p + 6) == BSWAP16('64'))
				{
					if (p[8] != '(' && !__intrinsic_isspace(p[8]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_MEMSET64, 8, PRIORITY_MEMSET64, OS_PUSH);
				}
				else
				{
					if (p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_MEMSET, 6, PRIORITY_MEMSET, OS_PUSH);
				}
			}
			break;
		case 'o':
			// "or"
			if (!bIsSeparatedLeft)
				break;
			if (p[1] != 'r')
				break;
			iTag = TAG_OR;
			nLength = 2;
			bPriority = PRIORITY_OR;
		APPEND_RET_OPERAND_OPERATOR:
			if (!IS_SEPARATED_RIGHT(p + nLength))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG(iTag, nLength, bPriority, OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND);
			APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH | OS_RET_OPERAND);
		case 'p':
			// "parse_int", "parse_real", "parse_reset", "printf"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) == BSWAP32('arse'))
			{
				if (*(uint32_t *)(p + 5) == BSWAP32('_int'))
				{
					if (p[9] != ';' && !__intrinsic_isspace(p[9]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_INT, 9, PRIORITY_PARSE_INT, OS_PUSH);
				}
				else if (*(uint32_t *)(p + 5) == BSWAP32('_rea'))
				{
					if (p[9] != 'l')
						break;
					if (p[10] != ';' && !__intrinsic_isspace(p[10]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_REAL, 10, PRIORITY_PARSE_REAL, OS_PUSH);
				}
				else if (*(uint32_t *)(p + 5) == BSWAP32('_res'))
				{
					if (*(uint16_t *)(p + 9) != BSWAP16('et'))
						break;
					if (p[11] != ';' && !__intrinsic_isspace(p[11]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_RESET, 11, PRIORITY_PARSE_RESET, OS_PUSH);
				}
			}
			else if (*(uint32_t *)(p + 1) == BSWAP32('rint'))
			{
				if (p[5] != 'f')
					break;
				if (p[6] != '(' && !__intrinsic_isspace(p[6]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_PRINTF, 6, PRIORITY_PRINTF, OS_PUSH);
			}
			break;
		case 'r':
			// "realloc", "return", "rol", "ror", "round::"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'e':
#if ALLOCATE_SUPPORT
				if (*(uint32_t *)(p + 2) == BSWAP32('allo'))
				{
					if (p[6] != 'c')
						break;
					if (p[7] != '(' && !__intrinsic_isspace(p[7]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_REALLOC, 7, PRIORITY_REALLOC, OS_PUSH);
				}
				else
#endif
				if (*(uint32_t *)(p + 2) == BSWAP32('turn'))
				{
					if (p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_RETURN, 6, PRIORITY_RETURN, OS_PUSH);
				}
				break;
			case 'o':
				if (p[2] == 'l')
				{
					iTag = TAG_ROL;
					nLength = 3;
					bPriority = PRIORITY_ROL;
					goto APPEND_WORD_OPERATOR;
				}
				else if (p[2] == 'r')
				{
					iTag = TAG_ROR;
					nLength = 3;
					bPriority = PRIORITY_ROR;
					goto APPEND_WORD_OPERATOR;
				}
				else if (p[2] == 'u')
				{
					if (*(uint32_t *)(p + 3) != BSWAP32('nd::'))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_ROUND, 7, PRIORITY_FUNCTION, OS_PUSH);
				}
				break;
			}
			break;
		case 's':
			// "sar", "strcpy", "strlen::"
			// not implemented: "switch"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				if (p[2] != 'r')
					break;
				iTag = TAG_SAR;
				nLength = 3;
				bPriority = PRIORITY_SAR;
				goto APPEND_WORD_OPERATOR;
			case 't':
				if (p[2] != 'r')
					break;
				if (p[3] == 'c')
				{
					if (*(uint16_t *)(p + 4) != BSWAP16('py'))
						break;
					if (p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_STRCPY, 6, PRIORITY_STRCPY, OS_PUSH);
				}
				else if (p[3] == 'l')
				{
					if (*(uint32_t *)(p + 4) != BSWAP32('en::'))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_STRLEN, 8, PRIORITY_FUNCTION, OS_PUSH);
				}
				break;
#if IMPLEMENTED
			case 'w':
				if (*(uint32_t *)(p + 2) != BSWAP32('itch'))
					break;
				if (p[6] != '(' && !__intrinsic_isspace(p[6]))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_SWITCH, 6, PRIORITY_SWITCH, OS_PUSH);
#endif
			}
			break;
		APPEND_WORD_OPERATOR:
			if (!IS_SEPARATED_RIGHT(p + nLength))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH);
		case 't':
			// "trunc::"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('runc'))
				break;
			if (*(uint16_t *)(p + 5) != BSWAP16('::'))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_TRUNC, 7, PRIORITY_FUNCTION, OS_PUSH);
		case 'u':
			// unicode or utf-8 string (u", u8")
			// "utof::"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case '"':
				p++;
				goto DOUBLE_QUOTED_STRING;
			case '8':
				if (p[2] != '"')
					break;
				p += 2;
				goto DOUBLE_QUOTED_STRING;
			case 't':
				if (*(uint32_t *)(p + 2) != BSWAP32('of::'))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_UTOF, 6, PRIORITY_FUNCTION, OS_PUSH);
			}
			break;
		case 'w':
			// "wcscpy", "wcslen::", "while"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'c':
				if (p[2] != 's')
					break;
				if (p[3] == 'c')
				{
					if (*(uint16_t *)(p + 4) != BSWAP16('py'))
						break;
					if (p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_WCSCPY, 6, PRIORITY_WCSCPY, OS_PUSH);
				}
				else if (p[3] == 'l')
				{
					if (*(uint32_t *)(p + 4) != BSWAP32('en::'))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_WCSLEN, 8, PRIORITY_FUNCTION, OS_PUSH);
				}
				break;
			case 'h':
				if (*(uint16_t *)(p + 2) != BSWAP16('il'))
					break;
				if (p[4] != 'e')
					break;
				if (p[5] != '(' && !__intrinsic_isspace(p[5]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_WHILE, 5, PRIORITY_WHILE, OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN);
			}
			break;
		case '|':
			// "||", "|="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
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
				if (p[2] == ']')
				{
					iTag = (TAG)(TAG_REV_ENDIAN2 + p[1] - '2');
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
			p += 2;
			continue;
		}
#if USE_PLUGIN
		if (bIsSeparatedLeft && PluginFunctions)
		{
			PLUGIN_FUNCTION_VECTOR *Vector;

			Vector = &PluginFunctionVector[*p];
			if (Vector->First)
			{
				size_t low = 0;
				size_t high = Vector->Last - Vector->First;
				do
				{
					size_t middle = (low + high - 1) / 2;
					PLUGIN_FUNCTION *Function = &Vector->First[middle];
					int ret = strncmp(Function->Name + 1, p + 1, Function->NameLength - 1);
					if (ret < 0)
					{
						low = middle + 1;
					}
					else if (ret > 0)
					{
						high = middle;
					}
					else
					{
						if (p[Function->NameLength] != '(' && !__intrinsic_isspace(p[Function->NameLength]))
						{
							BOOLEAN match = FALSE;
							while (++Function < Vector->Last && strncmp(Function->Name + 1, p + 1, Function->NameLength - 1) == 0)
								if (match = (p[Function->NameLength] == '(' || __intrinsic_isspace(p[Function->NameLength])))
									break;
							if (!match)
								break;
						}
						if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
							goto FAILED;
						bNextIsSeparatedLeft = TRUE;
						bCorrectTag = TRUE;
						lpMarkup->Tag      = TAG_PLUGIN;
						lpMarkup->Length   = Function->NameLength;
						lpMarkup->String   = p;
						lpMarkup->Priority = PRIORITY_PLUGIN;
						lpMarkup->Type     = OS_PUSH;
						lpMarkup->Depth    = 0;
						lpMarkup->Function = Function;
						p += Function->NameLength;
						goto CONTINUE;
					}
				} while (low < high);
			}
		}
#endif
		p++;

	CONTINUE:;
		#undef SET_FUNCTION
		#undef APPEND_TAG
		#undef APPEND_TAG_WITH_CONTINUE
		#undef IS_SEPARATED_RIGHT
	}

	lpEndOfTag = lpTagArray + nNumberOfTag;

	// add ternary block
	if (nFirstTernary != SIZE_MAX)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstTernary; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpBegin, *lpEnd;
			size_t nDepth;

			if (lpTag1->Tag != TAG_TERNARY)
				continue;
			lpBegin = NULL;
			nDepth = 0;
			for (MARKUP *lpElement = lpTag1 - 1; lpElement >= lpTagArray; lpElement--)
			{
				if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
				{
					if (nDepth || lpElement->Priority >= PRIORITY_TERNARY)
						continue;
				}
				else if (lpElement->Type & OS_CLOSE)
				{
					nDepth++;
					continue;
				}
				else if (nDepth)
				{
					nDepth--;
					continue;
				}
				lpBegin = lpElement;
				break;
			}
			lpEnd = NULL;
			nDepth = 0;
			for (MARKUP *lpElement = lpTag1 + 1; lpElement < lpEndOfTag; lpElement++)
			{
				if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
				{
					if (nDepth || lpElement->Tag != TAG_TERNARY_SPLIT)
						continue;
				}
				else if (lpElement->Type & OS_OPEN)
				{
					nDepth++;
					continue;
				}
				else
				{
					if (!nDepth)
						break;
					nDepth--;
					continue;
				}
				nDepth = 0;
				while (++lpElement < lpEndOfTag)
				{
					if (!(lpElement->Type & OS_SPLIT) || nDepth)
					{
						if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
						{
							if (nDepth || lpElement->Priority >= PRIORITY_TERNARY)
								continue;
						}
						else if (lpElement->Type & OS_OPEN)
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
					lpEnd = lpElement;
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
					goto FAILED;
				size = (size_t)lpEndOfTag - (size_t)lpBegin;
				(LPBYTE)lpBegin += (size_t)lpTagArray;
				src = lpBegin++;
				memmove(lpBegin, src, size);
				lpBegin->String += lpBegin->Length;
			}
			else
			{
				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED;
				memmove(lpTagArray + 1, lpTagArray, (size_t)lpEndOfTag);
				lpBegin = lpTagArray;
				lpBegin->String = lpSrc;
			}
			lpBegin->Tag      = TAG_PARENTHESIS_OPEN;
			lpBegin->Length   = 0;
			lpBegin->Priority = PRIORITY_PARENTHESIS_OPEN;
			lpBegin->Type     = OS_OPEN | OS_PARENTHESIS;
			lpBegin->Depth    = 0;
#if USE_PLUGIN
			lpBegin->Function = NULL;
#endif
			if (lpEnd)
			{
				size_t size;

				if (!ReAllocMarkup(&lpTagArray, &nNumberOfTag))
					goto FAILED;
				size = (size_t)lpEndOfTag - (size_t)lpEnd;
				(LPBYTE)lpEnd += (size_t)(lpTagArray + 1);
				memmove(lpEnd + 1, lpEnd, size);
			}
			else
			{
				if (!(lpEnd = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
					goto FAILED;
				lpEnd->String = lpSrc + nSrcLength;
			}
			(LPBYTE)lpTag1     += (size_t)(lpTagArray + 1);
			(LPBYTE)lpEndOfTag += (size_t)(lpTagArray + 2);
			lpEnd->Tag      = TAG_PARENTHESIS_CLOSE;
			lpEnd->Length   = 0;
			lpEnd->Priority = PRIORITY_PARENTHESIS_CLOSE;
			lpEnd->Type     = OS_PUSH | OS_CLOSE | OS_PARENTHESIS | OS_TERNARY_END;
			lpEnd->Depth    = 0;
#if USE_PLUGIN
			lpEnd->Function = NULL;
#endif
		}
	}

	// correct tag
	if (bCorrectTag)
	{
		MARKUP *lpFirstDoWhileLoop;

		lpFirstDoWhileLoop = NULL;
		for (MARKUP *lpTag1 = lpTagArray; lpTag1 < lpEndOfTag; lpTag1++)
		{
			switch (lpTag1->Tag)
			{

			#define CORRECT_FUNCTION(EndTag, NumberOfArgs)                                  \
			do                                                                              \
			{                                                                               \
			    MARKUP *lpElement, *lpClose;                                                \
			                                                                                \
			    if ((lpElement = lpTag1 + 1) < lpEndOfTag &&                                \
			        lpElement->Tag == TAG_PARENTHESIS_OPEN &&                               \
			        (lpClose = FindParenthesisClose(lpElement, lpEndOfTag)) < lpEndOfTag)   \
			    {                                                                           \
			        if (NumberOfArgs <= 1)                                                  \
			        {                                                                       \
			            lpClose->Tag = EndTag;                                              \
			            lpClose->Type |= OS_PUSH;                                           \
			            break;                                                              \
			        }                                                                       \
			        else                                                                    \
			        {                                                                       \
			            size_t nCount;                                                      \
			                                                                                \
			            nCount = NumberOfArgs - 1;                                          \
			            do                                                                  \
			                if ((lpElement = FindDelimiter(lpElement, lpClose)) >= lpClose) \
			                    break;                                                      \
			            while (--nCount);                                                   \
			            if (!nCount)                                                        \
			            {                                                                   \
			                lpClose->Tag = EndTag;                                          \
			                lpClose->Type |= OS_PUSH;                                       \
			                break;                                                          \
			            }                                                                   \
			        }                                                                       \
			    }                                                                           \
			    lpTag1->Tag = TAG_PARSE_ERROR;                                              \
			    lpTag1->Type |= OS_PUSH;                                                    \
			} while (0)

			case TAG_PRINTF:
				// correct printf
				CORRECT_FUNCTION(TAG_PRINTF_END, 1);
				break;
			case TAG_DPRINTF:
				// correct dprintf
				CORRECT_FUNCTION(TAG_DPRINTF_END, 1);
				break;
			case TAG_MEMMOVE:
				// correct memmove
				CORRECT_FUNCTION(TAG_MEMMOVE_END, 3);
				break;
			case TAG_MEMSET:
				// correct memset
				CORRECT_FUNCTION(TAG_MEMSET_END, 3);
				break;
			case TAG_MEMSET16:
				// correct memset16
				CORRECT_FUNCTION(TAG_MEMSET16_END, 3);
				break;
			case TAG_MEMSET32:
				// correct memset32
				CORRECT_FUNCTION(TAG_MEMSET32_END, 3);
				break;
			case TAG_MEMSET64:
				// correct memset64
				CORRECT_FUNCTION(TAG_MEMSET64_END, 3);
				break;
			case TAG_STRCPY:
				// correct strcpy
				CORRECT_FUNCTION(TAG_STRCPY_END, 2);
				break;
			case TAG_WCSCPY:
				// correct wcscpy
				CORRECT_FUNCTION(TAG_WCSCPY_END, 2);
				break;

#if ALLOCATE_SUPPORT
			case TAG_REALLOC:
				// correct realloc
				CORRECT_FUNCTION(TAG_REALLOC_END, 2);
				break;
#endif
#if USE_PLUGIN
			case TAG_PLUGIN:
				// correct plugin function
				CORRECT_FUNCTION(TAG_PLUGIN_END, 0);
				break;
#endif

			#undef CORRECT_FUNCTION

			case TAG_IF:
				// correct if block
				{
					MARKUP *lpElement, *lpClose, *lpNext, *lpElse;

					if ((lpElement = lpTag1 + 1) < lpEndOfTag &&
						lpElement->Tag == TAG_PARENTHESIS_OPEN &&
						(lpNext = (lpElement = lpClose = FindParenthesisClose(lpElement, lpEndOfTag)) + 1) < lpEndOfTag &&
						(
							lpNext->Tag == TAG_ELSE ||
							(lpElement = FindEndOfStructuredStatement(lpNext, lpEndOfTag)) < lpEndOfTag
						) &&
						(
							!(lpElse = ((lpNext = lpElement + 1) < lpEndOfTag && lpNext->Tag == TAG_ELSE) ? lpNext : NULL) ||
							(
								++lpNext < lpEndOfTag &&
								(lpElement = FindEndOfStructuredStatement(lpNext, lpEndOfTag)) < lpEndOfTag
							)
						))
					{
						lpClose->Tag = TAG_IF_EXPR;
						lpClose->Type |= OS_PUSH;
						while (--lpElement > lpClose)
							if (lpElement != lpElse)
								lpElement->Depth++;
					}
					else
					{
						lpTag1->Tag = TAG_PARSE_ERROR;
						lpTag1->Type |= OS_PUSH;
					}
				}
				break;
			case TAG_DO:
				// correct do-while loop
				{
					MARKUP *lpElement, *lpWhile;

					if ((lpElement = lpTag1 + 1) < lpEndOfTag &&
						(lpElement = (lpWhile = (lpElement->Tag == TAG_PARENTHESIS_OPEN ?
							FindParenthesisClose(lpElement, lpEndOfTag) :
							FindSplit(lpElement, lpEndOfTag)) + 1) + 1) < lpEndOfTag &&
						lpWhile->Tag == TAG_WHILE &&
						lpElement->Tag == TAG_PARENTHESIS_OPEN &&
						(lpElement = FindParenthesisClose(lpElement, lpEndOfTag)) < lpEndOfTag)
					{
						lpWhile->Type = OS_PUSH | OS_POST;
						lpElement->Tag = TAG_WHILE_EXPR;
						lpElement->Type |= OS_PUSH | OS_POST | OS_LOOP_END;
						if (!lpFirstDoWhileLoop)
							lpFirstDoWhileLoop = lpTag1;
					}
					else
					{
						lpTag1->Tag = TAG_PARSE_ERROR;
						lpTag1->Type |= OS_PUSH;
					}
				}
				break;
			case TAG_WHILE:
				// correct while loop
				if (!(lpTag1->Type & OS_POST))
				{
					MARKUP *lpElement, *lpClose;

					if ((lpElement = lpTag1 + 1) < lpEndOfTag &&
						lpElement->Tag == TAG_PARENTHESIS_OPEN &&
						(lpElement = (lpClose = FindParenthesisClose(lpElement, lpEndOfTag)) + 1) < lpEndOfTag &&
						(lpElement = FindEndOfStructuredStatement(lpElement, lpEndOfTag)) < lpEndOfTag)
					{
						lpClose->Tag = TAG_WHILE_EXPR;
						lpClose->Type |= OS_PUSH;
						lpElement->Type |= OS_PUSH | OS_LOOP_END;
					}
					else
					{
						lpTag1->Tag = TAG_PARSE_ERROR;
						lpTag1->Type |= OS_PUSH;
					}
				}
				break;
			case TAG_FOR:
				// correct for loop
				{
					MARKUP *lpOpen, *lpInitialize, *lpCondition, *lpUpdate, *lpEnd;

					if ((lpInitialize = (lpOpen = lpTag1 + 1) + 1) < lpEndOfTag &&
						lpOpen->Tag == TAG_PARENTHESIS_OPEN &&
						(lpEnd = (lpUpdate = FindParenthesisClose(lpOpen, lpEndOfTag)) + 1) < lpEndOfTag &&
						(lpCondition = (lpInitialize = FindSplit(lpInitialize, lpUpdate)) + 1) < lpUpdate &&
						(lpCondition = FindSplit(lpCondition, lpUpdate)) < lpUpdate &&
						(lpEnd = FindEndOfStructuredStatement(lpEnd, lpEndOfTag)) < lpEndOfTag)
					{
						lpInitialize->Tag = TAG_FOR_INITIALIZE;
						lpInitialize->Type |= OS_PUSH;
						lpCondition->Tag = TAG_FOR_CONDITION;
						lpCondition->Type |= OS_PUSH | OS_LOOP_BEGIN;
						lpUpdate->Tag = TAG_FOR_UPDATE;
						lpUpdate->Type |= OS_PUSH;
						lpEnd->Type |= OS_PUSH | OS_LOOP_END;
					}
					else
					{
						lpTag1->Tag = TAG_PARSE_ERROR;
						lpTag1->Type |= OS_PUSH;
					}
				}
				break;
			}
		}

		if (lpFirstDoWhileLoop)
		{
			// correct continue of do-while loop
			for (MARKUP *lpTag1 = lpFirstDoWhileLoop; lpTag1 < lpEndOfTag; lpTag1++)
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
	}

	// add last value
	if (nNumberOfTag)
	{
		LPSTR lpString;
		LPSTR lpEnd;

		lpString = lpTagArray[nNumberOfTag - 1].String + lpTagArray[nNumberOfTag - 1].Length;
		lpEnd = lpSrc + nSrcLength;
		if (lpEnd > lpString)
		{
			if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))
				goto FAILED;
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpEnd - lpString;
			lpMarkup->String   = lpString;
			lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type     = OS_PUSH;
			lpMarkup->Depth    = lpTagArray[nNumberOfTag - 2].Depth + (lpTagArray[nNumberOfTag - 2].Tag == TAG_IF_EXPR || lpTagArray[nNumberOfTag - 2].Tag == TAG_ELSE);
#if USE_PLUGIN
			lpMarkup->Function = NULL;
#endif
			if (!TrimMarkupString(lpMarkup))
				nNumberOfTag--;
			lpEndOfTag = lpTagArray + nNumberOfTag;
		}
	}
	else
	{
		lpMarkup = lpTagArray;
		lpMarkup->Tag      = TAG_NOT_OPERATOR;
		lpMarkup->Length   = nSrcLength;
		lpMarkup->String   = lpSrc;
		lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
		lpMarkup->Type     = OS_PUSH;
		lpMarkup->Depth    = 0;
#if USE_PLUGIN
		lpMarkup->Function = NULL;
#endif
		if (!TrimMarkupString(lpMarkup))
			goto FAILED;
		nNumberOfTag++;
		lpEndOfTag++;
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (!lpMarkupArray)
		goto FAILED;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	p = lpSrc;
	for (MARKUP *lpTag = lpTagArray; lpTag < lpEndOfTag; lpTag++)
	{
		if (lpTag->String > p)
		{
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpTag->String - p;
			lpMarkup->String   = p;
			lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type     = OS_PUSH;
			lpMarkup->Depth    = lpTag != lpTagArray ? (lpTag - 1)->Depth + ((lpTag - 1)->Tag == TAG_IF_EXPR || (lpTag - 1)->Tag == TAG_ELSE) : 0;
#if USE_PLUGIN
			lpMarkup->Function = NULL;
#endif
			if (TrimMarkupString(lpMarkup))
			{
				do	/* do { ... } while (0) */
				{
					size_t  prefixLength;
					BOOLEAN isStringOperand, inDoubleQuote;
					LPBYTE  p, end;

					prefixLength = 0;
					if (lpMarkup->String[0] != '"' &&
						lpMarkup->String[0] != 'u' ||
						lpMarkup->Length < 1 || (
							lpMarkup->String[prefixLength = 1] != '"' && (
								lpMarkup->String[1] != '8' ||
								lpMarkup->Length < 2 ||
								lpMarkup->String[prefixLength = 2] != '"')))
					{
						TAG tag;

						if (lpMarkup == lpMarkupArray)
							break;
						tag = (lpMarkup - 1)->Tag;
						if (tag != TAG_PROCEDURE &&
							tag != TAG_IMPORT_FUNCTION &&
							tag != TAG_IMPORT_REFERENCE &&
							tag != TAG_SECTION)
						{
							break;
						}
						isStringOperand = FALSE;
					}
					else if (!(isStringOperand = lpMarkup == lpMarkupArray))
					{
						TAG tag;

						tag = (lpMarkup - 1)->Tag;
						if (tag != TAG_PROCEDURE &&
							tag != TAG_IMPORT_FUNCTION &&
							tag != TAG_IMPORT_REFERENCE &&
							tag != TAG_SECTION)
						{
							isStringOperand = TRUE;
						}
					}
					p = lpMarkup->String;
					end = p + lpMarkup->Length;
					if (inDoubleQuote = isStringOperand)
						p += prefixLength + 1;
					while (p < end)
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
									if (!(length = --end - p))
										break;
									memcpy(p, p + 1, length);
									switch (c = *p)
									{
									case '0':
										*p = '\0';
										break;
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
										{
											unsigned char *src, c1, c2;

											c1 = *(src = p + 1);
											if (!CTOI(&c1, 'f', 16))
												break;
											c2 = *(src + 1);
											if (CTOI(&c2, 'f', 16))
											{
												c1 = c1 * 0x10 + c2;
												src++;
											}
											end -= src++ - p;
											*p = c1;
											memcpy(p + 1, src, end - p);
										}
										break;
									default:
										if (__intrinsic_isleadbyte(c))
											p++;
										break;
									}
								}
								p++;
							}
							else if (!isStringOperand)
							{
								if (!(length = --end - p))
									break;
								memcpy(p, p + 1, length);
								inDoubleQuote = !inDoubleQuote;
							}
							else
							{
								LPBYTE next;

								if ((next = p) + 1 >= end)
									break;
								do
									c = *(++next);
								while (__intrinsic_isspace(c));
								if (c == '"')
								{
									end -= ++next - p;
									memcpy(p, next, end - p);
								}
								else
								{
									end = p + 1;
									break;
								}
							}
						}
						else
						{
							p += 2;
						}
					}
					lpMarkup->Length = end - lpMarkup->String;
				} while (0);
				lpMarkup++;
			}
		}
		*lpMarkup = *lpTag;
		p = lpMarkup->String + lpMarkup->Length;
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
		case TAG_DEC:
			if (lpMarkup + 1 < lpEndOfMarkup)
				if (lpMarkup[1].Tag == TAG_NOT_OPERATOR)
					break;
			// post increment/decrement operator
			if (lpMarkup == lpMarkupArray)
				break;
			if ((lpMarkup - 1)->Tag != TAG_NOT_OPERATOR)
				break;
			lpMarkup->Type = OS_PUSH | OS_MONADIC | OS_POST;
			lpMarkup->Priority = PRIORITY_POST_INC_DEC;
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
		case TAG_BIT_AND:
			if (lpMarkup + 1 < lpEndOfMarkup)
				if (lpMarkup[1].Tag != TAG_NOT_OPERATOR)
					break;
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if ((lpMarkup - 1)->Tag == TAG_NOT_OPERATOR || ((lpMarkup - 1)->Type & (OS_CLOSE | OS_POST)) ||
					!(lpMarkup - 1)->Length)// Avoid ternary conditions, because added block before marking.
					break;
			// address-of operator
			lpMarkup->Tag = TAG_ADDRESS_OF;
			lpMarkup->Priority = PRIORITY_ADDRESS_OF;
			lpMarkup->Type = OS_PUSH | OS_MONADIC;
			break;
		case TAG_MODULENAME:
			lpMarkup->Length--;
			break;
		}
	}

	// return
	*lpnNumberOfMarkup = lpEndOfMarkup - lpMarkupArray;
	return lpMarkupArray;

FAILED:
	HeapFree(hHeap, 0, lpTagArray);
	return NULL;
}
//---------------------------------------------------------------------
//「中置記法の文字列を、後置記法（逆ポーランド記法）に変換し
//	因子単位で格納したベクタを返す関数」
//---------------------------------------------------------------------
static size_t __stdcall Postfix(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT MARKUP **lpPostfixBuffer)
{
	MARKUP **lpPostfixFirst, **lpPostfixTop, **lpEndOfPostfix;
	MARKUP **lpFactorBuffer, **lpFactorTop, **lpEndOfFactor;
	size_t *lpnNestBuffer, *lpnNestTop, *lpnEndOfNest;
	MARKUP *lpMarkup, *lpEndOfMarkup;

	lpEndOfPostfix = lpPostfixBuffer;
	lpFactorBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpFactorBuffer)
		goto FAILED1;
	lpnNestBuffer = (size_t *)HeapAlloc(hHeap, 0, sizeof(size_t) * (nNumberOfMarkup + 1));
	if (!lpnNestBuffer)
		goto FAILED2;
	lpPostfixTop = lpPostfixFirst = lpEndOfPostfix;
	lpFactorTop = lpEndOfFactor = lpFactorBuffer;
	lpnNestTop = lpnEndOfNest = lpnNestBuffer;

	#define POSTFIX_IS_EMPTY()  (lpEndOfPostfix == lpPostfixFirst)
	#define POSTFIX_PUSH(value) (*(lpPostfixTop = lpEndOfPostfix++) = (value))
	#define POSTFIX_POP()       (!POSTFIX_IS_EMPTY() ? *(lpEndOfPostfix = lpPostfixTop != lpPostfixFirst ? lpPostfixTop-- : lpPostfixTop) : NULL)
	#define FACTOR_IS_EMPTY()   (lpEndOfFactor == lpFactorBuffer)
	#define FACTOR_PUSH(value)  (*(lpFactorTop = lpEndOfFactor++) = (value))
	#define FACTOR_POP()        (!FACTOR_IS_EMPTY() ? *(lpEndOfFactor = lpFactorTop != lpFactorBuffer ? lpFactorTop-- : lpFactorTop) : NULL)
	#define NEST_IS_EMPTY()     (lpnEndOfNest == lpnNestBuffer)
	#define NEST_PUSH(value)    (*(lpnNestTop = lpnEndOfNest++) = (value))
	#define NEST_POP()          (!NEST_IS_EMPTY() ? *(lpnEndOfNest = lpnNestTop != lpnNestBuffer ? lpnNestTop-- : lpnNestTop) : 0)

	NEST_PUSH(0);
	for (lpMarkup = lpMarkupArray, lpEndOfMarkup = lpMarkupArray + nNumberOfMarkup; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		if (lpMarkup->Type & (OS_CLOSE | OS_SPLIT | OS_DELIMITER | OS_LEFT_ASSIGN | OS_TERNARY))
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
			else if (lpMarkup->Type & OS_LEFT_ASSIGN)
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
			else if (lpMarkup->Type & OS_SPLIT)
			{
				while (!FACTOR_IS_EMPTY())
					POSTFIX_PUSH(FACTOR_POP());
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
				lpPostfixTop = lpPostfixFirst = lpEndOfPostfix;
				lpnEndOfNest = (lpnNestTop = lpnNestBuffer) + 1;
				*lpnNestTop = 0;
			}
			else	// OS_DELIMITER
			{
				if (!FACTOR_IS_EMPTY())
				{
					MARKUP *lpPrev;
					size_t nDepth;

					lpPrev = lpMarkup;
					nDepth = 0;
					while (--lpPrev != lpMarkupArray)
						if (lpPrev->Type & (OS_OPEN | OS_CLOSE | OS_SPLIT | OS_DELIMITER | OS_TERNARY))
							if (lpPrev->Type & OS_CLOSE)
								nDepth++;
							else if (!nDepth)
								break;
							else if (lpPrev->Type & OS_OPEN)
								nDepth--;
					while (!FACTOR_IS_EMPTY() && (*lpFactorTop)->String >= lpPrev->String)
					{
						POSTFIX_PUSH(FACTOR_POP());
						while (!*lpnNestTop)
							lpnEndOfNest = lpnNestTop--;
						(*lpnNestTop)--;
					}
				}
				/*
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
				*/
			}
			continue;
		}
		while (*lpnNestTop && lpMarkup->Priority <= (*lpFactorTop)->Priority)
		{
			POSTFIX_PUSH(FACTOR_POP());
			(*lpnNestTop)--;
		}
		if (lpMarkup->Type & (OS_PUSH | OS_OPEN))
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
	while (!FACTOR_IS_EMPTY())
		POSTFIX_PUSH(FACTOR_POP());
	HeapFree(hHeap, 0, lpnNestBuffer);
FAILED2:
	HeapFree(hHeap, 0, lpFactorBuffer);
FAILED1:
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
static uint64_t __cdecl InternalParsing(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, const string *Src, BOOL IsInteger, va_list ArgPtr)
{
	#define PROCESS_DESIRED_ACCESS (PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION)

	uint64_t                       qwResult;
	VARIABLE                       operandZero;
	BOOL                           bInitialIsInteger;
	LPSTR                          lpszSrc;
	size_t                         nSrcLength;
	char                           *p, *end, c;
#if ADDITIONAL_TAGS
	size_t                         capacity;
	TEndWithAttribute              *variable;
	string                         *code;
	size_t                         nVariableLength;
#if defined(__BORLANDC__)
	vector<TSSGAttributeElement *> *attributes;
#else
	vector_TSSGAttributeElement    *attributes;
#endif
#endif
	size_t                         nNumberOfMarkup;
	MARKUP                         *lpMarkupArray;
	MARKUP                         **lpPostfix;
	VARIABLE                       *lpOperandBuffer, *lpEndOfOperand, *lpOperandTop;
	MARKUP_VARIABLE                *lpVariable;
	size_t                         nNumberOfVariable;
	size_t                         nNumberOfPostfix;
	size_t                         length;
	HANDLE                         hProcess;
	BOOLEAN                        bCompoundAssign;
	VARIABLE                       operand;
#if REPEAT_INDEX
	LPSTR                          lpVariableStringBuffer;
#endif

	qwResult = 0;

	p = string_begin(Src) - 1;
	do
		c = *(++p);
	while (__intrinsic_isspace(c));

#if LOCAL_MEMORY_SUPPORT
	if (*p == 'L')
		if (__intrinsic_isspace(p[1]))
		{
			p++;
			do
				c = *(++p);
			while (__intrinsic_isspace(c));
		}
		else if (p[1] == '{')
			p++;
#endif

	end = string_end(Src);
	while (end > p)
	{
		c = *(end - 1);
		end--;
		if (__intrinsic_isspace(c))
			continue;
		end++;
		break;
	}
	nSrcLength = end - p;

#if !ADDITIONAL_TAGS
	if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, nSrcLength + sizeof(uint32_t))))
		goto FAILED1;
	lpszSrc[nSrcLength] = '\0';
	memcpy(lpszSrc, p, nSrcLength);
#else
	variable = (TEndWithAttribute *)TSSGCtrl_GetAttribute(SSGCtrl, SSGS, atVARIABLE);
	if (variable && (nVariableLength = string_length(code = TEndWithAttribute_GetCode(variable))))
	{
		unsigned long bits;

#ifndef _WIN64
		_BitScanReverse(&bits, nVariableLength + nSrcLength + (sizeof(uint32_t) - 1));
#else
		_BitScanReverse64(&bits, nVariableLength + nSrcLength + (sizeof(uint32_t) - 1));
#endif
		capacity = (size_t)1 << (bits + 1);
		if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
			goto FAILED1;
		lpszSrc[nVariableLength + nSrcLength] = '\0';
		memcpy(lpszSrc, string_c_str(code), nVariableLength);
		memcpy(lpszSrc + nVariableLength, p, nSrcLength);
		nSrcLength += nVariableLength;
	}
	else
	{
		unsigned long bits;

#ifndef _WIN64
		_BitScanReverse(&bits, nSrcLength + (sizeof(uint32_t) - 1));
#else
		_BitScanReverse64(&bits, nSrcLength + (sizeof(uint32_t) - 1));
#endif
		capacity = (size_t)1 << (bits + 1);
		if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
			goto FAILED1;
		lpszSrc[nSrcLength] = '\0';
		memcpy(lpszSrc, p, nSrcLength);
	}
	attributes = SSGS->type// check for TSSGCtrl::LoopSSRFile
		? TSSGSubject_GetAttribute(SSGS)
		: TSSGAttributeSelector_GetNowAtteributeVec(TSSGCtrl_GetAttributeSelector(SSGCtrl));
	if (attributes)
	{
		LPVOID lpMem;

		nSrcLength = ReplaceDefineByHeap(attributes, &lpszSrc, nSrcLength, capacity);
		if (nSrcLength == SIZE_MAX)
			goto FAILED2;
		if (!(lpMem = HeapReAlloc(hHeap, 0, lpszSrc, nSrcLength + sizeof(uint32_t))))
			goto FAILED2;
		lpszSrc = (LPSTR)lpMem;
	}
#if LOCAL_MEMORY_SUPPORT
	p = lpszSrc - 1;
	do
		c = *(++p);
	while (__intrinsic_isspace(c));
	if (p[0] == 'L' && p[1] && __intrinsic_isascii(p[1]) && !__intrinsic_iscsym(p[1]) && p[1] != '=')
	{
		p++;
		do
			c = *(++p);
		while (__intrinsic_isspace(c));
		nSrcLength -= p - lpszSrc;
		memcpy(lpszSrc, p, nSrcLength + 1);
	}
#endif
#endif

	lpMarkupArray = Markup(lpszSrc, nSrcLength, &nNumberOfMarkup);
	if (!lpMarkupArray)
		goto FAILED2;

	lpPostfix = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpPostfix)
		goto FAILED3;
	nNumberOfPostfix = Postfix(lpMarkupArray, nNumberOfMarkup, lpPostfix);
	if (!nNumberOfPostfix)
		goto FAILED4;

	lpOperandBuffer = (VARIABLE *)HeapAlloc(hHeap, 0, sizeof(VARIABLE) * (nNumberOfPostfix + 1));
	if (!lpOperandBuffer)
		goto FAILED4;
	lpVariable = (MARKUP_VARIABLE *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(MARKUP_VARIABLE) * 0x10);
	if (!lpVariable)
		goto FAILED5;

	#define OPERAND_IS_EMPTY()  (lpEndOfOperand == lpOperandBuffer)
	#define OPERAND_PUSH(value) (*(lpOperandTop = lpEndOfOperand++) = (value))
	#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpEndOfOperand = lpOperandTop != lpOperandBuffer ? lpOperandTop-- : lpOperandTop) : operandZero)
	#define OPERAND_CLEAR()     (*(lpOperandTop = lpEndOfOperand = lpOperandBuffer) = operandZero)

	bInitialIsInteger = IsInteger;
	if (nNumberOfMarkup)
		if (lpMarkupArray->Tag == TAG_PARSE_INT)
			IsInteger = TRUE;
		else if (lpMarkupArray->Tag == TAG_PARSE_REAL)
			IsInteger = FALSE;
	hProcess = NULL;
	operandZero.Quad = 0;
	operandZero.IsQuad = !IsInteger;
	OPERAND_CLEAR();
#if !SUBJECT_STATUS
	nNumberOfVariable = 0;
#else
	if (IsInteger)
	{
		lpVariable[0].Length = 4;
		lpVariable[0].String = "Addr";
		lpVariable[0].Value.Quad = (uint64_t)SSGS->address;
		lpVariable[0].Value.IsQuad = sizeof(SSGS->address) > sizeof(uint32_t);
		lpVariable[1].Length = 4;
		lpVariable[1].String = "Read";
		lpVariable[1].Value.Quad = (uint64_t)SSGS->evaluateAtRead;
		lpVariable[1].Value.IsQuad = sizeof(SSGS->evaluateAtRead) > sizeof(uint32_t);
		lpVariable[2].Length = 4;
		lpVariable[2].String = "Size";
		lpVariable[2].Value.Quad = (uint64_t)TSSGSubject_GetSize(SSGS);
		lpVariable[2].Value.IsQuad = FALSE;
		lpVariable[3].Length = 4;
		lpVariable[3].String = "Type";
		lpVariable[3].Value.Quad = (uint64_t)SSGS->type;
		lpVariable[3].Value.IsQuad = sizeof(SSGS->type) > sizeof(uint32_t);
	}
	else
	{
		lpVariable[0].Length = 4;
		lpVariable[0].String = "Addr";
		lpVariable[0].Value.Real = (double)(size_t)SSGS->address;
		lpVariable[0].Value.IsQuad = TRUE;
		lpVariable[1].Length = 4;
		lpVariable[1].String = "Read";
		lpVariable[1].Value.Real = (double)SSGS->evaluateAtRead;
		lpVariable[1].Value.IsQuad = TRUE;
		lpVariable[2].Length = 4;
		lpVariable[2].String = "Size";
		lpVariable[2].Value.Real = (double)TSSGSubject_GetSize(SSGS);
		lpVariable[2].Value.IsQuad = TRUE;
		lpVariable[3].Length = 4;
		lpVariable[3].String = "Type";
		lpVariable[3].Value.Real = (double)SSGS->type;
		lpVariable[3].Value.IsQuad = TRUE;
	}
	nNumberOfVariable = 4;
#endif
	while (length = va_arg(ArgPtr, size_t))
	{
		lpVariable[nNumberOfVariable].Length = length;
		lpVariable[nNumberOfVariable].String = va_arg(ArgPtr, LPCSTR);
		lpVariable[nNumberOfVariable].Value.Quad = va_arg(ArgPtr, uint64_t);
		lpVariable[nNumberOfVariable].Value.IsQuad = !!lpVariable[nNumberOfVariable].Value.High || !IsInteger;
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
			goto FAILED6;
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
					goto FAILED7;
				p += (size_t)lpMem - (size_t)lpVariableStringBuffer;
				lpVariableStringBuffer = (LPSTR)lpMem;
			}
			*(uint32_t *)p = BSWAP32('Idx\0');
			_ultoa(lpProperty->RepeatDepth, p + 3, 10);
			if (nNumberOfVariable && !(nNumberOfVariable & 0x0F))
			{
				LPVOID lpMem;
				size_t nBytes;

				nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
				lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
				if (!lpMem)
					goto FAILED7;
				lpVariable = (MARKUP_VARIABLE *)lpMem;
			}
			lpVariable[nNumberOfVariable].Length = nVariableLength = strlen(p + 3) + 3;
			nSize += ++nVariableLength;
			lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
			p += nVariableLength;
			if (IsInteger)
			{
				lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
				lpVariable[nNumberOfVariable].Value.IsQuad = FALSE;
			}
			else
			{
				lpVariable[nNumberOfVariable].Value.Real = (double)lpProperty->RepeatIndex;
				lpVariable[nNumberOfVariable].Value.IsQuad = TRUE;
			}
			nNumberOfVariable++;
			if (++nForward)
			{
				*(uint32_t *) p      = BSWAP32('FwdI');
				*(uint16_t *)(p + 4) = BSWAP16('dx'  );
				_ultoa(nForward, p + 6, 10);
				nVariableLength = strlen(p + 6) + 6;
			}
			else
			{
				*(uint32_t *)p = BSWAP32('Idx\0');
				nVariableLength = 3;
			}
			if (nNumberOfVariable && !(nNumberOfVariable & 0x0F))
			{
				LPVOID lpMem;
				size_t nBytes;

				nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
				lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
				if (!lpMem)
					goto FAILED7;
				lpVariable = (MARKUP_VARIABLE *)lpMem;
			}
			lpVariable[nNumberOfVariable].Length = nVariableLength;
			nSize += ++nVariableLength;
			lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
			p += nVariableLength;
			if (IsInteger)
			{
				lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
				lpVariable[nNumberOfVariable].Value.IsQuad = FALSE;
			}
			else
			{
				lpVariable[nNumberOfVariable].Value.Real = (double)lpProperty->RepeatIndex;
				lpVariable[nNumberOfVariable].Value.IsQuad = TRUE;
			}
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
		MARKUP  *lpNext;
		LPCSTR  lpGuideText;
#if !defined(__BORLANDC__)
		size_t  nGuideTextLength;
#endif

		lpMarkup = lpPostfix[i];
		switch (lpMarkup->Tag)
		{
		case TAG_PARSE_INT:
			IsInteger = TRUE;
			operandZero.IsQuad = FALSE;
			break;
		case TAG_PARSE_REAL:
			IsInteger = FALSE;
			operandZero.IsQuad = TRUE;
			break;
		case TAG_PARSE_RESET:
			IsInteger = bInitialIsInteger;
			operandZero.IsQuad = !IsInteger;
			break;
		case TAG_IF:
		case TAG_DO:
		case TAG_WHILE:
		case TAG_FOR:
			OPERAND_CLEAR();
			break;
		case TAG_IF_EXPR:
			boolValue = OPERAND_IS_EMPTY() || (IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real);
			OPERAND_CLEAR();
			if (boolValue)
				continue;
			while (++i < nNumberOfPostfix && lpPostfix[i]->Depth > lpMarkup->Depth);
			if (i >= nNumberOfPostfix)
				continue;
			if (lpPostfix[i]->Tag == TAG_ELSE && lpPostfix[i]->Depth == lpMarkup->Depth)
			{
				lpMarkup = lpPostfix[i];
				break;
			}
			i--;
			continue;
		case TAG_ELSE:
			while (++i < nNumberOfPostfix && lpPostfix[i]->Depth > lpMarkup->Depth);
			i--;
			continue;
		case TAG_WHILE_EXPR:
			boolValue = OPERAND_IS_EMPTY() || (IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real);
			OPERAND_CLEAR();
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
			boolValue = OPERAND_IS_EMPTY() || (IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real);
			OPERAND_CLEAR();
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
		case TAG_STRCPY:
		case TAG_WCSCPY:
#if ALLOCATE_SUPPORT
		case TAG_REALLOC:
#endif
#if USE_PLUGIN
		case TAG_PLUGIN:
#endif
		case TAG_RETURN:
			break;

		#define IS_STRING_OPERAND(element) (            \
		    (element)->Tag == TAG_NOT_OPERATOR &&       \
		    (element)->Length && (                      \
		        (element)->String[0] == '"' || (        \
		        (element)->String[0] == 'u' &&          \
		        (element)->Length >= 1 && (             \
		            (element)->String[1] == '"' || (    \
		            (element)->String[1] == '8' &&      \
		            (element)->Length >= 2 &&           \
		            (element)->String[2] == '"')))))

		#define CHECK_STRING_OPERAND(element, prefixLength) (           \
		    (element)->Tag == TAG_NOT_OPERATOR &&                       \
		    (element)->Length && (                                      \
		        (element)->String[*(prefixLength) = 0] == '"' || (      \
		        (element)->String[0] == 'u' &&                          \
		        (element)->Length >= 1 && (                             \
		            (element)->String[*(prefixLength) = 1] == '"' || (  \
		            (element)->String[1] == '8' &&                      \
		            (element)->Length >= 2 &&                           \
		            (element)->String[*(prefixLength) = 2] == '"')))))

		case TAG_PRINTF_END:
		case TAG_DPRINTF_END:
			if (TSSGCtrl_GetSSGActionListner(SSGCtrl) && (lpMarkup->Tag != TAG_PRINTF_END || TMainForm_GetUserMode(MainForm) >= 3))
			{
				MARKUP    *element1, *element2;
				size_t    stackSize, bufferSize, depth;
				TAG       functionTag;
				uintptr_t *stack, *param;
				LPSTR     psz;
				VARIABLE  *operand;

				if ((element1 = lpMarkup - 1) <= lpMarkupArray)
					goto PARSING_ERROR;
				bufferSize = stackSize = 0;
				depth = 1;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						size_t prefixLength;

						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth++;
								continue;
							}
							if (--depth)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (!CHECK_STRING_OPERAND(element2, &prefixLength))
						{
							if (--lpEndOfOperand < lpOperandBuffer)
								goto PARSING_ERROR;
#ifndef _WIN64
							if (!lpEndOfOperand->IsQuad && IsInteger)
								stackSize += sizeof(uint32_t);
							else
#endif
								stackSize += sizeof(uint64_t);
						}
						else
						{
							size_t length;
							LPSTR  string;

							stackSize += sizeof(LPSTR);
							length = element2->Length - prefixLength - 1;
							string = element2->String + prefixLength + 1;
							if (length && string[length - 1] == '"')
								length--;
							bufferSize += length + 1;
						}
					}
				while (depth && --element1 != lpMarkupArray);
				lpOperandTop = lpEndOfOperand++;
				if ((element1 - 1) < lpMarkupArray)
					goto PARSING_ERROR;
				functionTag = lpMarkup->Tag - (TAG_PRINTF_END - TAG_PRINTF);
				if ((element1 - 1)->Tag != functionTag)
					goto PARSING_ERROR;
				if (!(bufferSize += stackSize))
					goto PARSING_ERROR;
				stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, bufferSize);
				if (!stack)
					goto FAILED8;
				param = stack;
				psz = (LPSTR)stack + stackSize;
				operand = lpOperandTop;
				depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						size_t prefixLength;

						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (!CHECK_STRING_OPERAND(element2, &prefixLength))
						{
#ifndef _WIN64
							if (!operand->IsQuad && IsInteger)
								*(param++) = operand->Low;
							else
#endif
								*(((uint64_t *)param)++) = operand->Quad;
							operand++;
						}
						else
						{
							size_t length;
							LPSTR  string;

							length = element2->Length - prefixLength - 1;
							string = element2->String + prefixLength + 1;
							if (length && string[length - 1] == '"')
								length--;
							memcpy(psz, string, length);
							*(param++) = (uintptr_t)psz;
							psz[length] = '\0';
							psz += length + 1;
						}
					}
				while (++element1 != lpMarkup);
				if (functionTag == TAG_PRINTF)
					GuidePrintV((const char *)*stack, (va_list)(stack + 1));
				else
					DebugPrintV((const char *)*stack, (va_list)(stack + 1));
				HeapFree(hHeap, 0, stack);
			}
			OPERAND_CLEAR();
			continue;

		#define FIND_PARENTHESIS_OPEN(element1)                             \
		do                                                                  \
		{                                                                   \
		    size_t depth;                                                   \
		                                                                    \
		    if ((element1 = lpMarkup - 1) <= lpMarkupArray)                 \
		        goto PARSING_ERROR;                                         \
		    depth = 1;                                                      \
		    do                                                              \
		        if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))   \
		        {                                                           \
		            MARKUP *element2;                                       \
		                                                                    \
		            if (element1->Type & (OS_OPEN | OS_CLOSE))              \
		            {                                                       \
		                if (element1->Type & OS_CLOSE)                      \
		                {                                                   \
		                    depth++;                                        \
		                    continue;                                       \
		                }                                                   \
		                if (--depth)                                        \
		                {                                                   \
		                    continue;                                       \
		                }                                                   \
		            }                                                       \
		            if (depth > 1)                                          \
		                continue;                                           \
		            element2 = element1;                                    \
		            do                                                      \
		                if (++element2 >= lpMarkup)                         \
		                    goto PARSING_ERROR;                             \
		            while (element2->Type & OS_OPEN);                       \
		            if (!IS_STRING_OPERAND(element2))                       \
		                lpEndOfOperand--;                                   \
		        }                                                           \
		    while (depth && --element1 != lpMarkupArray);                   \
		    if (lpEndOfOperand < lpOperandBuffer)                           \
		        goto PARSING_ERROR;                                         \
		    lpOperandTop = lpEndOfOperand++;                                \
		} while (0)

		case TAG_MEMMOVE_END:
			{
				MARKUP     *element1, *element2;
				size_t     depth, numberOfArgs;
				VARIABLE   *operand;
				LPSTR      lpEndOfSrc;
				char       cUnterminated;
				void       *lpBuffer;
				NTSTATUS   Status;
				HANDLE     hDestProcess;
				void       *lpDest;
				HANDLE     hSrcProcess;
				const void *lpSrc;
				size_t     nSize;

				FIND_PARENTHESIS_OPEN(element1);
				if (element1 - 1 < lpMarkupArray)
					goto PARSING_ERROR;
				if ((element1 - 1)->Tag != TAG_MEMMOVE)
					goto PARSING_ERROR;
				lpSrc = lpBuffer = lpEndOfSrc = NULL;
				hSrcProcess = hDestProcess = (HANDLE)TRUE;
				operand = lpOperandTop;
				numberOfArgs = depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (++numberOfArgs == 1)
						{
							if (IS_STRING_OPERAND(element2))
								goto PARSING_ERROR;
							if (element2->Tag == TAG_PARAM_LOCAL)
								hDestProcess = NULL;
							lpDest = IsInteger ? (void *)(uintptr_t)operand->Quad : (void *)(uintptr_t)operand->Real;
							operand++;
						}
						else if (numberOfArgs == 2)
						{
							size_t prefixLength;

							if (!CHECK_STRING_OPERAND(element2, &prefixLength))
							{
								if (element2->Tag == TAG_PARAM_LOCAL)
									hSrcProcess = NULL;
								lpSrc = IsInteger ? (void *)(uintptr_t)operand->Quad : (void *)(uintptr_t)operand->Real;
								operand++;
							}
							else
							{
								size_t length;
								LPSTR  string;

								hSrcProcess = NULL;
								length = element2->Length - prefixLength - 1;
								string = element2->String + prefixLength + 1;
								if (length && string[length - 1] == '"')
									length--;
								if (!prefixLength)
								{
									cUnterminated = *(lpEndOfSrc = (LPSTR)(lpSrc = string) + length);
									*lpEndOfSrc = '\0';
								}
								else if (!(--prefixLength))
								{
									int cchWideChar;

									cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, string, length, NULL, 0);
									if (!(lpBuffer = HeapAlloc(hHeap, 0, ((size_t)cchWideChar + 1) * sizeof(wchar_t))))
										goto FAILED8;
									MultiByteToWideChar(CP_THREAD_ACP, 0, string, length, (LPWSTR)lpBuffer, cchWideChar);
									((LPWSTR)(lpSrc = lpBuffer))[cchWideChar] = L'\0';
								}
								else
								{
									int cchUtf8;

									cchUtf8 = MultiByteToUtf8(CP_THREAD_ACP, 0, string, length, NULL, 0);
									if (!(lpBuffer = HeapAlloc(hHeap, 0, cchUtf8 + 1)))
										goto FAILED8;
									MultiByteToUtf8(CP_THREAD_ACP, 0, string, length, (LPSTR)lpBuffer, cchUtf8);
									((LPSTR)(lpSrc = lpBuffer))[cchUtf8] = '\0';
								}
							}
						}
						else
						{
							if (IS_STRING_OPERAND(element2))
								goto PARSING_ERROR;
							nSize = IsInteger ? (size_t)operand->Quad : (size_t)operand->Real;
							break;
						}
					}
				while (++element1 != lpMarkup);
				if (numberOfArgs < 3)
					goto PARSING_ERROR;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto FAILED7;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (lpEndOfSrc)
					*lpEndOfSrc = cUnterminated;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (uint32_t)lpSrc);
						goto FAILED8;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
						goto WRITE_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_MEMSET_END:
		case TAG_MEMSET16_END:
		case TAG_MEMSET32_END:
		case TAG_MEMSET64_END:
			{
				MARKUP   *element1, *element2;
				size_t   depth, numberOfArgs;
				VARIABLE *operand;
				TAG      functionTag;
				HANDLE   hDestProcess;
				PVOID    lpDest;
				uint64_t qwFill;
				size_t   nCount;
				BOOL     bSuccess;

				FIND_PARENTHESIS_OPEN(element1);
				if (element1 - 1 < lpMarkupArray)
					goto PARSING_ERROR;
				functionTag = lpMarkup->Tag - (TAG_MEMSET_END - TAG_MEMSET);
				if ((element1 - 1)->Tag != functionTag)
					goto PARSING_ERROR;
				operand = lpOperandTop;
				numberOfArgs = depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (IS_STRING_OPERAND(element2))
							goto PARSING_ERROR;
						if (++numberOfArgs == 1)
						{
							if (element2->Tag != TAG_PARAM_LOCAL)
							{
								if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
									goto FAILED7;
								hDestProcess = hProcess;
							}
							else
							{
								hDestProcess = NULL;
							}
							lpDest = IsInteger ? (PVOID)(uintptr_t)operand->Quad : (PVOID)(uintptr_t)operand->Real;
							operand++;
						}
						else if (numberOfArgs == 2)
						{
							qwFill = IsInteger ? (uint64_t)operand->Quad : (uint64_t)operand->Real;
							operand++;
						}
						else
						{
							nCount = IsInteger ? (size_t)operand->Quad : (size_t)operand->Real;
							break;
						}
					}
				while (++element1 != lpMarkup);
				if (numberOfArgs < 3)
					goto PARSING_ERROR;
				switch (functionTag)
				{
				default:
					bSuccess = FillProcessMemory(hDestProcess, lpDest, nCount, (uint8_t)qwFill);
					break;
				case TAG_MEMSET16:
					bSuccess = FillProcessMemory16(hDestProcess, lpDest, nCount, (uint16_t)qwFill);
					break;
				case TAG_MEMSET32:
					bSuccess = FillProcessMemory32(hDestProcess, lpDest, nCount, (uint32_t)qwFill);
					break;
				case TAG_MEMSET64:
					bSuccess = FillProcessMemory64(hDestProcess, lpDest, nCount, qwFill);
					break;
				}
				if (!bSuccess)
				{
					lpAddress = lpDest;
					goto WRITE_ERROR;
				}
			}
			break;
		case TAG_STRCPY_END:
			{
				MARKUP   *element1, *element2;
				size_t   depth, numberOfArgs;
				VARIABLE *operand;
				LPSTR    lpEndOfSrc;
				char     cUnterminated;
				LPSTR    lpBuffer;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nSize;

				FIND_PARENTHESIS_OPEN(element1);
				if (element1 - 1 < lpMarkupArray)
					goto PARSING_ERROR;
				if ((element1 - 1)->Tag != TAG_STRCPY)
					goto PARSING_ERROR;
				lpBuffer = lpEndOfSrc = NULL;
				operand = lpOperandTop;
				numberOfArgs = depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (++numberOfArgs == 1)
						{
							if (IS_STRING_OPERAND(element2))
								goto PARSING_ERROR;
							if (element2->Tag == TAG_PARAM_LOCAL)
							{
								hDestProcess = NULL;
							}
							else
							{
								if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
									goto FAILED7;
								hDestProcess = hProcess;
							}
							lpDest = IsInteger ? (LPSTR)(uintptr_t)operand->Quad : (LPSTR)(uintptr_t)operand->Real;
							operand++;
						}
						else if (numberOfArgs == 2)
						{
							size_t prefixLength;

							if (!CHECK_STRING_OPERAND(element2, &prefixLength))
							{
								if (element2->Tag == TAG_PARAM_LOCAL)
								{
									hSrcProcess = NULL;
								}
								else
								{
									if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
										goto FAILED7;
									hSrcProcess = hProcess;
								}
								lpSrc = IsInteger ? (LPCSTR)(uintptr_t)operand->Quad : (LPCSTR)(uintptr_t)operand->Real;
								operand++;
								nSize = StringLengthA(hSrcProcess, lpSrc) + 1;
							}
							else
							{
								size_t length;
								LPSTR  string;

								hSrcProcess = NULL;
								length = element2->Length - prefixLength - 1;
								string = element2->String + prefixLength + 1;
								if (length && string[length - 1] == '"')
									length--;
								if (prefixLength <= 1)
								{
									cUnterminated = *(lpEndOfSrc = (LPSTR)(lpSrc = string) + length);
									*lpEndOfSrc = '\0';
									nSize = length + 1;
								}
								else
								{
									nSize = (unsigned int)MultiByteToUtf8(CP_THREAD_ACP, 0, string, length, NULL, 0);
									if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
										goto FAILED8;
									MultiByteToUtf8(CP_THREAD_ACP, 0, string, length, lpBuffer, nSize);
									((LPSTR)lpSrc = lpBuffer)[nSize++] = '\0';
								}
							}
						}
						else
						{
							if (IS_STRING_OPERAND(element2))
								goto PARSING_ERROR;
							nSize = IsInteger ? (size_t)operand->Quad : (size_t)operand->Real;
							break;
						}
					}
				while (++element1 != lpMarkup);
				if (numberOfArgs < 2)
					goto PARSING_ERROR;
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (lpEndOfSrc)
					*lpEndOfSrc = cUnterminated;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (uint32_t)lpSrc);
						goto FAILED8;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
						goto WRITE_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_WCSCPY_END:
			{
				MARKUP   *element1, *element2;
				size_t   depth, numberOfArgs;
				VARIABLE *operand;
				LPWSTR   lpBuffer;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nSize;

				FIND_PARENTHESIS_OPEN(element1);
				if (element1 - 1 < lpMarkupArray)
					goto PARSING_ERROR;
				if ((element1 - 1)->Tag != TAG_WCSCPY)
					goto PARSING_ERROR;
				lpBuffer = NULL;
				operand = lpOperandTop;
				numberOfArgs = depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (++numberOfArgs == 1)
						{
							if (IS_STRING_OPERAND(element2))
								goto PARSING_ERROR;
							if (element2->Tag == TAG_PARAM_LOCAL)
							{
								hDestProcess = NULL;
							}
							else
							{
								if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
									goto FAILED7;
								hDestProcess = hProcess;
							}
							lpDest = IsInteger ? (LPWSTR)(uintptr_t)operand->Quad : (LPWSTR)(uintptr_t)operand->Real;
							operand++;
						}
						else
						{
							size_t prefixLength;

							if (!CHECK_STRING_OPERAND(element2, &prefixLength))
							{
								if (element2->Tag == TAG_PARAM_LOCAL)
								{
									hSrcProcess = NULL;
								}
								else
								{
									if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
										goto FAILED7;
									hSrcProcess = hProcess;
								}
								lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)operand->Quad : (LPCWSTR)(uintptr_t)operand->Real;
								operand++;
								nSize = (StringLengthW(hSrcProcess, lpSrc) + 1) * sizeof(wchar_t);
							}
							else
							{
								size_t length;
								LPSTR  string;

								hSrcProcess = NULL;
								length = element2->Length - prefixLength - 1;
								string = element2->String + prefixLength + 1;
								if (length && string[length - 1] == '"')
									length--;
								nSize = (unsigned int)MultiByteToWideChar(CP_THREAD_ACP, 0, string, length, NULL, 0);
								if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, ((size_t)nSize + 1) * sizeof(wchar_t))))
									goto FAILED8;
								MultiByteToWideChar(CP_THREAD_ACP, 0, string, length, lpBuffer, nSize);
								nSize *= sizeof(wchar_t);
								*(LPWSTR)((LPBYTE)(lpSrc = lpBuffer) + nSize) = L'\0';
								nSize += sizeof(wchar_t);
							}
							break;
						}
					}
				while (++element1 != lpMarkup);
				if (numberOfArgs < 2)
					goto PARSING_ERROR;
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (uint32_t)lpSrc);
						goto FAILED8;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
					WRITE_ERROR:
						TSSGActionListner_OnSubjectWriteError(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, (uint32_t)lpAddress);
						goto FAILED8;
					}
					goto PARSING_ERROR;
				}
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_REALLOC_END:
			{
				MARKUP   *element1, *element2;
				size_t   depth, numberOfArgs;
				VARIABLE *operand;
				uint64_t id;
				uint64_t size;
				LPVOID   address;

				FIND_PARENTHESIS_OPEN(element1);
				if (element1 - 1 < lpMarkupArray)
					goto PARSING_ERROR;
				if ((element1 - 1)->Tag != TAG_REALLOC)
					goto PARSING_ERROR;
				operand = lpOperandTop;
				numberOfArgs = depth = 0;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (IS_STRING_OPERAND(element2))
							goto PARSING_ERROR;
						if (++numberOfArgs == 1)
						{
							id = IsInteger ? (uint64_t)operand->Quad : (uint64_t)operand->Real;
							operand++;
						}
						else
						{
							size = IsInteger ? (uint64_t)operand->Quad : (uint64_t)operand->Real;
							break;
						}
					}
				while (++element1 != lpMarkup);
				if (numberOfArgs < 2)
					goto PARSING_ERROR;
				lpOperandTop->Quad = 0;
				address = NULL;
				if ((sizeof(size_t) == sizeof(uint32_t) ? !(size >> 32) && (uint32_t)size : size) && !(id >> 32))
				{
					FILETIME creationTime;

					creationTime.dwHighDateTime = creationTime.dwLowDateTime = 0;
					for (size_t j = 0; j < nNumberOfProcessMemory; j++)
					{
						size_t oldSize, newSize;

						if ((uint32_t)id != lpProcessMemory[j].Id)
							continue;
						address = lpProcessMemory[j].Address;
						oldSize = lpProcessMemory[j].Size;
						newSize = (size_t)size;
						if (lpProcessMemory[j].Protect)
						{
							oldSize = (oldSize + PAGE_SIZE - 1) & -(ptrdiff_t)PAGE_SIZE;
							newSize = (newSize + PAGE_SIZE - 1) & -(ptrdiff_t)PAGE_SIZE;
						}
						if (address && oldSize == newSize)
							break;
						if (lpProcessMemory[j].Protect)
						{
							DWORD protect;

							#define IsWritableProtect(Protect) ( \
							    ((Protect) & (                   \
							        PAGE_READWRITE         |     \
							        PAGE_WRITECOPY         |     \
							        PAGE_EXECUTE_READWRITE |     \
							        PAGE_EXECUTE_WRITECOPY)) &&  \
							    !((Protect) & (                  \
							        PAGE_NOACCESS          |     \
							        PAGE_GUARD)))

							#define IsReadableProtect(Protect) ( \
							    ((Protect) & (                   \
							        PAGE_READONLY          |     \
							        PAGE_READWRITE         |     \
							        PAGE_WRITECOPY         |     \
							        PAGE_EXECUTE_READ      |     \
							        PAGE_EXECUTE_READWRITE |     \
							        PAGE_EXECUTE_WRITECOPY)) &&  \
							    !((Protect) & (                  \
							        PAGE_NOACCESS          |     \
							        PAGE_GUARD)))

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto FAILED7;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED8;
								if (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime)
								{
									if (creationTime.dwLowDateTime != ftProcessCreationTime.dwLowDateTime ||
										creationTime.dwHighDateTime != ftProcessCreationTime.dwHighDateTime)
									{
										address = NULL;
										break;
									}
								}
								else
								{
									ftProcessCreationTime = creationTime;
								}
							}
							protect = lpProcessMemory[j].Protect;
							if (!IsWritableProtect(protect) && lpProcessMemory[j].Address)
								protect = PAGE_READWRITE;
							address = VirtualAllocEx(hProcess, NULL, newSize, MEM_COMMIT, protect);
							if (!address)
								break;
							if (lpProcessMemory[j].Address)
							{
								NTSTATUS status;

								if (!IsReadableProtect(lpProcessMemory[j].Protect))
									VirtualProtectEx(hProcess, lpProcessMemory[j].Address, oldSize, PAGE_READONLY, &protect);
								status = MoveProcessMemory(hProcess, address, hProcess, lpProcessMemory[j].Address, min(oldSize, newSize));
								if (NT_SUCCESS(status))
								{
									if (!IsWritableProtect(lpProcessMemory[j].Protect))
										VirtualProtectEx(hProcess, address, newSize, lpProcessMemory[j].Protect, &protect);
									VirtualFreeEx(hProcess, lpProcessMemory[j].Address, 0, MEM_RELEASE);
								}
								else
								{
									if (!IsReadableProtect(lpProcessMemory[j].Protect))
										VirtualProtectEx(hProcess, lpProcessMemory[j].Address, oldSize, lpProcessMemory[j].Protect, &protect);
									VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
									address = NULL;
									break;
								}
							}

							#undef IsWritableProtect
							#undef IsReadableProtect
						}
						else
						{
							address =
								lpProcessMemory[j].Address ?
									HeapReAlloc(pHeap, HEAP_ZERO_MEMORY, lpProcessMemory[j].Address, (size_t)size) :
									HeapAlloc(pHeap, HEAP_ZERO_MEMORY, (size_t)size);
							if (!address)
								break;
						}
						lpProcessMemory[j].Address = address;
						lpProcessMemory[j].Size = (size_t)size;
						break;
					}
				}
				lpOperandTop->Quad = (uint64_t)address;
			}
			if (IsInteger)
			{
				lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(uint32_t);
			}
			else
			{
				lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
#endif
#if USE_PLUGIN
		case TAG_PLUGIN_END:
			{
				MARKUP          *first, *element1, *element2;
				size_t          depth, stackSize, bufferSize;
				VARIABLE        *operand;
				PLUGIN_FUNCTION *function;
				uintptr_t       *stack;
				PARAM_TYPE      *paramType;

				FIND_PARENTHESIS_OPEN(first);
				if ((element1 = first - 1) < lpMarkupArray)
					goto PARSING_ERROR;
				if (element1->Tag != TAG_PLUGIN)
					goto PARSING_ERROR;
				function = element1->Function;
				paramType = function->ParamTypes;
				bufferSize = stackSize = 0;
#ifndef _WIN64
				operand = lpOperandTop;
#endif
				depth = 0;
				element1 = first;
				do
					if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
					{
						size_t prefixLength;

						if (element1->Type & (OS_OPEN | OS_CLOSE))
						{
							if (element1->Type & OS_CLOSE)
							{
								depth--;
								continue;
							}
							if (depth++)
							{
								continue;
							}
						}
						if (depth > 1)
							continue;
						element2 = element1;
						do
							if (++element2 >= lpMarkup)
								goto PARSING_ERROR;
						while (element2->Type & OS_OPEN);
						if (!CHECK_STRING_OPERAND(element2, &prefixLength))
						{
#ifndef _WIN64
							if (paramType != function->EndOfParamTypes)
							{
								stackSize +=
									*paramType != PARAM_QWORD && *paramType != PARAM_DOUBLE ?
										sizeof(uint32_t) :
										sizeof(uint64_t);
								paramType++;
							}
							else
							{
								stackSize +=
									!operand->IsQuad && IsInteger ?
										sizeof(uint32_t) :
										sizeof(uint64_t);
							}
#else
							if (paramType != function->EndOfParamTypes)
								paramType++;
							stackSize += sizeof(uintptr_t);
#endif
							operand++;
						}
						else if (paramType == function->EndOfParamTypes || *paramType != PARAM_STRING_W)
						{
							size_t length;
							LPSTR  string;

#ifndef _WIN64
							if (paramType != function->EndOfParamTypes)
							{
								stackSize +=
									*paramType != PARAM_QWORD && *paramType != PARAM_DOUBLE ?
										sizeof(uint32_t) :
										sizeof(uint64_t);
								paramType++;
							}
							else
							{
								stackSize += sizeof(LPSTR);
							}
#else
							if (paramType != function->EndOfParamTypes)
								paramType++;
							stackSize += sizeof(uintptr_t);
#endif
							length = element2->Length - prefixLength - 1;
							string = element2->String + prefixLength + 1;
							if (length && string[length - 1] == '"')
								length--;
							bufferSize += length + 1;
						}
						else
						{
							int   cchMultiByte, cchWideChar;
							LPSTR lpMultiByteStr;

							paramType++;
							stackSize += sizeof(LPWSTR);
							cchMultiByte = element2->Length - prefixLength - 1;
							lpMultiByteStr = element2->String + prefixLength + 1;
							if (cchMultiByte && lpMultiByteStr[cchMultiByte - 1] == '"')
								cchMultiByte--;
							cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cchMultiByte, NULL, 0);
							bufferSize += ((size_t)cchWideChar + 1) * sizeof(wchar_t);
						}
					}
				while (++element1 != lpMarkup);
				if (stackSize < function->StackSize)
					stackSize = function->StackSize;
				if (bufferSize += stackSize)
				{
					uintptr_t *param;
					LPSTR     psz;

					stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, bufferSize);
					if (!stack)
						goto FAILED8;
					param = stack;
					psz = (LPSTR)stack + stackSize;
					paramType = function->ParamTypes;
					operand = lpOperandTop;
					depth = 0;
					element1 = first;
					do
						if (element1->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
						{
							size_t prefixLength;

							if (element1->Type & (OS_OPEN | OS_CLOSE))
							{
								if (element1->Type & OS_CLOSE)
								{
									depth--;
									continue;
								}
								if (depth++)
								{
									continue;
								}
							}
							if (depth > 1)
								continue;
							element2 = element1;
							do
								if (++element2 >= lpMarkup)
									goto PARSING_ERROR;
							while (element2->Type & OS_OPEN);
							if (!CHECK_STRING_OPERAND(element2, &prefixLength))
							{
								if (paramType != function->EndOfParamTypes)
								{
									switch (*(paramType++))
									{
									default:
#ifndef _WIN64
										*(param++) = IsInteger ? operand->Low : (uint32_t)operand->Real;
										break;
									case PARAM_QWORD:
#endif
										*(((uint64_t *)param)++) = IsInteger ? operand->Quad : (uint64_t)operand->Real;
										break;
									case PARAM_DOUBLE:
										*(((double *)param)++) = IsInteger ? (double)operand->Quad : operand->Real;
										break;
									}
								}
								else
								{
#ifndef _WIN64
									if (!operand->IsQuad && IsInteger)
										*(param++) = operand->Low;
									else
#endif
										*(((uint64_t *)param)++) = operand->Quad;
								}
								operand++;
							}
							else if (paramType == function->EndOfParamTypes || *paramType != PARAM_STRING_W)
							{
								size_t length;
								LPSTR  string;

								length = element2->Length - prefixLength - 1;
								string = element2->String + prefixLength + 1;
								if (length && string[length - 1] == '"')
									length--;
								memcpy(psz, string, length);
								psz[length] = '\0';
								if (paramType != function->EndOfParamTypes)
								{
									switch (*(paramType++))
									{
									default:
#ifndef _WIN64
										*(param++) = (uint32_t)psz;
										break;
									case PARAM_QWORD:
#endif
										*(((uint64_t *)param)++) = (uint64_t)psz;
										break;
									case PARAM_DOUBLE:
										*(((double *)param)++) = (uintptr_t)psz;
										break;
									}
								}
								else
								{
									*(param++) = (uintptr_t)psz;
								}
								psz += length + 1;
							}
							else
							{
								#define lpWideCharStr ((LPWSTR)psz)

								int   cchMultiByte, cchWideChar;
								LPSTR lpMultiByteStr;

								paramType++;
								cchMultiByte = element2->Length - prefixLength - 1;
								lpMultiByteStr = element2->String + prefixLength + 1;
								if (cchMultiByte && lpMultiByteStr[cchMultiByte - 1] == '"')
									cchMultiByte--;
								cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cchMultiByte, NULL, 0);
								if (cchWideChar)
									cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cchMultiByte, lpWideCharStr, cchWideChar);
								*(param++) = (uintptr_t)lpWideCharStr;
								lpWideCharStr[cchWideChar] = L'\0';
								lpWideCharStr += cchWideChar + 1;

								#undef lpWideCharStr
							}
						}
					while (++element1 != lpMarkup);
				}
				else
				{
					stack = NULL;
				}
				ParsingContinue = TRUE;
				if (function->ReturnType != RETURN_DOUBLE)
					lpOperandTop->Quad = CallPluginFunction(function->Address, stack, stackSize);
				else
					lpOperandTop->Real = ((double(__cdecl *)(const void *, const void *, size_t))CallPluginFunction)(function->Address, stack, stackSize);
				if (stack)
					HeapFree(hHeap, 0, stack);
				switch (function->ReturnType)
				{
				case RETURN_DWORD:
					if (!(lpOperandTop->IsQuad = !IsInteger))
						lpOperandTop->High = 0;
					else
						lpOperandTop->Real = lpOperandTop->Low;
					break;
				case RETURN_QWORD:
					if (!IsInteger)
						lpOperandTop->Real = (double)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
					break;
				case RETURN_DOUBLE:
					if (!(lpOperandTop->IsQuad = !IsInteger))
					{
						int32_t msw;

						msw = lpOperandTop->High;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = (msw >= 0)))
								if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
									lpOperandTop->High = 0;
					}
					break;
				default:
					OPERAND_CLEAR();
					break;
				}
				if (!ParsingContinue)
					goto FAILED8;
			}
			break;
#endif

		#undef IS_STRING_OPERAND
		#undef CHECK_STRING_OPERAND
		#undef FIND_PARENTHESIS_OPEN

		case TAG_ADD:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low += operand.Low;
				else
					lpOperandTop->Quad += operand.Quad;
			}
			else
			{
				lpOperandTop->Real += operand.Real;
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
				VARIABLE swap = OPERAND_POP();
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
			else
			{
				lpOperandTop->Real -= operand.Real;
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
			else
			{
				lpOperandTop->Real *= operand.Real;
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
					goto FAILED8;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (int32_t)lpOperandTop->Low / (int32_t)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) / (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
				VARIABLE swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED8;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low /= operand.Low;
				else
					lpOperandTop->Quad /= operand.Quad;
			}
			else
			{
				if (!operand.Real)
					goto FAILED8;
				lpOperandTop->Real /= operand.Real;
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
					goto FAILED8;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (int32_t)lpOperandTop->Low % (int32_t)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) % (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
				VARIABLE swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto FAILED8;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low %= operand.Low;
				else
					lpOperandTop->Quad %= operand.Quad;
			}
			else
			{
				if (!operand.Real)
					goto FAILED8;
				lpOperandTop->Real = fmod(lpOperandTop->Real, operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_NEG:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = -(int32_t)lpOperandTop->Low;
				else
					lpOperandTop->Quad = -(int64_t)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Real = -lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_SHL:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				VARIABLE swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = operand.Quad < sizeof(uint32_t) * 8 ? lpOperandTop->Low << operand.Low : 0;
				else
					lpOperandTop->Quad = operand.Quad < sizeof(uint64_t) * 8 ? lpOperandTop->Quad << operand.Low : 0;
			}
			else
			{
				lpOperandTop->Real = ldexp(lpOperandTop->Real, (int)operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_SAR:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				if (!lpOperandTop->IsQuad)
				{
					if (operand.Quad > sizeof(uint32_t) * 8)
						operand.Low = sizeof(uint32_t) * 8;
					lpOperandTop->Low = (int32_t)lpOperandTop->Low >> operand.Low;
				}
				else
				{
					if (operand.Quad > sizeof(uint64_t) * 8)
						operand.Low = sizeof(uint64_t) * 8;
					lpOperandTop->Quad = (int64_t)lpOperandTop->Quad >> operand.Low;
				}
				break;
			}
		case TAG_SHR:
			if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
			{
				operand = OPERAND_POP();
			}
			else
			{
				VARIABLE swap = OPERAND_POP();
				operand = *lpOperandTop;
				*lpOperandTop = swap;
			}
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = operand.Quad < sizeof(uint32_t) * 8 ? lpOperandTop->Low >> operand.Low : 0;
				else
					lpOperandTop->Quad = operand.Quad < sizeof(uint64_t) * 8 ? lpOperandTop->Quad >> operand.Low : 0;
			}
			else
			{
				lpOperandTop->Real = ldexp(lpOperandTop->Real, -(int)operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_ROL:
			operand = OPERAND_POP();
			if (!IsInteger)
			{
				int32_t msw;

				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = (msw >= 0)))
						if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
							lpOperandTop->High = 0;
				operand.Low = (int)operand.Real;
			}
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = _rotl(lpOperandTop->Low, operand.Low);
			else
				lpOperandTop->Quad = _rotl64(lpOperandTop->Quad, operand.Low);
			if (!IsInteger)
			{
				lpOperandTop->Real = (double)lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ROR:
			operand = OPERAND_POP();
			if (!IsInteger)
			{
				int32_t msw;

				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = (msw >= 0)))
						if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
							lpOperandTop->High = 0;
				operand.Low = (int)operand.Real;
			}
			if (!lpOperandTop->IsQuad)
				lpOperandTop->Low = _rotr(lpOperandTop->Low, operand.Low);
			else
				lpOperandTop->Quad = _rotr64(lpOperandTop->Quad, operand.Low);
			if (!IsInteger)
			{
				lpOperandTop->Real = (double)lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_BIT_AND:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad &= operand.Quad;
				lpOperandTop->IsQuad |= operand.IsQuad;
			}
			else
			{
				lpOperandTop->Real = fband(lpOperandTop->Real, operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_OR:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad |= operand.Quad;
				lpOperandTop->IsQuad |= operand.IsQuad;
			}
			else
			{
				lpOperandTop->Real = fbor(lpOperandTop->Real, operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_XOR:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad ^= operand.Quad;
				lpOperandTop->IsQuad |= operand.IsQuad;
			}
			else
			{
				lpOperandTop->Real = fbxor(lpOperandTop->Real, operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				i -= 2;
			break;
		case TAG_BIT_NOT:
			if (IsInteger)
			{
				lpOperandTop->Low = ~lpOperandTop->Low;
				if (lpOperandTop->IsQuad)
					lpOperandTop->High = ~lpOperandTop->High;
			}
			else
			{
				lpOperandTop->Real = fbnot32(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
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
			else
			{
				boolValue = !!lpOperandTop->Real;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Real = boolValue;
					lpOperandTop->IsQuad = TRUE;
				}
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
			else
			{
				boolValue = !!lpOperandTop->Real;
				if (!(lpMarkup->Type & OS_RET_OPERAND))
				{
					lpOperandTop->Real = boolValue;
					lpOperandTop->IsQuad = TRUE;
				}
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
			else
			{
				lpOperandTop->Real = !lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_EQ:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad == operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = lpOperandTop->Real == operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_NE:
			operand = OPERAND_POP();
			if (IsInteger)
			{
				lpOperandTop->Quad = lpOperandTop->Quad != operand.Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = lpOperandTop->Real != operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_LT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low < (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) < (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
			else
			{
				lpOperandTop->Real = lpOperandTop->Real < operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_GT:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low > (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) > (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
			else
			{
				lpOperandTop->Real = lpOperandTop->Real > operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_LE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low <= (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) <= (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
			else
			{
				lpOperandTop->Real = lpOperandTop->Real <= operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_GE:
			if (IsInteger)
			{
				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low >= (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) >= (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
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
			else
			{
				lpOperandTop->Real = lpOperandTop->Real >= operand.Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_TERNARY:
			operand = OPERAND_POP();
			if (IsInteger ? !operand.Quad : !operand.Real)
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
			goto PROCESS_MEMORY;
		case TAG_REMOTE_INTEGER1:
		case TAG_REMOTE_INTEGER2:
		case TAG_REMOTE_INTEGER3:
		case TAG_REMOTE_INTEGER4:
		case TAG_REMOTE_INTEGER5:
		case TAG_REMOTE_INTEGER6:
		case TAG_REMOTE_INTEGER7:
		case TAG_REMOTE_INTEGER8:
			nSize = lpMarkup->Tag - TAG_REMOTE_INTEGER1 + 1;
			goto PROCESS_MEMORY;
		case TAG_REMOTE_REAL4:
		case TAG_REMOTE_FLOAT4:
			nSize = 4;
			goto PROCESS_MEMORY;
		case TAG_REMOTE_REAL8:
			nSize = 8;
		PROCESS_MEMORY:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED7;
			if (bCompoundAssign)
			{
				VARIABLE op1 = OPERAND_POP();
				VARIABLE op2 = *lpOperandTop;
				*lpOperandTop = op1;
				OPERAND_PUSH(op2);
			}
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (!bCompoundAssign && (!(lpNext = i + 1 < nNumberOfPostfix ? lpPostfix[i + 1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
			{
				bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
				if (bCompoundAssign)
				{
					VARIABLE op1 = OPERAND_POP();
					VARIABLE op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
					OPERAND_PUSH(op2);
				}
				lpOperandTop->Quad = 0;
				if (!ReadProcessMemory(hProcess, lpAddress, &lpOperandTop->Quad, nSize, NULL))
					lpOperandTop->Quad = 0;
				switch (lpMarkup->Tag)
				{
#ifndef _WIN64
				case TAG_INDIRECTION:
#endif
				case TAG_REMOTE1:
				case TAG_REMOTE2:
				case TAG_REMOTE3:
				case TAG_REMOTE4:
					if (lpOperandTop->IsQuad = !IsInteger)
						lpOperandTop->Real = *(float *)&lpOperandTop->Low;
					break;
				case TAG_REMOTE_INTEGER1:
				case TAG_REMOTE_INTEGER2:
				case TAG_REMOTE_INTEGER3:
				case TAG_REMOTE_INTEGER4:
					if (lpOperandTop->IsQuad = !IsInteger)
						lpOperandTop->Real = lpOperandTop->Low;
					break;
				case TAG_REMOTE_INTEGER5:
				case TAG_REMOTE_INTEGER6:
				case TAG_REMOTE_INTEGER7:
				case TAG_REMOTE_INTEGER8:
					if (!IsInteger)
					{
						lpOperandTop->Real = (double)lpOperandTop->Quad;
						lpOperandTop->IsQuad = TRUE;
					}
					break;
				case TAG_REMOTE_REAL4:
					if (!IsInteger)
					{
						lpOperandTop->Real = *(float *)&lpOperandTop->Low;
						lpOperandTop->IsQuad = TRUE;
					}
					else
					{
						int32_t msw;

						msw = lpOperandTop->Low;
						lpOperandTop->Quad = (uint64_t)*(float *)&lpOperandTop->Low;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = (msw >= 0)))
								if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
									lpOperandTop->High = 0;
					}
					break;
				case TAG_REMOTE_REAL8:
					if (IsInteger)
					{
						int32_t msw;

						msw = lpOperandTop->High;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = (msw >= 0)))
								if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
									lpOperandTop->High = 0;
					}
					break;
				case TAG_REMOTE_FLOAT4:
					lpOperandTop->Real = *(float *)&lpOperandTop->Low;
				default:
					lpOperandTop->IsQuad = TRUE;
					break;
				}
			}
			else
			{
				uint64_t qw;

				OPERAND_POP();
				qw = lpOperandTop->Quad;
				switch (lpMarkup->Tag)
				{
				case TAG_REMOTE_INTEGER1:
				case TAG_REMOTE_INTEGER2:
				case TAG_REMOTE_INTEGER3:
				case TAG_REMOTE_INTEGER4:
					if (!IsInteger)
						*(uint32_t *)&qw = (uint32_t)*(double *)&qw;
					break;
				case TAG_REMOTE_INTEGER5:
				case TAG_REMOTE_INTEGER6:
				case TAG_REMOTE_INTEGER7:
				case TAG_REMOTE_INTEGER8:
					if (!IsInteger)
						qw = (uint64_t)*(double *)&qw;
					break;
				case TAG_REMOTE_REAL4:
					*(float *)&qw = IsInteger ? (float)qw : (float)*(double *)&qw;
					break;
				case TAG_REMOTE_REAL8:
					if (IsInteger)
						*(double *)&qw = (double)qw;
					break;
				case TAG_REMOTE_FLOAT4:
					if (!IsInteger)
						*(float *)&qw = (float)*(double *)&qw;
					break;
				}
				if (!WriteProcessMemory(hProcess, lpAddress, &qw, nSize, NULL))
					goto WRITE_ERROR;
				i++;
				if (bCompoundAssign)
				{
					bCompoundAssign = FALSE;
					continue;
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
			nSize = lpMarkup->Tag - TAG_LOCAL1 + 1;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_INTEGER1:
		case TAG_LOCAL_INTEGER2:
		case TAG_LOCAL_INTEGER3:
		case TAG_LOCAL_INTEGER4:
		case TAG_LOCAL_INTEGER5:
		case TAG_LOCAL_INTEGER6:
		case TAG_LOCAL_INTEGER7:
		case TAG_LOCAL_INTEGER8:
			nSize = lpMarkup->Tag - TAG_LOCAL_INTEGER1 + 1;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_REAL4:
		case TAG_LOCAL_FLOAT4:
			nSize = 4;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_REAL8:
			nSize = 8;
		LOCAL_MEMORY:
			if (bCompoundAssign)
			{
				VARIABLE op1 = OPERAND_POP();
				VARIABLE op2 = *lpOperandTop;
				*lpOperandTop = op1;
				OPERAND_PUSH(op2);
			}
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (!bCompoundAssign && (!(lpNext = i + 1 < nNumberOfPostfix ? lpPostfix[i + 1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
			{
				bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
				if (bCompoundAssign)
				{
					VARIABLE op1 = OPERAND_POP();
					VARIABLE op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
					OPERAND_PUSH(op2);
				}
				if (!IsBadReadPtr(lpAddress, nSize))
				{
					switch (nSize)
					{
					case 1:
						lpOperandTop->Quad = *(uint8_t *)lpAddress;
						break;
					case 2:
						lpOperandTop->Quad = *(uint16_t *)lpAddress;
						break;
					case 3:
						lpOperandTop->Quad = *(uint16_t *)lpAddress | ((uint32_t)*((uint8_t *)lpAddress + 2) << 16);
						break;
					case 4:
						lpOperandTop->Quad = *(uint32_t *)lpAddress;
						break;
					case 5:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint8_t *)lpAddress + 4) << 32);
						break;
					case 6:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint16_t *)lpAddress + 2) << 32);
						break;
					case 7:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint16_t *)lpAddress + 2) << 32) | ((uint64_t)*((uint8_t *)lpAddress + 6) << 48);
						break;
					case 8:
						lpOperandTop->Quad = *(uint64_t *)lpAddress;
						break;
					}
					switch (lpMarkup->Tag)
					{
					case TAG_LOCAL1:
					case TAG_LOCAL2:
					case TAG_LOCAL3:
					case TAG_LOCAL4:
						if (lpOperandTop->IsQuad = !IsInteger)
							lpOperandTop->Real = *(float *)&lpOperandTop->Low;
						break;
					case TAG_LOCAL_INTEGER1:
					case TAG_LOCAL_INTEGER2:
					case TAG_LOCAL_INTEGER3:
					case TAG_LOCAL_INTEGER4:
						if (lpOperandTop->IsQuad = !IsInteger)
							lpOperandTop->Real = lpOperandTop->Low;
						break;
					case TAG_LOCAL_INTEGER5:
					case TAG_LOCAL_INTEGER6:
					case TAG_LOCAL_INTEGER7:
					case TAG_LOCAL_INTEGER8:
						if (!IsInteger)
							lpOperandTop->Real = (double)lpOperandTop->Quad;
						lpOperandTop->IsQuad = TRUE;
						break;
					case TAG_LOCAL_REAL4:
						if (!IsInteger)
						{
							lpOperandTop->Real = *(float *)&lpOperandTop->Low;
							lpOperandTop->IsQuad = TRUE;
						}
						else
						{
							int32_t msw;

							msw = lpOperandTop->Low;
							lpOperandTop->Quad = (uint64_t)*(float *)&lpOperandTop->Low;
							if (lpOperandTop->IsQuad = lpOperandTop->High)
								if (!(lpOperandTop->IsQuad = (msw >= 0)))
									if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
										lpOperandTop->High = 0;
						}
						break;
					case TAG_LOCAL_REAL8:
						if (IsInteger)
						{
							int32_t msw;

							msw = lpOperandTop->High;
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
							if (lpOperandTop->IsQuad = lpOperandTop->High)
								if (!(lpOperandTop->IsQuad = (msw >= 0)))
									if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
										lpOperandTop->High = 0;
						}
						break;
					case TAG_LOCAL_FLOAT4:
						lpOperandTop->Real = *(float *)&lpOperandTop->Low;
					default:
						lpOperandTop->IsQuad = TRUE;
						break;
					}
				}
				else
				{
					lpOperandTop->Quad = 0;
					lpOperandTop->IsQuad = nSize > sizeof(uint32_t);
				}
			}
			else
			{
				uint64_t qw;

				if (IsBadWritePtr(lpAddress, nSize))
					goto WRITE_ERROR;
				OPERAND_POP();
				qw = lpOperandTop->Quad;
				switch (lpMarkup->Tag)
				{
				case TAG_LOCAL_INTEGER1:
				case TAG_LOCAL_INTEGER2:
				case TAG_LOCAL_INTEGER3:
				case TAG_LOCAL_INTEGER4:
					if (!IsInteger)
						*(uint32_t *)&qw = (uint32_t)*(double *)&qw;
					break;
				case TAG_LOCAL_INTEGER5:
				case TAG_LOCAL_INTEGER6:
				case TAG_LOCAL_INTEGER7:
				case TAG_LOCAL_INTEGER8:
					if (!IsInteger)
						qw = (uint64_t)*(double *)&qw;
					break;
				case TAG_LOCAL_REAL4:
					*(float *)&qw = IsInteger ? (float)qw : (float)*(double *)&qw;
					break;
				case TAG_LOCAL_REAL8:
					if (IsInteger)
						*(double *)&qw = (double)qw;
				case TAG_LOCAL_FLOAT4:
					if (!IsInteger)
						*(float *)&qw = (float)*(double *)&qw;
					break;
				}
				switch (nSize)
				{
				case 1:
					*(uint8_t *)lpAddress = (uint8_t)qw;
					break;
				case 2:
					*(uint16_t *)lpAddress = (uint16_t)qw;
					break;
				case 3:
					*(uint16_t *)lpAddress = (uint16_t)qw;
					*((uint8_t *)lpAddress + 2) = (uint8_t)((uint32_t)qw >> 16);
					break;
				case 4:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					break;
				case 5:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint8_t *)lpAddress + 4) = (uint8_t)(qw >> 32);
					break;
				case 6:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint16_t *)lpAddress + 2) = (uint16_t)(qw >> 32);
					break;
				case 7:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint16_t *)lpAddress + 2) = (uint16_t)(qw >> 32);
					*((uint8_t *)lpAddress + 6) = (uint8_t)((uint32_t)(qw >> 32) >> 16);
					break;
				case 8:
					*(uint64_t *)lpAddress = qw;
					break;
				}
				i++;
				if (bCompoundAssign)
				{
					bCompoundAssign = FALSE;
					continue;
				}
			}
			break;
#endif
		case TAG_REV_ENDIAN2:
			lpOperandTop->Quad = __intrinsic_bswap16((uint16_t)lpOperandTop->Low);
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
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN6:
			lpOperandTop->Quad = __intrinsic_bswap48(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN7:
			lpOperandTop->Quad = __intrinsic_bswap56(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN8:
			lpOperandTop->Quad = __intrinsic_bswap64(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_ADDR_REPLACE:
			if (!IsInteger)
				lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, (unsigned long *)&lpOperandTop->Quad, atREPLACE) != 0)
				goto FAILED8;
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = *(uintptr_t *)&lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ADDR_ADJUST:
			if (!IsInteger)
				lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
			if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, (unsigned long *)&lpOperandTop->Quad, atADJUST) != 0)
				goto FAILED8;
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = *(uintptr_t *)&lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_LEFT_ASSIGN:
			if (i + 1 < nNumberOfPostfix)
			{
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
						lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
						if (!lpMem)
							goto FAILED8;
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
				goto FAILED7;
			if (IsInteger)
			{
				lpOperandTop->Quad = StringLengthA(hProcess, (LPCSTR)lpOperandTop->Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(uint32_t);
			}
			else
			{
				lpOperandTop->Real = StringLengthA(hProcess, (LPCSTR)(size_t)lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_WCSLEN:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED7;
			if (IsInteger)
			{
				lpOperandTop->Quad = StringLengthW(hProcess, (LPCWSTR)lpOperandTop->Quad);
				lpOperandTop->IsQuad = sizeof(size_t) > sizeof(uint32_t);
			}
			else
			{
				lpOperandTop->Real = StringLengthW(hProcess, (LPCWSTR)(size_t)lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_BSF:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
				{
					if (!_BitScanForward(&lpOperandTop->Low, lpOperandTop->Low))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				else
				{
					if (!_BitScanForward64(&lpOperandTop->Low, lpOperandTop->Quad))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				int32_t exp;

				if ((lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) && (exp = (lpOperandTop->High & 0x7FF00000)) != 0x7FF00000)
				{
					unsigned long bits;

					_BitScanForward64(&bits, lpOperandTop->Quad);
					lpOperandTop->Real = (exp >> 20) - (1023 + 52) + (bits <= 52 ? (int32_t)bits : 52);
				}
				else
				{
					lpOperandTop->Real = -1;
				}
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_BSR:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad || !IsInteger)
				{
					if (!_BitScanReverse(&lpOperandTop->Low, lpOperandTop->Low))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				else
				{
					if (!_BitScanReverse64(&lpOperandTop->Low, lpOperandTop->Quad))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				uint64_t x;
				int32_t  exp;

				if ((x = lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) && (exp = (lpOperandTop->High & 0x7FF00000)) != 0x7FF00000)
				{
					if (exp >>= 20)
					{
						lpOperandTop->Real = exp - 1023;
					}
					else
					{
						unsigned long bits;

						_BitScanReverse64(&bits, x);
						lpOperandTop->Real = exp - (1023 + 52) + (int32_t)bits;
					}
				}
				else
				{
					lpOperandTop->Real = -1;
				}
				lpOperandTop->IsQuad = TRUE;
			}
		case TAG_CAST32:
			if (IsInteger)
			{
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = (float)lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_CAST64:
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_I1TOI4:
			lpOperandTop->Quad = (int32_t)(int8_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I2TOI4:
			lpOperandTop->Quad = (int32_t)(int16_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I4TOI8:
			lpOperandTop->Quad = (int64_t)(int32_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_UTOF:
			lpOperandTop->Real = (double)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_ITOF:
			lpOperandTop->Real = lpOperandTop->IsQuad ? (double)(int64_t)lpOperandTop->Quad : (double)(int32_t)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_FTOI:
			{
				int32_t msw;

				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = (msw >= 0)))
						if (!(lpOperandTop->IsQuad = !!~lpOperandTop->High))
							lpOperandTop->High = 0;
			}
			break;
		case TAG_TRUNC:
			if (!IsInteger)
			{
				lpOperandTop->Real = trunc(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ROUND:
			if (!IsInteger)
			{
				lpOperandTop->Real = round(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_MEMORY:
			{
				LPVOID lpAddress;

				lpAddress = NULL;
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (!lpOperandTop->High)
				{
					FILETIME creationTime;

					creationTime.dwHighDateTime = creationTime.dwLowDateTime = 0;
					for (size_t j = 0; j < nNumberOfProcessMemory; j++)
					{
						if (lpOperandTop->Low != lpProcessMemory[j].Id)
							continue;
						if (lpProcessMemory[j].Address)
						{
							lpAddress = lpProcessMemory[j].Address;
							break;
						}
						if (lpProcessMemory[j].Protect)
						{
							if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto FAILED7;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED8;
								if (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime)
								{
									if (creationTime.dwLowDateTime != ftProcessCreationTime.dwLowDateTime ||
										creationTime.dwHighDateTime != ftProcessCreationTime.dwHighDateTime)
									{
										break;
									}
								}
								else
								{
									ftProcessCreationTime = creationTime;
								}
							}
							lpProcessMemory[j].Address = lpAddress = VirtualAllocEx(hProcess, NULL, lpProcessMemory[j].Size, MEM_COMMIT, lpProcessMemory[j].Protect);
						}
						else
						{
							lpProcessMemory[j].Address = lpAddress = HeapAlloc(pHeap, HEAP_ZERO_MEMORY, lpProcessMemory[j].Size);
						}
						break;
					}
				}
				lpOperandTop->Quad = (uint64_t)lpAddress;
			}
			if (IsInteger)
			{
				lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(uint32_t);
			}
			else
			{
				lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
#endif
		case TAG_PROCEDURE:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if (lpMarkup[1].Priority <= lpMarkup->Priority)
				break;
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED7;
			operand = OPERAND_POP();
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!operand.High && IS_INTRESOURCE(operand.Low))
			{
				lpOperandTop->Quad = (uint64_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, (LPSTR)operand.Quad);
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
				}
				else
				{
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
			}
			break;
		case TAG_MODULENAME:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if (lpMarkup[1].Priority <= lpMarkup->Priority)
				break;
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto FAILED7;
			operand = OPERAND_POP();
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
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
				lpOperandTop->Quad = (uint64_t)lpFunction;
				lpMarkup->String[lpMarkup->Length] = c;
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
				}
				else
				{
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
			}
			break;
		case TAG_HNUMBER:
			if (lpMarkup + 1 == lpMarkupArray + nNumberOfMarkup)
				break;
			if (lpMarkup[1].Priority <= lpMarkup->Priority)
				break;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!lpOperandTop->High)
			{
				THeapListData *HeapL;

				HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, lpOperandTop->Low - 1);
				lpOperandTop->Low = HeapL ? HeapL->heapListAddress : 0;
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(uint32_t);
				}
				else
				{
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			else
			{
				lpOperandTop->Quad = 0;
				if (IsInteger)
					lpOperandTop->IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(uint32_t);
			}
			break;
		case TAG_NOT_OPERATOR:
			{
				size_t          length;
				LPSTR           p, end;
				MARKUP_VARIABLE *element;
				char            *endptr;
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
					c = *end;
					*end = '\0';
					if (IsInteger)
					{
						operand.Quad = _strtoui64(p, &endptr, 0);
						operand.IsQuad = !!operand.High;
					}
					else
					{
						operand.Real = strtod(p, &endptr);
						operand.IsQuad = TRUE;
					}
					*end = c;
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
				if (!element && length && (p[0] == SCOPE_PREFIX || lpNext && (lpNext->Tag == TAG_ADDRESS_OF ||
																			  lpNext->Tag == TAG_INC ||
																			  lpNext->Tag == TAG_DEC)))
				{
					if (!(nNumberOfVariable & 0x0F))
					{
						LPVOID lpMem;
						size_t nBytes;

						nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
						lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
						if (!lpMem)
							goto FAILED8;
						lpVariable = (MARKUP_VARIABLE *)lpMem;
					}
					element = lpVariable + nNumberOfVariable++;
					element->Length = length;
					element->String = p;
					element->Value.Quad = 0;
					element->Value.IsQuad = FALSE;
#if SCOPE_SUPPORT
					if (attributes && element->String[0] == SCOPE_PREFIX)
					{
						TScopeAttribute *scope = NULL;
						uint32_t key = HashBytes(element->String + 1, element->Length - 1);
						for (TSSGAttributeElement **pos = vector_end(attributes);// enum is signed integer ...
							 --pos >= (TSSGAttributeElement **)vector_begin(attributes) && (*pos)->type >= atSCOPE; )
						{
							if ((*pos)->type == atSCOPE)
							{
								scope = (TScopeAttribute *)*pos;
#if !defined(__BORLANDC__)
								map_iterator it = map_find(&scope->heapMap, &key);
								if (it != map_end(&scope->heapMap))
								{
									element->Value.Quad = *(uint64_t *)&it->first[sizeof(key)];
									element->Value.IsQuad = !IsInteger || !!element->Value.High;
									element->It = it;
									break;
								}
#else
								map<unsigned long, pair<unsigned long, unsigned long> >::iterator
									it = scope->heapMap.find(key);
								if (it != scope->heapMap.end())
								{
									element->Value.Low = it->second.first;
									element->Value.High = it->second.second;
									element->Value.IsQuad = !IsInteger || !!element->Value.High;
									element->It = it;
									break;
								}
#endif
							}
						}
						if (scope && !element->It) {
#if !defined(__BORLANDC__)
							heapMapValue val = { key, element->Value.Low, element->Value.High };
							map_insert(&element->It, &scope->heapMap, map_lower_bound(&scope->heapMap, &val.key), &val);
#else
							element->It = scope->heapMap.insert(make_pair(key, make_pair(element->Value.Low, element->Value.High))).first;
#endif
						}
					}
#endif
				}
				switch (lpNext ? lpNext->Tag : ~0)
				{
#if SCOPE_SUPPORT
				case TAG_ADDRESS_OF:
					if (!element)
						break;
#if !defined(__BORLANDC__)
					operand.Quad = element->It ? (uint64_t)&element->It->first[sizeof(uint32_t)] : (uint64_t)&element->Value.Low;
#else
					operand.Quad = element->It ? (uint64_t)&element->It->second : (uint64_t)&element->Value.Low;
#endif
					operand.IsQuad = sizeof(void*) > sizeof(uint32_t);
					OPERAND_PUSH(operand);
					i++;
					if (!TSSGCtrl_GetSSGActionListner(SSGCtrl))
						continue;
					lpGuideText = "& アドレス取得";
#if !defined(__BORLANDC__)
					nGuideTextLength = 14;
#endif
					goto OUTPUT_GUIDE;
#endif
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
							operand.Real = element->Value.Real += 1;
							operand.IsQuad = TRUE;
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
							operand.Real = element->Value.Real;
							operand.IsQuad = TRUE;
							element->Value.Real += 1;
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
							operand.Real = element->Value.Real -= 1;
							operand.IsQuad = TRUE;
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
							operand.Real = element->Value.Real;
							operand.IsQuad = TRUE;
							element->Value.Real -= 1;
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
							lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
							if (!lpMem)
								goto FAILED8;
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
						operand.Quad = lpme ? (uint64_t)lpme->hModule : 0;
						operand.IsQuad = sizeof(HMODULE) > sizeof(uint32_t);
						OPERAND_PUSH(operand);
					}
					i++;
					break;
				case TAG_PROCESSID:
					{
						LPMODULEENTRY32A lpme;

						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpme = TProcessCtrl_GetModuleFromName(&SSGCtrl->processCtrl, lpMarkup->String);
						lpMarkup->String[lpMarkup->Length] = c;
						operand.Quad = lpme ? (uint64_t)lpme->th32ProcessID : 0;
						operand.IsQuad = FALSE;
						OPERAND_PUSH(operand);
					}
					i++;
					break;
				case TAG_PROCEDURE:
					if ((HMODULE)lpOperandTop->Quad)
					{
						LPSTR lpProcName;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED7;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
							operand.Quad = (uint64_t)operand.Real;
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpOperandTop->Quad = (uint64_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpProcName);
						lpMarkup->String[lpMarkup->Length] = c;
						if (IsInteger)
						{
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
						}
						else
						{
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
							lpOperandTop->IsQuad = TRUE;
						}
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
							goto FAILED7;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
							operand.Quad = (uint64_t)operand.Real;
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						c2 = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpModuleName, lpProcName);
						if (lpFunction && lpNext->Tag == TAG_IMPORT_FUNCTION)
							if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
								lpFunction = NULL;
						lpOperandTop->Quad = (uint64_t)lpFunction;
						lpMarkup->String[lpMarkup->Length] = c2;
						if (lpEndOfModuleName)
							*lpEndOfModuleName = c;
						if (IsInteger)
						{
							lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
						}
						else
						{
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
							lpOperandTop->IsQuad = TRUE;
						}
					}
					i++;
					break;
				case TAG_SECTION:
					if ((HMODULE)lpOperandTop->Quad)
					{
						BOOL  IsEndOfSection;
						DWORD dwSectionSize;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto FAILED7;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						IsEndOfSection = *(uint16_t *)lpNext->String == BSWAP16(':+');
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						lpOperandTop->Quad = (uint64_t)GetSectionAddress(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, IsEndOfSection ? &dwSectionSize : NULL);
						if (IsEndOfSection)
							lpOperandTop->Quad += dwSectionSize;
						if (IsInteger)
						{
							lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(uint32_t);
						}
						else
						{
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
							lpOperandTop->IsQuad = TRUE;
						}
						lpMarkup->String[lpMarkup->Length] = c;
					}
					i++;
					break;
				case TAG_HNUMBER:
					if (endptr != end)
						goto PARSING_ERROR;
					if (!IsInteger)
						operand.Quad = (uint64_t)operand.Real;
					if (operand.High)
						goto PARSING_ERROR;
					{
						THeapListData *HeapL;

						HeapL = TProcessCtrl_GetHeapList(&SSGCtrl->processCtrl, operand.Low - 1);
						if (HeapL)
							operand.Quad = HeapL->heapListAddress;
					}
					if (IsInteger)
					{
						operand.IsQuad = sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(uint32_t);
					}
					else
					{
						operand.Real = (size_t)operand.Quad;
						operand.IsQuad = TRUE;
					}
					OPERAND_PUSH(operand);
					i++;
					break;
				default:
					if (bCompoundAssign)
						goto ASSIGN;
					if (lpNext && lpNext->Tag != TAG_LEFT_ASSIGN && (lpNext->Type & OS_LEFT_ASSIGN))
						bCompoundAssign = TRUE;
					if (element)
						OPERAND_PUSH(element->Value);
					else if (endptr == end)
						OPERAND_PUSH(operand);
					else
						OPERAND_PUSH(operandZero);
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
			goto FAILED8;
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
				TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(SSGCtrl), SSGS, lpGuideText, lpOperandTop->Real);
			if (lpGuideText == lpMarkup->String)
				lpMarkup->String[lpMarkup->Length] = c;
#else
			lpGuideText = lpMarkup->String;
			nGuideTextLength = lpMarkup->Length;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(lpGuideText, nGuideTextLength, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(lpGuideText, nGuideTextLength, lpOperandTop->Real);
#endif
		}
		if (lpMarkup->Tag == TAG_RETURN)
			break;
	}
	qwResult = lpOperandTop->Quad;
FAILED8:
	if (hProcess)
		CloseHandle(hProcess);
FAILED7:
#if SCOPE_SUPPORT
	for (size_t i = 0; i < nNumberOfVariable; i++)
	{
		register PMARKUP_VARIABLE v = &lpVariable[i];
		if (v->It)
		{
#if !defined(__BORLANDC__)
			*(uint64_t*)&v->It->first[sizeof(uint32_t)] = v->Value.Quad;
#else
			v->It->second = make_pair(v->Value.Low, v->Value.High);
#endif
		}
	}
#endif
	if (TSSGCtrl_GetSSGActionListner(SSGCtrl) && TMainForm_GetUserMode(MainForm) >= 3 &&
		nNumberOfProcessMemory && !HeapValidate(pHeap, 0, NULL)) {
#if USE_TOOLTIP
		extern BOOL bActive;
		if (!bActive)
			ShowToolTip("Failed to HeapValidate.", (HICON)TTI_ERROR);
#endif
	}
#if REPEAT_INDEX
	if (lpVariableStringBuffer)
		HeapFree(hHeap, 0, lpVariableStringBuffer);
FAILED6:
#endif
	HeapFree(hHeap, 0, lpVariable);
FAILED5:
	HeapFree(hHeap, 0, lpOperandBuffer);
FAILED4:
	HeapFree(hHeap, 0, lpPostfix);
FAILED3:
	HeapFree(hHeap, 0, lpMarkupArray);
FAILED2:
	HeapFree(hHeap, 0, lpszSrc);
FAILED1:
	return qwResult;

	#undef PROCESS_DESIRED_ACCESS
	#undef SCOPE_PREFIX
	#undef OPERAND_IS_EMPTY
	#undef OPERAND_PUSH
	#undef OPERAND_POP
	#undef OPERAND_CLEAR
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
	uint64_t Result;
	va_list  ArgPtr;

#if defined(__BORLANDC__)
	va_start(ArgPtr, SSGS);
	va_arg(ArgPtr, LPCVOID);
#else
	va_start(ArgPtr, Src);
#endif
	Result = InternalParsing(this, SSGS, Src, TRUE, ArgPtr);
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
		uint64_t Quad;
		double   Real;
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
	Result.Quad = InternalParsing(this, SSGS, Src, FALSE, (va_list)&Param);

	return Result.Real;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------

#undef IMPLEMENTED

#if defined(__BORLANDC__)
#undef _ultoa
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

#undef atADJUST
#undef atREPLACE
#undef atVARIABLE
#undef atDEFINE

#undef OS_PUSH
#undef OS_OPEN
#undef OS_CLOSE
#undef OS_SPLIT
#undef OS_DELIMITER
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

#undef AllocMarkup

