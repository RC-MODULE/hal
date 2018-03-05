kill:
	-$(OS_RM) *.vcproj *.sln *.vcxproj *.db output output.264

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters *.db .* *.opendb 
	-$(OS_RD) Debug Release bin obj .vs
