#include "sleep.h"
#include "hal_target.h"
#include "led.h"
#include "hal.h"
#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif


#ifdef __GNUC__
/**************************NMC-GCC************************/
__attribute__((section(".data_hal_syncro"))) SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
//__attribute__((section(".data"))) int procNo=-1;

#else
/***************************NMCC*************************/
#pragma data_section ".data_hal_syncro"
  SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
#pragma data_section ".data"

#endif
    


#ifdef __cplusplus
};
#endif

