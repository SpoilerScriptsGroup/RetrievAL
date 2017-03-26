.486
.model flat

public @TDrawGrid_SetRowCount@8

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_SetRowCount(TDrawGrid *DrawGrid, const int RowCount)
;{
;	DrawGrid->RowCount = RowCount;
;}
@TDrawGrid_SetRowCount@8 proc near

	mov     eax, ecx
	mov     ecx, 005592B4H
	jmp     ecx

@TDrawGrid_SetRowCount@8 endp

end
