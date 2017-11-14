#!lua

-- A solution contains projects, and defines the available configurations
solution "mc7601access"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc7601-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp", "../../../src/mc7601/*.cpp" }
	  libdirs { "$(MC7601)/libload" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 includedirs { "$(MC7601)/libload","../../../include"}
		 links { "mc7601_ll_pc.lib","mc7601-nmc3" }

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 includedirs { "$(MC7601)/libload","../../../include"}
		 links { "mc7601_ll_pc.lib","mc7601-nmc3" }
		 
		 
	project "mc7601-nmc3"
      kind "Makefile"
      files { "../src_nm/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y"}
		   rebuildcommands {"make -B DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make"}
		   rebuildcommands {"make -B"}
		   cleancommands {"make clean"}		   