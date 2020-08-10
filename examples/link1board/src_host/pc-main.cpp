#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "nmpp.h"

#define NUM_NM_CORES 2

int main()
{
	if (halOpen("main0.abs", "main1.abs", NULL)){
		printf("Connection to mc12101 error!\n");
		return -1;
	}

	for (int i = 0; i < NUM_NM_CORES; i++) {
		int handshake = halSync(0xC0DE0086, i);
		if (handshake != (0xC0DE0000 | i)) {
			printf("Handshake with mc12101-nmc%d error!\n", i);
			return -1;
		}
	}
	for (int i = 0; i < NUM_NM_CORES; i++) {
		int ok = halSync(0, i);
		if (ok != 0x600DB00F) {
			printf("Memory nmc%d allocation error!\n", i);
			return -2;
		}
	}

	/*for (int i = 0; i < 1000; i++) {
		int o1 = halSync(0, 0);
		int o2 = halSync(0, 1);
		int g = o1 + 02;
	}*/
	unsigned int result[2];
	halGetResult(result, 0);
	halGetResult(result + 1, 1);
	printf("result=0x%x, 0x%x\n", result[0], result[1]);
};
