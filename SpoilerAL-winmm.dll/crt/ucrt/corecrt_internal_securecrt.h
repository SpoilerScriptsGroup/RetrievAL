/***
*corecrt_internal_securecrt.h - contains declarations of internal routines and variables for securecrt
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Declares routines and variables used internally in the SecureCRT implementation.
*       In this include file we define the macros needed to implement the secure functions
*       inlined in the *.inl files like tcscpy_s.inl, etc.
*
*       [Internal]
*
****/
#ifndef _CORECRT_INTERNAL_SECURECRT_H_
#define _CORECRT_INTERNAL_SECURECRT_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <memory.h>

#pragma function(memset)

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String resetting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _RESET_STRING(_String, _Count)                                          \
    memset(_String, 0,                                                          \
        (_Count) != (size_t)-1 && (_Count) != INT_MAX && (_Count) ?             \
            (_Count) * sizeof(*(_String)) :                                     \
            sizeof(*(_String)))

#endif	// _CORECRT_INTERNAL_SECURECRT_H_
