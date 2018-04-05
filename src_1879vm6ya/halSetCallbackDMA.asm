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

global _halLockDMA 				: label;
global _halUnlockDMA 			: label;
global _halIslockedDMA 		: label;

nobits ".nobits"
 GR7:word;
 AR5:word;
 global _flag_of_pack_DMA : word;
 global _locked_DMA : word;
 global callback_addr : word;
end ".nobits";


begin ".text"
<dummy>
	return;
	
<Lint_6407>
		[AR5] = ar5;
	delayed goto CALL_BACK;
		[GR7]=gr7;
////////////////////////////////////////////////
<CALL_BACK>
	//pswr clear 01e0h;//disable extern interruption for imu to avoid interruption inside interruption
	//the code below was written according the prescription of how to clear IAS register on the right was;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	//gr7 = false;
	//[1001000Ah] = gr7;//clear the control bit to fall signal of interaption 
	//[1001001Ah] = gr7;//clear the control bit to fall signal of interaption
	gr7 = true;
	[1001000Ch] = gr7;// mask interruptions of MDMA
	[1001001Ch] = gr7;// mask interruptions of MDMA
	gr7 = [10010000h];//read any MDMA register
	gr7 = gr7 << 1;//perform the arithmetic operation
	[40000406h] = gr7;//write the result into any reserved register of interaption contorl unit
	gr7 = 1;
	[4000045ch] = gr7;//clear the IAS for MDMA
<change_addr4call>
	ar5 = [callback_addr];//set callback address
	gr7 = [_flag_of_pack_DMA];
	[_locked_DMA] = gr7;// write locked flag of DMA
	call ar5;
	ar5=[AR5];
 	delayed	ireturn;
	gr7=[GR7];
///////////////////////////////////////////////////
<_halSetCallbackDMA>
	ar5 = ar7 - 2;
	push ar0,gr0;	
	push ar1,gr1;	
	gr1 = [--ar5];//read call back 
	gr1;
	if <>0 delayed goto SKIP_SET_DUMMY_CALLBACK; //in case call back = 0 set dummy;
		ar5 = change_addr4call + 1;
	gr1 = dummy;
<SKIP_SET_DUMMY_CALLBACK>
	[callback_addr] = gr1;
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
	////this function write the programm at Lint_6407 label 
	////into interruption vector of interruption controller
	gr7 = pswr;	
	push ar0,gr0 with gr7 >>= 6;
	push ar1,gr1 with gr7 <<= 31;
	if =0 delayed goto END;
		gr7 = 10;
	gr7 = false;
	[_flag_of_pack_DMA] = gr7;
	[_locked_DMA] = gr7;
	ar5 = Lint_6407;
	ar1 = 00000120h;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	ar0,gr0=[ar5++];
	[ar1++]=ar0,gr0;
	gr1 = dummy;
	[callback_addr] = gr1;
<END>	
	pop ar1,gr1;
	pop ar0,gr0;
	return;
////////////////////////////////////////////////////
<_readCallback>
	ar5 = [callback_addr];
	return;
////////////////////////////////////////////////////
<_halStatusDMA>
	push ar0,gr0;
	gr7 = [1001001Ah];
	gr0 = [_flag_of_pack_DMA];
	gr7 = gr0 or gr7;
	gr0 = 2;
	gr7 = gr0 xor gr7;
	if =0 call _halUnlockDMA;
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

///////////////////////////////////////////lock functions
//global _halLockDMA 				: label;
//global _halUnlockDMA 			: label;
//global _halIslockedDMA 		: label;

<_halLockDMA>
	gr7 = true;
	delayed return;
	 [_locked_DMA] = gr7;

<_halUnlockDMA>
	gr7 = false;
	delayed return;
	 [_locked_DMA] = gr7;

<_halIslockedDMA>
	delayed return;		
	 gr7 = [_locked_DMA];

end ".text";	

	