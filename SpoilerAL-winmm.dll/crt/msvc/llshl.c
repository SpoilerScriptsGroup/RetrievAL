//***
//llshl.c - long shift left
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       define long shift left routine (signed and unsigned are same)
//           __allshl
//
//*******************************************************************************

//***
//llshl - long shift left
//
//Purpose:
//       Does a Long Shift Left (signed and unsigned are identical)
//       Shifts a long left any number of bits.
//
//Entry:
//       EDX:EAX - long value to be shifted
//       CL    - number of bits to shift by
//
//Exit:
//       EDX:EAX - shifted value
//
//Uses:
//       CL is destroyed.
//
//Exceptions:
//
//*******************************************************************************

__declspec(naked) void __cdecl _allshl()
{
__asm
{
//
// Handle shifts of 64 or more bits (all get 0)
//
        cmp     cl, 64
        jae     short RETZERO

//
// Handle shifts of between 0 and 31 bits
//
        cmp     cl, 32
        jae     short MORE32
        shld    edx, eax, cl
        shl     eax, cl
        ret

//
// Handle shifts of between 32 and 63 bits
//
MORE32:
        mov     edx, eax
        xor     eax, eax
        and     cl, 31
        shl     edx, cl
        ret

//
// return 0 in edx:eax
//
RETZERO:
        xor     eax, eax
        xor     edx, edx
        ret
}
}

#if 0
__declspec(naked) __int64 __stdcall _llshl(__int64 value, unsigned char shift)
{
__asm
{
        mov     eax, dword ptr [esp + 4]
        mov     edx, dword ptr [esp + 8]
        mov     ecx, dword ptr [esp + 12]
        call    _allshl
        ret     12
}
}
#endif
