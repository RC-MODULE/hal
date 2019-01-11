#include "hal_host.h"
#include "hostprofiler.h"
#include "stdio.h"
#include "windows.h"


int main(){

	printf("Openning...\n");
	#if defined (NMPU0) && defined (NMPU1)
	if (halOpen("target0.abs","target1.abs",NULL))
	#elif  defined (NMPU1)
	if (halOpen("","target1.abs",NULL))		// Load executable file to board, connect to shared memory
	#else  	
	if (halOpen("target0.abs",NULL))			// Load executable file to board, connect to shared memory
	#endif	
	{	
		::Sleep(10000);
		return -1;
	}
	
	
	halProfilerPrint2tbl(".main.map", 0);


	unsigned sync=-1;
	#ifdef NMPU1
	halGetResult(&sync,1);					// get return value (0x600D)
	#else 	
	halGetResult(&sync);					// get return value (0x600D)
	#endif	

	
	//::Sleep(10000);
	//halClose();								// close board, disconect from shared memory
	::Sleep(1000);
	printf("Return value:%X\n",sync);
	halClose();
	return 0;

}