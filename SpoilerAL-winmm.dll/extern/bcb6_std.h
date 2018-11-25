#pragma once

#ifdef USING_NAMESPACE_BCB6
#define std_swap bcb6_std_swap
#elif defined(USING_NAMESPACE_BCB6_STD)
#define swap bcb6_std_swap
#endif

#define bcb6_std_swap(a, b)            \
do                                     \
{                                      \
    typedef struct {                   \
        char data[sizeof(*(a))];       \
    } DATA;                            \
                                       \
    DATA *__restrict _a = (DATA *)(a); \
    DATA *__restrict _b = (DATA *)(b); \
    DATA swap = *_a;                   \
    *_a = *_b;                         \
    *_b = swap;                        \
} while (0)
