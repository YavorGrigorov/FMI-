Карх

for(int i = 0; i < 10; ++i)
	std::cout << 'A';
	
	
-> asm

mov cx, 10				// set cnt reg
						//
go:						//
	mov ah, 02h         // set ah's value with 02h
	mov dl, 'A' 65      // -//- dl's value with 'A'
	int 21h             // print al on screen 
						//
loop go                 //


pseudo:

i = 10
Start:
	print A
	i = i - 1
	if i == 0
		goto Start



////////////////////////////////////////////////


if(a >= b)
	std::cout << 'a';
else 
	std::cout << 'b';

	
-> asm

cmp a, b			
mov ah, 02h

//first vers
jge isA
jmp isB

isA:
	mov dl, 'a'
	jmp exit
isB:
	mov dl, 'b'
exit: 
	int 21h
	mov 

//second vers

jl isB

isA:
	mov dl, 'a'
	jmp exit
isB:
	mov dl, 'b'
exit: 
	int 21h
	mov 4c00h
	int 21h
	
	
//////////////////////////////////////////////


Adressing :

Sev parts:
	Base	 -> BX 		// Base reg - idx adressing reg
				BP 		// Base ptr 
	Idx 	 -> SI 		// Source idx		
				DI 		// Destination idx 	
	Offset	 ->	offset	// 

Example :
	mov bx, 3
	[bx+4] 		//go to 4 bytes from bx
	[bx+si+3]	//
	// rest are in the pdf
	