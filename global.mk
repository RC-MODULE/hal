GNUWIN32  ?=c:/gnuwin32/bin
#NMPP       =$(ROOT)
DEVKIT    ?=$(ROOT)/deps/dev-kit-master
ARMTOOLCH ?=$(DEVKIT)/arm-rcm-linux-gnueabihf
NEURO     ?=$(DEVKIT)/nmsdk
VSHELL32  ?=$(DEVKIT)/vshell32
EASYNMC   ?=$(DEVKIT)/nmc-utils-0.1.1/libeasynmc-nmc
MC5103    ?=$(DEVKIT)/mc5103sdk
MC7601    ?=$(DEVKIT)/mc7601sdk
MB7707    ?=$(DEVKIT)/mb7707sdk
MC12101   ?=$(DEVKIT)/mc12101sdk


ifeq ($(OS),Windows_NT)
  SHELL = cmd
  WHICH = where
  RMALL = del /Q /A:A                  
  RMDIR = rmdir /Q /S
  MKDIR = mkdir  
  CPDIR = xcopy /e /y
  CP    = copy                
  MV    = move                 
  ECHO  = echo  
  OS_FIND  = gfind
  OS_DIFF  = diff
  OS_SCP   = pscp
  OS_CAT   = cat
  OS_CP    = cp
  #OS_RD    = rd /Q /S 
  OS_RM    = rm -f -r
  OS_RD    = rm -d -r
  define OSX
	$(subst /,\,$(1))
  endef  
else
  WHICH = which
  RMALL = rm -f                 
  RMDIR = rm -fr               
  MKDIR = mkdir -p             
  CPDIR = cp -r                
  CP    = cp                   
  MV    = mv                   
  ECHO  = echo    
  OS_DIFF  = diff   
  OS_SCP   = scp
  OS_RM    = rm -f -r
  RM       = rm -f -r
  OS_RD    = rm -d -r
  OS_FIND  = find
  OS_CAT   = cat
  OS_CP    = cp  
  
  define OSX
	$(subst \,/,$(1))
  endef  
endif

PLATFORMS = x86 mc12101_nmpu0 mc12101_nmpu0_gcc  mc12101_nmpu1 mc12101_nmpu1_gcc  mc5103 mc7601 mb7707
TARGET1=mc12101_nmc1
TARGET2=x86

define newline


endef	

define BACKSLASH
	$(subst /,\,$(1))
endef

ifdef VS140COMNTOOLS
VS_VER = vs2015
else ifdef VS80COMNTOOLS
VS_VER = vs2005
else 
$(warning NO VISUAL STUDIO INSTALLED. X86-BUILD ABORTED)
VS_VER = 
endif 


 