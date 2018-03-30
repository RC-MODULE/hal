#!lua

HAL = "../../.."
BOARD = "mb7707"
-- A solution contains projects, and defines the available configurations
solution "vshell"
	configurations { "Debug", "Release" }
	project "host"
		kind "ConsoleApp"
		language "C++"
		files { "../src-pc/*.cpp"}
		links {  "hal-"..BOARD.."silent-x86.lib", BOARD.."load.lib", "vshell.lib" } 
		libdirs { HAL.."/lib","$(HAL)/lib","$("..BOARD..")/libload","$(VSHELL)/lib"}
		includedirs { HAL.."/include","$(HAL)/include","$(VSHELL)/include"}
		debugenvs { "PATH=%PATH%;$(VSHELL)/bin" }
		configuration "Debug"
			defines { "DEBUG" }
			symbols  "On" 
		configuration "Release"
			defines { "NDEBUG" }
			symbols  "Off" 
		 
	project "target"
        kind "Makefile"
		files { "../src-nm/*.cpp", BOARD.."brd.cfg", "Makefile" }
	 
		configuration "Debug"
			buildcommands {"make DEBUG=y -f Makefile"}
			rebuildcommands {"make -B DEBUG=y -f Makefile"}
			cleancommands {"make clean"}
		   
		configuration "Release"
			buildcommands {"make -f Makefile"}
			rebuildcommands {"make -B -f Makefile"}
			cleancommands {"make clean"}		   
		   
		  