//С аргументом "приоритет" Если он равен 1/0 то время на 512 равно 407h/745h,
// что в пересчёте на int32 равно 2.06/3.72  Вообще то тики эти нестабильны и 
// брался минимум который более стабилен при известном терпении.
// плата  mc5103. dmah 
// если приоритет у DMA то время:
// N64: 40  80  100 200 400 800  1000  2000  4000
// tic: 122 1ee 388 6e8 d5e 1aa5 18e2  69c5  d318
//а если приоритет у цпу то время больше:
// N64: 40  80  100 200 400  800  1000  2000  4000
// tic: 1b1 34f 5ea c00 17d2 1aa5 3505  69c5  ----
global _SetCallBack: label;
global _halDmaStart: label;
global _dmainit: label;
global _dmac0: label;
global _dmac1: label;
begin ".text_hal"
.branch;
<_halDmaStart>//по таблице
   push ar5,gr5 with gr7=false;
   ar5=ar7-5 with gr5=gr7+1;    // gr5=1
   delayed skip universal_job;
   ar5=[ar5]; // ptr на структуру cnt,src,dst
   gr7=[ar5++]with gr5=gr5 R>>1; // gr7=dmacnt, gr5=80000000h;
<_dmainit>//   int dmainit( int dmah, int dst, int src, int dmacnt);
   push ar5,gr5 with gr7=false;
   ar5=ar7-8 with gr5=gr7+1;    // gr5=1
   gr7=[ar5++]with gr5=gr5 R>>1; // gr7=dmacnt, gr5=80000000h;
<universal_job>
//   [10000040h]=gr7;
   push ar0,gr0 with gr7=gr7>>1;  // gr7 в единицах int64
   gr0=[ar5++] with gr7=-gr7;    // gr0=dmasrc
//   [10000041h]=gr0; // src
   push ar2,gr2 with gr0 and gr5;// is src in global memory ?
   if =0 delayed goto localdma;
   gr2=7fff0000h with gr5=gr5 >> 9; //gr2=mask for test on global inner memory, gr5=400000h
//=====================================
<globaldma>
   ar0=[ar5++] with gr0 and gr2;  // ar0=dmadst, is src=inner ?
//   [10000042h]=ar0; //
   if =0  delayed goto G_inner2ext; // src=global inner
   dmac1=gr7; // proc for dma test as a result
      gr2=[ar5]; // dmah priority
<G_ext2inner>  gema=gr0;
               gima=ar0 with gr7=gr2<<23; // gr7=800000  dma priority
    gr2=gr5>>1;  // 20                                                                        0000 dma enable
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
   ar0=[ar5++] with gr0 and gr2; // ar0=dmadst, dst=inner if zero
   gr2=[ar5];  // gr2=1 or 0
   if =0  delayed goto L_inner2ext; // src=local inner
   dmac0=gr7 with gr5=gr5 >>4; // gr5=40000;
<L_ext2inner>
//   [10000043h]=ar0;
   lema=gr0;
   lima=ar0 with gr2=gr2<<20; // 100000h
   gr7=gr5<<1;  // gr7=80000
   gr5=gr5+gr2; // gr5=140000
//   [10000049h]=gr5;
   pcr clear gr7;  // direction: ext 2 inner
   pcr set gr5;  // enable & high dma priority

   goto L_fin;

<L_inner2ext>
//   [10000044h]=ar0;

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
   nul;nul;
//=====================================
<eofinit>
   pop ar2,gr2;
   pop ar0,gr0;
   pop ar5,gr5;
   delayed return;
   ar5=_dmac1;
   nul;nul;
<_dmac0>
   delayed return;
   gr7=dmac0; // read register
   nul;nul;
<_dmac1>
   delayed return;
   gr7=dmac1; // read register
   nul;nul;
<Lint> delayed call 11111111h; // заготовка для intr
   nul;nul;
   ireturn;
   nul;nul;nul;
<_SetCallBack>
   push ar5,gr5;
   push ar0,gr0;
   ar5=ar7-8;
   gr0=[ar5++];
   ar0=[ar5];
   ar5=Lint;
   gr5=[ar5++]; [ar0++]=gr5; [ar0++]=gr0;
   gr5=[ar5++]; gr5=[ar5++]; [ar0++]=gr5;
   gr5=[ar5++]; [ar0++]=gr5;
   gr5=[ar5++]; [ar0++]=gr5;
   gr5=[ar5++]; [ar0++]=gr5;
   gr5=[ar5++]; [ar0++]=gr5;
   gr5=[ar5++]; [ar0++]=gr5;
   imr set 48;  // оба канала DMA,  32+16
   pswr set 64; // разрешение внешних прерываний
   pop ar0,gr0;
   pop ar5,gr5;
   return;
end ".text_hal";

