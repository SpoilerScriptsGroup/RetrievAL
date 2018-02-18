#undef _UNICODE
#undef UNICODE

#include <windows.h>

#if _MSC_VER < 1310
#define TCHAR char
#define _tcslen strlen
#endif

#define SHRINK_BUFFER 1
#define ParseArgumentT ParseArgumentA
#include "ParseArgumentT.h"
