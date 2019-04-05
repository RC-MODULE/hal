//                                                      
//"30-апр-14, 13:28:13"; плата  mc5103
// если приоритет у DMA то время:
// N64: 40  80  100 200 400 800  1000  2000  4000
// tic: 122 1ee 388 6e8 d5e 1aa5 18e2  69c5  d318
//а если приоритет у цпу то время больше:
// N64: 40  80  100 200 400  800  1000  2000  4000
// tic: 1b1 34f 5ea c00 17d2 1aa5 3505  69c5  ----
global _SetCallBack: label;
global _dmainit: label;
global _dmaInit: label;
global _dmac0: label;
global _dmac1: label;
begin ".text_hal"
.branch;
<_dmaInit>
<_dmainit>
   push ar5,gr5;
   push ar0,gr0 with gr5=false; // gr5=0
   ar5=ar7-10 with gr5=gr5+1; // gr5=1
   gr7=[ar5++]with gr5=gr5 R>>1; // gr7=dmacnt, gr5=80000000h;

   gr0=[ar5++] with gr7=-gr7; // gr0=dmasrc
   push ar2,gr2 with gr0 and gr5; // dst in global memory ?
   if =0 delayed goto localdma;
   gr2=7fff8000h with gr5=gr5 >> 9; //gr2=mask for test on global inner memory, gr5=400000h
//=====================================
<globaldma> dmac1=gr7;
   ar0=[ar5++] with gr0 and gr2;  // ar0=dmadst, is src=inner ?
   if =0  delayed goto G_inner2ext; // src=global inner
   dmac1=gr7; // proc for dma test as a result
   gr2=[ar5];  // gr2=1 or 0
<G_ext2inner>  gema=gr0;
               gima=ar0 with gr7=gr2<<23; // gr7=800000  dma priority
    gr2=gr5>>1;  // 200000 dma enable
    gr7=gr7+gr2; // a00000
    pcr clear gr5;
    pcr set gr7;
    goto eofinit;
<G_inner2ext>
    gima=gr0;
    gema=ar0;
    gr7=gr5<<1;  // 800000
    gr2=gr5>>1;  // 200000
    gr7=gr7+gr2; // a00000
    gr7=gr7+gr5; // e00000
    pcr set gr7;
    goto eofinit;
//-------------------------
<localdma>
//   [ar6++]=gr7;//!!  gr7=-Cnt
   ar0=[ar5++] with gr0 and gr2; // ar0=dmadst, dst=inner if zero
   if =0  delayed goto L_inner2ext; // src=local inner
   dmac0=gr7 with gr5=gr5 >>4; // gr5=40000;
   gr2=[ar5];  // gr2=1 or 0
<L_ext2inner>
   lema=gr0;
   lima=ar0 with gr2=gr2<<20;
   gr7=gr5<<1;  // gr7=80000
   gr5=gr5+gr2; // gr5=140000
   pcr clear gr7;  // direction: ext 2 inner
   pcr set gr5;  // enable & high dma priority

   goto L_fin;

<L_inner2ext>
   lima=gr0;    // inner
   lema=ar0 with gr2=gr2<<20;    // ext, gr2=100000
   gr7=gr5<<1;  // gr7=80000
   gr5=gr5+gr7; // gr5=c0000
   gr5=gr5+gr2; // gr5=1c0000
   pcr set gr5; // local dma0 start
//  40000:dma go, 80000:int2ext, 100000: dma0 priority > cpu
//   [ar6++]=gr0;[ar6++]=ar0;[ar6++]=gr5;gr0=11111h;[ar6++]=gr0;
<L_fin>
	pop ar2,gr2;
	pop ar0,gr0;
	pop ar5,gr5;
	delayed return;
		ar5=_dmac0;
		nul;
		nul;

//=====================================
<eofinit>
	pop ar2,gr2;
	pop ar0,gr0;
	pop ar5,gr5;
	delayed return;
		ar5=_dmac1;
		nul;
		nul;
<_dmac0>
	delayed return;
		gr7=dmac0;
		nul;
		nul;
<_dmac1>
	delayed return;
		gr7=dmac1;
		nul;
		nul;
<Lint> 
	delayed call 11111111h;
		nul;
		nul;
	delayed ireturn;
		nul;
		nul;
		nul;
<_SetCallBack>
	push ar5,gr5;
	push ar0,gr0;
	ar5=ar7-8;
	gr0=[ar5++];
	ar0=[ar5];
	ar5=Lint;
	gr5=[ar5++]; 
	[ar0++]=gr5; 
	[ar0++]=gr0;
	gr5=[ar5++]; gr5=[ar5++]; [ar0++]=gr5;
	gr5=[ar5++]; [ar0++]=gr5;
	gr5=[ar5++]; [ar0++]=gr5;
	gr5=[ar5++]; [ar0++]=gr5;
	gr5=[ar5++]; [ar0++]=gr5;
	gr5=[ar5++]; [ar0++]=gr5;
	
	imr set 48;   // разрешить оба канала DMA 
	pswr set 40h; // Маска внешнего по отношению к ядру прерывания
	pop ar0,gr0;
	pop ar5,gr5;
return;

end ".text_hal";

