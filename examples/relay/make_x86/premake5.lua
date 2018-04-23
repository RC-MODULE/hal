#!lua

-- A solution contains projects, and defines the available configurations
solution "relay"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "relay-x86"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../*.cpp"}
	  libdirs { "$(MC12101)/lib","../../../lib"}
	  links { "mc12101load.lib","hal-mc12101-x86.lib"} 
	  includedirs { "$(MC12101)/include","../../../include"}

      configuration "Debug"
         defines { "DEBUG","NMPU0","NMPU1" }
         symbols  "On" 
		 

      configuration "Release"
         defines { "NDEBUG","NMPU0","NMPU1" }
         symbols  "Off" 

		 
	