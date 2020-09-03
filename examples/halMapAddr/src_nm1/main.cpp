// #include "math.h"
#include "hal.h"
#include "stdio.h"


int main()
{
	int addr = 0x369e;
	int ownAddr = 0;
	int toAddr = 0;
	

	ownAddr = (unsigned int)halMapAddrFrom((void*)0x123, 0);
	printf("halMapAddrFrom (0x00123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x2123, 0);
	printf("halMapAddrFrom (0x02123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x40123, 0);
	printf("halMapAddrFrom (0x40123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrFrom((void*)0x80123, 0);
	printf("halMapAddrFrom (0x80123,0)=%x\n",ownAddr);
	
	
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x123, 0);
	printf("halMapAddrTo (0x00123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x2123, 0);
	printf("halMapAddrTo (0x02123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x40123, 0);
	printf("halMapAddrTo (0x40123,0)=%x\n",ownAddr);
	
	ownAddr = (unsigned int)halMapAddrTo((void*)0x80123, 0);
	printf("halMapAddrTo (0x80123,0)=%x\n",ownAddr);
	
	

	return 1;
}
