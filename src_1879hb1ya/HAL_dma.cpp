// при передаче из 0x100000 в 0x101000 (армовые адреса) такие времена(  все hex )
// bytes 80 100 200 400 800 1000 2000 4000 
// ticks d4 12d 1e2 34a 674 c13  1808 2f96 (3 такта на int32)
//#include "mb7707_load.h"
#include "time.h"
extern "C"
{
  int xdmacinit( int dstptr,  int srcptr, int n );
  int xdmac0(); // возвращает текущий счЄтчик (0 если передача окончена )
}
int m, ta, icnt;
const int Message_to_host = 0x12345678; //  // ѕосылаемое хосту сообщение
#define dmasrc 0x100000
#define dmadst 0x101000
#define dmacnt 0x20

void setintvecxdmac()
{ icnt=0;
	__asm { push ar4,gr4;
		push ar0,gr0;
		push ar5,gr5;
	     ar4=0B0h;
	     delayed goto vecB0;
	     push ar0,gr0;
	     push ar4,gr4;
<setintvec6>
	ar0,gr0=[ar5++];[ar4++]=ar0,gr0;
	ar0,gr0=[ar5++];[ar4++]=ar0,gr0;

		gr4=intr_mask; // pr4
		gr1=40h; // int 6
	        gr4=gr4 and not gr1; 
	        intr_mask=gr4;
        pop ar5,gr5;
	pop ar0,gr0;
	pop ar4,gr4;
	goto fin;	
<vecB0> delayed callrel setintvec6;
		ar5,gr5=[--ar7];
	        ar0,gr0=[ar5++];
	delayed goto inthandler6;
	push ar0,gr0; // int handler start here
	push ar4,gr4;
<inthandler6>
	ar0=icnt;
        gr0=[ar0];
	gr0=gr0+1;
	[ar0]=gr0;
	pop ar4,gr4;
	pop ar0,gr0;
	ireturn;
<fin>
        }

}
