#include <windows.h>

HMODULE hMsImg32 = NULL;

static DWORD dwOSMajorVersion = 0;

typedef struct tagBGRA {
	BYTE Blue;
	BYTE Green;
	BYTE Red;
	BYTE Alpha;
} BGRA, NEAR* PBGRA, FAR* LPBGRA;

#pragma warning(push)
#pragma warning(disable:4273)
EXTERN_C BOOL __stdcall AlphaBlendCompatible(
	IN HDC           hdcDest,
	IN int           nXOriginDest,
	IN int           nYOriginDest,
	IN int           nWidthDest,
	IN int           nHeightDest,
	IN HDC           hdcSrc,
	IN int           nXOriginSrc,
	IN int           nYOriginSrc,
	IN int           nWidthSrc,
	IN int           nHeightSrc,
	IN BLENDFUNCTION blendFunction)
{
	DWORD dwErrCode;
	HDC   hDC;
	BOOL  bResult;
	HDC   hDestMemDC;

	do
	{
		DWORD dwFlags;

		dwFlags = *(LPDWORD)&blendFunction & 0xFF00FFFF;
		if (dwFlags == AC_SRC_OVER)
			break;
		else if (dwFlags != (AC_SRC_OVER | ((DWORD)AC_SRC_ALPHA << 24)))
			dwErrCode = ERROR_INVALID_PARAMETER;
		else if (dwOSMajorVersion >= 4)
			break;
		else if (dwOSMajorVersion)
			dwErrCode = ERROR_OLD_WIN_VERSION;
		else
		{
			OSVERSIONINFOW OSVersionInfo;

			OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);
#pragma warning(push)
#pragma warning(disable:4996)
			if (!GetVersionExW(&OSVersionInfo))
#pragma warning(pop)
				return FALSE;
			dwOSMajorVersion = OSVersionInfo.dwMajorVersion;
			if (dwOSMajorVersion >= 4)
				break;
			dwErrCode = ERROR_OLD_WIN_VERSION;
		}
		SetLastError(dwErrCode);
		return FALSE;
	} while (0);
	if (nXOriginDest + nWidthDest  <= 0 || nWidthDest  <= 0 ||
		nYOriginDest + nHeightDest <= 0 || nHeightDest <= 0 ||
		nXOriginSrc  + nWidthSrc   <= 0 || nWidthSrc   <= 0 ||
		nYOriginSrc  + nHeightSrc  <= 0 || nHeightSrc  <= 0 ||
		*(LPDWORD)&blendFunction == (AC_SRC_OVER | (0UL << 16)))
		return TRUE;
	if (*(LPDWORD)&blendFunction == (AC_SRC_OVER | (0xFFUL << 16)))
		return StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	hDC = GetDC(NULL);
	if (!hDC)
		return FALSE;
	bResult = FALSE;
	hDestMemDC = CreateCompatibleDC(hDC);
	if (hDestMemDC)
	{
		BITMAPV4HEADER bmi;
		HBITMAP        hDestBitmap;
		LPBGRA         pDestPixels;

		bmi.bV4Size          = sizeof(BITMAPINFOHEADER);
		bmi.bV4Width         = nWidthDest;
		bmi.bV4Height        = nHeightDest;
		bmi.bV4Planes        = 1;
		bmi.bV4BitCount      = 32;
		bmi.bV4V4Compression = BI_RGB;
		bmi.bV4SizeImage     = 0;
		bmi.bV4XPelsPerMeter = 0;
		bmi.bV4YPelsPerMeter = 0;
		bmi.bV4ClrUsed       = 0;
		bmi.bV4ClrImportant  = 0;
		hDestBitmap = CreateDIBSection(hDC, (const BITMAPINFO *)&bmi, DIB_RGB_COLORS, (void **)&pDestPixels, NULL, 0);
		if (hDestBitmap)
		{
			HBITMAP hDestOldBitmap;

			hDestOldBitmap = (HBITMAP)SelectObject(hDestMemDC, hDestBitmap);
			if (hDestOldBitmap)
			{
				HDC hSrcMemDC;

				if (BitBlt(hDestMemDC, 0, 0, nWidthDest, nHeightDest, hdcDest, nXOriginDest, nYOriginDest, SRCCOPY) &&
					(hSrcMemDC = CreateCompatibleDC(hDC)) != NULL)
				{
					HBITMAP hSrcBitmap;
					LPBGRA  pSrcPixels;

					if (blendFunction.AlphaFormat)
					{
						bmi.bV4Size                          = sizeof(BITMAPV4HEADER);
						bmi.bV4V4Compression                 = BI_BITFIELDS;
						bmi.bV4RedMask                       = 0x00FF0000;
						bmi.bV4GreenMask                     = 0x0000FF00;
						bmi.bV4BlueMask                      = 0x000000FF;
						bmi.bV4AlphaMask                     = 0xFF000000;
						bmi.bV4CSType                        = 0;
						bmi.bV4Endpoints.ciexyzRed.ciexyzX   = 0;
						bmi.bV4Endpoints.ciexyzRed.ciexyzY   = 0;
						bmi.bV4Endpoints.ciexyzRed.ciexyzZ   = 0;
						bmi.bV4Endpoints.ciexyzGreen.ciexyzX = 0;
						bmi.bV4Endpoints.ciexyzGreen.ciexyzY = 0;
						bmi.bV4Endpoints.ciexyzGreen.ciexyzZ = 0;
						bmi.bV4Endpoints.ciexyzBlue.ciexyzX  = 0;
						bmi.bV4Endpoints.ciexyzBlue.ciexyzY  = 0;
						bmi.bV4Endpoints.ciexyzBlue.ciexyzZ  = 0;
						bmi.bV4GammaRed                      = 0;
						bmi.bV4GammaGreen                    = 0;
						bmi.bV4GammaBlue                     = 0;
					}
					hSrcBitmap = CreateDIBSection(hDC, (const BITMAPINFO *)&bmi, DIB_RGB_COLORS, (void **)&pSrcPixels, NULL, 0);
					if (hSrcBitmap)
					{
						HBITMAP hSrcOldBitmap;

						hSrcOldBitmap = (HBITMAP)SelectObject(hSrcMemDC, hSrcBitmap);
						if (hSrcOldBitmap)
						{
							do	/* do { ... } wihle (0); */
							{
								LPBGRA pEndOfDestPixels;

								if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
								{
									if (!BitBlt(hSrcMemDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY))
										break;
								}
								else
								{
									int iStretchMode;

									iStretchMode = GetStretchBltMode(hdcDest);
									if (!iStretchMode)
										break;
									if (!SetStretchBltMode(hSrcMemDC, iStretchMode))
										break;
									if (!StretchBlt(hSrcMemDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY))
										break;
								}
								pEndOfDestPixels = pDestPixels + (unsigned int)nWidthDest * (unsigned int)nHeightDest;
								if (!blendFunction.AlphaFormat)
								{
									DWORD dwSrcRatio;
									DWORD dwDestRatio;

									dwSrcRatio = (DWORD)blendFunction.SourceConstantAlpha;
									dwSrcRatio += dwSrcRatio << 8;
									dwDestRatio = 65536 - dwSrcRatio;
									dwSrcRatio++;
									for (; pDestPixels != pEndOfDestPixels; pDestPixels++, pSrcPixels++)
									{
										pDestPixels->Blue = (BYTE)((
											pDestPixels->Blue * dwDestRatio +
											pSrcPixels->Blue * dwSrcRatio) >> 16);
										pDestPixels->Green = (BYTE)((
											pDestPixels->Green * dwDestRatio +
											pSrcPixels->Green * dwSrcRatio) >> 16);
										pDestPixels->Red = (BYTE)((
											pDestPixels->Red * dwDestRatio +
											pSrcPixels->Red * dwSrcRatio) >> 16);
									}
								}
								else
								{
									for (; pDestPixels != pEndOfDestPixels; pDestPixels++, pSrcPixels++)
									{
										DWORD dwSrcRatio;
										DWORD dwDestRatio;

										dwSrcRatio = (DWORD)pSrcPixels->Alpha;
										dwSrcRatio += dwSrcRatio << 8;
										dwDestRatio = 65536 - dwSrcRatio;
										dwSrcRatio++;
										pDestPixels->Blue = (BYTE)((
											pDestPixels->Blue * dwDestRatio +
											pSrcPixels->Blue * dwSrcRatio) >> 16);
										pDestPixels->Green = (BYTE)((
											pDestPixels->Green * dwDestRatio +
											pSrcPixels->Green * dwSrcRatio) >> 16);
										pDestPixels->Red = (BYTE)((
											pDestPixels->Red * dwDestRatio +
											pSrcPixels->Red * dwSrcRatio) >> 16);
									}
								}
								bResult = BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hDestMemDC, 0, 0, SRCCOPY);
							} while (0);
							dwErrCode = GetLastError();
							SelectObject(hSrcMemDC, hSrcOldBitmap);
						}
						else
						{
							dwErrCode = GetLastError();
						}
						DeleteObject(hSrcBitmap);
					}
					else
					{
						dwErrCode = GetLastError();
					}
					DeleteDC(hSrcMemDC);
				}
				else
				{
					dwErrCode = GetLastError();
				}
				SelectObject(hDestMemDC, hDestOldBitmap);
			}
			else
			{
				dwErrCode = GetLastError();
			}
			DeleteObject(hDestBitmap);
		}
		else
		{
			dwErrCode = GetLastError();
		}
		DeleteDC(hDestMemDC);
	}
	else
	{
		dwErrCode = GetLastError();
	}
	ReleaseDC(NULL, hDC);
	SetLastError(dwErrCode);
	return bResult;
}
#pragma warning(pop)
