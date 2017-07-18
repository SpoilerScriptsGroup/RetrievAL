.486
.model flat, c

includelib kernel32.lib

extrn _imp__HeapAlloc@12:dword
extrn _imp__HeapFree@12:dword
extrn _imp__WideCharToMultiByte@32:dword

CP_THREAD_ACP equ 3

extrn hHeap:dword

public AddressNamingFromUnicode@8

.code

align 16

AddressNamingFromUnicode@8 proc near

	DataSize equ <esp + 4>
	tmpC     equ <esp + 8>

	push    esi
	push    edi
	mov     eax, dword ptr [hHeap]
	xor     ecx, ecx
	DataSize equ <esp + 4 * (1 + 2)>
	tmpC     equ <esp + 4 * (2 + 2)>
	mov     edi, dword ptr [DataSize]                   ; DWORD dwBytes = DataSize + 2;
	mov     esi, dword ptr [tmpC]
	push    ecx
	push    ecx
	push    eax
	push    ecx
	inc     edi
	push    ecx
	push    edi
	push    esi
	push    -1
	push    ecx
	push    ecx
	inc     edi
	push    CP_THREAD_ACP
	push    edi                                         ; LPWSTR lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, dwBytes);
	push    ecx
	push    eax
	call    dword ptr [_imp__HeapAlloc@12]
	test    eax, eax                                    ; if (lpWideCharStr != NULL)
	jz      short L1                                    ; {
	mov     ecx, edi                                    ;     DWORD dwCount = (dwBytes >> 1) - 1;
	mov     edi, eax                                    ;     __movsw((unsigned short *)lpWideCharStr, (const unsigned short *)tmpC, dwCount);
	shr     ecx, 1
	mov     dword ptr [esp + 8], eax
	dec     ecx
	mov     dword ptr [esp + 40], eax
	rep movsw
	mov     word ptr [edi], cx                          ;     lpWideCharStr[dwCount] = L'\0';
	call    dword ptr [_imp__WideCharToMultiByte@32]    ;     WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, tmpC, DataSize + 1, NULL, NULL);
	call    dword ptr [_imp__HeapFree@12]               ;     HeapFree(hHeap, 0, lpWideCharStr);
	jmp     short L2                                    ; }
	                                                    ; else
	                                                    ; {
L1:
	add     esp, 44
	mov     byte ptr [esi], al                          ;     *tmpC = '\0';
	                                                    ; }
L2:
	pop     edi
	pop     esi
	ret     8

AddressNamingFromUnicode@8 endp

end
