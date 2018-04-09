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
global _halIsBusyDMA 		  : label;

global _halWereMirror     : label;
global _halGetCoreId			: label;
global _halSetMirror      : label;
global _halReadCoreID 		: label;
extern _halSyncro					: word;

nobits ".nobits"
 GR7:word;
 AR5:word;
 GR0:word;
 global _flag_of_pack_DMA : word;
 global callback_addr : word;
 global mirror_offset : word;
 global coreID : word;
end ".nobits";


begin ".text"
<dummy>
	return;
	
<Lint_6407>
		//// the limit of command in interruption vector is 4 64 bit wise words
		[AR5] = ar5;
		[GR0] = gr0;
	delayed goto CALL_BACK;
		[GR7] = gr7;
////////////////////////////////////////////////
<CALL_BACK>
	//pswr clear 01e0h;//disable extern interruption for imu to avoid interruption inside interruption
	//the code below was written according the prescription of how to clear IAS register on the right was;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	gr7 = [_halSyncro];
	if < delayed goto CLEAR_IAS; 
		gr0 = [coreID];
	gr7 = gr7 - gr0;
	if <>0 goto SKIP_CALLBACK;
<CLEAR_IAS>
	gr7 = true;
	[1001000Ch] = gr7;// mask interruptions of MDMA is masked to fall request of DMA to interruption controller
	[1001001Ch] = gr7;// mask interruptions of MDMA is masked to fall request of DMA to interruption controller
	gr7 = [10010000h];//read any MDMA register
	gr7 = gr7 << 1;//perform the arithmetic operation
	[40000406h] = gr7;//write the result into any reserved register of interaption contorl unit
	gr7 = 1;
	[4000045ch] = gr7;//clear the IAS for MDMA
	ar5 = [callback_addr] with gr7 = true;//set callback address
	[_halSyncro] = gr7;// write 0xffffffff to flag of DMA means DMA is free
	call ar5;
<SKIP_CALLBACK>	
	ar5 = [AR5];
	gr0 = [GR0];
 	delayed	ireturn;
	gr7 = [GR7];
///////////////////////////////////////////////////
<_halSetCallbackDMA>
	ar5 = ar7 - 2;
	push ar1,gr1;	
	gr1 = [--ar5];//read call back 
	gr1;
	if <>0 goto SKIP_SET_DUMMY_CALLBACK; //in case call back = 0 set dummy;
	gr1 = dummy;
<SKIP_SET_DUMMY_CALLBACK>
	[callback_addr] = gr1;
	pop ar1,gr1;
	return;
////////////////////////////////////////////////////
<_halEnbExtInt>
	delayed return;
		pswr set 040h;
		nop;
<_halDisExtInt>
	delayed return;	
		pswr clear 01e0h;
		nop;
////////////////////////////////////////////////////
<_halInitDMA>
	////this function write the programm at Lint_6407 label 
	////into interruption vector of interruption controller
	gr7 = [40000000h];
	push ar0,gr0 with gr7 >>= 24;//detecting core the programm uses
	[coreID] = gr7;
	if <>0 delayed goto SKIP_INIT;
		gr7 = 80000h;
		gr7 >>= 1;		
<SKIP_INIT>
	[mirror_offset] = gr7;	
	push ar1,gr1 with gr7 = false;
	[_flag_of_pack_DMA] = gr7;
	ar5 = Lint_6407;
	ar1 = 00000120h;
	gr7 = pswr;
	ar0,gr0 = [ar5++]; 
	[ar1++] = ar0,gr0 with gr7 >>= 5;
	ar0,gr0 = [ar5++] with gr7 <<= 31;
	[ar1++] = ar0,gr0 with gr7 >>= 31;
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	gr1 = dummy;
	[callback_addr] = gr1;
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
	if <>0  delayed goto SKIP_UNLOCK;
		pop ar0,gr0;
		nop;
	gr0 = true;
	[_halSyncro] = gr0;
	<SKIP_UNLOCK>
	return;

<_halMaskIntContMdma_mc12101>
	gr7 = 1;
	delayed return;
		[40000448h] = gr7;
		nop;

<_GetPswr>
	delayed return;		
		gr7 = pswr;
		nop;
		nop;

<_SetFlagDMA>
	ar5 = ar7 - 2;
	gr7 = [--ar5];
	delayed return;
		[_flag_of_pack_DMA] = gr7;
		nop;

///////////////////////////////////////////lock functions
//global _halLockDMA 				: label;
//global _halUnlockDMA 			: label;
//global _halIslockedDMA 		: label;

<_halLockDMA>
	gr7 = [coreID];
	delayed return;
	 [_halSyncro] = gr7;
	 nop;

<_halUnlockDMA>
	gr7 = true;
	delayed return;
	 [_halSyncro] = gr7;
	 nop;

<_halIsBusyDMA>
	delayed return;		
	 gr7 = [_halSyncro];
	 nop;

<_halWereMirror>
	gr7 = [40000000h];
	gr7 >>= 24;
	if =0 delayed return;
		gr7 = 40000h;
	delayed return; 
	 gr7 = 80000h;
	 nop;

<_halGetCoreId>
	gr7 = [40000000h];
	delayed return; 		
		gr7 >>= 24;
		nop;
		nop;

<_halReadCoreID>
	delayed return;
		gr7 = [coreID];
		nop;

<_halSetMirror>
	gr7 = [40000000h];
	gr7 >>= 24;
	if =0 delayed goto SKIP;
		gr7 = 40000h;
	gr7 = 80000h;	
	<SKIP>
	delayed return;
		[mirror_offset] = gr7;
		nop;



end ".text";	

	