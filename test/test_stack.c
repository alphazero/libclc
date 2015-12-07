// !!! FRIEND !!!
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
#include <stdbool.h>

#include "libclc.h"
#include "libclc/inline.h"
#include "libclc/objects.h"

#include "testing.h"

/* NOTE: tests assume correct clc.unit ops */

///////////////////////////////////////////////////////////////////////////
/// CLC 0x7 Stack ops /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef unsigned optype_t;
#define PUSH    1
#define POP     2
#define LEN     3
// REVU: TODO consolidate this
#define _ORD    777 // dummy op dumps container to stderr
#define _PRT    999 // dummy op dumps container to stderr
#define _END    0

/* spec a op step in the test sequence */
// REVU: TODO consolidate this
typedef struct op_spec {
	optype_t op;
	uint64_t in_arg  [1];
	uint64_t out_arg [1];
	uint8_t  out_rmeta;
	clc_stat stat;
} op_spec_t;

static void emit_op_spec (const op_spec_t s);

static void emit_op_spec (const op_spec_t s) {
	char * opname;
	switch (s.op) {
		case PUSH:
			opname = "PUSH";
			break;
		case POP:
			opname = "POP";
			break;
		case LEN:
			opname = "LEN";
			break;
		default:
			opname = "????";
	}
	fprintf(stderr, "\top-spec: %4s IN:{0x%llx} OUT:{0x%llx, 0x%02x} stat:%d\n", 
			opname, s.in_arg[0], s.out_arg[0], s.out_rmeta, s.stat);
	return;
}

static 
testrep_t const* test_container (char const*, struct clc_stack_op, const op_spec_t[]);

static 
testrep_t const * test_container (char const * name, struct clc_stack_op stack, 
		const op_spec_t test_sequence[])
{
	testrep_t * testrep;
	void *p = calloc (1024, sizeof(struct clc_unit));

	clc_stat const rs0 = __clc.unit.init(p); 
	if (rs0 != CLC_OK) {
		clc_error_println_m (rs0, "__clc_init(p)");
		testrep = failed (name, "expected CLC_OK");
		goto done;
	}

	unsigned n = 0;
	bool done = false;
	while(!done) {
		const op_spec_t spec = test_sequence[n];
		clc_stat rs = 0;
		uint64_t   rec_in = spec.in_arg[0];
		uint64_t   rec_out = 0; 
		uint8_t    rmeta_out = 0;
		switch(spec.op) {
			case PUSH:
				rs = stack.push (p, rec_in, &rmeta_out);
				break;
			case POP:
				rs = stack.pop (p, &rec_out, &rmeta_out);
				break;
			case LEN:
				rs = clc.unit.len (p, &rmeta_out);
				break;
			/* -- meta commands - not canonical ops -- */
			case _ORD: 
				__clc_debug_inorder ( p );
				rs = CLC_OK; 
				break;
			case _PRT: 
				__clc_debug ( p );
				rs = CLC_OK; 
				break;
			case _END:
				done = true;
				break;
			default:
				fprintf(stderr, "OP:%d\n", spec.op);
				testrep = failed (name, "TEST-BUG: unknown OP!");
				goto done;
		}
		if (rs != spec.stat) {
			fprintf(stderr, "\tspec[%d].stat: have:%d expect:%d\n", n, rs, spec.stat);
			emit_op_spec(spec);
			testrep = failed (name, "test result not per spec");
			goto done;
		}
		if (spec.op != LEN) {
			if (rec_out != spec.out_arg[0]) {
				fprintf(stderr, "\tspec[%d].item: have:0x%016llx expect:0x%016llx\n", n, rec_out, spec.out_arg[0]);
				emit_op_spec(spec);
				testrep = failed (name, "test result not per spec");
				goto done;
			}
		} else {
			if (rmeta_out != (uint8_t) spec.out_rmeta) {
				fprintf(stderr, "\tspec[%d].len: have:%d expect:%d\n", n, rmeta_out, spec.out_rmeta);
				emit_op_spec(spec);
				testrep = failed (name, "test result not per spec");
				goto done;
			}
		}
		n++;
	}

	testrep = passed (name);

done: free(p);

	  return testrep;
}

///////////////////////////////////////////////////////////////////////////
/// Test Sequence /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

const op_spec_t test_sequence_stack [] = {
/* check: general fifo queue operations                     */
/*    0 */ { PUSH,    { 0xabcd }, { 0,     }, 6, CLC_OK },
/*    1 */ { LEN,     { 0 },      { 0,     }, 1, CLC_OK },
/*    2 */ { POP,     { 0 },      { 0xabcd }, 6, CLC_OK },
/*    3 */ { LEN,     { 0 },      { 0,     }, 0, CLC_OK },
/*    4 */ { POP,     { 0 },      { 0,     }, 0, CLC_EMPTY },
/*    5 */ { PUSH,    { 0xabc1 }, { 0,     }, 6, CLC_OK },
/*    6 */ { PUSH,    { 0xabc2 }, { 0,     }, 5, CLC_OK },
/*    7 */ { PUSH,    { 0xabc3 }, { 0,     }, 4, CLC_OK },
/*    8 */ { PUSH,    { 0xabc4 }, { 0,     }, 3, CLC_OK },
/*    9 */ { PUSH,    { 0xabc5 }, { 0,     }, 2, CLC_OK },
/*   10 */ { PUSH,    { 0xabc6 }, { 0,     }, 1, CLC_OK },
/*   11 */ { PUSH,    { 0xabc7 }, { 0,     }, 0, CLC_OK },
/*   12 */ { PUSH,    { 0x1234 }, { 0,     }, 0, CLC_FULL },
/*   13 */ { LEN,     { 0 },      { 0,     }, 7, CLC_OK },
//           { _ORD,    { 0 },      { 0      }, 0, 0 },    
/*   14 */ { POP,     { 0 },      { 0xabc7 }, 0, CLC_OK },
/*   15 */ { POP,     { 0 },      { 0xabc6 }, 1, CLC_OK },
/*   16 */ { POP,     { 0 },      { 0xabc5 }, 2, CLC_OK },
/*   17 */ { POP,     { 0 },      { 0xabc4 }, 3, CLC_OK },
/*   18 */ { POP,     { 0 },      { 0xabc3 }, 4, CLC_OK },
/*   19 */ { POP,     { 0 },      { 0xabc2 }, 5, CLC_OK },
/*   20 */ { POP,     { 0 },      { 0xabc1 }, 6, CLC_OK },
/*   21 */ { POP,     { 0 },      { 0      }, 0, CLC_EMPTY },
/*   22 */ { LEN,     { 0 },      { 0      }, 0, CLC_OK },
/*   23 */ { _END,    { 0 },      { 0      }, 0, 0 } 
};


/* --------------------------------------------------------------------- */
/* void Tests         */

static testrep_t const * test_stack (void);
static testrep_t const * test_stack_inline (void);
static testrep_t const * test_stack_inline (void) 
{
	/* NOTE: using __clc object */
	return test_container ("clc_stack_op inline", __clc.stack, test_sequence_stack);
}
static testrep_t const * test_stack (void) 
{
	/* NOTE: using clc object */
	return test_container ("clc_stack_op", clc.stack, test_sequence_stack);
}

test_fn test_stack_ops[] = {
	test_stack_inline,
	test_stack,
	0,
};

/* --------------------------------------------------------------------- */
/* run the test suites -- */

int main (int argc, char **argv) {
	runsuite("clc.stack", test_stack_ops);
	return 0;
}

