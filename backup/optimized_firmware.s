	@ CMP AND BRANCHES??
  
  .cpu arm10tdmi
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r0, [r1, #4]				sub	sp, sp, #84
	mov	r4, r1							ldr	r1, .L107
	bl	fopen								subs	fp, r0, #0
	beq	.L98                NOP 
	mov	r3, #0             	mov	r2, fp
	add	r0, sp, #76         add	r1, sp, #72
	str	r3, [sp, #72]       NOP 
	str	r3, [sp, #76]       NOP 
	bl	getline             ldr	r2, [sp, #76]
	ldrb	r3, [r2, #0]	    NOP                 @ zero_extendqisi2
	cmp	r3, #80             NOP
	bne	.L3                 ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #54             beq	.L4

.L3:
	ldr	r2, .L107+4         ldr	r0, .L107+8
	ldr	r3, [r2, #0]    	  mov	r1, #1
	mov	r2, #36             bl	fwrite
	mov	r0, #1              bl	exit
.L4:
	mov	r0, fp              ldr	r1, .L107+12
	add	r2, sp, #64         add	r3, sp, #68
	bl	fscanf              cmp	r0, #2
	bne	.L99                ldr	r1, [sp, #68]
	ldr	r2, [sp, #64]       ldr	r0, .L107+16
	bl	printf              mov	r0, fp
	ldr	r1, .L107+20        add	r2, sp, #60
	bl	fscanf              cmp	r0, #1
	mov	r5, r0              bne	.L100
	ldr	r1, [sp, #60]       NOP
	cmp	r1, #255            ldreq	r0, .L107+24
	beq	.L94                b	.L106
.L102:
	ldr	r0, .L107+28        NOP
.L94:
	bl	printf             	mov	r0, fp
	bl	fgetc               and	r1, r0, #255
	cmp	r1, #10             bne	.L102
	ldr	r3, [sp, #68]       ldr	r2, [sp, #64]
	mov	r4, r3, asl #2      NOP
	mov	r1, r3, asr #1      NOP
	str	r1, [sp, #36]       bic	r3, r3, #1
	mov	r1, r2, asr #1      mov	r0, r4
	mov	r6, r2, asl #2      str	r1, [sp, #40]
	str	r3, [sp, #32]       bl	malloc
	str	r0, [sp, #8]        NOP
	mov	r0, r4              bl	malloc
	str	r0, [sp, #12]       NOP
	mov	r0, r4              bl	malloc
	str	r0, [sp, #16]       NOP
	mov	r0, r4              bl	malloc
	mov	r5, r4, asr #1      str	r0, [sp, #20]
	mov	r0, r5              bl	malloc
	str	r0, [sp, #24]       NOP
	mov	r0, r5              bl	malloc
	ldr	r2, [sp, #32]       str	r0, [sp, #28]
	cmp	r2, #0              movle	r9, #0
	ble	.L11                mov	r9, #0
	mov	r3, r6, asr #1      ldr	sl, [sp, #8]
	ldr	r8, [sp, #12]       ldr	r7, [sp, #16]
	ldr	r5, [sp, #20]       str	r3, [sp, #44]
	mov	r4, r9              NOP
.L12:
	mov	r0, r6              bl	malloc
	str	r0, [sl, #0]        NOP
	mov	r0, r6            	bl	malloc
	str	r0, [r8, #0]        NOP
	mov	r0, r6              bl	malloc
	str	r0, [r7, #0]        NOP
	mov	r0, r6              bl	malloc
	str	r0, [r5, #0]        NOP
	ldr	r0, [sp, #44]       bl	malloc
	ldr	r1, [sp, #24]       add	r9, r9, #2
	str	r0, [r1, r4]        NOP
	ldr	r0, [sp, #44]     	bl	malloc
	ldr	r2, [sp, #28]       NOP
	str	r0, [r2, r4]        NOP
	mov	r0, r6            	bl	malloc
	str	r0, [sl, #4]        NOP
	mov	r0, r6              bl	malloc
	str	r0, [r8, #4]        NOP
	mov	r0, r6              bl	malloc
	str	r0, [r7, #4]        NOP
	mov	r0, r6              bl	malloc
	ldr	r3, [sp, #32]       str	r0, [r5, #4]
	cmp	r9, r3              add	sl, sl, #8
	add	r8, r8, #8          add	r7, r7, #8
	add	r5, r5, #8        	add	r4, r4, #4
	blt	.L12
.L11:
	ldr	ip, [sp, #68]       NOP
	tst	ip, #1            	bne	.L103
.L13:
	ldr	r1, [sp, #64]       cmp	ip, #0
	bic	r3, r1, #3          movgt	r8, #0
	sub	r7, r3, #2          movgt	r6, r8
	ble	.L14                NOP
.L15:
	cmp	r7, #0              movle	r5, #0
	ble	.L30                mov	r5, #0
.L16:
	ldr	r3, [sp, #8]      	add	ip, r5, #2
	ldr	r2, [r3, r6]        add	r4, r5, #3
	add	r3, r5, #1          add	ip, r2, ip
	add	r3, r2, r3          NOP
	add	r4, r2, r4          NOP
	mov	r0, fp              add	r2, r2, r5
	ldr	r1, .L107+32        add	r5, r5, #4
	str	ip, [sp, #0]        str	r4, [sp, #4]
	bl	fscanf              cmp	r7, r5
	bgt	.L16                ldr	r1, [sp, #64]
.L30:
	cmp	r5, r1              bge	.L17
.L72:
	ldr	r1, [sp, #8]        mov	r0, fp
	ldr	r2, [r1, r6]        NOP
	ldr	r1, .L107+20        add	r2, r2, r5
	bl	fscanf              ldr	r1, [sp, #64]
	add	r5, r5, #1          NOP
	cmp	r1, r5            	bgt	.L72
.L17:
	cmp	r7, #0              movle	r5, #0
	ble	.L20                mov	r5, #0
.L21:
	ldr	r3, [sp, #12]       add	ip, r5, #2
	ldr	r2, [r3, r6]        add	r4, r5, #3
	add	r3, r5, #1          add	ip, r2, ip
	add	r3, r2, r3          NOP
	add	r4, r2, r4          mov	r0, fp
	add	r2, r2, r5          ldr	r1, .L107+32
	add	r5, r5, #4          str	ip, [sp, #0]
	str	r4, [sp, #4]        bl	fscanf
	cmp	r7, r5              bgt	.L21
	ldr	r1, [sp, #64]       NOP
.L20:
	cmp	r5, r1              bge	.L22
.L71:
	ldr	r1, [sp, #12]       mov	r0, fp
	ldr	r2, [r1, r6]        NOP
	ldr	r1, .L107+20        add	r2, r2, r5
	bl	fscanf              ldr	r1, [sp, #64]
	add	r5, r5, #1          NOP
	cmp	r1, r5              bgt	.L71
.L22:
	cmp	r7, #0              movle	r5, #0
	ble	.L25                mov	r5, #0
.L26:
	ldr	r3, [sp, #16]       add	ip, r5, #2
	ldr	r2, [r3, r6]        add	r4, r5, #3
	add	r3, r5, #1          add	ip, r2, ip
	add	r3, r2, r3          NOP
	add	r4, r2, r4          NOP
	mov	r0, fp              add	r2, r2, r5
	ldr	r1, .L107+32        add	r5, r5, #4
	str	ip, [sp, #0]        NOP
	str	r4, [sp, #4]        bl	fscanf
	cmp	r7, r5              bgt	.L26
	ldr	r1, [sp, #64]       NOP
.L25:
	cmp	r5, r1              bge	.L27
.L70:
	ldr	r1, [sp, #16]       mov	r0, fp
	ldr	r2, [r1, r6]        ldr	r1, .L107+20
	add	r2, r2, r5          bl	fscanf
	ldr	r1, [sp, #64]       add	r5, r5, #1
	cmp	r1, r5              bgt	.L70
.L27:
	ldr	ip, [sp, #68]       add	r8, r8, #1
	cmp	ip, r8              add	r6, r6, #4
	bgt	.L15                NOP
.L14:
	ldr	r2, [sp, #32]       bic	r3, r1, #7
	sub	r3, r3, #6          cmp	r2, #0
	str	r3, [sp, #52]       mov	r2, #66
	mvn	r3, #126            vdup.8	d31, r2
	vdup.8	d30, r3         mov	r2, #25
	mov	r3, #38             vdup.8	d29, r2
	vdup.8	d2, r3          mov	r2, #75
	mov	r3, #112            vdup.8	d3, r2
	vdup.8	d6, r3          mov	r2, #94
	mov	r3, #18             vdup.8	d4, r2
	vdup.8	d5, r3          mov	r2, #16
	mvn	r3, #127            vdup.8	d28, r2
	vdup.8	d7, r3          movle	r6, #0
	ble	.L42                mov	r3, #0
	ldr	sl, [sp, #8]        NOP
	ldr	r8, [sp, #12]       NOP
	ldr	r7, [sp, #16]       NOP
	ldr	r9, [sp, #20]       NOP
	str	r3, [sp, #48]       mov	r6, r3
.L32:
	ldr	r2, [sp, #52]       NOP
	cmp	r2, #0              NOP
	movle	r5, #0            NOP
	movle	r4, r5            NOP
	ble	.L95                mov	r4, #0
	mov	r5, r4              NOP
.L33:
	ldr	r3, [sl, #0]        ldr	r2, [r8, #0]
	ldr	r1, [r7, #0]        add	r3, r3, r4
	add	r2, r2, r4          vld1.8	{d20}, [r3]
	vld1.8	{d24}, [r2]     add	r1, r1, r4
	vld1.8	{d26}, [r1]     vmull.u8	q11, d20, d31
	vmull.u8	q8, d24, d30  NOP
	vadd.i16	q8, q8, q11   vmull.u8	q9, d26, d29
	ldr	r3, [r9, #0]        NOP
	vadd.i16	q9, q9, q8    NOP
	vshrn.i16	d18, q9, #8   vmull.u8	q8, d20, d2
	add	r3, r3, r4          vmull.u8	q11, d26, d6
	vadd.i8	d18, d18, d28   NOP
	vst1.8	{d18}, [r3]     NOP
	vsub.i16	q11, q11, q8  NOP
	vmull.u8	q8, d24, d3   ldr	r1, [sp, #24]
	vsub.i16	q11, q11, q8  NOP
	vshrn.i16	d22, q11, #8  NOP
	ldr	r2, [r1, r6]        vadd.i8	d22, d22, d7
	vmov.u8	r3, d22[0]      vmull.u8	q10, d20, d6
	vmull.u8	q12, d24, d4  strb	r3, [r2, r5]
	vsub.i16	q10, q10, q12 vmull.u8	q13, d26, d5
	ldr	r3, [sp, #28]       vsub.i16	q10, q10, q13
	vshrn.i16	d20, q10, #8  ldr	r2, [r3, r6]
	vadd.i8	d20, d20, d7    NOP
	vmov.u8	r3, d20[0]      NOP
  strb	r3, [r2, r5]      NOP
	ldr	r2, [sp, #24]       add	r0, r5, #1
	ldr	r1, [r2, r6]        vmov.u8	r3, d22[2]
	strb	r3, [r1, r0]      NOP
	ldr	r3, [sp, #28]       add	r4, r4, #8
	ldr	r2, [r3, r6]        NOP
	vmov.u8	r3, d20[2]      NOP
	strb	r3, [r2, r0]      NOP
	ldr	r2, [sp, #24]       add	r0, r5, #2
	ldr	r1, [r2, r6]        vmov.u8	r3, d22[4]
	strb	r3, [r1, r0]      NOP
	ldr	r3, [sp, #28]       NOP
	ldr	r2, [r3, r6]        NOP
	vmov.u8	r3, d20[4]      NOP
	strb	r3, [r2, r0]      NOP
	ldr	r2, [sp, #24]       add	r0, r5, #3
	ldr	r1, [r2, r6]        vmov.u8	r3, d22[6]
	strb	r3, [r1, r0]      NOP
	ldr	r3, [sp, #28]       ldr	r1, [sp, #52]
	ldr	r2, [r3, r6]        cmp	r1, r4
	vmov.u8	r3, d20[6]      NOP
	strb	r3, [r2, r0]      add	r5, r5, #4
	bgt	.L33                ldr	r1, [sp, #64]
	cmp	r1, r4              ble	.L104
.L69:
	ldr	r2, [r8, #0]        ldr	ip, [sl, #0]
	ldr	r3, [r7, #0]        ldrb	lr, [r2, r4]	@ zero_extendqisi2
	ldrb	r0, [r3, r4]	    NOP                 @ zero_extendqisi2
	ldrb	r1, [ip, r4]	    NOP                 @ zero_extendqisi2
	mov	r3, lr, asl #7      mov	ip, r1, asl #6
	mov	r2, r0, asl #5      mov	r1, r1, asl #1
	add	r3, r3, lr          add	r1, r1, ip
	sub	r2, r2, r0, asl #3  NOP
	add	r2, r2, r0          NOP
	add	r3, r3, r1          NOP
	add	r3, r3, r2          NOP
	mov	r3, r3, asr #8      ldr	r2, [r9, #0]
	add	r3, r3, #16         tst	r4, #1
	strb	r3, [r2, r4]      bne	.L35
	ldr	r3, [sl, #0]        ldr	r2, [r8, #0]
	ldrb	r1, [r3, r4]	    NOP                 @ zero_extendqisi2
	ldrb	ip, [r2, r4]	    NOP                	@ zero_extendqisi2
	ldr	r0, [r7, #0]        mov	r3, r1, asl #4
	mov	r2, r1, asl #2      ldrb	lr, [r0, r4]	@ zero_extendqisi2
	add	r2, r2, r3          NOP
	mov	r3, ip, asl #2      NOP
	add	r3, r3, ip          rsb	r1, r2, r1
	mov	r1, r1, asl #1      mov	r2, lr, asl #7
	sub	r3, r3, r3, asl #4  sub	r2, r2, lr, asl #4
	add	r3, r3, r1          ldr	r1, [sp, #24]
	add	r3, r3, r2          NOP
	mov	r3, r3, asr #8      ldr	r2, [r1, r6]
	sub	r3, r3, #128        NOP
	strb	r3, [r2, r5]    	ldr	r1, [r8, #0]
	ldr	r0, [sl, #0]        ldrb	r3, [r1, r4]	@ zero_extendqisi2
	ldr	ip, [r7, #0]        ldrb	lr, [r0, r4]	@ zero_extendqisi2
	mov	r2, r3, asl #6      ldrb	r0, [ip, r4]	@ zero_extendqisi2
	sub	r2, r2, r3, asl #4  NOP
	rsb	r3, r2, r3          mov	r1, lr, asl #7
	mov	r2, r0, asl #4      sub	r1, r1, lr, asl #4
	mov	r3, r3, asl #1      mov	r0, r0, asl #1
	add	r3, r3, r1          add	r0, r0, r2
	ldr	r1, [sp, #28]       rsb	r3, r0, r3
	mov	r3, r3, asr #8      ldr	r2, [r1, r6]
	sub	r3, r3, #128        NOP
	strb	r3, [r2, r5]      NOP
.L35:
	ldr	r1, [sp, #64]       add	r4, r4, #1
.L95:
	cmp	r1, r4              bgt	.L69
.L104:
	ldr	r2, [sp, #52]				NOP
	cmp	r2, #0              movle	r4, #0
	ble	.L96                mov	r0, #0
.L39:
	ldr	r3, [sl, #4]        ldr	r2, [r8, #4]
	ldr	r1, [r7, #4]        add	r3, r3, r0
	add	r2, r2, r0          vld1.8	{d20}, [r3]
	add	r1, r1, r0          vld1.8	{d18}, [r2]
	vld1.8	{d16}, [r1]     vmull.u8	q10, d20, d31
	vmull.u8	q9, d18, d30  ldr	r3, [r9, #4]
	vadd.i16	q9, q9, q10   vmull.u8	q8, d16, d29
	ldr	r1, [sp, #52]       vadd.i16	q8, q8, q9
	add	r3, r3, r0          vshrn.i16	d16, q8, #8
	add	r0, r0, #1          vadd.i8	d16, d16, d28
	cmp	r1, r0              vst1.8	{d16}, [r3]
	bgt	.L39                ldr	r1, [sp, #64]
	ldr	r4, [sp, #52]       NOP
	cmp	r1, r4              ble	.L105
.L68:
	ldr	r2, [r8, #4]        ldr	ip, [sl, #4]
	ldr	r3, [r7, #4]        ldrb	lr, [r2, r4]	@ zero_extendqisi2
	ldrb	r0, [r3, r4]			NOP									@ zero_extendqisi2
	ldrb	r1, [ip, r4]			NOP									@ zero_extendqisi2
	mov	r3, lr, asl #7      mov	ip, r1, asl #6
	mov	r2, r0, asl #5      mov	r1, r1, asl #1
	add	r1, r1, ip          add	r3, r3, lr
	sub	r2, r2, r0, asl #3  add	r3, r3, r1
	add	r2, r2, r0          NOP
	add	r3, r3, r2          NOP
	mov	r3, r3, asr #8      ldr	r2, [r9, #4]
	add	r3, r3, #16         NOP
	strb	r3, [r2, r4]      ldr	r1, [sp, #64]
	add	r4, r4, #1          NOP
.L96:
	cmp	r1, r4            	bgt	.L68
.L105:
	ldr	r2, [sp, #48]       ldr	r3, [sp, #32]
	add	r2, r2, #2          NOP
	cmp	r3, r2              NOP
	str	r2, [sp, #48]       add	sl, sl, #8
	add	r8, r8, #8          add	r7, r7, #8
	add	r9, r9, #8          add	r6, r6, #4
	bgt	.L32                sub	r3, r3, #2
	ldr	ip, [sp, #68]       add	r6, r3, #2
.L42:
	tst	ip, #1              beq	.L45
	ldr	r3, [sp, #52]       NOP
	cmp	r3, #0              movle	r4, #0
	ble	.L47                ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]        mov	r3, r6, asl #2
	add	r4, r1, r3          NOP
	add	r5, r2, r3          ldr	r1, [sp, #12]
	ldr	r2, [sp, #16]       add	lr, r1, r3
	add	ip, r2, r3          mov	r0, #0
.L48:
	ldr	r3, [r5, #0]        ldr	r2, [lr, #0]
	ldr	r1, [ip, #0]        add	r3, r3, r0
	add	r2, r2, r0          vld1.8	{d20}, [r3]
	add	r1, r1, r0          vld1.8	{d18}, [r2]
	vld1.8	{d16}, [r1]     vmull.u8	q10, d20, d31
	vmull.u8	q9, d18, d30  ldr	r3, [r4, #0]
	vadd.i16	q9, q9, q10   vmull.u8	q8, d16, d29
	ldr	r1, [sp, #52]       vadd.i16	q8, q8, q9
	add	r3, r3, r0          vshrn.i16	d16, q8, #8
	add	r0, r0, #1          vadd.i8	d16, d16, d28
	cmp	r1, r0              vst1.8	{d16}, [r3]
	bgt	.L48                ldr	r1, [sp, #64]
	ldr	r4, [sp, #52]       NOP
.L47:
	cmp	r1, r4              ble	.L45
	ldr	r2, [sp, #20]       ldr	r1, [sp, #8]
	mov	r3, r6, asl #2      NOP
	add	r8, r2, r3          NOP
	add	r7, r1, r3          ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]       add	r6, r2, r3
	add	r5, r1, r3          NOP
.L49:
	ldr	r2, [r6, #0]        ldr	ip, [r7, #0]
	ldr	r3, [r5, #0]        ldrb	lr, [r2, r4]	@ zero_extendqisi2
	ldrb	r0, [r3, r4]      NOP                 @ zero_extendqisi2
	ldrb	r1, [ip, r4]      NOP                 @ zero_extendqisi2
	mov	r3, lr, asl #7      mov	ip, r1, asl #6
	mov	r2, r0, asl #5      mov	r1, r1, asl #1
	add	r3, r3, lr          add	r1, r1, ip
	sub	r2, r2, r0, asl #3  NOP 
	add	r2, r2, r0          add	r3, r3, r1
	add	r3, r3, r2          NOP 
	mov	r3, r3, asr #8      ldr	r2, [r8, #0]
	add	r3, r3, #16         NOP 
	strb	r3, [r2, r4]      NOP 
	ldr	r3, [sp, #64]       add	r4, r4, #1
	cmp	r3, r4              bgt	.L49
.L45:
	ldr	r1, .L107+36        ldr	r0, .L107+40
	bl	fopen               ldr	r1, .L107+36
	mov	r7, r0              NOP 
	ldr	r0, .L107+44        bl	fopen
	ldr	r1, .L107+36        mov	r8, r0
	ldr	r0, .L107+48        bl	fopen
	ldr	r3, [sp, #68]       mov	sl, r0
	cmp	r3, #0              movgt	r6, #0
	ble	.L50                NOP 
.L51:
	ldr	r3, [sp, #64]        NOP 
	cmp	r3, #0              movgt	r4, #0
	movgt	r5, r6, asl #2    ble	.L54
.L52:
	ldr	r2, [sp, #20]       mov	r0, r7
	ldr	r3, [r2, r5]        ldr	r1, .L107+52
	ldrb	r2, [r3, r4]      bl	fprintf	@ zero_extendqisi2
	ldr	r3, [sp, #64]       add	r4, r4, #1
	cmp	r3, r4              bgt	.L52
.L54:
	mov	r0, #10             mov	r1, r7
	bl	fputc               ldr	r3, [sp, #68]
	add	r6, r6, #1          cmp	r3, r6
	bgt	.L51                NOP 
.L50:
	ldr	r3, [sp, #36]       NOP 
	cmp	r3, #0              movgt	r6, #0
	ble	.L55                NOP 
.L56:
	ldr	r1, [sp, #40]       NOP 
	cmp	r1, #0              NOP 
	movgt	r4, #0            movgt	r5, r6, asl #2
	ble	.L60                NOP 
.L57:
	ldr	r1, [sp, #24]       mov	r0, r8
	ldr	r3, [r1, r5]        ldr	r1, .L107+52
	ldrb	r2, [r3, r4]	    bl	fprintf @ zero_extendqisi2
	ldr	r2, [sp, #40]       add	r4, r4, #1
	cmp	r2, r4              bgt	.L57
.L60:
	mov	r0, #10             mov	r1, r8
	bl	fputc               ldr	r3, [sp, #36]
	add	r6, r6, #1          NOP 
	cmp	r3, r6              bgt	.L56
	mov	r6, #0              NOP 
.L58:
	ldr	r2, [sp, #40]       NOP 
	cmp	r2, #0              movgt	r4, #0
	movgt	r5, r6, asl #2    ble	.L63
.L61:
	ldr	r2, [sp, #28]       mov	r0, sl
	ldr	r3, [r2, r5]        ldr	r1, .L107+52
	ldrb	r2, [r3, r4]	    bl	fprintf@ zero_extendqisi2
	ldr	r3, [sp, #40]       add	r4, r4, #1
	cmp	r3, r4              bgt	.L61
.L63:
	mov	r1, sl              mov	r0, #10
	bl	fputc               ldr	r1, [sp, #36]
	add	r6, r6, #1          NOP 
	cmp	r1, r6              bgt	.L58
	
.L55:
	mov	r0, fp              bl	fclose
	ldr	r3, [sp, #68]       NOP 
	cmp	r3, #0              ble	.L64
	mov	r5, #0              NOP 
	mov	r4, r5              b	.L66
.L65:
	ldr	r3, [sp, #68]       add	r5, r5, #1
	cmp	r3, r5              add	r4, r4, #4
	ble	.L64                NOP
.L66:
	ldr	r3, [sp, #8]        NOP
	ldr	r0, [r3, r4]        bl	free
	ldr	r1, [sp, #12]       NOP
	ldr	r0, [r1, r4]        bl	free
	ldr	r2, [sp, #16]       NOP
	ldr	r0, [r2, r4]        bl	free
	ldr	r3, [sp, #20]       NOP
	ldr	r0, [r3, r4]        bl	free
	ldr	r1, [sp, #36]       NOP
	cmp	r1, r5              ble	.L65
	ldr	r2, [sp, #24]       add	r5, r5, #1
	ldr	r0, [r2, r4]        bl	free
	ldr	r3, [sp, #28]       NOP
	ldr	r0, [r3, r4]        bl	free
	ldr	r3, [sp, #68]       add	r4, r4, #4
	cmp	r3, r5              bgt	.L66
.L64:
	ldr	r0, [sp, #8]        bl	free
	ldr	r0, [sp, #12]       bl	free
	ldr	r0, [sp, #16]       bl	free
	ldr	r0, [sp, #20]       bl	free
	ldr	r0, [sp, #24]       bl	free
	ldr	r0, [sp, #28]       bl	free
	add	sp, sp, #84         NOP
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
.L103:
	mov	r0, r6              bl	malloc
	ldr	r1, [sp, #8]        NOP
	str	r0, [r1, r9, asl #2] NOP
	mov	r0, r6              bl	malloc
	ldr	r2, [sp, #12]       NOP
	str	r0, [r2, r9, asl #2] NOP
	mov	r0, r6              bl	malloc
	ldr	r3, [sp, #16]       NOP
	str	r0, [r3, r9, asl #2] NOP
	mov	r0, r6              bl	malloc
	ldr	r1, [sp, #20]       NOP
	ldr	ip, [sp, #68]       str	r0, [r1, r9, asl #2]
	b	.L13
.L98:
	ldr	r2, .L107+4         ldr	r0, .L107+56
	ldr	r3, [r2, #0]        mov	r1, #1
	mov	r2, #31             bl	fwrite
	mov	r0, #1              bl	exit
.L106:
	ldr	r2, .L107+4         ldr	r0, .L107+60
	ldr	r3, [r2, #0]        mov	r1, r5
	mov	r2, #31             bl	fwrite
	mov	r0, r5              bl	exit
.L100:
	ldr	r3, .L107+4         ldr	r2, [r4, #4]
	ldr	r0, [r3, #0]        ldr	r1, .L107+64
	bl	fprintf             mov	r0, #1
	bl	exit                NOP
.L99:
	ldr	r3, .L107+4         ldr	r2, [r4, #4]
	ldr	r0, [r3, #0]        ldr	r1, .L107+68
	bl	fprintf             mov	r0, #1
	bl	exit                NOP
.L108:
	.align	2
.L107:
	.word	.LC0
	.word	stderr
	.word	.LC2
	.word	.LC3
	.word	.LC5
	.word	.LC6
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC13
	.word	.LC12
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC1
	.word	.LC8
	.word	.LC7
	.word	.LC4
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"r\000"
	.space	2
.LC1:
	.ascii	"Please enter a valid file path\012\000"
.LC2:
	.ascii	"Invalid image format (must be 'P6')\012\000"
	.space	3
.LC3:
	.ascii	"%d %d\000"
	.space	2
.LC4:
	.ascii	"Invalid image size (error loading '%s')\012\000"
	.space	3
.LC5:
	.ascii	"height: %d, width: %d\012\000"
	.space	1
.LC6:
	.ascii	"%d\000"
	.space	1
.LC7:
	.ascii	"Invalid rgb range (error loading '%s')\012\000"
.LC8:
	.ascii	"Invalid rgb range, must be 255\012\000"
.LC9:
	.ascii	"range: %d\012\000"
	.space	1
.LC10:
	.ascii	"%c\012\000"
.LC11:
	.ascii	"%d%d%d%d\000"
	.space	3
.LC12:
	.ascii	"y.txt\000"
	.space	2
.LC13:
	.ascii	"w\000"
	.space	2
.LC14:
	.ascii	"cb.txt\000"
	.space	1
.LC15:
	.ascii	"cr.txt\000"
	.space	1
.LC16:
	.ascii	"%d \000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits
