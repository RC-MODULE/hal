#include "ubc_host.h"
#include "stdio.h"
#include "windows.h"

int main(){

	printf("Openning...\n");
	#if defined (NMPU0) && defined (NMPU1)
	if (ubcOpen("main0.abs","main1.abs",NULL))
	#elif  defined (NMPU1)
	if (ubcOpen("","main1.abs",NULL))		// Load executable file to board, connect to shared memory
	#else  	
	if (ubcOpen("main0.abs",NULL))			// Load executable file to board, connect to shared memory
	#endif	
	{	
		::Sleep(10000);
		return -1;
	}
	
	
	unsigned sync;
	#ifdef NMPU1
	ubcGetResult(&sync,1);					// get return value (0x600D)
	#else 	
	ubcGetResult(&sync);					// get return value (0x600D)
	#endif	

	printf("Return value:%X\n",sync);
	ubcClose();								// close board, disconect from shared memory
	::Sleep(5000);
	return 0;

}