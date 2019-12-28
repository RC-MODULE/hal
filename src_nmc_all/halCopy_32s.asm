//------------------------------------------------------------------------
//
//  Copyright (c) RC Module Inc.
//
//
//! \if file_doc
//!
//! \file   Copy.asm
//! \author Сергей Мушкаев
//! \brief  Функции инициализации и копирования.
//!
//! \endif
//!
//------------------------------------------------------------------------



extern vec_data:label;


begin ".text_hal"
  
<halCopyEven2Even>

<halCopyEven2Odd>

<halCopyOdd2Odd>

<halCopyOdd2Even>
	

   
global _halCopyRISC_:label;
<_halCopyRISC_>
.branch;
    ar5 = sp - 5	;
	push ar0,gr0	;
	push ar6,gr6	;
	gr7 = [ar5++]	; 			// size
	ar6 = [ar5++]	with gr7--;	// dst
    ar5 = [ar5++]	with gr7--;	// src
	//push ar1,gr1
	//push ar6,gr6	with gr7 = gr0 <<31;
	//gr6 = [--ar5]	with gr7 = gr7 >>30;	// dst
	//ar6 = gr6 		with gr6 = gr6 <<31
	//gr7 = [--ar5]	with gr6 = gr6 >>39;
	//ar5 = ar6 		with gr6 = gr0 + gr6;
	//ar6 = copy      with gr6+= gr7;
	//ar6=[ar6+=gr6]; 
	//goto ar6;
	//
	//ar0=gr0;
	//ar6=gr6;
	//
	//
	//
	//if <0 delayed goto OddSrcAddr with gr6<<31;
	//	ar0=gr0;
	//	ar6=gr6;
	//if <0 delayed goto Odd2OddCopy;
	//	gr7=[--ar5];// size
	//	gr7--;
	//if <= delayed goto SingleCopy gr7--;
	//	nul;
	//	nul;
	<Even2EvenCopy>
	if > delayed goto Even2EvenCopy;	
		ar0,gr0=[ar5++] with gr7--;
		[ar6++]=ar0,gr0 with gr7--;
	//	
	//<Even2OddCopy>
	//	ar1,gr1=[ar0++];
	//	ar4=gr1;
	//if > delayed skip -4 with gr7--; 
	//	gr5 = ar1 with gr4=gr5;
	//	[ar6++]=ar4,gr4;
	//	
	//<OddSrcAddr>
	//<Odd2OddCopy>
	
	//<Odd2EvenCopy>
	//<OneByOneCopy>
	//gr0=[ar0++];
	//[ar6++]=gr0;
	pop ar6,gr6;
	pop ar0,gr0;
	return;
.wait;



end ".text_hal";