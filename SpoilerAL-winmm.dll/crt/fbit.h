#ifndef _FBIT_H_
#define _FBIT_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

double __cdecl fband(double x, double y);
double __cdecl fbor(double x, double y);
double __cdecl fbxor(double x, double y);
double __cdecl fbnot(double x);

#ifdef __cplusplus
}
#endif

#endif	// _FBIT_H_
