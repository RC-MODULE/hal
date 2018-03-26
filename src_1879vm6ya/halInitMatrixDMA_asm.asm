global _halInitMatrixDMA_asm : label;

begin ".text"
<_halInitMatrixDMA_asm>
	//int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  int* dst, int dstStride32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	gr3 = 0fffffffeh;
	gr7 = [--ar5];//load src
	gr7 = gr7 and gr3;
	ar0 = gr7;

	gr0 = [--ar5];//load width
	ar1 = [--ar5] with gr0 = gr0 and gr3;//load height
	
	gr1 = [--ar5];//load srcstride
	gr1 = gr1 and gr3;
	
	gr1 = gr1 - gr0;
	if < delayed goto END;
		ar2 = [--ar5];//load dst
	gr7 = ar2;
	gr7 = gr7 and gr3;
	ar2 = gr7;
	gr2 = [--ar5];//load dststride
	gr2 = gr2 and gr3;
	gr2 = gr2 - gr0;
	if < delayed goto END;
///////////////////////////////////////////////////////////	//init dma
	gr7 = ar1;
  with gr3 = gr0 *: gr7;
.repeat 15;
  with gr3 = gr0 * gr7;
.endrepeat;
	////full counters
	gr7 = gr7 >> 1;
	[10010000h] = gr7;//wrt ammunt of data to mdma
	[10010010h] = gr7;//wrt ammunt of data to mdma
	
	//row counter
	gr7 = gr0 >> 1;
	[10010006h] = gr7;//wrt row counter to mdma
	[10010016h] = gr7;//wrt row counter to mdma
	//address
	[10010002h] = ar0;//wrt address to read data into mdma buffer
	[10010012h] = ar2;//wrt address to wrt data into dst
	
	//bias
	gr1++;gr1++;
	gr2++;gr2++;
	[10010004h] = gr1;//bias srs
	[10010014h] = gr2;//bias dst
	
	//address modes is with bias (double demention)
	gr7 = true;
	[10010008h] = gr7;
	[10010018h] = gr7;
	
	///interaption mask
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
	pop ar3,gr3;	
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	delayed return;
	gr7 = false;
end ".text";