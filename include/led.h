
#ifndef _LED_H_INCLUDED_
#define _LED_H_INCLUDED_
#ifdef __cplusplus
		extern "C" {
#endif

void halLed(unsigned ledMask);    
void halLedOn (unsigned ledIndx);    
void halLedOff(unsigned ledIndx);    
void halLedSOS(unsigned ledIndx, int count=0);    
void halLedBlink(unsigned ledIndex, int freq, int ms);
int  halLedCount();    
	
#ifdef __cplusplus
		};
#endif

#endif // _LED_H_INCLUDED_
	
	

