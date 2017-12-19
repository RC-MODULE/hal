#ifndef VIC_DEFINED
#define VIC_DEFINED

#define INTERRUPT  __attribute__ ((optimize ("0"))) __attribute__ ((interrupt ("IRQ")))  

#define VIC_Base0						0x20000000
#define VIC_Base1						0x20010000

#define PERIP_Base      		     	0x2000000          // Base address of Peripherals
//#define //PERIP_Size						0x09				//	01001		 256 KB 
#define PERIP_Size						0x0A				//	b01010				 512KB 

#define VIC_IRQSTATUS					0x000	// Read 32 0x00000000 VICIRQSTATUS IRQ Status Register
#define VIC_FIQSTATUS					0x04	//Read 32 0x00000000 VICFIQSTATUS FIQ Status Register
#define VIC_RAWINTR 					0x08	//Read 32 - VICRAWINTR Raw Interrupt Status Register
#define VIC_INTSELECT					0x00C	//Read/write 32 0x00000000 VICINTSELECT Interrupt Select Register
										// The VICINTSELECT Register selects whether the corresponding interrupt source generates an FIQ or IRQ interrupt. 
										// This register can be accessed with zero wait states.
										// [31:0] IntSelect Read/write Selects type of interrupt for interrupt rest:
										// 0 = IRQ interrupt (reset)
										// 1 = FIQ interrupt.
										// There is one bit ofthe register for each interrupt source.
										// 
#define VIC_INTENABLE					0x010	//Read/write 32 0x00000000 VICINTENABLE Interrupt Enable Register
										// [31:0] IntEnable Read/write Enables the interrupt rest lines, which allow the interrupts to reach the processor. 
										// Read:
										// 0 = interrupt disabled (reset) 1 = interrupt enabled.
										// The interrupt enable can only be set using this register. The VICINTENCLEAR Register must be used to disable the interrupt enable.
										// Write:
										// 0 = no effect
										// 1 = interrupt enabled.
										// On reset, all interrupts are disabled.
										// There is one bit of the register for each interrupt source.

#define VIC_INTENCLEAR					0x014	//Write 32 - VICINTENCLEAR Interrupt Enable Clear Register
										// The VICINTENCLEAR Register clears bits in the VICINTENABLE Register, 
										// and masks out the interrupt sources for the IRQ interrupt.
										// This register can be accessed with zero wait states.
										// [31:0] IntEnable Clear Write Clears corresponding bits in the VICINTENABLE Register: 
										// 0 = no effect
										// 1 = interrupt disabled in VICINTENABLE Register. There is one bit of the register for each interrupt source.
										
#define VIC_SOFTINT						0x01C	//Read/write 32 0x00000000 VICSOFTINT Software Interrupt Register
#define VIC_SOFTINTCLEAR				0x01C	//Write 32 - VICSOFTINTCLEAR Software Interrupt Clear Register
#define VIC_PROTECTION 					0x020 	//Read/write 1 0x0 VICPROTECTION Protection Enable Register
#define VIC_SWPRIORITY			 		0x024 	//Read/write 16 0xFFFF VICSWPRIORITY MASK Software Priority Mask Register
#define VIC_PRIORITYDAISY				0x028 	//Read/write 4 0xF VICPRIORITYDAISY Vector Priority Register for Daisy



#define VIC_VECTADDR					 0xf00	// Read/write 32 0x00000000 VICADDRESS Vector Address Register
										// Contains the address ofthe currently active ISR, with reset value 0x00000000.
										// A read of this register returns the address of the ISR and sets the current interrupt as being serviced. ]
										// A read must only be performed while there is an active interrupt. 
										// A write of any value to this register clears the current interrupt. 
										// A write must only be performed at the end of an interrupt service routine.


#define VIC_VECTADDR0					 0x100	// Read/write 32 0x00000000 VICVECTADDR0 Vector Address 0 Register
#define VIC_VECTADDR1					 0X104
#define VIC_VECTADDR2					 0X108
#define VIC_VECTADDR3					 0X10C
#define VIC_VECTADDR4					 0X110
#define VIC_VECTADDR5					 0X114
#define VIC_VECTADDR6					 0X118
#define VIC_VECTADDR7					 0X11C
#define VIC_VECTADDR8					 0X120
#define VIC_VECTADDR9					 0X124
#define VIC_VECTADDR10					 0X128
#define VIC_VECTADDR11					 0X12C
#define VIC_VECTADDR12					 0X130
#define VIC_VECTADDR13					 0X134
#define VIC_VECTADDR14					 0X138
#define VIC_VECTADDR15					 0X13C
#define VIC_VECTADDR16					 0x140
#define VIC_VECTADDR17					 0X144
#define VIC_VECTADDR18					 0X148
#define VIC_VECTADDR19					 0X14C
#define VIC_VECTADDR20					 0X150
#define VIC_VECTADDR21					 0X154
#define VIC_VECTADDR22					 0X158
#define VIC_VECTADDR23					 0X15C
#define VIC_VECTADDR24					 0X160
#define VIC_VECTADDR25					 0X164
#define VIC_VECTADDR26					 0X168
#define VIC_VECTADDR27					 0X16C
#define VIC_VECTADDR28					 0X170
#define VIC_VECTADDR29					 0X174
#define VIC_VECTADDR30					 0X178
#define VIC_VECTADDR31					 0X17C
	
	
	
#define VIC_VECTPRIORITY0     0x200    // R/W
#define VIC_VECTPRIORITY1     0x204    // R/W
#define VIC_VECTPRIORITY2     0x208    // R/W
#define VIC_VECTPRIORITY3     0x20C    // R/W
#define VIC_VECTPRIORITY4     0x210    // R/W
#define VIC_VECTPRIORITY5     0x214    // R/W
#define VIC_VECTPRIORITY6     0x218    // R/W
#define VIC_VECTPRIORITY7     0x21C    // R/W
#define VIC_VECTPRIORITY8     0x220    // R/W
#define VIC_VECTPRIORITY9     0x224    // R/W
#define VIC_VECTPRIORITY10     0x228    // R/W
#define VIC_VECTPRIORITY11     0x22C    // R/W
#define VIC_VECTPRIORITY12     0x230    // R/W
#define VIC_VECTPRIORITY13     0x234    // R/W
#define VIC_VECTPRIORITY14     0x238    // R/W
#define VIC_VECTPRIORITY15     0x23C    // R/W
#define VIC_VECTPRIORITY16     0x240    // R/W
#define VIC_VECTPRIORITY17     0x244    // R/W
#define VIC_VECTPRIORITY18     0x248    // R/W
#define VIC_VECTPRIORITY19     0x24C    // R/W
#define VIC_VECTPRIORITY20     0x250    // R/W
#define VIC_VECTPRIORITY21     0x254    // R/W
#define VIC_VECTPRIORITY22     0x258    // R/W
#define VIC_VECTPRIORITY23     0x25C    // R/W
#define VIC_VECTPRIORITY24     0x260    // R/W
#define VIC_VECTPRIORITY25     0x264    // R/W
#define VIC_VECTPRIORITY26     0x268    // R/W
#define VIC_VECTPRIORITY27     0x26C    // R/W
#define VIC_VECTPRIORITY28     0x270    // R/W
#define VIC_VECTPRIORITY29     0x274    // R/W
#define VIC_VECTPRIORITY30     0x278    // R/W
#define VIC_VECTPRIORITY31     0x27C    // R/	

//============ VIC0 ===============
#define IRQ_T0   	 1<<4
#define IRQ_T1	 	 1<<5
#define IRQ_NMC_HIGH 1<<14
#define IRQ_NMC_LOW  1<<15
#define IRQ_XDMAC0 	 1<<16
#define IRQ_XDMAC1 	 1<<17
#define IRQ_XDMAC2 	 1<<18
#define IRQ_XDMAC3 	 1<<19

//============ VIC1 ==================
#define IRQ_USB 	 1<<3
#define IRQ_NAND 	 1<<4
#define IRQ_TSP 	 1<<6
#define IRQ_CRYPTO 	 1<<8
#define IRQ_MSVD 	 1<<12
#define IRQ_I2S      1<<18
#define IRQ_SPDIF    1<<19
#define IRQ_AUDMA    1<<21
#define IRQ_TCM_ERR	 1<<22  
#define IRQ_TCM 	 1<<24  
	
#define VECT_WAITING 1
#define VECT_HANDLED 0







#endif
	