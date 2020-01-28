MEMORY
{
	C0_IMU0 : ORIGIN = 0x00000200*4, LENGTH = 0x00007E00*4
	C0_IMU1 : ORIGIN = 0x00008000*4, LENGTH = 0x00008000*4
	C0_IMU2 : ORIGIN = 0x00010000*4, LENGTH = 0x00008000*4
	C0_IMU3 : ORIGIN = 0x00018000*4, LENGTH = 0x00008000*4
	SYNCRO  : ORIGIN = 0x000A8000*4, LENGTH = 0x00000010*4          /*  shared SYNCRO BUFFER used by both nmc  */
/*	SHMEM0  : ORIGIN = 0x000A8010*4, LENGTH = 0x00008000*4  		    shared internal 128kB used by nmpu0     */
/*	SHMEM1  : ORIGIN = 0x000B0000*4, LENGTH = 0x00008000*4  		    shared internal 128kB used by nmpu0     */
    SHMEM1  : ORIGIN = 0x000B8000*4, LENGTH = 0x00008000*4 			/*	shared internal 128kB                   */
/*	EMI     : ORIGIN = 0x20000000*4, LENGTH = 0x04000000*4			    shared DDR2  	256MB used by nmpu0     */
	EMI     : ORIGIN = 0x24000000*4, LENGTH = 0x04000000*4		    /*  shared DDR2     256MB                   */
	
	

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
  } > C0_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU0
  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
  } > C0_IMU0
  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C0_IMU1
  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C0_IMU2
  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C0_IMU3


  .data_DDR : ALIGN(0x8)
  {
    *(.data_ddr);
  } > EMI
  .data_imu0.bss : ALIGN(0x8)
  {
    *(.data_imu0.bss);
  } > C0_IMU0
  .data_imu1.bss : ALIGN(0x8)
  {
    *(.data_imu1.bss);
  } > C0_IMU1
  .data_imu2.bss : ALIGN(0x8)
  {
    *(.data_imu2.bss);
  } > C0_IMU2
  .data_imu3.bss : ALIGN(0x8)
  {
    *(.data_imu3.bss);
  } > C0_IMU3


  .data_ddr.bss : ALIGN(0x8)
  {
    *(.data_ddr.bss);
  } > EMI
  .data : ALIGN(0x8)
  {
    *(.data);
  } > C0_IMU2
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > C0_IMU2
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C0_IMU3
  
  
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
  
  .data_hal_syncro : ALIGN(0x8)
  {
	*(.data_hal_syncro);
  } > SYNCRO
}