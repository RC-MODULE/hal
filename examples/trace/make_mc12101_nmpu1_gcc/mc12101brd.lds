MEMORY
{
  C0_IMU0 : ORIGIN = 0x00000800, LENGTH = 0x0001F800
  C0_IMU1 : ORIGIN = 0x00020000, LENGTH = 0x00020000
  C0_IMU2 : ORIGIN = 0x00040000, LENGTH = 0x00020000
  C0_IMU3 : ORIGIN = 0x00060000, LENGTH = 0x00020000
  SHMEM0 : ORIGIN = 0x002A0000, LENGTH = 0x00020000
  SHMEM1 : ORIGIN = 0x002C0000, LENGTH = 0x00040000
  EMI : ORIGIN = 0x80000000, LENGTH = 0x20000000
}

SECTIONS
{
	.text : ALIGN(0x8)
	{
		*(.text);
	} > C0_IMU0

	.init : ALIGN(0x8)
	{
		*(.init);
	} > C0_IMU0

	.fini : ALIGN(0x8)
	{
		*(.fini);
	} > C0_IMU0

	.data : ALIGN(0x8)
	{
		*(.data);
	} > C0_IMU0

	.bss : ALIGN(0x8)
	{
		*(.bss);
	} > C0_IMU0

	.stack : ALIGN(0x8)
	{
		*(.stack);
	} > C0_IMU0
	
	.trace : ALIGN(0x8)
	{
		*(._stub.text)
		*(._stub.data)
		*(._stub.stack)

		*(.trace.text)
		*(.trace.data)
	} > EMI
	.trace.buf : ALIGN(0x8)
	{
		*(.trace.bss)
	} > EMI
}
