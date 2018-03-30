global _halSetCallbackDMA : label;
global _readCallback      : label;
global _halInitDMA        : label;
global _halStatusDMA      : label;
global _haltest						: label;
global _halEnbExtInt			: label;
global _halDisExtInt			: label;
global _halMaskIntContMdma_mc12101 : label;
global _GetPswr 					: label;
global _SetFlagDMA				: label;
nobits ".nobits"
 GR7:word;
 AR5:word;
 global _flag_of_pack_DMA : word;
end ".nobits";


begin ".text"
<dummy>
	return;
	
<Lint_6407>
		[AR5] = ar5;
	delayed goto _CALL_BACK;
		[GR7]=gr7;
////////////////////////////////////////////////
<_CALL_BACK>
	pswr clear 01e0h;
	//the code below was written according the prescription of how to clear IAS register on the right was;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	gr7 = false;
	[1001000Ah] = gr7;//clear the control bit to fall signal of interaption 
	[1001001Ah] = gr7;//clear the control bit to fall signal of interaption
	gr7 = [10010000h];//read any MDMA register
	gr7 = gr7 << 1;//perform the arithmetic operation
	[40000406h] = gr7;//write the result into any reserved register of interaption contorl unit
	gr7 = 1;
	[4000045ch] = gr7;//clear the IAS for MDMA
<change_addr4call>
	call dummy;
	ar5=[AR5];
 	delayed	ireturn;
	gr7=[GR7];
///////////////////////////////////////////////////
<_halSetCallbackDMA>
	ar5 = ar7 - 2;
	push ar0,gr0;	
	push ar1,gr1;	
	gr1 = [--ar5];
	ar5 = Lint_6407;
	ar1 = 00000120h with gr1;
	if <>0 delayed goto PASS_SET_CALLBACK;
		ar5 = change_addr4call + 1;
	gr1 = dummy;
<PASS_SET_CALLBACK>
	[ar5] = gr1;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
////////////////////////////////////////////////////
<_halEnbExtInt>
	pswr set 040h;
	return;
<_halDisExtInt>
	pswr clear 01e0h;
	return;	
////////////////////////////////////////////////////
<_halInitDMA>
	gr7 = pswr;	
	push ar0,gr0 with gr7 >>= 6;
	push ar1,gr1 with gr7 <<= 31;
	if =0 delayed goto END;
		gr7 = 10;
	gr7 = false;
	[_flag_of_pack_DMA] = gr7;	
	ar5 = Lint_6407;
	ar1 = 00000120h;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	gr1 = dummy;
	[change_addr4call + 1] = gr1;
<END>	
	pop ar1,gr1;
	pop ar0,gr0;
	return;
////////////////////////////////////////////////////
<_readCallback>
	ar5 = [change_addr4call+1];
	return;
////////////////////////////////////////////////////
<_halStatusDMA>
	push ar0,gr0;
	gr7 = [10010000h];//read counter
	gr0 = [_flag_of_pack_DMA];
	gr7 = gr0 or gr7;
	pop ar0,gr0;
	return;

<_halMaskIntContMdma_mc12101>
	gr7 = 1;
	[40000448h] = gr7;
	return;

<_GetPswr>
	delayed return;		
	gr7 = pswr;
	nop;

<_SetFlagDMA>
	ar5 = ar7 - 2;
	gr7 = [--ar5];
	delayed return;
	[_flag_of_pack_DMA] = gr7;

end ".text";	

	