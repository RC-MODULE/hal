#!lua

-- A solution contains projects, and defines the available configurations
solution "mc12101access"
	configurations { "Debug", "Release" }
	
	platforms{"x64","x86"}
   -- A project defines one build target
	project "mc12101-x86"
		
		kind "ConsoleApp"
		language "C++"
		files { "../../../*.h", "../src_pc/*.cpp", "../../../src/mc12101/*.cpp" }
		
		libdirs { "$(MC12101)/lib","../../../lib" }
		includedirs { "$(MC12101)/include","../../../include"}
		
		configuration {"Debug","Win32"}
			architecture "x32"
			defines { "DEBUG" }
			symbols  "On" 
			links { "mc12101load.lib","hal-mc12101-x86d.lib","mc12101-nmc4-0", "mc12101-nmc4-1" }
		
		configuration {"Release","Win32"}
			architecture "x32"
			defines { "NDEBUG" }
			symbols  "Off" 
			links { "mc12101load.lib","hal-mc12101-x86.lib","mc12101-nmc4-0", "mc12101-nmc4-1" }
		
		configuration {"Debug","x64"}
			architecture "x64"
			defines { "DEBUG" }
			symbols  "On" 
			links { "mc12101load.lib","hal-mc12101-x64d.lib","mc12101-nmc4-0", "mc12101-nmc4-1" }
		
		configuration {"Release","x64"}
			architecture "x64"
			defines { "NDEBUG" }
			symbols  "Off" 
			links { "mc12101load.lib","hal-mc12101-x64.lib","mc12101-nmc4-0", "mc12101-nmc4-1" }
		
		
		
		 
		 
	project "mc12101-nmc4-0"
      kind "Makefile"
      files { "../../../include/*.h","../src_nm0/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile0"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile0"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile0"}
		   rebuildcommands {"make -B -f Makefile0"}
		   cleancommands {"make clean"}		   
		   
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../../../include/*.h","../src_nm1/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile1"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile1"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile1"}
		   rebuildcommands {"make -B -f Makefile1"}
		   cleancommands {"make clean"}		   		   