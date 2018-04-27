kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj *.db output output.264

clean:
	-$(OS_RM) *.mk *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters *.cache *.db .* *.opendb  $(TARGET) $(TARGET_X86)
	-$(OS_RD) Debug Release bin obj obj0 obj1 .vs
