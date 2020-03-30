#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc12101"
	configurations { "Debug", "Release" }
	platforms { "Win32","x64"}		
	
		
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101(gcc)"
      	kind "Makefile"
		files { "../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y -f    Makefile $(CPCONV)"}
			rebuildcommands {"make -B DEBUG=y -f Makefile $(CPCONV)"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make -f Makefile	$(CPCONV)"}
			rebuildcommands {"make -B -f Makefile 	$(CPCONV)"}
			cleancommands   {"make clean"}		   
	
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101(legacy)"
      	kind "Makefile"
		files { "../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y legacy $(CPCONV)"}
			rebuildcommands {"make -B DEBUG=y legacy $(CPCONV)"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make legacy	$(CPCONV)"}
			rebuildcommands {"make -B legacy 	$(CPCONV)"}
			cleancommands   {"make clean"}		   
	
	
		
	-- x86/x64 static library project  ---------------------------------	 

	project "hal-mc12101"
		kind "StaticLib"
	
		includedirs { "../include","$(MC12101)/include","../src_host_io"}
		
		targetdir ("../lib")
      	
		files { 	"../src_mc12101_host/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"../src_host_io/*.*"}
	  
		configuration {"Debug","Win32"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration {"Release","Win32"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG","NM6405"}
         		symbols  "Off" 
		
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG","NM6405"}
			symbols  "Off" 
		