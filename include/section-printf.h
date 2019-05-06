#ifndef SECTION_PRINTF_INCLUDED
#define SECTION_PRINTF_INCLUDED

#ifdef __GNUC__
#define INSECTION(name) __attribute__((section(name))) 
#else
#pragma data_section ".data_printf"
#pragma code_section ".text_printf"
#define INSECTION(name) 
#endif


#endif 