#include "link.h"

#define SIZE (640 * 480 * 3 / 4)
#define SMALL_SIZE 0x8000
#define PORT 1

__attribute__((section(".data_imu1"))) int src[SMALL_SIZE];

int main()
{
	halLinkInit(PORT, LINK_OUTPUT);
	for(int i = 0; i < SIZE; i++){
		src[i] = 0x55555555;
	}
	
	for(int i = 0; i < SIZE; i++){
		int localSize = ((SIZE - i) < SMALL_SIZE) ? (SIZE - i): SMALL_SIZE;
		halLinkStart(src, localSize, PORT, LINK_OUTPUT);
		while(halLinkIsCompleted(PORT, LINK_OUTPUT) == 0);
	}
	return 0;
}
