global _haltest 	: label;

const X=0000A8000h;
	

	
begin ".text_hal"

import from "critical";
import from "led";
import from "sleep";
import from "printx";

DECLARE_CS_CONST();

<SOS>	
	extern _halSyncro:word;
	PRINT_32X("gr7=",gr7);
	PRINT_MATRIX_32X("syncro:",_halSyncro,4,1);
	HAL_LED_SOS(0,1);
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
		HAL_ENTER_CRITICAL_SECTION(gr0);
		gr5=[X+1];
		with gr5;
		if <>0 delayed call SOS;
			[X]=gr7;
		
		//HAL_LED(gr7>>16);
		
		gr5=[X+1];
		with gr5;
		if <>0 delayed call SOS;
			[X]=gr0;
			
		HAL_EXIT_CRITICAL_SECTION(gr0);
	
		with gr7--;	
		//with true;
	if <>0 goto next;
		
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
	return ;
end ".text_hal";