#include <windows.h>
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std.h"
#include "bcb6_std_stack.h"
#include "TStringDivision.h"
#include "TSSGCtrl.h"
#include "TSSDir.h"
#include "TSSCalc.h"
#include "TSSToggle.h"
#include "TSSString.h"
#include "SSGSubjectProperty.h"

extern DWORD RepeatDepth;

extern char * __fastcall TrimPointer(const char **pfirst, const char *last);
extern string * __fastcall TrimString(string *s);
extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);
extern void __stdcall repeat_ReadSSRFile(TSSGCtrl *this, LPVOID ParentStack, LPVOID ADJElem, const string *LineS, DWORD RepeatIndex, DWORD ParentRepeat, TSSGSubject *SSGS);
extern void __stdcall Attribute_variable_open(TSSGCtrl *this, LPVOID ParentStack, LPCSTR lpCode, LPCSTR lpEndOfCode);
extern void __stdcall Attribute_variable_close(TSSGCtrl *this);
extern void __stdcall Attribute_expr(TSSGCtrl *this, LPCSTR Code, LPCSTR EndOfCode);
extern void __stdcall Attribute_define(TSSGCtrl *this, LPVOID ParentStack, LPCSTR Line, LPCSTR EndOfLine);
extern void __stdcall Attribute_undef(TSSGCtrl *this, LPCSTR Line);
extern void __stdcall Attribute_allcate(LPCSTR Code, LPCSTR EndOfCode);
extern void __stdcall Attribute_error_skip_open(TSSGCtrl *this);
extern void __stdcall Attribute_error_skip_close(TSSGCtrl *this);
extern void __stdcall Attribute_scope_open(TSSGCtrl *this, string *code);
extern void __stdcall Attribute_scope_close(TSSGCtrl *this);
extern void __stdcall Attribute_offset_open(TSSGCtrl *this, string *code);
extern void __stdcall Attribute_offset_close(TSSGCtrl *this);

void __cdecl TSSGCtrl_EnumReadSSG(TSSGCtrl *this, vector_string *SSGFile, LPVOID ParentStack, TDialogAdjustmentAttribute *ADJElem, DWORD RepeatIndex/* = 0*/, DWORD ParentRepeat/* = MAXDWORD*/)
{
	#define stack_PTSSDir_size(Stack)        stack_dword_size((stack_dword *)(Stack))
	#define stack_PTSSDir_top(Stack)         ((TSSDir *)stack_dword_top((stack_dword *)(Stack)))
	#define stack_PTSSDir_push(Stack, Value) stack_dword_push((stack_dword *)(Stack), (DWORD)(TSSDir *)(Value))
	#define stack_PTSSDir_pop(Stack)         stack_dword_pop((stack_dword *)(Stack))

	for (string *it = vector_begin(SSGFile); it != vector_end(SSGFile); ++it)
	{
		typedef enum {
			OPEN  = 0,
			CLOSE = 1,
		} TAG_TYPE;

		typedef enum {
			SUBJECT,
			INPUT,
			BACK,
			ROOT,
			REPLACE,
			SIZE_OPEN,
			SIZE_CLOSE,
			ADJUSTMENT_OPEN,
			ADJUSTMENT_CLOSE,
			ADJUST_CHECK_OPEN,
			ADJUST_CHECK_CLOSE,
			FUNNEL_OPEN,
			FUNNEL_CLOSE,
			REPEAT,
			IO_FEP_OPEN,
			IO_FEP_CLOSE,
			E_WITH_OPEN,
			E_WITH_CLOSE,
			ENABLED_OPEN,
			ENABLED_CLOSE,
			CHILD_RW_OPEN,
			CHILD_RW_CLOSE,
			CAUTION_OPEN,
			CAUTION_CLOSE,
			INVOLVE,
			NOTE,
			PROCESS,
			TITLE,
			MAKER,
			CREATOR,
			DISTINCTION,
			CALC,
			TOGGLE,
			STRING,
			VAL,
			SCRIPT_CLOSE,
			VARIABLE_OPEN,
			VARIABLE_CLOSE,
			EXPR,
			DEFINE,
			UNDEF,
			ALLOCATE,
			ERROR_SKIP_OPEN,
			ERROR_SKIP_CLOSE,
			SCOPE_OPEN,
			SCOPE_CLOSE,
			OFFSET_OPEN,
			OFFSET_CLOSE,
		} TAG;

		char     *p, c;
		size_t   length;
		TAG_TYPE close;
		TAG      tag;

		p = string_begin(it);
		while ((c = *(p++)) == ' ' || c == '\t');
		if (c != '[')
			continue;
		if (close = (*p == '/'))
			p++;
		if ((length = (string_end(it) - p)) < 4)
			continue;

		switch (*(LPDWORD)p)
		{
		// [subject]
		case BSWAP32('subj'):
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ject'))
				continue;
			p += 7;
			tag = SUBJECT;
			break;
		// [input]
		case BSWAP32('inpu'):
			if (close || length < 6 || p[4] != 't')
				continue;
			p += 5;
			tag = INPUT;
			break;
		// [back]
		case BSWAP32('back'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = BACK;
			break;
		// [root]
		case BSWAP32('root'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = ROOT;
			break;
		// [replace]
		case BSWAP32('repl'):
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('lace'))
				continue;
			p += 7;
			tag = REPLACE;
			break;
		// [size], [/size]
		case BSWAP32('size'):
			if (length < 5)
				continue;
			p += 4;
			tag = SIZE_OPEN + close;
			break;
		// [adjustment], [/adjustment], [adjust_check], [/adjust_check]
		case BSWAP32('adju'):
			if (length < 11)
				continue;
			switch (*(LPDWORD)(p + 4))
			{
			// [adjustment], [/adjustment]
			case BSWAP32('stme'):
				if (*(LPWORD)(p + 8) != BSWAP16('nt'))
					continue;
				p += 10;
				tag = ADJUSTMENT_OPEN + close;
				break;
			// [adjust_check], [/adjust_check]
			case BSWAP32('st_c'):
				if (length < 13 || *(LPDWORD)(p + 8) != BSWAP32('heck'))
					continue;
				p += 12;
				tag = ADJUST_CHECK_OPEN + close;
				break;
			default:
				continue;
			}
			break;
		// [funnel], [/funnel]
		case BSWAP32('funn'):
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('el'))
				continue;
			p += 6;
			tag = FUNNEL_OPEN + close;
			break;
		// [repeat]
		case BSWAP32('repe'):
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('at'))
				continue;
			p += 6;
			tag = REPEAT;
			break;
		// [io_fep], [/io_fep]
		case BSWAP32('io_f'):
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('ep'))
				continue;
			p += 6;
			tag = IO_FEP_OPEN + close;
			break;
		// [e_with], [/e_with]
		case BSWAP32('e_wi'):
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('th'))
				continue;
			p += 6;
			tag = E_WITH_OPEN + close;
			break;
		// [enabled], [/enabled]
		case BSWAP32('enab'):
			if (length < 8 || *(LPDWORD)(p + 3) != BSWAP32('bled'))
				continue;
			p += 7;
			tag = ENABLED_OPEN + close;
			break;
		// [child_rw], [/child_rw]
		case BSWAP32('chil'):
			if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('d_rw'))
				continue;
			p += 8;
			tag = CHILD_RW_OPEN + close;
			break;
		// [caution], [/caution]
		case BSWAP32('caut'):
			if (length < 8 || *(LPDWORD)(p + 3) != BSWAP32('tion'))
				continue;
			p += 7;
			tag = CAUTION_OPEN + close;
			break;
		// [involve]
		case BSWAP32('invo'):
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('olve'))
				continue;
			p += 7;
			tag = INVOLVE;
			break;
		// [note]
		case BSWAP32('note'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = NOTE;
			break;
		// [process]
		case BSWAP32('proc'):
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('cess'))
				continue;
			p += 7;
			tag = PROCESS;
			break;
		// [title]
		case BSWAP32('titl'):
			if (close || length < 6 || p[4] != 'e')
				continue;
			p += 5;
			tag = TITLE;
			break;
		// [maker]
		case BSWAP32('make'):
			if (close || length < 6 || p[4] != 'r')
				continue;
			p += 5;
			tag = MAKER;
			break;
		// [creator]
		case BSWAP32('crea'):
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ator'))
				continue;
			p += 7;
			tag = CREATOR;
			break;
		// [distinction]
		case BSWAP32('dist'):
			if (close || length < 12 || *(LPDWORD)(p + 4) != BSWAP32('inct') || *(LPDWORD)(p + 7) != BSWAP32('tion'))
				continue;
			p += 11;
			tag = DISTINCTION;
			break;
		// [calc]
		case BSWAP32('calc'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = CALC;
			break;
		// [toggle]
		case BSWAP32('togg'):
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('le'))
				continue;
			p += 6;
			tag = TOGGLE;
			break;
		// [string]
		case BSWAP32('stri'):
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ng'))
				continue;
			p += 6;
			tag = STRING;
			break;
		// [val]
		case BSWAP32('val]'):
			if (close)
				continue;
			p += 3;
			tag = VAL;
			break;
		case BSWAP32('val '):
		case BSWAP32('val\t'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = VAL;
			break;
		// [/script]
		case BSWAP32('scri'):
			if (!close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('pt'))
				continue;
			p += 6;
			tag = SCRIPT_CLOSE;
			break;
		// [variable], [/variable]
		case BSWAP32('vari'):
			if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('able'))
				continue;
			p += 8;
			tag = VARIABLE_OPEN + close;
			break;
		// [expr]
		case BSWAP32('expr'):
			if (close || length < 5)
				continue;
			p += 4;
			tag = EXPR;
			break;
		// [define ...]
		case BSWAP32('defi'):
			if (close || length < 9 || *(LPWORD)(p + 4) != BSWAP16('ne') || ((c = p[6]) != ' ' && c != '\t'))
				continue;
			p += 7;
			tag = DEFINE;
			break;
		// [undef ...]
		case BSWAP32('unde'):
			if (close || length < 8 || p[4] != 'f' || ((c = p[5]) != ' ' && c != '\t'))
				continue;
			p += 6;
			tag = UNDEF;
			break;
		// [allocate]
		case BSWAP32('allo'):
			if (close || length < 9 || *(LPDWORD)(p + 4) != BSWAP32('cate'))
				continue;
			p += 8;
			tag = ALLOCATE;
			break;
		// [error_skip], [/error_skip]
		case BSWAP32('erro'):
			if (length < 11 || *(LPDWORD)(p + 4) != BSWAP32('r_sk') || *(LPWORD)(p + 8) != BSWAP16('ip'))
				continue;
			p += 10;
			tag = ERROR_SKIP_OPEN + close;
			break;
		// [scope], [/scope]
		case BSWAP32('scop'):
			if (length < 6 || p[4] != 'e')
				continue;
			p += 5;
			tag = SCOPE_OPEN + close;
			break;
		// [offset], [/offset]
		case BSWAP32('offs'):
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('et'))
				continue;
			p += 6;
			tag = OFFSET_OPEN + close;
			break;
		default:
			continue;
		}

		while ((c = *(p++)) == ' ' || c == '\t');
		if (!c || ((c != ']') ^ (tag == DEFINE || tag == UNDEF)))
			continue;

		switch (tag)
		{
		// [subject], [input]
		case SUBJECT:
		case INPUT:
			{
				string              Name, Code, Tag;
				TSSGSubject         *SSGS; // 項目名、コード部分、項目種別の取得
				TSSGSubjectProperty *prop;

				string_ctor_assign_cstr_with_length(&Code, p, string_end(it) - p);
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &Code, ":", 1, 0, dtESCAPE);
				TStringDivision_Half_WithoutTokenDtor(&Tag, &this->strD, &Code, ",", 1, 0, dtESCAPE);
				if ((p = string_begin(&Tag))[0] == ',' && p[1] == '\0')
				{
					std_swap(&Tag, &Code);
					string_clear(&Code);
				}

				// この時点でクラス分け
				SSGS = TSSGCtrl_MakeSubjectClass(TrimString(&Tag));
				string_dtor(&Tag);
				if (SSGS)
				{
					if (tag == INPUT)
					{
						string  tmpS;
						BOOLEAN CanUnknown;

						TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &Code, ",", 1, 0, etTRIM);
						CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
						TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
						string_dtor(&tmpS);
						// アドレスは存在しない(その位置に必須入力判定がある)
						string_storage_append(&Code, 2);
						memmove(string_begin(&Code) + 2, string_begin(&Code), string_length(&Code) + 1);
						*(LPWORD)string_begin(&Code) = BSWAP16('0,');
						string_end(&Code) += 2;
					}
					TSSGSubject_SetCode_stdstr(SSGS, &Code);
					TSSGSubject_SetName_stdstr(SSGS, &Name);
				}
				string_dtor(&Code);
				string_dtor(&Name);
				if (!SSGS)
					// 未サポートのタグ
					break;

				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				SSGS->attribute = this->attributeSelector.nowAttributeVec;
				if (SSGS->type == stDIR)
				{
					TDirAttribute *NewAElem;

					// dir
					NewAElem = new_TDirAttribute();
					NewAElem->level = TSSGCtrl_GetDirLevel(this, SSGS) + 1;
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);

					stack_PTSSDir_push(ParentStack, (TSSDir *)SSGS);	// 親の切り替え
				}

				TSSGSubject_Setting(SSGS, this);

				prop = GetSubjectProperty(SSGS);
				if (prop)
				{
					prop->RepeatDepth  = RepeatDepth;
					prop->RepeatIndex  = RepeatIndex;
					prop->ParentRepeat = ParentRepeat;
				}
			}
			break;

		// [back]
		case BACK:
			if (stack_PTSSDir_size(ParentStack) > 1)
			{
				TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atDIR_LEVEL);
				stack_PTSSDir_pop(ParentStack);
			}
			break;

		// [root]
		case ROOT:
			{
				unsigned long StackSize;

				StackSize = stack_PTSSDir_size(ParentStack);
				if (StackSize > 1)
				{
					unsigned long i;

					TSSGAttributeSelector_ToRootElementByType(&this->attributeSelector, atDIR_LEVEL);
					i = StackSize - 1;
					do
						stack_PTSSDir_pop(ParentStack);
					while (--i);
				}
			}
			break;

		// [replace]
		case REPLACE:
			{
				string            LineS;
				TReplaceAttribute *NewAElem;
				vector_string     *tmpL;
				string            FName, DefaultExt;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TReplaceAttribute();
				TReplaceAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);

				string_ctor_assign(&FName, &NewAElem->fileName);
				string_ctor_assign_cstr_with_length(&DefaultExt, ".SSC", 4);
				tmpL = TSSGCtrl_GetSSGDataFile(this, NULL, FName, DefaultExt, NULL);
				if (tmpL == NULL)
				{
					delete_TReplaceAttribute(NewAElem);
					break;
				}
				// [replace]属性を追加
				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);

				// 再帰
				TSSGCtrl_EnumReadSSG(this, tmpL, ParentStack, ADJElem, RepeatIndex, ParentRepeat);
				// 帰ってきたので、[replace]属性を外す
				TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
			}
			break;

		// [size]
		case SIZE_OPEN:
			{
				// アクセスサイズ強制指定命令
				string LineS;
				long   Size;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				Size = TStringDivision_ToLongDef(&LineS, -1);
				string_dtor(&LineS);
				if (Size >= 0)
				{
					TSizeAttribute *NewAElem;

					// [size]属性を付加
					NewAElem = new_TSizeAttribute(); // size
					TSizeAttribute_Setting(NewAElem, Size);
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
			}
			break;

		// [/size]
		case SIZE_CLOSE:
			// [size]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atSIZE);
			break;

		// [adjustment]
		case ADJUSTMENT_OPEN:
			{
				string               LineS, tmpS;
				TAdjustmentAttribute *NewAElem;

				// [adjustment]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, 0, 0);
				NewAElem = TSSGCtrl_MakeAdjustmentClass(&tmpS);
				string_dtor(&tmpS);
				if (NewAElem != NULL)
				{
					TAdjustmentAttribute_Setting(NewAElem, this, string_c_str(&LineS));
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
				string_dtor(&LineS);
			}
			break;

		// [/adjustment]
		case ADJUSTMENT_CLOSE:
			// [adjustment]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST);
			break;

		// [adjust_check]
		case ADJUST_CHECK_OPEN:
			{
				string                LineS;
				TAdjustCheckAttribute *NewAElem;

				// [adjust_check]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TAdjustCheckAttribute();
				TAdjustCheckAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/adjust_check]
		case ADJUST_CHECK_CLOSE:
			// [adjust_check]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST_CHECK);
			break;

		// [funnel]
		case FUNNEL_OPEN:
			{
				string           LineS;
				TFunnelAttribute *NewAElem;

				// [funnel]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TFunnelAttribute();
				TFunnelAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/funnel]
		case FUNNEL_CLOSE:
			// [funnel]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atFUNNEL);
			break;

		// [repeat]
		case REPEAT:
			{
				static TSSGSubject SSGS = { TSSGSubject_VTable, 0 };
				string LineS;

				// 繰り返し書式発動！
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				repeat_ReadSSRFile(this, ParentStack, ADJElem, &LineS, RepeatIndex, ParentRepeat, &SSGS);
				string_dtor(&LineS);
			}
			break;

		// [io_fep]
		case IO_FEP_OPEN:
			{
				string           LineS;
				TIO_FEPAttribute *NewAElem;

				// [io_fep]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TIO_FEPAttribute(); // io_fep
				TIO_FEPAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/io_fep]
		case IO_FEP_CLOSE:
			// [io_fep]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atIO_FEP);
			break;

		// [e_with]
		case E_WITH_OPEN:
			{
				string            LineS;
				TEndWithAttribute *NewAElem;

				// [e_with]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEndWithAttribute(); // e_with
				TEndWithAttribute_Setting_stdstr(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/e_with]
		case E_WITH_CLOSE:
			// [e_with]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atE_WITH);
			break;

		// [enabled]
		case ENABLED_OPEN:
			{
				string            LineS;
				TEnabledAttribute *NewAElem;

				// [enabled]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEnabledAttribute(); // enabled
				TEnabledAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/enabled]
		case ENABLED_CLOSE:
			// [enabled]属性を除去
			TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atENABLED);
			break;

		// [child_rw]
		case CHILD_RW_OPEN:
			{
				string            LineS;
				TChildRWAttribute *NewAElem;

				// [child_rw]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TChildRWAttribute(); // child_rw
				TChildRWAttribute_Setting(NewAElem, NULL, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/child_rw]
		case CHILD_RW_CLOSE:
			// [child_rw]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCHILD_RW);
			break;

		// [caution]
		case CAUTION_OPEN:
			{
				string            LineS;
				TCautionAttribute *NewAElem;

				// [caution]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TCautionAttribute(); // caution
				TCautionAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/caution]
		case CAUTION_CLOSE:
			// [caution]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCAUTION);
			break;

		// [involve]
		case INVOLVE:
			{
				string        FName;
				const char    *end;
				vector_string tmpV;

				// 埋め込みファイル
				string_ctor_assign(&FName, &this->scriptDir);
				end = TrimPointer(&p, string_end(it));
				string_append_cstr_with_length(&FName, p, end - p);
				ReplaceDefine(&this->attributeSelector, &FName);
				string_append_cstr_with_length(&FName, ".ssl", 4);
				vector_ctor(&tmpV);
				vector_string_reserve(&tmpV, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 10 &&
						*(LPDWORD) p      == BSWAP32('[/in') &&
						*(LPDWORD)(p + 4) == BSWAP32('volv') &&
						*         (p + 8) ==         'e'    )
					{
						p += 9;
						do
							if ((c = *(p++)) == ']')
								goto INVOLVE_NESTED_BREAK;
						while (c == ' ' || c == '\t');
					}
					vector_string_push_back(&tmpV, it);
				}
			INVOLVE_NESTED_BREAK:
				vector_shrink_to_fit(&tmpV);
				TSSGCtrl_SetSSGDataFile(this, &tmpV, &FName, TRUE);
				vector_string_dtor(&tmpV);
				string_dtor(&FName);
				if (it == vector_end(SSGFile))
					return;  // [/involve]が存在しない
			}
			break;

		// [note]
		case NOTE:
			{
				const char    *end;
				string        LineS;
				BOOLEAN       IsMemoWordWrap;
				vector_string *memo;

				end = TrimPointer(&p, string_end(it));
				string_ctor_assign_cstr_with_length(&LineS, p, end - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				IsMemoWordWrap = (string_length(&LineS) == 8 && *(LPDWORD)(p = string_begin(&LineS)) == BSWAP32('word') && *(LPDWORD)(p + 4) == BSWAP32('wrap'));
				string_dtor(&LineS);
				if (!ADJElem)
				{
					// SSGモード
					this->isMemoWordWrap = IsMemoWordWrap;
					memo = &this->memo;
				}
				else
				{
					// ADJモード
					ADJElem->isMemoWordWrap = IsMemoWordWrap;
					memo = &ADJElem->memo;
				}
				vector_string_dtor(memo);
				vector_ctor(memo);
				vector_string_reserve(memo, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 7 &&
						*(LPDWORD) p      == BSWAP32('[/no') &&
						*(LPWORD )(p + 4) == BSWAP16('te'  ))
					{
						p += 6;
						do
							if ((c = *(p++)) == ']')
								// [/note]
								goto NOTE_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					vector_string_push_back(memo, it);
				}
			NOTE_NESTED_BREAK:
				vector_shrink_to_fit(memo);
				if (it == vector_end(SSGFile))
					return;  // [/note]が存在しない
			}
			break;

		// [process]
		case PROCESS:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				vector_string_push_back(&this->processNameVec, &LineS);
				string_dtor(&LineS);
			}
			break;

		// [title]
		case TITLE:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "title", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [maker]
		case MAKER:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "maker", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [creator]
		case CREATOR:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "creator", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [distinction]
		case DISTINCTION:
			{
				string LineS, tmpS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, "=", 1, 0, dtESCAPE | etREPLACE | etTRIM);
				if (*(p = string_begin(&tmpS)) != '=' || *(p + 1) != '\0')
					TSSGScriptStruct_SetDistinction_stdstr(&this->script, string_c_str(&tmpS), &LineS);
				string_dtor(&tmpS);
				string_dtor(&LineS);
			}
			break;

		// [calc], [toggle], [string]
		case CALC:
		case TOGGLE:
		case STRING:
			{
				string              LineS, tmpS, Name;
				TSSGSubject         *SSGS;
				BOOLEAN             CanUnknown;
				TSSGSubjectProperty *prop;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, tag != TOGGLE ? 1 : 3, 0);
				if (tag == CALC)
					SSGS = &new_TSSCalc()->super;
				else if (tag == TOGGLE)
					SSGS = &new_TSSToggle()->super;
				else/* if (tag == STRING)*/
					SSGS = &new_TSSString()->super;
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &tmpS, ",", 1, 0, etTRIM);
				CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
				TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
				string_dtor(&tmpS);
				TSSGSubject_SetName_stdstr(SSGS, &Name);
				string_dtor(&Name);
				string_storage_append(&LineS, 2);
				memmove(string_begin(&LineS) + 2, string_begin(&LineS), string_length(&LineS) + 1);
				*(LPWORD)string_begin(&LineS) = BSWAP16('0,');
				string_end(&LineS) += 2;
				TSSGSubject_SetCode_stdstr(SSGS, &LineS);
				string_dtor(&LineS);
				TSSGSubject_Setting(SSGS, this);
				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				SSGS->attribute = this->attributeSelector.nowAttributeVec;
				prop = GetSubjectProperty(SSGS);
				if (prop)
				{
					prop->RepeatDepth  = RepeatDepth;
					prop->RepeatIndex  = RepeatIndex;
					prop->ParentRepeat = ParentRepeat;
				}
			}
			break;

		// [val]
		case VAL:
			if (ADJElem)
			{
				string_assign_cstr_with_length(&ADJElem->valStr, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &ADJElem->valStr);
				string_shrink_to_fit(&ADJElem->valStr);
			}
			break;

		// [/script]
		case SCRIPT_CLOSE:
			return;

		// [variable]
		case VARIABLE_OPEN:
			Attribute_variable_open(this, ParentStack, p, string_end(it));
			break;

		// [/variable]
		case VARIABLE_CLOSE:
			// [variable]属性を除去
			Attribute_variable_close(this);
			break;

		// [expr]
		case EXPR:
			Attribute_expr(this, p, string_end(it));
			break;

		// [define ...]
		case DEFINE:
			Attribute_define(this, ParentStack, --p, string_end(it));
			break;

		// [undef ...]
		case UNDEF:
			Attribute_undef(this, --p);
			break;

		// [allocate]
		case ALLOCATE:
			Attribute_allcate(p, string_end(it));
			break;

		// [error_skip]
		case ERROR_SKIP_OPEN:
			Attribute_error_skip_open(this);
			break;

		// [/error_skip]
		case ERROR_SKIP_CLOSE:
			// [error_skip]属性を除去
			Attribute_error_skip_close(this);
			break;

		// [scope]
		case SCOPE_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_scope_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/scope]
		case SCOPE_CLOSE:
			Attribute_scope_close(this);
			break;

		// [offset]
		case OFFSET_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_offset_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/offset]
		case OFFSET_CLOSE:
			Attribute_offset_close(this);
			break;
		}
	}
}
