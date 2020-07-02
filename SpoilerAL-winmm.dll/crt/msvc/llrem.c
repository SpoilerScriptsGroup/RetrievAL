//***
//llrem.c - signed long remainder routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       defines the signed long remainder routine
//           __allrem
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [x + 4]

//***
//llrem - signed long remainder
//
//Purpose:
//       Does a signed long remainder of the arguments.  Arguments are
//       not changed.
//
//Entry:
//       Arguments are passed on the stack:
//               1st pushed: divisor (QWORD)
//               2nd pushed: dividend (QWORD)
//
//Exit:
//       EDX:EAX contains the remainder (dividend%divisor)
//       NOTE: this routine removes the parameters from the stack.
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

#ifdef CFUNCTION
#define __cdecl __fastcall
#define _allrem _llrem
#endif

__declspec(naked) __int64 __cdecl _allrem(__int64 dividend, __int64 divisor)
{
#if 0
	__asm
	{
		push    ebx
		push    edi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a%b will
		// generate a call to lrem(a, b)):
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
		//               |       EBX     |
		//               |---------------|
		//       ESP---->|       EDI     |
		//               -----------------
		//

		#define DVND (esp + 12)             // stack address of dividend (a)
		#define DVSR (esp + 20)             // stack address of divisor (b)


		// Determine sign of the result (edi = 0 if result is positive, non-zero
		// otherwise) and make operands positive.

		or      edi, -1                     // result sign assumed positive

		mov     eax, HIWORD(DVND)           // hi word of a
		or      eax, eax                    // test to see if signed
		jge     short L1                    // skip rest if a is already positive
		inc     edi                         // complement result sign flag bit
		mov     edx, LOWORD(DVND)           // lo word of a
		neg     eax                         // make a positive
		neg     edx
		sbb     eax, 0
		mov     HIWORD(DVND), eax           // save positive value
		mov     LOWORD(DVND), edx
	L1:
		mov     eax, HIWORD(DVSR)           // hi word of b
		or      eax, eax                    // test to see if signed
		jge     short L2                    // skip rest if b is already positive
		mov     edx, LOWORD(DVSR)           // lo word of b
		neg     eax                         // make b positive
		neg     edx
		sbb     eax, 0
		mov     HIWORD(DVSR), eax           // save positive value
		mov     LOWORD(DVSR), edx
	L2:

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//
		// NOTE - eax currently contains the high order word of DVSR
		//

		or      eax, eax                    // check to see if divisor < 4194304K
		jnz     short L3                    // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)           // load divisor
		mov     eax, HIWORD(DVND)           // load high word of dividend
		xor     edx, edx
		div     ecx                         // edx <- remainder
		mov     eax, LOWORD(DVND)           // edx:eax <- remainder:lo word of dividend
		div     ecx                         // edx <- final remainder
		mov     eax, edx                    // edx:eax <- remainder
		xor     edx, edx
		dec     edi                         // check result sign flag
		jns     short L4                    // negate result, restore stack and return
		jmp     short L8                    // result sign ok, restore stack and return

		//
		// Here we do it the hard way.  Remember, eax contains the high word of DVSR
		//

	L3:
		mov     ebx, eax                    // ebx:ecx <- divisor
		mov     ecx, LOWORD(DVSR)
		mov     edx, HIWORD(DVND)           // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
	L5:
		shr     ebx, 1                      // shift divisor right one bit
		rcr     ecx, 1
		shr     edx, 1                      // shift dividend right one bit
		rcr     eax, 1
		or      ebx, ebx
		jnz     short L5                    // loop until divisor < 4194304K
		div     ecx                         // now divide, ignore remainder

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		mov     ecx, eax                    // save a copy of quotient in ECX
		mul     dword ptr HIWORD(DVSR)
		xchg    ecx, eax                    // save product, get quotient in EAX
		mul     dword ptr LOWORD(DVSR)
		add     edx, ecx                    // EDX:EAX = QUOT * DVSR
		jc      short L6                    // carry means Quotient is off by 1

		//
		// do long compare here between original dividend and the result of the
		// multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
		// subtract the original divisor from the result.
		//

		cmp     edx, HIWORD(DVND)           // compare hi words of result and original
		ja      short L6                    // if result > original, do subtract
		jb      short L7                    // if result < original, we are ok
		cmp     eax, LOWORD(DVND)           // hi words are equal, compare lo words
		jbe     short L7                    // if less or equal we are ok, else subtract
	L6:
		sub     eax, LOWORD(DVSR)           // subtract divisor from result
		sbb     edx, HIWORD(DVSR)
	L7:

		//
		// Calculate remainder by subtracting the result from the original dividend.
		// Since the result is already in a register, we will do the subtract in the
		// opposite direction and negate the result if necessary.
		//

		sub     eax, LOWORD(DVND)           // subtract dividend from result
		sbb     edx, HIWORD(DVND)

		//
		// Now check the result sign flag to see if the result is supposed to be positive
		// or negative.  It is currently negated (because we subtracted in the 'wrong'
		// direction), so if the sign flag is set we are done, otherwise we must negate
		// the result to make it positive again.
		//

		dec     edi                         // check result sign flag
		jns     short L8                    // result is ok, restore stack and return
	L4:
		neg     edx                         // otherwise, negate the result
		neg     eax
		sbb     edx, 0

		//
		// Just the cleanup left to do.  edx:eax contains the quotient.
		// Restore the saved registers and return.
		//

	L8:
		pop     edi
		pop     ebx

		ret     16

		#undef DVND
		#undef DVSR
	}
#else
	__asm
	{
		push    ebx
		push    ebp
		push    esi
		push    edi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a%b will
		// generate a call to lrem(a, b)):
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
		//               |      EBX      |
		//               |---------------|
		//               |      EBP      |
		//               |---------------|
		//               |      ESI      |
		//               |---------------|
		//       ESP---->|      EDI      |
		//               -----------------
		//

		#define DVNDLO (esp + 20)           // stack address of dividend (a)
		#define DVNDHI (esp + 24)
		#define DVSRLO (esp + 28)           // stack address of divisor (b)
		#define DVSRHI (esp + 32)

		// Determine sign of the result (edi = 0 if result is positive, non-zero
		// otherwise) and make operands positive.

		mov     edi, dword ptr [DVNDHI]     // load dividend
		mov     esi, dword ptr [DVSRHI]     // load divisor
		mov     ebx, dword ptr [DVNDLO]
		mov     eax, edi
		sar     edi, 31
		mov     edx, esi
		sar     esi, 31
		mov     ebp, dword ptr [DVSRLO]
		xor     eax, edi
		add     ebx, edi
		sbb     eax, edi
		xor     ebx, edi
		xor     edx, esi
		add     ebp, esi
		sbb     edx, esi
		xor     ebp, esi

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//
		// NOTE - eax currently contains the high order word of DVSR
		//

		test    edx, edx                    // check to see if divisor < 4194304K
		jnz     hard                        // nope, gotta do this the hard way
		div     ebp                         // EDX <- remainder
		mov     eax, ebx                    // EDX:EAX <- remainder:lo word of dividend
		div     ebp                         // EDX <- final remainder
		mov     eax, edx                    // EDX:EAX <- remainder
		xor     edx, edx
		jmp     negate                      // negate result, restore stack and return

		align   16
	hard:
		//
		// Here we do it the hard way.  Remember, EAX contains the high word of DVSR
		//

		push    eax                         // save positive value
		push    edx

		#undef DVNDLO
		#undef DVNDHI
		#undef DVSRLO
		#undef DVSRHI
		#define DVNDLO ebx
		#define DVNDHI (esp +  4)
		#define DVSRLO ebp
		#define DVSRHI (esp     )

		mov     esi, edx
		jns     shift
		xor     edx, edx
		jmp     divide

		align   16
	shift:
		bsr     ecx, edx
		mov     esi, DVSRLO                 // EDX:ESI <- divisor
		inc     ecx
		shrd    esi, edx, cl
		mov     edx, eax                    // EDX:EAX <- dividend
		mov     eax, DVNDLO
		shrd    eax, edx, cl
		shr     edx, cl
	divide:
		div     esi                         // now divide, ignore remainder

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		#undef DVSRHI
		#define DVSRHI esi

		pop     esi                         // ESI <- DVSRHI
		mov     edx, eax                    // save a copy of quotient in ESI
		imul    eax, DVSRHI
		mov     ecx, eax                    // put partial product in ECX, get quotient in EAX
		mov     eax, edx
		mul     DVSRLO

		//
		// do long compare here between original dividend and the result of the
		// multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
		// subtract the original divisor from the result.
		//

		add     ecx, edx                    // ECX:EAX = QUOT * DVSR
		sub     DVNDLO, eax
		pop     edx                         // EDX <- DVNDHI
		mov     eax, DVNDLO
		sbb     edx, ecx                    // EDX:EAX = remainder
		jae     negate                      // if above or equal we're ok, else add
		add     eax, DVSRLO                 // add divisor to remainder
		adc     edx, DVSRHI

	negate:
		//
		// Now check the result sign flag to see if the result is supposed to be positive
		// or negative.  It is currently negated (because we subtracted in the 'wrong'
		// direction), so if the sign flag is set we are done, otherwise we must negate
		// the result to make it positive again.
		//

		xor     edx, edi                    // if EDI == -1, negate the remainder
		add     eax, edi
		sbb     edx, edi
		xor     eax, edi

		//
		// Just the cleanup left to do.  EDX:EAX contains the quotient.
		// Restore the saved registers and return.
		//

		pop     edi
		pop     esi
		pop     ebp
		pop     ebx

		ret     16

		#undef DVNDLO
		#undef DVNDHI
		#undef DVSRLO
		#undef DVSRHI
	}
#endif
}
