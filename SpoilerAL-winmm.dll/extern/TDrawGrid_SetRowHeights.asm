.486
.model flat

public @TDrawGrid_SetRowHeights@12

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_SetRowHeights(TDrawGrid *DrawGrid, const int Index, const int RowHeights)
;{
;	DrawGrid->RowHeights[Index] = RowHeights;
;}
@TDrawGrid_SetRowHeights@12 proc near

	mov     eax, dword ptr [esp]
	push    ecx
	mov     ecx, dword ptr [esp + 8]
	mov     dword ptr [esp + 4], 005592F0H
	mov     dword ptr [esp + 8], eax
	pop     eax
	ret

@TDrawGrid_SetRowHeights@12 endp

end
