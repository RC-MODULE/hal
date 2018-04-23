#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc12101"
   configurations { "Debug", "Release" }
		
		
	-- x86  library with printf support  ---------------------------------	 
	project "hal-mc12101-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC12101)/include","../src_host_io"}
		targetdir ("../lib")
      		files { 	"../src_mc12101_host/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"../src_host_io/*.*"}
	  
		configuration "Debug"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG","NM6405"}
			symbols  "Off" 
		
	-- x86  library without printf support  ---------------------------------	 
	project "hal-mc12101silent-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC12101)/include"}
		targetdir ("../lib")
      	files {	"../src_mc12101_host/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h"}

	  
		configuration "Debug"
			defines { "DEBUG","SILENT","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG","SILENT","NM6405"}
			symbols  "Off" 
		
		
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101"
      	kind "Makefile"
		files { "../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../src_ringbuffer/*.*",
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
	
	