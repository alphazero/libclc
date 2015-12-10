// DOOST
/* ===-----------------------------------------------------------------------===
 * Copyright J Muhammad H 2015                                             3.14~
 * ===--------------------      ~!!! FRIEND !!!~     ------------------------===
 *                     ~!!! Ever Present Ever Near !!!~
 *           ~!!! Good Mind Good Expression Good Actualization !!!~
 *                       ~!!! The REAL The LOVING !!!~
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * This copyright and permission notice shall be included in all copies or any  
 * portions of this Software.                       
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */

#include <stdlib.h>
#include <stdio.h>

#include "libclc.h"
#include "libclc/inline.h"
#include "libclc/objects.h"

#include "util/option.h"
#include "util/time.h"

#include "bench_helpers.h"

/* --------------------------------------------------------------------- */
/* program options ------ */

typedef struct option_struct {
	unsigned int  iters;
	unsigned char type;
	unsigned char policy;
	unsigned char  op;
} option_t;

// options w/ default values
#define NONE_SELECTED '-'
static option_t option = { 1000000, 0, 0, 0 };

// corresponding cmdline options spec
static option_spec option_specs[] = {
	UINT_OPTION     ("i", "number of instrumented iterations", option.iters),
	CHAR_REQ_OPTION ("p", "cache-policy {L:lru, F:fifo, Q:2q}", option.policy),
	CHAR_REQ_OPTION ("o", "op {p:put, x:putx, r:putr, g:get, d:del}", option.op),
	END_OPTIONSPEC()
};

/* --------------------------------------------------------------------- */
/* funcs ---------------- */

int run (void);
int main(int argc, char ** argv) {
	int const r = parse_options(argc, argv, option_specs);
	if (r != 0) {
		emit_usage(option_specs);
		return -1;
	}
	return run ();
}

static clc_stat put_lru  (void*); 
static clc_stat putx_lru (void*); 
static clc_stat putr_lru (void*); 
static clc_stat get_lru  (void*); 
static clc_stat del_lru  (void*); 
#ifdef DONE
static clc_stat put_fifo (void*); 
static clc_stat putx_fifo(void*); 
static clc_stat putr_fifo(void*); 
static clc_stat get_fifo (void*); 
static clc_stat del_fifo (void*); 

static clc_stat put_2q   (void*); 
static clc_stat putx_2q  (void*); 
static clc_stat putr_2q  (void*); 
static clc_stat get_2q   (void*); 
static clc_stat del_2q   (void*); 
#endif
int run(void) {
	clc_stat (*bench[15]) (void*)  = {
		put_lru,  putx_lru,  putr_lru,  get_lru,  del_lru,
#ifdef DONE
		put_fifo, putx_fifo, putr_fifo, get_fifo, del_fifo,
		put_2q,   putx_2q,   putr_2q,   get_2q,   del_2q,
#endif
	};
	unsigned idx = 0;
	switch (option.op) {
		case 'p': /* 0 */
			break;
		case 'x':
			idx = 1;
			break;
		case 'r':
			idx = 2;
			break;
		case 'g': 
			idx = 3;
			break;
		case 'd':
			idx = 4;
			break;
		default:
			fprintf(stderr, "BUG - unknown cache op option '%c'\n", option.op);
			return -1;
	}

	switch (option.policy) {
		case 'L': /* 0 */
			break;
		case 'F':
			idx += 5;
			break;
		case 'Q':
			idx += 10;
			break;
		default:
			fprintf(stderr, "BUG - unknown policy %c\n", option.policy);
			return -1;
	}

	void * p;
	bench_alloc_and_init_m (p, 1024); /* returns on error */

	clc_stat const s = bench[idx](p); 

	free (p);
	return s;
}

#define bench_put0_m(fn, _info)\
{\
	uint64_t rec = 0x314ULL;\
	uint64_t mask = ~0ULL;\
	uint64_t rec0 = 0x0ULL;\
	uint8_t  rmeta = 0;\
	clc_stat s = 0;\
	uint64_t t0 = 0ULL;\
	t0 = time_usec();\
	for (unsigned i=0; i<option.iters; i++) {\
		rec +=1;\
		s = fn (p, rec, mask, &rec0, &rmeta);\
		if (clc_is_error_m(s)) {\
			clc_error_println_m(s, "push");\
			return s;\
		}\
	}\
	TIME_OPS2 (_info, t0, option.iters);\
}
#define bench_put_m(fn, _info)\
{\
	uint64_t rec = 0x314ULL;\
	uint64_t rec0 = 0x0ULL;\
	uint8_t  rmeta = 0;\
	clc_stat s = 0;\
	uint64_t t0 = 0ULL;\
	t0 = time_usec();\
	for (unsigned i=0; i<option.iters; i++) {\
		rec +=1;\
		s = fn (p, rec, &rec0, &rmeta);\
		if (clc_is_error_m(s)) {\
			clc_error_println_m(s, "push");\
			return s;\
		}\
	}\
	TIME_OPS2 (_info, t0, option.iters);\
}

// REVU: these are policy invariant TODO macro these in a header
static clc_stat put_lru (void * p) {
	bench_put_m (__clc_cache_put_lru,   "cache.put  LRU inline         ");
	bench_put_m (clc_cache_put_lru,     "cache.put  LRU function       ");
	bench_put_m (__clc.cache.lru.put,   "cache.put  LRU (obj) inline   ");
	bench_put_m (clc.cache.lru.put,     "cache.put  LRU (obj) function ");
	return 0;
}

static clc_stat putx_lru (void * p) {
	bench_put0_m (__clc_cache_putx_lru, "cache.putx LRU inline         ");
	bench_put0_m (clc_cache_putx_lru,   "cache.putx LRU function       ");
	bench_put0_m (__clc.cache.lru.putx, "cache.putx LRU (obj) inline   ");
	bench_put0_m (clc.cache.lru.putx,   "cache.putx LRU (obj) function ");
	return 0;
}

static clc_stat putr_lru (void * p) {
	bench_put0_m (__clc_cache_putr_lru, "cache.putr LRU inline         ");
	bench_put0_m (clc_cache_putr_lru,   "cache.putr LRU function       ");
	bench_put0_m (__clc.cache.lru.putr, "cache.putr LRU (obj) inline   ");
	bench_put0_m (clc.cache.lru.putr,   "cache.putr LRU (obj) function ");
	return 0;
}

static clc_stat get_lru (void * p) {
	return 0;
}

static clc_stat del_lru (void * p) {
	return 0;
}
