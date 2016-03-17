# DOOST

PRODUCTS = \
	test_unit  bench_unit  \
	test_stack bench_stack \
	test_queue bench_queue \
	test_cache bench_cache 

.PHONY: default all \
	testing.o \
	$(PRODUCTS) \
	run-tests \
   	cleanall

default: run-tests 

include ../clc.mk

all: $(PRODUCTS) 

CFLAGS_TEMP = $(CFLAGS) -mmmx

testing.o: testing.h testing.c
	$(CLC_CC) $(CFLAGS_TEMP) -c testing.c

#test_unit: libclc util testing.o test_unit.c
#	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
#		testing.o test_unit.c -o test_unit 

test_stack: libclc util testing.o test_stack.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o test_stack.c -o test_stack 

test_queue: libclc util testing.o test_queue.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o test_queue.c -o test_queue 

test_cache: libclc util testing.o test_cache.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o test_cache.c -o test_cache 

#bench_unit: libclc util bench_unit.c
#	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
#		testing.o bench_unit.c -o bench_unit 

bench_stack: libclc util bench_stack.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o bench_stack.c -o bench_stack 

bench_queue: libclc util bench_queue.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o bench_queue.c -o bench_queue 

bench_cache: libclc util bench_cache.c
	$(CLC_CC) $(CFLAGS_TEMP) $(CLC_OBJS) $(CLC_UTIL_OBJS) \
		testing.o bench_cache.c -o bench_cache 

BENCH_ITERS = 5000000
run-tests: $(PRODUCTS) 
	@echo 
	@./test_stack
	@./test_queue
	@./test_cache
	@echo '*** TODO: complete test_cache ***'
	@echo '*** TODO: test_list  ***'
	@echo
	@echo '-- bench clc_stack --'
	@./bench_stack -i $(BENCH_ITERS) -o A ; 
	@./bench_stack -i $(BENCH_ITERS) -o P -i 70000000; echo
	@echo '-- bench clc_queue --'
	@./bench_queue -i $(BENCH_ITERS) -o A ;
	@./bench_queue -i $(BENCH_ITERS) -o E -i 70000000; echo
	@echo '-- bench clc_cache LRU --'
	@./bench_cache -i $(BENCH_ITERS) -p L -o p ; echo
	@./bench_cache -i $(BENCH_ITERS) -p L -o x ; echo
	@./bench_cache -i $(BENCH_ITERS) -p L -o r ; echo
	@./bench_cache -i $(BENCH_ITERS) -p L -o g ; echo
	@./bench_cache -i $(BENCH_ITERS) -p L -o d ; echo
	@echo '-- bench clc_cache FIFO (function) --'
	@echo 'TODO!'

.PHONY: cleanall
cleanall: clean
	rm -f test_unit
	rm -f test_stack 
	rm -f test_queue
	rm -f test_cache
	rm -f bench_stack
	rm -f bench_queue
	rm -f bench_cache

