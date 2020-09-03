


begin ".text_hal"

const ID = 4000_0000h;
// 7.3.2  Регистр идентификации процессорной системы.
//[31:24] INDEX ЧТ Номер процессорной системы в составе СБИС:
//00000000 – NMPU0;
//00000001 – NMPU1.

global _halMapAddrFrom:label;
<_halMapAddrFrom>
	ar5 = ar7-2;
	gr7 = [--ar5];
	ar5 = gr7 with gr7=gr7>>18;			// 1<<18 = 0x40000
	if > delayed  return;
		gr7 = [ID];
	
	push ar0,gr0 	with gr0 = false;
					with gr0++;
					with gr7>>=24;
					with gr7 = gr7 xor gr0;
	ar0 = ar5 	 	with gr7++;			// gr7=2  if NMPU0  , 	gr7=1 if NMPU1
					with gr0 = gr7<<18; // gr0=0x80000		,	gr0=0x40000
	ar5 = ar0+gr0; 
	pop ar0,gr0;
	return;



//global _halMapAddr:label;
//<_halMapAddr>

global _halMapAddrTo:label;
<_halMapAddrTo>
	ar5 = ar7-2;
	gr7 = [--ar5];
	ar5 = gr7 with gr7=gr7>>18;			// 1<<18 = 0x40000
	if > delayed  return;
		gr7 = [ID];
	
	push ar0,gr0 	with gr7>>=24;
	ar0 = ar5 	 	with gr7++;			// gr7=1  if NMPU0  , 	gr7=2 if NMPU1
					with gr0 = gr7<<18; // gr0=0x40000		,	gr0=0x80000
	ar5 = ar0+gr0; 
	pop ar0,gr0;
	return;
	

	


	
	
end ".text_hal";

//void* halMapAddr(const void* addr){
//	if ((unsigned)addr >0x40000)	
//		return addr;
//	if (halGetProcessorNo()==0){
//		return (void*)(0x40000+(unsigned)addr);
//	return (void*)(0x80000+(unsigned)addr);
//}
//