


begin ".text_hal"

global _halMapAddrTo:label;
<_halMapAddrTo>

global _halMapAddrFrom:label;
<_halMapAddrFrom>


global _halMapAddr:label;
<_halMapAddr>
	ar5 = ar7-2;
	gr7 = [--ar5];
	ar5 = gr7 with gr7=gr7>>18;
	if > delayed  return;
		gr7 = [40000000h];
	
	push ar0,gr0 	with gr7>>=24;
	ar0 = ar5 	 	with gr7++;
					with gr0 = gr7<<18;
	ar5 = ar0+gr0; 
	pop ar0,gr0;
	return;
	
	ar5 = ar7-2;
	gr7 = [--ar5];	// addr
	ar5 = gr7 with gr7=gr7>>18;
	if > delayed  return;
		gr7 = [40000000h];
	
	push ar0,gr0 	with gr7>>=24;
	ar0 = ar5 	 	with gr7++;
					with gr0 = gr7<<18;
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