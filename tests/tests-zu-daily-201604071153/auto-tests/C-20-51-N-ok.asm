segment	.data
align	4
global	x:object
x:
segment	.rodata
align	4
_L1:
	db	"ole", 0
segment	.data
	dd	_L1
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
	push	dword $x
	pop	eax
	push	dword [eax]
	call	prints
	add	esp, 4
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
