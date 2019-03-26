;	read 3 symbols from the cmd line
;	and print them in reverse

MODEL small
.stack 8

main:
	mov cx, 3
	
	reader:
	mov ah, 01h		;
	int 21h			; read and puts in ah??
	push ax         ;
	loop reader     ;
                    ;
	mov cx, 3       ;
	                ;
	printer:        ;
	mov ah, 02h     ; 02h -> print a symbol
	pop dx          ; pops from the stack and puts it in dx // 2 bytes
	int 21h         ; looks at dh?
	loop printer    ;

end main
