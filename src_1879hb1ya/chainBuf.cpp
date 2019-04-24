#include "dma.h"
#include "section-hal.h"
#ifdef __cplusplus
		extern "C" {
#endif

	INSECTION(".data_hal") 	int chainBuf[MAX_CHAIN_BUF_SIZE*8];

#ifdef __cplusplus
		};
#endif
