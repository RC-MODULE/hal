const StubTableUSB = 0C0h;
const StubTableStart = 0_0000_0000h;
const StubStepVector = 3;
const StubVectorsNum = 4;
const StubVectorSize = 4;

struct nmc4_state
	pc_reg: word;
	psw_reg: word;
	regs: long[8];
end nmc4_state;

extern _USB_Interrupt: label;
extern _stub_handle: word[StubVectorsNum];
extern _stub_params: word[StubVectorsNum];

nobits ".stub_stack"
	STUB_STACK: long[512];
end ".stub_stack";

data ".stub_data"
	global _trap_addr: word = 0_0000_0000h;
	global _user_addr: word = 0_0000_0000h;

	.align;
	global _context: nmc4_state = (0, 0, (0L dup 8));

	local StoreIRQ: long[StubVectorSize * StubVectorsNum] = (0L dup (StubVectorSize * StubVectorsNum));
	local StoreUSB: long[4] = (0L dup 4);
	local IMRL_ST: word = 0;
	local IMRH_ST: word = 0;

end ".stub_data";

// To use only of an interrupt context!!!
macro SaveContext()
	[_context.regs[0]] = ar0,gr0;
	[_context.regs[1]] = ar1,gr1;
	ar0 = _context.regs[2];
	[ar0++] = ar2,gr2;
	[ar0++] = ar3,gr3;
	[ar0++] = ar4,gr4;
	[ar0++] = ar5,gr5;
	[ar0++] = ar6,gr6;
	ar1,gr1 = [--ar7];
	[ar0] = ar7,gr7;
	[_context.pc_reg] = ar1,gr1;
end SaveContext;

// To use only of an interrupt context!!!
macro RestoreContext()
	ar0 = _context.regs[2];
	ar2,gr2 = [ar0++];
	ar3,gr3 = [ar0++];
	ar4,gr4 = [ar0++];
	ar5,gr5 = [ar0++];
	ar6,gr6 = [ar0++];
	ar1,gr1 = [_context.pc_reg];
	ar7,gr7 = [ar0];
	[ar7++] = ar1,gr1;
	ar1,gr1 = [_context.regs[1]];
	ar0,gr0 = [_context.regs[0]];
end RestoreContext;


macro Mask_INT(Reg)
	Reg = [0_4000_0408h];
	[IMRL_ST] = Reg;
	Reg = 0_0010_0000h; // USB interrupt!
	[0_4000_0408h] = Reg;
	
	Reg = [0_4000_0448h];
	[IMRH_ST] = Reg;
	Reg = 0_0000h;
	[0_4000_0448h] = Reg;
end Mask_INT;

macro Rest_INT(Reg1, Reg2)
	Reg1 = [IMRL_ST];
	Reg2 = 0_0010_0000h;
	Reg1 = Reg1 or Reg2;
	[0_4000_0408h] = Reg1;

	Reg1 = [IMRH_ST];
	[0_4000_0448h] = Reg1;
end Rest_INT;

begin ".stub_text"
local StubHandler: label;
<StubHandler>
	SaveContext();
	Mask_INT(gr0);

	ar7 = STUB_STACK;

	ar0 = [_user_addr];
	ar0 = ar0 - 2;
	[_user_addr] = ar0;

	gr0 = [_trap_addr];
	gr0 = gr0 >> 3;
	gr1 = gr0 << 3;
	[_trap_addr] = gr1;

	ar7 = ar7 + 2;

	ar0 = _stub_params;
	gr1 = [ar0 += gr0];
	ar0 = _stub_handle;
	gr2 = [ar0 += gr0];

	[ar7 - 1] = gr0;
	[ar7 - 2] = gr1;

	with gr2 = gr2 or gr2;

	pswr set 0_0040h;
	if <>0 call gr2;
	pswr clear 0_0040h;

	ar7 = ar7 - 2;

	Rest_INT(gr0, gr1);
	RestoreContext();

	ireturn;

local StubVector: label;
<StubVector>
	pswr clear 0_01E0h;
	[_trap_addr] = pc;
	delayed goto StubHandler;
	[_user_addr] = ar7;


//---------------------------------------------------------
local USBHandler:label;
<USBHandler>
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	push ar7,gr7;

	ar0 = [_USB_Interrupt];
	call ar0;

	pop ar7,gr7;
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;

	ireturn;

//---------------------------------------------------------
local USBVec:label;
<USBVec>
	pswr clear 0_01E0h;
	[_trap_addr] = pc;
	delayed goto USBHandler;
	[_user_addr] = ar7;


global _StubVectorsInit: label;
<_StubVectorsInit>
	pswr clear 0_01E0h;

	push ar6,gr6;
	push ar5,gr5;
	push ar4,gr4;
	push ar3,gr3;


	ar6 = StubTableUSB;
	ar5 = USBVec;
	ar4 = StoreUSB;

.repeat 4;
	ar3,gr3 = [ar6];
	[ar4++] = ar3,gr3;
	ar3,gr3 = [ar5++];
	[ar6++] = ar3,gr3;
.endrepeat;


	ar6 = StubTableStart;
	gr6 = StubVectorsNum;
	ar4 = StoreIRQ;

<InitStubVectors>
	ar5 = StubVector;

.repeat StubVectorSize;
	ar3,gr3 = [ar6];
	[ar4++] = ar3,gr3;
	ar3,gr3 = [ar5++];
	[ar6++] = ar3,gr3;
.endrepeat;

	with gr6 = gr6 - 1;
	if <>0 skip InitStubVectors;

	pop ar3,gr3;
	pop ar4,gr4;
	pop ar5,gr5;
	pop ar6,gr6;

	delayed return;
	pswr set 0_00E0h;
	
global _StubVectorsRest: label;
<_StubVectorsRest>
	pswr clear 0_01E0h;

	[ar7++] = ar6,gr6;
	[ar7++] = ar5,gr5;
	[ar7++] = ar4,gr4;

	ar6 = StubTableUSB;
	ar5 = StoreUSB;

.repeat 4;
	ar4,gr4 = [ar5++];
	[ar6++] = ar4,gr4;
.endrepeat;


	ar6 = StubTableStart;
	gr6 = StubVectorsNum;
	ar5 = StoreIRQ;

<RestStubVectors>
.repeat StubVectorSize;
	ar4,gr4 = [ar5++];
	[ar6++] = ar4,gr4;
.endrepeat;

	with gr6 = gr6 - 1;
	if <>0 skip RestStubVectors;

	ar4,gr4 = [--ar7];
	ar5,gr5 = [--ar7];
	ar6,gr6 = [--ar7];

	delayed return;
	pswr set 0_0040h;
end ".stub_text";

