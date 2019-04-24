#ifndef SECTION_NMPROFILER_INCLUDED
#define SECTION_NMPROFILER_INCLUDED

#ifdef __GNUC__
#define INSECTION(name) __attribute__((section(name))) 
#else
#pragma data_section ".data_nmprofiler"
#pragma code_section ".text_nmprofiler"
#define INSECTION(name) 
#endif


#endif 