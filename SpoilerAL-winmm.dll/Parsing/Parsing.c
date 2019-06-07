#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#if !defined(__BORLANDC__)
#include <stdint.h>
#include <float.h>
#include <math.h>
#else
#include <stdint.h>
#undef INTPTR_MIN
#undef INTPTR_MAX
#undef UINTPTR_MAX
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#ifdef _WIN64
#define INTPTR_MIN  _I64_MIN
#define INTPTR_MAX  _I64_MAX
#define UINTPTR_MAX _UI64_MAX
#else
#define INTPTR_MIN  INT_MIN
#define INTPTR_MAX  INT_MAX
#define UINTPTR_MAX UINT_MAX
#endif
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIZE_MAX    UINTPTR_MAX
#include <float.h>
#include <math.h>
#define trunc(x) ((x) >= 0 ? floor(x) : ceil(x))
#define round(x) floor((x) + 0.5)
#endif
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <mbstring.h>
#include <fbit.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "CompareProcessMemory.h"
#include "MoveProcessMemory.h"
#include "FillProcessMemory.h"
#include "StringLength.h"
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

#ifndef __BORLANDC__
#define __msreturn
#define __msfastcall __fastcall
#endif

#define IMPLEMENTED 0

#define ULL2DBL_LOST_BIT (64 - DBL_MANT_DIG)
#define ULL2DBL_LOST_MAX ((1 << ULL2DBL_LOST_BIT) - 1)

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
#define TMainForm_Guide(Mes, Flags)                                                    (MainForm)->Guide(Mes, Flags)
#define TSSGCtrl_GetAttribute(SSGCtrl, SSGS, Type)                                     (SSGCtrl)->GetAttribute(SSGS, Type)
#define TSSGCtrl_GetSSGActionListner(SSGCtrl)                                          (SSGCtrl)->GetSSGActionListner()
#define TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, Address, Mode)                  (SSGCtrl)->AddressAttributeFilter(SSGS, Address, Mode)
#define TSSGSubject_GetAttribute(SSGS)                                                 (SSGS)->GetAttribute()
#define TSSGSubject_GetSize(SSGS)                                                      (SSGS)->GetSize()
#define TSSGActionListner_OnProcessOpenError(SSGActionListner, SSGS)                   (SSGActionListner)->OnProcessOpenError(SSGS)
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
#include "TStringDivision.h"

#ifdef _MSC_VER
EXTERN_C int __fastcall internal_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr, const va_list endarg);
EXTERN_C int __fastcall internal_vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr, const va_list endarg);
#else
#define internal_vsnprintf(buffer, count, format, argptr, endarg) _vsnprintf(buffer, count, format, argptr)
#define internal_vsnwprintf(buffer, count, format, argptr, endarg) _vsnwprintf(buffer, count, format, argptr)
#endif
EXTERN_C char * __cdecl _strichr(const char *string, int c);
EXTERN_C wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c);
EXTERN_C unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
EXTERN_C char * __cdecl _strrichr(const char *string, int c);
EXTERN_C wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c);
EXTERN_C unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c);
EXTERN_C unsigned char * __cdecl _stristr(const unsigned char *string1, const unsigned char *string2);
EXTERN_C wchar_t * __cdecl _wcsistr(const wchar_t *string1, const wchar_t *string2);
EXTERN_C unsigned char * __cdecl _mbsistr(const unsigned char *string1, const unsigned char *string2);
EXTERN_C char *__fastcall internal_strtok(char *string, const char *delimiter, char **context);
EXTERN_C wchar_t *__fastcall internal_wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context);
EXTERN_C unsigned char *__fastcall internal_mbstok(unsigned char *string, const unsigned char *delimiter, unsigned char **context);
EXTERN_C uint32_t __cdecl rand32();
EXTERN_C uint64_t __cdecl rand64();
EXTERN_C float __cdecl randf32();
EXTERN_C double __cdecl randf64();

EXTERN_C void __stdcall Wait(DWORD dwMilliseconds);
EXTERN_C char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate);
EXTERN_C wchar_t * __fastcall UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate);
EXTERN_C unsigned char * __fastcall UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate);
EXTERN_C __int64 __fastcall UnescapeAnsiCharA(const char **pfirst, const char *last);
EXTERN_C unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last);
EXTERN_C unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last);
EXTERN_C int __cdecl GuidePrint(const char *format, ...);
EXTERN_C int __fastcall GuidePrintV(const char *format, va_list argptr, const va_list endarg);
EXTERN_C int __cdecl DebugPrint(const char *format, ...);
EXTERN_C int __fastcall DebugPrintV(const char *format, va_list argptr, const va_list endarg);
EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName);
EXTERN_C FARPROC * __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName);
EXTERN_C LPVOID __stdcall GetSectionAddress(HANDLE hProcess, HMODULE hModule, LPCSTR lpSectionName, LPDWORD lpdwSectionSize);

#if defined(_MSC_VER)
#if REPEAT_INDEX
#include "SubjectProperty\SSGSubjectProperty.h"
#endif
#pragma function(strlen)
extern HANDLE hHeap;
extern HANDLE pHeap;
#endif

#include "HashBytes.h"

#define OS_PUSH          0x00000001
#define OS_OPEN          0x00000002
#define OS_CLOSE         0x00000004
#define OS_SPLIT         0x00000008
#define OS_DELIMITER     0x00000010
#define OS_MONADIC       0x00000020
#define OS_POST          0x00000040
#define OS_SHORT_CIRCUIT 0x00000080
#define OS_LEFT_ASSIGN   0x00000100
#define OS_PARENTHESIS   0x00000200
#define OS_HAS_EXPR      0x00000400
#define OS_TERNARY       0x00000800
#define OS_TERNARY_END   0x00001000
#define OS_LOOP_BEGIN    0x00002000
#define OS_LOOP_END      0x00004000
#define OS_RET_OPERAND   0x00008000
#define OS_STRING        0x00010000

/*
 [Wikipedia] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/%E6%BC%94%E7%AE%97%E5%AD%90%E3%81%AE%E5%84%AA%E5%85%88%E9%A0%86%E4%BD%8D#.E3.83.97.E3.83.AD.E3.82.B0.E3.83.A9.E3.83.9F.E3.83.B3.E3.82.B0.E8.A8.80.E8.AA.9E

 [Microsoft Developer Network] - [優先順位と評価順序] - [C 演算子の優先順位と結合規則]
 https://msdn.microsoft.com/ja-jp/library/2bxt6kc4.aspx

 [Wikipedia] - [CとC++の演算子] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/C%E3%81%A8C%2B%2B%E3%81%AE%E6%BC%94%E7%AE%97%E5%AD%90#.E6.BC.94.E7.AE.97.E5.AD.90.E3.81.AE.E5.84.AA.E5.85.88.E9.A0.86.E4.BD.8D

 127 if                                                          OS_PUSH | OS_HAS_EXPR
 127 else                                                        OS_PUSH
 127 do                                                          OS_PUSH | OS_LOOP_BEGIN
 127 while                                                       OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_HAS_EXPR | OS_POST)
 127 for                                                         OS_PUSH | OS_HAS_EXPR
 127 break                                                       OS_PUSH
 127 continue                                                    OS_PUSH
#if IMPLEMENTED
 127 call                                                        OS_PUSH
 127 co_await                                                    OS_PUSH
 127 co_yield                                                    OS_PUSH
 127 co_return                                                   OS_PUSH
 127 switch                                                      OS_PUSH | OS_HAS_EXPR
 127 case                                                        OS_PUSH
 127 default                                                     OS_PUSH
 127 goto                                                        OS_PUSH
#endif
  64 (                                                           OS_OPEN | OS_PARENTHESIS
  64 [_                                                          OS_OPEN
  64 [.                                                          OS_OPEN
  64 [~                                                          OS_OPEN
  64 [:                                                          OS_OPEN
  64 ++ --                                                       OS_PUSH | OS_MONADIC | OS_POST 後置インクリメント 後置デクリメント
  60                                                             OS_PUSH | OS_MONADIC
  60 parse_int      parse_real     parse_reset                   OS_PUSH | OS_MONADIC
     MName
     ProcessId
     HNumber
     Memory
     Cast32         Cast64
     I1toI4         I2toI4         I4toI8
     utof           itof           ftoi
     trunc          round
     BitScanForward BitScanReverse
     A2U            A2W
     U2A            U2W
     W2A            W2U
     wait
     sleep
#if ALLOCATE_SUPPORT
     realloc
#endif
     alloca
     atoi           wtoi
     atof           wtof
     rand32         rand64
     min            max            imin           imax
     memcmp
     memmove
     memset         memset16       memset32       memset64
     printf         dprintf
     snprintf       snwprintf
     strdup         wcsdup
     strlen         wcslen
     strnlen        wcsnlen
     strcmp         wcscmp
     strncmp        wcsncmp
     stricmp        wcsicmp        mbsicmp
     strnicmp       wcsnicmp       mbsnbicmp
     strcpy         wcscpy
     strcat         wcscat
     strlcpy        wcslcpy
     strlcat        wcslcat
     strchr         wcschr         mbschr
     strichr        wcsichr        mbsichr
     strrchr        wcsrchr        mbsrchr
     strrichr       wcsrichr       mbsrichr
     strstr         wcsstr         mbsstr
     stristr        wcsistr        mbsistr
     strspn         wcsspn         mbsspn
     strcspn        wcscspn        mbscspn
     strpbrk        wcspbrk        mbspbrk
     strset         wcsset         mbsset
     strnset        wcsnset        mbsnbset
     strtok         wcstok         mbstok
     strlwr         wcslwr         mbslwr
     strupr         wcsupr         mbsupr
     strrev         wcsrev         mbsrev
     isalnum        isalpha        isascii        isblank
     iscntrl        iscsym         iscsymf        isdigit
     isgraph        iskana         isleadbyte     islower
     ismbchira      ismbckata      ismbcl0        ismbcl1
     ismbcl2        ismbclegal     ismbcsymbol    isprint
     ispunct        isspace        istrailbyte    isupper
     isxdigit
     toascii        tolower        toupper
  56 ++ -- - ! ~ * &                                             OS_PUSH | OS_MONADIC           前置インクリメント 前置デクリメント 単項マイナス 論理否定 ビットごとの論理否定 間接演算子
  52 * / % idiv imod                                             OS_PUSH                        乗算 除算 剰余算 符号付除算 符号付剰余算
  48 + -                                                         OS_PUSH                        加算 減算
  44 << >> sar rol ror                                           OS_PUSH                        左論理シフト 右論理シフト 右算術シフト 左ローテート 右ローテート
  40 < > <= >= lt gt le ge                                       OS_PUSH                        小なり 大なり 以下 以上 符号付小なり 符号付大なり 符号付以下 符号付以上
  36 == !=                                                       OS_PUSH                        等価 非等価
  32 &                                                           OS_PUSH                        ビットごとの論理積
  28 ^                                                           OS_PUSH                        ビットごとの排他的論理和
  24 |                                                           OS_PUSH                        ビットごとの論理和
  20 && and                                                      OS_PUSH | OS_SHORT_CIRCUIT     論理積
  16 || or                                                       OS_PUSH | OS_SHORT_CIRCUIT     論理和
  12 ? :                                                         OS_PUSH | OS_TERNARY           条件演算子
   8 =>                                                          OS_PUSH                        右辺代入
   8 = += -= *= /= %= &= |= ^= <<= >>=                           OS_PUSH | OS_LEFT_ASSIGN       左辺代入 加算代入 減算代入 乗算代入 除算代入 剰余代入 ビット積代入 ビット排他的論理和代入 ビット和代入 左論理シフト代入 右論理シフト代入
   4 ,                                                           OS_DELIMITER
   4 :]   :8]   :7]   :6]   :5]   :4]   :3]   :2]   :1]          OS_PUSH | OS_CLOSE
     :I]  :I8]  :I7]  :I6]  :I5]  :I4]  :I3]  :I2]  :I1]
     :R]  :R4]  :R8]
     :F]  :F4]  :F8]
     :L]  :L8]  :L7]  :L6]  :L5]
     :L4] :L3]  :L2]  :L1]
     :LI] :LI8] :LI7] :LI6] :LI5] :LI4] :LI3] :LI2] :LI1]
     :LR] :LR4] :LR8]
     :LF] :LF4] :LF8]
   4 ~]   ~8]   ~7]   ~6]   ~5]   ~4]   ~3]   ~2]                OS_PUSH | OS_CLOSE
   4 .]                                                          OS_PUSH | OS_CLOSE
   4 _]                                                          OS_PUSH | OS_CLOSE
   4 )                                                           OS_CLOSE | OS_PARENTHESIS
   4 ;                                                           OS_SPLIT
   0 return                                                      OS_PUSH
*/

typedef enum {
	TAG_NOT_OPERATOR     ,  // 127                 OS_PUSH
	TAG_IF               ,  // 127 if              OS_PUSH | OS_HAS_EXPR
	TAG_ELSE             ,  // 127 else            OS_PUSH
	TAG_DO               ,  // 127 do              OS_PUSH | OS_LOOP_BEGIN
	TAG_WHILE            ,  // 127 while           OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_HAS_EXPR | OS_POST)
	TAG_FOR              ,  // 127 for             OS_PUSH | OS_HAS_EXPR
	TAG_BREAK            ,  // 127 break           OS_PUSH
	TAG_CONTINUE         ,  // 127 continue        OS_PUSH
	TAG_PARAM_LOCAL      ,  // 127 L               OS_PUSH
#if IMPLEMENTED
	TAG_CALL             ,  // 127 call            OS_PUSH
	TAG_CO_AWAIT         ,  // 127 co_await        OS_PUSH
	TAG_CO_YIELD         ,  // 127 co_yield        OS_PUSH
	TAG_CO_RETURN        ,  // 127 co_return       OS_PUSH
	TAG_SWITCH           ,  // 127 switch          OS_PUSH | OS_HAS_EXPR
	TAG_CASE             ,  // 127 case            OS_PUSH
	TAG_DEFAULT          ,  // 127 default         OS_PUSH
	TAG_GOTO             ,  // 127 goto            OS_PUSH
	TAG_LABEL            ,  // 127                 OS_PUSH
#endif
	TAG_PARENTHESIS_OPEN ,  //  64 (               OS_OPEN | OS_PARENTHESIS
	TAG_ADDR_ADJUST_OPEN ,  //  64 [_              OS_OPEN
	TAG_ADDR_REPLACE_OPEN,  //  64 [.              OS_OPEN
	TAG_REV_ENDIAN_OPEN  ,  //  64 [~              OS_OPEN
	TAG_REMOTE_OPEN      ,  //  64 [:              OS_OPEN
	TAG_INC              ,  //  64 N++  (52 ++N)   OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_DEC              ,  //  64 N--  (52 --N)   OS_PUSH | OS_MONADIC | OS_POST (OS_PUSH | OS_MONADIC)
	TAG_PARSE_INT        ,  //  60 parse_int       OS_PUSH | OS_MONADIC
	TAG_PARSE_REAL       ,  //  60 parse_real      OS_PUSH | OS_MONADIC
	TAG_PARSE_RESET      ,  //  60 parse_reset     OS_PUSH | OS_MONADIC
	TAG_MNAME            ,  //  60 MName           OS_PUSH | OS_MONADIC
	TAG_PROCEDURE        ,  //  60 ::              OS_PUSH
	TAG_IMPORT_FUNCTION  ,  //  60 :!              OS_PUSH
	TAG_IMPORT_REFERENCE ,  //  60 :&              OS_PUSH
	TAG_MODULENAME       ,  //  60                 OS_PUSH
	TAG_SECTION          ,  //  60 := :+           OS_PUSH
	TAG_PROCESSID        ,  //  60 ProcessId       OS_PUSH | OS_MONADIC
	TAG_HNUMBER          ,  //  60 HNumber         OS_PUSH | OS_MONADIC
	TAG_MEMORY           ,  //  60 Memory          OS_PUSH | OS_MONADIC
	TAG_CAST32           ,  //  60 Cast32          OS_PUSH | OS_MONADIC
	TAG_CAST64           ,  //  60 Cast64          OS_PUSH | OS_MONADIC
	TAG_I1TOI4           ,  //  60 I1toI4          OS_PUSH | OS_MONADIC
	TAG_I2TOI4           ,  //  60 I2toI4          OS_PUSH | OS_MONADIC
	TAG_I4TOI8           ,  //  60 I4toI8          OS_PUSH | OS_MONADIC
	TAG_UTOF             ,  //  60 utof            OS_PUSH | OS_MONADIC
	TAG_ITOF             ,  //  60 itof            OS_PUSH | OS_MONADIC
	TAG_FTOI             ,  //  60 ftoi            OS_PUSH | OS_MONADIC
	TAG_TRUNC            ,  //  60 trunc           OS_PUSH | OS_MONADIC
	TAG_ROUND            ,  //  60 round           OS_PUSH | OS_MONADIC
	TAG_BSF              ,  //  60 BitScanForward  OS_PUSH | OS_MONADIC
	TAG_BSR              ,  //  60 BitScanReverse  OS_PUSH | OS_MONADIC
	TAG_A2U              ,  //  60 A2U             OS_PUSH | OS_MONADIC
	TAG_A2W              ,  //  60 A2W             OS_PUSH | OS_MONADIC
	TAG_U2A              ,  //  60 U2A             OS_PUSH | OS_MONADIC
	TAG_U2W              ,  //  60 U2W             OS_PUSH | OS_MONADIC
	TAG_W2A              ,  //  60 W2A             OS_PUSH | OS_MONADIC
	TAG_W2U              ,  //  60 W2U             OS_PUSH | OS_MONADIC
	TAG_WAIT             ,  //  60 wait            OS_PUSH | OS_MONADIC
	TAG_SLEEP            ,  //  60 sleep           OS_PUSH | OS_MONADIC
#if ALLOCATE_SUPPORT
	TAG_REALLOC          ,  //  60 realloc         OS_PUSH | OS_MONADIC
#endif
	TAG_ALLOCA           ,  //  60 alloca          OS_PUSH | OS_MONADIC
	TAG_ATOI             ,  //  60 atoi            OS_PUSH | OS_MONADIC
	TAG_WTOI             ,  //  60 wtoi            OS_PUSH | OS_MONADIC
	TAG_ATOF             ,  //  60 atof            OS_PUSH | OS_MONADIC
	TAG_WTOF             ,  //  60 wtof            OS_PUSH | OS_MONADIC
	TAG_RAND32           ,  //  60 rand32          OS_PUSH | OS_MONADIC
	TAG_RAND64           ,  //  60 rand64          OS_PUSH | OS_MONADIC
	TAG_MIN              ,  //  60 min             OS_PUSH | OS_MONADIC
	TAG_MAX              ,  //  60 max             OS_PUSH | OS_MONADIC
	TAG_IMIN             ,  //  60 imin            OS_PUSH | OS_MONADIC
	TAG_IMAX             ,  //  60 imax            OS_PUSH | OS_MONADIC
	TAG_MEMCMP           ,  //  60 memcmp          OS_PUSH | OS_MONADIC
	TAG_MEMMOVE          ,  //  60 memmove         OS_PUSH | OS_MONADIC
	TAG_MEMSET           ,  //  60 memset          OS_PUSH | OS_MONADIC
	TAG_MEMSET16         ,  //  60 memset16        OS_PUSH | OS_MONADIC
	TAG_MEMSET32         ,  //  60 memset32        OS_PUSH | OS_MONADIC
	TAG_MEMSET64         ,  //  60 memset64        OS_PUSH | OS_MONADIC
	TAG_PRINTF           ,  //  60 printf          OS_PUSH | OS_MONADIC
	TAG_DPRINTF          ,  //  60 dprintf         OS_PUSH | OS_MONADIC
	TAG_SNPRINTF         ,  //  60 snprintf        OS_PUSH | OS_MONADIC
	TAG_SNWPRINTF        ,  //  60 snwprintf       OS_PUSH | OS_MONADIC
	TAG_STRDUP           ,  //  60 strdup          OS_PUSH | OS_MONADIC
	TAG_WCSDUP           ,  //  60 wcsdup          OS_PUSH | OS_MONADIC
	TAG_STRLEN           ,  //  60 strlen          OS_PUSH | OS_MONADIC
	TAG_WCSLEN           ,  //  60 wcslen          OS_PUSH | OS_MONADIC
	TAG_STRNLEN          ,  //  60 strnlen         OS_PUSH | OS_MONADIC
	TAG_WCSNLEN          ,  //  60 wcsnlen         OS_PUSH | OS_MONADIC
	TAG_STRCMP           ,  //  60 strcmp          OS_PUSH | OS_MONADIC
	TAG_WCSCMP           ,  //  60 wcscmp          OS_PUSH | OS_MONADIC
	TAG_STRICMP          ,  //  60 stricmp         OS_PUSH | OS_MONADIC
	TAG_WCSICMP          ,  //  60 wcsicmp         OS_PUSH | OS_MONADIC
	TAG_MBSICMP          ,  //  60 mbsicmp         OS_PUSH | OS_MONADIC
	TAG_STRNCMP          ,  //  60 strncmp         OS_PUSH | OS_MONADIC
	TAG_WCSNCMP          ,  //  60 wcsncmp         OS_PUSH | OS_MONADIC
	TAG_STRNICMP         ,  //  60 strnicmp        OS_PUSH | OS_MONADIC
	TAG_WCSNICMP         ,  //  60 wcsnicmp        OS_PUSH | OS_MONADIC
	TAG_MBSNBICMP        ,  //  60 mbsnbicmp       OS_PUSH | OS_MONADIC
	TAG_STRCPY           ,  //  60 strcpy          OS_PUSH | OS_MONADIC
	TAG_WCSCPY           ,  //  60 wcscpy          OS_PUSH | OS_MONADIC
	TAG_STRCAT           ,  //  60 strcat          OS_PUSH | OS_MONADIC
	TAG_WCSCAT           ,  //  60 wcscat          OS_PUSH | OS_MONADIC
	TAG_STRLCPY          ,  //  60 strlcpy         OS_PUSH | OS_MONADIC
	TAG_WCSLCPY          ,  //  60 wcslcpy         OS_PUSH | OS_MONADIC
	TAG_STRLCAT          ,  //  60 strlcat         OS_PUSH | OS_MONADIC
	TAG_WCSLCAT          ,  //  60 wcslcat         OS_PUSH | OS_MONADIC
	TAG_STRCHR           ,  //  60 strchr          OS_PUSH | OS_MONADIC
	TAG_WCSCHR           ,  //  60 wcschr          OS_PUSH | OS_MONADIC
	TAG_MBSCHR           ,  //  60 mbschr          OS_PUSH | OS_MONADIC
	TAG_STRICHR          ,  //  60 strichr         OS_PUSH | OS_MONADIC
	TAG_WCSICHR          ,  //  60 wcsichr         OS_PUSH | OS_MONADIC
	TAG_MBSICHR          ,  //  60 mbsichr         OS_PUSH | OS_MONADIC
	TAG_STRRCHR          ,  //  60 strrchr         OS_PUSH | OS_MONADIC
	TAG_WCSRCHR          ,  //  60 wcsrchr         OS_PUSH | OS_MONADIC
	TAG_MBSRCHR          ,  //  60 mbsrchr         OS_PUSH | OS_MONADIC
	TAG_STRRICHR         ,  //  60 strrichr        OS_PUSH | OS_MONADIC
	TAG_WCSRICHR         ,  //  60 wcsrichr        OS_PUSH | OS_MONADIC
	TAG_MBSRICHR         ,  //  60 mbsrichr        OS_PUSH | OS_MONADIC
	TAG_STRSTR           ,  //  60 strstr          OS_PUSH | OS_MONADIC
	TAG_WCSSTR           ,  //  60 wcsstr          OS_PUSH | OS_MONADIC
	TAG_MBSSTR           ,  //  60 mbsstr          OS_PUSH | OS_MONADIC
	TAG_STRISTR          ,  //  60 stristr         OS_PUSH | OS_MONADIC
	TAG_WCSISTR          ,  //  60 wcsistr         OS_PUSH | OS_MONADIC
	TAG_MBSISTR          ,  //  60 mbsistr         OS_PUSH | OS_MONADIC
	TAG_STRSPN           ,  //  60 strspn          OS_PUSH | OS_MONADIC
	TAG_WCSSPN           ,  //  60 wcsspn          OS_PUSH | OS_MONADIC
	TAG_MBSSPN           ,  //  60 mbsspn          OS_PUSH | OS_MONADIC
	TAG_STRCSPN          ,  //  60 strcspn         OS_PUSH | OS_MONADIC
	TAG_WCSCSPN          ,  //  60 wcscspn         OS_PUSH | OS_MONADIC
	TAG_MBSCSPN          ,  //  60 mbscspn         OS_PUSH | OS_MONADIC
	TAG_STRPBRK          ,  //  60 strpbrk         OS_PUSH | OS_MONADIC
	TAG_WCSPBRK          ,  //  60 wcspbrk         OS_PUSH | OS_MONADIC
	TAG_MBSPBRK          ,  //  60 mbspbrk         OS_PUSH | OS_MONADIC
	TAG_STRSET           ,  //  60 strset          OS_PUSH | OS_MONADIC
	TAG_WCSSET           ,  //  60 wcsset          OS_PUSH | OS_MONADIC
	TAG_MBSSET           ,  //  60 mbsset          OS_PUSH | OS_MONADIC
	TAG_STRNSET          ,  //  60 strnset         OS_PUSH | OS_MONADIC
	TAG_WCSNSET          ,  //  60 wcsnset         OS_PUSH | OS_MONADIC
	TAG_MBSNBSET         ,  //  60 mbsnbset        OS_PUSH | OS_MONADIC
	TAG_STRTOK           ,  //  60 strtok          OS_PUSH | OS_MONADIC
	TAG_WCSTOK           ,  //  60 wcstok          OS_PUSH | OS_MONADIC
	TAG_MBSTOK           ,  //  60 mbstok          OS_PUSH | OS_MONADIC
	TAG_STRLWR           ,  //  60 strlwr          OS_PUSH | OS_MONADIC
	TAG_WCSLWR           ,  //  60 wcslwr          OS_PUSH | OS_MONADIC
	TAG_MBSLWR           ,  //  60 mbslwr          OS_PUSH | OS_MONADIC
	TAG_STRUPR           ,  //  60 strupr          OS_PUSH | OS_MONADIC
	TAG_WCSUPR           ,  //  60 wcsupr          OS_PUSH | OS_MONADIC
	TAG_MBSUPR           ,  //  60 mbsupr          OS_PUSH | OS_MONADIC
	TAG_STRREV           ,  //  60 strrev          OS_PUSH | OS_MONADIC
	TAG_WCSREV           ,  //  60 wcsrev          OS_PUSH | OS_MONADIC
	TAG_MBSREV           ,  //  60 mbsrev          OS_PUSH | OS_MONADIC
	TAG_ISALNUM          ,  //  60 isalnum         OS_PUSH | OS_MONADIC
	TAG_ISALPHA          ,  //  60 isalpha         OS_PUSH | OS_MONADIC
	TAG_ISASCII          ,  //  60 isascii         OS_PUSH | OS_MONADIC
	TAG_ISBLANK          ,  //  60 isblank         OS_PUSH | OS_MONADIC
	TAG_ISCNTRL          ,  //  60 iscntrl         OS_PUSH | OS_MONADIC
	TAG_ISCSYM           ,  //  60 iscsym          OS_PUSH | OS_MONADIC
	TAG_ISCSYMF          ,  //  60 iscsymf         OS_PUSH | OS_MONADIC
	TAG_ISDIGIT          ,  //  60 isdigit         OS_PUSH | OS_MONADIC
	TAG_ISGRAPH          ,  //  60 isgraph         OS_PUSH | OS_MONADIC
	TAG_ISKANA           ,  //  60 iskana          OS_PUSH | OS_MONADIC
	TAG_ISLEADBYTE       ,  //  60 isleadbyte      OS_PUSH | OS_MONADIC
	TAG_ISLOWER          ,  //  60 islower         OS_PUSH | OS_MONADIC
	TAG_ISMBCHIRA        ,  //  60 ismbchira       OS_PUSH | OS_MONADIC
	TAG_ISMBCKATA        ,  //  60 ismbckata       OS_PUSH | OS_MONADIC
	TAG_ISMBCL0          ,  //  60 ismbcl0         OS_PUSH | OS_MONADIC
	TAG_ISMBCL1          ,  //  60 ismbcl1         OS_PUSH | OS_MONADIC
	TAG_ISMBCL2          ,  //  60 ismbcl2         OS_PUSH | OS_MONADIC
	TAG_ISMBCLEGAL       ,  //  60 ismbclegal      OS_PUSH | OS_MONADIC
	TAG_ISMBCSYMBOL      ,  //  60 ismbcsymbol     OS_PUSH | OS_MONADIC
	TAG_ISPRINT          ,  //  60 isprint         OS_PUSH | OS_MONADIC
	TAG_ISPUNCT          ,  //  60 ispunct         OS_PUSH | OS_MONADIC
	TAG_ISSPACE          ,  //  60 isspace         OS_PUSH | OS_MONADIC
	TAG_ISTRAILBYTE      ,  //  60 istrailbyte     OS_PUSH | OS_MONADIC
	TAG_ISUPPER          ,  //  60 isupper         OS_PUSH | OS_MONADIC
	TAG_ISXDIGIT         ,  //  60 isxdigit        OS_PUSH | OS_MONADIC
	TAG_TOASCII          ,  //  60 toascii         OS_PUSH | OS_MONADIC
	TAG_TOLOWER          ,  //  60 tolower         OS_PUSH | OS_MONADIC
	TAG_TOUPPER          ,  //  60 toupper         OS_PUSH | OS_MONADIC
#if USE_PLUGIN
	TAG_PLUGIN           ,  //  60                 OS_PUSH | OS_MONADIC
#endif
	TAG_NEG              ,  //  56 -               OS_PUSH | OS_MONADIC
	TAG_NOT              ,  //  56 !               OS_PUSH | OS_MONADIC
	TAG_BIT_NOT          ,  //  56 ~               OS_PUSH | OS_MONADIC
	TAG_INDIRECTION      ,  //  56 *               OS_PUSH | OS_MONADIC
	TAG_ADDRESS_OF       ,  //  56 &               OS_PUSH | OS_MONADIC
	TAG_MUL              ,  //  52 *    ( 8 *= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_DIV              ,  //  52 /    ( 8 /= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_MOD              ,  //  52 %    ( 8 %= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_IDIV             ,  //  52 idiv            OS_PUSH
	TAG_IMOD             ,  //  52 imod            OS_PUSH
	TAG_ADD              ,  //  48 +    ( 8 += )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SUB              ,  //  48 -    ( 8 -= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHL              ,  //  44 <<   ( 8 <<=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHR              ,  //  44 >>   ( 8 >>=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SAR              ,  //  44 sar             OS_PUSH
	TAG_ROL              ,  //  44 rol             OS_PUSH
	TAG_ROR              ,  //  44 ror             OS_PUSH
	TAG_BT               ,  //  40 <               OS_PUSH
	TAG_AT               ,  //  40 >               OS_PUSH
	TAG_BE               ,  //  40 <=              OS_PUSH
	TAG_AE               ,  //  40 >=              OS_PUSH
	TAG_LT               ,  //  40 lt              OS_PUSH
	TAG_GT               ,  //  40 gt              OS_PUSH
	TAG_LE               ,  //  40 le              OS_PUSH
	TAG_GE               ,  //  40 ge              OS_PUSH
	TAG_EQ               ,  //  36 ==              OS_PUSH
	TAG_NE               ,  //  36 !=              OS_PUSH
	TAG_BIT_AND          ,  //  32 &    ( 8 &= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_XOR              ,  //  28 ^    ( 8 ^= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_BIT_OR           ,  //  24 |    ( 8 |= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_AND              ,  //  20 &&   (20 and)   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_OR               ,  //  16 ||   (16 or )   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_TERNARY          ,  //  12 ?               OS_PUSH | OS_TERNARY
	TAG_TERNARY_SPLIT    ,  //  12 :               OS_PUSH | OS_TERNARY
	TAG_RIGHT_ASSIGN     ,  //   8 =>              OS_PUSH
	TAG_LEFT_ASSIGN      ,  //   8 =               OS_PUSH | OS_LEFT_ASSIGN
	TAG_DELIMITER        ,  //   4 ,               OS_DELIMITER
	TAG_REMOTE1          ,  //   4 :1]             OS_PUSH | OS_CLOSE
	TAG_REMOTE2          ,  //   4 :2]             OS_PUSH | OS_CLOSE
	TAG_REMOTE3          ,  //   4 :3]             OS_PUSH | OS_CLOSE
	TAG_REMOTE4          ,  //   4 :] :4]          OS_PUSH | OS_CLOSE
	TAG_REMOTE5          ,  //   4 :5]             OS_PUSH | OS_CLOSE
	TAG_REMOTE6          ,  //   4 :6]             OS_PUSH | OS_CLOSE
	TAG_REMOTE7          ,  //   4 :7]             OS_PUSH | OS_CLOSE
	TAG_REMOTE8          ,  //   4 :8]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER1  ,  //   4 :I1]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER2  ,  //   4 :I2]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER3  ,  //   4 :I3]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER4  ,  //   4 :I] :I4]        OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER5  ,  //   4 :I5]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER6  ,  //   4 :I6]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER7  ,  //   4 :I7]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER8  ,  //   4 :I8] :F8]       OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL4     ,  //   4 :R] :R4]        OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL8     ,  //   4 :R8]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_FLOAT4    ,  //   4 :F] :F4]        OS_PUSH | OS_CLOSE
	TAG_LOCAL1           ,  //   4 :L1]            OS_PUSH | OS_CLOSE
	TAG_LOCAL2           ,  //   4 :L2]            OS_PUSH | OS_CLOSE
	TAG_LOCAL3           ,  //   4 :L3]            OS_PUSH | OS_CLOSE
	TAG_LOCAL4           ,  //   4 :L] :L4]        OS_PUSH | OS_CLOSE
	TAG_LOCAL5           ,  //   4 :L5]            OS_PUSH | OS_CLOSE
	TAG_LOCAL6           ,  //   4 :L6]            OS_PUSH | OS_CLOSE
	TAG_LOCAL7           ,  //   4 :L7]            OS_PUSH | OS_CLOSE
	TAG_LOCAL8           ,  //   4 :L8] :LF8]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER1   ,  //   4 :LI1]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER2   ,  //   4 :LI2]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER3   ,  //   4 :LI3]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER4   ,  //   4 :LI] :LI4]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER5   ,  //   4 :LI5]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER6   ,  //   4 :LI6]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER7   ,  //   4 :LI7]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER8   ,  //   4 :LI8]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL4      ,  //   4 :LR] :LR4]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL8      ,  //   4 :LR8]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_FLOAT4     ,  //   4 :LF] :LF4]      OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN2      ,  //   4 ~2]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN3      ,  //   4 ~3]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN4      ,  //   4 ~] ~4]          OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN5      ,  //   4 ~5]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN6      ,  //   4 ~6]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN7      ,  //   4 ~7]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN8      ,  //   4 ~8]             OS_PUSH | OS_CLOSE
	TAG_ADDR_REPLACE     ,  //   4 .]              OS_PUSH | OS_CLOSE
	TAG_ADDR_ADJUST      ,  //   4 _]              OS_PUSH | OS_CLOSE
	TAG_IF_EXPR          ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESIS
	TAG_WHILE_EXPR       ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESIS
	TAG_FOR_INITIALIZE   ,  //   4 ;               OS_PUSH | OS_SPLIT
	TAG_FOR_CONDITION    ,  //   4 ;               OS_PUSH | OS_SPLIT | OS_LOOP_BEGIN
	TAG_FOR_UPDATE       ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESIS
	TAG_PARENTHESIS_CLOSE,  //   4 )               OS_CLOSE | OS_PARENTHESIS
	TAG_SPLIT            ,  //   4 ;               OS_SPLIT
	TAG_RETURN           ,  //   0 return          OS_PUSH
	TAG_PARSE_ERROR      ,
} TAG;

typedef enum {
	PRIORITY_NOT_OPERATOR      = 127,   //                 OS_PUSH
	PRIORITY_IF                = 127,   // if              OS_PUSH | OS_HAS_EXPR
	PRIORITY_ELSE              = 127,   // else            OS_PUSH
	PRIORITY_DO                = 127,   // do              OS_PUSH
	PRIORITY_WHILE             = 127,   // while           OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_POST)
	PRIORITY_FOR               = 127,   // for             OS_PUSH | OS_HAS_EXPR
	PRIORITY_BREAK             = 127,   // break           OS_PUSH
	PRIORITY_CONTINUE          = 127,   // continue        OS_PUSH
	PRIORITY_PARAM_LOCAL       = 127,   // L               OS_PUSH
#if IMPLEMENTED
	PRIORITY_CALL              = 127,   // call            OS_PUSH
	PRIORITY_CO_AWAIT          = 127,   // co_await        OS_PUSH
	PRIORITY_CO_YIELD          = 127,   // co_yield        OS_PUSH
	PRIORITY_CO_RETURN         = 127,   // co_return       OS_PUSH
	PRIORITY_SWITCH            = 127,   // switch          OS_PUSH | OS_HAS_EXPR
	PRIORITY_CASE              = 127,   // case            OS_PUSH
	PRIORITY_DEFAULT           = 127,   // default         OS_PUSH
	PRIORITY_GOTO              = 127,   // goto            OS_PUSH
	PRIORITY_LABEL             = 127,   //                 OS_PUSH
#endif
	PRIORITY_PARENTHESIS_OPEN  =  64,   // (               OS_OPEN | OS_PARENTHESIS
	PRIORITY_ADDR_ADJUST_OPEN  =  64,   // [_              OS_OPEN
	PRIORITY_ADDR_REPLACE_OPEN =  64,   // [.              OS_OPEN
	PRIORITY_REV_ENDIAN_OPEN   =  64,   // [~              OS_OPEN
	PRIORITY_REMOTE_OPEN       =  64,   // [:              OS_OPEN
	PRIORITY_POST_INC_DEC      =  64,   // N++, N--        OS_PUSH | OS_MONADIC | OS_POST
	PRIORITY_FUNCTION          =  60,   // parse_int       OS_PUSH | OS_MONADIC
	                                    // parse_real      OS_PUSH | OS_MONADIC
	                                    // parse_reset     OS_PUSH | OS_MONADIC
	                                    // MName           OS_PUSH | OS_MONADIC
	                                    // ::              OS_PUSH
	                                    // :!              OS_PUSH
	                                    // :&              OS_PUSH
	                                    // := :+           OS_PUSH
	                                    // ProcessId       OS_PUSH | OS_MONADIC
	                                    // HNumber         OS_PUSH | OS_MONADIC
	                                    // Memory          OS_PUSH | OS_MONADIC
	                                    // Cast32          OS_PUSH | OS_MONADIC
	                                    // Cast64          OS_PUSH | OS_MONADIC
	                                    // I1toI4          OS_PUSH | OS_MONADIC
	                                    // I2toI4          OS_PUSH | OS_MONADIC
	                                    // I4toI8          OS_PUSH | OS_MONADIC
	                                    // utof            OS_PUSH | OS_MONADIC
	                                    // itof            OS_PUSH | OS_MONADIC
	                                    // ftoi            OS_PUSH | OS_MONADIC
	                                    // trunc           OS_PUSH | OS_MONADIC
	                                    // round           OS_PUSH | OS_MONADIC
	                                    // BitScanForward  OS_PUSH | OS_MONADIC
	                                    // BitScanReverse  OS_PUSH | OS_MONADIC
	                                    // A2U             OS_PUSH | OS_MONADIC
	                                    // A2W             OS_PUSH | OS_MONADIC
	                                    // U2A             OS_PUSH | OS_MONADIC
	                                    // U2W             OS_PUSH | OS_MONADIC
	                                    // W2A             OS_PUSH | OS_MONADIC
	                                    // W2U             OS_PUSH | OS_MONADIC
	                                    // wait            OS_PUSH | OS_MONADIC
	                                    // sleep           OS_PUSH | OS_MONADIC
#if ALLOCATE_SUPPORT
	                                    // realloc         OS_PUSH | OS_MONADIC
#endif
	                                    // alloca          OS_PUSH | OS_MONADIC
	                                    // atoi            OS_PUSH | OS_MONADIC
	                                    // wtoi            OS_PUSH | OS_MONADIC
	                                    // atof            OS_PUSH | OS_MONADIC
	                                    // wtof            OS_PUSH | OS_MONADIC
	                                    // rand32          OS_PUSH | OS_MONADIC
	                                    // rand64          OS_PUSH | OS_MONADIC
	                                    // min             OS_PUSH | OS_MONADIC
	                                    // max             OS_PUSH | OS_MONADIC
	                                    // imin            OS_PUSH | OS_MONADIC
	                                    // imax            OS_PUSH | OS_MONADIC
	                                    // memcmp          OS_PUSH | OS_MONADIC
	                                    // memmove         OS_PUSH | OS_MONADIC
	                                    // memset          OS_PUSH | OS_MONADIC
	                                    // memset16        OS_PUSH | OS_MONADIC
	                                    // memset32        OS_PUSH | OS_MONADIC
	                                    // memset64        OS_PUSH | OS_MONADIC
	                                    // printf          OS_PUSH | OS_MONADIC
	                                    // dprintf         OS_PUSH | OS_MONADIC
	                                    // snprintf        OS_PUSH | OS_MONADIC
	                                    // snwprintf       OS_PUSH | OS_MONADIC
	                                    // strdup          OS_PUSH | OS_MONADIC
	                                    // wcsdup          OS_PUSH | OS_MONADIC
	                                    // strlen          OS_PUSH | OS_MONADIC
	                                    // wcslen          OS_PUSH | OS_MONADIC
	                                    // strnlen         OS_PUSH | OS_MONADIC
	                                    // wcsnlen         OS_PUSH | OS_MONADIC
	                                    // strcmp          OS_PUSH | OS_MONADIC
	                                    // wcscmp          OS_PUSH | OS_MONADIC
	                                    // stricmp         OS_PUSH | OS_MONADIC
	                                    // wcsicmp         OS_PUSH | OS_MONADIC
	                                    // mbsicmp         OS_PUSH | OS_MONADIC
	                                    // strncmp         OS_PUSH | OS_MONADIC
	                                    // wcsncmp         OS_PUSH | OS_MONADIC
	                                    // strnicmp        OS_PUSH | OS_MONADIC
	                                    // wcsnicmp        OS_PUSH | OS_MONADIC
	                                    // mbsnbicmp       OS_PUSH | OS_MONADIC
	                                    // strcpy          OS_PUSH | OS_MONADIC
	                                    // wcscpy          OS_PUSH | OS_MONADIC
	                                    // strcat          OS_PUSH | OS_MONADIC
	                                    // wcscat          OS_PUSH | OS_MONADIC
	                                    // strlcpy         OS_PUSH | OS_MONADIC
	                                    // wcslcpy         OS_PUSH | OS_MONADIC
	                                    // strlcat         OS_PUSH | OS_MONADIC
	                                    // wcslcat         OS_PUSH | OS_MONADIC
	                                    // strchr          OS_PUSH | OS_MONADIC
	                                    // wcschr          OS_PUSH | OS_MONADIC
	                                    // mbschr          OS_PUSH | OS_MONADIC
	                                    // strichr         OS_PUSH | OS_MONADIC
	                                    // wcsichr         OS_PUSH | OS_MONADIC
	                                    // mbsichr         OS_PUSH | OS_MONADIC
	                                    // strrchr         OS_PUSH | OS_MONADIC
	                                    // wcsrchr         OS_PUSH | OS_MONADIC
	                                    // mbsrchr         OS_PUSH | OS_MONADIC
	                                    // strrichr        OS_PUSH | OS_MONADIC
	                                    // wcsrichr        OS_PUSH | OS_MONADIC
	                                    // mbsrichr        OS_PUSH | OS_MONADIC
	                                    // strstr          OS_PUSH | OS_MONADIC
	                                    // wcsstr          OS_PUSH | OS_MONADIC
	                                    // mbsstr          OS_PUSH | OS_MONADIC
	                                    // stristr         OS_PUSH | OS_MONADIC
	                                    // wcsistr         OS_PUSH | OS_MONADIC
	                                    // mbsistr         OS_PUSH | OS_MONADIC
	                                    // strspn          OS_PUSH | OS_MONADIC
	                                    // wcsspn          OS_PUSH | OS_MONADIC
	                                    // mbsspn          OS_PUSH | OS_MONADIC
	                                    // strcspn         OS_PUSH | OS_MONADIC
	                                    // wcscspn         OS_PUSH | OS_MONADIC
	                                    // mbscspn         OS_PUSH | OS_MONADIC
	                                    // strpbrk         OS_PUSH | OS_MONADIC
	                                    // wcspbrk         OS_PUSH | OS_MONADIC
	                                    // mbspbrk         OS_PUSH | OS_MONADIC
	                                    // strset          OS_PUSH | OS_MONADIC
	                                    // wcsset          OS_PUSH | OS_MONADIC
	                                    // mbsset          OS_PUSH | OS_MONADIC
	                                    // strnset         OS_PUSH | OS_MONADIC
	                                    // wcsnset         OS_PUSH | OS_MONADIC
	                                    // mbsnbset        OS_PUSH | OS_MONADIC
	                                    // strtok          OS_PUSH | OS_MONADIC
	                                    // wcstok          OS_PUSH | OS_MONADIC
	                                    // mbstok          OS_PUSH | OS_MONADIC
	                                    // strlwr          OS_PUSH | OS_MONADIC
	                                    // wcslwr          OS_PUSH | OS_MONADIC
	                                    // mbslwr          OS_PUSH | OS_MONADIC
	                                    // strupr          OS_PUSH | OS_MONADIC
	                                    // wcsupr          OS_PUSH | OS_MONADIC
	                                    // mbsupr          OS_PUSH | OS_MONADIC
	                                    // strrev          OS_PUSH | OS_MONADIC
	                                    // wcsrev          OS_PUSH | OS_MONADIC
	                                    // mbsrev          OS_PUSH | OS_MONADIC
	                                    // isalnum         OS_PUSH | OS_MONADIC
	                                    // isalpha         OS_PUSH | OS_MONADIC
	                                    // isascii         OS_PUSH | OS_MONADIC
	                                    // isblank         OS_PUSH | OS_MONADIC
	                                    // iscntrl         OS_PUSH | OS_MONADIC
	                                    // iscsym          OS_PUSH | OS_MONADIC
	                                    // iscsymf         OS_PUSH | OS_MONADIC
	                                    // isdigit         OS_PUSH | OS_MONADIC
	                                    // isgraph         OS_PUSH | OS_MONADIC
	                                    // iskana          OS_PUSH | OS_MONADIC
	                                    // isleadbyte      OS_PUSH | OS_MONADIC
	                                    // islower         OS_PUSH | OS_MONADIC
	                                    // ismbchira       OS_PUSH | OS_MONADIC
	                                    // ismbckata       OS_PUSH | OS_MONADIC
	                                    // ismbcl0         OS_PUSH | OS_MONADIC
	                                    // ismbcl1         OS_PUSH | OS_MONADIC
	                                    // ismbcl2         OS_PUSH | OS_MONADIC
	                                    // ismbclegal      OS_PUSH | OS_MONADIC
	                                    // ismbcsymbol     OS_PUSH | OS_MONADIC
	                                    // isprint         OS_PUSH | OS_MONADIC
	                                    // ispunct         OS_PUSH | OS_MONADIC
	                                    // isspace         OS_PUSH | OS_MONADIC
	                                    // istrailbyte     OS_PUSH | OS_MONADIC
	                                    // isupper         OS_PUSH | OS_MONADIC
	                                    // isxdigit        OS_PUSH | OS_MONADIC
	                                    // toascii         OS_PUSH | OS_MONADIC
	                                    // tolower         OS_PUSH | OS_MONADIC
	                                    // toupper         OS_PUSH | OS_MONADIC
	PRIORITY_NEG               =  56,   // -               OS_PUSH | OS_MONADIC
	PRIORITY_NOT               =  56,   // !               OS_PUSH | OS_MONADIC
	PRIORITY_BIT_NOT           =  56,   // ~               OS_PUSH | OS_MONADIC
	PRIORITY_INDIRECTION       =  56,   // *               OS_PUSH | OS_MONADIC
	PRIORITY_ADDRESS_OF        =  56,   // &               OS_PUSH | OS_MONADIC
	PRIORITY_PRE_INC           =  56,   // ++N             OS_PUSH | OS_MONADIC
	PRIORITY_PRE_DEC           =  56,   // --N             OS_PUSH | OS_MONADIC
	PRIORITY_MUL               =  52,   // *    ( 8 *= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_DIV               =  52,   // /    ( 8 /= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_MOD               =  52,   // %    ( 8 %= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_IDIV              =  52,   // idiv            OS_PUSH
	PRIORITY_IMOD              =  52,   // imod            OS_PUSH
	PRIORITY_ADD               =  48,   // +    ( 8 += )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SUB               =  48,   // -    ( 8 -= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHL               =  44,   // <<   ( 8 <<=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHR               =  44,   // >>   ( 8 >>=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SAR               =  44,   // sar             OS_PUSH
	PRIORITY_ROL               =  44,   // rol             OS_PUSH
	PRIORITY_ROR               =  44,   // ror             OS_PUSH
	PRIORITY_BT                =  40,   // <               OS_PUSH
	PRIORITY_AT                =  40,   // >               OS_PUSH
	PRIORITY_BE                =  40,   // <=              OS_PUSH
	PRIORITY_AE                =  40,   // >=              OS_PUSH
	PRIORITY_LT                =  40,   // lt              OS_PUSH
	PRIORITY_GT                =  40,   // gt              OS_PUSH
	PRIORITY_LE                =  40,   // le              OS_PUSH
	PRIORITY_GE                =  40,   // ge              OS_PUSH
	PRIORITY_EQ                =  36,   // ==              OS_PUSH
	PRIORITY_NE                =  36,   // !=              OS_PUSH
	PRIORITY_BIT_AND           =  32,   // &    ( 8 &= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_XOR               =  28,   // ^    ( 8 ^= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_BIT_OR            =  24,   // |    ( 8 |= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_AND               =  20,   // &&   (20 and)   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_OR                =  16,   // ||   (16 or )   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_TERNARY           =  12,   // ? :             OS_PUSH | OS_TERNARY
	PRIORITY_RIGHT_ASSIGN      =   8,   // =>              OS_PUSH
	PRIORITY_LEFT_ASSIGN       =   8,   // =               OS_PUSH | OS_LEFT_ASSIGN
	PRIORITY_DELIMITER         =   4,   // ,               OS_DELIMITER
	PRIORITY_READ_WRITE        =   4,   // :]  :1] :2] :3] OS_PUSH | OS_CLOSE
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
	PRIORITY_REV_ENDIAN        =   4,   // ~]  ~2] ~3] ~4] OS_PUSH | OS_CLOSE
	                                    // ~5] ~6] ~7] ~8]
	PRIORITY_ADDR_REPLACE      =   4,   // .]              OS_PUSH | OS_CLOSE
	PRIORITY_ADDR_ADJUST       =   4,   // _]              OS_PUSH | OS_CLOSE
	PRIORITY_PARENTHESIS_CLOSE =   4,   // )               OS_CLOSE | OS_PARENTHESIS
	PRIORITY_SPLIT             =   4,   // ;               OS_SPLIT
	PRIORITY_RETURN            =   0,   // return          OS_PUSH
} PRIORITY;

typedef struct _MARKUP {
	TAG                     Tag;
	size_t                  Length;
	LPSTR                   String;
	BYTE                    Priority;
	DWORD                   Type;
	size_t                  Depth;
	size_t                  LoopDepth;
	struct _MARKUP          *Param;
	struct _MARKUP          *Next;
	union {
		uint64_t            UnionBlock;
		struct {
			struct _MARKUP  **TruePart;
			struct _MARKUP  **FalsePart;
		};
		struct _MARKUP      **Jump;
		struct _MARKUP      *Close;
		struct {
			size_t          NumberOfOperand;
#if USE_PLUGIN
			PLUGIN_FUNCTION *Function;
#endif
		};
		LPCVOID             UnescapedString;
	};
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
	map_iterator Node;
#else
	map<unsigned long, pair<unsigned long, unsigned long> >::iterator Node;
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
static MARKUP * __fastcall ReAllocMarkup(MARKUP **lplpMarkupArray, size_t *lpnNumberOfMarkup)
{
	MARKUP *lpMarkupArray;
	size_t nNumberOfMarkup;

	lpMarkupArray = *lplpMarkupArray;
	nNumberOfMarkup = *lpnNumberOfMarkup;
	if (nNumberOfMarkup && !(nNumberOfMarkup & 0x0F))
	{
		lpMarkupArray = (MARKUP *)HeapReAlloc(hHeap, 0, lpMarkupArray, (nNumberOfMarkup + 0x10) * sizeof(MARKUP));
		if (!lpMarkupArray)
			return NULL;
		*lplpMarkupArray = lpMarkupArray;
	}
	*lpnNumberOfMarkup = nNumberOfMarkup + 1;
	return lpMarkupArray + nNumberOfMarkup;
}
//---------------------------------------------------------------------
static __forceinline size_t TrimMarkupString(char **pfirst, const char *last)
{
	unsigned __int64 result;

	result = __ui64return_TrimSpace(*pfirst, last);
	return (const char *)result - (*pfirst = (char *)(result >> 32));
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindParenthesisClose(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth;

		nDepth = 1;
		do
			if (lpMarkup->Type & OS_PARENTHESIS)
				if (lpMarkup->Type & OS_OPEN)
					nDepth++;
				else if (!--nDepth)
					break;
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindDelimiter(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth;

		nDepth = 0;
		do
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
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindSplit(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth;

		nDepth = 0;
		do
			if (lpMarkup->Type & (OS_OPEN | OS_CLOSE | OS_SPLIT))
				if (lpMarkup->Type & (OS_OPEN | OS_CLOSE))
					if (lpMarkup->Type & OS_OPEN)
						nDepth++;
					else if (nDepth)
						nDepth--;
					else
						return (MARKUP *)lpEndOfMarkup;
				else if (!nDepth)
					break;
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindEndOfStructuredStatement(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	for (; ; )
	{
		if (lpMarkup >= lpEndOfMarkup)
			break;
		if (lpMarkup->Type & OS_HAS_EXPR)
		{
			if (++lpMarkup >= lpEndOfMarkup)
				break;
			if (lpMarkup->Tag != TAG_PARENTHESIS_OPEN)
				break;
			if ((lpMarkup = FindParenthesisClose(lpMarkup + 1, lpEndOfMarkup)) + 1 >= lpEndOfMarkup)
				break;
			if (lpMarkup[1].Tag != TAG_ELSE && (lpMarkup = FindEndOfStructuredStatement(lpMarkup + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
				break;
			if (lpMarkup + 1 < lpEndOfMarkup && lpMarkup[1].Tag == TAG_ELSE)
			{
				lpMarkup += 2;
				continue;
			}
		}
		else if (lpMarkup->Tag == TAG_DO)
		{
			if ((lpMarkup = FindEndOfStructuredStatement(lpMarkup + 1, lpEndOfMarkup)) + 3 >= lpEndOfMarkup)
				break;
			if (lpMarkup[1].Tag != TAG_WHILE)
				break;
			if (lpMarkup[2].Tag != TAG_PARENTHESIS_OPEN)
				break;
			if ((lpMarkup = FindParenthesisClose(lpMarkup + 3, lpEndOfMarkup) + 1) >= lpEndOfMarkup)
				break;
			if (lpMarkup->Tag != TAG_SPLIT)
				if (lpMarkup->Tag == TAG_ELSE)
				{
					lpMarkup++;
					continue;
				}
				else
					break;
		}
		else if (lpMarkup->Tag == TAG_PARENTHESIS_OPEN)
			lpMarkup = FindParenthesisClose(lpMarkup + 1, lpEndOfMarkup);
		else
			lpMarkup = FindSplit(lpMarkup, lpEndOfMarkup);
		return (MARKUP *)lpMarkup;
	}
	return (MARKUP *)lpEndOfMarkup;
}
//---------------------------------------------------------------------
BOOL __fastcall CorrectFunction(MARKUP *lpMarkup, MARKUP *lpEndOfMarkup, size_t nNumberOfParams)
{
	MARKUP *lpFunction, *lpClose;

	lpFunction = lpMarkup;
	if (++lpMarkup < lpEndOfMarkup &&
		lpMarkup->Tag == TAG_PARENTHESIS_OPEN &&
		(lpClose = FindParenthesisClose(lpMarkup + 1, lpEndOfMarkup)) < lpEndOfMarkup)
	{
		size_t nCount;

		nCount = 0;
		if (lpMarkup + 1 < lpClose)
		{
			MARKUP *lpList;

			lpList = lpFunction;
			goto LOOP_BEGIN;

			do
			{
				MARKUP *lpParam;

				lpList = lpList->Next = lpMarkup;

			LOOP_BEGIN:
				lpParam = lpMarkup;
				do
					if (++lpParam >= lpEndOfMarkup)
						return FALSE;
				while (lpParam->Tag == TAG_PARENTHESIS_OPEN);
				lpList->Param = lpParam;
				nCount++;
			} while ((lpMarkup = FindDelimiter(lpMarkup + 1, lpClose)) < lpClose);
		}
		if (nCount >= nNumberOfParams)
		{
			lpFunction->NumberOfOperand = nCount;
			return TRUE;
		}
	}
	return FALSE;
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

		#define APPEND_TAG(tag, length, priority, type)                                         \
		do                                                                                      \
		{                                                                                       \
		    if (!(lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)))                        \
		        goto FAILED;                                                                    \
		    lpMarkup->Tag        = tag;                                                         \
		    lpMarkup->Length     = length;                                                      \
		    lpMarkup->String     = p;                                                           \
		    lpMarkup->Priority   = priority;                                                    \
		    lpMarkup->Type       = type;                                                        \
		    lpMarkup->Depth      = 0;                                                           \
		    lpMarkup->Param      = NULL;                                                        \
		    lpMarkup->Next       = NULL;                                                        \
		    lpMarkup->UnionBlock = 0;                                                           \
		} while (0)

		#define APPEND_TAG_WITH_CONTINUE(tag, length, priority, type)                           \
		if (1)                                                                                  \
		{                                                                                       \
		    APPEND_TAG(tag, length, priority, type);                                            \
		    p += length;                                                                        \
		    continue;                                                                           \
		} else while(0)

		#define APPEND_FUNCTION_SINGLE_PARAM(tag, length)                                       \
		if (1)                                                                                  \
		{                                                                                       \
		    if (p[length] == '(' || __intrinsic_isspace(p[length]))                             \
		        nLength = length;                                                               \
		    else if (p[length] == ':' && p[length + 1] == ':')                                  \
		        nLength = length + 2;                                                           \
		    else                                                                                \
		        break;                                                                          \
		    bNextIsSeparatedLeft = TRUE;                                                        \
		    bCorrectTag = TRUE;                                                                 \
		    APPEND_TAG_WITH_CONTINUE(tag, nLength, PRIORITY_FUNCTION, OS_PUSH | OS_MONADIC);    \
		} else while(0)

		#define APPEND_FUNCTION_MULTI_PARAM(tag, length)                                        \
		if (1)                                                                                  \
		{                                                                                       \
		    if (p[length] != '(' && !__intrinsic_isspace(p[length]))                            \
		        break;                                                                          \
		    bNextIsSeparatedLeft = TRUE;                                                        \
		    bCorrectTag = TRUE;                                                                 \
		    APPEND_TAG_WITH_CONTINUE(tag, length, PRIORITY_FUNCTION, OS_PUSH | OS_MONADIC);     \
		} else while(0)

		/*
		#define IS_SEPARATED_LEFT(p) (                                                          \
		    __intrinsic_isascii((p)[0]) &&                                                      \
		    !__intrinsic_isalnum((p)[0]) &&                                                     \
		    ((p)[0] != '_' || ((p) != lpSrc && (p)[-1] == '[')) &&                              \
		    (p)[0] != '$')
		*/

		#define IS_SEPARATED_RIGHT(p) (                                                         \
		    __intrinsic_isascii((p)[0]) &&                                                      \
		    !__intrinsic_isalnum((p)[0]) &&                                                     \
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
			lpMarkup->Tag        = TAG_MODULENAME;
			lpMarkup->Length     = p - lpTagArray[nNumberOfTag - 2].String - 1;
			lpMarkup->String     = lpTagArray[nNumberOfTag - 2].String + 2;
			lpMarkup->Priority   = PRIORITY_FUNCTION;
			lpMarkup->Type       = OS_PUSH;
			lpMarkup->Depth      = 0;
			lpMarkup->Param      = NULL;
			lpMarkup->Next       = NULL;
		    lpMarkup->UnionBlock = 0;
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
					if (++p >= end)
						break;
					if (!__intrinsic_isleadbyte(*p))
						continue;
				}
				if (++p >= end)
					break;
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
		case '\'':
		SINGLE_QUOTED_CHARACTER:
			// single-quoted character
			bNextIsSeparatedLeft = TRUE;
			while (++p < end && *p != '\'')
			{
				if (!__intrinsic_isleadbyte(*p))
				{
					if (*p != '\\')
						continue;
					if (++p >= end)
						break;
					if (!__intrinsic_isleadbyte(*p))
						continue;
				}
				if (++p >= end)
					break;
			}
			break;
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
		case 'A':
			// "A2U", "A2W"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2U'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_A2U, 3);
			case BSWAP16('2W'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_A2W, 3);
			}
			break;
		case 'B':
			// "BitScanForward", "BitScanReverse"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('BitS'))
				break;
			switch (*(uint32_t *)(p + 4))
			{
			case BSWAP32('canF'):
				if (*(uint32_t *)(p + 8) != BSWAP32('orwa'))
					break;
				if (*(uint16_t *)(p + 12) != BSWAP16('rd'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_BSF, 14);
			case BSWAP32('canR'):
				if (*(uint32_t *)(p + 8) != BSWAP32('ever'))
					break;
				if (*(uint16_t *)(p + 12) != BSWAP16('se'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_BSR, 14);
			}
			break;
		case 'C':
			// "Cast32", "Cast64"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('Cast'))
				break;
			switch (*(uint16_t *)(p + 4))
			{
			case BSWAP16('32'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_CAST32, 6);
			case BSWAP16('64'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_CAST64, 6);
			}
			break;
		case 'H':
			// "HNumber"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('Numb'))
				break;
			if (*(uint16_t *)(p + 5) != BSWAP16('er'))
				break;
			APPEND_FUNCTION_SINGLE_PARAM(TAG_HNUMBER, 7);
		case 'I':
			// "I1toI4", "I2toI4", "I4toI8"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)p)
			{
			case BSWAP32('I1to'):
				if (*(uint16_t *)(p + 4) != BSWAP16('I4'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I1TOI4, 6);
			case BSWAP32('I2to'):
				if (*(uint16_t *)(p + 4) != BSWAP16('I4'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I2TOI4, 6);
			case BSWAP32('I4to'):
				if (*(uint16_t *)(p + 4) != BSWAP16('I8'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I4TOI8, 6);
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
				switch (lpPrev[-1].Tag)
				{
				case TAG_A2U:
				case TAG_A2W:
				case TAG_U2A:
				case TAG_U2W:
				case TAG_W2A:
				case TAG_W2U:
				case TAG_STRLEN:
				case TAG_WCSLEN:
				case TAG_STRDUP:
				case TAG_WCSDUP:
				case TAG_STRLWR:
				case TAG_WCSLWR:
				case TAG_MBSLWR:
				case TAG_STRUPR:
				case TAG_WCSUPR:
				case TAG_MBSUPR:
				case TAG_STRREV:
				case TAG_WCSREV:
				case TAG_MBSREV:
					if (lpPrev->Tag != TAG_PARENTHESIS_OPEN)
						break;
				case TAG_ATOI:
				case TAG_WTOI:
				case TAG_ATOF:
				case TAG_WTOF:
				case TAG_MEMCMP:
				case TAG_MEMMOVE:
				case TAG_MEMSET:
				case TAG_MEMSET16:
				case TAG_MEMSET32:
				case TAG_MEMSET64:
				case TAG_PRINTF:
				case TAG_DPRINTF:
				case TAG_SNPRINTF:
				case TAG_SNWPRINTF:
				case TAG_STRNLEN:
				case TAG_WCSNLEN:
				case TAG_STRCMP:
				case TAG_WCSCMP:
				case TAG_STRICMP:
				case TAG_WCSICMP:
				case TAG_MBSICMP:
				case TAG_STRNCMP:
				case TAG_WCSNCMP:
				case TAG_STRNICMP:
				case TAG_WCSNICMP:
				case TAG_MBSNBICMP:
				case TAG_STRCPY:
				case TAG_WCSCPY:
				case TAG_STRCAT:
				case TAG_WCSCAT:
				case TAG_STRLCPY:
				case TAG_WCSLCPY:
				case TAG_STRLCAT:
				case TAG_WCSLCAT:
				case TAG_STRCHR:
				case TAG_WCSCHR:
				case TAG_MBSCHR:
				case TAG_STRICHR:
				case TAG_WCSICHR:
				case TAG_MBSICHR:
				case TAG_STRRCHR:
				case TAG_WCSRCHR:
				case TAG_MBSRCHR:
				case TAG_STRRICHR:
				case TAG_WCSRICHR:
				case TAG_MBSRICHR:
				case TAG_STRSTR:
				case TAG_WCSSTR:
				case TAG_MBSSTR:
				case TAG_STRISTR:
				case TAG_WCSISTR:
				case TAG_MBSISTR:
				case TAG_STRSPN:
				case TAG_WCSSPN:
				case TAG_MBSSPN:
				case TAG_STRCSPN:
				case TAG_WCSCSPN:
				case TAG_MBSCSPN:
				case TAG_STRPBRK:
				case TAG_WCSPBRK:
				case TAG_MBSPBRK:
				case TAG_STRSET:
				case TAG_WCSSET:
				case TAG_MBSSET:
				case TAG_STRNSET:
				case TAG_WCSNSET:
				case TAG_MBSNBSET:
				case TAG_STRTOK:
				case TAG_WCSTOK:
				case TAG_MBSTOK:
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARAM_LOCAL, 1, PRIORITY_PARAM_LOCAL, OS_PUSH);
				}
			}
			break;
		case 'M':
			// "MName", "Memory"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('Name'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_MNAME, 5);
			case BSWAP32('emor'):
				if (p[5] != 'y')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_MEMORY, 6);
			}
			break;
		case 'P':
			// "ProcessId"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('roce'))
				break;
			if (*(uint32_t *)(p + 5) != BSWAP32('ssId'))
				break;
			APPEND_FUNCTION_SINGLE_PARAM(TAG_PROCESSID, 9);
		case 'U':
			// "U2A", "U2W"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2A'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_U2A, 3);
			case BSWAP16('2W'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_U2W, 3);
			}
			break;
		case 'W':
			// "W2A", "W2U"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2A'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_W2A, 3);
			case BSWAP16('2U'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_W2U, 3);
			}
			break;
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
			// "alloca", "and", "atoi", "atof"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('ll'):
				if (*(uint32_t *)(p + 2) != BSWAP32('loca'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_ALLOCA, 6);
			case BSWAP16('nd'):
				iTag = TAG_AND;
				nLength = 3;
				bPriority = PRIORITY_AND;
				goto APPEND_RET_OPERAND_OPERATOR;
			case BSWAP16('to'):
				switch (p[3])
				{
				case 'i':
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ATOI, 4);
				case 'f':
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ATOF, 4);
				}
				break;
			}
			break;
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
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('ll'):
					if (p[4] != '(' && !__intrinsic_isspace(p[4]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CALL, 4, PRIORITY_CALL, OS_PUSH);
				case BSWAP16('se'):
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
				APPEND_FUNCTION_SINGLE_PARAM(TAG_DPRINTF, 7);
			}
			break;
		case 'e':
			// "elif", "else", "elseif"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)p)
			{
			case BSWAP32('elif'):
				if (p[4] != '(' && !__intrinsic_isspace(p[4]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG(TAG_ELSE, 4, PRIORITY_ELSE, 0);
				p += 4;
				APPEND_TAG_WITH_CONTINUE(TAG_IF, 0, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
			case BSWAP32('else'):
				if (p[4] != '(' && !__intrinsic_isspace(p[4]) && p[4] != ';')
					if (p[4] != 'i' || p[5] != 'f' || p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_ELSE, 4, PRIORITY_ELSE, 0);
			}
			break;
		case 'f':
			// "for", "ftoi"
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
				if (*(uint16_t *)(p + 2) != BSWAP16('oi'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_FTOI, 4);
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
			// "imax", "imin"
			// "idiv", "imod", "if", "itof",
			// "isalnum", "isalpha", "isascii", "isblank", "iscntrl", "iscsym", "iscsymf",
			// "isdigit", "isgraph", "iskana", "isleadbyte", "islower",
			// "ismbchira", "ismbckata", "ismbcl0", "ismbcl1", "ismbcl2", "ismbclegal", "ismbcsymbol",
			// "isprint", "ispunct", "isspace", "istrailbyte", "isupper", "isxdigit"
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
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('ax'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_IMAX, 4);
				case BSWAP16('in'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_IMIN, 4);
				case BSWAP16('od'):
					iTag = TAG_IMOD;
					nLength = 4;
					bPriority = PRIORITY_IMOD;
					goto APPEND_WORD_OPERATOR;
				}
				break;
			case 'f':
				if (p[2] != '(' && !__intrinsic_isspace(p[2]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_IF, 2, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
			case 's':
				switch (p[2])
				{
				case 'a':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('lnum'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISALNUM, 7);
					case BSWAP32('lpha'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISALPHA, 7);
					case BSWAP32('scii'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISASCII, 7);
					}
					break;
				case 'b':
					if (*(uint32_t *)(p + 3) != BSWAP32('lank'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISBLANK, 7);
				case 'c':
					switch (*(uint16_t *)(p + 3))
					{
					case BSWAP16('nt'):
						if (*(uint16_t *)(p + 5) != BSWAP16('rl'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCNTRL, 7);
					case BSWAP16('sy'):
						if (p[5] != 'm')
							break;
						if (p[6] != 'f')
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCSYM, 6);
						else
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCSYMF, 7);
						break;
					}
					break;
				case 'd':
					if (*(uint32_t *)(p + 3) != BSWAP32('igit'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISDIGIT, 7);
				case 'g':
					if (*(uint32_t *)(p + 3) != BSWAP32('raph'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISGRAPH, 7);
				case 'k':
					if (*(uint32_t *)(p + 2) != BSWAP32('kana'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISKANA, 6);
				case 'l':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('eadb'):
						if (*(uint32_t *)(p + 6) != BSWAP16('byte'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISLEADBYTE, 10);
					case BSWAP32('ower'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISLOWER, 7);
					}
					break;
				case 'm':
					if (*(uint16_t *)(p + 3) != BSWAP16('bc'))
						break;
					switch (*(uint16_t *)(p + 5))
					{
					case BSWAP16('hi'):
						if (*(uint16_t *)(p + 7) != BSWAP16('ra'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCHIRA, 9);
					case BSWAP16('ka'):
						if (*(uint16_t *)(p + 7) != BSWAP16('ta'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCKATA, 9);
					case BSWAP16('l0'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL0, 7);
					case BSWAP16('l1'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL1, 7);
					case BSWAP16('l2'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL2, 7);
					case BSWAP16('le'):
						if (*(uint32_t *)(p + 6) != BSWAP32('egal'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCLEGAL, 10);
					case BSWAP16('sy'):
						if (*(uint32_t *)(p + 7) != BSWAP32('mbol'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCSYMBOL, 11);
					}
					break;
				case 'p':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('rint'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISPRINT, 7);
					case BSWAP32('unct'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISPUNCT, 7);
					}
					break;
				case 's':
					if (*(uint32_t *)(p + 3) != BSWAP32('pace'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISSPACE, 7);
				case 't':
					if (*(uint32_t *)(p + 3) != BSWAP32('trai'))
						break;
					if (*(uint32_t *)(p + 7) != BSWAP32('lbyt'))
						break;
					if (p[11] != 'e')
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISTRAILBYTE, 12);
				case 'u':
					if (*(uint32_t *)(p + 3) != BSWAP32('pper'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISTRAILBYTE, 7);
				case 'x':
					if (*(uint32_t *)(p + 3) != BSWAP32('digi'))
						break;
					if (p[7] != 't')
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISXDIGIT, 8);
				}
				break;
			case 't':
				if (*(uint16_t *)(p + 2) != BSWAP16('of'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_ITOF, 4);
			}
			break;
		case 'l':
			// "lt", "le"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 't':
				iTag = TAG_LT;
				nLength = 2;
				bPriority = PRIORITY_LT;
				goto APPEND_WORD_OPERATOR;
			case 'e':
				iTag = TAG_LE;
				nLength = 2;
				bPriority = PRIORITY_LE;
				goto APPEND_WORD_OPERATOR;
			}
			break;
		case 'm':
			// "max", "min"
			// "mbschr", "mbscspn", "mbsichr", "mbsicmp", "mbsistr", "mbslwr", "mbsnbicmp", "mbsnbset", "mbspbrk", "mbsrchr", "mbsrev", "mbsrichr", "mbsset", "mbsspn", "mbsstr", "mbstok", "mbsupr",
			// "memcmp", "memmove", "memset", "memset16", "memset32", "memset64"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('ax'):
				APPEND_FUNCTION_MULTI_PARAM(TAG_MAX, 3);
			case BSWAP16('bs'):
				switch (p[3])
				{
				case 'c':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSCHR, 6);
					case BSWAP16('sp'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSCSPN, 7);
					}
					break;
				case 'i':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSICHR, 7);
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSICMP, 7);
					case BSWAP16('st'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSISTR, 7);
					}
					break;
				case 'l':
					if (*(uint16_t *)(p + 4) != BSWAP16('wr'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSLWR, 6);
				case 'n':
					if (p[4] != 'b')
						break;
					switch (p[5])
					{
					case 'i':
						if (*(uint32_t *)(p + 5) != BSWAP32('icmp'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBICMP, 9);
					case 's':
						if (*(uint16_t *)(p + 6) != BSWAP16('et'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBSET, 8);
					}
					break;
				case 'p':
					if (*(uint16_t *)(p + 4) != BSWAP16('br'))
						break;
					if (p[6] != 'k')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MBSPBRK, 7);
				case 'r':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRCHR, 7);
					case BSWAP16('ev'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSREV, 6);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRICHR, 8);
					}
					break;
				case 's':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('et'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSET, 6);
					case BSWAP16('pn'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSPN, 6);
					case BSWAP16('tr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSTR, 6);
					}
					break;
				case 't':
					if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MBSTOK, 6);
				case 'u':
					if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSUPR, 6);
				}
				break;
			case BSWAP16('em'):
				switch (*(uint16_t *)(p + 3))
				{
				case BSWAP16('cm'):
					if (p[5] != 'p')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MEMCMP, 6);
				case BSWAP16('mo'):
					if (*(uint16_t *)(p + 5) != BSWAP16('ve'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MEMMOVE, 7);
				case BSWAP16('se'):
					if (p[5] != 't')
						break;
					switch (*(uint16_t *)(p + 6))
					{
					case BSWAP16('16'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET16, 8);
					case BSWAP16('32'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET32, 8);
					case BSWAP16('64'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET64, 8);
					default:
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET, 6);
					}
					break;
				}
				break;
			case BSWAP16('in'):
				APPEND_FUNCTION_MULTI_PARAM(TAG_MIN, 3);
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
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('arse'):
				switch (*(uint32_t *)(p + 5))
				{
				case BSWAP32('_int'):
					if (p[9] != ';' && p[9] != '(' && !__intrinsic_isspace(p[9]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_INT, 9, PRIORITY_FUNCTION, OS_PUSH | OS_MONADIC);
				case BSWAP32('_rea'):
					if (p[9] != 'l')
						break;
					if (p[10] != ';' && p[10] != '(' && !__intrinsic_isspace(p[10]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_REAL, 10, PRIORITY_FUNCTION, OS_PUSH | OS_MONADIC);
				case BSWAP32('_res'):
					if (*(uint16_t *)(p + 9) != BSWAP16('et'))
						break;
					if (p[11] != ';' && p[11] != '(' && !__intrinsic_isspace(p[11]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_RESET, 11, PRIORITY_FUNCTION, OS_PUSH | OS_MONADIC);
				}
				break;
			case BSWAP32('rint'):
				if (p[5] != 'f')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_PRINTF, 6);
			}
			break;
		case 'r':
			// "rand32", "rand64", "realloc", "return", "rol", "ror", "round"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				switch (*(uint32_t *)(p + 2))
				{
				case BSWAP32('nd32'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_RAND32, 6);
				case BSWAP32('nd64'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_RAND64, 6);
				}
				break;
			case 'e':
#if ALLOCATE_SUPPORT
				if (*(uint32_t *)(p + 2) == BSWAP32('allo'))
				{
					if (p[6] != 'c')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_REALLOC, 7);
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
				switch (p[2])
				{
				case 'l':
					iTag = TAG_ROL;
					nLength = 3;
					bPriority = PRIORITY_ROL;
					goto APPEND_WORD_OPERATOR;
				case 'r':
					iTag = TAG_ROR;
					nLength = 3;
					bPriority = PRIORITY_ROR;
					goto APPEND_WORD_OPERATOR;
				case 'u':
					if (*(uint16_t *)(p + 3) != BSWAP16('nd'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ROUND, 5);
				}
				break;
			}
			break;
		case 's':
			// "sleep"
			// "sar", "snprintf", "snwprintf", "strcat", "strchr", "strcmp", "strcpy", "strcspn", "strdup", "strichr", "stricmp", "stristr", "strlcat", "strlcpy", "strlen", "strlwr", "strncmp", "strnicmp", "strnlen", "strnset", "strpbrk", "strrchr", "strrev", "strrichr", "strset", "strspn", "strstr", "strtok", "strupr"
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
			case 'l':
				if (*(uint16_t *)(p + 2) != 'ee')
					break;
				if (p[4] != 'p')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_SLEEP, 5);
				break;
			case 'n':
				switch (*(uint32_t *)(p + 2))
				{
				case BSWAP32('prin'):
					if (*(uint16_t *)(p + 6) != BSWAP16('tf'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_SNPRINTF, 8);
				case BSWAP32('wpri'):
					if (*(uint16_t *)(p + 6) != BSWAP16('nt'))
						break;
					if (p[8] != 'f')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_SNWPRINTF, 9);
				}
				break;
			case 't':
				if (p[2] != 'r')
					break;
				switch (p[3])
				{
				case 'c':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('at'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRCAT, 6);
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRCHR, 6);
					case BSWAP16('mp'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRCMP, 6);
					case BSWAP16('py'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRCPY, 6);
					case BSWAP16('sp'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRCSPN, 7);
					}
					break;
				case 'd':
					if (*(uint16_t *)(p + 4) != BSWAP16('up'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_STRDUP, 6);
				case 'i':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRICHR, 7);
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRICMP, 7);
					case BSWAP16('st'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRISTR, 7);
					}
					break;
				case 'l':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ca'):
						if (p[6] != 't')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRLCAT, 7);
					case BSWAP16('cp'):
						if (p[6] != 'y')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRLCPY, 7);
					case BSWAP16('en'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_STRLEN, 6);
					case BSWAP16('wr'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_STRLWR, 6);
					}
					break;
				case 'n':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRNCMP, 7);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('mp'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRNICMP, 8);
					case BSWAP16('le'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRNLEN, 7);
					case BSWAP16('se'):
						if (p[6] != 't')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRNSET, 7);
					}
					break;
				case 'p':
					if (*(uint16_t *)(p + 4) != BSWAP16('br'))
						break;
					if (p[6] != 'k')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_STRPBRK, 7);
				case 'r':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRRCHR, 7);
					case BSWAP16('ev'):
						if (*(uint16_t *)(p + 4) != BSWAP16('ev'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_STRREV, 6);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRRICHR, 8);
					}
					break;
				case 's':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('et'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRSET, 6);
					case BSWAP16('pn'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRSPN, 6);
					case BSWAP16('tr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRSTR, 6);
					}
					break;
				case 't':
					if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_STRTOK, 6);
				case 'u':
					if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_STRUPR, 6);
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
			// "toascii", "tolower", "toupper", "trunc"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('oasc'):
				if (*(uint16_t *)(p + 5) != BSWAP16('ii'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOASCII, 7);
			case BSWAP32('olow'):
				if (*(uint16_t *)(p + 5) != BSWAP16('er'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOLOWER, 7);
			case BSWAP32('oupp'):
				if (*(uint16_t *)(p + 5) != BSWAP16('er'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOUPPER, 7);
			case BSWAP32('runc'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TRUNC, 5);
			}
			break;
		case 'u':
			// unicode or utf-8 string (u", u8")
			// "utof"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case '"':
				p++;
				goto DOUBLE_QUOTED_STRING;
			case '\'':
				p++;
				goto SINGLE_QUOTED_CHARACTER;
			case '8':
				switch (p[2])
				{
				case '"':
					p += 2;
					goto DOUBLE_QUOTED_STRING;
				case '\'':
					p += 2;
					goto SINGLE_QUOTED_CHARACTER;
				}
				break;
			case 't':
				if (*(uint16_t *)(p + 2) != BSWAP16('of'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_UTOF, 4);
			}
			break;
		case 'w':
			// "wait",
			// "wcscat", "wcschr", "wcscmp", "wcscpy", "wcscspn", "wcsdup", "wcsichr", "wcsicmp", "wcsistr", "wcslcat", "wcslcpy", "wcslen", "wcslwr", "wcsncmp", "wcsnicmp", "wcsnlen", "wcsnset", "wcspbrk", "wcsrchr", "wcsrev", "wcsrichr", "wcsset", "wcsspn", "wcsstr", "wcstok", "wcsupr",
			// "wtoi", "wtof",
			// "while"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				if (*(uint16_t *)(p + 2) != BSWAP16('it'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_WAIT, 4);
			case 'c':
				if (p[2] != 's')
					break;
				switch (p[3])
				{
				case 'c':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('at'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCAT, 6);
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCHR, 6);
					case BSWAP16('mp'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCMP, 6);
					case BSWAP16('py'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCPY, 6);
					case BSWAP16('sp'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCSPN, 7);
					}
					break;
				case 'd':
					if (*(uint16_t *)(p + 4) != BSWAP16('up'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSDUP, 6);
				case 'i':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSICHR, 7);
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSICMP, 7);
					case BSWAP16('st'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSISTR, 7);
					}
					break;
				case 'l':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ca'):
						if (p[6] != 't')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSLCAT, 7);
					case BSWAP16('cp'):
						if (p[6] != 'y')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSLCPY, 7);
					case BSWAP16('en'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSLEN, 6);
					case BSWAP16('wr'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSLWR, 6);
					}
					break;
				case 'n':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNCMP, 7);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('mp'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNICMP, 8);
					case BSWAP16('le'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNLEN, 7);
					case BSWAP16('se'):
						if (p[6] != 't')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNSET, 7);
					}
					break;
				case 'p':
					if (*(uint16_t *)(p + 4) !=  BSWAP16('br'))
						break;
					if (p[6] != 'k')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WCSPBRK, 7);
				case 'r':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRCHR, 7);
					case BSWAP16('ev'):
						if (*(uint16_t *)(p + 4) !=  BSWAP16('ev'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSREV, 6);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRICHR, 8);
					}
					break;
				case 's':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('et'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSET, 6);
					case BSWAP16('pn'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSPN, 6);
					case BSWAP16('tr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSTR, 6);
					}
					break;
				case 't':
					if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WCSTOK, 6);
				case 'u':
					if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSUPR, 6);
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
			case 't':
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('oi'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WTOI, 4);
				case BSWAP16('of'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WTOF, 4);
				}
				break;
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
						lpMarkup->Tag             = TAG_PLUGIN;
						lpMarkup->Length          = Function->NameLength;
						lpMarkup->String          = p;
						lpMarkup->Priority        = PRIORITY_FUNCTION;
						lpMarkup->Type            = OS_PUSH;
						lpMarkup->Depth           = 0;
						lpMarkup->Param           = NULL;
						lpMarkup->Next            = NULL;
						lpMarkup->NumberOfOperand = 0;
						lpMarkup->Function        = Function;
						p += Function->NameLength;
						goto CONTINUE;
					}
				} while (low < high);
			}
		}
#endif
		p++;

	CONTINUE:;
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
					if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
					{
						if (nDepth || !(lpElement->Type & OS_SPLIT) && lpElement->Priority >= PRIORITY_TERNARY)
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
			lpBegin->Tag        = TAG_PARENTHESIS_OPEN;
			lpBegin->Length     = 0;
			lpBegin->Priority   = PRIORITY_PARENTHESIS_OPEN;
			lpBegin->Type       = OS_OPEN | OS_PARENTHESIS;
			lpBegin->Depth      = 0;
			lpBegin->Param      = NULL;
			lpBegin->Next       = NULL;
		    lpBegin->UnionBlock = 0;
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
			lpEnd->Tag        = TAG_PARENTHESIS_CLOSE;
			lpEnd->Length     = 0;
			lpEnd->Priority   = PRIORITY_PARENTHESIS_CLOSE;
			lpEnd->Type       = OS_PUSH | OS_CLOSE | OS_PARENTHESIS | OS_TERNARY_END;
			lpEnd->Depth      = 0;
			lpEnd->Param      = NULL;
			lpEnd->Next       = NULL;
		    lpEnd->UnionBlock = 0;
		}
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (!lpMarkupArray)
		goto FAILED;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	p = lpSrc;
	for (MARKUP *lpTag = lpTagArray; ; )
	{
		LPCSTR last;
		size_t length;

		if (p < (last = lpTag < lpEndOfTag ? lpTag[0].String : lpSrc + nSrcLength) && (
			(length = TrimMarkupString(&p, last)) ||
			lpTag > lpTagArray && lpTag < lpEndOfTag && (
				lpTag[-1].Tag == TAG_DELIMITER ?
					lpTag[0].Tag == TAG_DELIMITER || lpTag[0].Tag == TAG_PARENTHESIS_CLOSE :
					lpTag[-1].Tag == TAG_PARENTHESIS_OPEN && lpTag[0].Tag == TAG_DELIMITER)))
		{
			size_t prefixLength;

			lpMarkup->Tag        = TAG_NOT_OPERATOR;
			lpMarkup->Length     = length;
			lpMarkup->String     = p;
			lpMarkup->Priority   = PRIORITY_NOT_OPERATOR;
			lpMarkup->Type       = OS_PUSH;
			lpMarkup->Depth      = lpTag != lpTagArray ? lpTag[-1].Depth + (lpTag[-1].Tag == TAG_IF_EXPR || lpTag[-1].Tag == TAG_ELSE) : 0;
			lpMarkup->Param      = NULL;
			lpMarkup->Next       = NULL;
		    lpMarkup->UnionBlock = 0;
			if (lpMarkup->String[prefixLength = 0] != '"' && (lpMarkup->String[0] != 'u' ||
				lpMarkup->String[prefixLength = 1] != '"' && (lpMarkup->String[1] != '8' ||
				lpMarkup->String[prefixLength = 2] != '"')))
			{
				#define TAG_ADD_SUB_LENGTH 1

				char *p, *end, *next;
				BOOL hex, decpt;

				// correct the scientific notation of floating point number (e-notation, p-notation)
				if (lpTag >= lpEndOfTag ||
					lpTag->Tag != TAG_ADD && lpTag->Tag != TAG_SUB ||
					lpTag->Type & OS_LEFT_ASSIGN ||
					(next = lpTag + 1 < lpEndOfTag ? lpTag[1].String : lpSrc + nSrcLength) <= lpTag->String + TAG_ADD_SUB_LENGTH ||
					(end = (p = lpMarkup->String) + lpMarkup->Length) != lpTag->String ||
					p >= --end)
					goto INC_MARKUP;
				decpt = hex = FALSE;
				if (*end != 'e' && *end != 'E')
				{
					if (*end != 'p' && *end != 'P' ||
						p[0] != '0' || p[1] != 'x' && p[1] != 'X' ||
						(p += 2) >= end)
						goto INC_MARKUP;
					hex = TRUE;
				}
				if (*p == '.' && p + 1 == end)
					goto INC_MARKUP;
				do
				{
					switch (*(p++))
					{
					case '.':
						if (decpt)
							break;
						decpt = TRUE;
						continue;
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						continue;
					case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
					case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
						if (hex)
							continue;
						break;
					}
					goto INC_MARKUP;
				} while (p != end);
				end += 1 + TAG_ADD_SUB_LENGTH;
				for (; ; )
				{
					if (*end >= '0' && *end <= '9')
						if (++end < next)
							continue;
						else
							break;
					p = end;
					do
						if (!__intrinsic_isspace(*p))
							goto INC_MARKUP;
					while (++p < next);
					break;
				}
				lpMarkup->Length = end - lpMarkup->String;
				lpTag++;

				#undef TAG_ADD_SUB_LENGTH
			}
			else
			{
				char *p, *end;

				// correct double quoted string
				if (lpMarkup == lpMarkupArray)
					goto INC_MARKUP;
				if (lpMarkup[-1].Tag > TAG_PROCESSID || lpMarkup[-1].Tag < TAG_MNAME)
				{
					lpMarkup->Type = OS_PUSH | OS_STRING;
					goto INC_MARKUP;
				}
				end = (p = lpMarkup->String) + lpMarkup->Length;
				for (; ; )
				{
					char   c;
					size_t size;

					if ((c = *(p++)) != '"')
						if (p < end && !__intrinsic_isleadbyte(c) || ++p < end)
							continue;
						else
							break;
					if (!(size = --end - --p))
						break;
					memcpy(p, p + 1, size);
					if (*(p = UnescapeA(p, &end, FALSE)) != '"')
						break;
					if (!(size = --end - p))
						break;
					memcpy(p, p + 1, size);
				}
				lpMarkup->Length = end - lpMarkup->String;
			}
		INC_MARKUP:
			lpMarkup++;
		}
		if (lpTag >= lpEndOfTag)
			break;
		*lpMarkup = *(lpTag++);
		p = lpMarkup->String + lpMarkup->Length;
		lpMarkup++;
	}
	lpEndOfMarkup = lpMarkup;

	// release
	HeapFree(hHeap, 0, lpTagArray);

	// correct tag
	if (bCorrectTag)
	{
		MARKUP *lpFirstDoWhileLoop;

		lpFirstDoWhileLoop = NULL;
		for (MARKUP *lpMarkup1 = lpMarkupArray; lpMarkup1 < lpEndOfMarkup; lpMarkup1++)
		{
			switch (lpMarkup1->Tag)
			{
			default:
				continue;
			case TAG_IF:
				// correct if block
				{
					MARKUP *lpOpen, *lpClose, *lpBegin, *lpEnd, *lpElse;

					if ((lpOpen = lpMarkup1 + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = lpBegin = lpClose = FindParenthesisClose(lpOpen + 1, lpEndOfMarkup)) + 1 >= lpEndOfMarkup)
						break;
					if (lpEnd[1].Tag != TAG_ELSE && (lpEnd = FindEndOfStructuredStatement(++lpBegin, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) >= lpEndOfMarkup || lpElse->Tag != TAG_ELSE)
						lpEnd->Type |= OS_SPLIT;
					else
						lpElse->Type = OS_PUSH;
					lpClose->Tag = TAG_IF_EXPR;
					lpClose->Type |= OS_PUSH | OS_SPLIT;
					for (MARKUP *lpElement = lpBegin; lpElement < lpEnd; lpElement++)
						lpElement->Depth++;
				}
				continue;
			case TAG_ELSE:
				// correct else block
				{
					MARKUP *lpBegin, *lpEnd;

					if (!lpMarkup1->Type)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpBegin = lpMarkup1 + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					lpEnd->Type |= OS_SPLIT;
					for (MARKUP *lpElement = lpBegin; lpElement < lpEnd; lpElement++)
						lpElement->Depth++;
				}
				continue;
			case TAG_DO:
				// correct do-while loop
				{
					MARKUP *lpWhile, *lpEnd, *lpNext;

					if ((lpWhile = FindEndOfStructuredStatement(lpMarkup1 + 1, lpEndOfMarkup) + 1) + 2 >= lpEndOfMarkup)
						break;
					if (lpWhile[0].Tag != TAG_WHILE)
						break;
					if (lpWhile[1].Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpNext = (lpEnd = FindParenthesisClose(lpWhile + 2, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if (lpNext->Tag != TAG_SPLIT)
						if (lpNext->Tag == TAG_ELSE)
							lpNext->Type = OS_PUSH;
						else
							break;
					lpWhile->Type = OS_PUSH | OS_HAS_EXPR | OS_POST;
					lpEnd->Tag = TAG_WHILE_EXPR;
					lpEnd->Type |= OS_PUSH | OS_POST | OS_LOOP_END;
					if (!lpFirstDoWhileLoop)
						lpFirstDoWhileLoop = lpMarkup1;
				}
				continue;
			case TAG_WHILE:
				// correct while loop
				{
					MARKUP *lpOpen, *lpClose, *lpEnd, *lpElse;

					if (lpMarkup1->Type & OS_POST)
						continue;
					if ((lpOpen = lpMarkup1 + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = lpClose = FindParenthesisClose(lpOpen + 1, lpEndOfMarkup)) + 1 >= lpEndOfMarkup)
						break;
					if (lpEnd[1].Tag != TAG_ELSE && (lpEnd = FindEndOfStructuredStatement(lpEnd + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) < lpEndOfMarkup && lpElse->Tag == TAG_ELSE)
						lpElse->Type = OS_PUSH;
					lpClose->Tag = TAG_WHILE_EXPR;
					lpClose->Type |= OS_PUSH | OS_SPLIT;
					lpEnd->Type |= OS_PUSH | OS_SPLIT | OS_LOOP_END;
				}
				continue;
			case TAG_FOR:
				// correct for loop
				{
					MARKUP *lpOpen, *lpInitialize, *lpCondition, *lpUpdate, *lpEnd, *lpElse;

					if ((lpOpen = lpMarkup1 + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = lpUpdate = FindParenthesisClose(lpOpen + 1, lpEndOfMarkup)) + 1 >= lpEndOfMarkup)
						break;
					if ((lpInitialize = FindSplit(lpOpen + 1, lpUpdate)) >= lpUpdate)
						break;
					if ((lpCondition = FindSplit(lpInitialize + 1, lpUpdate)) >= lpUpdate)
						break;
					if (lpEnd[1].Tag != TAG_ELSE && (lpEnd = FindEndOfStructuredStatement(lpEnd + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) < lpEndOfMarkup && lpElse->Tag == TAG_ELSE)
						lpElse->Type = OS_PUSH;
					lpInitialize->Tag = TAG_FOR_INITIALIZE;
					lpInitialize->Type |= OS_PUSH;
					lpCondition->Tag = TAG_FOR_CONDITION;
					lpCondition->Type |= OS_PUSH | OS_LOOP_BEGIN;
					lpUpdate->Tag = TAG_FOR_UPDATE;
					lpUpdate->Type |= OS_PUSH | OS_SPLIT;
					lpEnd->Type |= OS_PUSH | OS_SPLIT | OS_LOOP_END;
				}
				continue;
			case TAG_PARSE_INT:     // parse_int
			case TAG_PARSE_REAL:    // parse_real
			case TAG_PARSE_RESET:   // parse_reset
				{
					MARKUP *lpOpen, *lpClose;

					if ((lpOpen = lpMarkup1 + 1) + 2 >= lpEndOfMarkup)
						continue;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						continue;
					if (lpOpen[1].Tag == TAG_PARENTHESIS_CLOSE)
						continue;
					if ((lpClose = FindParenthesisClose(lpOpen + 2, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					lpMarkup1->Type &= ~OS_MONADIC;
					(lpMarkup1->Close = lpClose)->Type |= OS_PUSH;
				}
				continue;
			case TAG_RAND32:        // rand32
			case TAG_RAND64:        // rand64
#if USE_PLUGIN
			case TAG_PLUGIN:        // plugin function
#endif
				if (CorrectFunction(lpMarkup1, lpEndOfMarkup, 0))
					continue;
				break;
			case TAG_MNAME:         // MName
			case TAG_PROCESSID:     // ProcessId
			case TAG_HNUMBER:       // HNumber
			case TAG_MEMORY:        // Memory
			case TAG_CAST32:        // Cast32
			case TAG_CAST64:        // Cast64
			case TAG_I1TOI4:        // I1toI4
			case TAG_I2TOI4:        // I2toI4
			case TAG_I4TOI8:        // I4toI8
			case TAG_UTOF:          // utof
			case TAG_ITOF:          // itof
			case TAG_FTOI:          // ftoi
			case TAG_TRUNC:         // trunc
			case TAG_ROUND:         // round
			case TAG_BSF:           // BitScanForward
			case TAG_BSR:           // BitScanReverse
			case TAG_A2U:           // A2U
			case TAG_A2W:           // A2W
			case TAG_U2A:           // U2A
			case TAG_U2W:           // U2W
			case TAG_W2A:           // W2A
			case TAG_W2U:           // W2U
			case TAG_WAIT:          // wait
			case TAG_SLEEP:         // sleep
			case TAG_ALLOCA:        // alloca
			case TAG_ATOI:          // atoi
			case TAG_WTOI:          // wtoi
			case TAG_ATOF:          // atof
			case TAG_WTOF:          // wtof
			case TAG_PRINTF:        // printf
			case TAG_DPRINTF:       // dprintf
			case TAG_STRDUP:        // strdup
			case TAG_WCSDUP:        // wcsdup
			case TAG_STRLEN:        // strlen
			case TAG_WCSLEN:        // wcslen
			case TAG_STRLWR:        // strlwr
			case TAG_WCSLWR:        // wcslwr
			case TAG_MBSLWR:        // mbslwr
			case TAG_STRUPR:        // strupr
			case TAG_WCSUPR:        // wcsupr
			case TAG_MBSUPR:        // mbsupr
			case TAG_STRREV:        // strrev
			case TAG_WCSREV:        // wcsrev
			case TAG_MBSREV:        // mbsrev
			case TAG_ISALNUM:       // isalnum
			case TAG_ISALPHA:       // isalpha
			case TAG_ISASCII:       // isascii
			case TAG_ISBLANK:       // isblank
			case TAG_ISCNTRL:       // iscntrl
			case TAG_ISCSYM:        // iscsym
			case TAG_ISCSYMF:       // iscsymf
			case TAG_ISDIGIT:       // isdigit
			case TAG_ISGRAPH:       // isgraph
			case TAG_ISKANA:        // iskana
			case TAG_ISLEADBYTE:    // isleadbyte
			case TAG_ISLOWER:       // islower
			case TAG_ISMBCHIRA:     // ismbchira
			case TAG_ISMBCKATA:     // ismbckata
			case TAG_ISMBCL0:       // ismbcl0
			case TAG_ISMBCL1:       // ismbcl1
			case TAG_ISMBCL2:       // ismbcl2
			case TAG_ISMBCLEGAL:    // ismbclegal
			case TAG_ISMBCSYMBOL:   // ismbcsymbol
			case TAG_ISPRINT:       // isprint
			case TAG_ISPUNCT:       // ispunct
			case TAG_ISSPACE:       // isspace
			case TAG_ISTRAILBYTE:   // istrailbyte
			case TAG_ISUPPER:       // isupper
			case TAG_ISXDIGIT:      // isxdigit
			case TAG_TOASCII:       // toascii
			case TAG_TOLOWER:       // tolower
			case TAG_TOUPPER:       // toupper
				if (lpMarkup1->Length < 2 || *(uint16_t *)(lpMarkup1->String + lpMarkup1->Length - 2) != '::')
				{
					if (!CorrectFunction(lpMarkup1, lpEndOfMarkup, 1))
						break;
					switch (lpMarkup1->Tag)
					{
					default:
						continue;
					case TAG_MNAME:         // MName
					case TAG_PROCESSID:     // ProcessId
						if (lpMarkup1->NumberOfOperand == 1)
							continue;
						lpMarkup1->Type &= ~OS_MONADIC;
						break;
					}
				}
				else
				{
					MARKUP *lpParam;

					lpParam = lpMarkup1;
					do
						if (++lpParam >= lpEndOfMarkup)
							goto PARSING_ERROR;
					while (lpParam->Tag == TAG_PARENTHESIS_OPEN);
					lpMarkup1->Param = lpParam;
					lpMarkup1->NumberOfOperand = 1;
					continue;
				}
				break;
#if ALLOCATE_SUPPORT
			case TAG_REALLOC:       // realloc
#endif
			case TAG_MIN:           // min
			case TAG_MAX:           // max
			case TAG_IMIN:          // imin
			case TAG_IMAX:          // imax
			case TAG_STRNLEN:       // strnlen
			case TAG_WCSNLEN:       // wcsnlen
			case TAG_STRCMP:        // strcmp
			case TAG_WCSCMP:        // wcscmp
			case TAG_STRICMP:       // stricmp
			case TAG_WCSICMP:       // wcsicmp
			case TAG_MBSICMP:       // mbsicmp
			case TAG_STRCPY:        // strcpy
			case TAG_WCSCPY:        // wcscpy
			case TAG_STRCAT:        // strcat
			case TAG_WCSCAT:        // wcscat
			case TAG_STRCHR:        // strchr
			case TAG_WCSCHR:        // wcschr
			case TAG_MBSCHR:        // mbschr
			case TAG_STRICHR:       // strichr
			case TAG_WCSICHR:       // wcsichr
			case TAG_MBSICHR:       // mbsichr
			case TAG_STRRCHR:       // strrchr
			case TAG_WCSRCHR:       // wcsrchr
			case TAG_MBSRCHR:       // mbsrchr
			case TAG_STRRICHR:      // strrichr
			case TAG_WCSRICHR:      // wcsrichr
			case TAG_MBSRICHR:      // mbsrichr
			case TAG_STRSTR:        // strstr
			case TAG_WCSSTR:        // wcsstr
			case TAG_MBSSTR:        // mbsstr
			case TAG_STRISTR:       // stristr
			case TAG_WCSISTR:       // wcsistr
			case TAG_MBSISTR:       // mbsistr
			case TAG_STRSPN:        // strspn
			case TAG_WCSSPN:        // wcsspn
			case TAG_MBSSPN:        // mbsspn
			case TAG_STRCSPN:       // strcspn
			case TAG_WCSCSPN:       // wcscspn
			case TAG_MBSCSPN:       // mbscspn
			case TAG_STRPBRK:       // strpbrk
			case TAG_WCSPBRK:       // wcspbrk
			case TAG_MBSPBRK:       // mbspbrk
			case TAG_STRSET:        // strset
			case TAG_WCSSET:        // wcsset
			case TAG_MBSSET:        // mbsset
			case TAG_STRTOK:        // strtok
			case TAG_WCSTOK:        // wcstok
			case TAG_MBSTOK:        // mbstok
				if (CorrectFunction(lpMarkup1, lpEndOfMarkup, 2))
					continue;
				break;
			case TAG_MEMCMP:        // memcmp
			case TAG_MEMMOVE:       // memmove
			case TAG_MEMSET:        // memset
			case TAG_MEMSET16:      // memset16
			case TAG_MEMSET32:      // memset32
			case TAG_MEMSET64:      // memset64
			case TAG_SNPRINTF:      // snprintf
			case TAG_SNWPRINTF:     // snwprintf
			case TAG_STRNCMP:       // strncmp
			case TAG_WCSNCMP:       // wcsncmp
			case TAG_STRNICMP:      // strnicmp
			case TAG_WCSNICMP:      // wcsnicmp
			case TAG_MBSNBICMP:     // mbsnbicmp
			case TAG_STRLCPY:       // strlcpy
			case TAG_WCSLCPY:       // wcslcpy
			case TAG_STRLCAT:       // strlcat
			case TAG_WCSLCAT:       // wcslcat
			case TAG_STRNSET:       // strnset
			case TAG_WCSNSET:       // wcsnset
			case TAG_MBSNBSET:      // mbsnbset
				if (CorrectFunction(lpMarkup1, lpEndOfMarkup, 3))
					continue;
				break;
			}
		PARSING_ERROR:
			lpMarkup1->Tag = TAG_PARSE_ERROR;
			lpMarkup1->Type |= OS_PUSH;
		}

		if (lpFirstDoWhileLoop)
		{
			// correct continue of do-while loop
			for (MARKUP *lpMarkup1 = lpFirstDoWhileLoop; lpMarkup1 < lpEndOfMarkup; lpMarkup1++)
			{
				size_t nDepth;

				if (lpMarkup1->Tag != TAG_DO)
					continue;
				nDepth = 0;
				for (MARKUP *lpMarkup2 = lpMarkup1 + 1; lpMarkup2 < lpEndOfMarkup; lpMarkup2++)
				{
					if (lpMarkup2->Type & OS_LOOP_BEGIN)
					{
						nDepth++;
					}
					else if (lpMarkup2->Type & OS_LOOP_END)
					{
						if (!nDepth)
							break;
						nDepth--;
					}
					else if (!nDepth && lpMarkup2->Tag == TAG_CONTINUE)
					{
						lpMarkup2->Type |= OS_POST;
					}
				}
			}
		}
	}

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
			if (lpMarkup[-1].Tag != TAG_NOT_OPERATOR)
				break;
			lpMarkup->Type = OS_PUSH | OS_MONADIC | OS_POST;
			lpMarkup->Priority = PRIORITY_POST_INC_DEC;
			break;
		case TAG_ADD:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & OS_SPLIT))
					break;
			// plus-sign operator (remove)
			lpMarkup->Type = 0;
			break;
		case TAG_SUB:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & OS_SPLIT))
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
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & OS_SPLIT))
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
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & OS_SPLIT))
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
static __inline DWORD IsStringOperand(const MARKUP *element)
{
	return element->Type & OS_STRING;
}
//---------------------------------------------------------------------
static BOOLEAN __fastcall CheckStringOperand(const MARKUP *element, size_t *prefixLength)
{
	if (!IsStringOperand(element))
		return FALSE;
	if (element->String[0] == '"')
		*prefixLength = 0;
	else if (element->String[1] == '"')
		*prefixLength = 1;
	else
		*prefixLength = 2;
	return TRUE;
}
//---------------------------------------------------------------------
static BOOL __fastcall UnescapeConstStrings(IN MARKUP *lpMarkupArray, IN MARKUP *lpEndOfMarkup, OUT LPVOID *lplpConstStringBuffer)
{
	LPBYTE lpBuffer, lpFirst, lpLast, p;
	size_t nSizeOfBuffer, nPrefixLength, nSize;
	LPVOID lpConstStringBuffer;
	DWORD  dwProtect;

	*lplpConstStringBuffer = NULL;
	nSizeOfBuffer = 0;
	for (MARKUP *lpMarkup = lpMarkupArray; lpMarkup != lpEndOfMarkup; lpMarkup++)
	{
		if (!CheckStringOperand(lpMarkup, &nPrefixLength))
			continue;

		// assert(1 == strlen(  "\""));
		// assert(2 == strlen( "u\""));
		// assert(3 == strlen("u8\""));

		// assert(1 == strlen(  "ｱ")                  );
		// assert(2 == wcslen( L"ｱ") * sizeof(wchar_t));
		// assert(3 == strlen(u8"ｱ")                  );

		if (nPrefixLength < 1)
			nSize = (lpMarkup->Length - 1) * 1 + 15;
		else if (nPrefixLength == 1)
			nSize = (lpMarkup->Length - 2) * 2 + 15;
		else
			nSize = (lpMarkup->Length - 3) * 3 + 15;
		nSize &= -16;
		nSizeOfBuffer += nSize;
	}
	if (!nSizeOfBuffer)
		return TRUE;
	lpBuffer = HeapAlloc(hHeap, 0, nSizeOfBuffer + 15);
	if (!lpBuffer)
		return FALSE;
	lpLast = (lpFirst = p = (LPBYTE)(((uintptr_t)lpBuffer + 15) & -16)) + nSizeOfBuffer;
	for (MARKUP *lpMarkup = lpMarkupArray; lpMarkup != lpEndOfMarkup; lpMarkup++)
	{
		LPCSTR lpMultiByteStr;
		size_t cbMultiByte;

		if (!CheckStringOperand(lpMarkup, &nPrefixLength))
			continue;
		lpMarkup->UnescapedString = (LPCVOID)(p - lpFirst);
		lpMultiByteStr = lpMarkup->String + nPrefixLength + 1;
		cbMultiByte = lpMarkup->Length - nPrefixLength - 1;
		if (nPrefixLength < 1)
		{
			char *end;

			memcpy(p, lpMultiByteStr, cbMultiByte);
			end = p + cbMultiByte;
			while (*(p = UnescapeA(p, &end, FALSE)) == '"')
			{
				char   *next, c;
				size_t size;

				if ((next = p + 1) >= end)
					break;
				do
					c = *(next++);
				while (__intrinsic_isspace(c) && next < end);
				if (c != '"' || !(size = (end -= next - p) - p))
					break;
				memcpy(p, next, size);
			}
			*(p++) = '\0';
		}
		else if (nPrefixLength == 1)
		{
			#define p ((wchar_t *)p)

			size_t  cchWideChar;
			wchar_t *end;

			cchWideChar = (unsigned int)MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, p, (LPWSTR)lpLast - p);
			end = p + cchWideChar;
			while (*(p = UnescapeW(p, &end, FALSE)) == L'"')
			{
				wchar_t *next, c;
				size_t  size;

				if ((next = p + 1) >= end)
					break;
				do
					c = *(next++);
				while (__intrinsic_iswspace(c) && next < end);
				if (c != L'u' || next >= end || *(next++) != L'"' || !(size = ((char *)end -= (char *)next - (char *)p) - (char *)p))
					break;
				memcpy(p, next, size);
			}
			*(p++) = L'\0';

			#undef p
		}
		else
		{
			size_t cchWideChar;

			if (cchWideChar = (unsigned int)MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, NULL, 0))
			{
				LPWSTR        lpWideCharStr;
				size_t        cbUtf8;
				unsigned char *end;

				if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar * sizeof(wchar_t))))
					goto FAILED;
				MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
				cbUtf8 = (unsigned int)WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, p, lpLast - p, NULL, NULL);
				HeapFree(hHeap, 0, lpWideCharStr);
				end = p + cbUtf8;
				while (*(p = UnescapeU(p, &end, FALSE)) == '"')
				{
					unsigned char *next, c;
					size_t        size;

					if ((next = p + 1) >= end)
						break;
					do
						c = *(next++);
					while (__intrinsic_isspace(c) && next < end);
					if (c != 'u' || next >= end || *(next++) != '8' || next >= end || *(next++) != '"' || !(size = (end -= next - p) - p))
						break;
					memcpy(p, next, size);
				}
			}
			*(p++) = '\0';
		}
		if ((uintptr_t)p & 1)
			*(p++) = 0;
		if ((uintptr_t)p & 2)
			*(((uint16_t *)p)++) = 0;
		if ((uintptr_t)p & 4)
			*(((uint32_t *)p)++) = 0;
		if ((uintptr_t)p & 8)
			*(((uint64_t *)p)++) = 0;
	}
	nSize = p - lpFirst;
	lpConstStringBuffer = VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_READWRITE);
	if (!lpConstStringBuffer)
		goto FAILED;
	memcpy(lpConstStringBuffer, lpFirst, nSize);
	VirtualProtect(lpConstStringBuffer, nSize, PAGE_READONLY, &dwProtect);
	HeapFree(hHeap, 0, lpBuffer);
	for (MARKUP *lpMarkup = lpMarkupArray; lpMarkup != lpEndOfMarkup; lpMarkup++)
		if (IsStringOperand(lpMarkup))
			(LPBYTE)lpMarkup->UnescapedString += (size_t)lpConstStringBuffer;
	*lplpConstStringBuffer = lpConstStringBuffer;
	return TRUE;

FAILED:
	HeapFree(hHeap, 0, lpBuffer);
	return FALSE;
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
				if (lpMarkup->Type & (OS_SPLIT | OS_TERNARY))
					NEST_PUSH(0);
			}
			else if (lpMarkup->Type & OS_LEFT_ASSIGN)
			{
				if (lpMarkup != lpMarkupArray && (lpMarkup[-1].Type & OS_CLOSE))
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
					while (!lpFactor[0]->Priority && lpFactor + 1 < lpFactorTop) lpFactor++;
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
						if (!nDepth && lpPrev->Priority < lpMarkup->Priority) {
							lpPrev++;
							break;
						} else
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
static LPVOID __fastcall AllocateHeapBuffer(LPVOID **lplpHeapBuffer, size_t *lpnNumberOfHeapBuffer, size_t cbSize)
{
	LPVOID *lpHeapBuffer;
	size_t nNumberOfHeapBuffer;
	LPVOID lpBuffer;

	lpHeapBuffer = *lplpHeapBuffer;
	nNumberOfHeapBuffer = *lpnNumberOfHeapBuffer;
	if (lpHeapBuffer)
	{
		if (!(nNumberOfHeapBuffer & 0x0F))
		{
			lpHeapBuffer = (LPVOID *)HeapReAlloc(hHeap, 0, lpHeapBuffer, (nNumberOfHeapBuffer + 0x10) * sizeof(LPVOID));
			if (!lpHeapBuffer)
				return NULL;
			*lplpHeapBuffer = lpHeapBuffer;
		}
	}
	else
	{
		lpHeapBuffer = (LPVOID *)HeapAlloc(hHeap, 0, 0x10 * sizeof(LPVOID));
		if (!lpHeapBuffer)
			return NULL;
		*lplpHeapBuffer = lpHeapBuffer;
	}
	lpBuffer = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cbSize);
	if (!lpBuffer)
		return NULL;
	lpHeapBuffer[nNumberOfHeapBuffer] = lpBuffer;
	*lpnNumberOfHeapBuffer = nNumberOfHeapBuffer + 1;
	return lpBuffer;
}
//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数」
//---------------------------------------------------------------------
uint64_t __cdecl InternalParsing(TSSGCtrl *this, TSSGSubject *SSGS, const string *Src, BOOL IsInteger, va_list ArgPtr)
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
	TPrologueAttribute             *variable;
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
	MARKUP                         *lpEndOfMarkup;
	LPVOID                         lpConstStringBuffer;
	MARKUP                         **lpPostfixBuffer, **lpPostfix, **lpEndOfPostfix;
	VARIABLE                       *lpOperandBuffer, *lpEndOfOperand, *lpOperandTop;
	MARKUP_VARIABLE                *lpVariable;
	size_t                         nNumberOfVariable;
	size_t                         nNumberOfPostfix;
	size_t                         length;
	HANDLE                         hProcess;
	LPVOID                         *lpHeapBuffer;
	size_t                         nNumberOfHeapBuffer;
	HANDLE                         strtok_process;
	HANDLE                         wcstok_process;
	HANDLE                         mbstok_process;
	char                           *strtok_context;
	wchar_t                        *wcstok_context;
	unsigned char                  *mbstok_context;
	BOOLEAN                        bCompoundAssign;
	VARIABLE                       operand;
#if REPEAT_INDEX
	LPSTR                          lpVariableStringBuffer;
#endif

	qwResult = 0;
	lpszSrc = NULL;
	lpMarkupArray = NULL;
	lpConstStringBuffer = NULL;
	lpPostfixBuffer = NULL;
	lpOperandBuffer = NULL;
	lpVariable = NULL;
#if REPEAT_INDEX
	lpVariableStringBuffer = NULL;
#endif
	nNumberOfVariable = 0;
	hProcess = NULL;
	lpHeapBuffer = NULL;
	nNumberOfHeapBuffer = 0;

	p = string_begin(Src) - 1;
	do
		c = *(++p);
	while (__intrinsic_isspace(c));

#if LOCAL_MEMORY_SUPPORT
	if (*p == 'L')
	{
		c = p[1];
		if (__intrinsic_isascii(c))
			if (__intrinsic_isspace(c))
			{
				p++;
				do
					c = *(++p);
				while (__intrinsic_isspace(c));
			}
			else if (!__intrinsic_isalnum(c) && c != '_')
				p++;
	}
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
		goto ALLOC_ERROR;
	memcpy(lpszSrc, p, nSrcLength);
	lpszSrc[nSrcLength] = '\0';
#else
	variable = (TPrologueAttribute*)TSSGCtrl_GetAttribute(this, SSGS, atPROLOGUE);
	if (variable && (nVariableLength = string_length(code = TEndWithAttribute_GetCode(variable))))
	{
		unsigned long bits;

#ifndef _WIN64
		_BitScanReverse(&bits, nVariableLength + nSrcLength + sizeof(uint32_t) - 1);
#else
		_BitScanReverse64(&bits, nVariableLength + nSrcLength + sizeof(uint32_t) - 1);
#endif
		capacity = (size_t)1 << (bits + 1);
		if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
			goto ALLOC_ERROR;
		memcpy(lpszSrc, string_c_str(code), nVariableLength);
		memcpy(lpszSrc + nVariableLength, p, nSrcLength);
		nSrcLength += nVariableLength;
	}
	else
	{
		unsigned long bits;

#ifndef _WIN64
		_BitScanReverse(&bits, nSrcLength + sizeof(uint32_t) - 1);
#else
		_BitScanReverse64(&bits, nSrcLength + sizeof(uint32_t) - 1);
#endif
		capacity = (size_t)1 << (bits + 1);
		if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
			goto ALLOC_ERROR;
		memcpy(lpszSrc, p, nSrcLength);
	}
	lpszSrc[nSrcLength] = '\0';

	// remove the c style comments
	if (nSrcLength >= 2)
	{
		char *end, *p1, *p2, c1, c2;

		end = (p1 = lpszSrc) + nSrcLength;
		c1 = *(p1++);
		do
		{
			switch (c1)
			{
			case '"':
			case '\'':
				while ((c2 = *(p1++)) != c1 && p1 < end)
				{
					if (!__intrinsic_isleadbyte(c2))
					{
						if (c2 != '\\')
							continue;
						c2 = *(p1++);
						if (p1 >= end)
							break;
						if (!__intrinsic_isleadbyte(c2))
							continue;
					}
					if (++p1 >= end)
						break;
				}
				break;
			case '/':
				switch (*p1)
				{
				case '*':
					// block comment
					p2 = p1;
					p1--;
					p2++;
					for (; ; )
					{
						c1 = *(p2++);
						if (p2 < end)
						{
							if (c1 != '*' || *p2 != '/')
							{
								if (!__intrinsic_isleadbyte(c1) || ++p2 < end)
									continue;
							}
							else
							{
								p2++;
								memcpy(p1, p2, (end -= p2 - p1) - p1 + 1);
								break;
							}
						}
						*(end = p1) = '\0';
						break;
					}
					break;
				case '/':
					// end of line comment
					p2 = p1;
					p1--;
					p2++;
					for (; ; )
					{
						c1 = *(p2++);
						if (p2 < end)
						{
							switch (c1)
							{
							default:
								if (!__intrinsic_isleadbyte(c1) || ++p2 < end)
									continue;
								break;
							case '\r':
								if (*p2 == '\n')
									p2++;
							case '\n':
								memcpy(p1, p2, (end -= p2 - p1) - p1 + 1);
								goto END_OF_LINE_COMMENT_NESTED_BREAK;
							}
							break;
						}
						*(end = p1) = '\0';
						break;
					}
					break;
				}
				break;
#if CODEPAGE_SUPPORT
			default:
				if (!__intrinsic_isleadbyte(c1))
					break;
#else
			case_unsigned_leadbyte:
#endif
				p1++;
				break;
			}
		END_OF_LINE_COMMENT_NESTED_BREAK:
			c1 = *(p1++);
		} while (p1 < end);
		nSrcLength = end - lpszSrc;
	}

	attributes = SSGS->type// check for TSSGCtrl::LoopSSRFile
		? TSSGSubject_GetAttribute(SSGS)
		: TSSGAttributeSelector_GetNowAtteributeVec(TSSGCtrl_GetAttributeSelector(this));
	if (attributes)
	{
		LPVOID lpMem;

		nSrcLength = ReplaceDefineByHeap(attributes, &lpszSrc, nSrcLength, capacity);
		if (nSrcLength == SIZE_MAX)
			goto ALLOC_ERROR;
		if (!(lpMem = HeapReAlloc(hHeap, 0, lpszSrc, nSrcLength + sizeof(uint32_t))))
			goto ALLOC_ERROR;
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

	*(uint32_t *)&lpszSrc[nSrcLength] = '\0';

	lpMarkupArray = Markup(lpszSrc, nSrcLength, &nNumberOfMarkup);
	if (!lpMarkupArray)
		goto ALLOC_ERROR;
	lpEndOfMarkup = lpMarkupArray + nNumberOfMarkup;

	if (!UnescapeConstStrings(lpMarkupArray, lpEndOfMarkup, &lpConstStringBuffer))
		goto ALLOC_ERROR;

	lpPostfixBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpPostfixBuffer)
		goto ALLOC_ERROR;
	nNumberOfPostfix = Postfix(lpMarkupArray, nNumberOfMarkup, lpPostfixBuffer);
	if (!nNumberOfPostfix)
		goto ALLOC_ERROR;

	lpOperandBuffer = (VARIABLE *)HeapAlloc(hHeap, 0, sizeof(VARIABLE) * (nNumberOfPostfix + 1));
	if (!lpOperandBuffer)
		goto ALLOC_ERROR;
	lpVariable = (MARKUP_VARIABLE *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(MARKUP_VARIABLE) * 0x10);
	if (!lpVariable)
		goto ALLOC_ERROR;

	#define OPERAND_IS_EMPTY()  (lpEndOfOperand == lpOperandBuffer)
	#define OPERAND_PUSH(value) (*(lpOperandTop = lpEndOfOperand++) = (value))
	#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpEndOfOperand = lpOperandTop != lpOperandBuffer ? lpOperandTop-- : lpOperandTop) : operandZero)
	#define OPERAND_CLEAR()     (*(lpOperandTop = lpEndOfOperand = lpOperandBuffer) = operandZero)

	bInitialIsInteger = IsInteger;
	if (nNumberOfMarkup)
		if (lpMarkupArray->Type & OS_MONADIC)
			if (lpMarkupArray->Tag == TAG_PARSE_INT)
				IsInteger = TRUE;
			else if (lpMarkupArray->Tag == TAG_PARSE_REAL)
				IsInteger = FALSE;
	operandZero.Quad = 0;
	operandZero.IsQuad = !IsInteger;
	OPERAND_CLEAR();
#if SUBJECT_STATUS
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
			goto ALLOC_ERROR;
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
					goto ALLOC_ERROR;
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
					goto ALLOC_ERROR;
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
					goto ALLOC_ERROR;
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

	strtok_process = NULL;
	wcstok_process = NULL;
	mbstok_process = NULL;
	strtok_context = NULL;
	wcstok_context = NULL;
	mbstok_context = NULL;
	bCompoundAssign = FALSE;
	for (lpEndOfPostfix = (lpPostfix = lpPostfixBuffer) + nNumberOfPostfix; lpPostfix < lpEndOfPostfix; lpPostfix++)
	{
		MARKUP   *lpMarkup;
		BOOLEAN  boolValue;
		size_t   nDepth;
		LPVOID   lpAddress;
		size_t   nSize;
		MARKUP   *lpNext;
		LPSTR    lpGuideText;
#if !defined(__BORLANDC__)
		size_t   nGuideTextLength;
#endif
		LPCSTR   lpMessage;
		DWORD    dw;
		uint64_t qw;

		lpMarkup = *lpPostfix;
		switch (lpMarkup->Tag)
		{
		case TAG_IF:
			OPERAND_CLEAR();
			if (lpMarkup->Length)
				break;
			continue;
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
			if (lpMarkup->FalsePart)
			{
				if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
					break;
			}
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->Depth > lpMarkup->Depth);
				if (lpPostfix < lpEndOfPostfix && (*lpPostfix)->Tag == TAG_ELSE && (*lpPostfix)->Depth == lpMarkup->Depth)
				{
					lpMarkup = *(lpMarkup->FalsePart = lpPostfix);
					break;
				}
				lpMarkup->FalsePart = --lpPostfix;
			}
			continue;
		case TAG_ELSE:
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->Depth > lpMarkup->Depth);
				lpMarkup->Jump = --lpPostfix;
			}
			continue;
		case TAG_WHILE_EXPR:
			boolValue = OPERAND_IS_EMPTY() || (IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real);
			OPERAND_CLEAR();
			if (boolValue)
			{
				if (lpMarkup->TruePart)
					lpPostfix = lpMarkup->TruePart;
				else
				{
					if (!(lpMarkup->Type & OS_POST))
					{
						if (lpMarkup->Type & OS_LOOP_END)
							goto LOOP_END;
					}
					else
					{
						if (lpPostfix > lpPostfixBuffer)
							while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
					}
					lpMarkup->TruePart = lpPostfix;
				}
			}
			else
			{
				if (lpMarkup->FalsePart)
				{
					if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
						break;
				}
				else
				{
					if (!(lpMarkup->Type & OS_POST))
					{
						nDepth = lpMarkup->Depth;
						if (lpMarkup->Type & OS_LOOP_END)
							goto LOOP_ELSE;
						nDepth--;
						while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
					}
					else
					{
						nDepth = lpMarkup->Depth;
					}
					goto LOOP_ELSE;
				}
			}
			continue;
		case TAG_FOR_INITIALIZE:
			OPERAND_CLEAR();
			continue;
		case TAG_FOR_CONDITION:
			boolValue = OPERAND_IS_EMPTY() || (IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real);
			OPERAND_CLEAR();
			if (boolValue)
			{
				if (lpMarkup->TruePart)
					lpPostfix = lpMarkup->TruePart;
				else
				{
					while (++lpPostfix < lpEndOfPostfix)
						if ((*lpPostfix)->Tag == TAG_FOR_UPDATE)
							if (!((*lpPostfix)->Type & OS_LOOP_END))
								break;
							else
								goto LOOP_END;
					lpMarkup->TruePart = lpPostfix;
				}
			}
			else
			{
				if (lpMarkup->FalsePart)
				{
					if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
						break;
				}
				else
				{
					while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
					nDepth = lpMarkup->Depth;
				LOOP_ELSE:
					if (++lpPostfix < lpEndOfPostfix &&
						(*lpPostfix)->Tag == TAG_ELSE && (*lpPostfix)->Depth == nDepth) {
						lpMarkup = *(lpMarkup->FalsePart = lpPostfix);
						break;
					}
					lpMarkup->FalsePart = --lpPostfix;
				}
			}
			continue;
		case TAG_FOR_UPDATE:
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				if (lpPostfix > lpPostfixBuffer)
					while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->Tag != TAG_FOR_INITIALIZE);
				lpMarkup->Jump = lpPostfix;
			}
			continue;
		case TAG_PARENTHESIS_CLOSE:
		case TAG_SPLIT:
			if (!(lpMarkup->Type & OS_LOOP_END))
				continue;
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
			LOOP_END:
				if (lpPostfix > lpPostfixBuffer)
					while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
				lpMarkup->Jump = lpPostfix;
			}
			continue;
		case TAG_BREAK:
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
				lpMarkup->Jump = lpPostfix;
			}
			break;
		case TAG_CONTINUE:
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				if (lpMarkup->Type & OS_POST)
				{
					while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth)
						if ((*lpPostfix)->LoopDepth == lpMarkup->LoopDepth && (*lpPostfix)->Tag == TAG_WHILE)
							break;
					lpPostfix--;
				}
				else
				{
					if (lpPostfix > lpPostfixBuffer)
						while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
				}
				lpMarkup->Jump = lpPostfix;
			}
			break;
		case TAG_PARAM_LOCAL:
		case TAG_IMPORT_FUNCTION:
		case TAG_IMPORT_REFERENCE:
			continue;
		case TAG_RETURN:
			break;
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
				lpPostfix -= 2;
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
				lpPostfix -= 2;
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
				lpPostfix -= 2;
			break;
		case TAG_IDIV:
			if (IsInteger)
			{
				BOOL IsQuad;

				operand = OPERAND_POP();
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto DIVIDE_BY_ZERO;
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
					goto DIVIDE_BY_ZERO;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low /= operand.Low;
				else
					lpOperandTop->Quad /= operand.Quad;
			}
			else
			{
				if (!operand.Real)
					goto DIVIDE_BY_ZERO;
				lpOperandTop->Real /= operand.Real;
			}
			if (bCompoundAssign)
				lpPostfix -= 2;
			break;
		case TAG_IMOD:
			if (IsInteger)
			{
				BOOL IsQuad;

				operand = OPERAND_POP();
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto DIVIDE_BY_ZERO;
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
					goto DIVIDE_BY_ZERO;
				if (!(lpOperandTop->IsQuad |= operand.IsQuad))
					lpOperandTop->Low %= operand.Low;
				else
					lpOperandTop->Quad %= operand.Quad;
			}
			else
			{
				if (!operand.Real)
					goto DIVIDE_BY_ZERO;
				lpOperandTop->Real = fmod(lpOperandTop->Real, operand.Real);
				lpOperandTop->IsQuad = TRUE;
			}
			if (bCompoundAssign)
				lpPostfix -= 2;
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
				lpPostfix -= 2;
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
				lpPostfix -= 2;
			break;
		case TAG_ROL:
			operand = OPERAND_POP();
			if (!IsInteger)
			{
				int32_t msw;

				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
					if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
				lpPostfix -= 2;
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
				lpPostfix -= 2;
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
				lpPostfix -= 2;
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
					while (++lpPostfix < lpEndOfPostfix)
					{
						if ((*lpPostfix)->Tag != TAG_AND)
							continue;
						if ((*lpPostfix)->String != lpMarkup->String)
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
					while (++lpPostfix < lpEndOfPostfix)
					{
						if ((*lpPostfix)->Tag != TAG_OR)
							continue;
						if ((*lpPostfix)->String != lpMarkup->String)
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
				if (lpMarkup->FalsePart)
					lpPostfix = lpMarkup->FalsePart;
				else
				{
					size_t nDepth;

					nDepth = 0;
					while (++lpPostfix < lpEndOfPostfix)
					{
						if ((*lpPostfix)->Tag == TAG_TERNARY)
						{
							nDepth++;
						}
						else if ((*lpPostfix)->Tag == TAG_TERNARY_SPLIT)
						{
							if (!nDepth)
								break;
							nDepth--;
						}
					}
					lpMarkup->FalsePart = lpPostfix;
				}
			}
			break;
		case TAG_TERNARY_SPLIT:
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				for (; lpPostfix + 1 < lpEndOfPostfix; lpPostfix++)
					if (lpPostfix[1]->Type & OS_TERNARY_END)
						break;
				lpMarkup->Jump = lpPostfix;
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
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto OPEN_ERROR;
			if (bCompoundAssign)
			{
				VARIABLE op1 = OPERAND_POP();
				VARIABLE op2 = *lpOperandTop;
				*lpOperandTop = op1;
				OPERAND_PUSH(op2);
			}
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (!bCompoundAssign && (!(lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
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
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
				lpPostfix++;
				if (bCompoundAssign)
				{
					bCompoundAssign = FALSE;
					continue;
				}
			}
			if (lpMarkup->Tag != TAG_INDIRECTION)
				break;
			if (!TSSGCtrl_GetSSGActionListner(this))
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
			if (!bCompoundAssign && (!(lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
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
								if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
								if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
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
				lpPostfix++;
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
			if (TSSGCtrl_AddressAttributeFilter(this, SSGS, (unsigned long *)&lpOperandTop->Quad, atREPLACE) != 0)
				goto FAILED_ADDR_REPLACE;
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
			if (TSSGCtrl_AddressAttributeFilter(this, SSGS, (unsigned long *)&lpOperandTop->Quad, atADJUST) != 0)
				goto FAILED_ADDR_ADJUST;
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
			{
				size_t length;
				LPSTR  p;
				size_t i;

				if (lpPostfix + 1 >= lpEndOfPostfix)
					goto PARSING_ERROR;
				lpNext = lpPostfix[1];
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
				for (i = 0; i < nNumberOfVariable; i++)
					if (lpVariable[i].Length == length)
						if (memcmp(lpVariable[i].String, p, length) == 0)
							break;
				if (i < nNumberOfVariable)
				{
					lpVariable[i].Value = *lpOperandTop;
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
							goto ALLOC_ERROR;
						lpVariable = (MARKUP_VARIABLE *)lpMem;
					}
					lpVariable[nNumberOfVariable].Length = length;
					lpVariable[nNumberOfVariable].String = p;
					lpVariable[nNumberOfVariable].Value = *lpOperandTop;
					nNumberOfVariable++;
				}
				lpPostfix++;
			}
			break;
		case TAG_PARSE_INT:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = TRUE;
			operandZero.IsQuad = FALSE;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PARSE_REAL:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = FALSE;
			operandZero.IsQuad = TRUE;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PARSE_RESET:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = bInitialIsInteger;
			operandZero.IsQuad = !IsInteger;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PROCEDURE:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto OPEN_ERROR;
			operand = OPERAND_POP();
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!operand.High && IS_INTRESOURCE(operand.Low))
			{
				lpOperandTop->Quad = (uintptr_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, (LPSTR)operand.Quad);
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
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto OPEN_ERROR;
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
				if (lpFunction && lpMarkup[-1].Tag == TAG_IMPORT_FUNCTION)
					if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
						lpFunction = NULL;
				lpOperandTop->Quad = (uintptr_t)lpFunction;
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
			{
				THeapListData *HeapL;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (!lpOperandTop->High && (HeapL = TProcessCtrl_GetHeapList(&this->processCtrl, lpOperandTop->Low - 1)))
					if (IsInteger)
						lpOperandTop->Quad = HeapL->heapListAddress;
					else
						lpOperandTop->Real = HeapL->heapListAddress;
				else
					lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger || sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(uint32_t);
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_MEMORY:
			{
				LPVOID lpAddress;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				lpAddress = NULL;
				if (!lpOperandTop->High)
				{
					FILETIME creationTime;

					creationTime.dwHighDateTime = creationTime.dwLowDateTime = 0;
					for (size_t i = 0; i < nNumberOfProcessMemory; i++)
					{
						size_t allocSize;

						if (lpOperandTop->Low != lpProcessMemory[i].Id)
							continue;
						if (lpProcessMemory[i].Address)
						{
							lpAddress = lpProcessMemory[i].Address;
							break;
						}
						allocSize = lpProcessMemory[i].Size;
#ifdef _WIN64
						if (!IsInteger)
							allocSize += ULL2DBL_LOST_MAX;
#endif
						if (lpProcessMemory[i].Protect)
						{
							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED_GET_PROCESS_TIMES;
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
							lpProcessMemory[i].Address = lpAddress = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, lpProcessMemory[i].Protect);
						}
						else
						{
							lpProcessMemory[i].Address = lpAddress = HeapAlloc(pHeap, HEAP_ZERO_MEMORY, allocSize);
						}
						break;
					}
				}
				lpOperandTop->Quad = (uintptr_t)lpAddress;
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(LPVOID) > sizeof(uint32_t);
				}
				else
				{
#ifdef _WIN64
					lpOperandTop->Quad = (lpOperandTop->Quad + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
#endif
		case TAG_CAST32:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
			{
			CLEAR_HIGH_DWORD:
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (bInitialIsInteger)
			{
				*(float *)&lpOperandTop->Low = (float)lpOperandTop->Real;
				goto CLEAR_HIGH_DWORD;
			}
			else
			{
				// floating-point calculation is always 64bit(double).
				// this cast is lost value of 'double' to 'float'.
				lpOperandTop->Real = (float)lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_CAST64:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger && bInitialIsInteger && !lpOperandTop->IsQuad)
				lpOperandTop->Real = *(float *)&lpOperandTop->Low;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_I1TOI4:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (uint32_t)(int8_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I2TOI4:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (uint32_t)(int16_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I4TOI8:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (int64_t)(int32_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_UTOF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Real = (double)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_ITOF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Real = lpOperandTop->IsQuad ? (double)(int64_t)lpOperandTop->Quad : (double)(int32_t)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_FTOI:
			{
				int32_t msw;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
						lpOperandTop->High = 0;
			}
			break;
		case TAG_TRUNC:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
			{
				lpOperandTop->Real = trunc(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ROUND:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
			{
				lpOperandTop->Real = round(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_BSF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
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
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
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
			break;
		case TAG_A2U:
			{
				LPSTR  lpBuffer;
				LPCSTR lpMultiByteStr;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				if (lpMarkup->Param->Tag != TAG_NOT_OPERATOR || !lpMarkup->Param->Length || *lpMarkup->Param->String != '"')
					lpMultiByteStr = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				else
					lpMultiByteStr = lpMarkup->Param->String + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpMultiByteStr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpMultiByteStr, lpBuffer, length, NULL))
					{
						lpBuffer[length] = '\0';
						lpMultiByteStr = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0))
				{
					LPWSTR  lpWideCharStr;
					int     cbMultiByte;

					if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar * sizeof(wchar_t))))
						goto A2U_ALLOC_ERROR;
					MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
					if (cbMultiByte = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL))
					{
						size_t nAllocSize;
						LPSTR  lpUtf8Str;

						nAllocSize = cbMultiByte;
#ifdef _WIN64
						if (!IsInteger)
							nAllocSize += ULL2DBL_LOST_MAX;
#endif
						if (lpUtf8Str = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize))
						{
#ifdef _WIN64
							*(size_t *)&lpUtf8Str = ((size_t)lpUtf8Str + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
							lpOperandTop->Quad = (size_t)lpUtf8Str;
							WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, lpUtf8Str, cbMultiByte, NULL, NULL);
						}
						else
						{
							HeapFree(hHeap, 0, lpWideCharStr);
							goto A2U_ALLOC_ERROR;
						}
					}
					HeapFree(hHeap, 0, lpWideCharStr);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			A2U_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_A2W:
			{
				LPSTR  lpBuffer;
				LPCSTR lpMultiByteStr;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				if (lpMarkup->Param->Tag != TAG_NOT_OPERATOR || !lpMarkup->Param->Length || *lpMarkup->Param->String != '"')
					lpMultiByteStr = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				else
					lpMultiByteStr = lpMarkup->Param->String + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpMultiByteStr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpMultiByteStr, lpBuffer, length, NULL))
					{
						lpBuffer[length] = '\0';
						lpMultiByteStr = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0))
				{
					size_t nAllocSize;
					LPWSTR lpWideCharStr;

					nAllocSize = (size_t)cchWideChar * sizeof(wchar_t);
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpWideCharStr = (LPWSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto A2W_ALLOC_ERROR;
#ifdef _WIN64
					*(size_t *)&lpWideCharStr = ((size_t)lpWideCharStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpWideCharStr;
					MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			A2W_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_U2A:
			{
				LPSTR  lpBuffer;
				LPCSTR lpUtf8Str;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpUtf8Str = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpUtf8Str, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpUtf8Str, lpBuffer, length, NULL))
					{
						lpBuffer[length] = '\0';
						lpUtf8Str = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, NULL, 0))
				{
					LPWSTR lpWideCharStr;
					int    cbMultiByte;

					if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar * sizeof(wchar_t))))
						goto U2A_ALLOC_ERROR;
					MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, lpWideCharStr, cchWideChar);
					if (cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL))
					{
						size_t nAllocSize;
						LPSTR  lpMultiByteStr;

						nAllocSize = cbMultiByte;
#ifdef _WIN64
						if (!IsInteger)
							nAllocSize += ULL2DBL_LOST_MAX;
#endif
						if (lpMultiByteStr = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize))
						{
#ifdef _WIN64
							*(size_t *)&lpMultiByteStr = ((size_t)lpMultiByteStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
							lpOperandTop->Quad = (size_t)lpMultiByteStr;
							WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, NULL, NULL);
						}
						else
						{
							HeapFree(hHeap, 0, lpWideCharStr);
							goto U2A_ALLOC_ERROR;
						}
					}
					HeapFree(hHeap, 0, lpWideCharStr);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			U2A_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_U2W:
			{
				LPSTR  lpBuffer;
				LPCSTR lpUtf8Str;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpUtf8Str = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpUtf8Str, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpUtf8Str, lpBuffer, length, NULL))
					{
						lpBuffer[length] = '\0';
						lpUtf8Str = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, NULL, 0))
				{
					size_t nAllocSize;
					LPWSTR lpWideCharStr;

					nAllocSize = (size_t)cchWideChar * sizeof(wchar_t);
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpWideCharStr = (LPWSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto U2W_ALLOC_ERROR;
#ifdef _WIN64
					*(size_t *)&lpWideCharStr = ((size_t)lpWideCharStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpWideCharStr;
					MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, lpWideCharStr, cchWideChar);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			U2W_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_W2A:
			{
				LPWSTR  lpBuffer;
				LPCWSTR lpWideCharStr;
				int     cbMultiByte;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpWideCharStr = IsInteger ? (LPCWSTR)lpOperandTop->Quad : (LPCWSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthW(hProcess, lpAddress = (LPVOID)lpWideCharStr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (length + 1) * sizeof(wchar_t));
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpWideCharStr, lpBuffer, length * sizeof(wchar_t), NULL))
					{
						lpBuffer[length] = L'\0';
						lpWideCharStr = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL))
				{
					size_t nAllocSize;
					LPSTR  lpMultiByteStr;

					nAllocSize = cbMultiByte;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpMultiByteStr = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto W2A_ALLOC_ERROR;
#ifdef _WIN64
					*(size_t *)&lpMultiByteStr = ((size_t)lpMultiByteStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpMultiByteStr;
					WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, lpMultiByteStr, cbMultiByte, NULL, NULL);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			W2A_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_W2U:
			{
				LPWSTR  lpBuffer;
				LPCWSTR lpWideCharStr;
				int     cbMultiByte;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpWideCharStr = IsInteger ? (LPCWSTR)lpOperandTop->Quad : (LPCWSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthW(hProcess, lpAddress = (LPVOID)lpWideCharStr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (length + 1) * sizeof(wchar_t));
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpWideCharStr, lpBuffer, length * sizeof(wchar_t), NULL))
					{
						lpBuffer[length] = L'\0';
						lpWideCharStr = lpBuffer;
					}
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				lpOperandTop->Quad = 0;
				if (cbMultiByte = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL))
				{
					size_t nAllocSize;
					LPSTR  lpUtf8Str;

					nAllocSize = cbMultiByte;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpUtf8Str = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto W2U_ALLOC_ERROR;
#ifdef _WIN64
					*(size_t *)&lpUtf8Str = ((size_t)lpUtf8Str + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpUtf8Str;
					WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, lpUtf8Str, cbMultiByte, NULL, NULL);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
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

			W2U_ALLOC_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WAIT:
			{
				DWORD dwMilliseconds;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				dwMilliseconds = IsInteger ? (DWORD)lpOperandTop->Quad : (DWORD)lpOperandTop->Real;
				Wait(dwMilliseconds);
			}
			break;
		case TAG_SLEEP:
			{
				DWORD dwMilliseconds;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				dwMilliseconds = IsInteger ? (DWORD)lpOperandTop->Quad : (DWORD)lpOperandTop->Real;
				Sleep(dwMilliseconds);
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_REALLOC:
			{
				MARKUP   *element;
				uint64_t id;
				uint64_t size;
				LPVOID   address;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				id = IsInteger ? (uint64_t)lpOperandTop[0].Quad : (uint64_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				size = IsInteger ? (uint64_t)lpOperandTop[1].Quad : (uint64_t)lpOperandTop[1].Real;
				lpOperandTop->Quad = 0;
				address = NULL;
				if ((sizeof(size_t) == sizeof(uint32_t) ? !(size >> 32) && (uint32_t)size : size) && !(id >> 32))
				{
					FILETIME creationTime;

					creationTime.dwHighDateTime = creationTime.dwLowDateTime = 0;
					for (size_t i = 0; i < nNumberOfProcessMemory; i++)
					{
						size_t oldSize, newSize, allocSize;

						if ((uint32_t)id != lpProcessMemory[i].Id)
							continue;
						address = lpProcessMemory[i].Address;
						oldSize = lpProcessMemory[i].Size;
						newSize = (size_t)size;
						if (lpProcessMemory[i].Protect)
						{
							oldSize = (oldSize + PAGE_SIZE - 1) & -(ptrdiff_t)PAGE_SIZE;
							newSize = (newSize + PAGE_SIZE - 1) & -(ptrdiff_t)PAGE_SIZE;
						}
						if (address && oldSize == newSize)
							break;
						if (lpProcessMemory[i].Protect)
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

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED_GET_PROCESS_TIMES;
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
							protect = lpProcessMemory[i].Protect;
							if (!IsWritableProtect(protect) && lpProcessMemory[i].Address)
								protect = PAGE_READWRITE;
							allocSize = newSize;
#ifdef _WIN64
							if (!IsInteger)
								allocSize += ULL2DBL_LOST_MAX;
#endif
							address = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, protect);
							if (!address)
								break;
							if (lpProcessMemory[i].Address)
							{
								NTSTATUS status;

								if (!IsReadableProtect(lpProcessMemory[i].Protect))
									VirtualProtectEx(hProcess, lpProcessMemory[i].Address, oldSize, PAGE_READONLY, &protect);
								status = MoveProcessMemory(hProcess, address, hProcess, lpProcessMemory[i].Address, min(oldSize, newSize));
								if (NT_SUCCESS(status))
								{
									if (!IsWritableProtect(lpProcessMemory[i].Protect))
										VirtualProtectEx(hProcess, address, newSize, lpProcessMemory[i].Protect, &protect);
									VirtualFreeEx(hProcess, lpProcessMemory[i].Address, 0, MEM_RELEASE);
								}
								else
								{
									if (!IsReadableProtect(lpProcessMemory[i].Protect))
										VirtualProtectEx(hProcess, lpProcessMemory[i].Address, oldSize, lpProcessMemory[i].Protect, &protect);
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
							allocSize = (size_t)size;
#ifdef _WIN64
							if (!IsInteger)
								allocSize += ULL2DBL_LOST_MAX;
#endif
							address =
								lpProcessMemory[i].Address ?
									HeapReAlloc(pHeap, HEAP_ZERO_MEMORY, lpProcessMemory[i].Address, allocSize) :
									HeapAlloc(pHeap, HEAP_ZERO_MEMORY, allocSize);
							if (!address)
								break;
						}
						lpProcessMemory[i].Address = address;
						lpProcessMemory[i].Size = (size_t)size;
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
#ifdef _WIN64
				lpOperandTop->Quad = (lpOperandTop->Quad + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
				lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
#endif
		case TAG_ALLOCA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!(uintptr_t)(lpOperandTop->Quad = (uintptr_t)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, (size_t)lpOperandTop->Quad)))
				goto ALLOC_ERROR;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ATOI:
			{
				MARKUP     *element;
				LPSTR      buffer;
				HANDLE     endptrProcess;
				char       sign, *p;
				const char *nptr;
				char       **endptr;
				int        base;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				buffer = NULL;
				nptr = IsInteger ? (const char *)(uintptr_t)lpOperandTop->Quad : (const char *)(uintptr_t)lpOperandTop->Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)nptr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(buffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, nptr, buffer, nSize, NULL))
						buffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, buffer);
						goto READ_ERROR;
					}
				}
				endptr = NULL;
				base = 0;
				if (element = element->Next)
				{
					if (IsStringOperand(element->Param))
						goto ATOI_PARSING_ERROR;
					endptr = IsInteger ? (char **)(uintptr_t)lpOperandTop->Quad : (char **)(uintptr_t)lpOperandTop->Real;
					if (!endptr || element->Param->Tag == TAG_PARAM_LOCAL)
						endptrProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						endptrProcess = hProcess;
					else
						goto ATOI_OPEN_ERROR;
					if (element = element->Next)
					{
						if (IsStringOperand(element->Param))
							goto ATOI_PARSING_ERROR;
						base = IsInteger ? (int)lpOperandTop->Quad : (int)lpOperandTop->Real;
					}
				}
				p = buffer ? buffer : (char *)nptr;
				do
					sign = *(p++);
				while (__intrinsic_isspace(sign));
				lpOperandTop->Quad = _strtoui64(buffer ? buffer : nptr, endptr ? &p : NULL, base);
				if (endptr)
				{
					if (buffer)
						endptr += nptr - buffer;
					if (endptrProcess)
					{
						if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &p, sizeof(char *), NULL))
							goto ATOI_WRITE_ERROR;
					}
					else
					{
						if (!IsBadWritePtr(lpAddress = endptr, sizeof(char *)))
							*endptr = p;
						else
							goto ATOI_WRITE_ERROR;
					}
				}
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				if (IsInteger)
				{
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = sign != '-' || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				else
				{
					lpOperandTop->Real = sign != '-' ? (double)lpOperandTop->Quad : (double)(int64_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			ATOI_PARSING_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto PARSING_ERROR;

			ATOI_OPEN_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto OPEN_ERROR;

			ATOI_WRITE_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto WRITE_ERROR;
			}
			break;
		case TAG_WTOI:
			{
				MARKUP  *element;
				LPWSTR  buffer;
				HANDLE  endptrProcess;
				wchar_t sign, *p;
				wchar_t *nptr;
				wchar_t **endptr;
				int     base;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				buffer = NULL;
				nptr = IsInteger ? (wchar_t *)(uintptr_t)lpOperandTop[0].Quad : (wchar_t *)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)nptr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(buffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, nptr, buffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)buffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, buffer);
						goto READ_ERROR;
					}
				}
				endptr = NULL;
				base = 0;
				if (element = element->Next)
				{
					if (IsStringOperand(element->Param))
						goto WTOI_PARSING_ERROR;
					endptr = IsInteger ? (wchar_t **)(uintptr_t)lpOperandTop[1].Quad : (wchar_t **)(uintptr_t)lpOperandTop[1].Real;
					if (!endptr || element->Param->Tag == TAG_PARAM_LOCAL)
						endptrProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						endptrProcess = hProcess;
					else
						goto WTOI_OPEN_ERROR;
					if (element = element->Next)
					{
						if (IsStringOperand(element->Param))
							goto WTOI_PARSING_ERROR;
						base = IsInteger ? (int)lpOperandTop[2].Quad : (int)lpOperandTop[2].Real;
					}
				}
				p = buffer ? buffer : nptr;
				do
					sign = *(p++);
				while (__intrinsic_iswspace(sign));
				lpOperandTop->Quad = _wcstoui64(buffer ? buffer : nptr, endptr ? &p : NULL, base);
				if (endptr)
				{
					if (buffer)
						(LPBYTE)endptr += (LPBYTE)nptr - (LPBYTE)buffer;
					if (endptrProcess)
					{
						if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &p, sizeof(wchar_t *), NULL))
							goto WTOI_WRITE_ERROR;
					}
					else
					{
						if (!IsBadWritePtr(lpAddress = endptr, sizeof(wchar_t *)))
							*endptr = p;
						else
							goto WTOI_WRITE_ERROR;
					}
				}
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				if (IsInteger)
				{
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = sign != '-' || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				else
				{
					lpOperandTop->Real = sign != '-' ? (double)lpOperandTop->Quad : (double)(int64_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WTOI_PARSING_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto PARSING_ERROR;

			WTOI_OPEN_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto OPEN_ERROR;

			WTOI_WRITE_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto WRITE_ERROR;
			}
			break;
		case TAG_ATOF:
			{
				MARKUP     *element;
				LPSTR      buffer;
				HANDLE     endptrProcess;
				char       sign, *p;
				const char *nptr;
				char       **endptr;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				buffer = NULL;
				nptr = IsInteger ? (const char *)(uintptr_t)lpOperandTop[0].Quad : (const char *)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)nptr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(buffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, nptr, buffer, nSize, NULL))
						buffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, buffer);
						goto READ_ERROR;
					}
				}
				endptr = NULL;
				if (element = element->Next)
				{
					if (IsStringOperand(element->Param))
						goto ATOF_PARSING_ERROR;
					endptr = IsInteger ? (char **)(uintptr_t)lpOperandTop[1].Quad : (char **)(uintptr_t)lpOperandTop[1].Real;
					if (!endptr || element->Param->Tag == TAG_PARAM_LOCAL)
						endptrProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						endptrProcess = hProcess;
					else
						goto ATOF_OPEN_ERROR;
				}
				if (IsInteger)
				{
					p = buffer ? buffer : (char *)nptr;
					do
						sign = *(p++);
					while (__intrinsic_isspace(sign));
				}
				lpOperandTop->Real = strtod(buffer ? buffer : nptr, endptr ? &p : NULL);
				if (endptr)
				{
					if (buffer)
						endptr += nptr - buffer;
					if (endptrProcess)
					{
						if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &p, sizeof(char *), NULL))
							goto ATOF_WRITE_ERROR;
					}
					else
					{
						if (!IsBadWritePtr(lpAddress = endptr, sizeof(char *)))
							*endptr = p;
						else
							goto ATOF_WRITE_ERROR;
					}
				}
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				if (!IsInteger)
				{
					lpOperandTop->IsQuad = TRUE;
				}
				else
				{
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = sign != '-' || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				break;

			ATOF_PARSING_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto PARSING_ERROR;

			ATOF_OPEN_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto OPEN_ERROR;

			ATOF_WRITE_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto WRITE_ERROR;
			}
			break;
		case TAG_WTOF:
			{
				MARKUP  *element;
				LPWSTR  buffer;
				HANDLE  endptrProcess;
				wchar_t sign, *p;
				wchar_t *nptr;
				wchar_t **endptr;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				buffer = NULL;
				nptr = IsInteger ? (wchar_t *)(uintptr_t)lpOperandTop[0].Quad : (wchar_t *)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)nptr, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(buffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, nptr, buffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)buffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, buffer);
						goto READ_ERROR;
					}
				}
				endptr = NULL;
				if (element = element->Next)
				{
					if (IsStringOperand(element->Param))
						goto WTOF_PARSING_ERROR;
					endptr = IsInteger ? (wchar_t **)(uintptr_t)lpOperandTop[1].Quad : (wchar_t **)(uintptr_t)lpOperandTop[1].Real;
					if (!endptr || element->Param->Tag == TAG_PARAM_LOCAL)
						endptrProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						endptrProcess = hProcess;
					else
						goto WTOF_OPEN_ERROR;
				}
				if (IsInteger)
				{
					p = buffer ? buffer : nptr;
					do
						sign = *(p++);
					while (__intrinsic_iswspace(sign));
				}
				lpOperandTop->Real = wcstod(buffer ? buffer : nptr, endptr ? &p : NULL);
				if (endptr)
				{
					if (buffer)
						(LPBYTE)endptr += (LPBYTE)nptr - (LPBYTE)buffer;
					if (endptrProcess)
					{
						if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &p, sizeof(wchar_t *), NULL))
							goto WTOF_WRITE_ERROR;
					}
					else
					{
						if (!IsBadWritePtr(lpAddress = endptr, sizeof(wchar_t *)))
							*endptr = p;
						else
							goto WTOF_WRITE_ERROR;
					}
				}
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				if (!IsInteger)
				{
					lpOperandTop->IsQuad = TRUE;
				}
				else
				{
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = sign != '-' || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				break;

			WTOF_PARSING_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto PARSING_ERROR;

			WTOF_OPEN_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto OPEN_ERROR;

			WTOF_WRITE_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				goto WRITE_ERROR;
			}
			break;
		case TAG_RAND32:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsInteger)
				{
					lpOperandTop->Quad = rand32();
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = randf32();
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_RAND64:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsInteger)
				{
					lpOperandTop->Quad = rand64();
					lpOperandTop->IsQuad = TRUE;
				}
				else
				{
					lpOperandTop->Real = randf64();
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_MIN:
			if (IsInteger)
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpOperandTop->Quad = min(lpOperandTop[0].Quad, lpOperandTop[1].Quad);
				lpOperandTop->IsQuad = lpOperandTop[0].IsQuad && lpOperandTop[1].IsQuad;
			}
			break;
		case TAG_IMIN:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsInteger)
				{
					lpOperandTop->Quad = min(
						lpOperandTop[0].IsQuad ? (int64_t)lpOperandTop[0].Quad : (int32_t)lpOperandTop[0].Low,
						lpOperandTop[1].IsQuad ? (int64_t)lpOperandTop[1].Quad : (int32_t)lpOperandTop[1].Low);
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = (int64_t)lpOperandTop->Quad > INT32_MAX || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				else
				{
					lpOperandTop->Real = min(lpOperandTop[0].Real, lpOperandTop[1].Real);
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_MAX:
			if (IsInteger)
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpOperandTop->Quad = max(lpOperandTop[0].Quad, lpOperandTop[1].Quad);
				lpOperandTop->IsQuad = lpOperandTop[0].IsQuad | lpOperandTop[1].IsQuad;
			}
			break;
		case TAG_IMAX:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsInteger)
				{
					lpOperandTop->Quad = max(
						lpOperandTop[0].IsQuad ? (int64_t)lpOperandTop[0].Quad : (int32_t)lpOperandTop[0].Low,
						lpOperandTop[1].IsQuad ? (int64_t)lpOperandTop[1].Quad : (int32_t)lpOperandTop[1].Low);
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = (int64_t)lpOperandTop->Quad > INT32_MAX || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
				else
				{
					lpOperandTop->Real = max(lpOperandTop[0].Real, lpOperandTop[1].Real);
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_MEMCMP:
			{
				MARKUP     *element;
				NTSTATUS   Status;
				int        iResult;
				HANDLE     hProcess1;
				const void *lpAddress1;
				HANDLE     hProcess2;
				const void *lpAddress2;
				size_t     nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				hProcess2 = hProcess1 = (HANDLE)TRUE;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess1 = NULL;
				lpAddress1 = IsInteger ? (void *)(uintptr_t)lpOperandTop[0].Quad : (void *)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess2 = NULL;
				lpAddress2 = IsInteger ? (void *)(uintptr_t)lpOperandTop[1].Quad : (void *)(uintptr_t)lpOperandTop[1].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nSize = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (hProcess1 || hProcess2)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hProcess1)
						hProcess1 = hProcess;
					if (hProcess2)
						hProcess2 = hProcess;
				}
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, nSize);
				if (NT_SUCCESS(Status))
				{
					if (IsInteger)
					{
						lpOperandTop->Quad = (unsigned int)iResult;
						lpOperandTop->IsQuad = FALSE;
					}
					else
					{
						lpOperandTop->Real = iResult;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_MEMMOVE:
			{
				MARKUP     *element;
				NTSTATUS   Status;
				HANDLE     hDestProcess;
				void       *lpDest;
				HANDLE     hSrcProcess;
				const void *lpSrc;
				size_t     nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				hSrcProcess = hDestProcess = (HANDLE)TRUE;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				lpDest = IsInteger ? (void *)(uintptr_t)lpOperandTop[0].Quad : (void *)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				lpSrc = IsInteger ? (void *)(uintptr_t)lpOperandTop[1].Quad : (void *)(uintptr_t)lpOperandTop[1].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nSize = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (NT_SUCCESS(Status))
					break;
				if (Status == STATUS_MEMORY_READ_FAILED)
				{
					lpAddress = (LPVOID)lpSrc;
					goto READ_ERROR;
				}
				if (Status == STATUS_MEMORY_WRITE_FAILED)
				{
					lpAddress = lpDest;
					goto WRITE_ERROR;
				}
				goto PARSING_ERROR;
			}
			break;
		case TAG_MEMSET:
		case TAG_MEMSET16:
		case TAG_MEMSET32:
		case TAG_MEMSET64:
		case TAG_STRNSET:
		case TAG_WCSNSET:
		case TAG_MBSNBSET:
			{
				MARKUP   *element;
				HANDLE   hDestProcess;
				PVOID    lpDest;
				uint64_t qwFill;
				size_t   nCount;
				size_t   nLength;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					hDestProcess = hProcess;
				}
				else
				{
					hDestProcess = NULL;
				}
				lpDest = IsInteger ? (PVOID)(uintptr_t)lpOperandTop[0].Quad : (PVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				qwFill = IsInteger ? (uint64_t)lpOperandTop[1].Quad : (uint64_t)lpOperandTop[1].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				switch (lpMarkup->Tag)
				{
				case TAG_STRNSET:
				case TAG_MBSNBSET:
					if ((nLength = StringLengthA(hDestProcess, lpAddress = lpDest, nCount)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(nCount = min(nCount, nLength)))
						break;
					if (lpMarkup->Tag == TAG_MBSNBSET && (qwFill & 0xFF00))
					{
						if (!FillProcessMemory16(hDestProcess, lpAddress = lpDest, nCount / 2, _byteswap_ushort((WORD)qwFill)))
							goto WRITE_ERROR;
						if (!(nCount & 1))
							break;
						if (hDestProcess)
						{
							const char spaceChar = ' ';

							if (!WriteProcessMemory(hDestProcess, (LPBYTE)lpDest + nCount - 1, &spaceChar, 1, NULL))
								goto WRITE_ERROR;
						}
						else
						{
							if (IsBadWritePtr((LPBYTE)lpDest + nCount - 1, 1))
								goto WRITE_ERROR;
							*((LPBYTE)lpDest + nCount - 1) = ' ';
						}
						break;
					}
				case TAG_MEMSET:
					if (!FillProcessMemory(hDestProcess, lpAddress = lpDest, nCount, (BYTE)qwFill))
						goto WRITE_ERROR;
					break;
				case TAG_WCSNSET:
					if ((nLength = StringLengthW(hDestProcess, lpAddress = lpDest, nCount)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(nCount = min(nCount, nLength)))
						break;
				case TAG_MEMSET16:
					if (!FillProcessMemory16(hDestProcess, lpAddress = lpDest, nCount, (WORD)qwFill))
						goto WRITE_ERROR;
					break;
				case TAG_MEMSET32:
					if (!FillProcessMemory32(hDestProcess, lpAddress = lpDest, nCount, (DWORD)qwFill))
						goto WRITE_ERROR;
					break;
				case TAG_MEMSET64:
					if (!FillProcessMemory64(hDestProcess, lpAddress = lpDest, nCount, qwFill))
						goto WRITE_ERROR;
					break;
				}
			}
			break;
		case TAG_PRINTF:
		case TAG_DPRINTF:
			{
				MARKUP     *element;
				size_t     stackSize;
				uintptr_t  *stack, *param;
				VARIABLE   *operand;
				int        result;
				const char *format;
				char       *buffer;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				result = 0;
				if (!TSSGCtrl_GetSSGActionListner(this) || lpMarkup->Tag == TAG_PRINTF && TMainForm_GetUserMode(MainForm) < 3)
					goto PRINTF_CONTINUE;
				stackSize = 0;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
#ifndef _WIN64
					if (!IsStringOperand(element->Param) || !operand->IsQuad && IsInteger)
						stackSize += sizeof(uint32_t);
					else
#endif
						stackSize += sizeof(uint64_t);
					operand++;
				} while (element = element->Next);
				if (!stackSize)
					goto PARSING_ERROR;
				stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, stackSize);
				if (!stack)
					goto ALLOC_ERROR;
				param = stack;
				format = NULL;
				buffer = NULL;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
					if (!format)
					{
						format = IsInteger ? (LPCSTR)(uintptr_t)operand->Quad : (LPCSTR)(uintptr_t)operand->Real;
						operand++;
						if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
						{
							size_t nSize;

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto PRINTF_OPEN_ERROR;
							if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)format, -1)) == SIZE_MAX)
								goto PRINTF_READ_ERROR;
							if (!(buffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
								goto PRINTF_ALLOC_ERROR;
							if (ReadProcessMemory(hProcess, format, buffer, nSize, NULL))
								((LPSTR)format = buffer)[nSize] = '\0';
							else
								goto PRINTF_READ_ERROR;
						}
					}
					else if (!IsStringOperand(element->Param))
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
						*(param++) = IsInteger ? (uintptr_t)operand->Quad : (uintptr_t)operand->Real;
						operand++;
					}
				} while (element = element->Next);
				if (lpMarkup->Tag == TAG_PRINTF)
					result = GuidePrintV(format, (va_list)stack, (va_list)param);
				else
					result = DebugPrintV(format, (va_list)stack, (va_list)param);
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				HeapFree(hHeap, 0, stack);

			PRINTF_CONTINUE:
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)result;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = result;
					lpOperandTop->IsQuad = TRUE;
				}
				continue;

			PRINTF_OPEN_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				HeapFree(hHeap, 0, stack);
				goto OPEN_ERROR;

			PRINTF_READ_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				HeapFree(hHeap, 0, stack);
				goto READ_ERROR;

			PRINTF_ALLOC_ERROR:
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				HeapFree(hHeap, 0, stack);
				goto ALLOC_ERROR;
			}
		case TAG_SNPRINTF:
			{
				MARKUP     *element;
				size_t     stackSize, index;
				uintptr_t  *stack, *param;
				VARIABLE   *operand;
				int        iResult;
				HANDLE     hDestProcess;
				LPSTR      lpDest;
				LPSTR      lpDestBuffer;
				size_t     nCount;
				LPCSTR     lpFormat;
				LPSTR      lpFormatBuffer;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				stackSize = 0;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
#ifndef _WIN64
					if (!IsStringOperand(element->Param) || !operand->IsQuad && IsInteger)
						stackSize += sizeof(uint32_t);
					else
#endif
						stackSize += sizeof(uint64_t);
					operand++;
				} while (element = element->Next);
				if (!stackSize)
					goto PARSING_ERROR;
				if (!(stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, stackSize)))
					goto ALLOC_ERROR;
				param = stack;
				hDestProcess = NULL;
				lpFormatBuffer = NULL;
				index = 0;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
					switch (index++)
					{
					case 0:
						if (IsStringOperand(element->Param))
							goto SNPRINTF_PARSING_ERROR;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
							hDestProcess = NULL;
						else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							hDestProcess = hProcess;
						else
							goto SNPRINTF_OPEN_ERROR;
						lpDest = IsInteger ? (LPSTR)(uintptr_t)operand->Quad : (LPSTR)(uintptr_t)operand->Real;
						operand++;
						break;
					case 1:
						if (IsStringOperand(element->Param))
							goto SNPRINTF_PARSING_ERROR;
						nCount = IsInteger ? (size_t)operand->Quad : (size_t)operand->Real;
						operand++;
						break;
					case 2:
						lpFormat = IsInteger ? (LPCSTR)(uintptr_t)operand->Quad : (LPCSTR)(uintptr_t)operand->Real;
						operand++;
						if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
						{
							size_t nSize;

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto SNPRINTF_OPEN_ERROR;
							if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpFormat, -1)) == SIZE_MAX)
								goto SNPRINTF_READ_ERROR;
							if (!(lpFormatBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
								goto SNPRINTF_ALLOC_ERROR;
							if (ReadProcessMemory(hProcess, lpFormat, lpFormatBuffer, nSize, NULL))
								((LPSTR)lpFormat = lpFormatBuffer)[nSize] = '\0';
							else
								goto SNPRINTF_READ_ERROR;
						}
						break;
					default:
						if (!IsStringOperand(element->Param))
						{
#ifndef _WIN64
							if (!operand->IsQuad && IsInteger)
								*(param++) = operand->Low;
							else
#endif
								*(((uint64_t *)param)++) = operand->Quad;
						}
						else
						{
							*(param++) = IsInteger ? (uintptr_t)operand->Quad : (uintptr_t)operand->Real;
						}
						operand++;
						break;
					}
				} while (element = element->Next);
				lpDestBuffer = NULL;
				if (hDestProcess && nCount && !(lpDestBuffer = (LPSTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nCount)))
					goto SNPRINTF_ALLOC_ERROR;
				iResult = internal_vsnprintf(lpDestBuffer ? lpDestBuffer : lpDest, nCount, lpFormat, (va_list)stack, (va_list)param);
				if (lpDestBuffer)
				{
					BOOL bSuccess;

					if (nCount)
						bSuccess = WriteProcessMemory(hDestProcess, lpAddress = (LPVOID)lpDest, lpDestBuffer, strlen(lpDestBuffer) + 1, NULL);
					else
						bSuccess = TRUE;
					HeapFree(hHeap, 0, lpDestBuffer);
					if (!bSuccess)
						goto SNPRINTF_WRITE_ERROR;
				}
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			SNPRINTF_PARSING_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto PARSING_ERROR;

			SNPRINTF_OPEN_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto OPEN_ERROR;

			SNPRINTF_READ_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto READ_ERROR;

			SNPRINTF_WRITE_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto WRITE_ERROR;

			SNPRINTF_ALLOC_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_SNWPRINTF:
			{
				MARKUP     *element;
				size_t     stackSize, index;
				uintptr_t  *stack, *param;
				VARIABLE   *operand;
				int        iResult;
				HANDLE     hDestProcess;
				LPWSTR     lpDest;
				LPWSTR     lpDestBuffer;
				size_t     nCount;
				LPCWSTR    lpFormat;
				LPWSTR     lpFormatBuffer;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				stackSize = 0;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
#ifndef _WIN64
					if (!IsStringOperand(element->Param) || !operand->IsQuad && IsInteger)
						stackSize += sizeof(uint32_t);
					else
#endif
						stackSize += sizeof(uint64_t);
					operand++;
				} while (element = element->Next);
				if (!stackSize)
					goto PARSING_ERROR;
				if (!(stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, stackSize)))
					goto ALLOC_ERROR;
				param = stack;
				hDestProcess = NULL;
				lpFormatBuffer = NULL;
				index = 0;
				element = lpMarkup;
				operand = lpOperandTop;
				do
				{
					switch (index++)
					{
					case 0:
						if (IsStringOperand(element->Param))
							goto SNWPRINTF_PARSING_ERROR;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
							hDestProcess = NULL;
						else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							hDestProcess = hProcess;
						else
							goto SNWPRINTF_OPEN_ERROR;
						lpDest = IsInteger ? (LPWSTR)(uintptr_t)operand->Quad : (LPWSTR)(uintptr_t)operand->Real;
						operand++;
						break;
					case 1:
						if (IsStringOperand(element->Param))
							goto SNWPRINTF_PARSING_ERROR;
						nCount = IsInteger ? (size_t)operand->Quad : (size_t)operand->Real;
						operand++;
						break;
					case 2:
						lpFormat = IsInteger ? (LPCWSTR)(uintptr_t)operand->Quad : (LPCWSTR)(uintptr_t)operand->Real;
						operand++;
						if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
						{
							size_t nSize;

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto SNWPRINTF_OPEN_ERROR;
							if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpFormat, -1)) == SIZE_MAX)
								goto SNWPRINTF_READ_ERROR;
							if (!(lpFormatBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
								goto SNWPRINTF_ALLOC_ERROR;
							if (ReadProcessMemory(hProcess, lpFormat, lpFormatBuffer, nSize, NULL))
								*(LPWSTR)((LPBYTE)lpFormatBuffer + nSize) = L'\0';
							else
								goto SNWPRINTF_READ_ERROR;
						}
						break;
					default:
						if (!IsStringOperand(element->Param))
						{
#ifndef _WIN64
							if (!operand->IsQuad && IsInteger)
								*(param++) = operand->Low;
							else
#endif
								*(((uint64_t *)param)++) = operand->Quad;
						}
						else
						{
							*(param++) = IsInteger ? (uintptr_t)operand->Quad : (uintptr_t)operand->Real;
						}
						operand++;
						break;
					}
				} while (element = element->Next);
				lpDestBuffer = NULL;
				if (hDestProcess && nCount && !(lpDestBuffer = (LPWSTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nCount * sizeof(wchar_t))))
					goto SNWPRINTF_ALLOC_ERROR;
				iResult = internal_vsnwprintf(lpDestBuffer ? lpDestBuffer : lpDest, nCount, lpFormat, (va_list)stack, (va_list)param);
				if (lpDestBuffer)
				{
					BOOL bSuccess;

					if (nCount)
						bSuccess = WriteProcessMemory(hDestProcess, lpAddress = (LPVOID)lpDest, lpDestBuffer, wcslen(lpDestBuffer) * sizeof(wchar_t) + sizeof(wchar_t), NULL);
					else
						bSuccess = TRUE;
					HeapFree(hHeap, 0, lpDestBuffer);
					if (!bSuccess)
						goto SNWPRINTF_WRITE_ERROR;
				}
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			SNWPRINTF_PARSING_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto PARSING_ERROR;

			SNWPRINTF_OPEN_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto OPEN_ERROR;

			SNWPRINTF_READ_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto READ_ERROR;

			SNWPRINTF_WRITE_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto WRITE_ERROR;

			SNWPRINTF_ALLOC_ERROR:
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, stack);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRDUP:
			{
				NTSTATUS Status;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpSrc = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop->Quad : (LPCSTR)(uintptr_t)lpOperandTop->Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize++;
				else
					goto READ_ERROR;
				if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nSize)))
					goto ALLOC_ERROR;
				Status = MoveProcessMemory(NULL, lpDest, hSrcProcess, lpSrc, nSize);
				if (NT_SUCCESS(Status))
				{
					lpDest[nSize] = '\0';
					if (IsInteger)
					{
						lpOperandTop->Quad = (uintptr_t)lpDest;
						lpOperandTop->IsQuad = sizeof(uintptr_t) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (uintptr_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_WCSDUP:
			{
				NTSTATUS Status;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop->Quad : (LPCWSTR)(uintptr_t)lpOperandTop->Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthW(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize = nSize * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nSize)))
					goto ALLOC_ERROR;
				Status = MoveProcessMemory(NULL, lpDest, hSrcProcess, lpSrc, nSize);
				if (NT_SUCCESS(Status))
				{
					*(LPWSTR)((LPBYTE)lpDest + nSize) = L'\0';
					if (IsInteger)
					{
						lpOperandTop->Quad = (uintptr_t)lpDest;
						lpOperandTop->IsQuad = sizeof(uintptr_t) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (uintptr_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_STRLEN:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(size_t)lpOperandTop->Real;
					if ((size_t)(lpOperandTop->Quad = StringLengthA(hProcess, lpAddress, -1)) == SIZE_MAX)
						goto READ_ERROR;
				}
				else
				{
					lpOperandTop->Quad = strlen(IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real);
				}
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(size_t) > sizeof(uint32_t);
				}
				else
				{
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_WCSLEN:
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(size_t)lpOperandTop->Real;
					if ((size_t)(lpOperandTop->Quad = StringLengthW(hProcess, lpAddress, -1)) == SIZE_MAX)
						goto READ_ERROR;
				}
				else
				{
					lpOperandTop->Quad = wcslen(IsInteger ? (LPCWSTR)lpOperandTop->Quad : (LPCWSTR)(size_t)lpOperandTop->Real);
				}
				if (IsInteger)
				{
					lpOperandTop->IsQuad = sizeof(size_t) > sizeof(uint32_t);
				}
				else
				{
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_STRNLEN:
			{
				MARKUP *element;
				HANDLE hProcess;
				LPCSTR lpString;
				size_t nMaxLength;
				size_t nLength;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess = hProcess;
				else
					goto OPEN_ERROR;
				element = element->Next;
				nMaxLength = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
				if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, nMaxLength)) == SIZE_MAX)
					goto READ_ERROR;
				if (IsInteger)
				{
					lpOperandTop->Quad = nLength;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nLength;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_WCSNLEN:
			{
				MARKUP  *element;
				HANDLE  hProcess;
				LPCWSTR lpString;
				size_t  nMaxLength;
				size_t  nLength;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpString = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess = hProcess;
				else
					goto OPEN_ERROR;
				element = element->Next;
				nMaxLength = (IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real) * sizeof(wchar_t);
				if ((nLength = StringLengthW(hProcess, lpAddress = (LPVOID)lpString, nMaxLength)) == SIZE_MAX)
					goto READ_ERROR;
				if (IsInteger)
				{
					lpOperandTop->Quad = nLength;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nLength;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_STRCMP:
			{
				MARKUP   *element;
				NTSTATUS Status;
				int      iResult;
				HANDLE   hProcess1;
				LPCSTR   lpAddress1;
				size_t   nSize1;
				HANDLE   hProcess2;
				LPCSTR   lpAddress2;
				size_t   nSize2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpAddress1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess1 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess1 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize1 = StringLengthA(hProcess1, lpAddress = (LPVOID)lpAddress1, -1)) != SIZE_MAX)
					nSize1++;
				else
					goto READ_ERROR;
				element = element->Next;
				lpAddress2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess2 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess2 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize2 = StringLengthA(hProcess2, lpAddress = (LPVOID)lpAddress2, -1)) != SIZE_MAX)
					nSize2++;
				else
					goto READ_ERROR;
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, min(nSize1, nSize2));
				if (NT_SUCCESS(Status))
				{
					if (IsInteger)
					{
						lpOperandTop->Quad = (unsigned int)iResult;
						lpOperandTop->IsQuad = FALSE;
					}
					else
					{
						lpOperandTop->Real = iResult;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_WCSCMP:
			{
				MARKUP   *element;
				int      iResult;
				NTSTATUS Status;
				HANDLE   hProcess1;
				LPCWSTR  lpAddress1;
				size_t   nSize1;
				HANDLE   hProcess2;
				LPCWSTR  lpAddress2;
				size_t   nSize2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpAddress1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess1 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess1 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize1 = StringLengthW(hProcess1, lpAddress = (LPVOID)lpAddress1, -1)) != SIZE_MAX)
					nSize1 = nSize1 * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				element = element->Next;
				lpAddress2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess2 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess2 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize2 = StringLengthW(hProcess2, lpAddress = (LPVOID)lpAddress2, -1)) != SIZE_MAX)
					nSize2 = nSize2 * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, min(nSize1, nSize2));
				if (NT_SUCCESS(Status))
				{
					if (IsInteger)
					{
						lpOperandTop->Quad = (unsigned int)iResult;
						lpOperandTop->IsQuad = FALSE;
					}
					else
					{
						lpOperandTop->Real = iResult;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_STRICMP:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				int    iResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto STRICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRICMP_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRICMP_READ_ERROR;
				}
				iResult = _stricmp(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSICMP:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				int     iResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString1 = lpBuffer1) + nSize) = L'\0';
					else
						goto WCSICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSICMP_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSICMP_READ_ERROR;
				}
				iResult = _wcsicmp(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSICMP:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				int    iResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto MBSICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSICMP_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSICMP_READ_ERROR;
				}
				iResult = _mbsicmp(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRNCMP:
			{
				MARKUP   *element;
				NTSTATUS Status;
				int      iResult;
				HANDLE   hProcess1;
				LPCSTR   lpAddress1;
				size_t   nSize1;
				HANDLE   hProcess2;
				LPCSTR   lpAddress2;
				size_t   nSize2;
				size_t   nSize3;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpAddress1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess1 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess1 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize1 = StringLengthA(hProcess1, lpAddress = (LPVOID)lpAddress1, -1)) != SIZE_MAX)
					nSize1++;
				else
					goto READ_ERROR;
				element = element->Next;
				lpAddress2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess2 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess2 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize2 = StringLengthA(hProcess2, lpAddress = (LPVOID)lpAddress2, -1)) != SIZE_MAX)
					nSize2++;
				else
					goto READ_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nSize3 = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, min(min(nSize1, nSize2), nSize3));
				if (NT_SUCCESS(Status))
				{
					if (IsInteger)
					{
						lpOperandTop->Quad = (unsigned int)iResult;
						lpOperandTop->IsQuad = FALSE;
					}
					else
					{
						lpOperandTop->Real = iResult;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_WCSNCMP:
			{
				MARKUP   *element;
				int      iResult;
				NTSTATUS Status;
				HANDLE   hProcess1;
				LPCWSTR  lpAddress1;
				size_t   nSize1;
				HANDLE   hProcess2;
				LPCWSTR  lpAddress2;
				size_t   nSize2;
				size_t   nSize3;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpAddress1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess1 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess1 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize1 = StringLengthW(hProcess1, lpAddress = (LPVOID)lpAddress1, -1)) != SIZE_MAX)
					nSize1 = nSize1 * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				element = element->Next;
				lpAddress2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hProcess2 = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hProcess2 = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize2 = StringLengthW(hProcess2, lpAddress = (LPVOID)lpAddress2, -1)) != SIZE_MAX)
					nSize2 = nSize2 * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nSize3 = (IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real) * sizeof(wchar_t);
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, min(min(nSize1, nSize2), nSize3));
				if (NT_SUCCESS(Status))
				{
					if (IsInteger)
					{
						lpOperandTop->Quad = (unsigned int)iResult;
						lpOperandTop->IsQuad = FALSE;
					}
					else
					{
						lpOperandTop->Real = iResult;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_STRNICMP:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				int    iResult;
				LPCSTR lpString1;
				LPCSTR lpString2;
				size_t nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto STRNICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRNICMP_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRNICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRNICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRNICMP_READ_ERROR;
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto STRNICMP_PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				iResult = _strnicmp(lpString1, lpString2, nCount);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRNICMP_PARSING_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto PARSING_ERROR;

			STRNICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRNICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRNICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSNICMP:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				int     iResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;
				size_t  nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString1 = lpBuffer1) + nSize) = L'\0';
					else
						goto WCSNICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSNICMP_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSNICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSNICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSNICMP_READ_ERROR;
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto WCSNICMP_PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				iResult = _wcsnicmp(lpString1, lpString2, nCount);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSNICMP_PARSING_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto PARSING_ERROR;

			WCSNICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSNICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSNICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSNBICMP:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				int    iResult;
				LPCSTR lpString1;
				LPCSTR lpString2;
				size_t nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto MBSNBICMP_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSNBICMP_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSNBICMP_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSNBICMP_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSNBICMP_READ_ERROR;
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto MBSNBICMP_PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				iResult = _mbsnbicmp(lpString1, lpString2, nCount);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (unsigned int)iResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = iResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSNBICMP_PARSING_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto PARSING_ERROR;

			MBSNBICMP_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSNBICMP_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSNBICMP_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop[0].Quad : (LPSTR)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				lpSrc = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize++;
				else
					goto READ_ERROR;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;
					size_t nLength;

					nLength = nSize - 1;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[2].Quad : (LPVOID)(uintptr_t)lpOperandTop[2].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				if (!hDestProcess && !lpDest)
				{
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nSize)))
						goto ALLOC_ERROR;
					if (IsInteger)
					{
						lpOperandTop->Quad = (size_t)lpDest;
						lpOperandTop->IsQuad = sizeof(lpDest) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (size_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_WCSCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPWSTR)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthW(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize = nSize * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;
					size_t nLength;

					nLength = nSize / sizeof(wchar_t) - 1;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[2].Quad : (LPVOID)(uintptr_t)lpOperandTop[2].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				if (!hDestProcess && !lpDest)
				{
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nSize)))
						goto ALLOC_ERROR;
					if (IsInteger)
					{
						lpOperandTop->Quad = (size_t)lpDest;
						lpOperandTop->IsQuad = sizeof(lpDest) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (size_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_STRCAT:
			{
				MARKUP   *element;
				size_t   nLength;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop[0].Quad : (LPSTR)(uintptr_t)lpOperandTop[0].Real;
				if ((nLength = StringLengthA(hDestProcess, lpAddress = (LPVOID)lpDest, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpDest += nLength;
				element = element->Next;
				lpSrc = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize++;
				else
					goto READ_ERROR;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;

					nLength += nSize - 1;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[2].Quad : (LPVOID)(uintptr_t)lpOperandTop[2].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_WCSCAT:
			{
				MARKUP   *element;
				size_t   nLength;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nSize;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPWSTR)(uintptr_t)lpOperandTop[0].Real;
				if ((nLength = StringLengthW(hDestProcess, lpAddress = (LPVOID)lpDest, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpDest += nLength;
				element = element->Next;
				lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthW(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) != SIZE_MAX)
					nSize = nSize * sizeof(wchar_t) + sizeof(wchar_t);
				else
					goto READ_ERROR;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;

					nLength += nSize / sizeof(wchar_t) - 1;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[2].Quad : (LPVOID)(uintptr_t)lpOperandTop[2].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_STRLCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nSize;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop[0].Quad : (LPSTR)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				lpSrc = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) == SIZE_MAX)
					goto READ_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (nCount)
				{
					size_t nLimitedSize;

					nLimitedSize = nCount - 1;
					if (nSize > nLimitedSize)
						nSize = nLimitedSize;
				}
				else
					nSize = 0;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;
					size_t nLength;

					nLength = nSize;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[3].Quad : (LPVOID)(uintptr_t)lpOperandTop[3].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				if (!hDestProcess && !lpDest)
				{
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nCount)))
						goto ALLOC_ERROR;
					if (IsInteger)
					{
						lpOperandTop->Quad = (size_t)lpDest;
						lpOperandTop->IsQuad = sizeof(lpDest) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (size_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				Status = STATUS_SUCCESS;
				if (nCount && NT_SUCCESS(Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)))
				{
					LPSTR lpTerminator;

					lpTerminator = lpDest + nSize;
					if (hDestProcess)
					{
						const char nullChar = '\0';

						if (!WriteProcessMemory(hDestProcess, lpTerminator, &nullChar, sizeof(nullChar), NULL))
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
					else
					{
						if (!IsBadWritePtr(lpTerminator, 1))
							*lpTerminator = '\0';
						else
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
				}
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_WCSLCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nSize;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPWSTR)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthW(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) == SIZE_MAX)
					goto READ_ERROR;
				nSize *= sizeof(wchar_t);
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = (IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real) * sizeof(wchar_t);
				if (nCount)
				{
					size_t nLimitedSize;

					nLimitedSize = (nCount - 1) * sizeof(wchar_t);
					if (nSize > nLimitedSize)
						nSize = nLimitedSize;
				}
				else
					nSize = 0;
				if (element = element->Next)
				{
					HANDLE hTargetProcess;
					size_t nLength;

					nLength = nSize / sizeof(wchar_t);
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[3].Quad : (LPVOID)(uintptr_t)lpOperandTop[3].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				if (!hDestProcess && !lpDest)
				{
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nCount * sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (IsInteger)
					{
						lpOperandTop->Quad = (size_t)lpDest;
						lpOperandTop->IsQuad = sizeof(lpDest) > sizeof(uint32_t);
					}
					else
					{
						lpOperandTop->Real = (size_t)lpDest;
						lpOperandTop->IsQuad = TRUE;
					}
				}
				Status = STATUS_SUCCESS;
				if (nCount && NT_SUCCESS(Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)))
				{
					LPWSTR lpTerminator;

					lpTerminator = (LPWSTR)((LPBYTE)lpDest + nSize);
					if (hDestProcess)
					{
						const wchar_t nullChar = L'\0';

						if (!WriteProcessMemory(hDestProcess, lpTerminator, &nullChar, sizeof(nullChar), NULL))
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
					else
					{
						if (!IsBadWritePtr(lpTerminator, sizeof(wchar_t)))
							*lpTerminator = L'\0';
						else
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
				}
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_STRLCAT:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPSTR    lpDest;
				HANDLE   hSrcProcess;
				LPCSTR   lpSrc;
				size_t   nLength;
				size_t   nSize;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop[0].Quad : (LPSTR)(uintptr_t)lpOperandTop[0].Real;
				if ((nLength = StringLengthA(hDestProcess, lpAddress = (LPVOID)lpDest, -1)) == SIZE_MAX)
					goto READ_ERROR;
				element = element->Next;
				lpSrc = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) == SIZE_MAX)
					goto READ_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (nCount)
				{
					if (nCount > nLength)
					{
						size_t nLimitedSize;

						nLimitedSize = nCount - nLength - 1;
						if (nSize > nLimitedSize)
							nSize = nLimitedSize;
					}
					else
					{
						nLength = nCount - 1;
						nSize = 0;
					}
					lpDest += nLength;
				}
				else
				{
					nLength = 0;
					nSize = 0;
				}
				if (element = element->Next)
				{
					HANDLE hTargetProcess;

					nLength += nSize;
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[3].Quad : (LPVOID)(uintptr_t)lpOperandTop[3].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				Status = STATUS_SUCCESS;
				if (nCount && NT_SUCCESS(Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)))
				{
					LPSTR lpTerminator;

					lpTerminator = lpDest + nSize;
					if (hDestProcess)
					{
						const char nullChar = '\0';

						if (!WriteProcessMemory(hDestProcess, lpTerminator, &nullChar, sizeof(nullChar), NULL))
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
					else
					{
						if (!IsBadWritePtr(lpTerminator, 1))
							*lpTerminator = '\0';
						else
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
				}
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_WCSLCAT:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPWSTR   lpDest;
				HANDLE   hSrcProcess;
				LPCWSTR  lpSrc;
				size_t   nLength;
				size_t   nSize;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPWSTR)(uintptr_t)lpOperandTop[0].Real;
				if ((nLength = StringLengthW(hDestProcess, lpAddress = (LPVOID)lpDest, -1)) == SIZE_MAX)
					goto READ_ERROR;
				element = element->Next;
				lpSrc = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if ((nSize = StringLengthW(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) == SIZE_MAX)
					goto READ_ERROR;
				nSize *= sizeof(wchar_t);
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (nCount)
				{
					if (nCount > nLength)
					{
						size_t nLimitedSize;

						nLimitedSize = (nCount - nLength - 1) * sizeof(wchar_t);
						if (nSize > nLimitedSize)
							nSize = nLimitedSize;
					}
					else
					{
						nLength = nCount - 1;
						nSize = 0;
					}
					lpDest += nLength;
				}
				else
				{
					nLength = 0;
					nSize = 0;
				}
				if (element = element->Next)
				{
					HANDLE hTargetProcess;

					nLength += nSize / sizeof(wchar_t);
					lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[3].Quad : (LPVOID)(uintptr_t)lpOperandTop[3].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
						hTargetProcess = GetCurrentProcess();
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hTargetProcess = hProcess;
					else
						goto OPEN_ERROR;
					if (IsInteger)
					{
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
							goto WRITE_ERROR;
					}
					else
					{
						double dLength;

						dLength = (double)nLength;
						if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
							goto WRITE_ERROR;
					}
				}
				Status = STATUS_SUCCESS;
				if (nCount && NT_SUCCESS(Status = MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)))
				{
					LPWSTR lpTerminator;

					lpTerminator = (LPWSTR)((LPBYTE)lpDest + nSize);
					if (hDestProcess)
					{
						const wchar_t nullChar = L'\0';

						if (!WriteProcessMemory(hDestProcess, lpTerminator, &nullChar, sizeof(nullChar), NULL))
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
					else
					{
						if (!IsBadWritePtr(lpTerminator, sizeof(wchar_t)))
							*lpTerminator = L'\0';
						else
							Status = STATUS_MEMORY_WRITE_FAILED;
					}
				}
				if (!NT_SUCCESS(Status))
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
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
		case TAG_STRCHR:
			{
				MARKUP *element;
				LPSTR  lpBuffer;
				LPSTR  lpResult;
				LPCSTR lpString;
				char   c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto STRCHR_PARSING_ERROR;
				c = IsInteger ? (char)lpOperandTop[1].Quad : (char)lpOperandTop[1].Real;
				if ((lpResult = strchr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_WCSCHR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer;
				LPWSTR  lpResult;
				LPCWSTR lpString;
				wchar_t c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto WCSCHR_PARSING_ERROR;
				c = IsInteger ? (wchar_t)lpOperandTop[1].Quad : (wchar_t)lpOperandTop[1].Real;
				if ((lpResult = wcschr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					(LPBYTE)lpResult += (LPBYTE)lpString - (LPBYTE)lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_MBSCHR:
			{
				MARKUP       *element;
				LPSTR        lpBuffer;
				LPSTR        lpResult;
				LPCSTR       lpString;
				unsigned int c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto MBSCHR_PARSING_ERROR;
				c = IsInteger ? (unsigned int)lpOperandTop[1].Quad : (unsigned int)lpOperandTop[1].Real;
				if ((lpResult = _mbschr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_STRICHR:
			{
				MARKUP *element;
				LPSTR  lpBuffer;
				LPSTR  lpResult;
				LPCSTR lpString;
				char   c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto STRICHR_PARSING_ERROR;
				c = IsInteger ? (char)lpOperandTop[1].Quad : (char)lpOperandTop[1].Real;
				if ((lpResult = _strichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_WCSICHR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer;
				LPWSTR  lpResult;
				LPCWSTR lpString;
				wchar_t c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto WCSICHR_PARSING_ERROR;
				c = IsInteger ? (wchar_t)lpOperandTop[1].Quad : (wchar_t)lpOperandTop[1].Real;
				if ((lpResult = _wcsichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					(LPBYTE)lpResult += (LPBYTE)lpString - (LPBYTE)lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_MBSICHR:
			{
				MARKUP       *element;
				LPSTR        lpBuffer;
				LPSTR        lpResult;
				LPCSTR       lpString;
				unsigned int c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto MBSICHR_PARSING_ERROR;
				c = IsInteger ? (unsigned int)lpOperandTop[1].Quad : (unsigned int)lpOperandTop[1].Real;
				if ((lpResult = _mbsichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_STRRCHR:
			{
				MARKUP *element;
				LPSTR  lpBuffer;
				LPSTR  lpResult;
				LPCSTR lpString;
				char   c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto STRRCHR_PARSING_ERROR;
				c = IsInteger ? (char)lpOperandTop[1].Quad : (char)lpOperandTop[1].Real;
				if ((lpResult = strrchr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRRCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_WCSRCHR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer;
				LPWSTR  lpResult;
				LPCWSTR lpString;
				wchar_t c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto WCSRCHR_PARSING_ERROR;
				c = IsInteger ? (wchar_t)lpOperandTop[1].Quad : (wchar_t)lpOperandTop[1].Real;
				if ((lpResult = wcsrchr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					(LPBYTE)lpResult += (LPBYTE)lpString - (LPBYTE)lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSRCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_MBSRCHR:
			{
				MARKUP       *element;
				LPSTR        lpBuffer;
				LPSTR        lpResult;
				LPCSTR       lpString;
				unsigned int c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto MBSRCHR_PARSING_ERROR;
				c = IsInteger ? (unsigned int)lpOperandTop[1].Quad : (unsigned int)lpOperandTop[1].Real;
				if ((lpResult = _mbsrchr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSRCHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_STRRICHR:
			{
				MARKUP *element;
				LPSTR  lpBuffer;
				LPSTR  lpResult;
				LPCSTR lpString;
				char   c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto STRRICHR_PARSING_ERROR;
				c = IsInteger ? (char)lpOperandTop[1].Quad : (char)lpOperandTop[1].Real;
				if ((lpResult = _strrichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRRICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_WCSRICHR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer;
				LPWSTR  lpResult;
				LPCWSTR lpString;
				wchar_t c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto WCSRICHR_PARSING_ERROR;
				c = IsInteger ? (wchar_t)lpOperandTop[1].Quad : (wchar_t)lpOperandTop[1].Real;
				if ((lpResult = _wcsrichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					(LPBYTE)lpResult += (LPBYTE)lpString - (LPBYTE)lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSRICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_MBSRICHR:
			{
				MARKUP       *element;
				LPSTR        lpBuffer;
				LPSTR        lpResult;
				LPCSTR       lpString;
				unsigned int c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto MBSRICHR_PARSING_ERROR;
				c = IsInteger ? (unsigned int)lpOperandTop[1].Quad : (unsigned int)lpOperandTop[1].Real;
				if ((lpResult = _mbsrichr(lpBuffer ? lpBuffer : lpString, c)) && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSRICHR_PARSING_ERROR:
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				goto PARSING_ERROR;
			}
			break;
		case TAG_STRSTR:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto STRSTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRSTR_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRSTR_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRSTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRSTR_READ_ERROR;
				}
				if ((lpResult = strstr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRSTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRSTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRSTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSSTR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				LPWSTR  lpResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer1 + nSize) = L'\0';
					else
						goto WCSSTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSSTR_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSSTR_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSSTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSSTR_READ_ERROR;
				}
				if ((lpResult = wcsstr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					(LPBYTE)lpResult += (LPBYTE)lpString1 - (LPBYTE)lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSSTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSSTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSSTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSSTR:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto MBSSTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSSTR_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSSTR_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSSTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSSTR_READ_ERROR;
				}
				if ((lpResult = _mbsstr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSSTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSSTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSSTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRISTR:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto STRISTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRISTR_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRISTR_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRISTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRISTR_READ_ERROR;
				}
				if ((lpResult = _stristr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRISTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRISTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRISTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSISTR:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				LPWSTR  lpResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer1 + nSize) = L'\0';
					else
						goto WCSISTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSISTR_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSISTR_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSISTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSISTR_READ_ERROR;
				}
				if ((lpResult = _wcsistr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					(LPBYTE)lpResult += (LPBYTE)lpString1 - (LPBYTE)lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSISTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSISTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSISTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSISTR:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto MBSISTR_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSISTR_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSISTR_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSISTR_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSISTR_READ_ERROR;
				}
				if ((lpResult = _mbsistr(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSISTR_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSISTR_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSISTR_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRSPN:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				size_t nResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto STRSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRSPN_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRSPN_READ_ERROR;
				}
				nResult = strspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSSPN:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				size_t  nResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString1 = lpBuffer1) + nSize) = L'\0';
					else
						goto WCSSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSSPN_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSSPN_READ_ERROR;
				}
				nResult = wcsspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSSPN:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				size_t nResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto MBSSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSSPN_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSSPN_READ_ERROR;
				}
				nResult = _mbsspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRCSPN:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				size_t nResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto STRCSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRCSPN_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRCSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRCSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRCSPN_READ_ERROR;
				}
				nResult = strcspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRCSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRCSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRCSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSCSPN:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				size_t  nResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString1 = lpBuffer1) + nSize) = L'\0';
					else
						goto WCSCSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSCSPN_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSCSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSCSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSCSPN_READ_ERROR;
				}
				nResult = wcscspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSCSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSCSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSCSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSCSPN:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				size_t nResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						((LPSTR)lpString1 = lpBuffer1)[nSize] = '\0';
					else
						goto MBSCSPN_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSCSPN_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSCSPN_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSCSPN_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSCSPN_READ_ERROR;
				}
				nResult = _mbscspn(lpString1, lpString2);
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = nResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = nResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSCSPN_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSCSPN_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSCSPN_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRPBRK:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto STRPBRK_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRPBRK_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRPBRK_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRPBRK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRPBRK_READ_ERROR;
				}
				if ((lpResult = strpbrk(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRPBRK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRPBRK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRPBRK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSPBRK:
			{
				MARKUP  *element;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				LPWSTR  lpResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer1 + nSize) = L'\0';
					else
						goto WCSPBRK_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSPBRK_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSPBRK_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSPBRK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSPBRK_READ_ERROR;
				}
				if ((lpResult = wcspbrk(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					(LPBYTE)lpResult += (LPBYTE)lpString1 - (LPBYTE)lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSPBRK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSPBRK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSPBRK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSPBRK:
			{
				MARKUP *element;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString1, lpBuffer1, nSize, NULL))
						lpBuffer1[nSize] = '\0';
					else
						goto MBSPBRK_READ_ERROR;
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSPBRK_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSPBRK_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSPBRK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSPBRK_READ_ERROR;
				}
				if ((lpResult = _mbspbrk(lpBuffer1 ? lpBuffer1 : lpString1, lpString2)) && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSPBRK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSPBRK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSPBRK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRSET:
			{
				MARKUP *element;
				HANDLE hDestProcess;
				PVOID  lpDest;
				char   cFill;
				size_t nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					hDestProcess = hProcess;
				}
				else
				{
					hDestProcess = NULL;
				}
				lpDest = IsInteger ? (PVOID)(uintptr_t)lpOperandTop[0].Quad : (PVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				cFill = IsInteger ? (char)lpOperandTop[1].Quad : (char)lpOperandTop[1].Real;
				if ((nCount = StringLengthA(hProcess, lpAddress = (LPVOID)lpDest, SIZE_MAX)) == SIZE_MAX)
					goto READ_ERROR;
				if (!FillProcessMemory(hDestProcess, lpAddress = lpDest, nCount, cFill))
					goto WRITE_ERROR;
			}
			break;
		case TAG_WCSSET:
			{
				MARKUP  *element;
				HANDLE  hDestProcess;
				PVOID   lpDest;
				wchar_t wFill;
				size_t  nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					hDestProcess = hProcess;
				}
				else
				{
					hDestProcess = NULL;
				}
				lpDest = IsInteger ? (PVOID)(uintptr_t)lpOperandTop[0].Quad : (PVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				wFill = IsInteger ? (wchar_t)lpOperandTop[1].Quad : (wchar_t)lpOperandTop[1].Real;
				if ((nCount = StringLengthW(hProcess, lpAddress = (LPVOID)lpDest, SIZE_MAX)) == SIZE_MAX)
					goto READ_ERROR;
				if (!FillProcessMemory16(hDestProcess, lpAddress = lpDest, nCount, wFill))
					goto WRITE_ERROR;
			}
			break;
		case TAG_MBSSET:
			{
				MARKUP       *element;
				HANDLE       hDestProcess;
				PVOID        lpDest;
				unsigned int uFill;
				size_t       nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag != TAG_PARAM_LOCAL)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					hDestProcess = hProcess;
				}
				else
				{
					hDestProcess = NULL;
				}
				lpDest = IsInteger ? (PVOID)(uintptr_t)lpOperandTop[0].Quad : (PVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				uFill = IsInteger ? (unsigned int)lpOperandTop[1].Quad : (unsigned int)lpOperandTop[1].Real;
				if ((nCount = StringLengthA(hProcess, lpAddress = (LPVOID)lpDest, SIZE_MAX)) == SIZE_MAX)
					goto READ_ERROR;
				if (!(uFill & 0xFF00))
				{
					if (!FillProcessMemory(hDestProcess, lpAddress = lpDest, nCount, (BYTE)uFill))
						goto WRITE_ERROR;
				}
				else
				{
					if (!FillProcessMemory16(hDestProcess, lpAddress = lpDest, nCount / 2, _byteswap_ushort((WORD)uFill)))
						goto WRITE_ERROR;
					if (!(nCount & 1))
						break;
					if (hDestProcess)
					{
						const char spaceChar = ' ';

						if (!WriteProcessMemory(hDestProcess, (LPBYTE)lpDest + nCount - 1, &spaceChar, 1, NULL))
							goto WRITE_ERROR;
					}
					else
					{
						if (IsBadWritePtr((LPBYTE)lpDest + nCount - 1, 1))
							goto WRITE_ERROR;
						*((LPBYTE)lpDest + nCount - 1) = ' ';
					}
				}
			}
			break;
		case TAG_STRTOK:
			{
				MARKUP *element;
				HANDLE hTargetProcess;
				HANDLE hContextProcess;
				size_t nSize;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;
				LPSTR  *lpContext;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				hContextProcess = NULL;
				lpContext = NULL;
				if (strtok_process != hTargetProcess)
				{
					strtok_process = hTargetProcess;
					strtok_context = NULL;
				}
				else if (lpMarkup->NumberOfOperand >= 3 && (lpContext = IsInteger ? (LPSTR *)(uintptr_t)lpOperandTop[2].Quad : (LPSTR *)(uintptr_t)lpOperandTop[2].Real))
				{
					if (element->Next->Next->Param->Tag == TAG_PARAM_LOCAL)
					{
						if (IsBadReadPtr(lpAddress = lpContext, sizeof(*lpContext)))
							goto READ_ERROR;
						strtok_context = *lpContext;
					}
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					{
						if (!ReadProcessMemory(hContextProcess = hProcess, lpAddress = lpContext, &strtok_context, sizeof(*lpContext), NULL))
							goto READ_ERROR;
					}
					else
					{
						goto OPEN_ERROR;
					}
				}
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (lpAddress = lpString1 ? (LPVOID)lpString1 : hTargetProcess ? strtok_context : NULL)
				{
					if ((nSize = StringLengthA(hTargetProcess, lpAddress, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (hTargetProcess)
					{
						if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
							goto ALLOC_ERROR;
						if (!ReadProcessMemory(hTargetProcess, lpAddress, lpBuffer1, nSize, NULL))
							goto STRTOK_READ_ERROR;
						lpBuffer1[nSize] = '\0';
						if (!lpString1)
						{
							lpString1 = strtok_context;
							strtok_context = lpBuffer1;
						}
					}
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto STRTOK_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto STRTOK_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto STRTOK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto STRTOK_READ_ERROR;
				}
				if (lpResult = internal_strtok(lpBuffer1 ? lpBuffer1 : lpString1, lpString2, &strtok_context))
				{
					if (lpBuffer1)
					{
						LPSTR     lpTerminator;
						size_t    nSize;
						ptrdiff_t iDiff;

						nSize = strtok_context - (lpTerminator = lpResult + strlen(lpResult));
						strtok_context += (iDiff = lpString1 - lpBuffer1);
						lpResult += iDiff;
						if (nSize && !WriteProcessMemory(strtok_process, lpAddress = lpTerminator + iDiff, lpTerminator, nSize, NULL))
							goto STRTOK_WRITE_ERROR;
					}
					if (lpContext)
					{
						if (hContextProcess)
						{
							if (!WriteProcessMemory(hContextProcess, lpAddress = lpContext, &strtok_context, sizeof(*lpContext), NULL))
								goto STRTOK_WRITE_ERROR;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = lpContext, sizeof(*lpContext)))
								goto STRTOK_WRITE_ERROR;
							*lpContext = strtok_context;
						}
					}
				}
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			STRTOK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			STRTOK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			STRTOK_WRITE_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto WRITE_ERROR;

			STRTOK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_WCSTOK:
			{
				MARKUP  *element;
				HANDLE  hTargetProcess;
				HANDLE  hContextProcess;
				size_t  nSize;
				LPWSTR  lpBuffer1;
				LPWSTR  lpBuffer2;
				LPWSTR  lpResult;
				LPCWSTR lpString1;
				LPCWSTR lpString2;
				LPWSTR  *lpContext;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				hContextProcess = NULL;
				lpContext = NULL;
				if (wcstok_process != hTargetProcess)
				{
					wcstok_process = hTargetProcess;
					wcstok_context = NULL;
				}
				else if (lpMarkup->NumberOfOperand >= 3 && (lpContext = IsInteger ? (LPWSTR *)(uintptr_t)lpOperandTop[2].Quad : (LPWSTR *)(uintptr_t)lpOperandTop[2].Real))
				{
					if (element->Next->Next->Param->Tag == TAG_PARAM_LOCAL)
					{
						if (IsBadReadPtr(lpAddress = lpContext, sizeof(*lpContext)))
							goto READ_ERROR;
						wcstok_context = *lpContext;
					}
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					{
						if (!ReadProcessMemory(hContextProcess = hProcess, lpAddress = lpContext, &wcstok_context, sizeof(*lpContext), NULL))
							goto READ_ERROR;
					}
					else
					{
						goto OPEN_ERROR;
					}
				}
				lpString1 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[0].Real;
				if (lpAddress = lpString1 ? (LPVOID)lpString1 : hTargetProcess ? wcstok_context : NULL)
				{
					if ((nSize = StringLengthW(hTargetProcess, lpAddress, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (hTargetProcess)
					{
						if (!(lpBuffer1 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
							goto ALLOC_ERROR;
						if (!ReadProcessMemory(hTargetProcess, lpAddress, lpBuffer1, nSize, NULL))
							goto WCSTOK_READ_ERROR;
						*(LPWSTR)((LPBYTE)lpBuffer1 + nSize) = L'\0';
						if (!lpString1)
						{
							lpString1 = wcstok_context;
							wcstok_context = lpBuffer1;
						}
					}
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCWSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCWSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto WCSTOK_OPEN_ERROR;
					if ((nSize = StringLengthW(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto WCSTOK_READ_ERROR;
					if (!(lpBuffer2 = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto WCSTOK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						*(LPWSTR)((LPBYTE)(lpString2 = lpBuffer2) + nSize) = L'\0';
					else
						goto WCSTOK_READ_ERROR;
				}
				if (lpResult = internal_wcstok(lpBuffer1 ? lpBuffer1 : lpString1, lpString2, &wcstok_context))
				{
					if (lpBuffer1)
					{
						LPWSTR    lpTerminator;
						size_t    nSize;
						ptrdiff_t iDiff;

						nSize = (LPBYTE)wcstok_context - (LPBYTE)(lpTerminator = lpResult + wcslen(lpResult));
						(LPBYTE)wcstok_context += (iDiff = (LPBYTE)lpString1 - (LPBYTE)lpBuffer1);
						(LPBYTE)lpResult += iDiff;
						if (nSize && !WriteProcessMemory(wcstok_process, lpAddress = (LPBYTE)lpTerminator + iDiff, lpTerminator, nSize, NULL))
							goto WCSTOK_WRITE_ERROR;
					}
					if (lpContext)
					{
						if (hContextProcess)
						{
							if (!WriteProcessMemory(hContextProcess, lpAddress = lpContext, &wcstok_context, sizeof(*lpContext), NULL))
								goto WCSTOK_WRITE_ERROR;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = lpContext, sizeof(*lpContext)))
								goto WCSTOK_WRITE_ERROR;
							*lpContext = wcstok_context;
						}
					}
				}
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			WCSTOK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			WCSTOK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			WCSTOK_WRITE_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto WRITE_ERROR;

			WCSTOK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_MBSTOK:
			{
				MARKUP *element;
				HANDLE hTargetProcess;
				HANDLE hContextProcess;
				size_t nSize;
				LPSTR  lpBuffer1;
				LPSTR  lpBuffer2;
				LPSTR  lpResult;
				LPCSTR lpString1;
				LPCSTR lpString2;
				LPSTR  *lpContext;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				hContextProcess = NULL;
				lpContext = NULL;
				if (mbstok_process != hTargetProcess)
				{
					mbstok_process = hTargetProcess;
					mbstok_context = NULL;
				}
				else if (lpMarkup->NumberOfOperand >= 3 && (lpContext = IsInteger ? (LPSTR *)(uintptr_t)lpOperandTop[2].Quad : (LPSTR *)(uintptr_t)lpOperandTop[2].Real))
				{
					if (element->Next->Next->Param->Tag == TAG_PARAM_LOCAL)
					{
						if (IsBadReadPtr(lpAddress = lpContext, sizeof(*lpContext)))
							goto READ_ERROR;
						mbstok_context = *lpContext;
					}
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					{
						if (!ReadProcessMemory(hContextProcess = hProcess, lpAddress = lpContext, &mbstok_context, sizeof(*lpContext), NULL))
							goto READ_ERROR;
					}
					else
					{
						goto OPEN_ERROR;
					}
				}
				lpString1 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[0].Quad : (LPCSTR)(uintptr_t)lpOperandTop[0].Real;
				if (lpAddress = lpString1 ? (LPVOID)lpString1 : hTargetProcess ? mbstok_context : NULL)
				{
					if ((nSize = StringLengthA(hTargetProcess, lpAddress, -1)) == SIZE_MAX)
						goto READ_ERROR;
					if (hTargetProcess)
					{
						if (!(lpBuffer1 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
							goto ALLOC_ERROR;
						if (!ReadProcessMemory(hTargetProcess, lpAddress, lpBuffer1, nSize, NULL))
							goto MBSTOK_READ_ERROR;
						lpBuffer1[nSize] = '\0';
						if (!lpString1)
						{
							lpString1 = mbstok_context;
							mbstok_context = lpBuffer1;
						}
					}
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCSTR)(uintptr_t)lpOperandTop[1].Quad : (LPCSTR)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nSize;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto MBSTOK_OPEN_ERROR;
					if ((nSize = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == SIZE_MAX)
						goto MBSTOK_READ_ERROR;
					if (!(lpBuffer2 = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto MBSTOK_ALLOC_ERROR;
					if (ReadProcessMemory(hProcess, lpString2, lpBuffer2, nSize, NULL))
						((LPSTR)lpString2 = lpBuffer2)[nSize] = '\0';
					else
						goto MBSTOK_READ_ERROR;
				}
				if (lpResult = internal_mbstok(lpBuffer1 ? lpBuffer1 : lpString1, lpString2, &mbstok_context))
				{
					if (lpBuffer1)
					{
						LPSTR     lpTerminator;
						size_t    nSize;
						ptrdiff_t iDiff;

						nSize = mbstok_context - (lpTerminator = lpResult + strlen(lpResult));
						mbstok_context += (iDiff = lpString1 - lpBuffer1);
						lpResult += iDiff;
						if (nSize && !WriteProcessMemory(mbstok_process, lpAddress = lpTerminator + iDiff, lpTerminator, nSize, NULL))
							goto MBSTOK_WRITE_ERROR;
					}
					if (lpContext)
					{
						if (hContextProcess)
						{
							if (!WriteProcessMemory(hContextProcess, lpAddress = lpContext, &mbstok_context, sizeof(*lpContext), NULL))
								goto MBSTOK_WRITE_ERROR;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = lpContext, sizeof(*lpContext)))
								goto MBSTOK_WRITE_ERROR;
							*lpContext = mbstok_context;
						}
					}
				}
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (IsInteger)
				{
					lpOperandTop->Quad = (size_t)lpResult;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = (size_t)lpResult;
					lpOperandTop->IsQuad = TRUE;
				}
				break;

			MBSTOK_OPEN_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto OPEN_ERROR;

			MBSTOK_READ_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto READ_ERROR;

			MBSTOK_WRITE_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto WRITE_ERROR;

			MBSTOK_ALLOC_ERROR:
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				goto ALLOC_ERROR;
			}
			break;
		case TAG_STRLWR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_strlwr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_WCSLWR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPWSTR lpBuffer;
				LPWSTR lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop->Quad : (LPWSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthW(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_wcslwr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_MBSLWR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_mbslwr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_STRUPR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_strupr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_WCSUPR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPWSTR lpBuffer;
				LPWSTR lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop->Quad : (LPWSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthW(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_wcsupr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_MBSUPR:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_mbsupr(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_STRREV:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_strrev(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_WCSREV:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPWSTR lpBuffer;
				LPWSTR lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPWSTR)(uintptr_t)lpOperandTop->Quad : (LPWSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthW(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, (nSize *= sizeof(wchar_t)) + sizeof(wchar_t))))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						*(LPWSTR)((LPBYTE)lpBuffer + nSize) = L'\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_wcsrev(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_MBSREV:
			{
				HANDLE hTargetProcess;
				size_t nSize;
				LPSTR  lpBuffer;
				LPSTR  lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPSTR)(uintptr_t)lpOperandTop->Quad : (LPSTR)(uintptr_t)lpOperandTop->Real;
				if ((nSize = StringLengthA(hTargetProcess, lpAddress = (LPVOID)lpString, -1)) == SIZE_MAX)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, nSize + 1)))
						goto ALLOC_ERROR;
					if (ReadProcessMemory(hTargetProcess, lpString, lpBuffer, nSize, NULL))
						lpBuffer[nSize] = '\0';
					else
					{
						HeapFree(hHeap, 0, lpBuffer);
						goto READ_ERROR;
					}
				}
				_mbsrev(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					BOOL bSuccess;

					bSuccess = WriteProcessMemory(hProcess, (LPVOID)lpString, lpBuffer, nSize, NULL);
					HeapFree(hHeap, 0, lpBuffer);
					if (!bSuccess)
						goto WRITE_ERROR;
				}
			}
			break;
		case TAG_ISALNUM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= '0' && (lpOperandTop->Low <= '9' || lpOperandTop->Low >= 'A' && (lpOperandTop->Low <= 'Z' || lpOperandTop->Low >= 'a'));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISALPHA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= 'A' && (lpOperandTop->Low <= 'Z' || lpOperandTop->Low >= 'a');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISASCII:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISBLANK:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low == ' ' || lpOperandTop->Low == '\t');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCNTRL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low <= 0x1F || lpOperandTop->Low == 0x7F);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCSYM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= '0' && (lpOperandTop->Low <= '9' || lpOperandTop->Low >= 'A' && (lpOperandTop->Low <= 'Z' || lpOperandTop->Low == '_' || lpOperandTop->Low >= 'a'));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCSYMF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= 'A' && (lpOperandTop->Low <= 'Z' || lpOperandTop->Low == '_' || lpOperandTop->Low >= 'a');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= '9' && lpOperandTop->Low >= '0';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISGRAPH:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 0x7E && lpOperandTop->Low >= 0x21;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISKANA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				(BYTE)lpOperandTop->Low >= 0xA1 && (BYTE)lpOperandTop->Low <= 0xDF &&
				(!lpOperandTop->IsQuad ?
					!(dw = lpOperandTop->Low & (UINT32_MAX - 0xFF)) || !(dw ^ (UINT32_MAX - 0xFF)) :
					!(qw = lpOperandTop->Quad & (UINT64_MAX - 0xFF)) || !(qw ^ (UINT64_MAX - 0xFF)));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISLEADBYTE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				(BYTE)lpOperandTop->Low >= 0x81 && ((BYTE)lpOperandTop->Low <= 0x9F || ((BYTE)lpOperandTop->Low >= 0xE0 && (BYTE)lpOperandTop->Low <= 0xFC)) &&
				(!lpOperandTop->IsQuad ?
					!(dw = lpOperandTop->Low & (UINT32_MAX - 0xFF)) || !(dw ^ (UINT32_MAX - 0xFF)) :
					!(qw = lpOperandTop->Quad & (UINT64_MAX - 0xFF)) || !(qw ^ (UINT64_MAX - 0xFF)));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= 'a';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCHIRA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x829F && lpOperandTop->Low <= 0x82F1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCKATA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x8340 && lpOperandTop->Low <= 0x8396;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL0:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x8140 && lpOperandTop->Low <= 0x889E;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL1:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x889F && lpOperandTop->Low <= 0x9872;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL2:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x989F && lpOperandTop->Low <= 0xEA9E;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCLEGAL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x8140 && lpOperandTop->Low <= 0xFCFC && (lpOperandTop->Low <= 0x9FFC || lpOperandTop->Low >= 0xE040) && (BYTE)lpOperandTop->Low != 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCSYMBOL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (int32_t)lpOperandTop->Low >= 0x8141 && lpOperandTop->Low <= 0x81AC;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISPRINT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 0x7E && lpOperandTop->Low >= 0x20;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISPUNCT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 0x7E && lpOperandTop->Low >= 0x21 && (lpOperandTop->Low <= 0x2F || lpOperandTop->Low >= 0x3A && (lpOperandTop->Low <= 0x40 || lpOperandTop->Low >= 0x5B && (lpOperandTop->Low <= 0x60 || lpOperandTop->Low >= 0x7B)));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISSPACE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low == ' ' || lpOperandTop->Low <= '\r' && lpOperandTop->Low >= '\t');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISTRAILBYTE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				(BYTE)lpOperandTop->Low >= 0x40 && (BYTE)lpOperandTop->Low <= 0xFC && (BYTE)lpOperandTop->Low != 0x7F &&
				((char)lpOperandTop->Low >= 0 ?
					!(lpOperandTop->Quad & (UINT64_MAX - 0xFF)) :
					!lpOperandTop->IsQuad ?
						!(dw = lpOperandTop->Low & (UINT32_MAX - 0xFF)) || !(dw ^ (UINT32_MAX - 0xFF)) :
						!(qw = lpOperandTop->Quad & (UINT64_MAX - 0xFF)) || !(qw ^ (UINT64_MAX - 0xFF)));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'Z' && lpOperandTop->Low >= 'A';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISXDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 'f' && lpOperandTop->Low >= '0' && (lpOperandTop->Low <= '9' || lpOperandTop->Low >= 'A' && (lpOperandTop->Low <= 'F' || lpOperandTop->Low >= 'a'));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_TOASCII:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad &= 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_TOLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad <= 'Z' && lpOperandTop->Low >= 'A')
				lpOperandTop->Low += 'a' - 'A';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_TOUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad <= 'z' && lpOperandTop->Low >= 'a')
				lpOperandTop->Low -= 'a' - 'A';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
#if USE_PLUGIN
		case TAG_PLUGIN:
			{
				MARKUP          *element;
				size_t          stackSize;
				VARIABLE        *operand;
				PLUGIN_FUNCTION *function;
				uintptr_t       *stack;
				PARAM_TYPE      *paramType;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) < lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				function = lpMarkup->Function;
				paramType = function->ParamTypes;
				stackSize = 0;
				if (lpMarkup->NumberOfOperand)
				{
					element = lpMarkup;
#ifndef _WIN64
					operand = lpOperandTop;
#endif
					do
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
								(!operand->IsQuad && IsInteger || IsStringOperand(element->Param)) ?
									sizeof(uint32_t) :
									sizeof(uint64_t);
						}
						operand++;
#else
						if (paramType != function->EndOfParamTypes)
							paramType++;
						stackSize += sizeof(uintptr_t);
#endif
					} while (element = element->Next);
					if (stackSize < function->StackSize)
						stackSize = function->StackSize;
				}
				if (stackSize)
				{
					uintptr_t *param;

					stack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, stackSize);
					if (!stack)
						goto ALLOC_ERROR;
					param = stack;
					paramType = function->ParamTypes;
					element = lpMarkup;
					operand = lpOperandTop;
					do
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
							case PARAM_STRING_A:
							case PARAM_STRING_W:
								*(param++) = IsInteger ? (uintptr_t)operand->Quad : (uintptr_t)operand->Real;
								break;
							}
						}
						else if (!IsStringOperand(element->Param))
						{
#ifndef _WIN64
							if (!operand->IsQuad && IsInteger)
								*(param++) = operand->Low;
							else
#endif
								*(((uint64_t *)param)++) = operand->Quad;
						}
						else
						{
							*(param++) = IsInteger ? (uintptr_t)operand->Quad : (uintptr_t)operand->Real;
						}
						operand++;
					} while (element = element->Next);
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
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
								lpOperandTop->High = 0;
					}
					break;
				default:
					OPERAND_CLEAR();
					break;
				}
				if (!ParsingContinue)
					goto FAILED;
			}
			break;
#endif
		case TAG_NOT_OPERATOR:
			{
				size_t          length;
				LPSTR           p, end;
				MARKUP_VARIABLE *element;
				char            *endptr;
				LPSTR           lpEndOfModuleName;
				LPSTR           lpModuleName;
				char            c;

				end = (p = lpMarkup->String) + (length = lpMarkup->Length);
				element = NULL;
				endptr = NULL;
				do	/* do { ... } while (0); */
				{
					size_t prefixLength;
					size_t i;

					if (!length)
						break;
					if (p[prefixLength = 0] != '\'' && (p[0] != 'u' ||
						p[prefixLength = 1] != '\'' && (p[1] != '8' ||
						p[prefixLength = 2] != '\'')))
					{
						if (!IsStringOperand(lpMarkup))
						{
							if (*p == '$')
							{
								p++;
								if (!--length)
									break;
							}
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
						}
						else
						{
							LPCVOID lpAddress;

							endptr = end;
							lpAddress = lpMarkup->UnescapedString;
							if (!(operand.IsQuad = !IsInteger))
								operand.Quad = (uintptr_t)lpAddress;
							else
								operand.Real = (uintptr_t)lpAddress;
							break;
						}
					}
					else
					{
						__int64 n;

						endptr = p + prefixLength + 1;
						if (prefixLength < 1)
							n = UnescapeAnsiCharA(&endptr, end);
						else if (prefixLength == 1)
							n = UnescapeUnicodeCharA(&endptr, end);
						else/* if (prefixLength == 2)*/
							n = UnescapeUtf8CharA(&endptr, end);
						if (endptr < end && *endptr == '\'')
							endptr++;
						if (!(operand.IsQuad = !IsInteger))
							operand.Quad = (uint32_t)n;
						else
							operand.Real = (double)n;
					}
					if (endptr == end)
						break;
					for (i = 0; i < nNumberOfVariable; i++)
					{
						if (lpVariable[i].Length != length)
							continue;
						if (memcmp(lpVariable[i].String, p, length) != 0)
							continue;
						element = lpVariable + i;
						break;
					}
				} while (0);
				lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL;
				if (!element && length && (p[0] == SCOPE_PREFIX || lpNext && (lpNext->Tag == TAG_INC ||
																			  lpNext->Tag == TAG_DEC ||
																			  lpNext->Tag == TAG_ADDRESS_OF)))
				{
					if (!(nNumberOfVariable & 0x0F))
					{
						LPVOID lpMem;
						size_t nBytes;

						nBytes = (nNumberOfVariable + 0x10) * sizeof(MARKUP_VARIABLE);
						lpMem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, lpVariable, nBytes);
						if (!lpMem)
							goto ALLOC_ERROR;
						lpVariable = (MARKUP_VARIABLE *)lpMem;
					}
					element = lpVariable + nNumberOfVariable++;
					element->Length = length;
					element->String = p;
					element->Value.Quad = 0;
					element->Value.IsQuad = !IsInteger;
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
									element->Value.Quad = *(uint64_t *)pair_second(it, key);
									element->Value.IsQuad = !IsInteger || !!element->Value.High;
									element->Node = it;
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
									element->Node = it;
									break;
								}
#endif
							}
						}
						if (scope && !element->Node) {
#if !defined(__BORLANDC__)
							heapMapPair val = { key, { element->Value.Low, element->Value.High } };
							map_insert(&element->Node, &scope->heapMap, map_lower_bound(&scope->heapMap, &val.key), &val);
#else
							element->Node = scope->heapMap.insert(make_pair(key, make_pair(element->Value.Low, element->Value.High))).first;
#endif
						}
					}
#endif
				}
				switch (lpNext ? lpNext->Tag : -1)
				{
#if SCOPE_SUPPORT
				case TAG_ADDRESS_OF:
					if (!element)
						break;
#if !defined(__BORLANDC__)
					operand.Quad = element->Node && (&lpPostfix[2] >= lpEndOfPostfix || lpPostfix[2]->Tag == TAG_RETURN)
						? (uint64_t)pair_second(element->Node, uint32_t)
						: (uint64_t)&element->Value;
#else
					operand.Quad = element->Node && (&lpPostfix[2] >= lpEndOfPostfix || lpPostfix[2]->Tag == TAG_RETURN)
						? (uint64_t)&element->Node->second
						: (uint64_t)&element->Value;
#endif
					if (IsInteger)
						operand.IsQuad = sizeof(LPVOID) > sizeof(uint32_t);
					else {
						operand.Real = (size_t)operand.Quad;
						operand.IsQuad = TRUE;
					}
					OPERAND_PUSH(operand);
					lpPostfix++;
					if (!TSSGCtrl_GetSSGActionListner(this))
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
					lpPostfix++;
					if (!TSSGCtrl_GetSSGActionListner(this))
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
					lpPostfix++;
					if (!TSSGCtrl_GetSSGActionListner(this))
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
								goto ALLOC_ERROR;
							lpVariable = (MARKUP_VARIABLE *)lpMem;
						}
						element = lpVariable + nNumberOfVariable++;
						element->Length = length;
						element->String = p;
					}
					element->Value = *lpOperandTop;
					lpPostfix++;
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
						lpme = TProcessCtrl_GetModuleFromName(&this->processCtrl, lpMarkup->String);
						lpMarkup->String[lpMarkup->Length] = c;
						operand.Quad = lpme ? (uintptr_t)lpme->hModule : 0;
						if (IsInteger)
							operand.IsQuad = sizeof(lpme->hModule) > sizeof(uint32_t);
						else {
							operand.Real = (size_t)operand.Quad;
							operand.IsQuad = TRUE;
						}
						OPERAND_PUSH(operand);
					}
					lpPostfix++;
					break;
				case TAG_PROCEDURE:
					if ((HMODULE)lpOperandTop->Quad)
					{
						LPSTR lpProcName;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
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
							lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpOperandTop->Quad = (uintptr_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpProcName);
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
					lpPostfix++;
					break;
				case TAG_MODULENAME:
					lpEndOfModuleName = (lpModuleName = lpNext->String) + lpNext->Length;
					c = *lpEndOfModuleName;
					*lpEndOfModuleName = '\0';
					if (TSSGCtrl_GetSSGActionListner(this))
					{
#if defined(__BORLANDC__)
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(TSSGCtrl_GetSSGActionListner(this), SSGS, lpModuleName, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(this), SSGS, lpModuleName, 0);
#else
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(lpModuleName, lpNext->Length, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(lpModuleName, lpNext->Length, 0);
#endif
					}
					lpPostfix++;
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

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
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
						lpOperandTop->Quad = (uintptr_t)lpFunction;
						lpMarkup->String[lpMarkup->Length] = c2;
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
					if (lpEndOfModuleName)
						*lpEndOfModuleName = c;
					lpPostfix++;
					break;
				case TAG_SECTION:
					if ((HMODULE)lpOperandTop->Quad)
					{
						BOOL  IsEndOfSection;
						DWORD dwSectionSize;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						IsEndOfSection = *(uint16_t *)lpNext->String == BSWAP16(':+');
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						lpOperandTop->Quad = (uintptr_t)GetSectionAddress(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, IsEndOfSection ? &dwSectionSize : NULL);
						if (*(uintptr_t *)&lpOperandTop->Quad && IsEndOfSection)
							*(uintptr_t *)&lpOperandTop->Quad += dwSectionSize;
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
					lpPostfix++;
					break;
				case TAG_PROCESSID:
					{
						LPMODULEENTRY32A lpme;

						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpme = TProcessCtrl_GetModuleFromName(&this->processCtrl, lpMarkup->String);
						lpMarkup->String[lpMarkup->Length] = c;
						operand.Quad = lpme ? lpme->th32ProcessID : 0;
						if (operand.IsQuad = !IsInteger)
							operand.Real = operand.Low;
						OPERAND_PUSH(operand);
					}
					lpPostfix++;
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
			goto PARSING_ERROR;
		}
		if (TSSGCtrl_GetSSGActionListner(this))
		{
#if defined(__BORLANDC__)
			char c;

			c = (lpGuideText = lpMarkup->String)[lpMarkup->Length];
			lpGuideText[lpMarkup->Length] = '"';
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(TSSGCtrl_GetSSGActionListner(this), SSGS, lpGuideText, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(TSSGCtrl_GetSSGActionListner(this), SSGS, lpGuideText, lpOperandTop->Real);
			if (lpGuideText == lpMarkup->String)
				lpGuideText[lpMarkup->Length] = c;
#else
			lpGuideText = lpMarkup->String;
			nGuideTextLength = lpMarkup->Length;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(lpGuideText, nGuideTextLength, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(
					lpGuideText,
					nGuideTextLength,
					lpOperandTop->IsQuad ? lpOperandTop->Real : (double)*(float *)&lpOperandTop->Low);
#endif
		}
		if (lpMarkup->Tag != TAG_RETURN)
			continue;
		break;

	PARSING_ERROR:
		if (TSSGCtrl_GetSSGActionListner(this))
		{
			lpMarkup->String[lpMarkup->Length] = '\0';
			TSSGActionListner_OnParsingError(TSSGCtrl_GetSSGActionListner(this), SSGS, lpMarkup->String);
		}
		goto FAILED;

	OPEN_ERROR:
		//TSSGActionListner_OnProcessOpenError(TSSGCtrl_GetSSGActionListner(this), SSGS);
		goto FAILED;

	READ_ERROR:
		TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(this), SSGS, (uint32_t)lpAddress);
		goto FAILED;

	WRITE_ERROR:
		TSSGActionListner_OnSubjectWriteError(TSSGCtrl_GetSSGActionListner(this), SSGS, (uint32_t)lpAddress);
		goto FAILED;

	ALLOC_ERROR:
		lpMessage = "メモリの確保に失敗しました。";
		goto GUIDE;

	DIVIDE_BY_ZERO:
		lpMessage = "0 で除算しました。";
		goto GUIDE;

	FAILED_GET_PROCESS_TIMES:
		lpMessage = "プロセスに関する時間情報を取得できませんでした。";
		goto GUIDE;

	FAILED_ADDR_REPLACE:
		lpMessage = "オフセット演算に失敗しました。";
		goto GUIDE;

	FAILED_ADDR_ADJUST:
		lpMessage = "アドレス補正に失敗しました。";
		goto GUIDE;

	GUIDE:
		if (TMainForm_GetUserMode(MainForm) != 1)
			TMainForm_Guide(lpMessage, 0);
		goto FAILED;
	}
	qwResult = lpOperandTop->Quad;
FAILED:
	if (lpHeapBuffer)
	{
		size_t i;

		i = nNumberOfHeapBuffer;
		while (i)
			HeapFree(hHeap, 0, lpHeapBuffer[--i]);
		HeapFree(hHeap, 0, lpHeapBuffer);
	}
	if (hProcess)
		CloseHandle(hProcess);
#if SCOPE_SUPPORT
	for (size_t i = 0; i < nNumberOfVariable; i++)
	{
		register PMARKUP_VARIABLE v = &lpVariable[i];
		if (v->Node)
		{
#if !defined(__BORLANDC__)
			*(uint64_t*)pair_second(v->Node, uint32_t) = v->Value.Quad;
#else
			v->Node->second = make_pair(v->Value.Low, v->Value.High);
#endif
		}
	}
#endif
	if (TSSGCtrl_GetSSGActionListner(this) && TMainForm_GetUserMode(MainForm) >= 3 &&
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
#endif
	if (lpVariable)
		HeapFree(hHeap, 0, lpVariable);
	if (lpOperandBuffer)
		HeapFree(hHeap, 0, lpOperandBuffer);
	if (lpPostfixBuffer)
		HeapFree(hHeap, 0, lpPostfixBuffer);
	if (lpConstStringBuffer)
		VirtualFree(lpConstStringBuffer, 0, MEM_RELEASE);
	if (lpMarkupArray)
		HeapFree(hHeap, 0, lpMarkupArray);
	if (lpszSrc)
		HeapFree(hHeap, 0, lpszSrc);
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
#undef TMainForm_Guide
#undef TSSGCtrl_GetAttribute
#undef TSSGCtrl_GetSSGActionListner
#undef TSSGCtrl_AddressAttributeFilter
#undef TSSGSubject_GetAttribute
#undef TSSGSubject_GetSize
#undef TSSGActionListner_OnProcessOpenError
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
#undef OS_STRING

#undef AllocMarkup

