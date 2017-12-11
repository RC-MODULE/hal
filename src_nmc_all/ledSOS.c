#include "led.h"
#include "sleep.h"

#ifdef __cplusplus
    extern "C" {
#endif

	
void halLedBlink(unsigned led, int freq, int ms){
	int count= freq*ms/1000;
	int dt=500/freq;
	int i;
	for(i=0; i<count; i++){
		halLedOn(led);
		halSleep(dt);
		halLedOff(led);
		halSleep(dt);
	}
}

void halLedSOS(unsigned index, int count){
	int dash =500;
	int dot  =150;
	int pause=150;
	halLedOff(index);
	halSleep(pause);
	if (count==0) 
		count--;
	while (count){
		//------- S ---------
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause*4);
		//------- O ---------
		halLedOn(index);	halSleep(dash); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dash); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dash); halLedOff(index); halSleep(pause*4);
		//------- S ---------
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause);
		halLedOn(index);	halSleep(dot); halLedOff(index); halSleep(pause*10);
		//------------------
		count--;
	}		
}

#ifdef __cplusplus
    };
#endif
