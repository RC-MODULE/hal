#!lua

-- A solution contains projects, and defines the available configurations
solution "test_virtuale"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "test_virtuale"
      kind "ConsoleApp"
      language "C++"
      files {"../../../../src_virtual/halDMA.c", "test.c"}
	  libdirs { "$(HAL)/lib"}
	  includedirs { "$(HAL)/include"}
	  links { "hal-virtual-x86.lib" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
		 
