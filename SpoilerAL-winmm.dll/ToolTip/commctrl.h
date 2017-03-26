#ifndef _USER_COMMCTRL_H_
#define _USER_COMMCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _WIN32_WINNT
#define _PREV_WIN32_WINNT _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#ifdef _WIN32_IE
#define _PREV_WIN32_IE _WIN32_IE
#undef _WIN32_IE
#endif

#define _WIN32_WINNT _WIN32_WINNT_WIN2K
#define _WIN32_IE    _WIN32_IE_IE501
#include <commctrl.h>

#ifdef _PREV_WIN32_IE
#undef _WIN32_IE
#define _WIN32_IE _PREV_WIN32_IE
#undef _PREV_WIN32_IE
#endif

#ifdef _PREV_WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT _PREV_WIN32_WINNT
#undef _PREV_WIN32_WINNT
#endif

#endif	// _USER_COMMCTRL_H_
