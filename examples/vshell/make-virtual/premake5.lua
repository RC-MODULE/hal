#!lua
HAL = "../../.."
-- A solution contains projects, and defines the available configurations
solution "host"
	configurations { "Debug", "Release" }
	project "host"
		kind "ConsoleApp"
		language "C++"
		files { "../src-pc/*.cpp"}
		links { "hal-virtual-x86.lib","vshell.lib" } 
		libdirs {HAL.."/lib","$(HAL)/lib","$(VSHELL)/lib"}
		includedirs { HAL.."/include","$(HAL)/include","$(VSHELL)/include"}
		debugenvs { "PATH=%PATH%;$(VSHELL)/bin"}
		configuration "Debug"
			defines { "DEBUG" }
			symbols  "On" 

		configuration "Release"
			defines { "NDEBUG" }
			symbols  "Off" 
	
	
solution "target"		 
	configurations { "Debug", "Release" }
	project "target"
		kind "ConsoleApp"
		language "C++"
		files { "../src-nm/*.cpp"}
		links { "hal-virtual-x86.lib"} 
		libdirs {HAL.."/lib","$(HAL)/lib"}
		includedirs { HAL.."/include","$(HAL)/include"}
	  
		configuration "Debug"
			defines { "DEBUG" }
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG" }
			symbols  "Off" 
		 

