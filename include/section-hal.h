#ifndef HAL_SECTION_INCLUDED
#define HAL_SECTION_INCLUDED

#ifdef __GNUC__
#define INSECTION(name) __attribute__((section(name))) 
#else
#pragma data_section ".data_hal"
#pragma code_section ".text_hal"
#define INSECTION(name) 
#endif


#endif 