#!lua

-- A solution contains projects, and defines the available configurations
solution "mc7601-led"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc7601-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  links { "hal-mc7601-x86.lib","mc7601_ll_pc.lib","mc7601-nmc3" } 
	  includedirs { "../../../include"}	  
	  libdirs { "$(MC7601)/libload","../../../lib" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
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