
#ifndef _LED_H_INCLUDED_
#define _LED_H_INCLUDED_
#ifdef __cplusplus
		extern "C" {
#endif

void halLed(unsigned ledMask);    
void halLed4Hi(unsigned led4Mask);    
void halLed4Lo(unsigned led4Mask);    
void halLedOn (unsigned ledIndx);    
void halLedOff(unsigned ledIndx);    
void halLedSwitch(unsigned ledIndx);    
void halLedSOS(unsigned ledIndx, int count);    
void halLedBlink(unsigned ledIndex, int freq, int ms);
int  halLedCount();    

void halLedOnSC (unsigned ledIndx);    
void halLedOffSC(unsigned ledIndx);    
void halLedSwitchSC(unsigned ledIndx);    
	
#ifdef __cplusplus
		};
#endif

#endif // _LED_H_INCLUDED_
	
	

