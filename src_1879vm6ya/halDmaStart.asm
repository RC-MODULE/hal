global _halInitSingleDMA 	: label;

extern _halMirrorOffset      : word;
extern _halCoreID             : word;
extern _halSyncro					: word;

extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;




	


begin ".text_hal"

import from led;
import from printx;
import from sleep;
import from critical;
import from periphery;

DECLARE_CS_CONST();
DECLARE_HW_CONST();
 
 global SOS:label;
<SOS>
	PRINT_32X("gr0=",gr0);
	PRINT_32X("gr7=",gr7);

	//HAL_LED_SOS(gr0,1);
	return;
/*
<_halInitSingleDMA2>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	gr0 = [_halCoreID];
	//call _halEnterCriticalSection;	
	
	
	HAL_ENTER_CRITICAL_SECTION(gr0);
	//HAL_ZERO_LOOPS(10000);
	//HAL_SLEEP(1);
	gr7 = [_halSyncro];
	gr7;
	if >= call SOS;
	
	[_halSyncro] = gr0;
	
	gr7 = 2;
	gr7+=gr0;
	//HAL_LED_SWITCH(gr7);
	//HAL_SLEEP(500);
///<WAIT_DMA>	
///	
///	//HAL_LED_ON(gr0);
///	
/////call _halExitCriticalSection;		
///	push ar1,gr1 with gr7 = false;
///	push ar2,gr2;
///	///init 
///	
///	[DMATR_Control] = gr7;	// clear the control register MDAM to fall interruption register
///	[DMARC_Control] = gr7;	// clear the control register MDAM to fall interruption register 
	gr7 = [--ar5];						// src
	ar0 = gr7 with gr7 >>= 18;			// check is src address in local space 
	gr0 = [_halMirrorOffset];
	if <>0 delayed goto SkipSrcMapping;	
		gr7 = [--ar5]; 					// dst
		ar1 = gr7  with gr7>>=18;		// 
	ar0 = ar0 + gr0;					// map to global address space
	nul;
	<SkipSrcMapping>
	if <>0 delayed goto SkipDstMapping;	// check dst src address in local space 
		gr7 = [--ar5] with gr1 = gr0;	// 32-bit size
		gr7 >>=1;						// 64-bit size
	ar1 = ar1 + gr1;					// map to global address space
	nul;
	<SkipDstMapping>	
	
	//PRINT_32X("src:",ar0);
	//PRINT_32X("dst:",ar1);
	[DMATR_Address]		= ar0;
	[DMARC_Address]		= ar1;
	
	
	
	
	[DMATR_MainCounter] = gr7; 			// set 64-bit size data for transmitter
	[DMARC_MainCounter] = gr7;			// set 64-bit size data for reciever

	gr0 = false;		
	[DMATR_AddressMode] = gr0; 			//  set one dimension mode
	[DMARC_AddressMode] = gr0; 			//  set one dimension mode
	

	gr0 =0; 							// выставляем Cpl =0
	[DMATR_Control] = gr0; 
	[DMARC_Control] = gr0; 
	
	// MDMA interruption mask bits:
	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	// Бит MIE (1-й разряд) – маска запроса на прерывание по ошибке или программной остановке: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	gr7 = 3;
	[DMATR_InterruptMask] = gr7;// masks (disable) complete and error  DMA interruptions 
	gr7 = 2;
	[DMARC_InterruptMask] = gr7;// clear (enable) MDMA interruption on complete and disable error IRQ
	
	// Interrupt controller
	gr1 = 1; 				// 0-th bit is MDMA channel
	[IRRH_CLR] = gr1;		// reset irq if another core initiated it before
	[IMRH_SET] = gr1;		// enable interrupt requests from MDMA
	[IASH_CLR] = gr1;		// clear the IAS for MDMA
	// start MDMA
	// Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	[DMARC_Control] = gr1;	//  control register start 
	[DMATR_Control] = gr1;	//  control register start 
	

<END>
	//HAL_LED_ON(3);
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
		gr7 = false;
		nop;
		nop;
		
*/		

macro USE_CS(use)
	.if (use==1);

		HAL_ENTER_CRITICAL_SECTION(gr0);
	.endif;
end   USE_CS;

macro MAPPED_ARGS(mapped)
	.if (mapped==0);
		own SkipSrcMapping:label;
		own	SkipDstMapping:label;
		gr7 = [--ar5];						// src
		ar0 = gr7 with gr7 >>= 18;			// check is src address in local space 
		gr0 = [_halMirrorOffset];
		if <>0 delayed goto SkipSrcMapping;	
			gr7 = [--ar5]; 					// dst
			ar1 = gr7  with gr7>>=18;		// 
		ar0 = ar0 + gr0;					// map to global address space
		nul;
		<SkipSrcMapping>
		if <>0 delayed goto SkipDstMapping;	// check dst src address in local space 
			gr7 = [--ar5] with gr1 = gr0;	// 32-bit size
			gr7 >>=1;						// 64-bit size
		ar1 = ar1 + gr1;					// map to global address space
		nul;
		<SkipDstMapping>	
	.endif;
	.if (mapped==1);
		ar0 = [--ar5];					// src
		ar1 = [--ar5]; 					// dst
		gr7 = [--ar5];					// 32-bit size
		gr7 >>=1;						// 64-bit size
	.endif;
end  MAPPED_ARGS;

// registers on enter must have:
// gr0 = 0;
// gr1 = 1;
macro ALIGNED_ARGS(aligned)
	.if (aligned==0);
		own Set1D_DMA :label;
		own Skip1D_DMA:label;
		gr0  = ar0;
		gr7  = ar1; 
		gr7 =  gr0 or gr7;
		with gr7=gr7<<28; 
		if =0 delayed skip Set1D_DMA with gr7 = true;
			with gr0 = false;		
			nul;
		//-------------------------------- 2D DMA mode --------------
		[DMATR_AddressMode] = gr1;					// set 2D Mode DMA 
		[DMARC_AddressMode] = gr1; 					// set 2D Mode DMA 
		[DMATR_RowCounter]  = gr1;					// row counter =1
		[DMARC_RowCounter]  = gr1 with gr7=gr1+1;	// row counter =1 
		[DMATR_Bias] 		= gr7;					// 
		delayed goto Skip1D_DMA;
		[DMARC_Bias] 		= gr7;					// 
		//-------------------------------- 1D DMA mode --------------
		<Set1D_DMA>
		[DMATR_AddressMode] = gr0; 					//  set 1D mode DMA
		[DMARC_AddressMode] = gr0; 					//  set 1D mode DMA
		<Skip1D_DMA>
	.endif;
	.if (aligned==1);
		[DMATR_AddressMode] = gr0; 					//  set 1D mode DMA
		[DMARC_AddressMode] = gr0; 					//  set 1D mode DMA
	.endif;
	
end   ALIGNED_ARGS;


macro HAL_DMA_INIT(func_name, use_cs,mapped_args,aligned_args)
global func_name:label;
<func_name>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	gr0 = [_halCoreID];	
	USE_CS(use_cs);
	//gr7 = [_halSyncro];
	//gr7;
	//if >= call SOS;
	//
	//gr0=03;
	[_halSyncro] = gr0;	// set flag that DMA is used by core gr0
	
	// read parameters and map them to mirror if needed
	MAPPED_ARGS(mapped_args);
	
	// init dma 
	[DMATR_Address]		= ar0;
	[DMARC_Address]		= ar1;
	[DMATR_MainCounter] = gr7 with gr0 = false;	// set 64-bit size data for transmitter
	[DMARC_MainCounter] = gr7 with gr1 = gr0+1;	// set 64-bit size data for reciever
	
	// init dma mode (1D-2D)
	// Note: If addresses are not aligned to 16 32-bit word boundary then 2D mode is needed to fix DMA bug
	ALIGNED_ARGS(aligned_args);
	// clear bit-complete Cpl 
	[DMATR_Control] 	= gr0; 		//   Cpl =0 . Должны сбрасывать здесь, потому что в обработчике нельзя (иначе не будет работать опрос по статусу)
	[DMARC_Control] 	= gr0; 		//  
	
	
	// MDMA interruption mask bits:
	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	// Бит MIE (1-й разряд) – маска запроса на прерывание по ошибке или программной остановке: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	gr7 = 3;
	[DMATR_InterruptMask] = gr7 with gr7--;	// masks (disable) complete and error  DMA interruptions  //gr7 = 2
	[DMARC_InterruptMask] = gr7;			// clear (enable) MDMA interruption on complete and disable error IRQ
	
	// Interrupt controller
	// gr1 = 1; 					// 0-th bit is MDMA channel
	[IRRH_CLR] 			= gr1;		// reset irq if another core initiated it before
	[IMRH_SET] 			= gr1;		// enable interrupt requests from MDMA
	[IASH_CLR] 			= gr1;		// clear the IAS for MDMA
	
	// start MDMA
	// Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	[DMARC_Control] 	= gr1;		//  control register start 
	[DMATR_Control] 	= gr1;		//  control register start 

	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
		gr7 = false;
		nop;
		nop;
		
end HAL_DMA_INIT;
				
				

 /*
<_halInitSingleDMA_>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	gr0 = [_halCoreID];
	//call _halEnterCriticalSection;	
	
	
	//HAL_ENTER_CRITICAL_SECTION(gr0);
	USE_CS(0);
	//HAL_ZERO_LOOPS(10000);
	//HAL_SLEEP(1);
	gr7 = [_halSyncro];
	gr7;
	if >= call SOS;
	
	[_halSyncro] = gr0;
	gr7 = [--ar5];						// src
	ar0 = gr7 with gr7 >>= 18;			// check is src address in local space 
	gr0 = [_halMirrorOffset];
	if <>0 delayed goto SkipSrcMapping;	
		gr7 = [--ar5]; 					// dst
		ar1 = gr7  with gr7>>=18;		// 
	ar0 = ar0 + gr0;					// map to global address space
	nul;
	<SkipSrcMapping>
	if <>0 delayed goto SkipDstMapping;	// check dst src address in local space 
		gr7 = [--ar5] with gr1 = gr0;	// 32-bit size
		gr7 >>=1;						// 64-bit size
	ar1 = ar1 + gr1;					// map to global address space
	nul;
	<SkipDstMapping>	
	
	//PRINT_32X("src:",ar0);
	//PRINT_32X("dst:",ar1);
	[DMATR_Address]		= ar0;
	[DMARC_Address]		= ar1;
	[DMATR_MainCounter] = gr7; 			// set 64-bit size data for transmitter
	[DMARC_MainCounter] = gr7;			// set 64-bit size data for reciever
	
	gr0  = ar0;
	gr1  = ar1; 
	gr7 = gr0 or gr1;
	with gr7=gr7<<28; 
	if =0 delayed skip Set1D_DMA with gr7 = true;
		with gr0 = false;		
		with gr1 = -gr7;
	//-------------------------------- 2D DMA mode --------------
	<MatrixDMA>

	[DMATR_AddressMode] = gr1;					// set 2D Mode DMA 
	[DMARC_AddressMode] = gr1; 					// set 2D Mode DMA 
	[DMATR_RowCounter]  = gr1;					// row counter =1
	[DMARC_RowCounter]  = gr1 with gr7=gr1+1;	// row counter =1 
	[DMATR_Bias] 		= gr7;	
	delayed goto StartDMA;
	[DMARC_Bias] 		= gr7;	
	//-------------------------------- 1D DMA mode --------------
	<Set1D_DMA>
	[DMATR_AddressMode] = gr0; 					//  set 1D mode DMA
	[DMARC_AddressMode] = gr0; 					//  set 1D mode DMA

	//----------------- start ----------------------------------
	<StartDMA>
	[DMATR_Control] 	= gr0; 		//  ? Cpl =0
	[DMARC_Control] 	= gr0; 		//  ?
	
	// MDMA interruption mask bits:
	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	// Бит MIE (1-й разряд) – маска запроса на прерывание по ошибке или программной остановке: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	gr7 = 3;
	[DMATR_InterruptMask] = gr7;	// masks (disable) complete and error  DMA interruptions 
	gr7 = 2;
	[DMARC_InterruptMask] = gr7;	// clear (enable) MDMA interruption on complete and disable error IRQ
	
	// Interrupt controller
	// gr1 = 1; 					// 0-th bit is MDMA channel
	[IRRH_CLR] 			= gr1;		// reset irq if another core initiated it before
	[IMRH_SET] 			= gr1;		// enable interrupt requests from MDMA
	[IASH_CLR] 			= gr1;		// clear the IAS for MDMA
	
	// start MDMA
	// Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	[DMARC_Control] 	= gr1;		//  control register start 
	[DMATR_Control] 	= gr1;		//  control register start 
	

<END>
	//HAL_LED_ON(3);
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
		gr7 = false;
		nop;
		nop;
		
*/	

	
//global _halInitSingleDMA:label;
//<_halInitSingleDMA>

HAL_DMA_INIT(_halInitSingleDMA,1,0,0);

HAL_DMA_INIT(_halDmaStart   ,0,0,0);
HAL_DMA_INIT(_halDmaStartA  ,0,0,1);
HAL_DMA_INIT(_halDmaStartM  ,0,1,0);
HAL_DMA_INIT(_halDmaStartMA ,0,1,1);
					
HAL_DMA_INIT(_halDmaStartC  ,1,0,0);
HAL_DMA_INIT(_halDmaStartCA ,1,0,1);
HAL_DMA_INIT(_halDmaStartCM ,1,1,0);
HAL_DMA_INIT(_halDmaStartCMA,1,1,1);


//global _halDmaInitCAM:label;
//<_halDmaInitCAM>

//HAL_DMA_INIT(_halDmaInit,0,0,0);
//HAL_DMA_INIT(_halDmaInitC,1,0,0);
/*
<_halInitSingleDMA>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	gr0 = [_halCoreID];	
	USE_CS(1);
	//gr7 = [_halSyncro];
	//gr7;
	//if >= call SOS;
	//
	[_halSyncro] = gr0;
	
	// read parameters and map them to mirror if needed
	NEED_TO_MAP(1);
	
	// init dma 
	[DMATR_Address]		= ar0;
	[DMARC_Address]		= ar1;
	[DMATR_MainCounter] = gr7 with gr0 = false;	// set 64-bit size data for transmitter
	[DMARC_MainCounter] = gr7 with gr1 = gr0+1;	// set 64-bit size data for reciever
	
	// init dma mode (1D-2D)
	// Note: If addresses are not aligned to 16 32-bit word boundary then 2D mode is needed to fix DMA bug
	NEED_2D(1);
	// clear bit-complete Cpl 
	[DMATR_Control] 	= gr0; 		//   Cpl =0 . Должны сбрасывать здесь, потому что в обработчике нельзя (иначе не будет работать опрос по статусу)
	[DMARC_Control] 	= gr0; 		//  
	
	
	// MDMA interruption mask bits:
	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	// Бит MIE (1-й разряд) – маска запроса на прерывание по ошибке или программной остановке: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	gr7 = 3;
	[DMATR_InterruptMask] = gr7 with gr7--;	// masks (disable) complete and error  DMA interruptions  //gr7 = 2
	[DMARC_InterruptMask] = gr7;			// clear (enable) MDMA interruption on complete and disable error IRQ
	
	// Interrupt controller
	// gr1 = 1; 					// 0-th bit is MDMA channel
	[IRRH_CLR] 			= gr1;		// reset irq if another core initiated it before
	[IMRH_SET] 			= gr1;		// enable interrupt requests from MDMA
	[IASH_CLR] 			= gr1;		// clear the IAS for MDMA
	
	// start MDMA
	// Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	[DMARC_Control] 	= gr1;		//  control register start 
	[DMATR_Control] 	= gr1;		//  control register start 

<END>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
		gr7 = false;
		nop;
		nop;
		
		*/
				
				
		
end ".text_hal";


	
///	//gr0 = ar2;
///	//gr0 = gr0 >> 29;//check is the address to wrt data DDR or not
///	//if =0 delayed goto PASS_DOUBLE_DEMENTION_SETUP;
///	//check arguments for are they aligned or not
///		gr0 = ar2;
///		gr7 = ar0;
///	gr7 = gr7 or gr0;
///	gr7 = gr7 << 28;
///	if =0 delayed goto PASS_DOUBLE_DEMENTION_SETUP;
///	////// double demention mode was set up to prevent user from hardwaer bug in MDMA,
///	////// becoze of error in FSM inside of MDMA addresses have to be both align as 4 the last significant bits = 0 
///	////// otherwise mode is double demention with bias = 2 rowcounter 1 to emulate sigle demention mode use double demention mode
///	////// this trick allows to escape reminder/quotient mode switching inside FSM
///	////// this bug takes effect only when dst address belows DDR but in some case we nouted bug can take effect even if dst address is shmem
///	//address mode is double demention
///	ar1 = 1 with gr7 = true;
///	[DMATR_AddressMode] = gr7;//double dimention mode
///	[DMARC_AddressMode] = gr7;//double dimention mode
///	//row counter
///	[DMATR_RowCounter] = ar1;//wrt row counter to mdma
///	[DMARC_RowCounter] = ar1;//wrt row counter to mdma
///	ar1++;
///
///	[DMATR_Bias] = ar1;//bias src
///	delayed goto START with gr7 = false;
///	[DMARC_Bias] = ar1;//bias dst
///
///<PASS_DOUBLE_DEMENTION_SETUP>
///	//address mode is sequential