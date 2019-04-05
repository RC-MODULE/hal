// Для использования на белом ответе малинке
// xdmac                                                     
// 16 мая 2014
// N ticks from 1000=>10001000 hex:
// 2,90; 100,20a;  200,38a;  400,6be;  800,cca;  1000,18e2
global _xdmacinit: label;
global _xdmacinitn: label;
global _xdmacn: label;
global _xdmac0: label;
global _xdmacChain: label;
global _xdmacChainN: label;
begin ".text_hal"
.branch;
<_xdmacinit> // dmainit( dst, src, N ); армовые адреса и байтовый счётчик
intr clear 0fc000000h;
   push ar5,gr5;
   ar5=ar7-7 with gr1=false; // gr1=0
   push ar0,gr0 with gr5=gr1+1; // gr5=1
   push ar1,gr1;
   gr7=[ar5++]with gr5=gr5<<2;   // gr7=dmacnt, gr5=4
   gr0=[ar5++] with gr7=gr7-1;   // gr0=dmasrc, gr7=count-1
   ar0=[ar5++]; // ar0=dmadst
//=====================================
ar5=401c000h; // базовый адрес xdmac
ar1=ar5;
[ar5++gr5]=gr1;// ar5=base+4
[ar5++]=gr7;   // count-1  4
[ar5++]=gr0;   // dmasrc   5
[ar5++]=ar0;   // dmadst   6
gr7=30f00h;
[ar5++]=gr7;   // src config 7  
[ar5++]=gr7;   // dst config 8

[ar5++]=gr1;   // XDDCC: chain config 9
gr7=11101001h with gr5=gr5>>1; // gr5=2
[ar5]=gr7;     // XDDES: dma enable setting 10
gr7=11010000h;
 [ar1]=gr7; // команда "начать передачу" [0]
pop ar1,gr1;
pop ar0,gr0;
pop ar5,gr5;
<_xdmac0>
   delayed return;
   gr7=[401c004h]; //gr7=status;
//===============================================
<_xdmacinitn> // dmainit( dst, src, N, channel ); армовые адреса и байтовый счётчик
intr clear 0fc000000h;
   push ar5,gr5;
   ar5=ar7-8 with gr5=false; // gr1=0
   push ar0,gr0 with gr5=gr5+1; // gr5=1
   gr0=[ar5++];  // channel ( 0..7)
   push ar1,gr1 with gr5=gr5+gr0; // gr5=1+channel
   gr7=[ar5++]with gr5=gr5+gr0;  // gr7=dmacnt, gr5=1+2*channel
   gr1=[ar5++]with gr7=gr7-1;      // gr0=dmasrc, gr7=count-1
   ar0=[ar5++]with gr5=gr5+gr0;  // ar0=dmadst, gr5=1+3*channel
//=====================================
ar5=401c000h with gr5=gr5<<2; // базовый адрес xdmac, gr5=4+12*channel
ar1=ar5 with gr0=false; // адрес регистра управления всеми каналами
[ar5++gr5]=gr0; //сброс регистра управления всеми каналами
[ar5++]=gr7;   // count-1 4
[ar5++]=gr1;   // dmasrc  5
[ar5++]=ar0;   // dmadst  6
gr7=30f00h;    //
[ar5++]=gr7;   // src config 7  
[ar5++]=gr7;   // dst config 8
[ar5++]=gr0;   // chain config 9

gr7=11101001h;
[ar5]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда "начать передачу"
pop ar1,gr1;
pop ar0,gr0;
pop ar5,gr5;
return;
<_xdmacChain> // i=dmacChain( *int )
intr clear 0fc000000h;
   push ar2,gr2;
   ar2=ar7-5 with gr2=false; // gr2=0
   push ar0,gr0 with gr2++; // gr2=1
   push ar1,gr1;
   push ar5,gr5;
   ar5=[ar2];  // Содержимое таблицы
   ar2=401c000h;    // базовый адрес xdmac канала 0
   ar1=ar2;
   gr7=[ar5++]with gr2=gr2<<2;   // gr7=dmacnt, gr2=4
   gr0=[ar5++] with gr7=gr7-1;   // gr0=dmasrc, gr7=count-1
   ar0=[ar5++] with gr1=false;
//=====================================
[ar2++gr2]=gr1;   // ar2=base+4
[ar2++]=gr7;        // count-1
[ar2++]=gr0;        // ARM dmasrc
[ar2++]=ar0;        // ARM dmadst
gr7=[ar5++];        // config src
[ar2++]=gr7;
gr7=[ar5++];        // config dst
[ar2++]=gr7;
gr7=[ar5++];        // config chain
[ar2++]=gr7;        // XDDCC:  chain config 9
gr7=11101001h with gr2=gr2>>1; // gr2=2
[ar2]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда начала передачи
pop ar5,gr5;
pop ar1,gr1;
pop ar0,gr0;
pop ar2,gr2;
  delayed return;
  gr7=[401c004h];  //gr7=status;
// xdmacChain
<_xdmacChainN> // i=dmacChain( *int, channel )
intr clear 0fc000000h;
   push ar2,gr2;
   ar2=ar7-6 with gr2=false; // gr2=0

    gr5=[ar2++]with gr2++; // gr5=channel. gr2=1
   push ar0,gr0 with gr7=gr5<<1;   // gr5=2*channel 
   push ar1,gr1 with gr5=gr5+gr7;  //  gr5=3*channel
   push ar5,gr5 with gr5++;             //  gr5=1+3*channel
   ar5=[ar2]with gr2=gr5<<2;  // ar5=Указатель таблицы,gr5=4+12*channel
   ar2=401c000h;    // базовый адрес xdmac
   ar1=ar2;
   gr7=[ar5++]; 
   gr0=[ar5++] with gr7=gr7-1;   // gr0=dmasrc, gr7=count-1
   ar0=[ar5++] with gr1=false;
//=====================================
[ar2++gr2]=gr1;   // ar2=base+4 
[ar2++]=gr7;        // count-1
[ar2++]=gr0;        // ARM dmasrc
[ar2++]=ar0;        // ARM dmadst
gr7=[ar5++];        // config src
[ar2++]=gr7;
gr7=[ar5++];        // config dst
[ar2++]=gr7;
gr7=[ar5++];        // config chain
[ar2++]=gr7;        // XDDCC:  chain config 9
gr7=11101001h with gr2=gr2>>1; // gr2=2
[ar2++gr2]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда начала передачи
pop ar5,gr5;
pop ar1,gr1;
pop ar0,gr0;
pop ar2,gr2;
  delayed return;
  gr7=[401c004h];  //gr7=status;
// xdmacChainN

<_xdmacn> // xdmac( int channel )
   push ar5,gr5;
   gr5=[ar7-5]; // channel
   ar5=401c004h with gr7=gr5+gr5;  // gr7=2*ch
   gr7=gr5+gr7;    // gr7=3*ch
   gr5=gr7<<2;     // gr5=4+12*ch
   gr7=[ar5+gr5];  //gr7=status xdmacn;
   pop ar5,gr5;
   return;
end ".text_hal";
