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
	  libdirs { "../../../lib" }
	  links { "hal-virtual-x86d.lib" }

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 includedirs { "../../../include"}
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 includedirs { "../../../include"}
		 
		 
		 
solution "board-target0"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "board-target0"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_nm0/*.cpp" }
	  libdirs { "../../../lib" }
	  links { "hal-virtual-x86d.lib" }


      configuration "Debug"
         defines { "DEBUG","PROCESSOR_ID=0" }
         symbols  "On" 
		 objdir("obj0")
		 includedirs { "","../../../include"}
		 

      configuration "Release"
         defines { "NDEBUG","PROCESSOR_ID=0" }
         symbols  "Off"
		 objdir("obj0")		 
		 includedirs { "../../../include"}
		 
solution "board-target1"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "board-target1"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "../src_nm1/*.cpp" }
	  libdirs { "../../../lib" }
	  links { "hal-virtual-x86d.lib" }


      configuration "Debug"
         defines { "DEBUG","PROCESSOR_ID=1" }
         symbols  "On" 
		 objdir("obj1")
		 includedirs { "","../../../include"}
		 

      configuration "Release"
         defines { "NDEBUG","PROCESSOR_ID=1" }
         symbols  "Off" 
		 objdir("obj1")
		 includedirs { "../../../include"}
		 
		 
		 
