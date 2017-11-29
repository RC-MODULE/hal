#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-virtual-x86"
   configurations { "Debug", "Release" }

	project "hal-virtual-x86"
      kind "StaticLib"
      files { "../src_virtual/*.*","../include/*.h"}
	  includedirs { "../include"}
	  targetdir ("../lib")
	  
	  configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		
	 