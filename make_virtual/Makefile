ROOT = ..
include $(ROOT)/global.mk
#-include ../local.mk

.SUFFIXES:


HOST      = hal-virtual-x86

ALL:  $(HOST)
	
$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj


clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj obj0 obj1
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	

$(HOST).vcproj vs2005:	
	premake5 vs2005

