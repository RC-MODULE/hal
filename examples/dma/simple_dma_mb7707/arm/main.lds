OUTPUT_ARCH(ARM)
MEMORY
{
 /*
	FLASH (rx) : ORIGIN = 0x80000000, LENGTH = 256K
    IM0  (rwx) : ORIGIN = 0x00100000, LENGTH = 256K
	IM2  (rwx) : ORIGIN = 0x80100000, LENGTH = 256K
	
	EM0  (rwx) : ORIGIN = 0x40000000, LENGTH = 16M
	EM1  (rwx) : ORIGIN = 0xc0000000, LENGTH = 16M
*/
}
SECTIONS {
    . = 0x100100;
    .start : {	image_start.o (.text)   }
    .text : { *(.text) }
    .rodata : { *(.rodata.*) }
    .data : { *(.data) }
    .bss : { 
		*(.bss) 
		_end = .;
	} 

	
}


/* end of allocated ram _end */
PROVIDE( __cs3_heap_start = 0x80100200 );

/* end of the heap -> align 8 byte */ 
PROVIDE ( __cs3_heap_limit  = 0x3E00 );