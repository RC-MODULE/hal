#!lua

-- A solution contains projects, and defines the available configurations
solution "mc12101-printf"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc12101-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  libdirs { "$(MC12101)/lib","../../../lib"}
	  links { "mc12101load.lib","mc12101-nmc4-0","mc12101-nmc4-1"} 
	  includedirs { "$(MC12101)/include","../../../include"}

      configuration "Debug"
         defines { "DEBUG","NMPU0","NMPU1" }
		 links { "hal-mc12101-x86d.lib"} 
         symbols  "On" 
		 

      configuration "Release"
         defines { "NDEBUG","NMPU0","NMPU1" }
		 links { "hal-mc12101-x86.lib"} 
         symbols  "Off" 

		 
		 
	project "mc12101-nmc4-0"
      kind "Makefile"
      files { "../src_nm0/*.cpp","Makefile0","mc12101brd-nmc0.cfg" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile0 " }
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile0 "}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   
		   
		   
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../src_nm1/*.cpp","Makefile1","mc12101brd-nmc1.cfg" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile1" }
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile1"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   		   