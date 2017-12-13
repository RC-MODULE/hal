#include "hal.h"
#include "cache.h"
#include "math.h"
#include "time.h"
#include "stdio.h"
#include "malloc.h"

clock_t t0,t1;

int main()
{ 
	
	printf("Starting cache test...\n");
	
	halInstrCacheDisable();
	t0=clock();
	sinf(0.5);
	t1=clock();
	
	printf("Cache disabled . Sin time = %d \n",t1-t0);	
	
	halInstrCacheEnable();
	t0=clock();
	sinf(0.5);
	t1=clock();
	
	printf("Cache enabled  . Sin time = %d \n",t1-t0);	
	
	
	
	return 0;
	
	
} 