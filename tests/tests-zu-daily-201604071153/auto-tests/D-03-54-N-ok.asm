segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 0
	pop	eax
	mov	[ebp-4], eax
	push	dword 0
	pop	eax
	cmp	eax, byte 0
	je	near _L1
segment	.rodata
align	4
_L2:
	db	"KO", 0
segment	.text
	push	dword $_L2
	call	prints
	add	esp, 4
align	4
_L1:
	push	dword 0
	pop	eax
	cmp	eax, byte 0
	je	near _L3
segment	.rodata
align	4
_L4:
	db	"elif1", 0
segment	.text
	push	dword $_L4
	call	prints
	add	esp, 4
align	4
_L3:
	push	dword 0
	pop	eax
	cmp	eax, byte 0
	je	near _L5
segment	.rodata
align	4
_L6:
	db	"elif2", 0
segment	.text
	push	dword $_L6
	call	prints
	add	esp, 4
align	4
_L5:
	push	dword 0
	pop	eax
	cmp	eax, byte 0
	je	near _L7
segment	.rodata
align	4
_L8:
	db	"elif3", 0
segment	.text
	push	dword $_L8
	call	prints
	add	esp, 4
align	4
_L7:
	push	dword 0
	pop	eax
	cmp	eax, byte 0
	je	near _L10
segment	.rodata
align	4
_L11:
	db	"elif4", 0
segment	.text
	push	dword $_L11
	call	prints
	add	esp, 4
	jmp	dword _L9
align	4
_L10:
segment	.rodata
align	4
_L12:
	db	"OK", 0
segment	.text
	push	dword $_L12
	call	prints
	add	esp, 4
align	4
_L9:
	push	dword [ebp-4]
	pop	eax
	leave
	ret
extern	readi
extern	readd
extern	printi
extern	printd
extern	prints
extern	println
extern	argc
extern	argv
extern	envp
