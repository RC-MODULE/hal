#include "stdio.h"
#include "led.h"
#include "hal.h"
#include "link.h"
#include "section-hal.h"

#define SIZE 2
#define PORT 0

INSECTION(".data_imu3") int dst[SIZE + 0xF];



int main()
{
	halLinkInit();
	//printf("dst=0x%x\n", dst);
	for(int i=0;i< SIZE;i++){
		dst[i] = 0xCDCDCDCD;
	}	

	int cnt = 1;
	//for(int size = 2; size <= SIZE; size+=2){
		halLed(cnt++);
		halLinkStart(dst, SIZE, PORT, LINK_INPUT);
		while(halLinkIsCompleted(PORT, LINK_INPUT) == 0);
		halSleep(10);
	//}
	
	for (int i = 0; i < 4; i++)
		printf("%8.8X ", dst[i]);
	printf("\n");
	
	return 0;
	
}
