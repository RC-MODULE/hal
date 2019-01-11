#!lua
ROOT = "../../.."
-- A solution contains projects, and defines the available configurations
solution "app_mc5103"

	configurations { "Debug", "Release" }
	project "host"
      kind "ConsoleApp"
      language "C++"
      files { "../src_pc/*.cpp"}
	  links { "mc5103load"} 
	  libdirs { "$(HAL)/lib","$(MC5103)/libload"}
	  includedirs { "$(HAL)/include"}
	  
      configuration "Debug"
		links {  "hal-mc5103-x86d"} 
        defines { "DEBUG" }
        symbols  "On" 
		 
		 

      configuration "Release"
		links {  "hal-mc5103-x86"} 
        defines { "NDEBUG" }
        symbols  "Off" 
		 


		 
	configurations { "Debug", "Release" }
	project "target"
      kind "Makefile"
      files { "../src_nm0/*.cpp", "*.cfg", "Makefile0" }
	  includedirs {"$(HAL)/include"}
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile0"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile0"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile0"}
		   rebuildcommands {"make -B -f Makefile0"}
		   cleancommands {"make clean"}		   
		   
		  