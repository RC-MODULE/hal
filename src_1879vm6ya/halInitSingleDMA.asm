global _halInitSingleDMA : label;
extern _flag_of_pack_DMA : word;
begin ".text"

<_halInitSingleDMA>
	//int halInitSingleDMA(int  src,  int  dst,  int  size32);
	ar5 = ar7 -2;
	push ar0,gr0;
	push ar1,gr1 with gr7 = false;
	[_flag_of_pack_DMA] = gr7; 
	gr7 = [--ar5];//src
	ar0 = gr7;	
	gr0 = [--ar5];//dst
	gr1 = [--ar5];//size
	gr1 = gr1 >> 1;
	//check arguments
	gr7 = gr7 or gr0;
	gr7 = gr7 << 28;

	if =0 delayed goto PASS_DOUBLE_DEMENTION_SETUP;// double demention mode was set up to prevent user from hardwaer bug in MDMA becoze of erro in FSM inside of MDMA addresses have to be both align as 4 the last significant bits = 0 otherwise mode is double demention with bias = 2 rowcounter 1 to emulate sigle demention mode use double demention mode
	//address mode is double demention
	ar1 = 1 with gr7 = true;
	[10010008h] = gr7;//double dimention mode
	[10010018h] = gr7;//double dimention mode
	//set up full conter
	[10010000h] = gr1;//wrt ammunt of data to mdma
	[10010010h] = gr1;//wrt ammunt of data to mdma
	//row counter
	[10010006h] = ar1;//wrt row counter to mdma
	[10010016h] = ar1;//wrt row counter to mdma
	ar1++;
	[10010004h] = ar1;//bias srs
	delayed goto START;
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
	[10010012h] = gr0;//wrt address to wrt data into dst
	///interruption mask
	gr7 = true;
	[1001000Ch] = gr7;
	gr7 = 2;
	[1001001Ch] = gr7;
	
	///init 
	gr7 = false;
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;
	
	////start
	gr7 = 1;
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;

<END>
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
	gr7 = false;
end ".text";