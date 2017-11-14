#include "led.h"
#include "sleep.h"
#include "stdio.h"
#include "malloc.h"


int main()
{ 
	
	printf("Starting led test...\n");
	
	printf("switch on all leds\n");
	halLed(0xFF);
	halSleep(1000);
	
	printf("switch off all leds\n");
	halLed(0x00);
	halSleep(1000);
	
	for(int led=0; led<8; led++){
		halLedOn(led);
		halSleep(100);
	}
	
	for(int led=0; led<8; led++){
		halLedOff(led);
		halSleep(100);
	}
	
	
	
	
	return 0;
	
	
} 