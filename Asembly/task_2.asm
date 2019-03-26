
; Read a symbol from cmd and print :
;	1 if letter
;	2 if num
;	3 else


main:
	mov ah, 01h
	int 21h	; read 
	
	mov dl, al ; assing al's val to dl ?
	
	mov ah, 02h
	je dl, 48, num
	je dl, 
	
	other:
	move al, 3
	jmp exit
	
	letter:
	mov al, 2
	jmp exit
	
	num:
	mov al, 1
	
	exit:
	int 21h
end main

