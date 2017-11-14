#!lua

-- A solution contains projects, and defines the available configurations
solution "mc5103access"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc5103-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  links { "hal-mc5103-x86.lib","mc5103load.lib","mc5103-nmc3" } 
	  includedirs { "../../../include"}	  
	  libdirs { "$(MC5103)/libload","../../../lib" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
	project "mc5103-nmc3"
      kind "Makefile"
      files { "../src_nm/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   