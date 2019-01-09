#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_Remove(
	OUT    string          *Result,
	IN     TStringDivision *Reserved1,
	IN     const string    *Src,
	IN     string          Token,
	IN     const char      *Dest,
	IN     unsigned long   Reserved2)
{
	size_t tokenLength;

	string_ctor_assign(Result, Src);
	tokenLength = string_length(&Token);
	if (tokenLength != 0)
	{
		LPSTR  p;
		size_t destLength;

		p = string_begin(Result);
		destLength = Dest != NULL ? strlen(Dest) : 0;
		if (destLength == tokenLength)
		{
			while (p < string_end(Result))
			{
				if (memcmp(p, string_c_str(&Token), destLength) == 0)
				{
					memcpy(p, Dest, destLength);
					p += destLength;
				}
				else if (!__intrinsic_isleadbyte(*p))
					p++;
				else
					p += 2;
			}
		}
		else
		{
			size_t resultLength;

			resultLength = string_length(Result);
			if (destLength < tokenLength)
			{
				if (destLength == 0)
				{
					while (p < string_end(Result))
					{
						if (memcmp(p, string_c_str(&Token), tokenLength) == 0)
						{
							LPSTR copySrc;

							copySrc = p + tokenLength;
							memcpy(p += destLength, copySrc, string_end(Result) - copySrc + 1);
							string_end(Result) = string_begin(Result) + (resultLength -= tokenLength);
						}
						else if (!__intrinsic_isleadbyte(*p))
							p++;
						else
							p += 2;
					}
				}
				else
				{
					while (p < string_end(Result))
					{
						if (memcmp(p, string_c_str(&Token), tokenLength) == 0)
						{
							LPSTR copySrc;

							memcpy(p, Dest, destLength);
							copySrc = p + tokenLength;
							memcpy(p += destLength, copySrc, string_end(Result) - copySrc + 1);
							string_end(Result) = string_begin(Result) + (resultLength -= tokenLength - destLength);
						}
						else if (!__intrinsic_isleadbyte(*p))
							p++;
						else
							p += 2;
					}
				}
			}
			else
			{
				while (p < string_end(Result))
				{
					if (memcmp(p, string_c_str(&Token), tokenLength) == 0)
					{
						LPSTR moveDest;

						p -= (size_t)string_begin(Result);
						string_resize(Result, resultLength += destLength - tokenLength);
						p += (size_t)string_begin(Result);
						moveDest = p + destLength;
						memmove(moveDest, p + tokenLength, string_end(Result) - moveDest);
						memcpy(p, Dest, destLength);
						p = moveDest;
					}
					else if (!__intrinsic_isleadbyte(*p))
						p++;
					else
						p += 2;
				}
			}
		}
	}
	string_dtor(&Token);
	return Result;
}
