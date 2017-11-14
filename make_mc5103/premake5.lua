#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc12101"
   configurations { "Debug", "Release" }

	project "hal-mc12101"
      kind "Makefile"
      files { "../src_src_1879vya1ya/*.*","../include/*.h", "Makefile" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands {"make clean"}		   
		