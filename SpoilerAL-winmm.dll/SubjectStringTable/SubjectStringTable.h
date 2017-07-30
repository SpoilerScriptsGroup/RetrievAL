#ifndef _SUBJECTSTRINGTABLE_H_
#define _SUBJECTSTRINGTABLE_H_

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bcb6_std_vector SubjectStringTable_array;
extern bcb6_std_vector SubjectStringTable_indices;

void __cdecl SubjectStringTable_dtor();
void __cdecl SubjectStringTable_clear();
size_t __fastcall SubjectStringTable_insert(bcb6_std_string *s);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGTABLE_H_

