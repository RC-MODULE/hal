#!lua

-- A solution contains projects, and defines the available configurations
solution "test_mb7707"
	configurations { "Debug", "Release" }
 -- A project defines one build target
   project "mb7707-x86"
		kind "ConsoleApp"
		language "C++"
		files { "**.h", "../src_pc/*.cpp"}
		links { "hal-mb7707-x86.lib","mb7707load.lib","mb7707-nmc3" } 
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
      files { "../*.cpp", "*.cfg", "Makefile" }
	  includedirs {"$(NMPP)/include","$(HAL)/include"}
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands {"make clean"}		   
		   
		  