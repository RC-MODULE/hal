#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc12101"
   configurations { "Debug", "Release" }
		
	
		
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101(gcc)"
      	kind "Makefile"
		files { "../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y -f    Makefile $(CPCONV)"}
			rebuildcommands {"make -B DEBUG=y -f Makefile $(CPCONV)"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make -f Makefile	$(CPCONV)"}
			rebuildcommands {"make -B -f Makefile 	$(CPCONV)"}
			cleancommands   {"make clean"}		   
	
	-- NeuroMatrix project ---------------------------------
	project "hal-mc12101(legacy)"
      	kind "Makefile"
		files { "../src_1879vm6ya/*.cpp",
				"../src_1879vm6ya/*.c",
				"../src_1879vm6ya/*.asm",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y legacy $(CPCONV)"}
			rebuildcommands {"make -B DEBUG=y legacy $(CPCONV)"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make legacy	$(CPCONV)"}
			rebuildcommands {"make -B legacy 	$(CPCONV)"}
			cleancommands   {"make clean"}		   
	
	