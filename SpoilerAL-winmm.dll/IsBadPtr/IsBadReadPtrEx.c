#define IsBadPtr IsBadReadPtrEx

#define IsReadableProtect(Protect) ( \
    ((Protect) & (                   \
        PAGE_READONLY          |     \
        PAGE_READWRITE         |     \
        PAGE_WRITECOPY         |     \
        PAGE_EXECUTE_READ      |     \
        PAGE_EXECUTE_READWRITE |     \
        PAGE_EXECUTE_WRITECOPY)) &&  \
    !((Protect) & (                  \
        PAGE_NOACCESS          |     \
        PAGE_GUARD)))

#define IsValidProtect IsReadableProtect

#include "IsBadPtrSource.h"
