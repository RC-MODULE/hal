#include "stdio.h"
#include "led.h"
#include "hal.h"
#include "link.h"
#include "section-hal.h"

#define SIZE 16384
#define PORT 1

INSECTION(".data_imu3") int src[SIZE + 2];


int main()
{
	halLinkInit(PORT, LINK_OUTPUT);
	unsigned int crc = 0;
	//printf("src=0x%x\n", src);
	for(int i=0;i< SIZE;i++){
		src[i] = i;
	}
	
	int cnt = 1;
	for(int size = 2; size <= SIZE; size+=2){
		halLed(cnt++);
		halLinkStart(src, size, PORT);
		while(halLinkIsCompleted(PORT) == 0);
		//nmppsCrcAcc_32s(src, size, &crc);
	}
	halLed(((int*)0x40001C0A)[0]);
	printf("crc=0x%x\n", crc);
	return 0;
}
