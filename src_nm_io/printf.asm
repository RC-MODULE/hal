
	// External references.
extern _vsprintf : label;
extern __allocateThrownObject : label;
extern __throwObject : label;

	// Function declaration.
local _bool._.8.8fullBuffer.1.2 : label;
local _void._.8.8makeMsgHeader.1int._.0.9._enum._NM_IO_ServiceID.9._int.2 : label;
global _void._.8.8dummy.1.2 : label;
global _int._.0.8.8beginMessage.1.2 : label;
global _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2 : label;
global _printf : label;

	// Data declaration.
extern _boardOutServBuf : word[131072];
extern _backBuffer : word[32768];
extern _boardSend : word[1];
extern _sended : word[1];
extern _sendMostDistant : word[1];
extern _nm_io_debug : word[1];
extern _hostReceive : word[1];
extern _sendConfirmed : word[1];
extern _backBufferReady : word[1];
extern _nm_io_debug_back : word[1];
extern _bool._.8.8incomplete : word[1];
extern __str__727063206572726f723a20696e636f6d706c6574652070726576696f7573206d65737361676500 : word[39];
extern ___char._const._.0__catch_info : word[2];
extern ___char._const.___catch_info : word[2];
extern ___char__catch_info : word[1];
extern __str__727063206572726f723a2063616e277420636f6d706c6574652d206d657373616765206973206e6f74207374617274656400 : word[50];
extern __str__727063206572726f723a20746f6f20626967206d65737361676500 : word[27];

nobits ".rpc_services.bss"
	global _boardOutServBuf : word[131072];
end ".rpc_services.bss";
nobits ".rpc_services.bss"
	global _backBuffer : word[32768];
end ".rpc_services.bss";
data ".rpc_services"
	global _boardSend : word[1] = (0);
end ".rpc_services";
data ".rpc_services"
	global _sended : word[1] = (_boardOutServBuf + 0);
end ".rpc_services";
data ".rpc_services"
	global _sendMostDistant : word[1] = (_boardOutServBuf + 0);
end ".rpc_services";
data ".rpc_services"
	global _nm_io_debug : word[1] = (0);
end ".rpc_services";
data ".rpc_services"
	global _hostReceive : word[1] = (0);
end ".rpc_services";
data ".rpc_services"
	global _sendConfirmed : word[1] = (_boardOutServBuf + 0);
end ".rpc_services";
data ".rpc_services"
	global _backBufferReady : word[1] = (0);
end ".rpc_services";
data ".rpc_services"
	global _nm_io_debug_back : word[1] = (0);
end ".rpc_services";
data ".data"
	global _bool._.8.8incomplete : word[1] = (0);
end ".data";
data ".data"
	weak __str__727063206572726f723a20696e636f6d706c6574652070726576696f7573206d65737361676500 : word[39] = ("rpc error: incomplete previous message\000");
end ".data";
data ".data"
	weak ___char._const._.0__catch_info : word[2] = (16, ___char._const.___catch_info + 0);
end ".data";
data ".data"
	weak ___char._const.___catch_info : word[2] = (3, ___char__catch_info + 0);
end ".data";
data ".data"
	weak ___char__catch_info : word[1] = (1);
end ".data";
data ".data"
	weak __str__727063206572726f723a2063616e277420636f6d706c6574652d206d657373616765206973206e6f74207374617274656400 : word[50] = ("rpc error: can't complete- message is not started\000");
end ".data";
data ".data"
	weak __str__727063206572726f723a20746f6f20626967206d65737361676500 : word[27] = ("rpc error: too big message\000");
end ".data";

begin ".text"

local _bool._.8.8fullBuffer.1.2 : label;
<_bool._.8.8fullBuffer.1.2>
<L1>

//--- d:\GIT\nm_io\src\nm_io_service\nm_part\printf.c

//1:    //////////////////////////////////////////////////////////////////////////
//2:    //                                                                      //
//3:    //               NeuroMatrix(r) C runtime library                       //
//4:    //                                                                      //
//5:    //  printf.c                                                            //
//6:    //  Реализация, резидентная часть.                                      //
//7:    //                                                                      //
//8:    // Copyright (c) 2004-2008 RC Module                                    //
//9:    //                                                                      //
//10:   //  Автор: Бирюков А.А.                                                 //
//11:   //                                                                      //
//12:   // $Revision:: 2    $      $Date:: 25.06.13 19:20    $                  //
//13:   //                                                                      //
//14:   //////////////////////////////////////////////////////////////////////////
//15:   
//16:   #include "host_duties.h"
//17:   #include <stddef.h>
//18:   #include <stdio.h>
//19:   #include <stdarg.h>
//20:   #include <stdlib.h>
//21:   
//22:   //#include <setjmp.h>
//23:   // 
//24:   //extern jmp_buf buf;
//25:   
//26:   extern "C"
//27:   {
//28:   	// Barrier sync with host PC with array exchange
//29:   	//int ncl_hostSyncArray(
//30:   	//	int value,        // Sync value
//31:   	//	void *outAddress, // Sended array address (can be NULL)
//32:   	//	size_t outLen,    // Sended array length (can be 0)
//33:   	//	void **inAddress, // Received array address pointer (can be NULL)
//34:   	//	size_t *inLen);   // Received array size pointer (can be NULL)
//35:   
//36:       int vsprintf( char *dst, const char* format, va_list args );
//37:   
//38:   #pragma data_section ".rpc_services"
//39:   //  Это должно лежать в разделяемой памяти с определенным смещением
//40:   //  относительно начала секций
//41:   //  модифицируются только с платы
//42:   volatile int boardOutServBuf[ b2hBufSize ]; //  .rpc_services.bss +0
//43:   //  модифицируются только с хоста
//44:   volatile int backBuffer[ backBufferSize ];  //  .rpc_services.bss +b2hBufSize
//45:   
//46:   //  модифицируются только с платы
//47:   volatile int boardSend=0;               //  пакеты  //  .rpc_services +0 
//48:   volatile int* sended= boardOutServBuf;  //  слова   //  .rpc_services +1 
//49:   volatile int* sendMostDistant=boardOutServBuf;      //  .rpc_services +2    //используется при возврате в начало буфера
//50:   volatile int nm_io_debug=0;                    // пc//  .rpc_services +3 
//51:   //  модифицируются только с хоста
//52:   volatile int hostReceive=0;                    // п //  .rpc_services +4 
//53:   volatile int* sendConfirmed=boardOutServBuf;   // с //  .rpc_services +5
//54:   //  модифицируется отовсюду
//55:   volatile int backBufferReady= 0;		       //   //  .rpc_services +6 
//56:   volatile int nm_io_debug_back=0;               // пc//  .rpc_services +7 
//57:   #pragma default_data_section 
//58:   };
//59:   
//60:   static bool fullBuffer()

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<60,1>

//61:   { 
//62:       if ( sendConfirmed>sended )

	ar1 = [_sended];                                  //<62,24>
	ar0 = [_sendConfirmed];                                  //<62,10>
	gr0 = ar1 set;                                  //<62,24>
	gr1 = ar0 set;                                  //<62,24>
	gr0 - gr1;                                  //<62,24>
	if u< goto L2;                                  //<62,24>
	goto L3;                                  //<62,24>
<L2>

//63:           return sended+ packetLimit >= sendConfirmed;

	ar0 = [_sendConfirmed];                                  //<63,39>
	ar1 = 32768 set;                                  //<63,24>
	ar2 = [_sended];                                  //<63,16>
	ar1 = ar1 set;                                  //<63,22>
	gr0 = ar2 set;                                  //<63,22>
	gr1 = ar1 set;                                  //<63,22>
	gr0 = gr0 + gr1 noflags;                                  //<63,22>
	ar1 = gr0 set;                                  //<63,22>
	gr0 = ar1 set;                                  //<63,36>
	gr1 = ar0 set;                                  //<63,36>
	gr0 - gr1;                                  //<63,36>
	if u>= goto L4;                                  //<63,36>
	goto L5;                                  //<63,36>
<L4>
	ar0 = 1 set;                                  //<63,36>
	[ar6] = ar0;                                  //<63,36>
	goto L6;                                  //<63,36>
<L5>
	gr0 = false noflags;                                  //<63,36>
	[ar6] = gr0;                                  //<63,36>
	goto L6;                                  //<63,36>
<L6>
	ar0 = [ar6];                                  //<63,9>
	gr7 = ar0 set;                                  //<63,9>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<63,9>
<L3>

//64:       else
//65:           return false;

	gr0 = false noflags;                                  //<65,16>
	gr7 = gr0 set;                                  //<65,9>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 2;
	pop ar6,gr6;
return;                                  //<65,9>

	//end of function

end ".text";

begin ".text"

local _void._.8.8makeMsgHeader.1int._.0.9._enum._NM_IO_ServiceID.9._int.2 : label;
<_void._.8.8makeMsgHeader.1int._.0.9._enum._NM_IO_ServiceID.9._int.2>
<L7>

//66:   }
//67:   
//68:   static void makeMsgHeader( int* ptr, NM_IO_ServiceID id, int size )

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
                                  //<68,1>

//69:   {
//70:       //  Формат пакета: |размер- 1 слово|тип- 1 слово|данные- <размер> слов|
//71:       *ptr++ = (int)size;

	ar0 = [ar6+-5];                                  //<71,19>
	ar1 = 1 set;                                  //<71,19>
	gr0 = ar0 set;                                  //<71,19>
	gr1 = ar1 set;                                  //<71,19>
	gr0 = gr0 + gr1 noflags;                                  //<71,19>
	ar1 = gr0 set;                                  //<71,19>
	[ar6 + -5] = ar1;                                  //<71,19>
	ar1 = [ar6+-7];                                  //<71,19>
	[ar0] = ar1;                                  //<71,19>

//72:       *ptr++ = (int)id;

	ar0 = [ar6+-5];                                  //<72,19>
	ar1 = 1 set;                                  //<72,19>
	gr0 = ar0 set;                                  //<72,19>
	gr1 = ar1 set;                                  //<72,19>
	gr0 = gr0 + gr1 noflags;                                  //<72,19>
	ar1 = gr0 set;                                  //<72,19>
	[ar6 + -5] = ar1;                                  //<72,19>
	ar1 = [ar6+-6];                                  //<72,19>
	ar1 = ar1 set;                                  //<72,19>
	[ar0] = ar1;                                  //<72,19>

//73:   }

	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<73,1>

	//end of function

end ".text";

begin ".text"

global _void._.8.8dummy.1.2 : label;
<_void._.8.8dummy.1.2>
<L8>

//74:   
//75:   void dummy(){}

	push ar6, gr6;
	ar6 = ar7;
                                  //<75,1>
	pop ar6,gr6;
return;                                  //<75,14>

	//end of function

end ".text";

begin ".text"

global _int._.0.8.8beginMessage.1.2 : label;
<_int._.0.8.8beginMessage.1.2>
<L9>

//76:   
//77:   bool incomplete= false;
//78:   int* beginMessage()

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
                                  //<78,1>

//79:   {
//80:       if ( incomplete )

	gr0 = [_bool._.8.8incomplete];                                  //<80,10>
	gr1 = false noflags;                                  //<80,10>
	gr0 - gr1;                                  //<80,10>
	if <>0 goto L10;                                  //<80,10>
	goto L11;                                  //<80,10>
<L10>

//81:           throw "rpc error: incomplete previous message";

	ar1 = __str__727063206572726f723a20696e636f6d706c6574652070726576696f7573206d65737361676500 set;                                  //<81,56>
	ar2 = 1 set;                                  //<81,56>
	ar0 = ar7 set;                                  //<81,56>
	ar7 = ar7 + 2;                                  //<81,56>
	[ar0++] = ar0;                                  //<81,56>
	ar0 = ar0 set;                                  //<81,56>
	[ar0++] = ar2;                                  //<81,56>
	call __allocateThrownObject;                                  //<81,56>
	ar7 = ar7 - 2;                                  //<81,56>
	ar0 = ar5 set;                                  //<81,56>
	[ar0] = ar1;                                  //<81,56>
	ar1 = ___char._const._.0__catch_info set;                                  //<81,56>
	ar2 = 0 set;                                  //<81,56>
	ar0 = ar7 set;                                  //<81,56>
	ar7 = ar7 + 2;                                  //<81,56>
	[ar0++] = ar2;                                  //<81,56>
	ar0 = ar0 set;                                  //<81,56>
	[ar0++] = ar1;                                  //<81,56>
	call __throwObject;                                  //<81,56>
	ar7 = ar7 - 2;                                  //<81,56>
	goto L11;                                  //<81,56>
<L11>

//82:       if ( (sended - boardOutServBuf) >= b2hBufLimit ){

	gr1 = 98304 set;                                  //<82,40>
	ar0 = _boardOutServBuf set;                                  //<82,20>
	ar1 = [_sended];                                  //<82,11>
	gr0 = ar1 set;                                  //<82,18>
	gr2 = ar0 set;                                  //<82,18>
	gr0 = gr0 - gr2 noflags;                                  //<82,18>
	ar0 = gr0 set;                                  //<82,18>
	gr0 = ar0 set;                                  //<82,18>
	gr0 - gr1;                                  //<82,37>
	if v>= goto L12;                                  //<82,37>
	goto L13;                                  //<82,37>
<L12>

//83:           //longjmp(buf,boardSend);
//84:           while ( sendConfirmed <= boardOutServBuf+packetLimit ) 

	ar0 = 32768 set;                                  //<84,50>
	ar1 = _boardOutServBuf set;                                  //<84,34>
	ar0 = ar0 set;                                  //<84,49>
	gr0 = ar1 set;                                  //<84,49>
	gr1 = ar0 set;                                  //<84,49>
	gr0 = gr0 + gr1 noflags;                                  //<84,49>
	ar1 = gr0 set;                                  //<84,49>
	ar0 = [_sendConfirmed];                                  //<84,17>
	gr0 = ar1 set;                                  //<84,31>
	gr1 = ar0 set;                                  //<84,31>
	gr0 - gr1;                                  //<84,31>
	if u>= goto L14;                                  //<84,31>
	goto L15;                                  //<84,31>
<L14>

//85:               dummy(); //  Здесь ждем при засоре 1

	ar0 = ar7 set;                                  //<85,13>
	call _void._.8.8dummy.1.2;                                  //<85,13>
	goto L12;                                  //<84,9>
<L15>

//86:           sendMostDistant= sended;

	ar0 = [_sended];                                  //<86,26>
	[_sendMostDistant] = ar0;                                  //<86,26>

//87:           sended= boardOutServBuf;

	ar0 = _boardOutServBuf set;                                  //<87,17>
	[_sended] = ar0;                                  //<87,17>

//88:       }

	goto L13;                                  //<88,5>
<L13>

//89:       while( fullBuffer() )

	ar0 = ar7 set;                                  //<89,12>
	call _bool._.8.8fullBuffer.1.2;                                  //<89,12>
	ar0 = gr7 set;                                  //<89,12>
	gr0 = ar0 set;                                  //<89,12>
	gr1 = false noflags;                                  //<89,12>
	gr0 - gr1;                                  //<89,12>
	if <>0 goto L16;                                  //<89,12>
	goto L17;                                  //<89,12>
<L16>

//90:           dummy(); //  Здесь ждем при засоре 2

	ar0 = ar7 set;                                  //<90,9>
	call _void._.8.8dummy.1.2;                                  //<90,9>
	goto L13;                                  //<89,5>
<L17>

//91:       incomplete= true;

	ar0 = 1 set;                                  //<91,17>
	[_bool._.8.8incomplete] = ar0;                                  //<91,17>

//92:       return (int*)sended+2;

	ar0 = 2 set;                                  //<92,25>
	ar1 = [_sended];                                  //<92,18>
	ar0 = ar0 set;                                  //<92,24>
	gr0 = ar1 set;                                  //<92,24>
	gr1 = ar0 set;                                  //<92,24>
	gr0 = gr0 + gr1 noflags;                                  //<92,24>
	ar0 = gr0 set;                                  //<92,24>
	ar5 = ar0 set;                                  //<92,5>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<92,5>

	//end of function

end ".text";

begin ".text"

global _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2 : label;
<_void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2>
<L18>

//93:   }
//94:   
//95:   void completeMessage( NM_IO_ServiceID id, int size )

	push ar6, gr6;
	ar6 = ar7;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
                                  //<95,1>

//96:   {
//97:       if ( !incomplete )

	gr0 = [_bool._.8.8incomplete];                                  //<97,11>
	gr1 = false noflags;                                  //<97,11>
	gr0 - gr1;                                  //<97,11>
	if <>0 goto L20;                                  //<97,11>
	goto L19;                                  //<97,11>
<L19>

//98:           throw "rpc error: can't complete- message is not started";

	ar1 = __str__727063206572726f723a2063616e277420636f6d706c6574652d206d657373616765206973206e6f74207374617274656400 set;                                  //<98,67>
	ar2 = 1 set;                                  //<98,67>
	ar0 = ar7 set;                                  //<98,67>
	ar7 = ar7 + 2;                                  //<98,67>
	[ar0++] = ar0;                                  //<98,67>
	ar0 = ar0 set;                                  //<98,67>
	[ar0++] = ar2;                                  //<98,67>
	call __allocateThrownObject;                                  //<98,67>
	ar7 = ar7 - 2;                                  //<98,67>
	ar0 = ar5 set;                                  //<98,67>
	[ar0] = ar1;                                  //<98,67>
	ar1 = ___char._const._.0__catch_info set;                                  //<98,67>
	ar2 = 0 set;                                  //<98,67>
	ar0 = ar7 set;                                  //<98,67>
	ar7 = ar7 + 2;                                  //<98,67>
	[ar0++] = ar2;                                  //<98,67>
	ar0 = ar0 set;                                  //<98,67>
	[ar0++] = ar1;                                  //<98,67>
	call __throwObject;                                  //<98,67>
	ar7 = ar7 - 2;                                  //<98,67>
	goto L20;                                  //<98,67>
<L20>

//99:       if ( size+2 >packetLimit )

	gr1 = 32768 set;                                  //<99,18>
	gr2 = 2 set;                                  //<99,15>
	gr0 = [ar6+-6];                                  //<99,10>
	gr0 = gr0 + gr2 noflags;                                  //<99,14>
	gr0 - gr1;                                  //<99,18>
	if v> goto L21;                                  //<99,18>
	goto L22;                                  //<99,18>
<L21>

//100:          throw "rpc error: too big message";

	ar1 = __str__727063206572726f723a20746f6f20626967206d65737361676500 set;                                  //<100,44>
	ar2 = 1 set;                                  //<100,44>
	ar0 = ar7 set;                                  //<100,44>
	ar7 = ar7 + 2;                                  //<100,44>
	[ar0++] = ar0;                                  //<100,44>
	ar0 = ar0 set;                                  //<100,44>
	[ar0++] = ar2;                                  //<100,44>
	call __allocateThrownObject;                                  //<100,44>
	ar7 = ar7 - 2;                                  //<100,44>
	ar0 = ar5 set;                                  //<100,44>
	[ar0] = ar1;                                  //<100,44>
	ar1 = ___char._const._.0__catch_info set;                                  //<100,44>
	ar2 = 0 set;                                  //<100,44>
	ar0 = ar7 set;                                  //<100,44>
	ar7 = ar7 + 2;                                  //<100,44>
	[ar0++] = ar2;                                  //<100,44>
	ar0 = ar0 set;                                  //<100,44>
	[ar0++] = ar1;                                  //<100,44>
	call __throwObject;                                  //<100,44>
	ar7 = ar7 - 2;                                  //<100,44>
	goto L22;                                  //<100,44>
<L22>

//101:      incomplete= false;

	gr0 = false noflags;                                  //<101,17>
	[_bool._.8.8incomplete] = gr0;                                  //<101,17>

//102:      makeMsgHeader( (int*)sended, id, size );

	ar1 = [_sended];                                  //<102,38>
	ar2 = [ar6+-5];                                  //<102,38>
	ar3 = [ar6+-6];                                  //<102,38>
	ar0 = ar7 set;                                  //<102,38>
	ar7 = ar7 + 4;                                  //<102,38>
	[ar0++] = ar0;                                  //<102,38>
	[ar0++] = ar3;                                  //<102,38>
	[ar0++] = ar2;                                  //<102,38>
	ar0 = ar0 set;                                  //<102,38>
	[ar0++] = ar1;                                  //<102,38>
	call _void._.8.8makeMsgHeader.1int._.0.9._enum._NM_IO_ServiceID.9._int.2;                                  //<102,38>
	ar7 = ar7 - 4;                                  //<102,38>

//103:      sended+= size+2;

	gr1 = 2 set;                                  //<103,19>
	gr0 = [ar6+-6];                                  //<103,19>
	gr0 = gr0 + gr1 noflags;                                  //<103,19>
	ar1 = [_sended];                                  //<103,19>
	ar0 = gr0 set;                                  //<103,19>
	gr0 = ar1 set;                                  //<103,19>
	gr1 = ar0 set;                                  //<103,19>
	gr0 = gr0 + gr1 noflags;                                  //<103,19>
	ar0 = gr0 set;                                  //<103,19>
	[_sended] = ar0;                                  //<103,19>

//104:      boardSend++;

	gr0 = [_boardSend];                                  //<104,14>
	gr1 = 1 set;                                  //<104,14>
	gr0 = gr0 + gr1 noflags;                                  //<104,14>
	[_boardSend] = gr0;                                  //<104,14>

//105:      return;

	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
return;                                  //<105,5>

	//end of function

end ".text";

begin ".text"

global _printf : label;
<_printf>
<L23>

//106:  }
//107:  
//108:  extern "C" int printf( const char* format,...)

	push ar6, gr6;
	ar6 = ar7;
	ar7 += 4;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
                                  //<108,1>

//109:  {
//110:      va_list argptr;
//111:      va_start(argptr, format);

	ar0 = 1 set;                                  //<111,29>
	ar1 = -5 set;                                  //<111,29>
	gr0 = ar1 set;                                  //<111,29>
	gr1 = ar6 set;                                  //<111,29>
	gr0 = gr0 + gr1 noflags;                                  //<111,29>
	ar1 = gr0 set;                                  //<111,29>
	ar0 = ar0 set;                                  //<111,29>
	gr0 = ar1 set;                                  //<111,29>
	gr1 = ar0 set;                                  //<111,29>
	gr0 = gr0 - gr1 noflags;                                  //<111,29>
	ar0 = gr0 set;                                  //<111,29>
	[ar6] = ar0;                                  //<111,29>

//112:  
//113:      char* buf= (char*)beginMessage();

	ar0 = ar7 set;                                  //<113,23>
	call _int._.0.8.8beginMessage.1.2;                                  //<113,23>
	ar0 = ar5 set;                                  //<113,23>
	[ar6 + 1] = ar0;                                  //<113,23>

//114:      nm_io_debug = (int)buf;

	ar0 = [ar6+1];                                  //<114,24>
	ar0 = ar0 set;                                  //<114,24>
	[_nm_io_debug] = ar0;                                  //<114,24>

//115:      int r= vsprintf( buf, format, argptr);

	ar1 = [ar6+1];                                  //<115,22>
	ar2 = [ar6+-5];                                  //<115,27>
	ar3 = [ar6];                                  //<115,35>
	ar0 = ar7 set;                                  //<115,20>
	ar7 = ar7 + 4;                                  //<115,20>
	[ar0++] = ar0;                                  //<115,20>
	[ar0++] = ar3;                                  //<115,20>
	[ar0++] = ar2;                                  //<115,20>
	ar0 = ar0 set;                                  //<115,20>
	[ar0++] = ar1;                                  //<115,20>
	call _vsprintf;                                  //<115,20>
	ar7 = ar7 - 4;                                  //<115,20>
	ar0 = gr7 set;                                  //<115,20>
	[ar6 + 2] = ar0;                                  //<115,20>

//116:      nm_io_debug = 0;

	gr0 = false noflags;                                  //<116,19>
	[_nm_io_debug] = gr0;                                  //<116,19>

//117:      completeMessage( NMSYNC_SERVICE_PRINTF, r );

	ar1 = 4278190082 set;                                  //<117,46>
	ar2 = [ar6+2];                                  //<117,46>
	ar0 = ar7 set;                                  //<117,46>
	ar7 = ar7 + 2;                                  //<117,46>
	[ar0++] = ar2;                                  //<117,46>
	ar0 = ar0 set;                                  //<117,46>
	[ar0++] = ar1;                                  //<117,46>
	call _void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2;                                  //<117,46>
	ar7 = ar7 - 2;                                  //<117,46>

//118:      va_end( argptr );

	gr0 = false noflags;                                  //<118,21>
	ar0 = gr0 set;                                  //<118,21>
	[ar6] = ar0;                                  //<118,21>

//119:      return r;

	ar0 = [ar6+2];                                  //<119,12>
	gr7 = ar0 set;                                  //<119,5>
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	ar7 -= 4;
	pop ar6,gr6;
return;                                  //<119,5>

	//end of function

end ".text";

begin ".text"

end ".text";
