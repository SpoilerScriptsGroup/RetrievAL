#include "Parsing61.h"

//---------------------------------------------------------------------
#define AllocMarkup() \
	(MARKUP *)HeapAlloc(hHeap, 0, 0x10 * sizeof(MARKUP))
//---------------------------------------------------------------------
extern MARKUP *ReAllocMarkup(MARKUP **lppMarkupArray, size_t *lpnNumberOfMarkup);
//---------------------------------------------------------------------
extern void TrimMarkupString(MARKUP *lpMarkup);
//---------------------------------------------------------------------
MARKUP *Markup61(IN LPCSTR lpSrc, IN size_t nSrcLength, OUT LPSTR *lppMarkupStringBuffer, OUT size_t *lpnNumberOfMarkup)
{
	size_t  nStringLength;
	LPSTR   lpMarkupStringBuffer;
	LPSTR   lpDest;
	char    ch;
	size_t  i;
	MARKUP  *lpTagArray;
	size_t  nNumberOfTag;
	BOOLEAN bIsLaedByte, bPrevIsTailByte;
	MARKUP  *lpMarkupArray;
	MARKUP  *lpMarkup, *lpEndOfMarkup;
	size_t  nMarkupIndex;
	size_t  nMarkupLength;

	// check parameters
	if (lpSrc == NULL || nSrcLength == 0)
		return NULL;

	// allocate buffer
	lpMarkupStringBuffer = (char *)HeapAlloc(hHeap, 0, nSrcLength + sizeof(DWORD));
	if (lpMarkupStringBuffer == NULL)
		return NULL;

	// copy
	for (lpDest = lpMarkupStringBuffer; (*lpDest = ch = *(lpSrc++)) != '\0'; lpDest++)
	{
		// check lead byte of multibyte string
		if (!__intrinsic_isleadbyte(ch))
		{
			if (ch != '\\')
			{
				// split by semicolon
				if (ch == ';')
					*lpDest = '\0';
				continue;
			}
			// skip next character of escape
			if ((*(++lpDest) = ch = *(lpSrc++)) == '\0')
				break;
			if (!__intrinsic_isleadbyte(ch))
				continue;
		}
		// skip tail byte
		if ((*(++lpDest) = *(lpSrc++)) == '\0')
			break;
	}
	*(LPDWORD)lpDest = 0;
	nStringLength = lpDest - lpMarkupStringBuffer;

	// mark up the tags and operators
	lpTagArray = AllocMarkup();
	if (lpTagArray == NULL)
		goto FAILED1;
	nNumberOfTag = 0;
	bPrevIsTailByte = FALSE;
	for (char *p = lpMarkupStringBuffer, *end = lpMarkupStringBuffer + nStringLength; p < end; bPrevIsTailByte = bIsLaedByte)
	{
		TAG    iTag;
		size_t nLength;

		#define APPEND_TAG(tag, length, priority, type)                         \
		    if ((lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)) == NULL) \
		        goto FAILED2;                                                   \
		    lpMarkup->Tag      = tag;                                           \
		    lpMarkup->Length   = length;                                        \
		    lpMarkup->String   = p;                                             \
		    lpMarkup->Priority = priority;                                      \
		    lpMarkup->Type     = type;                                          \
		    p += length;                                                        \
		    continue

		bIsLaedByte = FALSE;
		switch (*(LPBYTE)p)
		{
		case (BYTE)'(':
			APPEND_TAG(TAG_PARENTHESIS_OPEN, 1, 100, OS_OPEN);
		case (BYTE)'[':
			switch (*(LPBYTE)(p + 1))
			{
			case (BYTE)'_':
				APPEND_TAG(TAG_ADDR_ADJUST_OPEN, 2, 90, OS_OPEN);
			case (BYTE)'.':
				APPEND_TAG(TAG_ADDR_REPLACE_OPEN, 2, 85, OS_OPEN);
			case (BYTE)'~':
				APPEND_TAG(TAG_REV_ENDIAN_OPEN, 2, 83, OS_OPEN);
			case (BYTE)':':
				APPEND_TAG(TAG_READ_OPEN, 2, 80, OS_OPEN);
			}
			break;
		case (BYTE)'M':
			if (*(LPDWORD)(p + 1) != BSWAP32('Name'))
				break;
			if (*(LPWORD)(p + 5) != BSWAP16('::'))
				break;
			iTag = TAG_MNAME;
			nLength = 7;
			goto APPEND_FUNCTIONAL_OPERATOR;
		case (BYTE)'H':
			if (*(LPDWORD)(p + 1) != BSWAP32('Numb'))
				break;
			if (*(LPDWORD)(p + 5) != BSWAP32('er::'))
				break;
			iTag = TAG_HNUMBER;
			nLength = 9;
		APPEND_FUNCTIONAL_OPERATOR:
			if (p == lpMarkupStringBuffer || (
				!bPrevIsTailByte &&
				__intrinsic_isascii(*(LPBYTE)(p - 1)) &&
				!__intrinsic_isdigit(*(LPBYTE)(p - 1)) &&
				!__intrinsic_isalpha(*(LPBYTE)(p - 1)) &&
				*(LPBYTE)(p - 1) != (BYTE)'$'))
			{
				APPEND_TAG(iTag, nLength, 75, OS_PUSH);
			}
			p += nLength;
			continue;
		case (BYTE)'~':
			switch (*(LPBYTE)(p + 1))
			{
			case (BYTE)']':
				APPEND_TAG(TAG_REV_ENDIAN4, 2, 18, OS_CLOSE | OS_PUSH);
			case (BYTE)'3':
				if (*(LPBYTE)(p + 2) != (BYTE)']')
					break;
				APPEND_TAG(TAG_REV_ENDIAN3, 3, 18, OS_CLOSE | OS_PUSH);
			case (BYTE)'2':
				if (*(LPBYTE)(p + 2) != (BYTE)']')
					break;
				APPEND_TAG(TAG_REV_ENDIAN2, 3, 18, OS_CLOSE | OS_PUSH);
			default:
				APPEND_TAG(TAG_BIT_NOT, 1, 45, OS_PUSH);
			}
			break;
		case (BYTE)'*':
			APPEND_TAG(TAG_MUL, 1, 51, OS_PUSH);
		case (BYTE)'/':
			APPEND_TAG(TAG_DIV, 1, 51, OS_PUSH);
		case (BYTE)'%':
			APPEND_TAG(TAG_MOD, 1, 51, OS_PUSH);
		case (BYTE)'+':
			APPEND_TAG(TAG_ADD, 1, 50, OS_PUSH);
		case (BYTE)'-':
			APPEND_TAG(TAG_SUB, 1, 50, OS_PUSH);
		case (BYTE)'<':
			switch (*(LPBYTE)(p + 1))
			{
			case (BYTE)'=':
				APPEND_TAG(TAG_BE, 2, 40, OS_PUSH);
			default:
				APPEND_TAG(TAG_BT, 1, 40, OS_PUSH);
			}
		case (BYTE)'>':
			if (*(LPBYTE)(p + 1) == (BYTE)'=')
			{
				APPEND_TAG(TAG_AE, 2, 40, OS_PUSH);
			}
			else
			{
				APPEND_TAG(TAG_AT, 1, 40, OS_PUSH);
			}
		case (BYTE)'=':
			switch (*(LPBYTE)(p + 1))
			{
			case (BYTE)'=':
				APPEND_TAG(TAG_EQ, 2, 35, OS_PUSH);
			case (BYTE)'>':
				APPEND_TAG(TAG_ASSIGN, 2, 25, OS_PUSH);
			}
			break;
		case (BYTE)'!':
			if (*(LPBYTE)(p + 1) != (BYTE)'=')
				break;
			APPEND_TAG(TAG_NE, 2, 35, OS_PUSH);
		case (BYTE)'&':
			if (*(LPBYTE)(p + 1) != (BYTE)'&')
			{
				APPEND_TAG(TAG_BIT_AND, 1, 45, OS_PUSH);
			}
			else
			{
				APPEND_TAG(TAG_AND, 2, 30, OS_PUSH);
			}
		case (BYTE)'^':
			APPEND_TAG(TAG_XOR, 1, 45, OS_PUSH);
		case (BYTE)'|':
			if (*(LPBYTE)(p + 1) != (BYTE)'|')
			{
				APPEND_TAG(TAG_BIT_OR, 1, 45, OS_PUSH);
			}
			else
			{
				APPEND_TAG(TAG_OR, 2, 30, OS_PUSH);
			}
		case (BYTE)':':
			switch (*(LPBYTE)(p + 1))
			{
			case (BYTE)']':
				APPEND_TAG(TAG_READ4, 2, 20, OS_CLOSE | OS_PUSH);
			case (BYTE)'3':
				if (*(LPBYTE)(p + 2) != (BYTE)']')
					break;
				APPEND_TAG(TAG_READ3, 3, 20, OS_CLOSE | OS_PUSH);
			case (BYTE)'2':
				if (*(LPBYTE)(p + 2) != (BYTE)']')
					break;
				APPEND_TAG(TAG_READ2, 3, 20, OS_CLOSE | OS_PUSH);
			case (BYTE)'1':
				if (*(LPBYTE)(p + 2) != (BYTE)']')
					break;
				APPEND_TAG(TAG_READ1, 3, 20, OS_CLOSE | OS_PUSH);
			}
			break;
		case (BYTE)'.':
			if (*(LPBYTE)(p + 1) != (BYTE)']')
				break;
			APPEND_TAG(TAG_ADDR_REPLACE, 2, 15, OS_CLOSE | OS_PUSH);
		case (BYTE)'_':
			if (*(LPBYTE)(p + 1) != (BYTE)']')
				break;
			APPEND_TAG(TAG_ADDR_ADJUST, 2, 10, OS_CLOSE | OS_PUSH);
		case (BYTE)')':
			APPEND_TAG(TAG_PARENTHESIS_CLOSE, 1, 0, OS_CLOSE);
		case (BYTE)'\0':
			APPEND_TAG(TAG_SPLIT, 1, 0, 0);
		default:
			if (!(bIsLaedByte = __intrinsic_isleadbyte(*(LPBYTE)p)))
				break;
			p += 2;
			continue;
		}
		p++;

		#undef APPEND_TAG
	}

	// add last value
	if (nNumberOfTag != 0)
	{
		LPSTR lpString;
		LPSTR lpEnd;

		lpString = lpTagArray[nNumberOfTag - 1].String + lpTagArray[nNumberOfTag - 1].Length;
		lpEnd = lpMarkupStringBuffer + nStringLength;
		if (lpEnd > lpString)
		{
			if ((lpMarkup = ReAllocMarkup(&lpTagArray, &nNumberOfTag)) == NULL)
				goto FAILED2;
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpEnd - lpString;
			lpMarkup->String   = lpString;
			lpMarkup->Priority = CHAR_MAX;
			lpMarkup->Type     = OS_PUSH;
			TrimMarkupString(lpMarkup);
			if (lpMarkup->Length == 0)
				nNumberOfTag--;
		}
	}
	else
	{
		lpMarkup = lpTagArray;
		lpMarkup->Tag      = TAG_NOT_OPERATOR;
		lpMarkup->Length   = nStringLength;
		lpMarkup->String   = lpMarkupStringBuffer;
		lpMarkup->Priority = CHAR_MAX;
		lpMarkup->Type     = OS_PUSH;
		TrimMarkupString(lpMarkup);
		if (lpMarkup->Length == 0)
			goto FAILED2;
		nNumberOfTag++;
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (lpMarkupArray == NULL)
		goto FAILED2;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	nMarkupIndex = 0;
	nMarkupLength = 0;
	for (i = 0; i < nNumberOfTag; i++)
	{
		nMarkupIndex += nMarkupLength;
		if ((size_t)(lpTagArray[i].String - lpMarkupStringBuffer) > nMarkupIndex)
		{
			lpMarkup->Tag      = TAG_NOT_OPERATOR;
			lpMarkup->Length   = lpTagArray[i].String - lpMarkupStringBuffer - nMarkupIndex;
			lpMarkup->String   = lpMarkupStringBuffer + nMarkupIndex;
			lpMarkup->Priority = CHAR_MAX;
			lpMarkup->Type     = OS_PUSH;
			nMarkupIndex += lpMarkup->Length;
			TrimMarkupString(lpMarkup);
			if (lpMarkup->Length != 0)
				lpMarkup++;
		}
		*lpMarkup = lpTagArray[i];
		nMarkupLength = lpMarkup->Length;
		nMarkupIndex = lpMarkup->String - lpMarkupStringBuffer;
		lpMarkup++;
	}
	lpEndOfMarkup = lpMarkup;

	// release
	HeapFree(hHeap, 0, lpTagArray);

	// return
	*lppMarkupStringBuffer = lpMarkupStringBuffer;
	*lpnNumberOfMarkup = lpEndOfMarkup - lpMarkupArray;
	return lpMarkupArray;

FAILED2:
	HeapFree(hHeap, 0, lpTagArray);
FAILED1:
	HeapFree(hHeap, 0, lpMarkupStringBuffer);
	return NULL;
}
//---------------------------------------------------------------------
//「中置記法の文字列を、後置記法（逆ポーランド記法）に変換し
//	因子単位で格納したベクタを返す関数」
//---------------------------------------------------------------------
size_t Postfix61(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT MARKUP **lpPostfix, IN MARKUP **lpFactorBuffer, IN size_t *lpnNestBuffer)
{
	MARKUP **lpEndOfPostfix;
	MARKUP *lpFactor, **lpFactorTop, **lpEndOfFactor;
	size_t *lpnNestTop, *lpnEndOfNest;
	MARKUP *lpMarkup, *lpEndOfMarkup;

	lpEndOfPostfix = lpPostfix;
	lpFactorTop = lpEndOfFactor = lpFactorBuffer;
	lpnNestTop = lpnEndOfNest = lpnNestBuffer;

	#define POSTFIX_PUSH(value) (*(lpEndOfPostfix++) = (value))
	#define FACTOR_IS_EMPTY()   (lpEndOfFactor == lpFactorBuffer)
	#define FACTOR_PUSH(value)  (*(lpFactorTop = lpEndOfFactor++) = (value))
	#define FACTOR_POP()        (!FACTOR_IS_EMPTY() ? *(lpEndOfFactor = lpFactorTop--) : NULL)
	#define NEST_IS_EMPTY()     (lpnEndOfNest == lpnNestBuffer)
	#define NEST_PUSH(value)    (*(lpnNestTop = lpnEndOfNest++) = (value))
	#define NEST_POP()          (!NEST_IS_EMPTY() ? *(lpnEndOfNest = lpnNestTop--) : 0)

	NEST_PUSH(0);
	for (lpMarkup = lpMarkupArray, lpEndOfMarkup = lpMarkupArray + nNumberOfMarkup; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		if (lpMarkup->Type & OS_CLOSE)
		{
			while (*lpnNestTop != 0 && (lpFactor = FACTOR_POP()) != NULL)
			{
				POSTFIX_PUSH(lpFactor);
				(*lpnNestTop)--;
			}
			if (lpMarkup->Type & OS_PUSH)
				POSTFIX_PUSH(lpMarkup);
			if (lpnNestTop != lpnNestBuffer)
				NEST_POP();
			else
				*lpnNestTop = 0;
			continue;
		}
		while (!FACTOR_IS_EMPTY() && lpMarkup->Priority <= (*lpFactorTop)->Priority)
		{
			POSTFIX_PUSH(FACTOR_POP());
			(*lpnNestTop)--;
		}
		if (lpMarkup->Type & OS_PUSH)
		{
			// 除去不可フラグが立っているならスタックへ
			FACTOR_PUSH(lpMarkup);
			(*lpnNestTop)++;
		}
		else if (lpMarkup->Type & OS_OPEN)
		{
			// 開始括弧ならスタックプッシュ
			NEST_PUSH(0);
		}
	}
	while ((lpFactor = FACTOR_POP()) != NULL)
	{
		POSTFIX_PUSH(lpFactor);
	}
	return lpEndOfPostfix - lpPostfix;

	#undef POSTFIX_PUSH
	#undef FACTOR_IS_EMPTY
	#undef FACTOR_PUSH
	#undef FACTOR_POP
	#undef NEST_IS_EMPTY
	#undef NEST_PUSH
	#undef NEST_POP
}
#if defined(__BORLANDC__)
unsigned long TSSGCtrl::Parsing(IN TSSGSubject *SSGS, IN const string &Src, IN unsigned long Val)
{
#define SSGCtrl this
#else
EXTERN_C unsigned long __cdecl Parsing61(IN TSSGCtrl *SSGCtrl, IN TSSGSubject *SSGS, IN const string *Src, IN unsigned long Val)
{
#endif
	DWORD          dwResult;
	LPSTR          lpMarkupStringBuffer;
	size_t         nNumberOfMarkup;
	MARKUP         *lpMarkupArray;
	MARKUP         **lpPostfix;
	MARKUP         **lpFactorBuffer;
	size_t         *lpnNestBuffer;
	LPDWORD        lpdwOperandBuffer, lpdwEndOfOperand, lpdwOperandTop;
	DWORD_VARIABLE *lpVariable;
	size_t         nNumberOfVariable;
	HANDLE         hProcess;
	DWORD          value;

	dwResult = 0;
#if defined(__BORLANDC__)
	if (hHeap == NULL && (hHeap = GetProcessHeap()) == NULL)
		goto FAILED1;
#endif
	lpMarkupArray = Markup61(string_c_str(Src), string_length(Src), &lpMarkupStringBuffer, &nNumberOfMarkup);
	if (lpMarkupArray == NULL)
		goto FAILED1;
	lpPostfix = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * (nNumberOfMarkup));
	if (lpPostfix == NULL)
		goto FAILED2;
	lpFactorBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * (nNumberOfMarkup));
	if (lpFactorBuffer == NULL)
		goto FAILED3;
	lpnNestBuffer = (size_t *)HeapAlloc(hHeap, 0, sizeof(size_t) * (nNumberOfMarkup + 1));
	if (lpnNestBuffer == NULL)
		goto FAILED4;
	lpdwOperandBuffer = (LPDWORD)HeapAlloc(hHeap, 0, sizeof(DWORD) * (nNumberOfMarkup + 1));
	if (lpdwOperandBuffer == NULL)
		goto FAILED5;
	lpVariable = (DWORD_VARIABLE *)HeapAlloc(hHeap, 0, sizeof(DWORD_VARIABLE) * 0x10);
	if (lpVariable == NULL)
		goto FAILED6;

	lpdwOperandTop = lpdwEndOfOperand = lpdwOperandBuffer;

	#define OPERAND_IS_EMPTY()  (lpdwEndOfOperand == lpdwOperandBuffer)
	#define OPERAND_PUSH(value) (*(lpdwOperandTop = lpdwEndOfOperand++) = (value))
	#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpdwEndOfOperand = lpdwOperandTop--) : 0)

	hProcess = NULL;
	OPERAND_PUSH(0);
	lpVariable->Length = 3;
	lpVariable->String = "Val";
	lpVariable->Value = Val;
	nNumberOfVariable = 1;

	for (size_t i = 0; i < nNumberOfMarkup; i++)
	{
		size_t j, nNumberOfPostfix;

		for (j = i; i < nNumberOfMarkup; i++)
			if (lpMarkupArray[i].Tag == TAG_SPLIT)
				break;
		if (i == j)
			continue;
		nNumberOfPostfix = Postfix61(lpMarkupArray + j, i - j, lpPostfix, lpFactorBuffer, lpnNestBuffer);
		for (j = 0; j < nNumberOfPostfix; j++)
		{
			MARKUP *lpMarkup;
			size_t nSize;

			lpMarkup = lpPostfix[j];
			switch (lpMarkup->Tag)
			{
			case TAG_ADD:
				value = OPERAND_POP();
				*lpdwOperandTop += value;
				break;
			case TAG_SUB:
				value = OPERAND_POP();
				*lpdwOperandTop -= value;
				break;
			case TAG_MUL:
				value = OPERAND_POP();
				*lpdwOperandTop *= value;
				break;
			case TAG_MOD:
				value = OPERAND_POP();
				if (value == 0)
					goto FAILED8;
				*lpdwOperandTop %= value;
				break;
			case TAG_DIV:
				value = OPERAND_POP();
				if (value == 0)
					goto FAILED8;
				*lpdwOperandTop /= value;
				break;
			case TAG_AND:
				value = OPERAND_POP();
				*lpdwOperandTop = value && *lpdwOperandTop;
				break;
			case TAG_OR:
				value = OPERAND_POP();
				*lpdwOperandTop = value || *lpdwOperandTop;
				break;
			case TAG_EQ:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop == value;
				break;
			case TAG_NE:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop != value;
				break;
			case TAG_ADDR_REPLACE:
				if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, lpdwOperandTop, AT_REPLACE) != 0)
					goto FAILED8;
				break;
			case TAG_ADDR_ADJUST:
				if (TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, lpdwOperandTop, AT_ADJUST) != 0)
					goto FAILED8;
				break;
			case TAG_BIT_AND:
				value = OPERAND_POP();
				*lpdwOperandTop &= value;
				break;
			case TAG_BIT_OR:
				value = OPERAND_POP();
				*lpdwOperandTop |= value;
				break;
			case TAG_XOR:
			case TAG_BIT_NOT:
				value = OPERAND_POP();
				*lpdwOperandTop ^= value;
				break;
			case TAG_BT:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop < value;
				break;
			case TAG_AT:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop > value;
				break;
			case TAG_BE:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop <= value;
				break;
			case TAG_AE:
				value = OPERAND_POP();
				*lpdwOperandTop = *lpdwOperandTop >= value;
				break;
			case TAG_READ4:
				nSize = 4;
				goto READ_PROCESS_MEMORY;
			case TAG_READ3:
				nSize = 3;
				goto READ_PROCESS_MEMORY;
			case TAG_READ2:
				nSize = 2;
				goto READ_PROCESS_MEMORY;
			case TAG_READ1:
				nSize = 1;
			READ_PROCESS_MEMORY:
				value = 0;
				if (*lpdwOperandTop != 0)
				{
					if (hProcess == NULL && (hProcess = TProcessCtrl_Open(SSGCtrl->processCtrl, PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION)) == NULL)
						goto FAILED7;
					if (!ReadProcessMemory(hProcess, (LPVOID)*lpdwOperandTop, &value, nSize, NULL))
						value = 0;
				}
				*lpdwOperandTop = value;
				break;
			case TAG_REV_ENDIAN4:
				*lpdwOperandTop = __intrinsic_bswap32(*lpdwOperandTop);
				break;
			case TAG_REV_ENDIAN3:
				*lpdwOperandTop = __intrinsic_bswap24(*lpdwOperandTop);
				break;
			case TAG_REV_ENDIAN2:
				*lpdwOperandTop = __intrinsic_bswap16(*(LPWORD)lpdwOperandTop);
				break;
			default:
				if (*lpMarkup->String == '$')
				{
					size_t length;

					value = 0;
					length = lpMarkup->Length - 1;
					for (size_t k = 0; k < nNumberOfVariable; k++)
					{
						if (lpVariable[k].Length != length)
							continue;
						if (memcmp(lpVariable[k].String, lpMarkup->String + 1, length) != 0)
							continue;
						value = lpVariable[k].Value;
						break;
					}
					OPERAND_PUSH(value);
					break;
				}
				else if (j + 1 < nNumberOfPostfix)
				{
					MARKUP *lpNext;
					BOOL   bNestedBreak;

					lpNext = lpPostfix[j + 1];
					switch (lpNext->Tag)
					{
					case TAG_MNAME:
						{
							char             ch;
							LPMODULEENTRY32A lpme;

							ch = lpMarkup->String[lpMarkup->Length];
							lpMarkup->String[lpMarkup->Length] = '\0';
							lpme = TProcessCtrl_GetModuleFromName(SSGCtrl->processCtrl, lpMarkup->String);
							lpMarkup->String[lpMarkup->Length] = ch;
							OPERAND_PUSH(lpme != NULL ? (unsigned long)lpme->hModule : 0);
						}
						j++;
						bNestedBreak = TRUE;
						break;
					case TAG_HNUMBER:
						value = 0;
						{
							char          ch;
							unsigned long listNo;
							char          *endptr;

							ch = lpMarkup->String[lpMarkup->Length];
							lpMarkup->String[lpMarkup->Length] = '\0';
							listNo = strtoul(lpMarkup->String, &endptr, 0);
							if (*endptr == '\0')
							{
								THeapListData *HeapL;

								HeapL = TProcessCtrl_GetHeapList(SSGCtrl->processCtrl, listNo - 1);
								if (HeapL != NULL)
									value = HeapL->heapListAddress;
							}
							lpMarkup->String[lpMarkup->Length] = ch;
						}
						OPERAND_PUSH(value);
						j++;
						bNestedBreak = TRUE;
						break;
					case TAG_ASSIGN:
						do	/* do { ... } while (0); */
						{
							size_t k;

							for (k = 0; k < nNumberOfVariable; k++)
								if (lpVariable[k].Length == lpMarkup->Length)
									if (memcmp(lpVariable[k].String, lpMarkup->String, lpMarkup->Length) == 0)
										break;
							if (k < nNumberOfVariable)
							{
								lpVariable[k].Value = *lpdwOperandTop;
								break;
							}
							if ((nNumberOfVariable & 0x0F) == 0)
							{
								LPVOID lpMem;
								size_t nBytes;

								nBytes = (nNumberOfVariable + 0x10) * sizeof(DWORD_VARIABLE);
								lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
								if (lpMem == NULL)
									goto FAILED8;
								lpVariable = (DWORD_VARIABLE *)lpMem;
							}
							lpVariable[nNumberOfVariable].Length = lpMarkup->Length;
							lpVariable[nNumberOfVariable].String = lpMarkup->String;
							lpVariable[nNumberOfVariable].Value = *lpdwOperandTop;
							nNumberOfVariable++;
						} while (0);
						j++;
						bNestedBreak = TRUE;
						break;
					default:
						bNestedBreak = FALSE;
						break;
					}
					if (bNestedBreak)
						break;
				}
				{
					char ch;
					char *endptr;

					ch = lpMarkup->String[lpMarkup->Length];
					lpMarkup->String[lpMarkup->Length] = '\0';
					value = strtoul(lpMarkup->String, &endptr, 0);
					if (*endptr == '\0')
					{
						lpMarkup->String[lpMarkup->Length] = ch;
						OPERAND_PUSH(value);
					}
					else
					{
						if (SSGCtrl->ssgActionListner != NULL)
							TSSGActionListner_OnParsingError(SSGCtrl->ssgActionListner, SSGS, lpMarkup->String);
						goto FAILED8;
					}
				}
				break;
			}
			if (SSGCtrl->ssgActionListner != NULL)
			{
				char ch;

				ch = lpMarkup->String[lpMarkup->Length];
				lpMarkup->String[lpMarkup->Length] = '\0';
				TSSGActionListner_OnParsingProcess61(SSGCtrl->ssgActionListner, SSGS, lpMarkup->String, *lpdwOperandTop);
				lpMarkup->String[lpMarkup->Length] = ch;
			}
		}
	}
	dwResult = *lpdwOperandTop;
FAILED8:
	if (hProcess != NULL)
		CloseHandle(hProcess);
FAILED7:
	HeapFree(hHeap, 0, lpVariable);
FAILED6:
	HeapFree(hHeap, 0, lpdwOperandBuffer);
FAILED5:
	HeapFree(hHeap, 0, lpnNestBuffer);
FAILED4:
	HeapFree(hHeap, 0, lpFactorBuffer);
FAILED3:
	HeapFree(hHeap, 0, lpPostfix);
FAILED2:
	HeapFree(hHeap, 0, lpMarkupArray);
	HeapFree(hHeap, 0, lpMarkupStringBuffer);
FAILED1:
	return dwResult;

	#undef OPERAND_IS_EMPTY
	#undef OPERAND_PUSH
	#undef OPERAND_POP
#if defined(__BORLANDC__)
	#undef SSGCtrl
#endif
}
