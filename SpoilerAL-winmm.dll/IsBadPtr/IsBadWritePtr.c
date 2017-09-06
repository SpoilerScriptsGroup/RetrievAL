#define IsBadPtr IsBadWritePtrCompatible

#define IsValidAddress(Protect) (   \
    ((Protect) & (                  \
        PAGE_READWRITE         |    \
        PAGE_WRITECOPY         |    \
        PAGE_EXECUTE_READWRITE |    \
        PAGE_EXECUTE_WRITECOPY)) && \
    !((Protect) & (                 \
        PAGE_NOACCESS          |    \
        PAGE_GUARD)))

#include "IsBadPtrSource.h"
