#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-virtual-x86"
   configurations { "Debug", "Release" }

	project "hal-virtual-x86"
      kind "StaticLib"
      files { 	"../src_virtual/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h"}
	  includedirs { "../include"}
	  targetdir ("../lib")
	  
	  configuration "Debug"
		targetsuffix ("d")
        defines { "DEBUG","VIRTAUL_BOARD" }
        symbols  "On" 
		 
      configuration "Release"
         defines { "NDEBUG","VIRTAUL_BOARD" }
         symbols  "Off" 
		
	 