//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.py
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN( 16 );

	FUNCTION( hw_init_hook ,      "hw_init_hook    ");//  0000000020000016  [ .text ]
	FUNCTION( _sum ,               "sum             ");//  0000000020000070  [ .text ]
	FUNCTION( _mycos ,             "mycos           ");//  00000000200000a0  [ .text ]
	FUNCTION( _mysin ,             "mysin           ");//  00000000200000ca  [ .text ]
	NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000020000666  [ .text ]
	NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000020000714  [ .text ]
	NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000020000726  [ .text ]
	FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000020000794  [ .text ]
	FUNCTION( UDiv64 ,            "UDiv64          ");//  00000000200007fa  [ .text ]
	NONCFUNC( LShift32 ,          "LShift32        ");//  0000000020000818  [ .text ]
	NONCFUNC( UMod32 ,            "UMod32          ");//  00000000200008a4  [ .text ]
	NONCFUNC( IMod32 ,            "IMod32          ");//  00000000200008d0  [ .text ]
	FUNCTION( UMod64 ,            "UMod64          ");//  0000000020000906  [ .text ]
	FUNCTION( IMod64 ,            "IMod64          ");//  0000000020000942  [ .text ]
	NONCFUNC( Mul32 ,             "Mul32           ");//  000000002000098e  [ .text ]
	NONCFUNC( RShift32 ,          "RShift32        ");//  00000000200009a8  [ .text ]
	FUNCTION( RShift64 ,          "RShift64        ");//  0000000020000a34  [ .text ]
	NONCFUNC( DAdd ,              "DAdd            ");//  0000000020000b50  [ .text ]
	NONCFUNC( DCmp ,              "DCmp            ");//  0000000020000b58  [ .text ]
	NONCFUNC( DDiv ,              "DDiv            ");//  0000000020000b62  [ .text ]
	FUNCTION( _fmod_yx ,           "fmod_yx         ");//  0000000020000b82  [ .text ]
	NONCFUNC( DMul ,              "DMul            ");//  0000000020000baa  [ .text ]
	NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  0000000020000bb2  [ .text ]
	FUNCTION( ConvFtoD ,          "ConvFtoD        ");//  0000000020000bc8  [ .text ]
	NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  0000000020000bd0  [ .text ]
	NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  0000000020000bd8  [ .text ]
	FUNCTION( ConvU32toF ,        "ConvU32toF      ");//  0000000020000be2  [ .text ]
	FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  00000000200011ea  [ .text_hal ]
	FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  00000000200011fe  [ .text_hal ]

PROFILE_END();
end ".text_nmprofiler";
