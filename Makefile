ifeq ($(OS),Windows_NT)
  SHELL   = cmd
  OS_WHICH= where
  OS_RM   = del /Q /A:A
  OS_RD   = rd /Q /S
  OS_SET_ENV = setx  /m 
else
  OS_WHICH= which
  OS_RM   = rm -f -r
  OS_RD   = rm -f -r
  OS_SET_ENV = export 
endif

setenv:
	$(OS_SET_ENV) HAL "$(realpath .)"
#setenv -ua HAL "$(realpath .)"