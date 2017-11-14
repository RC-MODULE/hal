//#include "mem_map.h"

#define INTERNAL_RAM_BASE  	0x00100000
#define INTERNAL_RAM_SIZE  	256*1024 // Byte size
#define RAM_IMAGE_PTR   	INTERNAL_RAM_BASE
#define ENTRY_POINT_PTR 	RAM_IMAGE_PTR

.section ".text"

	
Mode_USR        =     0x10
Mode_FIQ        =     0x11
Mode_IRQ        =     0x12
Mode_SVC        =     0x13
Mode_ABT        =     0x17
Mode_UNDEF      =     0x1B
Mode_SYS        =     0x1F ; #available on ARM Arch 4 and later

I_Bit           =     0x80 ; #when I bit is set, IRQ is disabled
F_Bit           =     0x40 ; #when F bit is set, FIQ is disabled
T_Bit			=     0x20 ; #when T bit is set, THUMB state bit




/*.equ STACK_BASE, 0x180000-4*/

; #.global main

.global start0
start0:
	MOV R0, #0x0;
	MOV R1, #0x1;
	MOV R2, #0x2;
	MOV R3, #0x3;
	MOV R4, #0x4;
	MOV R5, #0x5;
	MOV R6, #0x6;
	MOV R7, #0x7;
	MOV R8, #0x8;
	MOV R9, #0x9;
	MOV R10, #0x10;
	MOV R11, #0x11;
	MOV R12, #0x12;
	
	
	; #=========== Init Stack pointer in SVC & IRQ modes ==========
    ; #Enter SVC mode and set up the SVC stack pointer
	MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit 					; #No interrupts
	LDR 	SP, =(INTERNAL_RAM_BASE + INTERNAL_RAM_SIZE - 4) 	; # set SCV stack pointer
	
	; #Enter IRQ mode and set up the IRQ stack pointer
	MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit ; #No interrupts
	LDR     SP, =(INTERNAL_RAM_BASE + INTERNAL_RAM_SIZE - 256) 
	
	MSR     CPSR_cf, #Mode_SVC			; #enter SVC mode enable IRQ
	


	
    B	main
  
    
