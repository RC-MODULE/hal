import from led.mlb;
import from rtmacro.mlb;


begin ".text_hal"

//global ledBit: word[8]=(1,2,4,8,16,32,64,128);

global _halLed:label;
<_halLed>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED(gr7);
	return;
	
global _halLedOn:label;
<_halLedOn>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_ON(gr7);
	return;

global _halLedOff:label;
<_halLedOff>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_OFF(gr7);
	return;

global _halLedSwitch:label;
<_halLedSwitch>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_SWITCH(gr7);
	return;


global _halLed4Hi:label;
<_halLed4Hi>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED4HI(gr7);
	return;

global _halLed4Lo:label;
<_halLed4Lo>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED4LO(gr7);
	return;
	
end ".text_hal";
