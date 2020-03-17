

extern cpPortDirection:word[3];

//void halLinkStart(void* vector, int size32, int port);
begin ".text_link"
global _halLinkStart: label;
<_halLinkStart>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	ar0 = [--ar5];
	gr0 = [--ar5];
	gr1 = [--ar5]	with gr0 >>= 1;
	ar1 = cpPortDirection;
	gr7 = [ar1+=gr1]	with gr1 <<= 10;	//чтение направления порта, gr1 *= 0x400h
	gr7 <<= 4;			//gr7 = 10h, если порт принимающий
	ar1 = 40001800h	with gr1 += gr7;			//ar1 - базовый адрес коммуникационных портов, gr1 - смещение до нужного канала
	ar1 += gr1	with gr7 = false;
	
	[ar1 + 0Ah] = gr7;		//reset Control
	[ar1] = gr0;			//MainCounter
	[ar1 + 02h] = ar0;		//Addr
	[ar1 + 08h] = gr7;		//AddrMode
	gr7 = 2;
	[ar1 + 0Ch] = gr7	with gr7--;		//IntMask
	[ar1 + 0Ah] = gr7;			//start	
	pop ar1, gr1;
	pop ar0, gr0;
	return;

//void halLinkStart2D(void* vector, int size32, int width, int stride, int port);	
global _halLinkStart2D: label;
<_halLinkStart2D>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	ar0 = [--ar5];							//addr
	gr0 = [--ar5];							//size32
	ar2 = [--ar5]	with gr0 >>= 1;			//width
	gr2 = [--ar5];							//stride
	gr1 = [--ar5];//	with gr2 >>= 1;			//port
	
	ar1 = cpPortDirection;
	gr7 = [ar1+=gr1]	with gr1 <<= 10;	//чтение направления порта, gr1 *= 0x400h
	gr7 <<= 4;			//gr7 = 10h, если порт принимающий
	ar1 = 40001800h	with gr1 += gr7;			//ar1 - базовый адрес коммуникационных портов, gr1 - смещение до нужного канала
	ar1 += gr1	with gr7 = false;
	
	[ar1 + 0Ah] = gr7;						//reset Control
	[ar1] = gr0			with gr7 = false;	//MainCounter
	[ar1 + 02h] = ar0	with gr7++;			//Addr
	gr0 = ar2			with gr7++;			//width
	gr0 >>= 1;
	[ar1 + 06h] = gr0	with gr0 = gr0 - gr7;			//RowCounter = width, width-2
	gr2 = gr2 - gr0;	
	[ar1 + 04h] = gr2;			//bias
	
	[ar1 + 0Ch] = gr7	with gr7--;		//IntMask
	[ar1 + 08h] = gr7;			//AddrMode
	[ar1 + 0Ah] = gr7;			//start	
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;

//int halLinkIsCompleted(int port);
global _halLinkIsCompleted: label;
<_halLinkIsCompleted>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	gr0 = [--ar5];
	ar0 = cpPortDirection	with gr1 = gr0 << 10;
	gr7 = [ar0 += gr0]	with gr0 = false;
	gr7 <<= 4;
	ar1 = 4000180Ah	with gr1 += gr7; 	//	ar0 - базовый адрес регистра Control коммуникационных портов	
	ar1 += gr1	with gr0++;
	gr7 = [ar1]	with gr0++;		//gr0 = 2;
	pop ar1, gr1	with gr7 = gr7 and gr0;
	pop ar0, gr0	with gr7 >>= 1;
	return;
end ".text_link";