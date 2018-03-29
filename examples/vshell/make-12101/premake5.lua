#!lua

HAL = "../../.."
-- A solution contains projects, and defines the available configurations
solution "vshell"
	configurations { "Debug", "Release" }
	project "host"
		kind "ConsoleApp"
		language "C++"
		files { "../src-pc/*.cpp"}
		links { "hal-mc12101-x86.lib","mc12101load.lib","vshell.lib"  } 
	    libdirs {"$(HAL)/lib","$(MC12101)/lib","$(VSHELL)/lib"}
		includedirs { "$(NMPP)/include","$(HAL)/include","$(VSHELL)/include","../include"}
		debugenvs { "PATH=%PATH%;$(VSHELL)/bin" }
		configuration "Debug"
			defines { "DEBUG" }
			symbols  "On" 
		configuration "Release"
			defines { "NDEBUG" }
			symbols  "Off" 
		 
	project "target"
        kind "Makefile"
		files { "../src-nm/*.cpp", "mc12101brd-nmc0.cfg", "Makefile" }
	 
		configuration "Debug"
			buildcommands {"make DEBUG=y -f Makefile"}
			rebuildcommands {"make -B DEBUG=y -f Makefile"}
			cleancommands {"make clean"}
		   
		configuration "Release"
			buildcommands {"make -f Makefile"}
			rebuildcommands {"make -B -f Makefile"}
			cleancommands {"make clean"}		   
		   
		  