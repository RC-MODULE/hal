#include "hal.h"
#include "section-hal.h"
#include "hal_target.h"
#include "stdio.h"
#include "link.h"
#include "nmpp.h"

#define LINK_PORT 2
#define SIZE 256


INSECTION(".data_imu1") int data[16384];
INSECTION(".data_imu1") volatile bool flag;

void interrupt() {
	static int counter = 0;
	counter++;
	/*printf("counter = %d\n", counter);
	for (int i = 0; i < 14; i++) {
		printf("0x%x ", data[i]);
	}
	printf("\n\n");*/
	nmppsSet_32s(data, 0xCDCDCDCD, 16384);
	if (counter != 2000) {
		halLinkStart(data, SIZE, LINK_PORT);
	}
	else {
		flag = false;
	}
	
	halLed(counter);
}


INSECTION(".text_shared0") int main()
{
	try {
		int fromHost = halHostSync(0xC0DE0001);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			throw - 1;
		}
		halLinkInit(LINK_PORT, LINK_INPUT);
		halLinkSetCallback(interrupt, LINK_PORT);
	}
	catch (int& e) {
		if (e == -2) {
			halHostSync(0xDEADB00F);
		}
		return e;
	}
	halHostSync(0x600DB00F);	// send ok to host
	flag = true;
	nmppsSet_32s(data, 0xCDCDCDCD, 16384);
	halLinkStart(data, SIZE, LINK_PORT);
	while (flag);
	/*for (int i = 0; i < 3000; i++) {
		//halHostSync(i);	// send ok to host
		printf("%d\n", i);
		//halLinkStart(data, SIZE, LINK_PORT);
		//while (halLinkIsCompleted(LINK_PORT) == 0);
	}*/
	return 0x611D611D;
} 

