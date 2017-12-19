import from macros640x;

global start: label;

const STACK_ADDRESS = 1000h;
const RESULT_ADDR = 2004_0000h;
const RESULT_SUCCESS = 0FFFFFFFFh;
const RESULT_ERROR = 0h;

const ARM2NMC_NMI_FLAG = 2004_0000h;
const ARM2NMC_ARML_FLAG = (ARM2NMC_NMI_FLAG + 1);
const ARM2NMC_ARMH_FLAG = (ARM2NMC_NMI_FLAG + 2);
const NMC2ARM_NMCL_FLAG = (ARM2NMC_NMI_FLAG + 3);
const NMC2ARM_NMCH_FLAG = (ARM2NMC_NMI_FLAG + 4);
const FLAG_SUCCESS = 1;
const TIMEOUT = 1024;

const SCTL_Base = 0800_A400h;
const SCTL_IRQH_NMC0 = (SCTL_Base + 0Ch);
const SCTL_IRQL_NMC0 = (SCTL_Base + 0Dh);

macro NOP(count)
    .repeat count;
    nul;
    .endrepeat;
end NOP;

macro SLEEP(count)
    push ar0,gr0;
    own loop: label;
    gr0 = count;
    gr0--;
    <loop>
    if > goto loop with gr0--;
    pop ar0,gr0;
end SLEEP;

macro INTERRUPT_MASK(bits)
    pr4 = bits;
    NOP(9); //pipeline cleanup (required according to doc)
end INTERRUPT_MASK;

macro INTERRUPT_CLEAR(bits)
    pr5 = bits;
    NOP(9); //pipeline cleanup (required according to doc)
end INTERRUPT_CLEAR;

macro INTERRUPT_SET(bits)
    pr6 = bits;
end INTERRUPT_SET;

//NMI handler at address 0
begin test_go
    goto 200h;
end test_go;

//INT_ARMH_IN handler at address 80h
begin int_0
    [ARM2NMC_ARMH_FLAG] = gr7;
    ireturn;
end int_0;

//INT_ARML_IN handler at address C8h
begin int_9
    [ARM2NMC_ARML_FLAG] = gr7;
    ireturn;
end int_9;

begin textTest
<start>
.wait;

    REG_WINDOW0();

    gr0 = 0h;
    gr1 = 0h;
    gr2 = 0h;
    gr3 = 0h;
    gr4 = 0h;
    gr5 = 0h;
    gr6 = 0h;
    gr7 = FLAG_SUCCESS;

    // Init interrupt controller
    ar7 = STACK_ADDRESS;
    pswr set 40h; //allow external interrupt in NMC core

    INTERRUPT_CLEAR(0ffffh);    //clear all requests in INT controller
    INTERRUPT_MASK(0fdfeh);     //mask all interrupts except INT_ARMH_IN (IRQ0) and INT_ARML_IN (IRQ9)

    [ARM2NMC_NMI_FLAG] = gr7; //means NMC handled NMI interrupt

    //set interrupts for ARM
    pr7 = 0Ch; // ARMUH and ARMUL interrupts =>SCTL=>VIC=>ARM

<finish>
    goto finish;
end textTest;

data data_dummy
// Write all zeroes to tail of the program
    null_data: long[512] = ( 0l dup 512);
end data_dummy;
