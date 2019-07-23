import from led.mlb;
import from rtmacro.mlb;


begin ".text_hal"

//global ledBit: word[8]=(1,2,4,8,16,32,64,128);

//global _halLed:label;
//<_halLed>
//	ar5 = ar7-2;
//	gr7 = [--ar5];
//	HAL_LED(gr7);
//	return;
/*	
global _halLedOnSC:label;
<_halLedOnSC>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_ON_SC(gr7);
	return;

global _halLedOffSC:label;
<_halLedOffSC>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_OFF_SC(gr7);
	return;

global _halLedSwitchSC:label;
<_halLedSwitchSC>
	ar5 = ar7-2;
	gr7 = [--ar5];
	HAL_LED_SWITCH_SC(gr7);
	return;
*/

	
end ".text";
