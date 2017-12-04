
	// External references.
extern _backBuffer : word;
extern _backBufferReady : word;
extern _errno : word;
extern ___assertfail : label;
extern _memcpy : label;
extern _strcpy : label;
extern _strlen : label;
extern _int._.0.8.8beginMessage.1.2 : label;
extern _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2 : label;

	// Function declaration.
global __putchar : label;
global _fff : label;
global _wait_for_host : label;
global __getchar : label;
global __open : label;
global __close : label;
global __read : label;
global __write : label;
global __flush : label;
global __getpos : label;
global __setpos : label;
global __rename : label;
global __remove : label;
global __getenv : label;
global _system : label;
global __stat64i32 : label;
global __sys_info : label;
global __getf_size : label;

	// Data declaration.


begin ".text"

global __putchar : label;
<__putchar>
<L1>

//--- d:\GIT\nm_io\src\nm_io_service\nm_part\env_call.c

//1:    //----------------------------------------------------------------------//
//2:    //                                                                      //
//3:    //         NeuroMatrix SDK                                              //
//4:    //         Библиотека С: работа с файлами                               //
//5:    //         Реализация системных вызовов при работе с файлами            //
//6:    //         как вызова удаленных процедур на хосте                       //
//7:    //         ответная часть на хосте - services.cpp                       //
//8:    //                                                                      //
//9:    //         Автор: Алексей Бирюков                                       //
//10:   //                                                                      //
//11:   //                      (c) RC Module Inc., Moscow, Russia              //
//12:   //                                                                      //
//13:   //----------------------------------------------------------------------//
//14:   #include <cassert>
//15:   #include "string.h"
//16:   #include "host_duties.h"
//17:   
//18:   typedef int _HANDLE;
//19:   typedef int fpos_t;
//20:   
//21:   //  Тип возвращаемый функцией _fstat, должен определяться в stat.h
//22:   
//23:   typedef unsigned short _ino_t;      /* i-node number (not used on DOS) */
//24:   typedef unsigned int _dev_t;        /* device code */
//25:   typedef int _off_t;                /* file offset value */
//26:   typedef long long __time64_t;     /* 64-bit time value */
//27:   
//28:   struct _stat64i32 {
//29:           _dev_t     st_dev;
//30:           _ino_t     st_ino;
//31:           unsigned short st_mode;
//32:           short      st_nlink;
//33:           short      st_uid;
//34:           short      st_gid;
//35:           _dev_t     st_rdev;
//36:           _off_t     st_size;
//37:           __time64_t st_atime;
//38:           __time64_t st_mtime;
//39:           __time64_t st_ctime;
//40:   };
//41:   
//42:   #define _stat  _stat64i32
//43:   #define _fstat  _stat64i32
//44:   
//45:   
//46:   int* beginMessage();
//47:   void completeMessage( NM_IO_ServiceID id, int size );
//48:   
//49:   //  errno ???
//50:   
//51:   #ifdef __cplusplus
//52:       extern "C" {
//53:   #endif
//54:   
//55:   extern int exit(int);
//56:   
//57:   extern volatile int backBuffer[ backBufferSize ];
//58:   extern volatile int backBufferReady;
//59:   extern int errno;
//60:   
//61:   int _putchar(int ch){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<61,1>

//62:       char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<62,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<62,23>
	ar0 = ar5 set;                                  //<62,23>
	[ar6] = ar0;                                  //<62,23>

//63:   	*buf++ =ch;

	ar0 = [ar6];                                  //<63,10>
	ar1 = 1 set;                                  //<63,10>
	gr0 = ar0 set;                                  //<63,10>
	gr1 = ar1 set;                                  //<63,10>
	gr0 = gr0 + gr1 noflags;                                  //<63,10>
	ar1 = gr0 set;                                  //<63,10>
	[ar6] = ar1;                                  //<63,10>
	ar1 = [ar6+-5];                                  //<63,10>
	gr0 = ar1 set;                                  //<63,10>
	gr0 = gr0 << 24;                                  //<63,10>
	gr0 = gr0 A>> 24;                                  //<63,10>
	[ar0] = gr0;                                  //<63,10>

//64:   	completeMessage( INDEX_PUTCHAR, 1 );

	ar1 = 1 set;                                  //<64,34>
	ar2 = 1 set;                                  //<64,34>
	ar0 = ar7 set;                                  //<64,34>
	ar7 = ar7 + 2;                                  //<64,34>
	[ar0++] = ar2;                                  //<64,34>
	ar0 = ar0 set;                                  //<64,34>
	[ar0++] = ar1;                                  //<64,34>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<64,34>
	ar7 = ar7 - 2;                                  //<64,34>

//65:       return 0;

	gr0 = false noflags;                                  //<65,12>
	gr7 = gr0 set;                                  //<65,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<65,5>

	//end of function

end ".text";

begin ".text"

global _fff : label;
<_fff>
<L2>

//66:   }
//67:   
//68:   int fff( volatile int i)//  for volatile bug

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
                                  //<68,1>

//69:   {
//70:       return i;

	ar0 = [ar6+-5];                                  //<70,12>
	gr7 = ar0 set;                                  //<70,5>
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<70,5>

	//end of function

end ".text";

begin ".text"

global _wait_for_host : label;
<_wait_for_host>
<L3>

//71:   }
//72:   
//73:   void wait_for_host()

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
                                  //<73,1>

//74:   {
//75:   	while ( fff( backBufferReady )==0 )

	goto L4;                                  //<75,2>
<L4>
<L5>
	gr1 = false noflags;                                  //<75,34>
	ar1 = [_backBufferReady];                                  //<75,15>
	ar0 = ar7 set;                                  //<75,13>
	ar7 = ar7 + 2;                                  //<75,13>
	[ar0++] = ar0;                                  //<75,13>
	ar0 = ar0 set;                                  //<75,13>
	[ar0++] = ar1;                                  //<75,13>
	call _fff;                                  //<75,13>
	ar7 = ar7 - 2;                                  //<75,13>
	gr0 = gr7 set;                                  //<75,13>
	gr0 - gr1;                                  //<75,32>
	if =0 goto L4;                                  //<75,32>
	goto L6;                                  //<75,32>
<L6>

//76:   		;
//77:   }

	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<77,1>

	//end of function

end ".text";

begin ".text"

global __getchar : label;
<__getchar>
<L7>

//78:   
//79:   int _getchar(void){

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
                                  //<79,1>

//80:       beginMessage();

	ar0 = ar7 set;                                  //<80,5>
	call _int._.0.8.8beginMessage.1.2;                                  //<80,5>
	ar0 = ar5 set;                                  //<80,5>

//81:   	completeMessage( INDEX_GETCHAR, 0 );

	ar1 = 2 set;                                  //<81,34>
	gr0 = false noflags;                                  //<81,34>
	ar0 = ar7 set;                                  //<81,34>
	ar7 = ar7 + 2;                                  //<81,34>
	[ar0++] = gr0;                                  //<81,34>
	ar0 = ar0 set;                                  //<81,34>
	[ar0++] = ar1;                                  //<81,34>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<81,34>
	ar7 = ar7 - 2;                                  //<81,34>

//82:       wait_for_host();

	ar0 = ar7 set;                                  //<82,5>
	call _wait_for_host;                                  //<82,5>

//83:       assert(backBufferReady==1);

	gr1 = 1 set;                                  //<83,32>
	gr0 = [_backBufferReady];                                  //<83,32>
	gr0 - gr1;                                  //<83,32>
	if =0 goto L10;                                  //<83,32>
	goto L9;                                  //<83,32>
<L9>
	ar0 = ar7 set;                                  //<83,32>
	call ___assertfail;                                  //<83,32>
	goto L10;                                  //<83,32>
<L10>
<L8>

//84:   	backBufferReady=0;

	gr0 = false noflags;                                  //<84,18>
	[_backBufferReady] = gr0;                                  //<84,18>

//85:       return backBuffer[0];

	gr0 = false noflags;                                  //<85,23>
	ar1 = _backBuffer set;                                  //<85,12>
	ar0 = gr0 set;                                  //<85,23>
	gr0 = ar1 set;                                  //<85,23>
	gr1 = ar0 set;                                  //<85,23>
	gr0 = gr0 + gr1 noflags;                                  //<85,23>
	ar0 = [gr0];                                  //<85,23>
	gr7 = ar0 set;                                  //<85,5>
	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<85,5>

	//end of function

end ".text";

begin ".text"

global __open : label;
<__open>
<L11>

//86:   }
//87:   
//88:   _HANDLE _open(const char* fileName, unsigned int oflags){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<88,1>

//89:       int* buf= beginMessage();

	ar0 = ar7 set;                                  //<89,15>
	call _int._.0.8.8beginMessage.1.2;                                  //<89,15>
	ar0 = ar5 set;                                  //<89,15>
	[ar6] = ar0;                                  //<89,15>

//90:       int* end= buf;

	ar0 = [ar6];                                  //<90,15>
	[ar6 + 1] = ar0;                                  //<90,15>

//91:       do

	goto L12;                                  //<91,5>
<L12>

//92:           *end++ = *fileName;

	ar0 = [ar6+1];                                  //<92,19>
	ar1 = 1 set;                                  //<92,19>
	gr0 = ar0 set;                                  //<92,19>
	gr1 = ar1 set;                                  //<92,19>
	gr0 = gr0 + gr1 noflags;                                  //<92,19>
	ar1 = gr0 set;                                  //<92,19>
	[ar6 + 1] = ar1;                                  //<92,19>
	ar1 = [ar6+-5];                                  //<92,19>
	ar1 = [ar1];                                  //<92,19>
	ar1 = ar1 set;                                  //<92,19>
	[ar0] = ar1;                                  //<92,19>

//93:       while (*fileName++);

	ar0 = [ar6+-5];                                  //<93,21>
	ar1 = 1 set;                                  //<93,21>
	gr0 = ar0 set;                                  //<93,21>
	gr1 = ar1 set;                                  //<93,21>
	gr0 = gr0 + gr1 noflags;                                  //<93,21>
	ar1 = gr0 set;                                  //<93,21>
	[ar6 + -5] = ar1;                                  //<93,21>
	ar0 = [ar0];                                  //<93,12>
	gr0 = ar0 set;                                  //<93,12>
	gr1 = false noflags;                                  //<93,12>
	gr0 - gr1;                                  //<93,12>
	if <>0 goto L12;                                  //<93,12>
	goto L14;                                  //<93,12>
<L14>

//94:   	*end++ =oflags;

	ar0 = [ar6+1];                                  //<94,10>
	ar1 = 1 set;                                  //<94,10>
	gr0 = ar0 set;                                  //<94,10>
	gr1 = ar1 set;                                  //<94,10>
	gr0 = gr0 + gr1 noflags;                                  //<94,10>
	ar1 = gr0 set;                                  //<94,10>
	[ar6 + 1] = ar1;                                  //<94,10>
	ar1 = [ar6+-6];                                  //<94,10>
	ar1 = ar1 set;                                  //<94,10>
	[ar0] = ar1;                                  //<94,10>

//95:   	completeMessage( INDEX_OPEN, end-buf );

	ar1 = 3 set;                                  //<95,35>
	ar0 = [ar6];                                  //<95,35>
	ar2 = [ar6+1];                                  //<95,35>
	gr0 = ar2 set;                                  //<95,35>
	gr1 = ar0 set;                                  //<95,35>
	gr0 = gr0 - gr1 noflags;                                  //<95,35>
	ar0 = gr0 set;                                  //<95,35>
	ar2 = ar0 set;                                  //<95,35>
	ar0 = ar7 set;                                  //<95,35>
	ar7 = ar7 + 2;                                  //<95,35>
	[ar0++] = ar2;                                  //<95,35>
	ar0 = ar0 set;                                  //<95,35>
	[ar0++] = ar1;                                  //<95,35>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<95,35>
	ar7 = ar7 - 2;                                  //<95,35>

//96:       wait_for_host();

	ar0 = ar7 set;                                  //<96,5>
	call _wait_for_host;                                  //<96,5>

//97:       assert(backBufferReady==2);

	gr1 = 2 set;                                  //<97,32>
	gr0 = [_backBufferReady];                                  //<97,32>
	gr0 - gr1;                                  //<97,32>
	if =0 goto L17;                                  //<97,32>
	goto L16;                                  //<97,32>
<L16>
	ar0 = ar7 set;                                  //<97,32>
	call ___assertfail;                                  //<97,32>
	goto L17;                                  //<97,32>
<L17>
<L15>

//98:   	backBufferReady=0;

	gr0 = false noflags;                                  //<98,18>
	[_backBufferReady] = gr0;                                  //<98,18>

//99:       errno= backBuffer[1];

	ar0 = 1 set;                                  //<99,23>
	ar1 = _backBuffer set;                                  //<99,23>
	ar0 = ar0 set;                                  //<99,23>
	gr0 = ar1 set;                                  //<99,23>
	gr1 = ar0 set;                                  //<99,23>
	gr0 = gr0 + gr1 noflags;                                  //<99,23>
	ar0 = [gr0];                                  //<99,23>
	[_errno] = ar0;                                  //<99,23>

//100:      return backBuffer[0];

	gr0 = false noflags;                                  //<100,23>
	ar1 = _backBuffer set;                                  //<100,12>
	ar0 = gr0 set;                                  //<100,23>
	gr0 = ar1 set;                                  //<100,23>
	gr1 = ar0 set;                                  //<100,23>
	gr0 = gr0 + gr1 noflags;                                  //<100,23>
	ar0 = [gr0];                                  //<100,23>
	gr7 = ar0 set;                                  //<100,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<100,5>

	//end of function

end ".text";

begin ".text"

global __close : label;
<__close>
<L18>

//101:  }
//102:  
//103:  int _close(_HANDLE file){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<103,1>

//104:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<104,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<104,23>
	ar0 = ar5 set;                                  //<104,23>
	[ar6] = ar0;                                  //<104,23>

//105:  	*buf++ =file;

	ar0 = [ar6];                                  //<105,10>
	ar1 = 1 set;                                  //<105,10>
	gr0 = ar0 set;                                  //<105,10>
	gr1 = ar1 set;                                  //<105,10>
	gr0 = gr0 + gr1 noflags;                                  //<105,10>
	ar1 = gr0 set;                                  //<105,10>
	[ar6] = ar1;                                  //<105,10>
	ar1 = [ar6+-5];                                  //<105,10>
	gr0 = ar1 set;                                  //<105,10>
	gr0 = gr0 << 24;                                  //<105,10>
	gr0 = gr0 A>> 24;                                  //<105,10>
	[ar0] = gr0;                                  //<105,10>

//106:  	completeMessage( INDEX_CLOSE, 1 );

	ar1 = 4 set;                                  //<106,32>
	ar2 = 1 set;                                  //<106,32>
	ar0 = ar7 set;                                  //<106,32>
	ar7 = ar7 + 2;                                  //<106,32>
	[ar0++] = ar2;                                  //<106,32>
	ar0 = ar0 set;                                  //<106,32>
	[ar0++] = ar1;                                  //<106,32>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<106,32>
	ar7 = ar7 - 2;                                  //<106,32>

//107:      wait_for_host();

	ar0 = ar7 set;                                  //<107,5>
	call _wait_for_host;                                  //<107,5>

//108:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<108,32>
	gr0 = [_backBufferReady];                                  //<108,32>
	gr0 - gr1;                                  //<108,32>
	if =0 goto L21;                                  //<108,32>
	goto L20;                                  //<108,32>
<L20>
	ar0 = ar7 set;                                  //<108,32>
	call ___assertfail;                                  //<108,32>
	goto L21;                                  //<108,32>
<L21>
<L19>

//109:  	backBufferReady=0;

	gr0 = false noflags;                                  //<109,18>
	[_backBufferReady] = gr0;                                  //<109,18>

//110:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<110,23>
	ar1 = _backBuffer set;                                  //<110,23>
	ar0 = ar0 set;                                  //<110,23>
	gr0 = ar1 set;                                  //<110,23>
	gr1 = ar0 set;                                  //<110,23>
	gr0 = gr0 + gr1 noflags;                                  //<110,23>
	ar0 = [gr0];                                  //<110,23>
	[_errno] = ar0;                                  //<110,23>

//111:      return backBuffer[0];

	gr0 = false noflags;                                  //<111,23>
	ar1 = _backBuffer set;                                  //<111,12>
	ar0 = gr0 set;                                  //<111,23>
	gr0 = ar1 set;                                  //<111,23>
	gr1 = ar0 set;                                  //<111,23>
	gr0 = gr0 + gr1 noflags;                                  //<111,23>
	ar0 = [gr0];                                  //<111,23>
	gr7 = ar0 set;                                  //<111,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<111,5>

	//end of function

end ".text";

begin ".text"

global __read : label;
<__read>
<L22>

//112:  }
//113:  
//114:  int _read(_HANDLE file, void* dst, int size){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 4;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<114,1>

//115:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<115,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<115,23>
	ar0 = ar5 set;                                  //<115,23>
	[ar6] = ar0;                                  //<115,23>

//116:      *buf++= file;

	ar0 = [ar6];                                  //<116,13>
	ar1 = 1 set;                                  //<116,13>
	gr0 = ar0 set;                                  //<116,13>
	gr1 = ar1 set;                                  //<116,13>
	gr0 = gr0 + gr1 noflags;                                  //<116,13>
	ar1 = gr0 set;                                  //<116,13>
	[ar6] = ar1;                                  //<116,13>
	ar1 = [ar6+-5];                                  //<116,13>
	gr0 = ar1 set;                                  //<116,13>
	gr0 = gr0 << 24;                                  //<116,13>
	gr0 = gr0 A>> 24;                                  //<116,13>
	[ar0] = gr0;                                  //<116,13>

//117:      *buf++= reinterpret_cast<int>( dst);

	ar0 = [ar6];                                  //<117,40>
	ar1 = 1 set;                                  //<117,40>
	gr0 = ar0 set;                                  //<117,40>
	gr1 = ar1 set;                                  //<117,40>
	gr0 = gr0 + gr1 noflags;                                  //<117,40>
	ar1 = gr0 set;                                  //<117,40>
	[ar6] = ar1;                                  //<117,40>
	ar1 = [ar6+-6];                                  //<117,40>
	ar1 = ar1 set;                                  //<117,40>
	gr0 = ar1 set;                                  //<117,40>
	gr0 = gr0 << 24;                                  //<117,40>
	gr0 = gr0 A>> 24;                                  //<117,40>
	[ar0] = gr0;                                  //<117,40>

//118:      *buf++= size;

	ar0 = [ar6];                                  //<118,13>
	ar1 = 1 set;                                  //<118,13>
	gr0 = ar0 set;                                  //<118,13>
	gr1 = ar1 set;                                  //<118,13>
	gr0 = gr0 + gr1 noflags;                                  //<118,13>
	ar1 = gr0 set;                                  //<118,13>
	[ar6] = ar1;                                  //<118,13>
	ar1 = [ar6+-7];                                  //<118,13>
	gr0 = ar1 set;                                  //<118,13>
	gr0 = gr0 << 24;                                  //<118,13>
	gr0 = gr0 A>> 24;                                  //<118,13>
	[ar0] = gr0;                                  //<118,13>

//119:  	completeMessage( INDEX_READ, 3 );

	ar1 = 5 set;                                  //<119,31>
	ar2 = 3 set;                                  //<119,31>
	ar0 = ar7 set;                                  //<119,31>
	ar7 = ar7 + 2;                                  //<119,31>
	[ar0++] = ar2;                                  //<119,31>
	ar0 = ar0 set;                                  //<119,31>
	[ar0++] = ar1;                                  //<119,31>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<119,31>
	ar7 = ar7 - 2;                                  //<119,31>

//120:      wait_for_host();

	ar0 = ar7 set;                                  //<120,5>
	call _wait_for_host;                                  //<120,5>

//121:      assert( backBufferReady== backBuffer[0]+2 );

	gr1 = 2 set;                                  //<121,49>
	gr0 = false noflags;                                  //<121,49>
	ar1 = _backBuffer set;                                  //<121,49>
	ar0 = gr0 set;                                  //<121,49>
	gr0 = ar1 set;                                  //<121,49>
	gr2 = ar0 set;                                  //<121,49>
	gr0 = gr0 + gr2 noflags;                                  //<121,49>
	gr0 = [gr0];                                  //<121,49>
	gr1 = gr0 + gr1 noflags;                                  //<121,49>
	gr0 = [_backBufferReady];                                  //<121,49>
	gr0 - gr1;                                  //<121,49>
	if =0 goto L25;                                  //<121,49>
	goto L24;                                  //<121,49>
<L24>
	ar0 = ar7 set;                                  //<121,49>
	call ___assertfail;                                  //<121,49>
	goto L25;                                  //<121,49>
<L25>
<L23>

//122:      int i;
//123:      int* iDst= (int*)dst;

	ar0 = [ar6+-6];                                  //<123,22>
	[ar6 + 2] = ar0;                                  //<123,22>

//124:      if ( backBuffer[0] >0 ){

	gr1 = false noflags;                                  //<124,25>
	gr0 = false noflags;                                  //<124,21>
	ar1 = _backBuffer set;                                  //<124,10>
	ar0 = gr0 set;                                  //<124,21>
	gr0 = ar1 set;                                  //<124,21>
	gr2 = ar0 set;                                  //<124,21>
	gr0 = gr0 + gr2 noflags;                                  //<124,21>
	gr0 = [gr0];                                  //<124,21>
	gr0 - gr1;                                  //<124,25>
	if v> goto L26;                                  //<124,25>
	goto L27;                                  //<124,25>
<L26>

//125:          for (i=0; i<backBuffer[0]; i++ )

	gr0 = false noflags;                                  //<125,16>
	[ar6 + 1] = gr0;                                  //<125,16>
	goto L28;                                  //<125,9>
<L29>

//126:          {
//127:              *iDst++ = backBuffer[i+2];

	ar0 = [ar6+2];                                  //<127,36>
	ar1 = 1 set;                                  //<127,36>
	gr0 = ar0 set;                                  //<127,36>
	gr1 = ar1 set;                                  //<127,36>
	gr0 = gr0 + gr1 noflags;                                  //<127,36>
	ar1 = gr0 set;                                  //<127,36>
	[ar6 + 2] = ar1;                                  //<127,36>
	gr1 = 2 set;                                  //<127,36>
	gr0 = [ar6+1];                                  //<127,36>
	gr0 = gr0 + gr1 noflags;                                  //<127,36>
	ar2 = _backBuffer set;                                  //<127,36>
	ar1 = gr0 set;                                  //<127,36>
	gr0 = ar2 set;                                  //<127,36>
	gr1 = ar1 set;                                  //<127,36>
	gr0 = gr0 + gr1 noflags;                                  //<127,36>
	ar1 = [gr0];                                  //<127,36>
	[ar0] = ar1;                                  //<127,36>
	gr0 = [ar6+1];                                  //<125,37>
	gr1 = 1 set;                                  //<125,37>
	gr0 = gr0 + gr1 noflags;                                  //<125,37>
	[ar6 + 1] = gr0;                                  //<125,37>
	goto L28;                                  //<125,9>
<L28>
	gr0 = false noflags;                                  //<125,32>
	ar1 = _backBuffer set;                                  //<125,21>
	ar0 = gr0 set;                                  //<125,32>
	gr0 = ar1 set;                                  //<125,32>
	gr1 = ar0 set;                                  //<125,32>
	gr0 = gr0 + gr1 noflags;                                  //<125,32>
	gr1 = [gr0];                                  //<125,32>
	gr0 = [ar6+1];                                  //<125,19>
	gr0 - gr1;                                  //<125,20>
	if v< goto L29;                                  //<125,20>
	goto L27;                                  //<125,20>
<L27>
<L30>

//128:          }
//129:      }
//130:      backBufferReady= 0;

	gr0 = false noflags;                                  //<130,22>
	[_backBufferReady] = gr0;                                  //<130,22>

//131:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<131,23>
	ar1 = _backBuffer set;                                  //<131,23>
	ar0 = ar0 set;                                  //<131,23>
	gr0 = ar1 set;                                  //<131,23>
	gr1 = ar0 set;                                  //<131,23>
	gr0 = gr0 + gr1 noflags;                                  //<131,23>
	ar0 = [gr0];                                  //<131,23>
	[_errno] = ar0;                                  //<131,23>

//132:      return backBuffer[0];

	gr0 = false noflags;                                  //<132,23>
	ar1 = _backBuffer set;                                  //<132,12>
	ar0 = gr0 set;                                  //<132,23>
	gr0 = ar1 set;                                  //<132,23>
	gr1 = ar0 set;                                  //<132,23>
	gr0 = gr0 + gr1 noflags;                                  //<132,23>
	ar0 = [gr0];                                  //<132,23>
	gr7 = ar0 set;                                  //<132,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 4;
	pop ar6,gr6;
return;                                  //<132,5>

	//end of function

end ".text";

begin ".text"

global __write : label;
<__write>
<L32>

//133:  }
//134:  
//135:  int _write(_HANDLE file, const void* content, int size){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<135,1>

//136:  //    exit (115);
//137:      int* buf= (int*)beginMessage();

	ar0 = ar7 set;                                  //<137,21>
	call _int._.0.8.8beginMessage.1.2;                                  //<137,21>
	ar0 = ar5 set;                                  //<137,21>
	[ar6] = ar0;                                  //<137,21>

//138:      *buf++= file;

	ar0 = [ar6];                                  //<138,13>
	ar1 = 1 set;                                  //<138,13>
	gr0 = ar0 set;                                  //<138,13>
	gr1 = ar1 set;                                  //<138,13>
	gr0 = gr0 + gr1 noflags;                                  //<138,13>
	ar1 = gr0 set;                                  //<138,13>
	[ar6] = ar1;                                  //<138,13>
	ar1 = [ar6+-5];                                  //<138,13>
	[ar0] = ar1;                                  //<138,13>

//139:      *buf++= size;

	ar0 = [ar6];                                  //<139,13>
	ar1 = 1 set;                                  //<139,13>
	gr0 = ar0 set;                                  //<139,13>
	gr1 = ar1 set;                                  //<139,13>
	gr0 = gr0 + gr1 noflags;                                  //<139,13>
	ar1 = gr0 set;                                  //<139,13>
	[ar6] = ar1;                                  //<139,13>
	ar1 = [ar6+-7];                                  //<139,13>
	[ar0] = ar1;                                  //<139,13>

//140:      int i;
//141:      for (i=0; i<size; i++ )

	gr0 = false noflags;                                  //<141,12>
	[ar6 + 1] = gr0;                                  //<141,12>
	goto L33;                                  //<141,5>
<L34>

//142:      {
//143:          *buf++= ((int*)content)[i];

	ar0 = [ar6];                                  //<143,34>
	ar1 = 1 set;                                  //<143,34>
	gr0 = ar0 set;                                  //<143,34>
	gr1 = ar1 set;                                  //<143,34>
	gr0 = gr0 + gr1 noflags;                                  //<143,34>
	ar1 = gr0 set;                                  //<143,34>
	[ar6] = ar1;                                  //<143,34>
	ar1 = [ar6+1];                                  //<143,34>
	ar2 = [ar6+-6];                                  //<143,34>
	ar1 = ar1 set;                                  //<143,34>
	gr0 = ar2 set;                                  //<143,34>
	gr1 = ar1 set;                                  //<143,34>
	gr0 = gr0 + gr1 noflags;                                  //<143,34>
	ar1 = [gr0];                                  //<143,34>
	[ar0] = ar1;                                  //<143,34>
	gr0 = [ar6+1];                                  //<141,24>
	gr1 = 1 set;                                  //<141,24>
	gr0 = gr0 + gr1 noflags;                                  //<141,24>
	[ar6 + 1] = gr0;                                  //<141,24>
	goto L33;                                  //<141,5>
<L33>
	gr1 = [ar6+-7];                                  //<141,17>
	gr0 = [ar6+1];                                  //<141,15>
	gr0 - gr1;                                  //<141,16>
	if v< goto L34;                                  //<141,16>
	goto L35;                                  //<141,16>
<L35>

//144:      }
//145:  	completeMessage( INDEX_WRITE, size+2 );

	ar1 = 6 set;                                  //<145,37>
	gr1 = 2 set;                                  //<145,37>
	gr0 = [ar6+-7];                                  //<145,37>
	gr0 = gr0 + gr1 noflags;                                  //<145,37>
	ar0 = ar7 set;                                  //<145,37>
	ar7 = ar7 + 2;                                  //<145,37>
	[ar0++] = gr0;                                  //<145,37>
	ar0 = ar0 set;                                  //<145,37>
	[ar0++] = ar1;                                  //<145,37>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<145,37>
	ar7 = ar7 - 2;                                  //<145,37>

//146:      wait_for_host();

	ar0 = ar7 set;                                  //<146,5>
	call _wait_for_host;                                  //<146,5>

//147:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<147,32>
	gr0 = [_backBufferReady];                                  //<147,32>
	gr0 - gr1;                                  //<147,32>
	if =0 goto L39;                                  //<147,32>
	goto L38;                                  //<147,32>
<L38>
	ar0 = ar7 set;                                  //<147,32>
	call ___assertfail;                                  //<147,32>
	goto L39;                                  //<147,32>
<L39>
<L37>

//148:  	backBufferReady=0;

	gr0 = false noflags;                                  //<148,18>
	[_backBufferReady] = gr0;                                  //<148,18>

//149:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<149,23>
	ar1 = _backBuffer set;                                  //<149,23>
	ar0 = ar0 set;                                  //<149,23>
	gr0 = ar1 set;                                  //<149,23>
	gr1 = ar0 set;                                  //<149,23>
	gr0 = gr0 + gr1 noflags;                                  //<149,23>
	ar0 = [gr0];                                  //<149,23>
	[_errno] = ar0;                                  //<149,23>

//150:      return backBuffer[0];

	gr0 = false noflags;                                  //<150,23>
	ar1 = _backBuffer set;                                  //<150,12>
	ar0 = gr0 set;                                  //<150,23>
	gr0 = ar1 set;                                  //<150,23>
	gr1 = ar0 set;                                  //<150,23>
	gr0 = gr0 + gr1 noflags;                                  //<150,23>
	ar0 = [gr0];                                  //<150,23>
	gr7 = ar0 set;                                  //<150,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<150,5>

	//end of function

end ".text";

begin ".text"

global __flush : label;
<__flush>
<L40>

//151:  }
//152:  
//153:  int _flush( _HANDLE ){

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
                                  //<153,1>

//154:      //  параметр игнорируем, поскольку делать будем _flushall:
//155:      //  _flush(_HANDLE) на хосте нету
//156:      beginMessage();

	ar0 = ar7 set;                                  //<156,5>
	call _int._.0.8.8beginMessage.1.2;                                  //<156,5>
	ar0 = ar5 set;                                  //<156,5>

//157:  	completeMessage( INDEX_FLUSH, 0 );

	ar1 = 7 set;                                  //<157,32>
	gr0 = false noflags;                                  //<157,32>
	ar0 = ar7 set;                                  //<157,32>
	ar7 = ar7 + 2;                                  //<157,32>
	[ar0++] = gr0;                                  //<157,32>
	ar0 = ar0 set;                                  //<157,32>
	[ar0++] = ar1;                                  //<157,32>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<157,32>
	ar7 = ar7 - 2;                                  //<157,32>

//158:      return 0;

	gr0 = false noflags;                                  //<158,12>
	gr7 = gr0 set;                                  //<158,5>
	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<158,5>

	//end of function

end ".text";

begin ".text"

global __getpos : label;
<__getpos>
<L41>

//159:  }
//160:  
//161:  int _getpos(_HANDLE file, fpos_t* pos){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<161,1>

//162:      int* buf= (int*)beginMessage();

	ar0 = ar7 set;                                  //<162,21>
	call _int._.0.8.8beginMessage.1.2;                                  //<162,21>
	ar0 = ar5 set;                                  //<162,21>
	[ar6] = ar0;                                  //<162,21>

//163:      *buf++= file;

	ar0 = [ar6];                                  //<163,13>
	ar1 = 1 set;                                  //<163,13>
	gr0 = ar0 set;                                  //<163,13>
	gr1 = ar1 set;                                  //<163,13>
	gr0 = gr0 + gr1 noflags;                                  //<163,13>
	ar1 = gr0 set;                                  //<163,13>
	[ar6] = ar1;                                  //<163,13>
	ar1 = [ar6+-5];                                  //<163,13>
	[ar0] = ar1;                                  //<163,13>

//164:  	completeMessage( INDEX_GETPOS, 1 );

	ar1 = 8 set;                                  //<164,33>
	ar2 = 1 set;                                  //<164,33>
	ar0 = ar7 set;                                  //<164,33>
	ar7 = ar7 + 2;                                  //<164,33>
	[ar0++] = ar2;                                  //<164,33>
	ar0 = ar0 set;                                  //<164,33>
	[ar0++] = ar1;                                  //<164,33>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<164,33>
	ar7 = ar7 - 2;                                  //<164,33>

//165:      wait_for_host();

	ar0 = ar7 set;                                  //<165,5>
	call _wait_for_host;                                  //<165,5>

//166:      assert(backBufferReady== 2 );

	gr1 = 2 set;                                  //<166,34>
	gr0 = [_backBufferReady];                                  //<166,34>
	gr0 - gr1;                                  //<166,34>
	if =0 goto L44;                                  //<166,34>
	goto L43;                                  //<166,34>
<L43>
	ar0 = ar7 set;                                  //<166,34>
	call ___assertfail;                                  //<166,34>
	goto L44;                                  //<166,34>
<L44>
<L42>

//167:  	backBufferReady=0;

	gr0 = false noflags;                                  //<167,18>
	[_backBufferReady] = gr0;                                  //<167,18>

//168:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<168,23>
	ar1 = _backBuffer set;                                  //<168,23>
	ar0 = ar0 set;                                  //<168,23>
	gr0 = ar1 set;                                  //<168,23>
	gr1 = ar0 set;                                  //<168,23>
	gr0 = gr0 + gr1 noflags;                                  //<168,23>
	ar0 = [gr0];                                  //<168,23>
	[_errno] = ar0;                                  //<168,23>

//169:      if ( backBuffer[0] != -1 ){

	gr0 = 1 set;                                  //<169,28>
	gr1 = - gr0 noflags;                                  //<169,27>
	gr0 = false noflags;                                  //<169,21>
	ar1 = _backBuffer set;                                  //<169,10>
	ar0 = gr0 set;                                  //<169,21>
	gr0 = ar1 set;                                  //<169,21>
	gr2 = ar0 set;                                  //<169,21>
	gr0 = gr0 + gr2 noflags;                                  //<169,21>
	gr0 = [gr0];                                  //<169,21>
	gr0 - gr1;                                  //<169,24>
	if <>0 goto L45;                                  //<169,24>
	goto L46;                                  //<169,24>
<L45>

//170:          *pos= backBuffer[0];

	ar0 = [ar6+-6];                                  //<170,26>
	gr0 = false noflags;                                  //<170,26>
	ar2 = _backBuffer set;                                  //<170,26>
	ar1 = gr0 set;                                  //<170,26>
	gr0 = ar2 set;                                  //<170,26>
	gr1 = ar1 set;                                  //<170,26>
	gr0 = gr0 + gr1 noflags;                                  //<170,26>
	ar1 = [gr0];                                  //<170,26>
	[ar0] = ar1;                                  //<170,26>

//171:          return -1;

	gr0 = 1 set;                                  //<171,17>
	gr0 = - gr0 noflags;                                  //<171,16>
	gr7 = gr0 set;                                  //<171,9>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<171,9>
<L46>

//172:      }
//173:      return 0;

	gr0 = false noflags;                                  //<173,12>
	gr7 = gr0 set;                                  //<173,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<173,5>

	//end of function

end ".text";

begin ".text"

global __setpos : label;
<__setpos>
<L47>

//174:  }
//175:  
//176:  int _setpos(_HANDLE file, const fpos_t* pos){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<176,1>

//177:      int* buf= (int*)beginMessage();

	ar0 = ar7 set;                                  //<177,21>
	call _int._.0.8.8beginMessage.1.2;                                  //<177,21>
	ar0 = ar5 set;                                  //<177,21>
	[ar6] = ar0;                                  //<177,21>

//178:      *buf++= file;

	ar0 = [ar6];                                  //<178,13>
	ar1 = 1 set;                                  //<178,13>
	gr0 = ar0 set;                                  //<178,13>
	gr1 = ar1 set;                                  //<178,13>
	gr0 = gr0 + gr1 noflags;                                  //<178,13>
	ar1 = gr0 set;                                  //<178,13>
	[ar6] = ar1;                                  //<178,13>
	ar1 = [ar6+-5];                                  //<178,13>
	[ar0] = ar1;                                  //<178,13>

//179:      *buf++= *pos;

	ar0 = [ar6];                                  //<179,14>
	ar1 = 1 set;                                  //<179,14>
	gr0 = ar0 set;                                  //<179,14>
	gr1 = ar1 set;                                  //<179,14>
	gr0 = gr0 + gr1 noflags;                                  //<179,14>
	ar1 = gr0 set;                                  //<179,14>
	[ar6] = ar1;                                  //<179,14>
	ar1 = [ar6+-6];                                  //<179,14>
	ar1 = [ar1];                                  //<179,14>
	[ar0] = ar1;                                  //<179,14>

//180:  	completeMessage( INDEX_SETPOS, 2 );

	ar1 = 9 set;                                  //<180,33>
	ar2 = 2 set;                                  //<180,33>
	ar0 = ar7 set;                                  //<180,33>
	ar7 = ar7 + 2;                                  //<180,33>
	[ar0++] = ar2;                                  //<180,33>
	ar0 = ar0 set;                                  //<180,33>
	[ar0++] = ar1;                                  //<180,33>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<180,33>
	ar7 = ar7 - 2;                                  //<180,33>

//181:      wait_for_host();

	ar0 = ar7 set;                                  //<181,5>
	call _wait_for_host;                                  //<181,5>

//182:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<182,32>
	gr0 = [_backBufferReady];                                  //<182,32>
	gr0 - gr1;                                  //<182,32>
	if =0 goto L50;                                  //<182,32>
	goto L49;                                  //<182,32>
<L49>
	ar0 = ar7 set;                                  //<182,32>
	call ___assertfail;                                  //<182,32>
	goto L50;                                  //<182,32>
<L50>
<L48>

//183:  	backBufferReady=0;

	gr0 = false noflags;                                  //<183,18>
	[_backBufferReady] = gr0;                                  //<183,18>

//184:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<184,23>
	ar1 = _backBuffer set;                                  //<184,23>
	ar0 = ar0 set;                                  //<184,23>
	gr0 = ar1 set;                                  //<184,23>
	gr1 = ar0 set;                                  //<184,23>
	gr0 = gr0 + gr1 noflags;                                  //<184,23>
	ar0 = [gr0];                                  //<184,23>
	[_errno] = ar0;                                  //<184,23>

//185:      return backBuffer[0]!=-1 ? -1:0;

	gr0 = 1 set;                                  //<185,28>
	gr1 = - gr0 noflags;                                  //<185,27>
	gr0 = false noflags;                                  //<185,23>
	ar1 = _backBuffer set;                                  //<185,12>
	ar0 = gr0 set;                                  //<185,23>
	gr0 = ar1 set;                                  //<185,23>
	gr2 = ar0 set;                                  //<185,23>
	gr0 = gr0 + gr2 noflags;                                  //<185,23>
	gr0 = [gr0];                                  //<185,23>
	gr0 - gr1;                                  //<185,25>
	if <>0 goto L51;                                  //<185,25>
	goto L52;                                  //<185,25>
<L51>
	gr0 = 1 set;                                  //<185,33>
	gr0 = - gr0 noflags;                                  //<185,32>
	ar0 = gr0 set;                                  //<185,37>
	[ar6 + 1] = ar0;                                  //<185,37>
	goto L53;                                  //<185,37>
<L52>
	gr0 = false noflags;                                  //<185,35>
	ar0 = gr0 set;                                  //<185,37>
	[ar6 + 1] = ar0;                                  //<185,37>
	goto L53;                                  //<185,37>
<L53>
	ar0 = [ar6+1];                                  //<185,5>
	gr7 = ar0 set;                                  //<185,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<185,5>

	//end of function

end ".text";

begin ".text"

global __rename : label;
<__rename>
<L54>

//186:  }
//187:  
//188:  int _rename(const char* oldName, const char* newName){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 4;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<188,1>

//189:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<189,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<189,23>
	ar0 = ar5 set;                                  //<189,23>
	[ar6] = ar0;                                  //<189,23>

//190:      int len1= strlen( oldName );

	ar1 = [ar6+-5];                                  //<190,23>
	ar0 = ar7 set;                                  //<190,21>
	ar7 = ar7 + 2;                                  //<190,21>
	[ar0++] = ar0;                                  //<190,21>
	ar0 = ar0 set;                                  //<190,21>
	[ar0++] = ar1;                                  //<190,21>
	call _strlen;                                  //<190,21>
	ar7 = ar7 - 2;                                  //<190,21>
	ar0 = gr7 set;                                  //<190,21>
	ar0 = ar0 set;                                  //<190,21>
	[ar6 + 1] = ar0;                                  //<190,21>

//191:      int len2= strlen( newName );

	ar1 = [ar6+-6];                                  //<191,23>
	ar0 = ar7 set;                                  //<191,21>
	ar7 = ar7 + 2;                                  //<191,21>
	[ar0++] = ar0;                                  //<191,21>
	ar0 = ar0 set;                                  //<191,21>
	[ar0++] = ar1;                                  //<191,21>
	call _strlen;                                  //<191,21>
	ar7 = ar7 - 2;                                  //<191,21>
	ar0 = gr7 set;                                  //<191,21>
	ar0 = ar0 set;                                  //<191,21>
	[ar6 + 2] = ar0;                                  //<191,21>

//192:      strcpy( buf, oldName );

	ar1 = [ar6];                                  //<192,25>
	ar2 = [ar6+-5];                                  //<192,25>
	ar0 = ar7 set;                                  //<192,25>
	ar7 = ar7 + 2;                                  //<192,25>
	[ar0++] = ar2;                                  //<192,25>
	ar0 = ar0 set;                                  //<192,25>
	[ar0++] = ar1;                                  //<192,25>
	call _strcpy;                                  //<192,25>
	ar7 = ar7 - 2;                                  //<192,25>
	ar0 = ar5 set;                                  //<192,25>

//193:      strcpy( buf+len1+1, newName );

	ar0 = 1 set;                                  //<193,32>
	ar1 = [ar6+1];                                  //<193,32>
	ar2 = [ar6];                                  //<193,32>
	ar1 = ar1 set;                                  //<193,32>
	gr0 = ar2 set;                                  //<193,32>
	gr1 = ar1 set;                                  //<193,32>
	gr0 = gr0 + gr1 noflags;                                  //<193,32>
	ar1 = gr0 set;                                  //<193,32>
	ar0 = ar0 set;                                  //<193,32>
	gr0 = ar1 set;                                  //<193,32>
	gr1 = ar0 set;                                  //<193,32>
	gr0 = gr0 + gr1 noflags;                                  //<193,32>
	ar1 = gr0 set;                                  //<193,32>
	ar2 = [ar6+-6];                                  //<193,32>
	ar0 = ar7 set;                                  //<193,32>
	ar7 = ar7 + 2;                                  //<193,32>
	[ar0++] = ar2;                                  //<193,32>
	ar0 = ar0 set;                                  //<193,32>
	[ar0++] = ar1;                                  //<193,32>
	call _strcpy;                                  //<193,32>
	ar7 = ar7 - 2;                                  //<193,32>
	ar0 = ar5 set;                                  //<193,32>

//194:  	completeMessage( INDEX_RENAME, len1+len2+2 );

	ar1 = 10 set;                                  //<194,43>
	gr1 = 2 set;                                  //<194,43>
	gr2 = [ar6+2];                                  //<194,43>
	gr0 = [ar6+1];                                  //<194,43>
	gr0 = gr0 + gr2 noflags;                                  //<194,43>
	gr0 = gr0 + gr1 noflags;                                  //<194,43>
	ar0 = ar7 set;                                  //<194,43>
	ar7 = ar7 + 2;                                  //<194,43>
	[ar0++] = gr0;                                  //<194,43>
	ar0 = ar0 set;                                  //<194,43>
	[ar0++] = ar1;                                  //<194,43>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<194,43>
	ar7 = ar7 - 2;                                  //<194,43>

//195:      wait_for_host();

	ar0 = ar7 set;                                  //<195,5>
	call _wait_for_host;                                  //<195,5>

//196:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<196,32>
	gr0 = [_backBufferReady];                                  //<196,32>
	gr0 - gr1;                                  //<196,32>
	if =0 goto L57;                                  //<196,32>
	goto L56;                                  //<196,32>
<L56>
	ar0 = ar7 set;                                  //<196,32>
	call ___assertfail;                                  //<196,32>
	goto L57;                                  //<196,32>
<L57>
<L55>

//197:  	backBufferReady=0;

	gr0 = false noflags;                                  //<197,18>
	[_backBufferReady] = gr0;                                  //<197,18>

//198:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<198,23>
	ar1 = _backBuffer set;                                  //<198,23>
	ar0 = ar0 set;                                  //<198,23>
	gr0 = ar1 set;                                  //<198,23>
	gr1 = ar0 set;                                  //<198,23>
	gr0 = gr0 + gr1 noflags;                                  //<198,23>
	ar0 = [gr0];                                  //<198,23>
	[_errno] = ar0;                                  //<198,23>

//199:      return backBuffer[0];

	gr0 = false noflags;                                  //<199,23>
	ar1 = _backBuffer set;                                  //<199,12>
	ar0 = gr0 set;                                  //<199,23>
	gr0 = ar1 set;                                  //<199,23>
	gr1 = ar0 set;                                  //<199,23>
	gr0 = gr0 + gr1 noflags;                                  //<199,23>
	ar0 = [gr0];                                  //<199,23>
	gr7 = ar0 set;                                  //<199,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 4;
	pop ar6,gr6;
return;                                  //<199,5>

	//end of function

end ".text";

begin ".text"

global __remove : label;
<__remove>
<L58>

//200:  }
//201:  
//202:  int _remove(const char* path){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<202,1>

//203:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<203,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<203,23>
	ar0 = ar5 set;                                  //<203,23>
	[ar6] = ar0;                                  //<203,23>

//204:      int len= strlen( path );

	ar1 = [ar6+-5];                                  //<204,22>
	ar0 = ar7 set;                                  //<204,20>
	ar7 = ar7 + 2;                                  //<204,20>
	[ar0++] = ar0;                                  //<204,20>
	ar0 = ar0 set;                                  //<204,20>
	[ar0++] = ar1;                                  //<204,20>
	call _strlen;                                  //<204,20>
	ar7 = ar7 - 2;                                  //<204,20>
	ar0 = gr7 set;                                  //<204,20>
	ar0 = ar0 set;                                  //<204,20>
	[ar6 + 1] = ar0;                                  //<204,20>

//205:      strcpy( buf, path );

	ar1 = [ar6];                                  //<205,22>
	ar2 = [ar6+-5];                                  //<205,22>
	ar0 = ar7 set;                                  //<205,22>
	ar7 = ar7 + 2;                                  //<205,22>
	[ar0++] = ar2;                                  //<205,22>
	ar0 = ar0 set;                                  //<205,22>
	[ar0++] = ar1;                                  //<205,22>
	call _strcpy;                                  //<205,22>
	ar7 = ar7 - 2;                                  //<205,22>
	ar0 = ar5 set;                                  //<205,22>

//206:  	completeMessage( INDEX_REMOVE, len+1 );

	ar1 = 11 set;                                  //<206,37>
	gr1 = 1 set;                                  //<206,37>
	gr0 = [ar6+1];                                  //<206,37>
	gr0 = gr0 + gr1 noflags;                                  //<206,37>
	ar0 = ar7 set;                                  //<206,37>
	ar7 = ar7 + 2;                                  //<206,37>
	[ar0++] = gr0;                                  //<206,37>
	ar0 = ar0 set;                                  //<206,37>
	[ar0++] = ar1;                                  //<206,37>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<206,37>
	ar7 = ar7 - 2;                                  //<206,37>

//207:      wait_for_host();

	ar0 = ar7 set;                                  //<207,5>
	call _wait_for_host;                                  //<207,5>

//208:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<208,32>
	gr0 = [_backBufferReady];                                  //<208,32>
	gr0 - gr1;                                  //<208,32>
	if =0 goto L61;                                  //<208,32>
	goto L60;                                  //<208,32>
<L60>
	ar0 = ar7 set;                                  //<208,32>
	call ___assertfail;                                  //<208,32>
	goto L61;                                  //<208,32>
<L61>
<L59>

//209:  	backBufferReady=0;

	gr0 = false noflags;                                  //<209,18>
	[_backBufferReady] = gr0;                                  //<209,18>

//210:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<210,23>
	ar1 = _backBuffer set;                                  //<210,23>
	ar0 = ar0 set;                                  //<210,23>
	gr0 = ar1 set;                                  //<210,23>
	gr1 = ar0 set;                                  //<210,23>
	gr0 = gr0 + gr1 noflags;                                  //<210,23>
	ar0 = [gr0];                                  //<210,23>
	[_errno] = ar0;                                  //<210,23>

//211:      return backBuffer[0];

	gr0 = false noflags;                                  //<211,23>
	ar1 = _backBuffer set;                                  //<211,12>
	ar0 = gr0 set;                                  //<211,23>
	gr0 = ar1 set;                                  //<211,23>
	gr1 = ar0 set;                                  //<211,23>
	gr0 = gr0 + gr1 noflags;                                  //<211,23>
	ar0 = [gr0];                                  //<211,23>
	gr7 = ar0 set;                                  //<211,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<211,5>

	//end of function

end ".text";

begin ".text"

global __getenv : label;
<__getenv>
<L62>

//212:  }
//213:  
//214:  size_t _getenv(const char *name, char *p_buf, size_t bufsize){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 4;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
                                  //<214,1>

//215:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<215,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<215,23>
	ar0 = ar5 set;                                  //<215,23>
	[ar6] = ar0;                                  //<215,23>

//216:      int len= strlen( name );

	ar1 = [ar6+-5];                                  //<216,22>
	ar0 = ar7 set;                                  //<216,20>
	ar7 = ar7 + 2;                                  //<216,20>
	[ar0++] = ar0;                                  //<216,20>
	ar0 = ar0 set;                                  //<216,20>
	[ar0++] = ar1;                                  //<216,20>
	call _strlen;                                  //<216,20>
	ar7 = ar7 - 2;                                  //<216,20>
	ar0 = gr7 set;                                  //<216,20>
	ar0 = ar0 set;                                  //<216,20>
	[ar6 + 1] = ar0;                                  //<216,20>

//217:      strcpy( buf, name );

	ar1 = [ar6];                                  //<217,22>
	ar2 = [ar6+-5];                                  //<217,22>
	ar0 = ar7 set;                                  //<217,22>
	ar7 = ar7 + 2;                                  //<217,22>
	[ar0++] = ar2;                                  //<217,22>
	ar0 = ar0 set;                                  //<217,22>
	[ar0++] = ar1;                                  //<217,22>
	call _strcpy;                                  //<217,22>
	ar7 = ar7 - 2;                                  //<217,22>
	ar0 = ar5 set;                                  //<217,22>

//218:  	completeMessage( INDEX_GETENV, len+1 );

	ar1 = 14 set;                                  //<218,37>
	gr1 = 1 set;                                  //<218,37>
	gr0 = [ar6+1];                                  //<218,37>
	gr0 = gr0 + gr1 noflags;                                  //<218,37>
	ar0 = ar7 set;                                  //<218,37>
	ar7 = ar7 + 2;                                  //<218,37>
	[ar0++] = gr0;                                  //<218,37>
	ar0 = ar0 set;                                  //<218,37>
	[ar0++] = ar1;                                  //<218,37>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<218,37>
	ar7 = ar7 - 2;                                  //<218,37>

//219:      wait_for_host();

	ar0 = ar7 set;                                  //<219,5>
	call _wait_for_host;                                  //<219,5>

//220:      int i;
//221:      for ( i=0; i<bufsize; i++ ){

	gr0 = false noflags;                                  //<221,13>
	[ar6 + 2] = gr0;                                  //<221,13>
	goto L63;                                  //<221,5>
<L68>
	gr0 = [ar6+2];                                  //<221,28>
	gr1 = 1 set;                                  //<221,28>
	gr0 = gr0 + gr1 noflags;                                  //<221,28>
	[ar6 + 2] = gr0;                                  //<221,28>
	goto L63;                                  //<221,5>
<L63>
	gr1 = [ar6+-7];                                  //<221,18>
	ar0 = [ar6+2];                                  //<221,16>
	gr0 = ar0 set;                                  //<221,16>
	gr0 - gr1;                                  //<221,17>
	if u< goto L64;                                  //<221,17>
	goto L65;                                  //<221,17>
<L64>

//222:          p_buf[i]= backBuffer[i+1];

	ar0 = [ar6+2];                                  //<222,32>
	ar1 = [ar6+-6];                                  //<222,32>
	ar0 = ar0 set;                                  //<222,32>
	gr1 = 1 set;                                  //<222,32>
	gr0 = [ar6+2];                                  //<222,32>
	gr0 = gr0 + gr1 noflags;                                  //<222,32>
	ar3 = _backBuffer set;                                  //<222,32>
	ar2 = gr0 set;                                  //<222,32>
	gr0 = ar3 set;                                  //<222,32>
	gr1 = ar2 set;                                  //<222,32>
	gr0 = gr0 + gr1 noflags;                                  //<222,32>
	ar2 = [gr0];                                  //<222,32>
	gr0 = ar2 set;                                  //<222,32>
	gr0 = gr0 << 24;                                  //<222,32>
	gr1 = gr0 A>> 24;                                  //<222,32>
	gr0 = ar1 set;                                  //<222,32>
	gr2 = ar0 set;                                  //<222,32>
	gr0 = gr0 + gr2 noflags;                                  //<222,32>
	[gr0] = gr1;                                  //<222,32>

//223:          if ( p_buf[i]==0 )

	gr1 = false noflags;                                  //<223,24>
	ar0 = [ar6+2];                                  //<223,20>
	ar1 = [ar6+-6];                                  //<223,14>
	ar0 = ar0 set;                                  //<223,21>
	gr0 = ar1 set;                                  //<223,21>
	gr2 = ar0 set;                                  //<223,21>
	gr0 = gr0 + gr2 noflags;                                  //<223,21>
	ar0 = [gr0];                                  //<223,21>
	gr0 = ar0 set;                                  //<223,21>
	gr0 - gr1;                                  //<223,22>
	if =0 goto L65;                                  //<223,22>
	goto L68;                                  //<223,22>
<L65>
<L67>

//224:              break;
//225:      }
//226:  	backBufferReady=0;

	gr0 = false noflags;                                  //<226,18>
	[_backBufferReady] = gr0;                                  //<226,18>

//227:      if ( i==bufsize ){

	gr1 = [ar6+-7];                                  //<227,13>
	ar0 = [ar6+2];                                  //<227,10>
	gr0 = ar0 set;                                  //<227,10>
	gr0 - gr1;                                  //<227,11>
	if =0 goto L69;                                  //<227,11>
	goto L70;                                  //<227,11>
<L69>

//228:          errno= 34;//ERANGE;

	ar0 = 34 set;                                  //<228,16>
	[_errno] = ar0;                                  //<228,16>

//229:      }

	goto L71;                                  //<229,5>
<L70>

//230:      else
//231:          errno= backBuffer[0];

	gr0 = false noflags;                                  //<231,27>
	ar1 = _backBuffer set;                                  //<231,27>
	ar0 = gr0 set;                                  //<231,27>
	gr0 = ar1 set;                                  //<231,27>
	gr1 = ar0 set;                                  //<231,27>
	gr0 = gr0 + gr1 noflags;                                  //<231,27>
	ar0 = [gr0];                                  //<231,27>
	[_errno] = ar0;                                  //<231,27>
	goto L71;                                  //<231,30>
<L71>

//232:      return i;

	ar0 = [ar6+2];                                  //<232,12>
	ar0 = ar0 set;                                  //<232,12>
	gr7 = ar0 set;                                  //<232,5>
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 4;
	pop ar6,gr6;
return;                                  //<232,5>

	//end of function

end ".text";

begin ".text"

global _system : label;
<_system>
<L72>

//233:  }
//234:  
//235:  int system(const char *command){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<235,1>

//236:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<236,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<236,23>
	ar0 = ar5 set;                                  //<236,23>
	[ar6] = ar0;                                  //<236,23>

//237:      int len= strlen( command );

	ar1 = [ar6+-5];                                  //<237,22>
	ar0 = ar7 set;                                  //<237,20>
	ar7 = ar7 + 2;                                  //<237,20>
	[ar0++] = ar0;                                  //<237,20>
	ar0 = ar0 set;                                  //<237,20>
	[ar0++] = ar1;                                  //<237,20>
	call _strlen;                                  //<237,20>
	ar7 = ar7 - 2;                                  //<237,20>
	ar0 = gr7 set;                                  //<237,20>
	ar0 = ar0 set;                                  //<237,20>
	[ar6 + 1] = ar0;                                  //<237,20>

//238:      strcpy( buf, command );

	ar1 = [ar6];                                  //<238,25>
	ar2 = [ar6+-5];                                  //<238,25>
	ar0 = ar7 set;                                  //<238,25>
	ar7 = ar7 + 2;                                  //<238,25>
	[ar0++] = ar2;                                  //<238,25>
	ar0 = ar0 set;                                  //<238,25>
	[ar0++] = ar1;                                  //<238,25>
	call _strcpy;                                  //<238,25>
	ar7 = ar7 - 2;                                  //<238,25>
	ar0 = ar5 set;                                  //<238,25>

//239:  	completeMessage( INDEX_SYSTEM, len+1 );

	ar1 = 15 set;                                  //<239,37>
	gr1 = 1 set;                                  //<239,37>
	gr0 = [ar6+1];                                  //<239,37>
	gr0 = gr0 + gr1 noflags;                                  //<239,37>
	ar0 = ar7 set;                                  //<239,37>
	ar7 = ar7 + 2;                                  //<239,37>
	[ar0++] = gr0;                                  //<239,37>
	ar0 = ar0 set;                                  //<239,37>
	[ar0++] = ar1;                                  //<239,37>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<239,37>
	ar7 = ar7 - 2;                                  //<239,37>

//240:      wait_for_host();

	ar0 = ar7 set;                                  //<240,5>
	call _wait_for_host;                                  //<240,5>

//241:      assert(backBufferReady==2);

	gr1 = 2 set;                                  //<241,32>
	gr0 = [_backBufferReady];                                  //<241,32>
	gr0 - gr1;                                  //<241,32>
	if =0 goto L75;                                  //<241,32>
	goto L74;                                  //<241,32>
<L74>
	ar0 = ar7 set;                                  //<241,32>
	call ___assertfail;                                  //<241,32>
	goto L75;                                  //<241,32>
<L75>
<L73>

//242:  	backBufferReady=0;

	gr0 = false noflags;                                  //<242,18>
	[_backBufferReady] = gr0;                                  //<242,18>

//243:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<243,23>
	ar1 = _backBuffer set;                                  //<243,23>
	ar0 = ar0 set;                                  //<243,23>
	gr0 = ar1 set;                                  //<243,23>
	gr1 = ar0 set;                                  //<243,23>
	gr0 = gr0 + gr1 noflags;                                  //<243,23>
	ar0 = [gr0];                                  //<243,23>
	[_errno] = ar0;                                  //<243,23>

//244:      return backBuffer[0];

	gr0 = false noflags;                                  //<244,23>
	ar1 = _backBuffer set;                                  //<244,12>
	ar0 = gr0 set;                                  //<244,23>
	gr0 = ar1 set;                                  //<244,23>
	gr1 = ar0 set;                                  //<244,23>
	gr0 = gr0 + gr1 noflags;                                  //<244,23>
	ar0 = [gr0];                                  //<244,23>
	gr7 = ar0 set;                                  //<244,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<244,5>

	//end of function

end ".text";

begin ".text"

global __stat64i32 : label;
<__stat64i32>
<L76>

//245:  }
//246:  //int printf(const char *format, ...);
//247:  
//248:  int _fstat(_HANDLE handle, struct _stat *buffer){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
                                  //<248,1>

//249:  
//250:  #ifdef _USE_32BIT_TIME_T
//251:  #error _USE_32BIT_TIME_T is unsupported
//252:  #endif
//253:  
//254:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<254,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<254,23>
	ar0 = ar5 set;                                  //<254,23>
	[ar6] = ar0;                                  //<254,23>

//255:  	*buf++ =handle;

	ar0 = [ar6];                                  //<255,10>
	ar1 = 1 set;                                  //<255,10>
	gr0 = ar0 set;                                  //<255,10>
	gr1 = ar1 set;                                  //<255,10>
	gr0 = gr0 + gr1 noflags;                                  //<255,10>
	ar1 = gr0 set;                                  //<255,10>
	[ar6] = ar1;                                  //<255,10>
	ar1 = [ar6+-5];                                  //<255,10>
	gr0 = ar1 set;                                  //<255,10>
	gr0 = gr0 << 24;                                  //<255,10>
	gr0 = gr0 A>> 24;                                  //<255,10>
	[ar0] = gr0;                                  //<255,10>

//256:  	completeMessage( INDEX_FSTAT, 1 );

	ar1 = 18 set;                                  //<256,32>
	ar2 = 1 set;                                  //<256,32>
	ar0 = ar7 set;                                  //<256,32>
	ar7 = ar7 + 2;                                  //<256,32>
	[ar0++] = ar2;                                  //<256,32>
	ar0 = ar0 set;                                  //<256,32>
	[ar0++] = ar1;                                  //<256,32>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<256,32>
	ar7 = ar7 - 2;                                  //<256,32>

//257:      wait_for_host();

	ar0 = ar7 set;                                  //<257,5>
	call _wait_for_host;                                  //<257,5>

//258:      assert(backBufferReady== sizeof(struct _stat)+2 );

	ar0 = 2 set;                                  //<258,55>
	gr1 = ar0 set;                                  //<258,55>
	gr0 = 14 set;                                  //<258,55>
	gr1 = gr0 + gr1 noflags;                                  //<258,55>
	ar0 = [_backBufferReady];                                  //<258,55>
	gr0 = ar0 set;                                  //<258,55>
	gr0 - gr1;                                  //<258,55>
	if =0 goto L79;                                  //<258,55>
	goto L78;                                  //<258,55>
<L78>
	ar0 = ar7 set;                                  //<258,55>
	call ___assertfail;                                  //<258,55>
	goto L79;                                  //<258,55>
<L79>
<L77>

//259:  	backBufferReady=0;

	gr0 = false noflags;                                  //<259,18>
	[_backBufferReady] = gr0;                                  //<259,18>

//260:      errno= backBuffer[1];

	ar0 = 1 set;                                  //<260,23>
	ar1 = _backBuffer set;                                  //<260,23>
	ar0 = ar0 set;                                  //<260,23>
	gr0 = ar1 set;                                  //<260,23>
	gr1 = ar0 set;                                  //<260,23>
	gr0 = gr0 + gr1 noflags;                                  //<260,23>
	ar0 = [gr0];                                  //<260,23>
	[_errno] = ar0;                                  //<260,23>

//261:      if ( backBuffer[0]==0 ){

	gr1 = false noflags;                                  //<261,25>
	gr0 = false noflags;                                  //<261,21>
	ar1 = _backBuffer set;                                  //<261,10>
	ar0 = gr0 set;                                  //<261,21>
	gr0 = ar1 set;                                  //<261,21>
	gr2 = ar0 set;                                  //<261,21>
	gr0 = gr0 + gr2 noflags;                                  //<261,21>
	gr0 = [gr0];                                  //<261,21>
	gr0 - gr1;                                  //<261,23>
	if =0 goto L80;                                  //<261,23>
	goto L81;                                  //<261,23>
<L80>

//262:          memcpy( buffer, (int const*)backBuffer+2, sizeof(struct _stat) );

	ar1 = [ar6+-6];                                  //<262,70>
	ar0 = 2 set;                                  //<262,70>
	ar2 = _backBuffer set;                                  //<262,70>
	ar0 = ar0 set;                                  //<262,70>
	gr0 = ar2 set;                                  //<262,70>
	gr1 = ar0 set;                                  //<262,70>
	gr0 = gr0 + gr1 noflags;                                  //<262,70>
	ar2 = gr0 set;                                  //<262,70>
	ar3 = 14 set;                                  //<262,70>
	ar0 = ar7 set;                                  //<262,70>
	ar7 = ar7 + 4;                                  //<262,70>
	[ar0++] = ar0;                                  //<262,70>
	[ar0++] = ar3;                                  //<262,70>
	[ar0++] = ar2;                                  //<262,70>
	ar0 = ar0 set;                                  //<262,70>
	[ar0++] = ar1;                                  //<262,70>
	call _memcpy;                                  //<262,70>
	ar7 = ar7 - 4;                                  //<262,70>
	ar0 = ar5 set;                                  //<262,70>

//263:      }

	goto L81;                                  //<263,5>
<L81>

//264:      return backBuffer[0];

	gr0 = false noflags;                                  //<264,23>
	ar1 = _backBuffer set;                                  //<264,12>
	ar0 = gr0 set;                                  //<264,23>
	gr0 = ar1 set;                                  //<264,23>
	gr1 = ar0 set;                                  //<264,23>
	gr0 = gr0 + gr1 noflags;                                  //<264,23>
	ar0 = [gr0];                                  //<264,23>
	gr7 = ar0 set;                                  //<264,5>
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<264,5>

	//end of function

end ".text";

begin ".text"

global __sys_info : label;
<__sys_info>
<L82>

//265:  }
//266:  
//267:  //extern "C" int putchar( int c );
//268:  //
//269:  //int _fstatX(_HANDLE handle, void *buffer){
//270:  //
//271:  //#ifdef _USE_32BIT_TIME_T
//272:  //#error _USE_32BIT_TIME_T is unsupported
//273:  //#endif
//274:  //
//275:  ////    printf( "=x%d=", handle );
//276:  //    putchar( '=' );
//277:  //    putchar( '>' );
//278:  //    putchar( '0'+(handle/100000)%10 );
//279:  //    putchar( '0'+(handle/10000)%10 );
//280:  //    putchar( '0'+(handle/1000)%10 );
//281:  //    putchar( '0'+(handle/100)%10 );
//282:  //    putchar( '0'+(handle/10)%10 );
//283:  //    putchar( '0'+(handle/1)%10 );
//284:  //    putchar( '=' );
//285:  //    char* buf= (char*)beginMessage();
//286:  //	*buf++ =handle;
//287:  //	completeMessage( INDEX_FSTAT, 1 );
//288:  //    wait_for_host();
//289:  //    assert(backBufferReady== sizeof(struct _stat)+2 );
//290:  //	backBufferReady=0;
//291:  //    errno= backBuffer[1];
//292:  //    if ( backBuffer[0]==0 ){
//293:  //        memcpy( buffer, (int const*)backBuffer+2, sizeof(struct _stat) );
//294:  //    }
//295:  //    return backBuffer[0];
//296:  //}
//297:  
//298:  /* Function to get system information*/
//299:  size_t _sys_info (int /*operation*/, void* buf, size_t bufsize)

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<299,1>

//300:  {
//301:      int i;
//302:      for ( i=0; i<bufsize; i++ ){

	gr0 = false noflags;                                  //<302,13>
	[ar6] = gr0;                                  //<302,13>
	goto L83;                                  //<302,5>
<L84>

//303:          ((int*)buf)[i]=42;

	ar0 = [ar6];                                  //<303,24>
	ar2 = [ar6+-6];                                  //<303,24>
	ar1 = ar0 set;                                  //<303,24>
	ar0 = 42 set;                                  //<303,24>
	gr0 = ar2 set;                                  //<303,24>
	gr1 = ar1 set;                                  //<303,24>
	gr0 = gr0 + gr1 noflags;                                  //<303,24>
	[gr0] = ar0;                                  //<303,24>
	gr0 = [ar6];                                  //<302,28>
	gr1 = 1 set;                                  //<302,28>
	gr0 = gr0 + gr1 noflags;                                  //<302,28>
	[ar6] = gr0;                                  //<302,28>
	goto L83;                                  //<302,5>
<L83>
	gr1 = [ar6+-7];                                  //<302,18>
	ar0 = [ar6];                                  //<302,16>
	gr0 = ar0 set;                                  //<302,16>
	gr0 - gr1;                                  //<302,17>
	if u< goto L84;                                  //<302,17>
	goto L85;                                  //<302,17>
<L85>

//304:      }
//305:      return i;

	ar0 = [ar6];                                  //<305,12>
	ar0 = ar0 set;                                  //<305,12>
	gr7 = ar0 set;                                  //<305,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<305,5>

	//end of function

end ".text";

begin ".text"

global __getf_size : label;
<__getf_size>
<L87>

//306:  //    return _env_call_func(INDEX_SYS_INFO, operation, buf, bufsize);
//307:  }
//308:  
//309:  
//310:  fpos_t _getf_size(_HANDLE file){

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 14;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<310,1>

//311:      struct _stat buffer;
//312:      _fstat( file, &buffer );

	ar1 = [ar6+-5];                                  //<312,20>
	ar0 = 0 set;                                  //<312,20>
	gr0 = ar0 set;                                  //<312,20>
	gr1 = ar6 set;                                  //<312,20>
	gr0 = gr0 + gr1 noflags;                                  //<312,20>
	ar2 = gr0 set;                                  //<312,20>
	ar0 = ar7 set;                                  //<312,20>
	ar7 = ar7 + 2;                                  //<312,20>
	[ar0++] = ar2;                                  //<312,20>
	ar0 = ar0 set;                                  //<312,20>
	[ar0++] = ar1;                                  //<312,20>
	call __stat64i32;                                  //<312,20>
	ar7 = ar7 - 2;                                  //<312,20>
	ar0 = gr7 set;                                  //<312,20>

//313:      return buffer.st_size;

	ar1 = 7 set;                                  //<313,19>
	ar0 = 0 set;                                  //<313,19>
	gr0 = ar1 set;                                  //<313,19>
	gr1 = ar0 set;                                  //<313,19>
	gr0 = gr0 + gr1 noflags;                                  //<313,19>
	ar0 = gr0 set;                                  //<313,19>
	gr0 = ar6 set;                                  //<313,19>
	gr1 = ar0 set;                                  //<313,19>
	gr0 = gr0 + gr1 noflags;                                  //<313,19>
	ar0 = [gr0];                                  //<313,19>
	gr7 = ar0 set;                                  //<313,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 14;
	pop ar6,gr6;
return;                                  //<313,5>

	//end of function

end ".text";

begin ".text"

end ".text";
