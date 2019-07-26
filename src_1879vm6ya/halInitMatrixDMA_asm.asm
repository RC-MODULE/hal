global _halInitMatrixDMA_asm : label;

extern _flag_of_pack_DMA 	: word;
extern _halMirrorOffset      : word;
extern _halCoreID             : word;
extern _halSyncro					: word;

extern _halEnterCriticalSection : label;
extern _halExitCriticalSection  : label;

begin ".text_hal"
<_halInitMatrixDMA_asm>
	//int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  int* dst, int dstStride32);
	ar5 = ar7 - 2;
	push ar0,gr0;
	gr0 = [_halCoreID];
<WAIT_DMA>	
call _halEnterCriticalSection;	
	gr7 = [_halSyncro];
	gr7;
	if >= goto WAIT_DMA;
	[_halSyncro] = gr0;
call _halExitCriticalSection;
	///init 
	push ar1,gr1 with gr7 = false;
	[1001000Ah] = gr7;//clear controll register
	[1001001Ah] = gr7;//clear controll register
	[_flag_of_pack_DMA] = gr7;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	gr4 = [_halMirrorOffset];
	gr7 = [--ar5];//load src
	ar4 = gr7 with gr7 >>= 18;
	if <>0 delayed goto SKIP_SRC_MIRROR_SETUP;
		gr0 = [--ar5];//load width
		ar1 = [--ar5];//load height
	ar4 = ar4 + gr4;
	<SKIP_SRC_MIRROR_SETUP>
	ar0 = ar4;
	gr1 = [--ar5];//load srcstride
	gr7 = [--ar5] with gr1 = gr1 - gr0;//load dst //got src bias
	ar4 = gr7 with gr7 >>= 18;
	if <>0 delayed goto SKIP_DST_MIRROR_SETUP;
		gr2 = [--ar5];//load dststride
		gr2 = gr2 - gr0; //got dst bias
	ar4 = ar4 + gr4;
	<SKIP_DST_MIRROR_SETUP>
	//ar2 = ar4;
/////////////////////////////////////////////////////////////init dma
	
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
	[10010012h] = ar4;//wrt address to wrt data into dst
	//bias
	gr1++;gr1++;
	gr2++;gr2++;
	[10010004h] = gr1;//bias srs
	[10010014h] = gr2;//bias dst
	//address modes is with bias (double demention)
	gr7 = true;
	[10010008h] = gr7;//double demention mode
	[10010018h] = gr7;//double demention mode
	///interaption mask
	gr7 = true;
	[1001000Ch] = gr7;//both interruptions are on
	gr7 = 2;
	[1001001Ch] = gr7;//both interruptions are on
	////start
	gr7 = 1;
	[1001000Ah] = gr7;
	[1001001Ah] = gr7;
	gr7 = false;//exit code 0
<END>
	pop ar4,gr4;	
	pop ar3,gr3;	
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
	
	
	
end ".text_hal";