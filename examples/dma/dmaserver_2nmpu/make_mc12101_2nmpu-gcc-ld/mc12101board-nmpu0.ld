MEMORY
{
	C0_IMU0 : ORIGIN = 0x00000200*4, LENGTH = 0x00007E00*4
	C0_IMU1 : ORIGIN = 0x00008000*4, LENGTH = 0x00008000*4
	C0_IMU2 : ORIGIN = 0x00010000*4, LENGTH = 0x00008000*4
	C0_IMU3 : ORIGIN = 0x00018000*4, LENGTH = 0x00008000*4
	C0_IMU4 : ORIGIN = 0x00020000*4, LENGTH = 0x00008000*4
	C0_IMU5 : ORIGIN = 0x00028000*4, LENGTH = 0x00008000*4
	C0_IMU6 : ORIGIN = 0x00030000*4, LENGTH = 0x00008000*4
	C0_IMU7 : ORIGIN = 0x00038000*4, LENGTH = 0x00008000*4
	SYNCRO  : ORIGIN = 0x000A8000*4, LENGTH = 0x00000010*4          /*  shared SYNCRO BUFFER used by both nmc  */
	SHMEM0  : ORIGIN = 0x000A8010*4, LENGTH = 0x00008000*4-0x10*4   /*  shared internal 128kB                  */
	SHMEM1  : ORIGIN = 0x000B0000*4, LENGTH = 0x00008000*4  		/*  shared internal 128kB                  */
/*  SHMEM1  : ORIGIN = 0x000B8000*4, LENGTH = 0x00008000*4 				shared internal 128kB used by nmpu1    */
	EMI     : ORIGIN = 0x20000010*4, LENGTH = 0x04000000*4 - 10*4 	/*  shared DDR2  	256MB  			       */
/*	EMI     : ORIGIN = 0x24000000*4, LENGTH = 0x04000000*4		        shared DDR2     256MB used by nmpu1	   */
}

SECTIONS
{
  .text : ALIGN(0x8)
  {
    *(.text);
  } > EMI
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C0_IMU5
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU5
  .mem_bank0 : ALIGN(0x8)
  {
    *(.mem_bank0);
  } > C0_IMU0
  .mem_bank1 : ALIGN(0x8)
  {
    *(.mem_bank1);
  } > C0_IMU1
  .mem_bank2 : ALIGN(0x8)
  {
    *(.mem_bank2);
  } > C0_IMU2
  .mem_bank3 : ALIGN(0x8)
  {
    *(.mem_bank3);
  } > C0_IMU3
  .mem_bank4 : ALIGN(0x8)
  {
    *(.mem_bank4);
  } > C0_IMU4
  .mem_bank5 : ALIGN(0x8)
  {
    *(.mem_bank5);
  } > C0_IMU5
  .mem_bank6 : ALIGN(0x8)
  {
    *(.mem_bank6);
  } > C0_IMU6
  .mem_bank7 : ALIGN(0x8)
  {
    *(.mem_bank7);
  } > C0_IMU7
  .data_DDR : ALIGN(0x8)
  {
    *(.data_DDR);
  } > EMI
  .mem_bank0.bss : ALIGN(0x8)
  {
    *(.mem_bank0.bss);
  } > C0_IMU0
  .mem_bank1.bss : ALIGN(0x8)
  {
    *(.mem_bank1.bss);
  } > C0_IMU1
  .mem_bank2.bss : ALIGN(0x8)
  {
    *(.mem_bank2.bss);
  } > C0_IMU2
  .mem_bank3.bss : ALIGN(0x8)
  {
    *(.mem_bank3.bss);
  } > C0_IMU3
  .mem_bank4.bss : ALIGN(0x8)
  {
    *(.mem_bank4.bss);
  } > C0_IMU4
  .mem_bank5.bss : ALIGN(0x8)
  {
    *(.mem_bank5.bss);
  } > C0_IMU5
  .mem_bank6.bss : ALIGN(0x8)
  {
    *(.mem_bank6.bss);
  } > C0_IMU6
  .mem_bank7.bss : ALIGN(0x8)
  {
    *(.mem_bank7.bss);
  } > C0_IMU7
  .data_DDR.bss : ALIGN(0x8)
  {
    *(.data_DDR.bss);
  } > EMI
  .data : ALIGN(0x8)
  {
    *(.data);
  } > C0_IMU6
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > C0_IMU6
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C0_IMU7


  .data_hal_syncro : ALIGN(0x8)
  {
	*(.data_hal_syncro);
  } > SYNCRO
  

 .rpc_services.bss : ALIGN(0x8)
  {
    *(.rpc_services.bss);

  } > EMI
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI
  
  .heap : ALIGN(0x8)
  {
    *(.heap);
	. = ORIGIN(EMI) + LENGTH(EMI);
    __HEAP_0_END = .;
  } > EMI

  .heap1 : ALIGN(0x8)
  {
    *(.heap1);
	. = ORIGIN(C0_IMU1) + LENGTH(C0_IMU1);
    __HEAP_1_END = .;
  } > C0_IMU1

  .heap2 : ALIGN(0x8)
  {
    *(.heap2);
	. = ORIGIN(C0_IMU2) + LENGTH(C0_IMU2);
    __HEAP_2_END = .;
  } > C0_IMU2

  .heap3 : ALIGN(0x8)
  {
    *(.heap3);
	. = ORIGIN(C0_IMU3) + LENGTH(C0_IMU3);
    __HEAP_3_END = .;
  } > C0_IMU3
  

  
} 