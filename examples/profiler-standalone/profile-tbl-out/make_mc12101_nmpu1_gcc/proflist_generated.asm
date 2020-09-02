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
	NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000020000652  [ .text ]
	NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000020000700  [ .text ]
	NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000020000712  [ .text ]
	FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000020000780  [ .text ]
	FUNCTION( UDiv64 ,            "UDiv64          ");//  00000000200007e6  [ .text ]
	NONCFUNC( LShift32 ,          "LShift32        ");//  0000000020000804  [ .text ]
	NONCFUNC( UMod32 ,            "UMod32          ");//  0000000020000890  [ .text ]
	NONCFUNC( IMod32 ,            "IMod32          ");//  00000000200008bc  [ .text ]
	FUNCTION( UMod64 ,            "UMod64          ");//  00000000200008f2  [ .text ]
	FUNCTION( IMod64 ,            "IMod64          ");//  000000002000092e  [ .text ]
	NONCFUNC( Mul32 ,             "Mul32           ");//  000000002000097a  [ .text ]
	NONCFUNC( RShift32 ,          "RShift32        ");//  0000000020000994  [ .text ]
	FUNCTION( RShift64 ,          "RShift64        ");//  0000000020000a20  [ .text ]
	NONCFUNC( DAdd ,              "DAdd            ");//  0000000020000b3c  [ .text ]
	NONCFUNC( DCmp ,              "DCmp            ");//  0000000020000b44  [ .text ]
	NONCFUNC( DDiv ,              "DDiv            ");//  0000000020000b4e  [ .text ]
	FUNCTION( _fmod_yx ,           "fmod_yx         ");//  0000000020000b6e  [ .text ]
	NONCFUNC( DMul ,              "DMul            ");//  0000000020000b96  [ .text ]
	NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  0000000020000b9e  [ .text ]
	NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  0000000020000bb4  [ .text ]
	NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  0000000020000bbc  [ .text ]
	FUNCTION( _halMapAddr ,        "halMapAddr      ");//  0000000020001164  [ .text_hal ]
	//FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  0000000020001164  [ .text_hal ]
	//FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  0000000020001164  [ .text_hal ]

PROFILE_END();
end ".text_nmprofiler";
