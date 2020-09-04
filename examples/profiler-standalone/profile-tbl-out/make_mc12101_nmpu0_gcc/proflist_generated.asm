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
	NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000000008666  [ .text ]
	NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000000008714  [ .text ]
	NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000000008726  [ .text ]
	FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000000008794  [ .text ]
	FUNCTION( UDiv64 ,            "UDiv64          ");//  00000000000087fa  [ .text ]
	NONCFUNC( LShift32 ,          "LShift32        ");//  0000000000008818  [ .text ]
	NONCFUNC( UMod32 ,            "UMod32          ");//  00000000000088a4  [ .text ]
	NONCFUNC( IMod32 ,            "IMod32          ");//  00000000000088d0  [ .text ]
	FUNCTION( UMod64 ,            "UMod64          ");//  0000000000008906  [ .text ]
	FUNCTION( IMod64 ,            "IMod64          ");//  0000000000008942  [ .text ]
	NONCFUNC( Mul32 ,             "Mul32           ");//  000000000000898e  [ .text ]
	NONCFUNC( RShift32 ,          "RShift32        ");//  00000000000089a8  [ .text ]
	FUNCTION( RShift64 ,          "RShift64        ");//  0000000000008a34  [ .text ]
	NONCFUNC( DAdd ,              "DAdd            ");//  0000000000008b50  [ .text ]
	NONCFUNC( DCmp ,              "DCmp            ");//  0000000000008b58  [ .text ]
	NONCFUNC( DDiv ,              "DDiv            ");//  0000000000008b62  [ .text ]
	FUNCTION( _fmod_yx ,           "fmod_yx         ");//  0000000000008b82  [ .text ]
	NONCFUNC( DMul ,              "DMul            ");//  0000000000008baa  [ .text ]
	NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  0000000000008bb2  [ .text ]
	FUNCTION( ConvFtoD ,          "ConvFtoD        ");//  0000000000008bc8  [ .text ]
	NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  0000000000008bd0  [ .text ]
	NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  0000000000008bd8  [ .text ]
	FUNCTION( ConvU32toF ,        "ConvU32toF      ");//  0000000000008be2  [ .text ]
	FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  00000000000091ea  [ .text_hal ]
	FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  00000000000091fe  [ .text_hal ]

PROFILE_END();
end ".text_nmprofiler";
