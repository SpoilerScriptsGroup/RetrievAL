//***
//llmul.c - long multiply routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       Defines long multiply routine
//       Both signed and unsigned routines are the same, since multiply's
//       work out the same in 2's complement
//       creates the following routine:
//           __allmul
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [x + 4]

//***
//llmul - long multiply routine
//
//Purpose:
//       Does a long multiply (same for signed/unsigned)
//       Parameters are not changed.
//
//Entry:
//       Parameters are passed on the stack:
//               1st pushed: multiplier (QWORD)
//               2nd pushed: multiplicand (QWORD)
//
//Exit:
//       EDX:EAX - product of multiplier and multiplicand
//       NOTE: parameters are removed from the stack
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

__declspec(naked) void __cdecl _allmul()
{
__asm
{
#define A (esp + 4)                     // stack address of a
#define B (esp + 12)                    // stack address of b

//
//       AHI, BHI : upper 32 bits of A and B
//       ALO, BLO : lower 32 bits of A and B
//
//             ALO * BLO
//       ALO * BHI
// +     BLO * AHI
// ---------------------
//

#if 0
        mov     eax, HIWORD(A)
        mov     ecx, HIWORD(B)
        or      ecx, eax                // test for both hiwords zero.
        mov     ecx, LOWORD(B)
        jnz     short hard              // both are zero, just mult ALO and BLO

        mov     eax, LOWORD(A)
        mul     ecx

        ret     16                      //  callee restores the stack

hard:
        push    ebx

// must redefine A and B since esp has been altered

#define A2 (esp + 8)                    // stack address of a
#define B2 (esp + 16)                   // stack address of b

#else
        mov     eax, HIWORD(A)
        mov     edx, HIWORD(B)
        mov     ecx, LOWORD(B)
        or      edx, eax                // test for both hiwords zero.
        push    ebx
        jnz     short hard              // both are zero, just mult ALO and BLO

// must redefine A and B since esp has been altered

#define A2 (esp + 8)                    // stack address of a
#define B2 (esp + 16)                   // stack address of b

        mov     eax, LOWORD(A2)
        pop     ebx
        mul     ecx

        ret     16                      //  callee restores the stack

hard:
#endif
        mul     ecx                     // eax has AHI, ecx has BLO, so AHI * BLO
        mov     ebx, eax                // save result

        mov     eax, LOWORD(A2)
        mul     dword ptr HIWORD(B2)    // ALO * BHI
        add     ebx, eax                // ebx = ((ALO * BHI) + (AHI * BLO))

        mov     eax, LOWORD(A2)         // ecx = BLO
        mul     ecx                     // so edx:eax = ALO * BLO
        add     edx, ebx                // now edx has all the LO * HI stuff

        pop     ebx

        ret     16                      // callee restores the stack
}
}

#if 0
__declspec(naked) __int64 __stdcall _llmul(__int64 multiplier, __int64 multiplicand)
{
__asm
{
        jmp     _allmul
}
}
#endif
