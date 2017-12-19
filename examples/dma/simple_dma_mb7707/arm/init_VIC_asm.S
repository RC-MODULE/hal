 .align
 .pool
 .text
 .align
 .pool


 
#include "vic.h"
.global Init_VIC	
Init_VIC:
	PUSH 	{R0-R2,lr}
	; #=========== Enable VIC ======================================
	LDR		R1, =PERIP_Base|PERIP_Size		 
	; # Peripheral Port remap Register. MCR p15, 0, Rd, c15, c2, 4.  Secure Monitor or Privileged when NS=0
	MCR		p15, 0, R1, c15, c2, 4 		; #Write Peripheral Port Memory Remap Register
	MRC     p15, 0, r0, c1, c0, 0       ; #read CP15 register 1 into r0
	ORR     r0, r0, #(1 << 24)          ; #enable VIC interface
	MCR     p15, 0, r0, c1, c0, 0       ; #write cp15 register 1  / ; Secure Control Register. MCR p15, 0, Rd, c1, c0, 0 Secure Monitor or Privileged when NS=0
	; #=============================================================
	LDR					R1, =VIC_Base0
	MOV					R2, #0
	STR					R2, [R1,#VIC_INTSELECT]
	LDR					R2, =0xFFFFFFFF ; # disable all IRQ
 	STR					R2, [R1,#VIC_INTENCLEAR]
	
	LDR					R1, =VIC_Base1
	MOV					R2, #0
	STR					R2, [R1,#VIC_INTSELECT]
	LDR					R2, =0xFFFFFFFF ; # disable all IRQ
 	STR					R2, [R1,#VIC_INTENCLEAR]

	POP 	{R0-R2,lr}
	BX		lr