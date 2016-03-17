
CC   = gcc-5

STD  = -std=c11
WARN = -Wall -Werror -pedantic -Wno-sign-compare
OPT  = -O3 #-march=native

CLC_INCLUDE = ../include
# for the expected include pattern include "util/header.h"
CLC_UTIL_INCLUDE = ..
INCLUDES = -I $(CLC_INCLUDE) -I $(CLC_UTIL_INCLUDE)

DEFINES = -DCLC_ASSERT_IN_ARGS -DCLC_ASSERT_OUT_ARGS
#DEFINES = 

# begin -- lifted from Redis makefile
CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[35;3m"
DELCOLOR="\033[31;2m"
BINCOLOR="\033[37;2m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"
QUIET_CC    = @printf '    %b %b\n' $(CCCOLOR)cc$(ENDCOLOR) $(SRCCOLOR)$@$(ENDCOLOR) 1>&2;
# end -- lifted from Redis makefile

# final CC flags
CFLAGS = $(STD) $(OPT) $(WARN) $(DEFINES) $(INCLUDES)
CLC_CC = $(QUIET_CC)$(CC)

CLC_UTIL_IMPLS = $(wildcard ../util/*.c)
CLC_TEST_IMPLS = $(wildcard ../test/*.c)
CLC_BENCH_IMPLS= $(wildcard ../bench/*.c)

CLC_OBJS       = $(wildcard ../src/*.o)
CLC_UTIL_OBJS  = $(wildcard ../util/*.o)
CLC_TEST_OBJS  = $(wildcard ../test/*.o)
CLC_BENCH_OBJS = $(wildcard ../bench/*.o)

libclc: 
	@make -C ../src 

util: $(CLC_UITL_IMPLS)
	@make -C ../util clc_util.o

bench: $(CLC_UITL_IMPLS)
	@make -C ../bench all

# /// cleanup ///////////////////////////////////////////

.PHONY: clean cleanall
DISPOSABLES = $(wildcard *.o *.so *.dylib)
.SILENT:
	( for file in ${DISPOSABLES} ; do rm $$file; done )
clean: .SILENT 
	@printf '    %b %b' $(DELCOLOR)rm$(ENDCOLOR) 1>&2;
	@printf '%b ' $(BINCOLOR)$(DISPOSABLES)$(ENDCOLOR) 1>&2;
	@printf '\n'

