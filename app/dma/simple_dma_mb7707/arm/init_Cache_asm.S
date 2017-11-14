# 1 "../../src/init_Cache_asm.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "../../src/init_Cache_asm.S"
 .align
 .pool
 .text
 .align
 .pool



.global Init_Cache
Init_Cache:
 PUSH {R0}
 MCR p15, 0, r0, c7, c5, 0 @ ; invalidate all I cache.
 MCR p15, 0, r0, c7, c6, 0 @ ; invalidate all D cache.
 MRC p15, 0, r0, c1, c0, 0 @ ; read CP15 register 1 into r0
 ORR r0, r0, #(0x1 <<12) @ ; enable I Cache
 @ORR r0, r0, #(0x1 <<2) @ ; enable D Cache
 MCR p15, 0, r0, c1, c0, 0 @ ; write CP15 register 1
 POP {R0}
 BX lr
