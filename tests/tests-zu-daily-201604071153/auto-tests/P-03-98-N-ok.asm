segment	.bss
align	4
global	f:object
f:
	resb	4
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 28
	push	dword 0
	pop	eax
	mov	[ebp-4], eax
	push	dword 4
	push	dword [esp]
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword 0
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	sub	esp, eax
	push	esp
	push	dword [esp]
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-16]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L1:
	lea	eax, [ebp+-16]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L3
	push	dword 2
	push	dword [esp]
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-16]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
align	4
_L2:
	lea	eax, [ebp+-16]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-16]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L1
align	4
_L3:
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-20]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L4:
	lea	eax, [ebp+-20]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L6
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-20]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
align	4
_L5:
	lea	eax, [ebp+-20]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-20]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L4
align	4
_L6:
segment	.rodata
align	4
_L7:
	db	10, 0
segment	.text
	push	dword $_L7
	call	prints
	add	esp, 4
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-24]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L8:
	lea	eax, [ebp+-24]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L10
	push	dword 6
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-24]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	eax
	push	dword [eax]
	pop	ecx
	pop	eax
	cdq
	idiv	ecx
	push	eax
	push	dword [esp]
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-24]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
align	4
_L9:
	lea	eax, [ebp+-24]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-24]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L8
align	4
_L10:
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-28]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L11:
	lea	eax, [ebp+-28]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L13
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+-28]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
align	4
_L12:
	lea	eax, [ebp+-28]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-28]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L11
align	4
_L13:
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
