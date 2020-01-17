#include <math.h>

// Floating point used flag
const int _fltused = 0x9875;

// Floating point infinity
const double fpconst_inf       = HUGE_VAL;  // 0x7FF0000000000000
const double fpconst_minus_inf = -HUGE_VAL; // 0xFFF0000000000000

// Floating point NaN
const double fpconst_nan       = -NAN;      // 0x7FF8000000000000
const double fpconst_nan_ind   =  NAN;      // 0xFFF8000000000000

// Floating point constants
const double fpconst_half      =  0.5;
const double fpconst_one       =  1.0;
const double fpconst_two       =  2.0;
const double fpconst_minus_one = -1.0;
