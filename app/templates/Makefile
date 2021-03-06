#########################################################################
#                                                                       #
# This Makefile recursively allows to :                                 #
# configure,build,run or test given tests or examples                   #
# from current folder                                                   #
#                                                                       #
# S.Mushkaev  (RC)Module 2015                                           #
#                                                                       #
#                                                                       #
#########################################################################
# USAGE:
#
# /> make configure PLATFORMS="mc5103 emu6405 mc12101"
#  Recursively generates Makefiles for all tests/examples for mc5103,emu6405,mc12101 platforms 
#
# /> make configure 
#  Recursively generates Makefiles for all tests/examples for default platform list defined by $(PLATFORMS) in global.mk
#
# /> make 
#  Recursively makes(builds) all Makefiles  from current folder 
#
# /> make run PLATFORMS="mc5103 emu6405"
#  Recursively runs all tests/examples from current folder on mc5103 board and emulator
#
# /> make run 
#  Recursively runs all tests/examples from current folder on all platforms from $(PLATFORMS) list defined in global.mk 
#
# /> make test TARGET1=mc5103 TARGET2=vs8
#  Recursively builds and runs all tests from current folder  
#  Then two exitcodes fo $(TARGET1) and $(TARGET2) platforms are compared
#  if exitcodes are identical - test is passed
#
# /> make test 
#  Recursively builds and runs all tests from current folder with default $(TARGET1) and $(TARGET2) defined in global.mk 
#
# /> make clean 
#  Recursive cleanup (Remove all builds in /make_* folders)
#
# /> make kill
#  Recursive clean with Makefile removal of all /make_* folders


include $(ROOT)/global.mk

# override settings by custom list in local.mk
-include $(PATH2LOCAL)/local.mk

ifndef TEMPLATE_DIR
$(error *** ERROR: 'TEMPLATE_DIR' environment variable is not defined!  )
endif 


# get list of all sub-folders
WORK_DIRS=$(subst /,,$(wildcard */))


WORK_DIRS := $(filter-out $(SKIP_DIRS),$(WORK_DIRS))

# for each sub-folder try to start Makefile with (build/test/run/clean targets) 
#.default 
test run clean: 
	@echo ***** build *************
	$(foreach dir,$(WORK_DIRS), $(MAKE) -C $(dir) $(MAKECMDGOALS) 	$(newline))
	@echo Command '$(MAKECMDGOALS)' completed!
	
# for each sub-folder try to create Makefile
configure: $(addsuffix /Makefile,$(WORK_DIRS))
	@echo ***** configure **********
	$(foreach dir,$(WORK_DIRS), $(MAKE) -C ./$(dir) $(MAKECMDGOALS) $(newline))
	
	
#recursive delete all sub-makefiles
kill: 
	$(foreach mak,$(wildcard */Makefile), $(MAKE) -C $(dir $(mak)) kill				$(newline))
	$(foreach mak,$(wildcard */Makefile), -rm   $(mak) 								$(newline))
	@echo Command 'kill' completed!
	
define configure_makefile
$(1)/Makefile:
	-$(OS_FIND) $(1)/local.mk && echo PATH2LOCAL=.> $(1)/Makefile
	-$(OS_FIND) $(1)/local.mk || echo PATH2LOCAL=$(PATH2LOCAL)/..> $(1)/Makefile
	-$(OS_FIND) $(1)/*.cpp || echo ROOT=../$(ROOT)| $(OS_CAT) - "$(TEMPLATE_DIR)/Makefile" >> $(1)/Makefile
	-$(OS_FIND) $(1)/*.cpp && echo ROOT=../$(ROOT)| $(OS_CAT) - "$(TEMPLATE_DIR)/sometest/Makefile" >> $(1)/Makefile
endef
	
$(foreach dir,$(WORK_DIRS), $(eval $(call configure_makefile,$(dir))) )	


define newline


endef	
