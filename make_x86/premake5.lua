#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-x86"
   configurations { "Debug", "Release" }

	project "hal-x86"
      kind "StaticLib"
      files { "../src_x86/*.*","../include/*.h"}
	  includedirs { "../include"}
	  targetdir ("../lib")
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		
	 