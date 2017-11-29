#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mb7707"
   configurations { "Debug", "Release" }

	project "hal-mb7707"
      kind "Makefile"
      files { "../src_1879hb1ya/*.*","../include/*.h", "Makefile" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile"}
		   rebuildcommands {"make -B -f Makefile"}
		   cleancommands {"make clean"}		   
		
		
		
	project "hal-mb7707-x86"
      kind "StaticLib"
      files { "../src_mb7707_host/*.*","../include/*.h"}
	  includedirs { "../include","$(MB7707)/libload"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 targetdir ("../lib")
		
	 
	project "hal-mb7707io-x86"
      kind "StaticLib"
      files { "../src_mb7707_host_io/*.*","../src_io/*.*","../include/*.h"}
	  includedirs { "../include","$(MB7707)/libload","../src_mb7707_host_io","../src_io"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG","NM6405"}
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG","NM6405" }
         symbols  "Off" 
		 targetdir ("../lib")
		
