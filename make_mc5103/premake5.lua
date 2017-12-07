#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc5103"
   configurations { "Debug", "Release" }

	project "hal-mc5103"
      kind "Makefile"
      files { "../src_1879vm5ya/*.*","../include/*.h", "Makefile" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands {"make clean"}		   
		
		
		
	project "hal-mc5103-x86"
      kind "StaticLib"
      files { 	"../src_mc5103_host/*.*",
				"../src_x86/*.*",
				"../include/*.h"}
	  includedirs { "../include","$(MC5103)/libload"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 targetdir ("../lib")
		
	 
	project "hal-mc5103io-x86"
      kind "StaticLib"
      files { 	"../src_mc5103_host_io/*.*",
				"../src_x86/*.*",
				"../src_host_io/*.*",
				"../include/*.h"}
	  includedirs { "../include","$(MC5103)/libload","../src_mc5103_host_io","../src_host_io"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG","NM6405"}
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG","NM6405" }
         symbols  "Off" 
		 targetdir ("../lib")
		
