	.file	"sum.c"
	.text
	.local	g_lehmer64_state
	.comm	g_lehmer64_state,16,16
	.type	init_seed, @function
init_seed:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	pushq	%r12
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	movl	$0, %edi
	call	time@PLT
	movl	%eax, %edi
	call	srand@PLT
	call	rand@PLT
	cltq
	movq	%rax, %r12
	sarq	$63, %rax
	movq	%rax, %r13
	movq	%r12, g_lehmer64_state(%rip)
	movq	%r13, 8+g_lehmer64_state(%rip)
	nop
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	init_seed, .-init_seed
	.type	lehmer64, @function
lehmer64:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	g_lehmer64_state(%rip), %rax
	movq	8+g_lehmer64_state(%rip), %rdx
	movabsq	$-2696494805208442699, %rcx
	movq	%rcx, %rsi
	imulq	%rdx, %rsi
	imulq	$0, %rax, %rcx
	addq	%rcx, %rsi
	movabsq	$-2696494805208442699, %rcx
	mulq	%rcx
	leaq	(%rsi,%rdx), %rcx
	movq	%rcx, %rdx
	movq	%rax, g_lehmer64_state(%rip)
	movq	%rdx, 8+g_lehmer64_state(%rip)
	movq	g_lehmer64_state(%rip), %rax
	movq	8+g_lehmer64_state(%rip), %rdx
	movq	%rdx, %rax
	xorl	%edx, %edx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	lehmer64, .-lehmer64
	.type	bandwith, @function
bandwith:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2sdq	-8(%rbp), %xmm0
	pxor	%xmm1, %xmm1
	cvtsi2sdq	-16(%rbp), %xmm1
	movsd	.LC0(%rip), %xmm2
	divsd	%xmm2, %xmm1
	divsd	%xmm1, %xmm0
	movq	%xmm0, %rax
	movq	%rax, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	bandwith, .-bandwith
	.section	.rodata
.LC1:
	.string	"%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$1000000000, -56(%rbp)
	movq	-56(%rbp), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -48(%rbp)
	movq	-56(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-48(%rbp), %rax
	movl	$7, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	$0, -40(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	leaq	-32(%rbp), %rax
	movl	$0, %ecx
	movl	$0, %edx
	movq	%rax, %rsi
	leaq	main._omp_fn.0(%rip), %rax
	movq	%rax, %rdi
	call	GOMP_parallel@PLT
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.type	main._omp_fn.0, @function
main._omp_fn.0:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$48, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -32(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rbx
	testq	%rbx, %rbx
	je	.L15
	call	omp_get_num_threads@PLT
	movslq	%eax, %r12
	call	omp_get_thread_num@PLT
	movslq	%eax, %rsi
	movq	%rbx, %rax
	movl	$0, %edx
	divq	%r12
	movq	%rax, %rcx
	movq	%rbx, %rax
	movl	$0, %edx
	divq	%r12
	movq	%rdx, %rax
	cmpq	%rax, %rsi
	jb	.L11
.L14:
	imulq	%rcx, %rsi
	movq	%rsi, %rdx
	addq	%rdx, %rax
	leaq	(%rax,%rcx), %rdx
	cmpq	%rdx, %rax
	jnb	.L15
	movq	%rax, -40(%rbp)
.L13:
	movq	-40(%rbp), %rax
	leaq	0(,%rax,4), %rcx
	movq	-32(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	movslq	%eax, %rcx
	movq	-56(%rbp), %rax
	movq	16(%rax), %rax
	addq	%rax, %rcx
	movq	-56(%rbp), %rax
	movq	%rcx, 16(%rax)
	addq	$1, -40(%rbp)
	cmpq	%rdx, -40(%rbp)
	jb	.L13
	jmp	.L15
.L11:
	movl	$0, %eax
	addq	$1, %rcx
	jmp	.L14
.L15:
	nop
	addq	$48, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main._omp_fn.0, .-main._omp_fn.0
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
