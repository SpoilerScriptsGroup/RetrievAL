#ifndef _SUBJECTSTRINGOPERATOR_H_
#define _SUBJECTSTRINGOPERATOR_H_

#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl SubjectStringTable_StringCtor(string *s);
string * __fastcall SubjectStringTable_GetString(string *s);
void __fastcall SubjectStringTable_SetString(string *dest, string *src);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGOPERATOR_H_

