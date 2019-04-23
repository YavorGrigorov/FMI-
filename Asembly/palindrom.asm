	
	; read a 5 char string and see if it's palindrom
	; read word alfa, space, beta and return if equal
	; read 10 digits and see if they are ordered accending // if that's how it's spelled
	.stack 5
	
	.data
		s db 5 ; define 5 bytes?
	
	.code

	main:
		mov cx, 5	;cx = 5
		xor bx, bx;
		xor ax, ax;
		
		read:
		mov ah, 1	; some light reading
		int 21h		;
		
		; all the checks
		cmp al, 13	; is enter
		je mainPart	; skip
		cmp al, 9	; in tab
		je mainPart	; skip
		cmp al, 32 	; is space
		je mainPart	; skip
		
		push ax		; push into stack
		
		mov bx, cx	; bx, the offset man
		mov s[bx], al	; some light writing
		
		loop read	;
		
		mainPart:
		mov ah, 5	;
		sub ah, cx	; 5 - cx = strlen
		mov al, 0	; 
		mov cx, ax	; cx is 5
		mov bx, 5	; bx, offsets again
		;s contains the word written backwards
		;	e.g. "abc" in s is "cba"
		
		strcmp:
		pop ax	; get the last char
		
		cmp al, s[bx]	;the cmp
		dec bx	; --bx, 
		loop strcmp
		
		mov al, 0; true
		jmp theEnd
		
		allahuAkkbar:
		mov al, 1; false
		
		theEnd:
		mov ah, 2;
		int 21h	; print
		
	end main
	
	
	
	
