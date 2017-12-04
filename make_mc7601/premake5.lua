#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc7601"
   configurations { "Debug", "Release" }

	project "hal-mc7601"
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
		
		
		
	project "hal-mc7601-x86"
      kind "StaticLib"
      files { "../src_mc7601_host/*.*","../include/*.h"}
	  includedirs { "../include","$(MC7601)/include"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 targetdir ("../lib")
		
	 
	project "hal-mc7601io-x86"
      kind "StaticLib"
      files { "../src_mc7601_host_io/*.*","../include/*.h"}
	  includedirs { "../include","$(MC7601)/include","../src_mc7601_host_io"}
	  
	  
	  configuration "Debug"
         defines { "DEBUG","NM6405"}
         symbols  "On" 
		 targetdir ("../lib")
		 

      configuration "Release"
         defines { "NDEBUG","NM6405" }
         symbols  "Off" 
		 targetdir ("../lib")
		