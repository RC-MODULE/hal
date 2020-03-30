
data ".data_hal"
	cpUserCallback: word[3] = (0h dup 3);	//3 пользовательских callback-ов
	global cpPortDirection: word[3] = (0h dup 3);
	link_gr7: word[2];
	link_ar5: word[2];
end ".data_hal";

import from periphery;
import from led;


/*macro cpInterrupt(nameHandler)
	nul;nul;
	nul;nul;
	delayed ireturn;
		nul;
		nul;
		nul;
end cpInterrupt;*/
macro cpInterrupt(nameHandler)
	[link_gr7] = gr7;
	[link_ar5] = ar5;
	goto nameHandler;
end cpInterrupt;

macro cpHandler(nameHandler, numberCallback)
<nameHandler>
	gr7 = [cpUserCallback + numberCallback];
	with gr7;
	if <>0	call gr7;
	ar5 = [link_ar5];	
	gr7 = [link_gr7];
 	ireturn;
end cpHandler;

begin ".text_hal"
global _halLinkSetCallback: label;
<_halLinkSetCallback>
	ar5 = ar7 - 2;
	push ar0, gr0;
	gr7 = [--ar5];
	gr0 = [--ar5];
	ar0 = cpUserCallback;
	[ar0 += gr0] = gr7;
	pop ar0, gr0;
	return;
	
<cpInterrupts>
	cpInterrupt(link0Handler);
	cpInterrupt(link1Handler);
	cpInterrupt(link2Handler);
	
	cpHandler(link0Handler, 0);
	cpHandler(link1Handler, 1);
	cpHandler(link2Handler, 2);
<NUL_NUL>
	nul;nul;

//void halLinkInit(int port, int direction);
global _halLinkInit: label;
<_halLinkInit>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	gr0 = [--ar5];		//номер порта 0..2
	gr1 = [--ar5];		//0 если порт передающий, 1, если порт принимающий
	ar0 = cpPortDirection;
	[ar0 += gr0] = gr1	with gr7 = false;
	
	//ar0 = cpUserCallback;
	//[ar0 += gr0] = gr7;		//сброс callback'a
	
	//вычисление номера канала
	// CP0TR - 0
	// CP0RC - 1
	// CP1TR - 2
	// CP1RC - 3
	// CP2TR - 4
	// CP2RC - 5
	gr0 <<= 1;
	gr0 += gr1;	
	
	//сдвиг единицы в нужный разряд для активации прерывания.
	//прерывания коммутационных портов начинаются с номера 24
	// CP0TR - 24 (0)
	// CP0RC - 25 (1)
	// CP1TR - 26 (2)
	// CP1RC - 27 (3)
	// CP2TR - 28 (4)
	// CP2RC - 29 (5)
	EndShift: label;
	gr1 = 1	with gr7 = gr0;
<ShiftBit>
	if =0 delayed goto EndShift;
		nul;
		nul;
	delayed goto ShiftBit;
		gr1 <<= 1;
		gr7--;
<EndShift>
	gr7 = gr1 << 24;	
	
	//вектора прерывания начинаются с адреса 20h и каждый вектор занимает по 4 длинных слова
	ar0 = 20h + 24 * 8h 	with gr0 <<= 3;
	ar0 += gr0;
	ar1 = cpInterrupts		with gr1 = gr0 >> 1;	//делаем одинаковые векторы прерывания на два канала, поэтому gr1 = gr0 / 2
	ar5 = ar1 + gr1;
	ar1,gr1 = [ar5++];
	[ar0++] = ar1, gr1;
	ar1,gr1 = [ar5++];
	[ar0++] = ar1, gr1;
	ar1,gr1 = [ar5++];
	[ar0++] = ar1, gr1;
	ar1,gr1 = [ar5++];
	[ar0++] = ar1, gr1;	
	
	[4000_040Ah] = gr7		with gr7 = false;	//enable link interrupt
	
	
	//enable extern interrupts
	intr clear 40h;
	pswr set 040h;	
	
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_hal";