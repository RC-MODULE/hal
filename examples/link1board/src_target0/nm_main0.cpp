#include "hal_target.h"
#include "hal.h"
#include "section-hal.h"
#include "stdio.h"
#include "link.h"

#define LINK_PORT 2
#define SIZE 256

INSECTION(".data_imu7") int data[16384];

INSECTION(".text_shared0") int main()
{
	try {
		int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			throw -1;
		}
		halLinkInit(LINK_PORT, LINK_OUTPUT);
	}
	catch (int& e) {
		if (e == -2) {
			halHostSync(0xDEADB00F);
		}
		return e;
	}
	halHostSync(0x600DB00F);	// send ok to host

	for (int i = 0; i < 16384; i++) {
		data[i] = i;
	}

	for (int i = 0; i < 2000; i++) {
		
		//halHostSync(i);	// send ok to host
		halLinkStart(data, SIZE, LINK_PORT);
		while (halLinkIsCompleted(LINK_PORT) == 0);
	}
	
	return 0x600D600D;
} 

