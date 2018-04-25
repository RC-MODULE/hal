#!lua

-- A solution contains projects, and defines the available configurations
  solution "test-x86"		 
	configurations { "Debug", "Release" }
	project "test-x86"
      kind "ConsoleApp"
      language "C++"
      files { "../../../src/common/*.*","../../../src/pc/*.*", "../*.cpp" }
	  links { "nmpp-x86.lib","hal-virtual-x86.lib" } 
	  libdirs { "$(NMPP)/lib","$(HAL)/lib"}
	  includedirs { "../../../src/common","../../../include","$(NMPP)/include","$(HAL)/include"}
	  
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 

