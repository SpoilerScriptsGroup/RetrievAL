//***
//lldiv.c - signed long divide routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       defines the signed long divide routine
//           __alldiv
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [x + 4]

//***
//lldiv - signed long divide
//
//Purpose:
//       Does a signed long divide of the arguments.  Arguments are
//       not changed.
//
//Entry:
//       Arguments are passed on the stack:
//               1st pushed: divisor (QWORD)
//               2nd pushed: dividend (QWORD)
//
//Exit:
//       EDX:EAX contains the quotient (dividend/divisor)
//       NOTE: this routine removes the parameters from the stack.
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

__declspec(naked) void __cdecl _alldiv()
{
	__asm
	{
		push    edi
		push    esi
		push    ebx

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a/b will
		// generate a call to lldiv(a, b)):
		//
		//               -----------------
		//               |               |
		//               |---------------|
		//               |               |
		//               |--divisor (b)--|
		//               |               |
		//               |---------------|
		//               |               |
		//               |--dividend (a)-|
		//               |               |
		//               |---------------|
		//               | return addr** |
		//               |---------------|
		//               |      EDI      |
		//               |---------------|
		//               |      ESI      |
		//               |---------------|
		//       ESP---->|      EBX      |
		//               -----------------
		//

		#define DVND (esp + 16)         // stack address of dividend (a)
		#define DVSR (esp + 24)         // stack address of divisor (b)


		// Determine sign of the result (edi = 0 if result is positive, non-zero
		// otherwise) and make operands positive.

		xor     edi, edi                // result sign assumed positive

#if 0
		mov     eax, HIWORD(DVND)       // hi word of a
		or      eax, eax                // test to see if signed
		jge     short L1                // skip rest if a is already positive
		inc     edi                     // complement result sign flag
		mov     edx, LOWORD(DVND)       // lo word of a
		neg     eax                     // make a positive
		neg     edx
		sbb     eax, 0
		mov     HIWORD(DVND), eax       // save positive value
		mov     LOWORD(DVND), edx
#else
		mov     eax, HIWORD(DVND)       // hi word of a
		mov     edx, HIWORD(DVSR)       // hi word of b
		or      eax, eax                // test to see if signed
		jge     short L1                // skip rest if a is already positive
		mov     ecx, LOWORD(DVND)       // lo word of a
		xor     eax, -1                 // make a positive
		neg     ecx
		sbb     eax, -1
		dec     edi                     // complement result sign flag
		mov     HIWORD(DVND), eax       // save positive value
		mov     LOWORD(DVND), ecx
#endif
	L1:
#if 0
		mov     eax, HIWORD(DVSR)       // hi word of b
		or      eax, eax                // test to see if signed
		jge     short L2                // skip rest if b is already positive
		inc     edi                     // complement the result sign flag
		mov     edx, LOWORD(DVSR)       // lo word of a
		neg     eax                     // make b positive
		neg     edx
		sbb     eax,0
		mov     HIWORD(DVSR), eax       // save positive value
		mov     LOWORD(DVSR), edx
#else
		or      edx, edx                // test to see if signed
		jge     short L2                // skip rest if b is already positive
		mov     ecx, LOWORD(DVSR)       // lo word of b
		xor     edx, -1                 // make b positive
		neg     ecx
		sbb     edx, -1
		xor     edi, -1                 // complement the result sign flag
		mov     HIWORD(DVSR), edx       // save positive value
		mov     LOWORD(DVSR), ecx
#endif
	L2:

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//
		// NOTE - eax currently contains the high order word of DVSR
		//

#if 0
		or      eax, eax                // check to see if divisor < 4194304K
		jnz     short L3                // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)       // load divisor
		mov     eax, HIWORD(DVND)       // load high word of dividend
#else
		or      edx, edx                // check to see if divisor < 4194304K
		jnz     short L3                // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)       // load divisor
#endif
		xor     edx, edx
		div     ecx                     // eax <- high order bits of quotient
		mov     ebx, eax                // save high bits of quotient
		mov     eax, LOWORD(DVND)       // edx:eax <- remainder:lo word of dividend
		div     ecx                     // eax <- low order bits of quotient
		mov     edx, ebx                // edx:eax <- quotient
		jmp     short L4                // set sign, restore stack and return

		//
		// Here we do it the hard way.  Remember, eax contains the high word of DVSR
		//

	L3:
#if 0
		mov     ebx, eax                // ebx:ecx <- divisor
		mov     ecx, LOWORD(DVSR)
		mov     edx, HIWORD(DVND)       // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
	L5:
		shr     ebx, 1                  // shift divisor right one bit
		rcr     ecx, 1
		shr     edx, 1                  // shift dividend right one bit
		rcr     eax, 1
		or      ebx, ebx
		jnz     short L5                // loop until divisor < 4194304K
		div     ecx                     // now divide, ignore remainder
#else
		mov     ebx, edx
		jns     short shift
		xor     edx, edx
		jmp     short divide

	shift:
		cmp     edx, 1 shl 4
		jae     bitscan
		mov     ecx, edx                // ecx:ebx <- divisor
		mov     ebx, LOWORD(DVSR)
		shr     ecx, 1
		mov     edx, eax                // edx:eax <- dividend
		rcr     ebx, 1
		mov     eax, LOWORD(DVND)
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		jmp     divide

	bitscan:
		bsr     ecx, edx
		mov     ebx, LOWORD(DVSR)       // edx:ebx <- divisor
		inc     ecx
		shrd    ebx, edx, cl
		mov     edx, eax                // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
		shrd    eax, edx, cl
		shr     edx, cl
	divide:
		div     ebx                     // now divide, ignore remainder
#endif
		mov     esi, eax                // save quotient

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		mul     dword ptr HIWORD(DVSR)  // QUOT * HIWORD(DVSR)
		mov     ecx, eax
		mov     eax, LOWORD(DVSR)
		mul     esi                     // QUOT * LOWORD(DVSR)
		add     edx, ecx                // EDX:EAX = QUOT * DVSR
		jc      short L6                // carry means Quotient is off by 1

		//
		// do long compare here between original dividend and the result of the
		// multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
		// subtract one (1) from the quotient.
		//

		cmp     edx, HIWORD(DVND)       // compare hi words of result and original
		ja      short L6                // if result > original, do subtract
		jb      short L7                // if result < original, we are ok
		cmp     eax, LOWORD(DVND)       // hi words are equal, compare lo words
		jbe     short L7                // if less or equal we are ok, else subtract
	L6:
		dec     esi                     // subtract 1 from quotient
	L7:
		xor     edx, edx                // edx:eax <- quotient
		mov     eax, esi

		//
		// Just the cleanup left to do.  edx:eax contains the quotient.  Set the sign
		// according to the save value, cleanup the stack, and return.
		//

	L4:
#if 0
		dec     edi                     // check to see if result is negative
		jnz     short L8                // if EDI == 0, result should be negative
		neg     edx                     // otherwise, negate the result
		neg     eax
		sbb     edx, 0
#else
		xor     eax, edi                // otherwise, negate the result
		xor     edx, edi
		sub     eax, edi
		sbb     edx, edi
#endif

		//
		// Restore the saved registers and return.
		//

#if 0
	L8:
#endif
		pop     ebx
		pop     esi
		pop     edi

		ret     16

		#undef DVND
		#undef DVSR
	}
}

#if 0
__declspec(naked) __int64 __stdcall _lldiv(__int64 dividend, __int64 divisor)
{
	__asm
	{
		jmp     _alldiv
	}
}
#endif
