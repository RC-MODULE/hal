#include "hal_host.h"
#include "sleep.h"
#include "stdio.h"
#include "windows.h"

//#undef NMPU1
int main(){

	unsigned sync;
	printf("Openning...\n");
	#if defined (NMPU0) && defined (NMPU1)
		if (halOpen("main0.abs","main1.abs",NULL))
			return -1;
		halGetResult(&sync,0);					// get return value (0x600D)				
		printf("Return value:%X\n", sync);
		halGetResult(&sync,1);					// get return value (0x600D)				
	#elif  defined (NMPU1)
		if (halOpen("","main1.abs",NULL))		// Load executable file to board, connect to shared memory
			return -1;
		halGetResult(&sync,1);					// get return value (0x600D)				
	#else  	
		if (halOpen("main0.abs",NULL))			// Load executable file to board, connect to shared memory
			return -1;
		halGetResult(&sync);					// get return value (0x600D)				
		//while(1);
	#endif	
	
	printf("Return value:%X\n", sync);
	halSleep(1000);
	halClose();									// close board, disconect from shared memory
	
	return 0;

}