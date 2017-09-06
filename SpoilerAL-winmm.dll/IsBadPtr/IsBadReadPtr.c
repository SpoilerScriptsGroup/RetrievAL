#define IsBadPtr IsBadReadPtrCompatible

#define IsValidAddress(Protect) (   \
    ((Protect) & (                  \
        PAGE_READONLY          |    \
        PAGE_READWRITE         |    \
        PAGE_WRITECOPY         |    \
        PAGE_EXECUTE_READ      |    \
        PAGE_EXECUTE_READWRITE |    \
        PAGE_EXECUTE_WRITECOPY)) && \
    !((Protect) & (                 \
        PAGE_NOACCESS          |    \
        PAGE_GUARD)))

#include "IsBadPtrSource.h"
