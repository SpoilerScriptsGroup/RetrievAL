.486
.model flat

public @_TDrawGrid_MouseToCell@20

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_MouseToCell(TDrawGrid *DrawGrid, int X, int Y, int &ACol, int &ARow)
;{
;	DrawGrid->MouseToCell(X, Y, ACol, ARow);
;}
@_TDrawGrid_MouseToCell@20 proc near

	mov     eax, dword ptr [esp]
	push    ecx
	mov     ecx, dword ptr [esp + 8]
	mov     dword ptr [esp + 4], 00559DB8H
	mov     dword ptr [esp + 8], eax
	pop     eax
	ret

@_TDrawGrid_MouseToCell@20 endp

end
