#ifndef _SUBJECTSTRINGOPERATOR_H_
#define _SUBJECTSTRINGOPERATOR_H_

#include "bcb6_std_string.h"

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl SubjectStringTable_StringCtor(bcb6_std_string *s);
bcb6_std_string * __fastcall SubjectStringTable_GetString(bcb6_std_string *s);
void __fastcall SubjectStringTable_SetString(bcb6_std_string *dest, bcb6_std_string *src);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGOPERATOR_H_

