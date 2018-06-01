
test:     file format elf32-littlearm


Disassembly of section .init:

000082e0 <_init>:
    82e0:	e92d4010 	push	{r4, lr}
    82e4:	eb00001c 	bl	835c <call_gmon_start>
    82e8:	e8bd8010 	pop	{r4, pc}

Disassembly of section .plt:

000082ec <.plt>:
    82ec:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    82f0:	e59fe004 	ldr	lr, [pc, #4]	; 82fc <_init+0x1c>
    82f4:	e08fe00e 	add	lr, pc, lr
    82f8:	e5bef008 	ldr	pc, [lr, #8]!
    82fc:	000082c4 	.word	0x000082c4
    8300:	e28fc600 	add	ip, pc, #0	; 0x0
    8304:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8308:	e5bcf2c4 	ldr	pc, [ip, #708]!
    830c:	e28fc600 	add	ip, pc, #0	; 0x0
    8310:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8314:	e5bcf2bc 	ldr	pc, [ip, #700]!
    8318:	e28fc600 	add	ip, pc, #0	; 0x0
    831c:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8320:	e5bcf2b4 	ldr	pc, [ip, #692]!

Disassembly of section .text:

00008324 <_start>:
    8324:	e59fc024 	ldr	ip, [pc, #36]	; 8350 <_start+0x2c>
    8328:	e3a0b000 	mov	fp, #0	; 0x0
    832c:	e49d1004 	pop	{r1}		; (ldr r1, [sp], #4)
    8330:	e1a0200d 	mov	r2, sp
    8334:	e52d2004 	push	{r2}		; (str r2, [sp, #-4]!)
    8338:	e52d0004 	push	{r0}		; (str r0, [sp, #-4]!)
    833c:	e59f0010 	ldr	r0, [pc, #16]	; 8354 <_start+0x30>
    8340:	e59f3010 	ldr	r3, [pc, #16]	; 8358 <_start+0x34>
    8344:	e52dc004 	push	{ip}		; (str ip, [sp, #-4]!)
    8348:	ebffffef 	bl	830c <_init+0x2c>
    834c:	ebffffeb 	bl	8300 <_init+0x20>
    8350:	000083ec 	.word	0x000083ec
    8354:	000083d8 	.word	0x000083d8
    8358:	000083f0 	.word	0x000083f0

0000835c <call_gmon_start>:
    835c:	e59f3014 	ldr	r3, [pc, #20]	; 8378 <call_gmon_start+0x1c>
    8360:	e59f2014 	ldr	r2, [pc, #20]	; 837c <call_gmon_start+0x20>
    8364:	e08f3003 	add	r3, pc, r3
    8368:	e7931002 	ldr	r1, [r3, r2]
    836c:	e3510000 	cmp	r1, #0	; 0x0
    8370:	012fff1e 	bxeq	lr
    8374:	eaffffe7 	b	8318 <_init+0x38>
    8378:	00008254 	.word	0x00008254
    837c:	00000018 	.word	0x00000018

00008380 <__do_global_dtors_aux>:
    8380:	e59f2010 	ldr	r2, [pc, #16]	; 8398 <__do_global_dtors_aux+0x18>
    8384:	e5d23000 	ldrb	r3, [r2]
    8388:	e3530000 	cmp	r3, #0	; 0x0
    838c:	03a03001 	moveq	r3, #1	; 0x1
    8390:	05c23000 	strbeq	r3, [r2]
    8394:	e12fff1e 	bx	lr
    8398:	000105e4 	.word	0x000105e4

0000839c <frame_dummy>:
    839c:	e59f0020 	ldr	r0, [pc, #32]	; 83c4 <frame_dummy+0x28>
    83a0:	e92d4010 	push	{r4, lr}
    83a4:	e5903000 	ldr	r3, [r0]
    83a8:	e3530000 	cmp	r3, #0	; 0x0
    83ac:	08bd8010 	popeq	{r4, pc}
    83b0:	e59f3010 	ldr	r3, [pc, #16]	; 83c8 <frame_dummy+0x2c>
    83b4:	e3530000 	cmp	r3, #0	; 0x0
    83b8:	08bd8010 	popeq	{r4, pc}
    83bc:	e12fff33 	blx	r3
    83c0:	e8bd8010 	pop	{r4, pc}
    83c4:	000104cc 	.word	0x000104cc
    83c8:	00000000 	.word	0x00000000

000083cc <test1>:
	int a[10];
	int i;

	for (i = 0; i < 10; i++)
		a[i] = i;
}
    83cc:	e12fff1e 	bx	lr

000083d0 <test2>:
	int *p = b;
	for (i = 0; i < 10; i++)
		*p++ = i;

	return 0;
}
    83d0:	e3a00000 	mov	r0, #0	; 0x0
    83d4:	e12fff1e 	bx	lr

000083d8 <main>:

int main(int argc, char **argv)
{
    83d8:	e92d4010 	push	{r4, lr}
	test1();
    83dc:	ebfffffa 	bl	83cc <test1>
	test2();
    83e0:	ebfffffa 	bl	83d0 <test2>
	//entry_a(NULL);

	return 0;
}
    83e4:	e3a00000 	mov	r0, #0	; 0x0
    83e8:	e8bd8010 	pop	{r4, pc}

000083ec <__libc_csu_fini>:
    83ec:	e12fff1e 	bx	lr

000083f0 <__libc_csu_init>:
    83f0:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    83f4:	e1a08001 	mov	r8, r1
    83f8:	e1a07002 	mov	r7, r2
    83fc:	e1a0a000 	mov	sl, r0
    8400:	ebffffb6 	bl	82e0 <_init>
    8404:	e59f1044 	ldr	r1, [pc, #68]	; 8450 <__libc_csu_init+0x60>
    8408:	e59f3044 	ldr	r3, [pc, #68]	; 8454 <__libc_csu_init+0x64>
    840c:	e59f2044 	ldr	r2, [pc, #68]	; 8458 <__libc_csu_init+0x68>
    8410:	e0613003 	rsb	r3, r1, r3
    8414:	e08f2002 	add	r2, pc, r2
    8418:	e1b05143 	asrs	r5, r3, #2
    841c:	e0822001 	add	r2, r2, r1
    8420:	08bd87f0 	popeq	{r4, r5, r6, r7, r8, r9, sl, pc}
    8424:	e1a06002 	mov	r6, r2
    8428:	e3a04000 	mov	r4, #0	; 0x0
    842c:	e1a0000a 	mov	r0, sl
    8430:	e1a01008 	mov	r1, r8
    8434:	e1a02007 	mov	r2, r7
    8438:	e1a0e00f 	mov	lr, pc
    843c:	e796f104 	ldr	pc, [r6, r4, lsl #2]
    8440:	e2844001 	add	r4, r4, #1	; 0x1
    8444:	e1540005 	cmp	r4, r5
    8448:	3afffff7 	bcc	842c <__libc_csu_init+0x3c>
    844c:	e8bd87f0 	pop	{r4, r5, r6, r7, r8, r9, sl, pc}
    8450:	ffffff04 	.word	0xffffff04
    8454:	ffffff08 	.word	0xffffff08
    8458:	000081a4 	.word	0x000081a4

Disassembly of section .fini:

0000845c <_fini>:
    845c:	e92d4010 	push	{r4, lr}
    8460:	e8bd8010 	pop	{r4, pc}
