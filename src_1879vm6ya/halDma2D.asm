
extern _halMirrorOffset      	: word;
extern _halCoreID             	: word;
extern _halSyncro				: word;
extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;
extern GR7						:word[2];
extern AR5						:word[2];
extern GR0						:word[2];
 
begin ".text_hal"

halDmaOriginHandler:word;
halDmaToDoSize64   :word=0;
halDmaSrcStride    :word;
halDmaDstStride    :word;
halDmaSrc   	   :word;
halDmaDst  	       :word;
halDmaWidth64      :word;


import from led;
import from printx;
import from sleep;
import from critical;
import from periphery;


DECLARE_CS_CONST();
DECLARE_HW_CONST();
 

//<SOS>
//	HAL_LED_SOS(gr0,1);
//	return;
counter:word=0;
<halDmaHandler2D>
	//gr0=[counter];
	//HAL_LED_ON(gr0);
	//gr0++;
	//[counter]=gr0;
	
	//gr0 = false;
	//[DMATR_InterruptMask] = gr7;		// mask(disable) interrupt request in DMATR
	//[DMARC_InterruptMask] = gr7;		// mask(disable) interrupt request in DMARC
	[DMATR_Control]   	  = gr0; 		// clear bit-complete Cpl 
	[DMARC_Control] 	  = gr0; 		// clear bit-complete Cpl 
	
	gr0 = [DMATR_MainCounter];			// read any MDMA register
	gr0 = gr0 + 1 noflags;				// perform any logic or arithmetic operation
	[IRESERVED] = gr0 with gr0=false;	// write the result into any reserved register of interruption controller
	[IASH_CLR]  = gr7;					// clear IAS for MDMA

	gr0=[halDmaSrc] ;
	gr7=[halDmaSrcStride];
	gr5=[halDmaDst]			with gr0+=gr7;
	gr7=[halDmaDstStride];
	gr7=[halDmaToDoSize64]	with gr5+=gr7;
	[halDmaSrc]		= gr0;
	[DMATR_Address]	= gr0;
	gr0=[halDmaWidth64];
	[halDmaDst]			= gr5;
	[DMARC_Address]		= gr5;	
	[DMATR_MainCounter]	= gr0 with gr5 = gr1;
	[DMARC_MainCounter]	= gr0 with gr7 = gr7-gr0;
	if >  delayed goto ContinueDMA 	with gr1 = false;
		[halDmaToDoSize64]=gr7		with gr1++;

	// final dma session
	gr7=[halDmaOriginHandler];
	[VECTOR_DMA_HANDLER]=gr7;
	
	// MDMA interruption mask bits:
	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	// Бит MIE (1-й разряд) – маска запроса на прерывание по ошибке или программной остановке: 
	// 0 – запрос разрешён;
	// 1 – запрос замаскирован.
	//gr7 = 3;
	//[DMATR_InterruptMask] = gr7 with gr7--;	// masks (disable) complete and error  DMA interruptions  //gr7 = 2
	//[DMARC_InterruptMask] = gr7;			// clear (enable) MDMA interruption on complete and disable error IRQ
	
	<ContinueDMA>
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
	
	//pop  ar0,gr0;
	ar5,gr5 = [AR5] with gr1=gr5;
	ar0,gr0 = [GR0];
 	delayed	ireturn;
		gr7 = [GR7];
		nop;

	
	
	
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
			nul;							//
		ar1 = ar1 + gr1;					// map to global address space
		nul;
		<SkipDstMapping>	
	.endif;
	.if (mapped==1);
		ar0 = [--ar5];						// src
		ar1 = [--ar5]; 						// dst
		gr7 = [--ar5];						// 32-bit size
	.endif;
end  MAPPED_ARGS;

// registers on enter must have:
// ar0 = src
// ar1 = dst
// gr0 = 0;
// gr1 = 1;
// gr7 = size in 32-bit words
macro ALIGNED_ARGS(aligned)
	.if (aligned==0);
		own MatrixDMA:label;
		own StartDMA:label;
		gr2  = ar0;										// src
		gr3  = ar1	with gr0 = gr7 or gr2; 				// dst
		gr2=[--ar5] with gr0 = gr0 or gr3;				// width
		gr3=[--ar5]	with gr0 = gr0 or gr2;				// src stride32
		gr4=[--ar5] with gr0 = gr0 or gr3;				// dst stride32
					with gr0 = gr0 or gr4;
					with gr0 = gr0<<28; 				// check low 4 bits
		if =0 delayed goto MatrixDMA with gr7>>=1;		// gr7 = size in 64-bit words
			[DMATR_AddressMode] = gr1 with gr0=false;	// set 2D Mode DMA 
		//-- Row-wise DMA --------------------- 2D slow (64-bit word) DMA mode --------------
		[DMARC_AddressMode] = gr1 with gr2>>=1;			// set 2D Mode DMA  
		[DMATR_MainCounter] = gr2;						// main counter = width/2 (in 64-bit words)
		[DMARC_MainCounter] = gr2;						// main counter = width/2 (in 64-bit words) ; 
		[DMATR_RowCounter]  = gr1;						// row counter =1 
		[DMARC_RowCounter]  = gr1 with gr0=gr1+1;		// row counter =1 
		[DMATR_Bias] 		= gr0 with gr7=gr7-gr2;		// bias=2 ; gr7 = number of 64-bit words to copy in next dma sessions
		if =0 delayed goto StartDMA;					// if =0 it means height=1 and only one start  DMA is needed
			[DMARC_Bias] 	= gr0 with gr0=false;		// bias=2

		// here height is > 1 . We need special row-wise dma handler 
		[halDmaToDoSize64]  = gr7;
		[halDmaWidth64]		= gr2;
		[halDmaSrcStride]  	= gr3;
		[halDmaDstStride]  	= gr4;
		[halDmaSrc]		  	= ar0;
		[halDmaDst]  		= ar1;
		ar5 = [VECTOR_DMA_HANDLER];						// read current dma handler
		gr7 = halDmaHandler2D;							// special dma handler for row-wise 2D DMA
		[halDmaOriginHandler]= ar5;						// save origin handler
		delayed goto StartDMA;
			[VECTOR_DMA_HANDLER]= gr7;					// set new handler for 2D DMA
		
		//-------------------------------- 2D Fast (Packet) DMA mode --------------
		<MatrixDMA>
		[DMATR_MainCounter] = gr7;
		[DMARC_MainCounter] = gr7 with gr2>>=1;			// row counter = width/2 (in 64-bit words)
		[DMATR_RowCounter]  = gr2 with gr7=gr2-1;		// row counter = width/2 (in 64-bit words)
		[DMARC_RowCounter]  = gr2 with gr7<<=1;			// gr7 = width-2
		[DMARC_AddressMode] = gr1 with gr3=gr3-gr7;		// set 2D mode DMA ; calc src bias
		[DMATR_Bias] 		= gr3 with gr4=gr4-gr7; 	// set 2D mode DMA ; calc dst bias
		[DMARC_Bias] 		= gr4;			
		//-------------------------------------------------------------------------
		<StartDMA>
	.endif;
	.if (aligned==1);
		gr2=[--ar5] with gr7>>=1;						// width ; gr7 = size in 64-bit words
		gr3=[--ar5];									// src stride32
		gr4=[--ar5];									// dst stride32
		[DMATR_AddressMode] = gr1;						// set 2D mode DMA 
		[DMATR_MainCounter] = gr7;
		[DMARC_MainCounter] = gr7 with gr2>>=1;			// size in 32-bit words
		[DMATR_RowCounter]  = gr2 with gr7= gr2-1;		// row counter = width/2
		[DMARC_RowCounter]  = gr2 with gr7<<=1;			// gr7 = width-2
		[DMARC_AddressMode] = gr1 with gr3=gr3-gr7;		// set 2D Mode DMA 
		[DMATR_Bias] 		= gr3 with gr4=gr4-gr7; 	// set 2D mode DMA ; calc dst bias
		[DMARC_Bias] 		= gr4;			
	.endif;
	
end   ALIGNED_ARGS;


macro HAL_DMA2D_INIT(func_name, use_cs,mapped_args,aligned_args)
global func_name:label;
<func_name>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	
	gr0 = [_halCoreID];	
	USE_CS(use_cs);
	//gr7 = [_halSyncro];
	//gr7;
	//if >= call SOS;
	//
	[_halSyncro] = gr0;
	
	// read parameters and map them to mirror if needed
	MAPPED_ARGS(mapped_args);
	
	// init dma 
	[DMATR_Address]		= ar0	with gr0 = false;	// set 64-bit size data for transmitter
	[DMARC_Address]		= ar1	with gr1 = gr0+1;	// set 64-bit size data for reciever
	
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

	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
		gr7 = false;
		nop;
		nop;
		
end HAL_DMA2D_INIT;

global _halDma2D_IsCompleted:	label;
<_halDma2D_IsCompleted>
	// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. 
	// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	gr7 = [halDmaToDoSize64];
	ar5 = [DMARC_Control] with gr7; 
	if > delayed return with gr7=false;
		nul;
		nul;
		nul;
	delayed return ;
		gr7 = ar5;
		gr7 <<=30;
		gr7 >>=31;
		


HAL_DMA2D_INIT(_halDma2D_Start   ,0,0,0);
HAL_DMA2D_INIT(_halDma2D_StartA  ,0,0,1);
HAL_DMA2D_INIT(_halDma2D_StartM  ,0,1,0);
HAL_DMA2D_INIT(_halDma2D_StartMA ,0,1,1);

HAL_DMA2D_INIT(_halDma2D_StartC  ,1,0,0);
HAL_DMA2D_INIT(_halDma2D_StartCA ,1,0,1);
HAL_DMA2D_INIT(_halDma2D_StartCM ,1,1,0);
HAL_DMA2D_INIT(_halDma2D_StartCMA,1,1,1);

		
end ".text_hal";


	