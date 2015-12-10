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
	unsigned char op_type;
} option_t;

// options w/ default values
#define NONE_SELECTED '-'
static option_t option = { 1000000, NONE_SELECTED };

// corresponding cmdline options spec
static option_spec option_specs[] = {
	UINT_OPTION     ("i", "number of instrumented iterations", option.iters),
	CHAR_REQ_OPTION ("o", "op {A:all, E:enqueue D:dequeue}", option.op_type),
	END_OPTIONSPEC()
};

/* --------------------------------------------------------------------- */
/* funcs ---------------- */

static int run (void);

int main(int argc, char ** argv) {
	int const r = parse_options(argc, argv, option_specs);
	if (r != 0) {
		emit_usage(option_specs);
		return -1;
	}
	return run ();
}

static clc_stat enqueue_dequeue (void* p);
static clc_stat enqueue (void* p);
static clc_stat dequeue (void* p);

static int run(void) {

	void * p;
	bench_alloc_and_init_m (p, 1024); /* returns on error */

	clc_stat s = 0;
	switch (option.op_type) {
		case 'A':
			s = enqueue_dequeue (p);
			break;
		case 'E':
			s = enqueue(p);
			break;
		case 'D':
			s = dequeue(p);
			break;
		default:
			fprintf(stderr, "BUG - unknown op_type %c\n", option.op_type);
			return -1;
	}

	free (p);
	return s;
}

#define enqueue_dequeue_m(p, pre, post, info)\
{\
	uint8_t rmeta = 0;\
	uint64_t rec = 0ULL;\
	clc_stat s = 0;\
	uint64_t t0 = time_usec();\
	unsigned opcnt = 0;\
	for(unsigned i=0; i<option.iters; i++) {\
		rec = 0x314;\
		for(;;){\
			s = bench_join_m(pre, post, enqueue)(p, rec, &rmeta);\
			if (clc_is_error_m(s)) {\
				clc_error_println_m(s, "enqueue");\
				return s;\
			}\
			if (s == CLC_FULL) break;\
			opcnt++;\
			rec <<= 1;\
		}\
		for(;;){\
			s = bench_join_m(pre, post, dequeue)(p, &rec, &rmeta);\
			if (clc_is_error_m(s)) {\
				clc_error_println_m(s, "dequeue");\
				return s;\
			}\
			if (s == CLC_EMPTY) break;\
			opcnt++;\
		}\
	}\
	TIME_OPS2 (info, t0, opcnt);\
}
static clc_stat enqueue_dequeue (void* p) {
	enqueue_dequeue_m (p, __clc_queue, _, "queue >>7 | <<7 inline        ");
	enqueue_dequeue_m (p, clc_queue, _,   "queue >>7 | <<7 function      ");
	return 0;
}

#define enqueue_m(p, pre, post, info)\
{\
	uint8_t rmeta = 0;\
	uint64_t rec = 0x314ULL;\
	clc_stat s = 0;\
	uint64_t t0 = time_usec();\
	unsigned opcnt = 0;\
	for(unsigned i=0; i<option.iters; i++) {\
		s = bench_join_m(pre, post, enqueue)(p, rec, &rmeta);\
		if (clc_is_error_m(s)) {\
			clc_error_println_m(s, "enqueue");\
			return s;\
		}\
		if (s == CLC_FULL) {\
			__clc_reset(p); /* not queue specific */\
			rec = 0x314;\
		}\
		opcnt++;\
		rec <<= 1;\
	}\
	TIME_OPS2 (info, t0, opcnt);\
}
static clc_stat enqueue (void * p) {
	enqueue_m (p, __clc_queue, _, "queue enqueue only inline     ");
	enqueue_m (p, clc_queue, _,   "queue enqueue only function   ");
	return 0;
}
// REVU: not sure how to actually test dequeue only without
// turning this bench into a major production. skip it for now.
static clc_stat dequeue (void * p) {
	return CLC_ENOTIMPL;
}
