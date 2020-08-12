#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-virtual"
	configurations { "Debug", "Release" }
	platforms { "Win32","x64"}
	project "hal-virtual"
		kind "StaticLib"
		files { 	"../src_virtual/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h"}
		includedirs { "../include", "$(NMPP)/include"}
		targetdir ("../lib")
	  
		--configuration "Debug"
		--targetsuffix ("d")
        --defines { "DEBUG","VIRTAUL_BOARD" }
        --symbols  "On" 
		-- 
		--configuration "Release"
        -- defines { "NDEBUG","VIRTAUL_BOARD" }
        -- symbols  "Off" 
		
	 
		configuration {"Debug","Win32"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG","VIRTUAL_BOARD"}
			symbols  "On" 
		
		configuration {"Release","Win32"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG","VIRTUAL_BOARD"}
				symbols  "Off" 
		
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG","VIRTUAL_BOARD"}
			symbols  "On" 
		
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG","VIRTUAL_BOARD"}
			symbols  "Off" 