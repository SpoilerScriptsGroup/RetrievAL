#ifndef _SUBJECTSTRINGTABLE_H_
#define _SUBJECTSTRINGTABLE_H_

#include <windows.h>

#include "bcb6_std_vector.h"
#include "bcb6_std_vector_string.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bcb6_std_vector_string SubjectStringTable_array;
extern bcb6_std_vector_size_t SubjectStringTable_indices;

void __cdecl SubjectStringTable_dtor();
void __cdecl SubjectStringTable_clear();
size_t __fastcall SubjectStringTable_insert(const bcb6_std_string *s);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGTABLE_H_

