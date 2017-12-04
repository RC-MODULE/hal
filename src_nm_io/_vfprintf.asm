
	// External references.
extern _vsprintf : label;
extern _fwrite : label;

	// Function declaration.
global __vfprintf : label;

	// Data declaration.
extern _vfprintf_buf : word[8096];

nobits ".bss"
	global _vfprintf_buf : word[8096];
end ".bss";

begin ".text"

global __vfprintf : label;
<__vfprintf>
<L1>

//--- d:\GIT\nm_io\src\nm_io_service\nm_part\_vfprintf.c

//1:    
//2:    #include "isc_lib\include\stdio.h"
//3:    
//4:    #include <cassert>
//5:    #include "string.h"
//6:    
//7:    using namespace std;
//8:    
//9:    #ifdef __cplusplus
//10:       extern "C" {
//11:   #endif
//12:   
//13:   char vfprintf_buf[ 8096 ];
//14:   
//15:   
//16:   int _vfprintf(FILE *stream, const char *format, va_list arg, int ){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
                                  //<16,1>

//17:       int r= vsprintf( vfprintf_buf, format, arg );

	ar1 = _vfprintf_buf set;                                  //<17,22>
	ar2 = [ar6+-6];                                  //<17,36>
	ar3 = [ar6+-7];                                  //<17,44>
	ar0 = ar7 set;                                  //<17,20>
	ar7 = ar7 + 4;                                  //<17,20>
	[ar0++] = ar0;                                  //<17,20>
	[ar0++] = ar3;                                  //<17,20>
	[ar0++] = ar2;                                  //<17,20>
	ar0 = ar0 set;                                  //<17,20>
	[ar0++] = ar1;                                  //<17,20>
	call _vsprintf;                                  //<17,20>
	ar7 = ar7 - 4;                                  //<17,20>
	ar0 = gr7 set;                                  //<17,20>
	[ar6] = ar0;                                  //<17,20>

//18:       if ( r>0 )

	gr1 = false noflags;                                  //<18,12>
	gr0 = [ar6];                                  //<18,10>
	gr0 - gr1;                                  //<18,12>
	if v> goto L2;                                  //<18,12>
	goto L3;                                  //<18,12>
<L2>

//19:           r= fwrite( vfprintf_buf, 1, r, stream );

	ar1 = _vfprintf_buf set;                                  //<19,40>
	ar0 = 1 set;                                  //<19,40>
	ar2 = ar0 set;                                  //<19,40>
	ar0 = [ar6];                                  //<19,40>
	ar3 = ar0 set;                                  //<19,40>
	ar4 = [ar6+-5];                                  //<19,40>
	ar0 = ar7 set;                                  //<19,40>
	ar7 = ar7 + 4;                                  //<19,40>
	[ar0++] = ar4;                                  //<19,40>
	[ar0++] = ar3;                                  //<19,40>
	[ar0++] = ar2;                                  //<19,40>
	ar0 = ar0 set;                                  //<19,40>
	[ar0++] = ar1;                                  //<19,40>
	call _fwrite;                                  //<19,40>
	ar7 = ar7 - 4;                                  //<19,40>
	ar0 = gr7 set;                                  //<19,40>
	ar0 = ar0 set;                                  //<19,40>
	[ar6] = ar0;                                  //<19,40>
	goto L3;                                  //<19,49>
<L3>

//20:       return r;

	ar0 = [ar6];                                  //<20,12>
	gr7 = ar0 set;                                  //<20,5>
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<20,5>

	//end of function

end ".text";

begin ".text"

end ".text";
