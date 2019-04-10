extern "C"{
int nonfirst, chain[10], matrixcnt, packetcnt;
int ofdmainit;

#ifdef __GNUC__
__attribute__((section(".text_hal")))
#else
#pragma code_section ".text_hal"
#endif
void halOpenDMA()
{ 
	nonfirst=0; 
	chain[9]=0;
	matrixcnt=0;
	packetcnt=-1;
	ofdmainit=0;
}

};