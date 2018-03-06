// Для использования на белом ответе малинке ( 7707 )
// nm_xdmac                                                     
// 16 мая 2014
// N ticks from 1000=>10001000 hex:
// 2,90; 100,20a;  200,38a;  400,6be;  800,cca;  1000,18e2
global _xdmacinit: label;
global _xdmacChain: label;
global _xdmac0: label;
begin ".data"
pre_chain: word[6]=( 1,2,3,4,5,6);
_chain_tbl: word[16]=( 15,100010h,101040h,30f00h,30f00h,0,0,0,
                                      15,100020h,101080h,30f00h,30f00h,0,0,0);
end ".data";
begin ".text"
.branch;
//                             cnt-1     src          dst
<_xdmacinit> // dmainit( dst, src, N ); армовые адреса и байтовый счётчик
intr clear 0fc000000h;
   push ar5,gr5;
   ar5=ar7-7 with gr5=false; // gr5=0
   push ar0,gr0 with gr5++; // gr5=1
   push ar1,gr1;
   gr7=[ar5++]with gr5=gr5<<2;   // gr7=dmacnt, gr5=4
   gr0=[ar5++] with gr7=gr7-1;   // gr0=dmasrc, gr7=count-1
   ar0=[ar5++] with gr1=false;
//=====================================
ar5=401c000h;    // базовый адрес xdmac канала 0
ar1=ar5;
[ar5++gr5]=gr1;   // ar5=base+4
[ar5++]=gr7;        // count-1
[ar5++]=gr0;        // ARM dmasrc
[ar5++]=ar0;        // ARM dmadst
gr7=30f00h;
[ar5++]=gr7;
[ar5++]=gr7;
gr1=_chain_tbl;// NM адрес chain таблицы
[40000h]=gr1; // на место dmasrc[0]
gr1=gr1<<2;      // ARM адрес chain таблицы
gr1=gr1>>4;      // clear 4 bits of adr
gr1=gr1<<4;     
gr1++;
[ar5++]=gr1;   // XDDCC:  chain config 9
gr0=32; // плюс 8 к NM адресу _chain_tbl
gr0=gr0+gr1;
[_chain_tbl+5]=gr0;
gr7=11101001h with gr5=gr5>>1; // gr5=2
[ar5++gr5]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда начала передачи
pop ar1,gr1;
pop ar0,gr0;
pop ar5,gr5;
  delayed return;
  gr7=[401c004h];  //gr7=status;
// xdmacinit
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
[ar2++gr2]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда начала передачи
pop ar5,gr5;
pop ar1,gr1;
pop ar0,gr0;
pop ar2,gr2;
  delayed return;
  gr7=[401c004h];  //gr7=status;
// xdmacChain
<_xdmac0>
   delayed return;
   gr7=[401c004h]; //gr7=status;
end ".text";
