
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
 GR7											:word;
 AR5											:word;
 GR0											:word;
 global _flag_of_pack_DMA :word;
 global callback_addr     :word;
 global mirror_offset     :word;
 global coreID            :word;
end ".nobits";

extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;


begin ".text_hal"


import from led;
import from printx;
import from critical;
import from sleep;

DECLARE_CS_CONST();
// 8.3.2 (стр 152):	Запрос на прерывание по нормальному завершению работы контроллера ПДП  выставляется, 
// когда выполняется следующее условие: 
// - установлен бит Cpl регистра DMARC_Control и 
// - сброшен бит маски MIC в регистре DMARC_InterruptMask  (в т.ч. в конце процесса передачи).

const DMATR_MainCounter		= 1001_0000h;	// [16]
const DMATR_Address			= 1001_0002h;	// [32]
const DMATR_Bias            = 1001_0004h;	// [32]
const DMATR_RowCounter      = 1001_0006h;	// [16]
const DMATR_AddressMode     = 1001_0008h;	// [1]
const DMATR_Control			= 1001_000Ah; 	// [4] Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
							 				// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. 
											// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
const DMATR_InterruptMask	= 1001_000Ch;	// [2] Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
							 				// 0 – запрос разрешён;
							 				// 1 – запрос замаскирован
											// После RESET = 0
const DMATR_State			= 1001_000Eh;	// [10]	  

const DMARC_MainCounter	    = 1001_0010h;
const DMARC_Address		    = 1001_0012h;
const DMARC_Bias            = 1001_0014h;
const DMARC_RowCounter      = 1001_0016h;
const DMARC_AddressMode     = 1001_0018h;
const DMARC_Control		    = 1001_001Ah;

const DMARC_InterruptMask   = 1001_001Ch;
const DMARC_State			= 1001_001Eh;

// Таблица 7.22   Прерывания процессорной системы /стр. 140
// 32	0(поз бита)	прерывание по завершению работы канала ПДП память-память (от MDMAC)
// 33	1(поз бита)	прерывание по ошибке доступа в память канала ПДП память-память (от MDMAC)

// Таблица 7.23 / стр. 143
const IRESERVED = 4000_0406h;
const IRRH		= 4000_0440h;	//	Регистр запросов на прерывание с номерами от 32 до 39. Значение 1 в бите данного регистра показывает наличие запроса, для которого ещё не выполнена команда перехода по адресу-вектору прерывания. Программный сброс битов данного регистра производится только с помощью регистра IRRH_CLR.
const IRRH_SET 	= 4000_0442h;	//	ЧТ/ЗП	Побитовая установка регистра IRRH.
const IRRH_CLR 	= 4000_0444h;	//	ЧТ/ЗП	Побитовый сброс регистра IRRH. Программный сброс бита регистра IRRH следует делать, когда данный запрос замаскирован.


const IASH	    = 4000_0458h; 	// ЧТ Регистр подтверждения и статуса запросов с номерами от 32 до 39. Значение 0 - запрос обработан, 1 - запрос обрабатывается (выполнен или выполняется переход по адресу-вектору, но бит регистра IASH ещё не сброшен
const IASH_CLR  = 4000_045Ch;	// ЧТ/ЗП	Побитовый сброс регистра IASH.
const IMRH      = 4000_0448h;	// ЧТ/ЗП	Регистр маски прерываний с номерами о 0 до 31.
								// Значение 0 - прерывание запрещено, 1 - прерывание разрешено.
								//          после RESET 0
const IMRH_SET  = 4000_044Ah;	// ЧТ/ЗП	Побитовая установка регистра IMRH.
const IMRH_CLR  = 4000_044Ch;	// ЧТ/ЗП	Побитовый сброс регистра IMRH.


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
		[AR5] = ar5;		
		[GR0] = gr0;
	delayed goto CALL_BACK;
		[GR7] = gr7;
////////////////////////////////////////////////
<CALL_BACK>
	//the code below was written according the prescription of how to clear IAS register on the right way;
	//for more information see "Микросхема интегральная  NM6407 Руководство по эксплуатации" page 142;
	// 0. 
	gr0 = false;
	gr0 = [coreID]; 
	gr7 = [_halSyncro]; 	// read  stateDMA from	struct SyncBuf , where
	gr7 - gr0;
	if <>0 call SOS;
	//PRINT_32X("core:",gr0);
	
	gr7 = 4;
	gr7+= gr0;
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
	[_halSyncro] = gr7;		// write 0xffffffff to flag of DMA means DMA is free
	ar5 = [callback_addr];  //set callback address
	call ar5;
	
	//call _halExitCriticalSection;
	gr0	= [coreID];
	HAL_EXIT_CRITICAL_SECTION(gr0);
//	HAL_SLEEP(1);
//	HAL_ZERO_LOOPS(10000);


	ar5 = [AR5];
	gr0 = [GR0];
 	delayed	ireturn;
		gr7 = [GR7];
		nop;









///////////////////////////////////////////////////
global _halDmaSetCallback:label;
<_halDmaSetCallback>
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

///////////////////////////////////////////////////
<_halSetDummyCallbackDMA>
	gr7 = dummy;
	delayed return;
		nul;
		[callback_addr] = gr7;

///////////////////////////////////////////////////
global _halDmaGetCallback:label;
<_halDmaGetCallback>
<_halGetCallbackDMA>
	delayed return;
		nul;
		ar5=[callback_addr];
	
	
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
	////this function write the programm at Lint_6407 label 
	////into interruption vector of interruption controller
	gr7 = [40000000h];
	push ar0,gr0 with gr7 >>= 24;//detecting core the programm uses
	[coreID] = gr7;

	//gr7 = [40000000h];
	//with gr7 >>= 24;//detecting core the programm uses
	//gr7 = [--ar5];
	//HAL_LED_BLINK(gr7,10,1000);
	
	with gr7;
	//HAL_LED_SOS(gr7);
	if <>0 delayed goto SKIP_INIT;
		gr7 = 80000h;
	gr7 >>= 1;		
<SKIP_INIT>
	[mirror_offset] = gr7;	
	push ar1,gr1 with gr7 = false;
	[_flag_of_pack_DMA] = gr7;
	ar5 = Lint_6407;
	ar1 = 00000120h;
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
	gr1 = dummy;
	[callback_addr] = gr1;
//turn on mask in interruption controller
	gr7 = 1;

	gr0 = false;
	//[DMATR_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	//[DMARC_Control]=gr0; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	
	
	gr0 = 1;				// 0th-bit is MDMA channel 
	[IMRH_CLR] = gr0;		// mask(disable) interrupt requests from MDMA
	
	//enable extern interruptions at core
	intr clear 40h;
	pswr set 040h;	
	

	gr7=[coreID];
	
	pop ar1,gr1;
	pop ar0,gr0;
	return;
////////////////////////////////////////////////////
//<_readCallback>
//	delayed return;
//		ar5 = [callback_addr];
//		nop;
////////////////////////////////////////////////////

<_halStatusDMA>
	push ar0,gr0;
	
	gr0 = [coreID]; 
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
	push ar0,gr0;
	
	//gr0 = [coreID]; 
	//ar0 = 4;
	//ar0+= gr0;
	//HAL_LED_ON(ar0);

	//gr7=  -gr0; 
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
	
	//<exit>
	//gr7 = gr0;

	//HAL_LED_OFF(ar0);


	pop ar0,gr0;
	return with gr7=gr7>>1;


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

<_halOpenStatusDMA>
	gr7 = [40000000h];
	gr7 >>= 24;
	if =0 delayed goto SKIP_;
		gr7 = 40000h;
	gr7 = 80000h;	
	<SKIP_>
	[mirror_offset] = gr7;
	gr7 = false;
	delayed return;
		[_flag_of_pack_DMA] = gr7;
		nop;

end ".text_hal";	

	

