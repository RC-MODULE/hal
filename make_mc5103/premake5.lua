#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc5103"
   	configurations { "Debug", "Release" }

	-- x86  library with printf support  ---------------------------------	 
	project "hal-mc5103"
      		kind "StaticLib"
		includedirs { "../include","$(MC5103)/libload","../src_host_io"}
 		targetdir ("../lib")	  
      		files { 	"../src_mc5103_host/*.*",
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
		 
	-- NeuroMatrix project ---------------------------------		
	project "hal-mc5103-nmc3"
     		kind "Makefile"
		includedirs { "../include","$(MC5103)/libload"}
      		files { "../src_1879vm5ya/*.*",
		
				"../src_ringbuffer/*.*",
				"../include/*.h", 
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y -f 	 Makefile 2>.make & iconv -f cp1251 -t cp866 .make"}
			rebuildcommands {"make -B DEBUG=y -f Makefile 2>.make & iconv -f cp1251 -t cp866 .make"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make -f Makefile		2>.make & iconv -f cp1251 -t cp866 .make"}
			rebuildcommands {"make -B -f Makefile 	2>.make & iconv -f cp1251 -t cp866 .make"}
			cleancommands   {"make clean"}		   
		
		
		
