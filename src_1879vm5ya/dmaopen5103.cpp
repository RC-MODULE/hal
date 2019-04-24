#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif

	
INSECTION(".data_hal") int nonfirst, chain[10], matrixcnt, packetcnt;
INSECTION(".data_hal") int ofdmainit;

INSECTION(".text_hal") void halOpenDMA()
{ 
	nonfirst=0; 
	chain[9]=0;
	matrixcnt=0;
	packetcnt=-1;
	ofdmainit=0;
}

#ifdef __cplusplus
		};
#endif
