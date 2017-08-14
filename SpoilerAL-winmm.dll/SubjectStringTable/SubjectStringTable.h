#ifndef _SUBJECTSTRINGTABLE_H_
#define _SUBJECTSTRINGTABLE_H_

#include <windows.h>

#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#define typename size_t
#include "bcb6_std_vector_template.h"

#ifdef __cplusplus
extern "C" {
#endif

extern vector_string SubjectStringTable_array;
extern vector_size_t SubjectStringTable_indices;

void __cdecl SubjectStringTable_dtor();
void __cdecl SubjectStringTable_clear();
size_t __fastcall SubjectStringTable_insert(const string *s);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGTABLE_H_

