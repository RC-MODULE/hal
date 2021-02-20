#include "stdio.h"
#include "led.h"
#include "hal.h"
#include "link.h"
#include "section-hal.h"

#define SIZE 16384
#define PORT 0

INSECTION(".data_imu3") int dst[SIZE + 0xF];


int main()
{
	halLinkInit(PORT, LINK_INPUT);
	unsigned int crc = 0;
	//printf("dst=0x%x\n", dst);
	for(int i=0;i< SIZE;i++){
		dst[i] = 0xCDCDCDCD;
	}	

	int cnt = 1;
	for(int size = 2; size <= SIZE; size+=2){
		halLed(cnt++);
		halLinkStart(dst, size, PORT);
		while(halLinkIsCompleted(PORT) == 0);
		//nmppsCrcAcc_32s(dst, size, &crc);
	}
	//halLed(((int*)0x4000181A)[0]);
	printf("crc=0x%x\n", crc);
	return 0;
	
}
