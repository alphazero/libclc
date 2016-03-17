# DOOST

.PHONY: default

default: clc_util.o

include ../clc.mk

%.o: %.c %.h 
	$(CLC_CC) $(CFLAGS) -c $<

UTIL_OBJS    = $(patsubst %.c,%.o,$(wildcard *.c))
clc_util.o: $(UTIL_OBJS)
	@printf ''

cleanall: clean
	@echo
