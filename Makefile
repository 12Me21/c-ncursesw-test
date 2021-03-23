srcs:= gap-buffer string main

output:= e

# compiler flags
CFLAGS+= -Wextra -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Werror=implicit-function-declaration -ftabstop=3 -Wno-missing-field-initializers -Wno-char-subscripts -Wno-parentheses -Werror=incompatible-pointer-types -Wdouble-promotion -Wno-missing-braces -Wint-conversion -Wno-sign-compare -Os -s
# libs to include when linking (passed as -l<name> flags)
#libs:= X11 lua5.3

#(haven't tested this!)
#srcdir:= src

#this will put the files in .junk/whatever
#junkdir:= whatever

######################################################################

include .Nice.mk
