

import from led;

//void halLinkStart(void* vector, int size32, int port, int direction);
begin ".text_hal"
global _halLinkStart: label;
<_halLinkStart>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	ar0 = [--ar5];
	gr0 = [--ar5];
	gr1 = [--ar5]	with gr0 >>= 1;
	gr7 = [--ar5] 	with gr1 <<= 10;	//чтение направления порта, gr1 *= 0x400h
	gr7 <<= 4;							//gr7 = 10h, если порт принимающий
	ar1 = 40001800h	with gr1 += gr7;			//ar1 - базовый адрес коммуникационных портов, gr1 - смещение до нужного канала
	ar1 += gr1	with gr7 = false;
	
	[ar1 + 0Ah] = gr7;		//reset Control
	[ar1 + 08h] = gr7	with gr7++;		//AddrMode
	[ar1] = gr0			with gr7++;		//MainCounter, gr7 = 2
	[ar1 + 02h] = ar0;		//Addr
	gr7 = 3;
	[ar1 + 0Ch] = gr7	with gr7--;		//IntMask, gr7 = 1
	gr7 = 1;
	[ar1 + 0Ah] = gr7;			//start	
	
	gr7 = [ar1 + 0Ah];
	pop ar1, gr1;
	pop ar0, gr0;
	return;

//void halLinkStart2D(void* vector, int size32, int width, int stride, int portm int direction);	
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
	gr1 = [--ar5];							//port
	gr7 = [--ar5]		with gr1 <<= 10;	//чтение направления порта, gr1 *= 0x400h
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

//int halLinkIsCompleted(int port, int direction);
global _halLinkIsCompleted: label;
<_halLinkIsCompleted>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	gr0 = [--ar5];
	gr7 = [--ar5]	with gr0 <<= 10;
	gr7 <<= 4;
	ar0 = 40001800h + 0Ah	with gr0 += gr7; 	//	ar0 - базовый адрес регистра Control коммуникационных портов	
	ar0 += gr0	with gr1 = false;
	gr7 = [ar0]	with gr1++;
	//gr1 = 3;
	gr7>>=1;
	gr7 = gr7 and gr1;
	pop ar1, gr1;
	pop ar0, gr0;
	return;



//int halLinkGetStatus(int port, int direction);
global _halLinkGetState: label;
<_halLinkGetState>
	ar5 = sp - 2;
	push ar0, gr0;
	gr0 = [--ar5];
	gr7 = [--ar5]	with gr0 <<= 10;
	gr7 <<= 4;
	ar0 = 40001800h + 0Eh	with gr0 += gr7;
	gr7 = [ar0 += gr0];	
	pop ar0, gr0	with gr7 >>= 24;
	return;

//int halLinkGetActiveDataCounter(int port, int direction);
global _halLinkGetActiveDataCounter: label;
<_halLinkGetActiveDataCounter>
	ar5 = sp - 2;
	push ar0, gr0;
	gr0 = [--ar5];
	gr7 = [--ar5]	with gr0 <<= 10;
	gr7 <<= 4;
	ar0 = 40001800h + 0Eh	with gr0 += gr7;
	gr7 = [ar0 += gr0];
	gr0 = 1fh;
	pop ar0, gr0	with gr7 = gr7 and gr0;
	return;
end ".text_hal";