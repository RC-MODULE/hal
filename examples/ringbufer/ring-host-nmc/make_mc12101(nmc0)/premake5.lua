#!lua

-- A solution contains projects, and defines the available configurations
solution "mc12101access"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "mc12101-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp"}
	  libdirs { "$(MC12101)/lib","../../../../lib"}
	  links { "mc12101load.lib","hal-mc12101-x86.lib","mc12101-nmc4-0"} 
	  includedirs { "$(MC12101)/include","../../../../include"}

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 

		 
		 
	project "mc12101-nmc4-0"
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