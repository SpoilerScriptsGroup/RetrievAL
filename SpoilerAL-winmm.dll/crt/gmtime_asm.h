static errno_t __cdecl internal_gmtime32_s(struct tm *dest, const __time32_t *source);
static errno_t __cdecl internal_gmtime64_s(struct tm *dest, const __time64_t *source);
static errno_t __cdecl _gmtime_s_less_than_400_years_left();
static errno_t __cdecl _gmtime_s_jan();
static errno_t __cdecl _gmtime_s_feb();
static errno_t __cdecl _gmtime_s_mar();
static errno_t __cdecl _gmtime_s_apr();
static errno_t __cdecl _gmtime_s_may();
static errno_t __cdecl _gmtime_s_jun();
static errno_t __cdecl _gmtime_s_jul();
static errno_t __cdecl _gmtime_s_aug();
static errno_t __cdecl _gmtime_s_sep();
static errno_t __cdecl _gmtime_s_oct();
static errno_t __cdecl _gmtime_s_nov();
static errno_t __cdecl _gmtime_s_dec();
static errno_t __cdecl _gmtime_s_next_month();

__declspec(naked) errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     ecx, dword ptr [dest]
		mov     edx, dword ptr [source]
		test    ecx, ecx
		jz      L2
		test    edx, edx
		jz      L1
		mov     edx, dword ptr [edx]
		mov     eax, 0x88888889
		cmp     edx, MIN_LOCAL_TIME
		jge     internal_gmtime32_s
	L1:
		or      edx, -1
		mov     dword ptr [ecx     ], edx
		mov     dword ptr [ecx +  4], edx
		mov     dword ptr [ecx +  8], edx
		mov     dword ptr [ecx + 12], edx
		mov     dword ptr [ecx + 16], edx
		mov     dword ptr [ecx + 20], edx
		mov     dword ptr [ecx + 24], edx
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], edx
	L2:
		mov     eax, EINVAL
		ret

		#undef dest
		#undef source
	}
}

__declspec(naked) errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     edx, dword ptr [dest]
		mov     ecx, dword ptr [source]
		test    edx, edx
		jz      L5
		test    ecx, ecx
		jz      L4
		mov     eax, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		test    ecx, ecx
		jg      L3
		jl      L1
		test    eax, eax
		js      internal_gmtime64_s
		jmp     L2

	L1:
		inc     ecx
		jnz     L4
		cmp     eax, MIN_LOCAL_TIME
		jb      L4
	L2:
		mov     ecx, edx
		mov     edx, eax
		mov     eax, 0x88888889
		jmp     internal_gmtime32_s

	L3:
		cmp     ecx, 7
		jb      internal_gmtime64_s
		ja      L4
		cmp     eax, 0x9358E1CF
		jbe     internal_gmtime64_s
	L4:
		or      ecx, -1
		mov     dword ptr [edx     ], ecx
		mov     dword ptr [edx +  4], ecx
		mov     dword ptr [edx +  8], ecx
		mov     dword ptr [edx + 12], ecx
		mov     dword ptr [edx + 16], ecx
		mov     dword ptr [edx + 20], ecx
		mov     dword ptr [edx + 24], ecx
		mov     dword ptr [edx + 28], ecx
		mov     dword ptr [edx + 32], ecx
	L5:
		mov     eax, EINVAL
		ret

		#undef dest
		#undef source
	}
}

__declspec(naked) static errno_t __cdecl internal_gmtime32_s(struct tm *dest, const __time32_t *source)
{
	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		push    ebx
		push    esi
		push    edi
		lea     edi, [edx + DAY_SEC]
		mul     edi
		shr     edx, 5
		mov     eax, 0x04444445
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
		mov     eax, 0x0AAAAAAB
		mov     ebx, edx
		mov     dword ptr [ecx], edi
		mul     edx
		mov     dword ptr [ecx + 4], esi
		lea     eax, [edx + edx * 2]
		shl     eax, 3
		lea     esi, [edx + SINCE(1970) - (SINCE(1600) + LEAP_DAY) - 1]
		sub     ebx, eax
		sub     esi, YEAR400
		mov     dword ptr [ecx + 8], ebx
		jb      L1
		mov     edi, 400
		jmp     _gmtime_s_less_than_400_years_left

	L1:
		add     esi, YEAR400
		xor     edi, edi
		jmp     _gmtime_s_less_than_400_years_left

		#undef dest
		#undef source
	}

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

__declspec(naked) static errno_t __cdecl internal_gmtime64_s(struct tm *dest, const __time64_t *source)
{
	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		push    ebx
		push    esi
		push    edi
		push    ebp
		mov     esi, 0x72894AB7
		mov     ebx, eax
		mul     esi
		mov     ebp, edx
		mov     eax, 0xC22E4506
		mul     ebx
		push    ebx
		mov     ebx, eax
		mov     eax, ecx
		mov     edi, edx
		mul     esi
		mov     esi, eax
		mov     eax, ecx
		mov     ecx, edx
		mov     edx, 0xC22E4506
		mul     edx
		add     ebx, esi
		adc     ecx, 0
		mov     esi, eax
		xor     eax, eax
		add     ebp, ebx
		adc     edi, eax
		add     ecx, edi
		adc     eax, eax
		add     esi, ecx
		adc     edx, eax
		pop     ebx
		shrd    esi, edx, 16
		imul    eax, esi, DAY_SEC
		sub     ebx, eax
		mov     eax, 0x04444445
		mul     ebx
		mov     eax, 0x04444445
		mov     ecx, edx
		mul     edx
		mov     ebp, ecx
		add     esi, SINCE(1970) - (SINCE(1600) + LEAP_DAY)
		shl     ecx, 4
		mov     eax, edx
		shl     eax, 4
		sub     ecx, ebp
		shl     ecx, 2
		sub     eax, edx
		shl     eax, 2
		sub     ebx, ecx
		mov     ecx, dword ptr [dest + 16]
		sub     ebp, eax
		mov     dword ptr [ecx], ebx
		mov     dword ptr [ecx + 4], ebp
		mov     dword ptr [ecx + 8], edx
		xor     edi, edi
		sub     esi, YEAR400 * 2
		jae     L2
		add     esi, YEAR400
		jc      L1
		add     esi, YEAR400
		pop     ebp
		jmp     _gmtime_s_less_than_400_years_left

	L1:
		mov     edi, 400
		pop     ebp
		jmp     _gmtime_s_less_than_400_years_left

	L2:
		cmp     esi, YEAR400
		jae     L3
		mov     edi, 400 * 2
		pop     ebp
		jmp     _gmtime_s_less_than_400_years_left

	L3:
		sub     esi, YEAR400
		mov     edi, 400 * 3
		pop     ebp
		jmp     _gmtime_s_less_than_400_years_left

		#undef dest
		#undef source
	}

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

#define LABEL(name)                             \
    }                                           \
}                                               \
__declspec(naked) static errno_t __cdecl name() \
{                                               \
    __asm                                       \
    {

__declspec(naked) static errno_t __cdecl _gmtime_s_less_than_400_years_left()
{
	#define LEAP_DAY (31 + 28)
	#define YEAR     365
	#define YEAR4    (YEAR * 4 + 1)
	#define YEAR100  (YEAR4 * 25 - 1)

	static const errno_t(__cdecl *JumpTable[])() = {
		_gmtime_s_jan,
		_gmtime_s_feb,
		_gmtime_s_mar,
		_gmtime_s_apr,
		_gmtime_s_may,
		_gmtime_s_jun,
		_gmtime_s_jul,
		_gmtime_s_aug,
		_gmtime_s_sep,
		_gmtime_s_oct,
		_gmtime_s_nov,
		_gmtime_s_dec,
	};

	__asm
	{
		mov     eax, 0x24924925
		lea     ebx, [esi + 2]
		mul     ebx
		lea     eax, [ebx + edx]
		lea     edx, [edx * 8]
		xor     ebx, ebx
		sub     eax, edx
		cmp     esi, YEAR - LEAP_DAY + 1
		mov     dword ptr [ecx + 24], eax
		adc     ebx, ebx
		jnz     L30
		cmp     esi, YEAR100 * 2
		jae     L1
		sub     esi, YEAR100
		jb      L3
		add     edi, 100
		jmp     L4

	L1:
		sub     esi, YEAR100 * 3
		jb      L2
		add     edi, 300
		jmp     L4

	L2:
		add     edi, 200
	L3:
		add     esi, YEAR100
	L4:
		cmp     esi, YEAR - LEAP_DAY + 1
		jb      L30
		cmp     esi, YEAR4 * 16
		jae     L19
		cmp     esi, YEAR4 * 8
		jae     L11
		cmp     esi, YEAR4 * 4
		jae     L7
		cmp     esi, YEAR4 * 2
		jae     L5
		sub     esi, YEAR4
		jb      L28
		add     edi, 4
		jmp     L29

	L5:
		sub     esi, YEAR4 * 3
		jae     L6
		add     edi, 4 * 2
		jmp     L28

	L6:
		add     edi, 4 * 3
		jmp     L29

	L7:
		cmp     esi, YEAR4 * 6
		jae     L9
		sub     esi, YEAR4 * 5
		jae     L8
		add     edi, 4 * 4
		jmp     L28

	L8:
		add     edi, 4 * 5
		jmp     L29

	L9:
		sub     esi, YEAR4 * 7
		jae     L10
		add     edi, 4 * 6
		jmp     L28

	L10:
		add     edi, 4 * 7
		jmp     L29

	L11:
		cmp     esi, YEAR4 * 12
		jae     L15
		cmp     esi, YEAR4 * 10
		jae     L13
		sub     esi, YEAR4 * 9
		jae     L12
		add     edi, 4 * 8
		jmp     L28

	L12:
		add     edi, 4 * 9
		jmp     L29

	L13:
		sub     esi, YEAR4 * 11
		jae     L14
		add     edi, 4 * 10
		jmp     L28

	L14:
		add     edi, 4 * 11
		jmp     L29

	L15:
		cmp     esi, YEAR4 * 14
		jae     L17
		sub     esi, YEAR4 * 13
		jae     L16
		add     edi, 4 * 12
		jmp     L28

	L16:
		add     edi, 4 * 13
		jmp     L29

	L17:
		sub     esi, YEAR4 * 15
		jae     L18
		add     edi, 4 * 14
		jmp     L28

	L18:
		add     edi, 4 * 15
		jmp     L29

	L19:
		cmp     esi, YEAR4 * 21
		jae     L24
		cmp     esi, YEAR4 * 19
		jae     L22
		sub     esi, YEAR4 * 18
		jae     L21
		add     esi, YEAR4
		jc      L20
		add     edi, 4 * 16
		jmp     L28

	L20:
		add     edi, 4 * 17
		jmp     L29

	L21:
		add     edi, 4 * 18
		jmp     L29

	L22:
		sub     esi, YEAR4 * 20
		jae     L23
		add     edi, 4 * 19
		jmp     L28

	L23:
		add     edi, 4 * 20
		jmp     L29

	L24:
		cmp     esi, YEAR4 * 23
		jae     L26
		sub     esi, YEAR4 * 22
		jae     L25
		add     edi, 4 * 21
		jmp     L28

	L25:
		add     edi, 4 * 22
		jmp     L29

	L26:
		sub     esi, YEAR4 * 24
		jb      L27
		add     edi, 4 * 24
		jmp     L29

	L27:
		add     edi, 4 * 23
	L28:
		add     esi, YEAR4
	L29:
		cmp     esi, YEAR - LEAP_DAY + 1
		adc     ebx, ebx
		jz      L31
	L30:
		lea     esi, [esi + ebx + LEAP_DAY - 1]
		jmp     L36

	L31:
		cmp     esi, YEAR * 2 - LEAP_DAY + 1
		jae     L32
		sub     esi, YEAR - LEAP_DAY + 1
		jb      L35
		inc     edi
		jmp     L36

	L32:
		sub     esi, YEAR * 3 - LEAP_DAY + 1
		jae     L33
		add     edi, 2
		jmp     L35

	L33:
		sub     esi, YEAR
		jb      L34
		add     edi, 4
		jmp     L36

	L34:
		add     edi, 3
	L35:
		add     esi, YEAR
	L36:
		sub     edi, 1900 - 1600
		mov     edx, esi
		shr     esi, 5
		xor     eax, eax
		mov     dword ptr [ecx + 16], esi
		mov     dword ptr [ecx + 20], edi
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], eax
		jmp     dword ptr [JumpTable + esi * 4]

	LABEL(_gmtime_s_jan)
		sub     edx, 30
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_feb)
		sub     edx, 30
		add     ebx, 28
		sub     edx, ebx
		ja      _gmtime_s_next_month
		add     edx, ebx
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_mar)
		sub     edx, ebx
		sub     edx, 89
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_apr)
		sub     edx, ebx
		sub     edx, 119
		ja      _gmtime_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_may)
		sub     edx, ebx
		sub     edx, 150
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_jun)
		sub     edx, ebx
		sub     edx, 180
		ja      _gmtime_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_jul)
		sub     edx, ebx
		sub     edx, 211
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_aug)
		sub     edx, ebx
		sub     edx, 242
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_sep)
		sub     edx, ebx
		sub     edx, 272
		ja      _gmtime_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_oct)
		sub     edx, ebx
		sub     edx, 303
		ja      _gmtime_s_next_month
		add     edx, 31
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_nov)
		sub     edx, ebx
		sub     edx, 333
		ja      _gmtime_s_next_month
		add     edx, 30
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_dec)
		sub     edx, ebx
		sub     edx, 333
		pop     edi
		mov     dword ptr [ecx + 12], edx
		pop     esi
		pop     ebx
		ret

	LABEL(_gmtime_s_next_month)
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

	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
}

#undef LABEL
