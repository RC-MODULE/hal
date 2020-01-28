#!lua

HAL = "../../../.."
-- A solution contains projects, and defines the available configurations
solution "board-nmc0"
	configurations { "Debug", "Release" }
	-- platforms{"x64","x86"}
   -- A project defines one build target
   project "board-nmc0"
      
      kind "ConsoleApp"
      language "C++"
	  files { "**.h", "../src_nm0/*.cpp" }
      libdirs { HAL.."/lib"}
      includedirs { HAL.."/include"}

      configuration "Debug"
		links { "hal-virtual-x86d.lib"} 
        defines { "DEBUG" }
        symbols  "On" 

		 

      configuration "Release"
		links { "hal-virtual-x86.lib"} 
         defines { "NDEBUG" }
         symbols  "Off" 
 
		 
		 
solution "board-nmc1"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "board-nmc1"
      kind "ConsoleApp"
      language "C++"
	  libdirs { HAL.."/lib"}
      files { "**.h", "../src_nm1/*.cpp" }
      
      libdirs { HAL.."/lib"}
      includedirs { HAL.."/include"}


      configuration "Debug"
		links { "hal-virtual-x86d.lib"} 
        defines { "DEBUG" }
        symbols  "On" 
		 

      configuration "Release"
		links { "hal-virtual-x86.lib"} 
        defines { "NDEBUG" }
        symbols  "Off" 
		 
		 
