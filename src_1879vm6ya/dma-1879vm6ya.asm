global _dmaInit : label;
global _dmaStatus : label;

begin "text"
<_dmaInit>  // int dmaInit(src,dst,size32)
	
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	gr7 = 040000h;
	gr1 = [--ar5];	//src address
	// 0x40000 mirror address trasparation
	gr0 = gr1-gr7;
	gr0 A>>=31;	
	gr0 = gr0 and gr7;
	gr1+= gr0;
	ar0 = gr1;
	
	gr0 = [--ar5];	//dst address
	// 0x40000 mirror address trasparation
	gr1 = gr0-gr7;
	gr1 A>>=31;
	gr1 = gr1 and gr7;
	gr0+= gr1;

	gr7 = [--ar5];	// size in 32-bit words;
	gr7>>=1;	  	// size in 64-bit words;
	
	//counters
	// DMATR=DMA TRansmit
	// DMARC=DMA ReCive
	[10010000h] = gr7;	// DMATR_MainCounter (transmit) количество 64-разрядных слов предать/принять
	[10010010h] = gr7;	// DMARC_MainCounter wrt ammunt of data to mdma
	//address
	[10010002h] = ar0;	// DMATR_Address wrt address to read data into mdma buffer
	[10010012h] = gr0;	// DMARC_Address wrt address to wrt data into dst
	//address modes
	gr7 = false;		// одномерный или двумерный тип адресации. 0-одномерная, единица - двухмерная.
	[10010008h] = gr7; 	// DMATR_AddressMode	0x1001_0008h	1	ЧТ/ЗП
	[10010018h] = gr7;	// DMARC_AddressMode	0x1001_0018h	1	ЧТ/ЗП
	//interruptions are off
	gr7 = true;			// 0 – запрос разрешён; 1 – запрос замаскирован.
	[1001000Ch] = gr7;	// DMATR_InterruptMask	0x1001_000Ch	2	ЧТ/ЗП
	[1001001Ch] = gr7;	// DMARC_InterruptMask	0x1001_001Ch	2	ЧТ/ЗП
	///init 
	gr7 = false;		// Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
	[1001000Ah] = gr7;	// DMATR_Control	0x1001_000Ah	4	ЧТ/ЗП
	[1001001Ah] = gr7;	// DMARC_Control	0x1001_001Ah	4	ЧТ/ЗП
	////start
	gr7 = 1;			// Бит En (0-й разряд).  Запись в бит En = 1 запускает передачу/приём.
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;

	pop ar1,gr1;
	pop ar0,gr0;
	return with gr7 = false;
	
	

<_dmaStatus>	
	gr7 = [1001001Ah]; // Бит Cpl (1-й разряд). При чтении Cpl = 1 означает, что текущие передача или приём завершены. Чтобы произвести следующий запуск, необходимо записать Cpl = 0.
delayed return;	
	gr7 = not gr7;
	gr7 = gr7<<30;
	gr7 = gr7>>31;

end "text";	