
#ifndef _LED_H_INCLUDED_
#define _LED_H_INCLUDED_
#ifdef __cplusplus
		extern "C" {
#endif

void halLed(unsigned ledMask);    
void halLedOn (unsigned ledIndx);    
void halLedOff(unsigned ledIndx);    
	
#ifdef __cplusplus
		};
#endif

#endif // _LED_H_INCLUDED_
	
	

