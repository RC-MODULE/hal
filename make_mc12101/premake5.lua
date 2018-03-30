#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc12101"
   configurations { "Debug", "Release" }
		
		
	-- x86  project ---------------------------------
	project "hal-mc12101-x86"
      kind "StaticLib"
	  defines { "NM6405" }
      files { 	"../src_mc12101_host/*.*",
				"../src_x86/*.*",
				"../src_host_io/*.*",
				"../include/*.h"}
	  includedirs { "../include","$(MC12101)/include","../src_mc12101_host_io","../src_host_io"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 targetdir ("../lib")
		
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101"
      kind "Makefile"
      files { 	"../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../include/*.h",
				"Makefile" }
	 
	  configuration "Debug"
		   buildcommands   {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands   {"make clean"}
		   
	  configuration "Release"
		   buildcommands   {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands   {"make clean"}		   
	
	