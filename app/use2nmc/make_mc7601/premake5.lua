#!lua

-- A solution contains projects, and defines the available configurations
solution "mc7601access"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc7601-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp" }
	  libdirs { "../../../lib" }
	  links { "hal-mc7601-x86.lib" }

	  libdirs { "$(MC7601)/libload" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 includedirs { "$(MC7601)/libload","../../../include"}
		 links { "mc7601_ll_pc.lib" , "mc7601-nmc3-0", "mc7601-nmc3-1"}

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 includedirs { "$(MC7601)/libload","../../../include"}
		 links { "mc7601_ll_pc.lib" , "mc7601-nmc3-0", "mc7601-nmc3-1"}
		 
		 
	project "mc7601-nmc3-0"
      kind "Makefile"
      files { "../src_nm0/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   
		   
	project "mc7601-nmc3-1"
      kind "Makefile"
      files { "../src_nm1/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   
		   