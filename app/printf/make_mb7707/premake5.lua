#!lua

-- A solution contains projects, and defines the available configurations
solution "mb7707-printf"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mb7707-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  links { "hal-mb7707io-x86.lib","mb7707load.lib","mb7707-nmc3" } 
	  includedirs { "../../../include"}	  
	  libdirs { "$(mb7707)/libload","../../../lib" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
	project "mb7707-nmc3"
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