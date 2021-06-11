#include "hal.h"
#include "link.h"
#include "section-hal.h"

#define SIZE 2
#define PORT_RC 0
#define PORT_TR 1

INSECTION(".data_imu3") int dst[SIZE + 2];


int main()
{
	halLinkInit();
	for(int i = 0; i < SIZE; i++){
		dst[i] = 0xCDCDCDCD;
	}

	// Прием 2 32-разрядных слов с порта PORT_RC (или 8 байтов)
	halLinkStart(dst, SIZE, PORT_RC, LINK_INPUT);
	while(halLinkIsCompleted(PORT_RC, LINK_INPUT) == 0);

	for(int i = 0; i < SIZE; i++){
		dst[i] += 1;
	}

	// Отсылка 2 32-разрядных слов с порта PORT_TR (или 8 байтов)
	halLinkStart(dst, SIZE, PORT_TR, LINK_OUTPUT);
	while(halLinkIsCompleted(PORT_TR, LINK_OUTPUT) == 0);

	return 0;
	
}
