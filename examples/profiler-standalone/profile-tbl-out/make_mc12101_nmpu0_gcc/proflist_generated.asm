//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.py
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN( 16 );

	FUNCTION( hw_init_hook ,      "hw_init_hook    ");//  0000000000008016  [ .text ]
	FUNCTION( _sum ,               "sum             ");//  0000000000008070  [ .text ]
	FUNCTION( _mycos ,             "mycos           ");//  00000000000080a0  [ .text ]
	FUNCTION( _mysin ,             "mysin           ");//  00000000000080ca  [ .text ]
	NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000000008652  [ .text ]
	NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000000008700  [ .text ]
	NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000000008712  [ .text ]
	FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000000008780  [ .text ]
	FUNCTION( UDiv64 ,            "UDiv64          ");//  00000000000087e6  [ .text ]
	NONCFUNC( LShift32 ,          "LShift32        ");//  0000000000008804  [ .text ]
	NONCFUNC( UMod32 ,            "UMod32          ");//  0000000000008890  [ .text ]
	NONCFUNC( IMod32 ,            "IMod32          ");//  00000000000088bc  [ .text ]
	FUNCTION( UMod64 ,            "UMod64          ");//  00000000000088f2  [ .text ]
	FUNCTION( IMod64 ,            "IMod64          ");//  000000000000892e  [ .text ]
	NONCFUNC( Mul32 ,             "Mul32           ");//  000000000000897a  [ .text ]
	NONCFUNC( RShift32 ,          "RShift32        ");//  0000000000008994  [ .text ]
	FUNCTION( RShift64 ,          "RShift64        ");//  0000000000008a20  [ .text ]
	NONCFUNC( DAdd ,              "DAdd            ");//  0000000000008b3c  [ .text ]
	NONCFUNC( DCmp ,              "DCmp            ");//  0000000000008b44  [ .text ]
	NONCFUNC( DDiv ,              "DDiv            ");//  0000000000008b4e  [ .text ]
	FUNCTION( _fmod_yx ,           "fmod_yx         ");//  0000000000008b6e  [ .text ]
	NONCFUNC( DMul ,              "DMul            ");//  0000000000008b96  [ .text ]
	NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  0000000000008b9e  [ .text ]
	NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  0000000000008bb4  [ .text ]
	NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  0000000000008bbc  [ .text ]
	FUNCTION( _halMapAddr ,        "halMapAddr      ");//  0000000000009164  [ .text_hal ]
	//FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  0000000000009164  [ .text_hal ]
	//FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  0000000000009164  [ .text_hal ]

PROFILE_END();
end ".text_nmprofiler";
