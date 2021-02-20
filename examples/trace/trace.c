
extern "C" {
#define NULL	((void *)0)

#define LLC_FUNC_INTERRUPT               (void**)0x164
typedef void (*usb_Interrupt) (void);
usb_Interrupt USB_Interrupt = (usb_Interrupt)(*LLC_FUNC_INTERRUPT);




typedef void (*stub_hnd)(int, void *);
typedef void *stub_prm;

struct nmc4_state {
	int pc_reg;
	int psw_reg;
	long regs[8];
}  __attribute__((packed, aligned(8)));

extern struct nmc4_state context;

extern void StubVectorsInit(void);
extern void StubVectorsRest(void);

static stub_hnd exeption_handler(int, stub_hnd, stub_prm) __attribute__((section(".trace.text")));

static void step_trace(int, void *) __attribute__((section(".trace.text")));
static void dummy(int, void *) __attribute__((section(".trace.text")));

stub_hnd stub_handle[4] __attribute__((section(".trace.data")))  = {dummy, dummy, dummy, dummy};
stub_prm stub_params[4] __attribute__((section(".trace.data"))) = {NULL, NULL, NULL, NULL};

#define TRACE_SIZE	0x400

long trace_buffer[TRACE_SIZE] __attribute__((section(".trace.bss")))  = {-1};
unsigned long trace_count __attribute__((section(".trace.data"))) = 0;
volatile  int wait __attribute__((section(".trace.data"))) = 0;

void start_trace(void) __attribute__((section(".trace.text")));
void start_trace(void)
{
	exeption_handler(3, step_trace, NULL);
	wait = 0;
	StubVectorsInit();
}

void stop_trace(void) __attribute__((section(".trace.text")));
void stop_trace(void)
{
	StubVectorsRest();
	wait = 1;
	exeption_handler(3, dummy, NULL);
}

static void step_trace(int id, void *args)
{
	trace_buffer[trace_count + 0] = *((long *)&context.pc_reg);
	//trace_buffer[trace_count + 1] = context.regs[0];
	//trace_buffer[trace_count + 2] = context.regs[3];
	//trace_buffer[trace_count + 3] = context.regs[7];

	trace_count = (trace_count + 1) & (TRACE_SIZE - 1);
	trace_buffer[trace_count] = 0xdeadc0dedeadc0de;

	while (wait) {};

	return;
}

static void dummy(int id, void *args)
{
	while (1) {};
}

static stub_hnd exeption_handler(int n, stub_hnd new_pnt, stub_prm prm)
{
	stub_hnd old_pnt;

	old_pnt = stub_handle[n];

	stub_handle[n] = new_pnt;
	stub_params[n] = prm;

	return old_pnt;
}

};