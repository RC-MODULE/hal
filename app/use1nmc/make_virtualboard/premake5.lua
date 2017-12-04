#!lua

-- A solution contains projects, and defines the available configurations
solution "board-host"
	configurations { "Debug", "Release" }
	-- platforms{"x64","x86"}
   -- A project defines one build target
   project "board-host"
      
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_pc/*.cpp" }
      links { "../../../lib/hal-virtual-x86.lib"} 
      includedirs { "../../../include"}

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
 
		 
		 
solution "board-target"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "board-target"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_nm/*.cpp" }
      links { "../../../lib/hal-virtual-x86.lib"} 
      includedirs { "../../../include"}


      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
