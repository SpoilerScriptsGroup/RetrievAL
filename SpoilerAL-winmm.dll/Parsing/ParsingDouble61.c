#include "Parsing61.h"

//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数（double版）」
//	・四則演算や比較演算のみ(剰余演算子[%]は不可)
//---------------------------------------------------------------------
#if defined(__BORLANDC__)
double TSSGCtrl::ParsingDouble(IN TSSGSubject *SSGS, IN const string &Src, IN double Val)
{
	#define SSGCtrl this
#else
EXTERN_C double __cdecl ParsingDouble61(IN TSSGCtrl *SSGCtrl, IN TSSGSubject *SSGS, IN const string *Src, IN double Val)
{
#endif
	double         dblResult;
	LPSTR          lpMarkupStringBuffer;
	size_t         nNumberOfMarkup;
	MARKUP         *lpMarkupArray;
	MARKUP         **lpPostfix;
	MARKUP         **lpFactorBuffer;
	size_t         *lpnNestBuffer;
	double         *lpdblOperandBuffer, *lpdblEndOfOperand, *lpdblOperandTop;
	FLOAT_VARIABLE *lpVariable;
	size_t         nNumberOfVariable;
	HANDLE         hProcess;
	double         value;

	dblResult = 0;
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
	lpdblOperandBuffer = (double *)HeapAlloc(hHeap, 0, sizeof(double) * (nNumberOfMarkup + 1));
	if (lpdblOperandBuffer == NULL)
		goto FAILED5;
	lpVariable = (FLOAT_VARIABLE *)HeapAlloc(hHeap, 0, sizeof(FLOAT_VARIABLE) * 0x10);
	if (lpVariable == NULL)
		goto FAILED6;

	lpdblOperandTop = lpdblEndOfOperand = lpdblOperandBuffer;

	#define OPERAND_IS_EMPTY()  (lpdblEndOfOperand == lpdblOperandBuffer)
	#define OPERAND_PUSH(value) (*(lpdblOperandTop = lpdblEndOfOperand++) = (value))
	#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpdblEndOfOperand = lpdblOperandTop--) : 0)

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

			lpMarkup = lpPostfix[j];
			switch (lpMarkup->Tag)
			{
			case TAG_ADD:
				value = OPERAND_POP();
				*lpdblOperandTop += value;
				break;
			case TAG_SUB:
				value = OPERAND_POP();
				*lpdblOperandTop -= value;
				break;
			case TAG_MUL:
				value = OPERAND_POP();
				if (value == 0)
					goto FAILED7;
				*lpdblOperandTop *= value;
				break;
			case TAG_DIV:
				value = OPERAND_POP();
				*lpdblOperandTop /= value;
				break;
			case TAG_AND:
				value = OPERAND_POP();
				*lpdblOperandTop = value && *lpdblOperandTop;
				break;
			case TAG_OR:
				value = OPERAND_POP();
				*lpdblOperandTop = value || *lpdblOperandTop;
				break;
			case TAG_EQ:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop == value;
				break;
			case TAG_NE:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop != value;
				break;
			case TAG_LT:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop < value;
				break;
			case TAG_GT:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop > value;
				break;
			case TAG_LE:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop <= value;
				break;
			case TAG_GE:
				value = OPERAND_POP();
				*lpdblOperandTop = *lpdblOperandTop >= value;
				break;
			default:
				if (lpMarkup->Length > 1 && *lpMarkup->String == '$')
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

					lpNext = lpPostfix[j + 1];
					if (lpNext->Tag == TAG_ASSIGN)
					{
						size_t k;

						for (k = 0; k < nNumberOfVariable; k++)
							if (lpVariable[k].Length == lpMarkup->Length)
								if (memcmp(lpVariable[k].String, lpMarkup->String, lpMarkup->Length) == 0)
									break;
						if (k < nNumberOfVariable)
						{
							lpVariable[k].Value = *lpdblOperandTop;
						}
						else
						{
							if ((nNumberOfVariable & 0x0F) == 0)
							{
								LPVOID lpMem;
								size_t nBytes;

								nBytes = (nNumberOfVariable + 0x10) * sizeof(FLOAT_VARIABLE);
								lpMem = HeapReAlloc(hHeap, 0, lpVariable, nBytes);
								if (lpMem == NULL)
									goto FAILED7;
								lpVariable = (FLOAT_VARIABLE *)lpMem;
							}
							lpVariable[nNumberOfVariable].Length = lpMarkup->Length;
							lpVariable[nNumberOfVariable].String = lpMarkup->String;
							lpVariable[nNumberOfVariable].Value = *lpdblOperandTop;
							nNumberOfVariable++;
						}
						j++;
						break;
					}
				}
				{
					char ch;
					char *endptr;

					ch = lpMarkup->String[lpMarkup->Length];
					lpMarkup->String[lpMarkup->Length] = '\0';
					value = strtod(lpMarkup->String, &endptr);
					if (*endptr == '\0')
					{
						lpMarkup->String[lpMarkup->Length] = ch;
						OPERAND_PUSH(value);
					}
					else
					{
						if (SSGCtrl->ssgActionListner != NULL)
							TSSGActionListner_OnParsingError(SSGCtrl->ssgActionListner, SSGS, lpMarkup->String);
						goto FAILED7;
					}
				}
				break;
			}
			if (SSGCtrl->ssgActionListner != NULL)
			{
				char ch;

				ch = lpMarkup->String[lpMarkup->Length];
				lpMarkup->String[lpMarkup->Length] = '\0';
				TSSGActionListner_OnParsingDoubleProcess61(SSGCtrl->ssgActionListner, SSGS, lpMarkup->String, *lpdblOperandTop);
				lpMarkup->String[lpMarkup->Length] = ch;
			}
		}
	}
	dblResult = *lpdblOperandTop;
FAILED7:
	if (hProcess != NULL)
		CloseHandle(hProcess);
	HeapFree(hHeap, 0, lpVariable);
FAILED6:
	HeapFree(hHeap, 0, lpdblOperandBuffer);
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
	return dblResult;

	#undef OPERAND_IS_EMPTY
	#undef OPERAND_PUSH
	#undef OPERAND_POP
#if defined(__BORLANDC__)
	#undef SSGCtrl
#endif
}
