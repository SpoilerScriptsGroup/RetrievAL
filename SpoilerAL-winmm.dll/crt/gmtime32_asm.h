static void __cdecl _gmtime32_s_jan();
static void __cdecl _gmtime32_s_feb();
static void __cdecl _gmtime32_s_mar();
static void __cdecl _gmtime32_s_apr();
static void __cdecl _gmtime32_s_may();
static void __cdecl _gmtime32_s_jun();
static void __cdecl _gmtime32_s_jul();
static void __cdecl _gmtime32_s_aug();
static void __cdecl _gmtime32_s_sep();
static void __cdecl _gmtime32_s_oct();
static void __cdecl _gmtime32_s_nov();
static void __cdecl _gmtime32_s_dec();
static void __cdecl _gmtime32_s_next_month();

#define LABEL(name)                          \
    }                                        \
}                                            \
__declspec(naked) static void __cdecl name() \
{                                            \
    __asm                                    \
    {

__declspec(naked) errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	static const void(__cdecl *JumpTable[])() = {
		_gmtime32_s_jan,
		_gmtime32_s_feb,
		_gmtime32_s_mar,
		_gmtime32_s_apr,
		_gmtime32_s_may,
		_gmtime32_s_jun,
		_gmtime32_s_jul,
		_gmtime32_s_aug,
		_gmtime32_s_sep,
		_gmtime32_s_oct,
		_gmtime32_s_nov,
		_gmtime32_s_dec,
	};

	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB     (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     ecx, dword ptr [dest]
		mov     edx, dword ptr [source]
		test    ecx, ecx
		jz      L8
		test    edx, edx
		jz      L7
		mov     edx, dword ptr [edx]
		mov     eax, 88888889H
		cmp     edx, MIN_LOCAL_TIME
		jl      L7
		push    ebx
		push    esi
		push    edi
		lea     edi, [edx + DAY_SEC]
		mul     edi
		shr     edx, 5
		mov     eax, 04444445H
		mov     ebx, edx
		mov     esi, edx
		mul     edx
		shl     ebx, 4
		mov     eax, edx
		shl     eax, 4
		sub     ebx, esi
		shl     ebx, 2
		sub     eax, edx
		shl     eax, 2
		sub     edi, ebx
		sub     esi, eax
		mov     eax, 0AAAAAABH
		mov     ebx, edx
		mov     dword ptr [ecx], edi
		mul     edx
		mov     dword ptr [ecx + 4], esi
		lea     eax, [edx + edx * 2]
		shl     eax, 3
		lea     esi, [edx + SINCE(1970) - SINCE(1600) - JAN_FEB - 1]
		sub     ebx, eax
		sub     esi, YEAR400
		mov     dword ptr [ecx + 8], ebx
		jb      L1
		mov     edi, 400
		jmp     L2

	L1:
		add     esi, YEAR400
		xor     edi, edi
	L2:
		mov     eax, 24924925H
		lea     ebx, [esi + 2]
		mul     ebx
		lea     eax, [ebx + edx]
		lea     edx, [edx * 8]
		xor     ebx, ebx
		sub     eax, edx
		cmp     esi, YEAR - JAN_FEB + 1
		mov     dword ptr [ecx + 24], eax
		adc     ebx, 0
		jnz     L4
		mov     eax, 002CDB61H
		add     edi, 300
		sub     esi, YEAR100 * 3
		jae     L3
		sub     edi, 100
		add     esi, YEAR100
		jc      L3
		sub     edi, 100
		add     esi, YEAR100
		jc      L3
		sub     edi, 100
		add     esi, YEAR100
	L3:
		cmp     esi, YEAR - JAN_FEB + 1
		jb      L4
		mul     esi
		lea     edi, [edi + edx * 4]
		imul    eax, edx, YEAR4
		sub     esi, eax
		cmp     esi, YEAR - JAN_FEB + 1
		adc     ebx, 0
		jz      L5
	L4:
		lea     esi, [esi + ebx + JAN_FEB - 1]
		jmp     L6

	L5:
		add     esi, JAN_FEB - 1
		add     edi, 4
		sub     esi, YEAR * 4
		jae     L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
	L6:
		sub     edi, 1900 - 1600
		mov     edx, esi
		shr     esi, 5
		xor     eax, eax
		mov     dword ptr [ecx + 16], esi
		mov     dword ptr [ecx + 20], edi
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], eax
		jmp     dword ptr [JumpTable + esi * 4]

	L7:
		mov     dword ptr [ecx     ], -1
		mov     dword ptr [ecx +  4], -1
		mov     dword ptr [ecx +  8], -1
		mov     dword ptr [ecx + 12], -1
		mov     dword ptr [ecx + 16], -1
		mov     dword ptr [ecx + 20], -1
		mov     dword ptr [ecx + 24], -1
		mov     dword ptr [ecx + 28], -1
		mov     dword ptr [ecx + 32], -1
	L8:
		mov     eax, EINVAL
		ret

	LABEL(_gmtime32_s_jan)
		sub     edx, 30
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_feb)
		sub     edx, 30
		add     ebx, 28
		sub     edx, ebx
		ja      _gmtime32_s_next_month
		add     edx, ebx
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_mar)
		sub     edx, ebx
		sub     edx, 89
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_apr)
		sub     edx, ebx
		sub     edx, 119
		ja      _gmtime32_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_may)
		sub     edx, ebx
		sub     edx, 150
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_jun)
		sub     edx, ebx
		sub     edx, 180
		ja      _gmtime32_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_jul)
		sub     edx, ebx
		sub     edx, 211
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_aug)
		sub     edx, ebx
		sub     edx, 242
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_sep)
		sub     edx, ebx
		sub     edx, 272
		ja      _gmtime32_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_oct)
		sub     edx, ebx
		sub     edx, 303
		ja      _gmtime32_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_nov)
		sub     edx, ebx
		sub     edx, 333
		ja      _gmtime32_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_dec)
		sub     edx, ebx
		sub     edx, 333
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime32_s_next_month)
		inc     esi
		pop     edi
		mov     dword ptr [ecx + 12], edx
		mov     dword ptr [ecx + 16], esi
		pop     esi
		pop     ebx
		ret

		#undef dest
		#undef source
	}

	#undef DAY_SEC
	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

#undef LABEL
