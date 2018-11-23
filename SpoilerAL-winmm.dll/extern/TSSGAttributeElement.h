#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TSSArg.h"
#include "TStringDivision.h"

typedef LPVOID(__cdecl * const LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL)(LPCVOID elem1, LPCVOID elem2);
#define TSSGAttributeElement_IsEqual(elem1, elem2) (*(*(LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL **)(elem1) + 1))((LPCVOID)(elem1), (LPCVOID)(elem2))

typedef enum AttrType {
	//atUNKNOWN,
	atDIR_LEVEL    = 1,
	atADJUST       = 2,
	atREPLACE      = 4,
	atSIZE         = 8,
	atALL          = 15,
	atFUNNEL       = 16,
	atIO_FEP       = 32,
	atE_WITH       = 64,
	atENABLED      = 128,
	atADJUST_CHECK = 256,
	atCHILD_RW     = 512,
	atCAUTION      = 1024,
	atVARIABLE     = 0x0800,
	atDEFINE       = 0x1000,
	atERRORSKIP    = 0x2000,
	atSCOPE        = 0x4000,
} AtType;// for debugger

//---------------------------------------------------------------------
//「クラス宣言」
//---------------------------------------------------------------------
typedef struct _TSSGAttributeElement {
	LPVOID *VTable;
	AtType type;
} TSSGAttributeElement, TSimpleAdjustmentAttribute;

EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();
EXTERN_C void __fastcall delete_TSSGAttributeElement(TSSGAttributeElement *this);
#define TSSGAttributeElement_GetType(/*TSSGAttributeElement * */this) \
	(this)->type
//----------------------------------------------------------------------------
//「階層属性クラス」
//----------------------------------------------------------------------------
typedef struct _TDirAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long level;
} TDirAttribute;

EXTERN_C TDirAttribute * __cdecl new_TDirAttribute();
EXTERN_C void __fastcall delete_TDirAttribute(TDirAttribute *this);
#define TDirAttribute_Setting(/*TDirAttribute * */this, /*unsigned long */Level) \
	((this)->level = Level)
//----------------------------------------------------------------------------
//「読み書きサイズ指定属性クラス」
//----------------------------------------------------------------------------
typedef struct _TSizeAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long size;
} TSizeAttribute;

EXTERN_C TSizeAttribute * __cdecl new_TSizeAttribute();
EXTERN_C void __fastcall delete_TSizeAttribute(TSizeAttribute *this);
#define TSizeAttribute_Setting(/*TSizeAttribute * */this, /*unsigned long */Size) \
	((this)->size = Size)
//----------------------------------------------------------------------------
//「アドレス置き換え属性クラス」
//----------------------------------------------------------------------------
typedef struct _TReplaceAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string offsetCode;
	bcb6_std_string fileName;
	unsigned long   offsetNum;
	DWORD           padding;
} TReplaceAttribute;

EXTERN_C TReplaceAttribute * __cdecl new_TReplaceAttribute();
EXTERN_C void __fastcall delete_TReplaceAttribute(TReplaceAttribute *this);
EXTERN_C void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *);
//----------------------------------------------------------------------------
//「ファンネル（随行書き込み）属性クラス」
//----------------------------------------------------------------------------
typedef struct _TFunnelAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TFunnelAttribute;

EXTERN_C TFunnelAttribute * __cdecl new_TFunnelAttribute();
EXTERN_C void __fastcall delete_TFunnelAttribute(TFunnelAttribute *this);
#define TFunnelAttribute_Setting(/*TFunnelAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//「入出力時変換属性クラス」
//----------------------------------------------------------------------------
typedef struct _TIO_FEPAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string inputCode;
	bcb6_std_string outputCode;
} TIO_FEPAttribute;

EXTERN_C TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute();
EXTERN_C void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *this);
#define TIO_FEPAttribute_GetInputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->inputCode);
#define TIO_FEPAttribute_GetOutputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->outputCode);
//----------------------------------------------------------------------------
//「コード付加属性クラス」
//----------------------------------------------------------------------------
typedef struct _TEndWithAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEndWithAttribute;

#define TEndWithAttribute_Procedures (LPVOID *)0x0064030C
EXTERN_C TEndWithAttribute * __cdecl new_TEndWithAttribute();
EXTERN_C void __fastcall delete_TEndWithAttribute(TEndWithAttribute *this);
#define TEndWithAttribute_GetCode(/*TEndWithAttribute * */this) \
	(&(this)->code)
#define TEndWithAttribute_Setting(/*TEndWithAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
#define TEndWithAttribute_Setting_cstr(/*TEndWithAttribute * */this, /*LPCSTR */Code, /*size_t */Length) \
	bcb6_std_string_assign_cstr_with_length(&(this)->code, Code, Length)
//----------------------------------------------------------------------------
//「有効状態属性クラス」
//----------------------------------------------------------------------------
typedef struct _TEnabledAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEnabledAttribute;

EXTERN_C TEnabledAttribute * __cdecl new_TEnabledAttribute();
EXTERN_C void __fastcall delete_TEnabledAttribute(TEnabledAttribute *this);
#define TEnabledAttribute_Setting(/*TEnabledAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
//----------------------------------------------------------------------------
//「下位項目一括可否属性クラス」
//----------------------------------------------------------------------------
typedef struct _TChildRWAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned char prohibit;
} TChildRWAttribute;

EXTERN_C TChildRWAttribute * __cdecl new_TChildRWAttribute();
EXTERN_C void __fastcall delete_TChildRWAttribute(TChildRWAttribute *this);
//----------------------------------------------------------------------------
//「注意事項指定属性クラス」
//----------------------------------------------------------------------------
typedef struct _TCautionAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TCautionAttribute;

EXTERN_C TCautionAttribute * __cdecl new_TCautionAttribute();
EXTERN_C void __fastcall delete_TCautionAttribute(TCautionAttribute *this);
#define TCautionAttribute_Setting(/*TCautionAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//「アドレス補正の試作などで用いる、補正試作時に優先使用するかなどの属性クラス」
//----------------------------------------------------------------------------
typedef struct _TAdjustCheckAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	BOOLEAN check;
	BOOLEAN mustCheck;
} TAdjustCheckAttribute;

EXTERN_C TAdjustCheckAttribute * __cdecl new_TAdjustCheckAttribute();
EXTERN_C void __fastcall delete_TAdjustCheckAttribute(TAdjustCheckAttribute *this);
//----------------------------------------------------------------------------
//「基本補正属性抽象クラス」
//----------------------------------------------------------------------------
typedef struct _TAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long status;
	unsigned long checkType;
	unsigned long adjustVal;
	int           elemOrder;// define here for convenience' sake.
} TAdjustmentAttribute;

#define TAdjustmentAttribute_Setting(Attr, SSGC, Code) \
	((void(__cdecl *)(void *, void *, const char *))(Attr)->VTable[3])(Attr, SSGC, Code)
//----------------------------------------------------------------------------
//「ヒープ補正属性クラス」
//----------------------------------------------------------------------------
typedef struct _THeapAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TAdjustmentAttribute super;
	};
	BYTE         padding[16];
	bcb6_std_map heapMap;
} THeapAdjustmentAttribute, TScopeAttribute;
//----------------------------------------------------------------------------
//「CRC補正属性クラス」
//----------------------------------------------------------------------------
typedef struct _TCRCAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	BYTE         padding[16];
	bcb6_std_map crcMap;
} TCRCAdjustmentAttribute;
//----------------------------------------------------------------------------
//「サーチ補正属性クラス」
//----------------------------------------------------------------------------
typedef struct _TSearchAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	BYTE            padding[16];
	bcb6_std_string startAddressStr;
	bcb6_std_string minAddressStr;
	bcb6_std_string maxAddressStr;
	unsigned long   step;
	bcb6_std_string searchCode;
	bcb6_std_map    diffMap;
} TSearchAdjustmentAttribute;
//----------------------------------------------------------------------------
//「ダイアログ補正属性クラス」
//----------------------------------------------------------------------------
typedef struct _TDialogAdjustmentAttribute {
	union {
		struct {
			LPVOID          *VTable;
			AtType          type;
			BYTE            padding[16];
			bcb6_std_string startAddressStr;
			bcb6_std_string minAddressStr;
			bcb6_std_string maxAddressStr;
			unsigned long   step;
			bcb6_std_string searchCode;
			bcb6_std_map    diffMap;
		};
		TSearchAdjustmentAttribute super;
	};
	bcb6_std_string        fileName;
	bcb6_std_string        valStr;
	BOOLEAN                isMemoWordWrap;
	bcb6_std_vector_string memo;
	bcb6_std_vector_string answerVec;
	bcb6_std_vector_byte   flagVec;
	bcb6_std_vector        subjectVec;
} TDialogAdjustmentAttribute;
//----------------------------------------------------------------------------
typedef struct value_type {
	unsigned long key;
	struct {
		unsigned long low;
		unsigned long high;
	};
} heapMapValue;
//----------------------------------------------------------------------------

