#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-mc12101"
   configurations { "Debug", "Release" }
   startproject "mc12101-x86"

   -- A project defines one build target
   project "mc12101-x86"
		kind "ConsoleApp"
		language "C++"
		files { "../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(MC12101)/lib","$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../include"}
		links { "mc12101load.lib","vshell.lib","nmpp-x86.lib","hal-mc12101-x86d.lib","mc12101-nmc4-0","mc12101-nmc4-1"}

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
	project "mc12101-nmc4-0"
      kind "Makefile"
      files { "../../include/*.h","../src_target0/*.*", "../../src_proc0/nm/*.*","../../src_proc0/common/*.*","../../src_proc0/nmgl/*.*","../../nmglvs_mc12101-gcc/src_nmc0/*.*", "mc12101brd-nmc0.cfg", "Makefile0" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile0"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile0"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile0"}
		   rebuildcommands {"make -B -f Makefile0"}
		   cleancommands {"make clean"}		   
		   
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../../include/*.h","../src_target1/*.*", "../../src_proc1/nm/*.*","../../src_proc1/common/*.*", "../../nmglvs_mc12101-gcc/src_nmc1/*.*","mc12101brd-nmc1.cfg", "Makefile1" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile1"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile1"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile1"}
		   rebuildcommands {"make -B -f Makefile1"}
		   cleancommands {"make clean"}		   
		   
		  