#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <mbstring.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "TSSCalc.h"
#include "TSSString.h"
#include "TSSFloatCalc.h"

string * __fastcall SubjectStringTable_GetString(string *s);

TSSArg* (__cdecl * const TSSGSubject_MakeArg)(TSSGSubject*) = (LPVOID)0x0052CFB4;

void __fastcall TMainForm_DrawTreeCell_DrawStr(
	TMainForm   * const main,
	string      * const DrawStr,// it's constructed
	string      * const Subject,// must be destruct
	TSSGSubject * const SSGS,
	TSSGCtrl    * const ssgCtrl)
{
	extern BOOL EnableParserFix;
	unsigned long aType;
	const vector* attrs;
	TSSArg* Arg;
	const TFormatAttribute* format = NULL;
	string Current = { NULL };

	aType = TSSGSubject_GetArgType  (SSGS);
	attrs = TSSGSubject_GetAttribute(SSGS);
	for (const TFormatAttribute** pos = vector_end(attrs);
		 --pos >= (TFormatAttribute**)vector_begin(attrs) &&
		 (*pos)->type >= atFORMAT; )
	{
		if ((*pos)->type == atFORMAT && (*pos)->offsetNum & 1 << aType)
		{
			format = *pos;
			break;
		}
	}

	if (main->isNowValueDraw &&
		(!format ||
		 string_length(&format->offsetCode) != 1 ||
		 string_at(&format->offsetCode,  0) != '_') &&
		(Arg = TSSGSubject_MakeArg(SSGS)) &&
		!TSSGSubject_Read(SSGS, ssgCtrl, Arg))
	{
		LPCSTR spec = format ? string_c_str(&format->offsetCode) : "";
		string_ctor(&Current);
		switch (Arg->type)
		{
		case atLONG:
			{
				int len;
				TSSCalc *SS = (TSSCalc*)SSGS;
				string  *VH = SubjectStringTable_GetString(&SS->nowValHeadStr);

				if (EnableParserFix && string_at(VH, 0) == '_' && string_length(VH) > 1)
					spec = string_c_str(VH) + 1;
				else if (!*spec)
					spec = SS->isUnsigned ? "[%u]" : "[%d]";

				string_reserve(&Current, 0xFF);
				len = _snprintf(string_begin(&Current), 0x100, spec, ((TSSArgLong*)Arg)->value);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(0xFF, len);
			}
			break;
		case atLONG_INDEX:
			{
				string FName, DefaultExt;
				vector_string* ListFile;
				char prefix;
				unsigned long Val = ((TSSArgLongIndex*)Arg)->value;

				TSSArgLongIndexSubject_GetIndexFileName(&FName, SSGS);
				if (prefix = string_at(&FName, 0) == '+')
				{
					__movsb(string_begin(&FName), string_begin(&FName) + 1, string_length(&FName));
					--string_end(&FName);
				}

				string_ctor_assign_cstr_with_length(&DefaultExt, ".LST", 4);
				if ((ListFile = TSSGCtrl_GetSSGDataFile(ssgCtrl, SSGS, FName, DefaultExt, NULL)) &&
					vector_size(ListFile) > Val)
				{
					string *line = &vector_at(ListFile, Val);
					LPCSTR pos, head = string_begin(line), tail = string_end(line);

					if (prefix && (pos = _mbschr(head, '=')))
						head = pos + 1;
					if (*spec)
					{
						int cap, len = tail - head + 2;
						do
						{
							string_reserve(&Current, cap = len);
							len = _snprintf(string_begin(&Current), cap + 1, spec, head);
						}
						while (len > cap);
						string_end(&Current) = string_begin(&Current) + len;
					}
					else
					{
						string_reserve(&Current, tail - head + 2);
						string_append_cstr_with_length(&Current, "[", 1);
						string_append_range(&Current, head, tail);
						string_append_cstr_with_length(&Current, "]", 1);
					}
				}
			}
			break;
		case atBOOL:
			{
				static LPCSTR const check[] = { "off", "on" };
				int len;

				string_reserve(&Current, 0xFF);
				len = _snprintf(string_begin(&Current), 0x100, *spec ? spec : "[%s]", check[((TSSArgBool*)Arg)->value]);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(0xFF, len);
			}
			break;
		case atSTRING:
			{
				string* val = &((TSSArgString*)Arg)->value;
				if (((TSSString*)SSGS)->caution)
					string_assign_cstr_with_length(val, "..", 2);
				if (*spec)
				{
					int cap, len = string_length(val) + 2;
					do
					{
						string_reserve(&Current, cap = len);
						len = _snprintf(string_begin(&Current), cap + 1, spec, string_c_str(val));
					}
					while (len > cap);
					string_end(&Current) = string_begin(&Current) + len;
				}
				else
				{
					string_reserve(&Current, string_length(val) + 2);
					string_append_cstr_with_length(&Current, "[", 1);
					string_append(&Current, val);
					string_append_cstr_with_length(&Current, "]", 1);
				}
			}
			break;
		case atBOOL_VECTOR:
			{
				string bits;
				int    len;
				
				if (vector_bool_size(((TSSArgBoolVector*)Arg)->value) <= 32)
					TSSArg_ToString(&bits, Arg);
				else
					string_ctor_assign_cstr_with_length(&bits, "..", 2);

				string_reserve(&Current, 0xFF);
				len = _snprintf(string_begin(&Current), 0x100, *spec ? spec : "[%s]", string_c_str(&bits));
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(0xFF, len);

				string_dtor(&bits);
			}
			break;
		case atDOUBLE:
			{
				int len;
				string *VH = SubjectStringTable_GetString(&((TSSFloatCalc*)SSGS)->nowValHeadStr);

				if (EnableParserFix && string_at(VH, 0) == '_' && string_length(VH) > 1)
					spec = string_c_str(VH) + 1;
				else if (!*spec)
					spec = "[%f]";

				string_reserve(&Current, 0xFF);
				len = _snprintf(string_begin(&Current), 0x100, spec, ((TSSArgDouble*)Arg)->value);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(0xFF, len);
			}
			break;
		default:
			if (*spec)
				string_assign(&Current, &format->offsetCode);
			else
				string_assign_cstr_with_length(&Current, "[..]", 4);
		}
		delete_TSSArg(Arg);
	}

	string_reserve(DrawStr, string_length(Subject) + string_length(&Current));
	if (!format || string_empty(&format->fileName))
		string_assign(DrawStr, Subject);
	else
	{
		int cap, len = string_capacity(DrawStr);
		do
		{
			string_reserve(DrawStr, cap = len);
			len = _snprintf(
				string_begin(DrawStr),
				cap + 1,
				string_c_str(&format->fileName),
				string_c_str(Subject));
		}
		while (len > cap);
		string_end(DrawStr) = string_begin(DrawStr) + len;
	}
	if (!format || (long)format->offsetNum >= 0)
		string_append(DrawStr, &Current);
	else
	{
		string_swap(DrawStr, &Current);
		string_append(DrawStr, &Current);
	}
	string_dtor(&Current);
	string_dtor(Subject);
}

static void __fastcall ModifyNowValueBoolVector(string *DrawStr, TSSArg *Arg);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC                            edi
		#define offsetof_TSSCalc_nowValHeadStr 160
		#define Format                         (esp + 12)

		lea     ecx, [SSC + offsetof_TSSCalc_nowValHeadStr]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax]
		mov     dl, byte ptr [eax]
		inc     eax
		cmp     eax, ecx
		jae     L1
		cmp     dl, '_'
		jne     L1
		mov     dword ptr [Format], eax
	L1:
		jmp     TStringDivision_ToString

		#undef SSC
		#undef offsetof_TSSCalc_nowValHeadStr
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc()
{
	__asm
	{
		#define SSFC                                edi
		#define offsetof_TSSFloatCalc_nowValHeadStr 176
		#define Format                              (esp + 16)

		lea     ecx, [SSFC + offsetof_TSSFloatCalc_nowValHeadStr]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax]
		mov     dl, byte ptr [eax]
		inc     eax
		cmp     eax, ecx
		jae     L1
		cmp     dl, '_'
		jne     L1
		mov     dword ptr [Format], eax
	L1:
		jmp     TStringDivision_ToStringDouble

		#undef SSFC
		#undef offsetof_TSSFloatCalc_nowValHeadStr
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueBoolVector(string* DrawStr, const char* first, const char *last, void *reserved)
{
	__asm
	{
		#define ReturnAddress 00445B80H
		#define DrawStr       (ebp - 18H)
		#define Arg           (ebp - 254H)

		mov     edx, dword ptr [Arg]
		lea     ecx, [DrawStr]
		push    ReturnAddress
		jmp     ModifyNowValueBoolVector

		#undef ReturnAddress
		#undef DrawStr
		#undef Arg
	}
}

static void __fastcall ModifyNowValueBoolVector(string *DrawStr, TSSArg *Arg)
{
	string s;
	size_t insertLength, requireLength;
	LPSTR  p, dest;

	if (vector_bool_size(((TSSArgBoolVector*)Arg)->value) > 32)
		string_ctor_assign_cstr_with_length(&s, "..", 2);
	else
		TSSArg_ToString(&s, Arg);
	insertLength = string_length(&s);
	requireLength = string_length(DrawStr) + insertLength + 2;
	if (requireLength >= (size_t)(string_end_of_storage(DrawStr) - string_begin(DrawStr)))
		string_reserve(DrawStr, requireLength);
	p = string_end(DrawStr);
	*p = '[';
	dest = ++p;
	*(LPWORD)(p += insertLength) = BSWAP16(']\0');
	string_end(DrawStr) = p + 1;
	__movsb(dest, string_c_str(&s), insertLength);
	string_dtor(&s);
}
