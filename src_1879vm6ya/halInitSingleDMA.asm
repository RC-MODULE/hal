global _halInitSingleDMA 	: label;

extern mirror_offset      : word;
extern coreID             : word;
extern _halSyncro					: word;

extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;




// 8.3.2 (стр 152):	Запрос на прерывание по нормальному завершению работы контроллера ПДП  выставляется, 
// когда выполняется следующее условие: 
// - установлен бит Cpl регистра DMARC_Control и 
// - сброшен бит маски MIC в регистре DMARC_InterruptMask  (в т.ч. в конце процесса передачи).

const DMATR_MainCounter		= 1001_0000h;
const DMATR_Address			= 1001_0002h;
const DMATR_Bias            = 1001_0004h;
const DMATR_RowCounter      = 1001_0006h;
const DMATR_AddressMode     = 1001_0008h;
const DMATR_Control			= 1001_000Ah; 	// Бит En (0-й разряд). Запись в бит En = 1 запускает передачу/приём.
							 				// Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. 
											// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
const DMATR_InterruptMask	= 1001_000Ch;	// Бит MIC (0-й разряд) – маска запроса на прерывание по нормальному завершению обмена данными: 
							 				// 0 – запрос разрешён;
							 				// 1 – запрос замаскирован
											// После RESET = 0
const DMATR_State			= 1001_000Eh;				  

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

	


begin ".text_hal"

import from led;
import from printx;
import from sleep;
import from mutex;

 
 global SOS:label;
<SOS>
	HAL_LED_SOS(gr0,1);
	return;

<_halInitSingleDMA>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	gr0 = [coreID];
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
	gr0 = [mirror_offset];
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