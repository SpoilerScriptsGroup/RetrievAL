#ifndef _FPCONST_H_
#define _FPCONST_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const int _fltused;
extern const double _minus_inf;
extern const double _nan;
extern const double _nan_ind;
extern const double _half;
extern const double _one;
extern const double _two;
extern const double _minus_one;

#ifdef __cplusplus
}
#endif

#endif	// _FPCONST_H_
