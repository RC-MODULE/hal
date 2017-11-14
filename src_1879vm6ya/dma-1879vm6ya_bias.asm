global _dma_bias_init : label;

begin "text"
<_dma_bias_init>
	///void mdma(N,srs,dst,row2read,bias_read,row2wrt,bias_wrt);
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	gr7 = [--ar5];//N
	ar0 = [--ar5];//srs address
	gr0 = [--ar5];//dst address
	gr2 = [--ar5];//row 2 read
	gr1 = [--ar5];//bias of transvered array
	ar2 = [--ar5];//row 2 wrt
	ar1 = [--ar5];//bias of loading location
	//counters
	[10010000h] = gr7;//wrt ammunt of data to mdma
	[10010010h] = gr7;//wrt ammunt of data to mdma
	
	//address
	[10010002h] = ar0;//wrt address to read data into mdma buffer
	[10010012h] = gr0;//wrt address to wrt data into dst
	
	//bias
	[10010014h] = ar1;//bias to wrt mem
	[10010004h] = gr1;//bias to read mem
	
	////row counter
	[10010006h] = gr2;
	[10010016h] = ar2;

	


	//address modes double deminsion
	gr7 = true;
	[10010008h] = gr7;
	gr7 = false;
	[10010018h] = gr7;
	
	//interaptions are off
	gr7 = true;
	[1001000Ch] = gr7;
	[1001001Ch] = gr7;


	///init to clear dma register
	gr7 = false;
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;

	////start
	gr7 = 1;
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;

	pop ar0,gr0;
	pop ar1,gr1;
	pop ar2,gr2;
	return;
end "text";	