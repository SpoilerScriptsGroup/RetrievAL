.486
.model flat

public @TDrawGrid_GetRowHeights@8

.code

align 16

;EXTERN_C int __msfastcall TDrawGrid_GetRowHeights(TDrawGrid *DrawGrid, int Index)
;{
;	return DrawGrid->RowHeights[Index];
;}
@TDrawGrid_GetRowHeights@8 proc near

	mov     eax, ecx
	mov     ecx, 00558E78H
	jmp     ecx

@TDrawGrid_GetRowHeights@8 endp

end
