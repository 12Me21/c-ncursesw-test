srcs:= string main gap-buffer term buffer

output:= e

# compiler flags
CFLAGS+= -Wextra -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Werror=implicit-function-declaration -ftabstop=3 -Wno-missing-field-initializers -Wno-char-subscripts -Wno-parentheses -Werror=incompatible-pointer-types -Wdouble-promotion -Wno-missing-braces -Wint-conversion -Wno-sign-compare -g -std=c11
# libs to include when linking (passed as -l<name> flags)
libs:= ncursesw

#first: $(output)
#	mkfifo .junk/err
#	./e 2>.junk/err

######################################################################

include .Nice.mk
