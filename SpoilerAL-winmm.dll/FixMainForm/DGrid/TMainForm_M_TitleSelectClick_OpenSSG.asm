.486
.model flat, c

public TMainForm_M_TitleSelectClick_OpenSSG

.code

align 16

;void __cdecl TMainForm_M_TitleSelectClick_OpenSSG(TSSGCtrl &ssgCtrl, TSSGScriptStruct &selectScript, int &nowSelectSubjectIndex)
;{
;	nowSelectSubjectIndex = -2;
;	ssgCtrl.OpenSSG(&selectScript);
;}
TMainForm_M_TitleSelectClick_OpenSSG proc near

	mov     ecx, 004FCE80H
	mov     dword ptr [ebx + 554H], -2
	jmp     ecx

TMainForm_M_TitleSelectClick_OpenSSG endp

end
