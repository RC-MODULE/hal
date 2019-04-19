#!lua
root="../../.."
-- A solution contains projects, and defines the available configurations
solution "test_mc5103"
	configurations { "Debug", "Release" }
	project "test_mc5103"
		kind "Makefile"
		files { "../*.cpp", "*.cfg", "Makefile" }
		includedirs {root.."/include"}
		debugcommand "$(GNUWIN32)/bin/make"
		debugargs  "rund" 

		configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands {"make clean"}
				   
		configuration "Release"
		   buildcommands {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands {"make clean"}		   
		   
		  