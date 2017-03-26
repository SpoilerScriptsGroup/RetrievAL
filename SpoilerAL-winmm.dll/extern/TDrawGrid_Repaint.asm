.486
.model flat

public @TDrawGrid_Repaint@4

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_Repaint(TDrawGrid *DrawGrid)
;{
;	DrawGrid->Repaint();
;}
@TDrawGrid_Repaint@4 proc near

	mov     eax, ecx
	mov     ecx, dword ptr [ecx]
	jmp     dword ptr [ecx + 80H]

@TDrawGrid_Repaint@4 endp

end
