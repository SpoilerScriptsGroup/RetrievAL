#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <mbstring.h>
#include "intrinsic.h"
#include "rtl.h"
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "TSSCalc.h"
#include "TSSString.h"
#include "TSSFloatCalc.h"

string * __fastcall SubjectStringTable_GetString(string *s);

TSSArg* (__cdecl * const TSSGSubject_MakeArg)(TSSGSubject*) = (LPVOID)0x0052CFB4;

void __fastcall TMainForm_DrawTreeCell_DrawStr(
	TMainForm   *const main,
	string      *const DrawStr,// it's constructed
	string      *const Subject,// must be destruct
	TSSGSubject *const SSGS,
	TSSGCtrl    *const ssgCtrl)
{
	extern BOOL EnableParserFix;

	string Current;
	TSSArg *Arg = NULL;
	const TFormatAttribute *format = NULL;
	const vector *atV = TSSGSubject_GetAttribute(SSGS);
	const ULONG aType = TSSGSubject_GetArgType(SSGS);
	const COORD index = TSSGAttributeElement_GetViaCoord(atFORMAT, atV).dwFontSize;

	if (index.X)
		for (TFormatAttribute **base = &vector_type_at(atV, TFormatAttribute *, index.Y),
			 **cur  = base + index.X;
			 --cur >= base; )
			if ((*cur)->offsetNum & 1 << aType)
			{
				format = *cur;
				break;
			}

	string_ctor_null(&Current);

	if (main->isNowValueDraw &&
		(!format ||
		 string_length(&format->offsetCode) != 1 ||
		 string_at(&format->offsetCode, 0u) != '_') &&
		(Arg = TSSGSubject_MakeArg(SSGS)) &&
		!TSSGSubject_Read(SSGS, ssgCtrl, Arg))
	{
		LPCSTR spec = format ? string_c_str(&format->offsetCode) : "";
		switch (Arg->type)
		{
		case atLONG:
			{
				int len;
				TSSCalc *SS = (TSSCalc *)SSGS;
				string  *VH = SubjectStringTable_GetString(&SS->nowValHeadStr);

				if (EnableParserFix && string_at(VH, 0) == '_' && string_length(VH) > 1)
					spec = string_c_str(VH) + 1;
				else if (!*spec)
					spec = SS->isUnsigned ? "[%u]" : "[%d]";

				string_reserve(&Current, SCHAR_MAX);
				len = _snprintf(string_begin(&Current), string_storage_capacity(&Current), spec, ((TSSArgLong *)Arg)->value);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(string_capacity(&Current), (size_t)len);
			}
			break;
		case atLONG_INDEX:
			{
				string FName, DefaultExt;
				vector_string *ListFile;
				BOOL prefix;
				unsigned long Val = ((TSSArgLongIndex *)Arg)->value;

				TSSArgLongIndexSubject_GetIndexFileName(&FName, SSGS);
				if (prefix = string_at(&FName, 0) == '+')
				{
					__movsb(string_begin(&FName), string_begin(&FName) + 1, string_size(&FName));
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
						int cap, len = tail - head + strlen(spec);
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

				string_reserve(&Current, SCHAR_MAX);
				len = _snprintf(string_begin(&Current), string_storage_capacity(&Current), *spec ? spec : "[%s]", check[((TSSArgBool *)Arg)->value]);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(string_capacity(&Current), (size_t)len);
			}
			break;
		case atSTRING:
			{
				string *val = &((TSSArgString *)Arg)->value;
				*(string_end(val) = string_begin(val) + strcspn(string_begin(val), "\r\f\v\n\b\a")) = '\0';
				if (*spec)
				{
					int cap, len = string_length(val) + strlen(spec);
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

				if (bvector_size(((TSSArgBoolVector *)Arg)->value) <= 32)
					TSSArg_ToString(&bits, Arg);
				else
					string_ctor_assign_cstr_with_length(&bits, "..", 2);

				string_reserve(&Current, SCHAR_MAX);
				len = _snprintf(string_begin(&Current), string_storage_capacity(&Current), *spec ? spec : "[%s]", string_c_str(&bits));
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(string_capacity(&Current), (size_t)len);

				string_dtor(&bits);
			}
			break;
		case atDOUBLE:
			{
				int len;
				string *VH = SubjectStringTable_GetString(&((TSSFloatCalc *)SSGS)->nowValHeadStr);

				if (EnableParserFix && string_at(VH, 0) == '_' && string_length(VH) > 1)
					spec = string_c_str(VH) + 1;
				else if (!*spec)
					spec = "[%f]";

				string_reserve(&Current, SCHAR_MAX);
				len = _snprintf(string_begin(&Current), string_storage_capacity(&Current), spec, ((TSSArgDouble *)Arg)->value);
				if (len >= 0) string_end(&Current) = string_begin(&Current) + min(string_capacity(&Current), (size_t)len);
			}
			break;
		default:
			if (*spec)
				string_assign(&Current, &format->offsetCode);
			else
				string_assign_cstr_with_length(&Current, "[?]", 3);
		}
	}
	if (Arg) delete_TSSArg(Arg);

	if (!format || string_empty(&format->fileName))
	{
		string_reserve(DrawStr, string_length(Subject) + string_length(&Current));
		string_assign (DrawStr, Subject);
	}
	else
	{
		int cap, len = string_length(&format->fileName) + string_length(Subject) + string_length(&Current);
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
		string_swap  (DrawStr, &Current);
		string_append(DrawStr, &Current);
	}
	string_dtor(&Current);
	string_dtor(Subject);
}

static void __fastcall ModifyNowValueBoolVector(string *DrawStr, TSSArgBoolVector *Arg)
{
	string str;
	size_t len, cap;
	LPSTR  end;

	if (bvector_size(Arg->value) > 32)
		string_ctor_assign_cstr_with_length(&str, "..", 2);
	else
		TSSArg_ToString(&str, Arg);
	len = string_length(&str);
	cap = string_length(DrawStr) + len + 2;
	if (cap >= string_storage_capacity(DrawStr))
		string_reserve(DrawStr, cap);
	end = string_end(DrawStr);
	*end++ = '[';
	__movsb(end, string_c_str(&str), len);
	*(end += len) = ']';
	*(string_end(DrawStr) = end + 1) = '\0';
	string_dtor(&str);
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC    edi
		#define Format (esp + 12)

		lea     ecx, [SSC + TSSCalc.nowValHeadStr]
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
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc()
{
	__asm
	{
		#define SSFC   edi
		#define Format (esp + 16)

		lea     ecx, [SSFC + TSSFloatCalc.nowValHeadStr]
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
