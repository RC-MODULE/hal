global _halEnterCriticalSection_asm : label;
global _halExitCriticalSection_asm  : label;

extern _halSyncro    : word;

const CS_TURN   =_halSyncro	+ 1;
const CS_FLAG0  =_halSyncro	+ 2;

begin ".text_hal"
<_halEnterCriticalSection_asm>
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2; 

	//flag[id] =true;
	//turn = opid
	//while( flag[opid] && turn == opid)
	//{}

	gr2 = [40000000h];
	gr2>>=24; 								// gr2= id (0,1)
	ar2 = CS_FLAG0 with gr7 = true;
	ar1 = ar2;
	[ar2+=gr2] = gr7 with gr7 = -gr7; // flag[id]=true
	gr2 = not gr2;
	gr1 = gr2 and gr7;// gr1 = opid
	[CS_TURN] = gr1;	// turn = opid
	ar2 = ar1 + gr1;

	<WHILE>
	gr7 = [CS_TURN];	// gr7 = turn
	gr2 = [ar2]; 			// gr2=flag[opid];
	gr7 = gr7 xor gr1;// turn xor opid
	gr7 = not gr7;    // turn == opid
	gr7 = gr7 and gr2;
	gr7;
	if <>0 goto WHILE;
	pop  ar2,gr2;	
	pop  ar1,gr1;	
	pop  ar0,gr0;	
  return;

<_halExitCriticalSection_asm>	
	//flag[id] = false;
	gr7 = [40000000h];
	push ar0,gr0 with gr7 >>= 24;//got core ID
	ar0 = CS_FLAG0 with gr0 = gr7;
	ar0 = ar0 + gr0;
	gr7 = false;
	[ar0] = gr7; 
	pop ar0,gr0;
	return;


end  ".text_hal";