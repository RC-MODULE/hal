extern "C" {
	void halLed(unsigned ledMask){};    
	void halLedOn (unsigned ledIndx){};    
	void halLedOff(unsigned ledIndx){};    
	void halLedSOS(unsigned ledIndx){};    

void halLed4Hi(unsigned led4Mask){}
void halLed4Lo(unsigned led4Mask){}   
void halLedSwitch(unsigned ledIndx){}
void halLedBlink(unsigned ledIndex, int freq, int ms){}
int  halLedCount(){return 0;}

void halLedOnSC (unsigned ledIndx){}
void halLedOffSC(unsigned ledIndx){}   
void halLedSwitchSC(unsigned ledIndx){}

};