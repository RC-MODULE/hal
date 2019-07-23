

begin ".text_hal"

global _halMapAddr:label;
<_halMapAddr>
	ar5 = ar7-2;
	//push ar0,gr0;
	gr0 = [--ar5];
	ar5 = gr7 with gr7=gr7>>18;
	
	//if >0 delayed goto 
	//pop ar0,gr0;
	
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