ifndef OS_RM
 	OS_RM    =rm -f -r
  	OS_RD    =rm -d -r
endif

kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj *.db output output.264 

clean:
	-$(OS_RD) obj
	-$(OS_RM) Release/*.o 
	-$(OS_RM) Debug/*.o 
	-$(OS_RM) nmc4vars_win.mk *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters *.cache *.db .* *.opendb  $(TARGET) $(TARGET0) $(TARGET1) $(TARGET_X86)
	-$(OS_RD) Debug Release bin obj obj0 obj1 .vs
