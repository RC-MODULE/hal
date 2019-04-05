import from rtmacro.mlb;
begin ".text_hal"

// gr0 - msecs
global halSleep:label;
<halSleep>
	push ar0,gr0;
	push ar1,gr1 with gr1=gr0;
	push ar2,gr2;
    gr2 = 70500; 
	MULT32(gr0,gr1,gr2);
	//sleep_loop:label;
	<sleep_loop>
	gr0--;
	if > goto sleep_loop;

	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
return;


global _halSleep:label;
<_halSleep>
	ar5 = ar7-2;
	push ar0,gr0;
	gr0 = [--ar5]; // msecs
	call halSleep;
	pop ar0,gr0;
	return;
	
	
end ".text_hal";