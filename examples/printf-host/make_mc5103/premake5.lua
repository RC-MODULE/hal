#!lua

-- A solution contains projects, and defines the available configurations
solution "mc5103-printf"
	configurations { "Debug", "Release" }
	startproject "mc5103-printf"
   -- A project defines one build target
   project "mc5103-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  includedirs { "../../../include"}	  
	  libdirs { "$(MC5103)/libload","../../../lib" }

      configuration "Debug"
		links { "hal-mc5103-x86.lib","mc5103load.lib"}
		--,"mc5103-nmc3" } 
        defines { "DEBUG" }
        symbols  "On" 

      configuration "Release"
	  	links { "hal-mc5103-x86.lib","mc5103load.lib"}
		--,"mc5103-nmc3" } 
        defines { "NDEBUG" }
        symbols  "Off" 
		 
		 
	project "mc5103-nmc3"
      kind "Makefile"
      files { "../src_nm0/*.*" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   