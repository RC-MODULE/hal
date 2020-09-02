#!lua

-- A solution contains projects, and defines the available configurations
solution "board-host"
	configurations { "Debug", "Release" }
	platforms{"x64","x86"}
   -- A project defines one build target
   project "board-host"
      
		kind "ConsoleApp"
		language "C++"
		files { "**.h", "../src_pc/*.cpp" }
		libdirs { "../../../lib" }
		includedirs { "../../../include"}

       
		 
		configuration {"Debug","Win32"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
			links { "hal-virtual-x86d.lib" }
		
		configuration {"Release","Win32"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG","NM6405"}
         	symbols  "Off" 
			links { "hal-virtual-x86.lib" }
		
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
			links { "hal-virtual-x64d.lib" }
		
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG","NM6405"}
			symbols  "Off" 
			links { "hal-virtual-x64.lib" }
			
		 
		 
solution "board-target0"
   configurations { "Debug", "Release" }
   platforms{"x64","x86"}

   -- A project defines one build target
   project "board-target0"
		kind "ConsoleApp"
		language "C++"
		files { "**.h", "../src_nm0/*.cpp" }
		libdirs { "../../../lib" }
		includedirs { "../../../include"}
		objdir("obj0")		 
		 
		 
		configuration {"Debug","Win32"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG","PROCESSOR_ID=0"}
			symbols  "On" 
			links { "hal-virtual-x86d.lib" }
		
		configuration {"Release","Win32"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG","PROCESSOR_ID=0"}
         	symbols  "Off" 
			links { "hal-virtual-x86.lib" }
		
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG","PROCESSOR_ID=0"}
			symbols  "On" 
			links { "hal-virtual-x64d.lib" }
		
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG","PROCESSOR_ID=0"}
			symbols  "Off" 
			links { "hal-virtual-x64.lib" }
			
		 
solution "board-target1"
   configurations { "Debug", "Release" }
   platforms{"x64","x86"}

   -- A project defines one build target
   project "board-target1"
		kind "ConsoleApp"
		language "C++"
		files { "**.h", "../src_nm1/*.cpp" }
		libdirs { "../../../lib" }
		includedirs { "../../../include"}
		objdir("obj1")		 
		 
		 
		configuration {"Debug","Win32"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG","PROCESSOR_ID=1"}
			symbols  "On" 
			links { "hal-virtual-x86d.lib" }
		
		configuration {"Release","Win32"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG","PROCESSOR_ID=1"}
         	symbols  "Off" 
			links { "hal-virtual-x86.lib" }
		
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG","PROCESSOR_ID=1"}
			symbols  "On" 
			links { "hal-virtual-x64d.lib" }
		
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG","PROCESSOR_ID=1"}
			symbols  "Off" 
			links { "hal-virtual-x64.lib" }
		 
		 
		 
