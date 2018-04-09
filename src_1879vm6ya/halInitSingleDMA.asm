global _halInitSingleDMA 	: label;

extern _flag_of_pack_DMA 	: word;
extern mirror_offset      : word;
extern coreID             : word;
extern _halSyncro					: word;

begin ".text"
<_halInitSingleDMA>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 - 2;
	push ar0,gr0;
<WAITE_DMA>	
	gr7 = [_halSyncro];
	gr7;
	if < delayed goto SKIP_CORE_INDEFICATION;
		gr0 = [coreID];
	gr7 = gr7 - gr0;
	if <>0 goto WAITE_DMA;

<SKIP_CORE_INDEFICATION>
	[_halSyncro] = gr0;	
	push ar1,gr1 with gr7 = false;
	push ar2,gr2;
	[_flag_of_pack_DMA] = gr7;
	///init 
	[1001000Ah] = gr7;//clear the control register MDAM
	[1001001Ah] = gr7;//clear the control register MDAM 
	gr7 = [--ar5];//src
	gr2 = [mirror_offset];
	ar2 = gr7 with gr7 >>= 18;
	if <>0 delayed goto SKIP_SRC_MIRROR;	
		gr7 = [--ar5];//dst
		gr1 = [--ar5];//size
		ar2 = ar2 + gr2;
	<SKIP_SRC_MIRROR>
	ar0 = ar2;//ar0 is src
	ar2 = gr7 with gr7 >>= 18;	
	if <>0 delayed goto SKIP_DST_MIRROR with gr1 = gr1 >> 1;
		[10010000h] = gr1;//wrt ammnt of data to transiver mdma
	ar2 = ar2 + gr2;//ar2 is dst
	<SKIP_DST_MIRROR>	
	[10010010h] = gr1;//wrt ammnt of data to resiver mdma
	gr0 = ar2;
	gr0 = gr0 >> 29;//check is the address to wrt data DDR or not
	if =0 delayed goto PASS_DOUBLE_DEMENTION_SETUP;
	//check arguments for are they aligned or not
		gr0 = ar2;
		gr7 = ar0;
	gr7 = gr7 or gr0;
	gr7 = gr7 << 28;
	if =0 delayed goto PASS_DOUBLE_DEMENTION_SETUP;
	////// double demention mode was set up to prevent user from hardwaer bug in MDMA,
	////// becoze of erro in FSM inside of MDMA addresses have to be both align as 4 the last significant bits = 0 
	////// otherwise mode is double demention with bias = 2 rowcounter 1 to emulate sigle demention mode use double demention mode
	////// this trick allows to escape reminder/quotient mode swith inside FSM
	//address mode is double demention
	ar1 = 1 with gr7 = true;
	[10010008h] = gr7;//double dimention mode
	[10010018h] = gr7;//double dimention mode
	//row counter
	[10010006h] = ar1;//wrt row counter to mdma
	[10010016h] = ar1;//wrt row counter to mdma
	ar1++;
	[10010004h] = ar1;//bias src
	delayed goto START with gr7 = false;
	[10010014h] = ar1;//bias dst

<PASS_DOUBLE_DEMENTION_SETUP>
	//address mode is sequential
	gr7 = false;
	[10010008h] = gr7;
	[10010018h] = gr7;
	//counters
	[10010000h] = gr1;//wrt ammunt of data to mdma
	[10010010h] = gr1;//wrt ammunt of data to mdma
	//address
<START>
	[10010002h] = ar0;//wrt address to read data into mdma buffer
	[10010012h] = ar2;//wrt address to wrt data into dst
	///interruption mask
	[1001000Ch] = gr7;
	[1001001Ch] = gr7;
	
	////start
	gr7 = 1;
	[1001001Ah] = gr7;
	[1001000Ah] = gr7;

<END>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
	gr7 = false;
end ".text";