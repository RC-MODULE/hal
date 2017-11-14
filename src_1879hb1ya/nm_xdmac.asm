// Для использования на белом ответе малинке
// xdmac                                                     
// 16 мая 2014
// N ticks from 1000=>10001000 hex:
// 2,90; 100,20a;  200,38a;  400,6be;  800,cca;  1000,18e2
global _xdmacinit: label;
global _xdmac0: label;
begin ".text"
.branch;
<_xdmacinit> // dmainit( dst, src, N ); армовые адреса и байтовый счётчик
intr clear 0fc000000h;
   push ar5,gr5;
   ar5=ar7-7;// with gr1=false; // gr1=0
   push ar0,gr0;
   gr5=1;
   push ar1,gr1;
   gr1=0;
   gr7=[ar5++]with gr5=gr5<<2;   // gr7=dmacnt, gr5=4
   gr0=[ar5++] with gr7=gr7-1;   // gr0=dmasrc, gr7=count-1
   ar0=[ar5++]; // ar0=dmadst
//=====================================
ar5=401c000h; // базовый адрес xdmac канала 0
ar1=ar5;
[ar5++gr5]=gr1;
[ar5++]=gr7;   // count-1
[ar5++]=gr0;   // dmadst
[ar5++]=ar0;   // dmadst
gr7=30f00h;
[ar5++]=gr7;
[ar5++]=gr7;
[ar5++]=gr1;
gr7=11101001h with gr5=gr5>>1; // gr5=2
[ar5++gr5]=gr7;
gr7=11010000h;
[ar1]=gr7; // команда начала передачи
pop ar1,gr1;
pop ar0,gr0;
pop ar5,gr5;
//  delayed return;
//  gr7=[401c004h];  //gr7=status;
<_xdmac0>
   delayed return;
   gr7=[401c004h]; //gr7=status;
end ".text";
