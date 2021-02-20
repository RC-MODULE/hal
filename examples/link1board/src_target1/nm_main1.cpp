#include "hal.h"
#include "section-hal.h"
#include "hal_target.h"
#include "stdio.h"
#include "link.h"
#include "nmpp.h"
#include "led.h"

#define LINK_PORT 2
#define SIZE 16

INSECTION(".data_imu1") int data[16384];

INSECTION(".text_shared0") int main()
{
	halLinkInit(LINK_PORT, LINK_INPUT);
	nmppsSet_32s(data, 0xCDCDCDCD, 16384);


	int counter = 0;
	for (int i = 0; i < 100; i++) {
		halLed4Hi(counter++ % 16);
		halLinkStart(data, SIZE, LINK_PORT);
		while (halLinkIsCompleted(LINK_PORT) == 0);
		halSleep(10);
	}
	return 0x611D611D;
} 

