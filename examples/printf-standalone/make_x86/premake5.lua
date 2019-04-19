#!lua
root="../../.."
-- A solution contains projects, and defines the available configurations
  solution "test-x86"		 
	configurations { "Debug", "Release" }
	project "test-x86"
      kind "ConsoleApp"
      language "C++"
      files { "../*.cpp" }
	  links { "hal-virtual-x86.lib" } 
	  libdirs { root.."/lib"}
	  includedirs { root.."/include"}
	  
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 

