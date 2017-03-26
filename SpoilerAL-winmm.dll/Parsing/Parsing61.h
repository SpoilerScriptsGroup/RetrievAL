#ifndef _PARSING61_H_
#define _PARSING61_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"

#if defined(__BORLANDC__)
#define pbcb6_std_string                                                                 string
#define bcb6_std_string_c_str(str)                                                       (str).c_str()
#define bcb6_std_string_length(str)                                                      (str).length()
#define TSSGCtrl_GetAttribute(SSGCtrl, SSGS, Type)                                       (SSGCtrl)->GetAttribute(SSGS, Type)
#define TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, Address, Mode)                    (SSGCtrl)->AddressAttributeFilter(SSGS, Address, Mode)
#define TSSGSubject_GetAttribute(SSGS)                                                   (SSGS)->GetAttribute();
#define TSSGActionListner_OnParsingError(SSGActionListner, SSGS, Code)                   (SSGActionListner)->OnParsingError(SSGS, Code)
#define TSSGActionListner_OnParsingProcess61(SSGActionListner, SSGS, Code, TopVal)       (SSGActionListner)->OnParsingProcess(SSGS, Code, TopVal)
#define TSSGActionListner_OnParsingDoubleProcess61(SSGActionListner, SSGS, Code, TopVal) (SSGActionListner)->OnParsingDoubleProcess(SSGS, Code, TopVal)
#define TProcessCtrl_Open(ProcessCtrl, Mode)                                             (ProcessCtrl).Open(Mode)
#define TProcessCtrl_GetModuleFromName(ProcessCtrl, Name)                                (ProcessCtrl).GetModuleFromName(Name)
#define TProcessCtrl_GetHeapList(ProcessCtrl, ListNo)                                    (ProcessCtrl).GetHeapList(ListNo)
#else
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGActionListner.h"
#include "THeapListData.h"
#include "TProcessCtrl.h"
#endif

#if !defined(__BORLANDC__)
#pragma function(memcmp)
extern HANDLE hHeap;
#endif

/*
 100 (                 OS_OPEN
  90 [_                OS_OPEN
  85 [.                OS_OPEN
  83 [~                OS_OPEN
  80 [:                OS_OPEN
  75 MName:: HNumber:: OS_PUSH
  51 * / %             OS_PUSH            乗算 除算 剰余算
  50 + -               OS_PUSH            加算 減算
  45 ~ & ^ |           OS_PUSH            ビットごとの論理否定 ビットごとの論理積 ビットごとの排他的論理和 ビットごとの論理和
  40 < > <= >=         OS_PUSH            小なり 大なり 以下 以上
  35 == !=             OS_PUSH            等価 非等価
  30 && ||             OS_PUSH            論理積 論理和
  25 =>                OS_PUSH            右辺代入
  20 :] :3] :2] :1]    OS_CLOSE | OS_PUSH
  18 ~] ~3] ~2]        OS_CLOSE | OS_PUSH
  15 .]                OS_CLOSE | OS_PUSH
  10 _]                OS_CLOSE | OS_PUSH
   0 )                 OS_CLOSE
*/

#define OS_PUSH    0x01
#define OS_OPEN    0x02
#define OS_CLOSE   0x04
#define AT_ADJUST  0x02
#define AT_REPLACE 0x04

typedef enum {
	TAG_NOT_OPERATOR     ,  // 127           OS_PUSH
	TAG_PARENTHESIS_OPEN ,  // 100 (         OS_OPEN
	TAG_ADDR_ADJUST_OPEN ,  //  90 [_        OS_OPEN
	TAG_ADDR_REPLACE_OPEN,  //  85 [.        OS_OPEN
	TAG_REV_ENDIAN_OPEN  ,  //  83 [~        OS_OPEN
	TAG_READ_OPEN        ,  //  80 [:        OS_OPEN
	TAG_MNAME            ,  //  75 MName::   OS_PUSH
	TAG_HNUMBER          ,  //  75 HNumber:: OS_PUSH
	TAG_MUL              ,  //  51 *         OS_PUSH
	TAG_DIV              ,  //  51 /         OS_PUSH
	TAG_MOD              ,  //  51 %         OS_PUSH
	TAG_ADD              ,  //  50 +         OS_PUSH
	TAG_SUB              ,  //  50 -         OS_PUSH
	TAG_BIT_NOT          ,  //  45 ~         OS_PUSH
	TAG_BIT_AND          ,  //  45 &         OS_PUSH
	TAG_XOR              ,  //  45 ^         OS_PUSH
	TAG_BIT_OR           ,  //  45 |         OS_PUSH
	TAG_BT               ,  //  40 <         OS_PUSH
	TAG_AT               ,  //  40 >         OS_PUSH
	TAG_BE               ,  //  40 <=        OS_PUSH
	TAG_AE               ,  //  40 >=        OS_PUSH
	TAG_EQ               ,  //  35 ==        OS_PUSH
	TAG_NE               ,  //  35 !=        OS_PUSH
	TAG_AND              ,  //  30 &&        OS_PUSH
	TAG_OR               ,  //  30 ||        OS_PUSH
	TAG_ASSIGN           ,  //  25 =>        OS_PUSH
	TAG_READ4            ,  //  20 :]        OS_CLOSE | OS_PUSH
	TAG_READ3            ,  //  20 :3]       OS_CLOSE | OS_PUSH
	TAG_READ2            ,  //  20 :2]       OS_CLOSE | OS_PUSH
	TAG_READ1            ,  //  20 :1]       OS_CLOSE | OS_PUSH
	TAG_REV_ENDIAN4      ,  //  18 ~]        OS_CLOSE | OS_PUSH
	TAG_REV_ENDIAN3      ,  //  18 ~3]       OS_CLOSE | OS_PUSH
	TAG_REV_ENDIAN2      ,  //  18 ~2]       OS_CLOSE | OS_PUSH
	TAG_ADDR_REPLACE     ,  //  15 .]        OS_CLOSE | OS_PUSH
	TAG_ADDR_ADJUST      ,  //  10 _]        OS_CLOSE | OS_PUSH
	TAG_PARENTHESIS_CLOSE,  //   0 )         OS_CLOSE
	TAG_SPLIT            ,  //   0 ;         0
} TAG;

typedef struct {
	TAG    Tag;
	size_t Length;
	LPSTR  String;
	char   Priority;
	char   Type;
} MARKUP, *PMARKUP;

typedef struct {
	size_t Length;
	LPSTR  String;
	DWORD  Value;
} DWORD_VARIABLE, *PDWORD_VARIABLE;

typedef struct {
	size_t Length;
	LPCSTR String;
	double Value;
} FLOAT_VARIABLE, *PFLOAT_VARIABLE;

MARKUP *Markup61(IN LPCSTR lpSrc, IN size_t nSrcLength, OUT LPSTR *lppMarkupStringBuffer, OUT size_t *lpnNumberOfMarkup);
size_t Postfix61(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT MARKUP **lpPostfix, IN MARKUP **lpFactorBuffer, IN size_t *lpnNestBuffer);
#if !defined(__BORLANDC__)
EXTERN_C unsigned long __cdecl Parsing61(IN TSSGCtrl *SSGCtrl, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, IN unsigned long Val);
#endif

#endif	// _PARSING61_H_
