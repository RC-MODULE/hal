
//#include "stdio.h"
extern "C"{
	
#define MANTISSA  0x007FFFFF
#define EXPONENT  0x7F800000
#define EXPONENT0 0x3F800000
#define SIGN      0x80000000

#define EXPONENT_64   0x7FF0000000000000l
#define EXPONENT0_64  0x3FF0000000000000l

union F32{
	unsigned i;
	float f;
} f32;

union F64 {
	unsigned long long i;
	double f;
} f64;

// Calcualate crc summ of float array 
unsigned halCrc_32f(float* src, int size){
	F32 crcMantissa;
	//F64 crcMantissa64;
	crcMantissa.f=1;
	//crcMantissa64.f=1;
	unsigned crcExp=0xBABADEDA;
	unsigned c;
	for(int i=0; i<size; i++){
		f32.f= src[i];
		c=crcExp>>31; 	
		crcExp<<=1;		
		crcExp|=c;					// do cyclic shift
		crcExp^=f32.i&~MANTISSA;	// acc exponenta and sign
		
		f32.i&=~EXPONENT;			// reset exponenta
		f32.i|= EXPONENT0;			// set exponenta = 0
		//f64.f=f32.f;
		
		crcMantissa.f+=f32.f;		// acc float
		crcMantissa.i&=~EXPONENT;	// reset exponenta
		crcMantissa.i|= EXPONENT0;	// set exponenta = 0
		
		//crcMantissa64.f+=f64.f;		// acc float
		//crcMantissa64.i&=~EXPONENT_64;	// reset exponenta
		//crcMantissa64.i|= EXPONENT0_64;	// set exponenta = 0
	}
	crcExp^=(crcExp<<16);
	crcExp^=(crcExp<<8);
	crcExp&=0xFF800000;
	f32.f=crcMantissa.f;
	//f32.f = crcMantissa64.f;
	return crcExp^f32.i;
}

unsigned halCrcAcc_32f(float* src, int size, unsigned *crc){
	(*crc)^=halCrc_32f(src,size);
	return *crc;
}
};