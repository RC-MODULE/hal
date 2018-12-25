#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc7601"
   	configurations { "Debug", "Release" }
	-- x86  library with printf support  ---------------------------------	 
	project "hal-mc7601-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC7601)/include","../src_mc12101_host"}
		targetdir ("../lib")
		files { 	"../src_mc7601_host/*.*",
				"../src_x86/*.*",
				"../include/*.h"}
	  
	  
	  
		configuration "Debug"
			targetsuffix ("d")
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG","NM6405"}
			symbols  "Off" 
		
	-- x86  library without printf support  ---------------------------------	 
	project "hal-mc7601silent-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC7601)/include"}
		targetdir ("../lib")
      		files { 	"../src_mc7601_host/*.*",
				"../src_x86/*.*",
				"../include/*.h"}
	  
	  
	  
		configuration "Debug"
			targetsuffix ("d")
			defines { "DEBUG","SILENT","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG","SILENT","NM6405"}
			symbols  "Off" 
		

	-- NeuroMatrix project ---------------------------------
	project "hal-mc7601"
      	kind "Makefile"
      		files { "../src_src_1879vya1ya/*.*","../include/*.h", "Makefile" }
	 
		  configuration "Debug"
			buildcommands   {"make DEBUG=y -f Makefile"}
			rebuildcommands {"make -B DEBUG=y -f Makefile"}
			cleancommands   {"make clean"}
		   
		  configuration "Release"
			buildcommands   {"make -f Makefile"}
			rebuildcommands {"make -B -f Makefile"}
			cleancommands   {"make clean"}		   
	
	