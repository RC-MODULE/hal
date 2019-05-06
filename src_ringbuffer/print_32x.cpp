#include "stdio.h"
extern "C" void print_32x(char* text,unsigned x){
	printf("%s%x\n",text,x);
}
