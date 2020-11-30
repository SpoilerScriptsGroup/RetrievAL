//
// corecrt_internal.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// CoreCRT Internals
//
#ifndef _CORECRT_INTERNAL_H_
#define _CORECRT_INTERNAL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <crtdbg.h>
#ifndef _ASSERT_EXPR
#define _ASSERT_EXPR(expr, msg) ((void)0)
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Precondition Validation Macros
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _VALIDATE_RETURN(expr, errorcode, retexpr)                             \
    do                                                                         \
    {                                                                          \
        int _expr_val = !!(expr);                                              \
                                                                               \
        _ASSERT_EXPR(_expr_val, _CRT_WIDE(#expr));                             \
        if (!_expr_val)                                                        \
        {                                                                      \
            errno = errorcode;                                                 \
            return retexpr;                                                    \
        }                                                                      \
    } while (0)

#define _VALIDATE_RETURN_ERRCODE(expr, errorcode)                              \
    do                                                                         \
    {                                                                          \
        int _expr_val = !!(expr);                                              \
                                                                               \
        _ASSERT_EXPR(_expr_val, _CRT_WIDE(#expr));                             \
        if (!_expr_val)                                                        \
        {                                                                      \
            errno = errorcode;                                                 \
            return errorcode;                                                  \
        }                                                                      \
    } while (0)

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// CRT SAL Annotations
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _CRT_UNBOUNDED_BUFFER_SIZE  ((size_t)-1)

#endif	// _CORECRT_INTERNAL_H_
