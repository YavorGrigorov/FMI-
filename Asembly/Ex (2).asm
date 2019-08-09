
; ~(a | b) & (a ^ c) ^ c
.data 
	a 	db 1
	b 	db 0
	c 	db 1
	res db 0
.code
main:

	;mov res,a 	; res = a
	;OR 	res, b 	; a or b
	;NOT res
	;XOR a, c	; a = a xor c ??
	;AND res, a	;
	;XOR res, c	;
	
	;orr..:
	
	mov al, a
	mov dl, b
	OR al, dl
	NOT al
	
	mov res, al  ; res = a | b
	
	mov al, a
	mov dl, c
	XOR al, dl
	
	mov dl, res
	AND al, dl
	
	mov dl, c
	XOR al, dl
	
	mov res, al

end main ;profit?