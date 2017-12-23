#include "TranscodeMultiByte.h"

void __stdcall AddressNamingFromUtf8(unsigned long DataSize, char *tmpC)
{
	Utf8ToMultiByte(CP_THREAD_ACP, 0, tmpC, -1, tmpC, DataSize + 1, NULL, NULL);
}
