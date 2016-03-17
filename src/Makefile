# DOOST

default: libclc.o

include ../clc.mk

CLC_HEADERS=$(wildcard $(CLC_INCLUDE)/*.h)
CLC_INLINE_HEADERS=$(wildcard $(CLC_INCLUDE)/inline/*.h)
CLC_IMPLS = $(wildcard *.c)

libclc.o: $(CLC_HEADERS) $(CLC_INLINE_HEADERS) $(CLC_IMPLS)
	$(CLC_CC) $(CFLAGS) -c $(CLC_IMPLS)

.PHONY: cleanall
cleanall: clean
	@echo
