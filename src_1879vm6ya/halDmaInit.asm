
global _halSetCallbackDMA :label;
global _halSetDummyCallbackDMA :label;
global _halGetCallbackDMA :label;
//global _readCallback      :label;
global _halOpenDMA        :label;
global _halStatusDMA      :label;
global _haltest						:label;
global _halEnbExtInt			:label;
global _halDisExtInt			:label;
global _halMaskIntContMdma_mc12101 :label;
global _GetPswr 					:label;
global _SetFlagDMA				:label;

global _halLockDMA 				:label;
global _halUnlockDMA 			:label;
global _halIsBusyDMA 		  :label;

global _halWhereMirror     :label;
global _halGetCoreId			:label;
global _halSetMirror      :label;
global _halReadCoreID 		:label;
global _halOpenStatusDMA  :label;
extern _halSyncro					:word;


nobits ".nobits"
global GR7											:word[2];
global AR5											:word[2];
global GR0											:word[2];
 global _flag_of_pack_DMA :word;
 global _halDmaUserCallbackPtr     :word;
 global _halMirrorOffset     :word;
 global _halCoreID            :word;
end ".nobits";

extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;


begin ".text_hal"


import from led;
import from printx;
import from critical;
import from sleep;
import from periphery;

DECLARE_CS_CONST();
DECLARE_HW_CONST();

extern SOS:label;
<dummy>
	return;
	
global _TEST:label;
<_TEST>	
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	//gr7=3;
	//[IMRH]=gr7;
	//gr7 = [DMATR_InterruptMask];
	//HAL_LED(gr7);
	gr7 = [40000000h];
	with gr7 >>= 24;//detecting core the programm uses
	//gr7 = [--ar5];
	//HAL_LED_SOS(0);
	//HAL_LED_SOS(gr7,1);
	
	
	pop ar1,gr1;
	pop ar0,gr0;
	return;
	

	
	
<Lint_6407>
		//// the limit of commands in interruption vector is 4 64 bit-wise words !!!
		[GR0] = ar0,gr0 with gr0=false noflags;
		[GR7] = gr7 with gr7 = gr0+1 noflags;
	delayed goto halDmaHandler;
		[AR5] = ar5,gr5;		

<Vector_CpuIrq0>
		//// the limit of commands in interruption vector is 4 64 bit-wise words !!!
		[GR0] = ar0,gr0 with gr0=false noflags;
		[GR7] = gr7 with gr7 = gr0+1 noflags;
	delayed goto halCpuIrqHandler0;
		[AR5] = ar5,gr5;		

////////////////////////////////////////////////
//global _halDmaSetHandler:label;
//<_halDmaSetHandler>
//
//global _halDmaChangeHandler:label;
//<_halDmaChangeHandler>
//	//ar5 = [--ar5]
//	
//global _halDmaRestoreHandler:label;
//<_halDmaRestoreHandler>

<halCpuIrqHandler0>

	call SOS;
	
	ar5,gr5 = [AR5];
	ar0,gr0 = [GR0];
 	delayed	ireturn;
		gr7 = [GR7];
		nop;

<halDmaHandler>
	//the code below was written according the prescription of how to clear IAS register on the right way;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	// 0. 
	gr0 = [_halCoreID]; 
	gr7 = [_halSyncro]; 	// read  stateDMA from	struct SyncBuf , where
	gr7 - gr0;
	//if <>0 
	//call SOS;
	//PRINT_32X("core:",gr0);
	
	//gr7 = 4;
	//gr7+= gr0;
	//HAL_LED_SWITCH(gr7);
	//HAL_LED_BLINK(gr7,20,100);
	//HAL_LED4LO(1);
	
							// -1  free	
							// 0 used by core 0
							// 1 used by core 1
	//PRINT_32X("gr7:",gr7);
	//gr7 - gr0;
	//if <>0 goto SKIP_CALLBACK;
	// page 142 Magic script
	// При работе с устройствами, которые работают в режиме программного подтверждения следует обеспечить, чтобы команда снятия запроса в запрашивающем устройстве выполнялась строго раньше, чем команда снятия бита статуса прерывания (IAS) в контроллере прерываний. Рекомендуется следующий алгоритм программного подтверждения: 
	//gr7 = true noflags;

	// Запрос на прерывание по нормальному завершению работы контроллера ПДП  выставляется,
	// когда выполняется следующее условие: 
	// установлен бит Cpl регистра DMARC_Control и 
	// сброшен бит маски MIC в регистре DMARC_InterruptMask  (в т.ч. в конце процесса передачи).
	// 1.
	//gr0 = false;
	//[DMATR_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	//[DMARC_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	// Здесь нельзя сбрасывать Cpl потому что Cpl может  опрашиваться отдельной функцией
	gr7 = 1;
	[DMATR_InterruptMask] = gr7;	// mask(disable) interrupt request in DMATR
	[DMARC_InterruptMask] = gr7;	// mask(disable) interrupt request in DMARC
	// 2.
	gr7 = [DMATR_MainCounter];		// read any MDMA register
	// 3.
	gr7 = gr7 + 1 noflags;			// perform any logic or arithmetic operation
	// 4.
	[IRESERVED] = gr7;				// write the result into any reserved register of interruption controller
	// 5.
	gr7 = 1;
	[IASH_CLR] = gr7;				// clear IAS for MDMA

	
	gr7 = 1;						// 0th-bit is MDMA channel 
	[IMRH_CLR] = gr7;				// mask(disable) interrupt requests from MDMA
	
	
	gr7 = true noflags;
	[_halSyncro] = gr7;				// write 0xffffffff to flag of DMA means DMA is free
	gr7 = [_halDmaUserCallbackPtr]; //get user callback address
	with gr7;
	if <>0 call gr7;
	
	ar5,gr5 = [AR5];
	ar0,gr0 = [GR0];
 	delayed	ireturn;
		gr7 = [GR7];
		nop;




<halDmaHandlerC>
	//the code below was written according the prescription of how to clear IAS register on the right way;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	// 0. 
	gr0 = [_halCoreID]; 
	gr7 = [_halSyncro]; 	// read  stateDMA from	struct SyncBuf , where
	gr7 - gr0;
	if <>0 call SOS;
	
	//gr7 = 4;
	//gr7+= gr0;
	// page 142 Magic script
	// При работе с устройствами, которые работают в режиме программного подтверждения следует обеспечить, чтобы команда снятия запроса в запрашивающем устройстве выполнялась строго раньше, чем команда снятия бита статуса прерывания (IAS) в контроллере прерываний. Рекомендуется следующий алгоритм программного подтверждения: 
	//gr7 = true noflags;

	// Запрос на прерывание по нормальному завершению работы контроллера ПДП  выставляется,
	// когда выполняется следующее условие: 
	// установлен бит Cpl регистра DMARC_Control и 
	// сброшен бит маски MIC в регистре DMARC_InterruptMask  (в т.ч. в конце процесса передачи).
	// 1.
	//gr0 = false;
	//[DMATR_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	//[DMARC_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	// Здесь нельзя сбрасывать Cpl потому что Cpl может  опрашиваться отдельной функцией
	gr7 = 1;
	[DMATR_InterruptMask] = gr7;	// mask(disable) interrupt request in DMATR
	[DMARC_InterruptMask] = gr7;	// mask(disable) interrupt request in DMARC
	// 2.
	gr7 = [DMATR_MainCounter];		// read any MDMA register
	// 3.
	gr7 = gr7 + 1 noflags;			// perform any logic or arithmetic operation
	// 4.
	[IRESERVED] = gr7;				// write the result into any reserved register of interrupt controller
	// 5.
	gr7 = 1;
	[IASH_CLR] = gr7;				// clear IAS for MDMA

	
	gr7 = 1;						// 0th-bit is MDMA channel 
	[IMRH_CLR] = gr7;				// mask(disable) interrupt requests from MDMA
	
	
	gr7 = true noflags;
	[_halSyncro] = gr7;				// write 0xffffffff to flag of DMA means DMA is free
	gr7 = [_halDmaUserCallbackPtr]; //get user callback address
	with gr7;
	if <>0 call gr7;
	
	gr0	= [_halCoreID];
	HAL_EXIT_CRITICAL_SECTION(gr0);

	ar5,gr5 = [AR5];
	ar0,gr0 = [GR0];
 	delayed	ireturn;
		gr7 = [GR7];
		nop;





///////////////////////////////////////////////////
global _halDmaSetCallback:label;
<_halDmaSetCallback>
<_halSetCallbackDMA>
	ar5 = ar7 - 2;
	ar5 = [--ar5];
	delayed return;
		[_halDmaUserCallbackPtr] = ar5;
	
///////////////////////////////////////////////////
//<_halSetDummyCallbackDMA>
//	gr7 = dummy;
//	delayed return;
//		nul;
//		[_halDmaUserCallbackPtr] = gr7;

///////////////////////////////////////////////////
global _halDmaGetCallback:label;
<_halDmaGetCallback>
<_halGetCallbackDMA>
	delayed return;
		nul;
		ar5=[_halDmaUserCallbackPtr];
	
	
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
<_halOpenDMA>
global _halDmaInit:label;
<_halDmaInit>
	////this function write the programm at Lint_6407 label 
	////into interruption vector of interruption controller
	gr7 = [40000000h];
	push ar0,gr0 		with gr7 >>= 24;			//detecting core id
	[_halCoreID] = gr7 	with gr7++;
	push ar1,gr1		with gr7 = gr7<<18;
	[_halMirrorOffset] = gr7 with gr7 = false;		// set offset to mirror 
	
	ar5 = Lint_6407;
	ar1 = VECTOR_DMA;
	//gr7 = pswr;
	//simular lines of code below 
	//are copping the programm to interruption vector of interruption controller
	//the programm is next
	//	[AR5] = ar5;
	//	[GR0] = gr0;
	//delayed goto CALL_BACK;
	//	[GR7] = gr7;
	ar0,gr0 = [ar5++]; 
	[ar1++] = ar0,gr0;//
	ar0,gr0 = [ar5++];//
	[ar1++] = ar0,gr0;//
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	gr1 = false;
	[_halDmaUserCallbackPtr] = gr1;
//turn on mask in interruption controller


	// set initial interrupt handler 
	ar5 = Vector_CpuIrq0;
	ar1 = VECTOR_CPU_IRQ0;
	ar0,gr0 = [ar5++]; 
	[ar1++] = ar0,gr0;//
	ar0,gr0 = [ar5++];//
	[ar1++] = ar0,gr0;//
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	ar0,gr0 = [ar5++];
	[ar1++] = ar0,gr0;
	
	
	
	gr7 = 1;

	gr0 = false;
	//[DMATR_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	//[DMARC_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	
	
	gr0 = 1;				// 0th-bit is MDMA channel 
	[IMRH_CLR] = gr0;		// mask(disable) interrupt requests from MDMA
	gr0 = 1<<12;
	[IMRL_SET]=gr0;			// enable  interrupt from nmc1
	
	//enable extern interruptions at core
	intr clear 40h;
	pswr set 040h;	
	

	gr7=[_halCoreID];
	
	pop ar1,gr1;
	pop ar0,gr0;
	return;


global _halDmaInitC:label;
<_halDmaInitC>
	call _halDmaInit;
	gr7=halDmaHandlerC;
	delayed return;
		[VECTOR_DMA_HANDLER]=gr7;

////////////////////////////////////////////////////
//<_readCallback>
//	delayed return;
//		ar5 = [_halDmaUserCallbackPtr];
//		nop;
////////////////////////////////////////////////////

<_halStatusDMA>
	push ar0,gr0;
	
	gr0 = [_halCoreID]; 
	ar0 = 4;
	ar0+= gr0;
	//HAL_LED_ON(ar0);

	gr7=  -gr0; 
	gr7 = [DMARC_Control]; 
	//gr0 = [_flag_of_pack_DMA];
	//gr7 = gr0 or gr7;
	gr0 = 2;
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. 
	// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	gr7 = gr0 and gr7;
	//gr0 = gr0 and gr7;
	//with gr0;
	//if =0 skip exit; 
	
	//gr7= gr7 xor gr0;
	//[1001_001Ah]=gr7;
	
	<exit>
	//gr7 = gr0;

	//HAL_LED_OFF(ar0);


	pop ar0,gr0;
	return;

global _halDmaIsCompleted:label;
<_halDmaIsCompleted>
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. 
	// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	gr7 = [DMARC_Control]; 
	delayed return ;
		gr7 <<=30;
		gr7 >>=31;
		nul;


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
	gr7 = [_halCoreID];
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

<_halWhereMirror>
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
		gr7 = [_halCoreID];
		nop;

<_halSetMirror>
	gr7 = [40000000h];
	gr7 >>= 24;
	if =0 delayed goto SKIP;
		gr7 = 40000h;
	gr7 = 80000h;	
	<SKIP>
	delayed return;
		[_halMirrorOffset] = gr7;
		nop;

<_halOpenStatusDMA>
	gr7 = [40000000h];
	gr7 >>= 24;
	if =0 delayed goto SKIP_;
		gr7 = 40000h;
	gr7 = 80000h;	
	<SKIP_>
	[_halMirrorOffset] = gr7;
	gr7 = false;
	delayed return;
		[_flag_of_pack_DMA] = gr7;
		nop;

end ".text_hal";	

	

