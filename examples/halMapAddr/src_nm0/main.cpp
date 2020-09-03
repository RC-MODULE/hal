// #include "math.h"
#include "hal.h"
#include "stdio.h"


int main()
{
	int addr = 0x369e;
	int ownAddr = 0;
	int toAddr = 0;
	
	printf("\n\n");
	//printf("toAddr = halMapAddrTo (addr,...):\n");
	//toAddr = (unsigned int)halMapAddrTo((void*)x123, 1);
	//printf("addr = %x toAddr = %x\n\n", addr, toAddr);
	
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x123, 1);
	printf("halMapAddrFrom (0x00123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x2123, 1);
	printf("halMapAddrFrom (0x02123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x40123, 1);
	printf("halMapAddrFrom (0x40123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x80123, 1);
	printf("halMapAddrFrom (0x80123,1)=%x\n",ownAddr);
	
	
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x123, 1);
	printf("halMapAddrTo (0x00123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x2123, 1);
	printf("halMapAddrTo (0x02123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x40123, 1);
	printf("halMapAddrTo (0x40123,1)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x80123, 1);
	printf("halMapAddrTo (0x80123,1)=%x\n",ownAddr);
	
	
	//printf("addr = %x ownAddr = %x\n\n", addr, ownAddr);

	

	return 1;
}
