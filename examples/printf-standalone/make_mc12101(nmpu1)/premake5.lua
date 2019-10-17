#!lua

-- A solution contains projects, and defines the available configurations
solution "mc12101-printf"
   configurations { "Debug", "Release" }


		 
		 
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../src_nm1/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   