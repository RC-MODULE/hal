global _haltest 	: label;

const X=0000A8000h;
	
	
begin ".text_hal"

import from "critical";
import from "led";
import from "sleep";

DECLARE_CS_CONST();

<SOS>
	
	HAL_LED_SOS(1,1);
	HAL_LED(gr7);
	HAL_SLEEP(10000);
	
	
	
	
return;



global _halTestCS:label;
<_halTestCS>
	ar5=sp-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar5,gr5;
	gr7 = [--ar5];
	gr0 = 0;
	gr1 = 1;
	
	
	<next>
		HAL_ENTER_CRITICAL_SECTION(gr1);
		gr5=[X];
		with gr5;
		if <>0 delayed call SOS;
			[X+1]=gr7;
		
		//HAL_LED4HI(gr7>>16);
		gr5=[X];
		with gr5;
		if <>0 delayed call SOS;
			[X+1]=gr0;
			
		HAL_EXIT_CRITICAL_SECTION(gr1);
	
		with gr7--;	
		//with true;
	if <>0 goto next;
		
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
	return ;
end ".text_hal";