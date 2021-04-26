#include "stdio.h"
#include "led.h"
#include "hal.h"
#include "link.h"
#include "section-hal.h"

#define SIZE 2
#define PORT 0

INSECTION(".data_imu3") int src[SIZE + 2];


int main()
{
	halLinkInit();
	//printf("src=0x%x\n", src);
	for(int i=0;i< SIZE;i++){
		src[i] = 0x12345678 + i;
	}
	
	printf("%p\n", src);
	for (int i = 0; i < 4; i++)
		printf("%8.8X ", src[i]);
	printf("\n");

	int cnt = 1;
	//for(int size = 2; size <= SIZE; size+=2){
		halLed(cnt++);
		halLinkStart(src, SIZE, PORT, LINK_OUTPUT);
		while(halLinkIsCompleted(PORT, LINK_OUTPUT) == 0);
		halSleep(10);
		//nmppsCrcAcc_32s(src, size, &crc);
	//}
	return 0;
}
