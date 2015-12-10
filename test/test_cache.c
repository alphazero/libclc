/* !!! DOOST !!! */
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

#include <libc.h>
#include "testing.h"
#include <stdbool.h>

#include "libclc.h"
#include "libclc/inline.h"
#include "libclc/objects.h"

///////////////////////////////////////////////////////////////////////////
/// CLC-0X7R Cache Ops & Policy Tests Framework ///////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef unsigned optype_t;
#define PUT     1
#define PUTX    2
#define PUTR    3
#define GET     4
#define DEL     5
// REVU: TODO consolidate this
#define LEN     16 
#define CLR     17 
#define _PRT    999 // dummy op dumps container to stderr
#define _ORD    777 // dummy op dumps container to stderr
#define _END    0

/* spec a op step in the test sequence */
typedef struct op_spec {
	optype_t op;
	uint64_t in_arg[2];
	uint64_t out_arg[1];
	uint8_t  out_rmeta;
	clc_stat stat;
} op_spec_t;

void emit_op_spec (const op_spec_t s);
void emit_op_spec (const op_spec_t s) {
	char * opname;
	switch (s.op) {
		case PUT:
			opname = "PUT";
			break;
		case PUTX:
			opname = "PUTX";
			break;
		case PUTR:
			opname = "PUTR";
			break;
		case GET:
			opname = "GET";
			break;
		case DEL:
			opname = "DEL";
			break;
		case LEN:
			opname = "LEN";
			break;
		case CLR:
			opname = "CLR";
			break;
		case _PRT: 
			opname = "_PRT";
			break;
		case _END:
			opname = "_END";
			break;
		default:
			opname = "????";
	}
	fprintf(stderr, "\top-spec: %4s IN:{0x%llx, 0x%llx} OUT:{0x%llx, 0x%02x} stat:%d\n", 
			opname, s.in_arg[0], s.in_arg[1], s.out_arg[0], s.out_rmeta, s.stat);
	return;
}

static 
testrep_t const* test_container (char const*, struct clc_cache_op, const op_spec_t[]);

static 
testrep_t const * test_container (char const * name, struct clc_cache_op cache, 
		const op_spec_t test_sequence[])
{
	testrep_t * testrep;
	void *p = calloc (1024, sizeof(struct clc_unit));

	clc_stat const rs0 = __clc_init(p); 
	if (rs0 != CLC_OK) {
		clc_error_println_m (rs0, "__clc_init(p)");
		testrep = failed (name, "expected CLC_OK");
		goto done;
	}

	unsigned n = 0;
	bool done = false;
	while (!done){
		const op_spec_t spec = test_sequence[n]; 
		clc_stat rs = 0;
		uint64_t rec_in = spec.in_arg[0];
		uint64_t mask_in = spec.in_arg[1];
		uint64_t rec_out = 0;
		uint8_t  rmeta_out = 0;
		switch (spec.op) {
			case PUT:
				rs = cache.put (p, rec_in, &rec_out, &rmeta_out);
				break;
			case PUTX:
				rs = cache.putx (p, rec_in, mask_in, &rec_out, &rmeta_out);
				break;
			case PUTR:
				rs = cache.putr (p, rec_in, mask_in, &rec_out, &rmeta_out);
				break;
			case GET:
				rs = cache.get  (p, rec_in, mask_in, &rec_out, &rmeta_out);
				break;
			case DEL:
				rs = cache.del  (p, rec_in, mask_in, &rec_out, &rmeta_out);
				break;
			case LEN:
				rs = __clc_len (p, &rmeta_out);
				break;
			case CLR:
				rs = __clc_clear ( p );
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
		fprintf(stderr, "** n:[%d] rs:%d rmeta:%d\n", n, rs, rmeta_out);
		if (rs != spec.stat) {
			fprintf(stderr, "\tspec[%d].stat: have:%d expect:%d\n", n, rs, spec.stat);
			emit_op_spec(spec);
			testrep = failed (name, "test result not per spec");
			goto done;
		}
		if (spec.op != LEN) {
			if (rec_out != spec.out_arg[0]) {
				printf("\tspec[%d].k0: have:0x%016llx expect:0x%016llx\n", n, rec_out, spec.out_arg[0]);
				emit_op_spec(spec);
				return failed (name, "test result not per spec");
			}
		} 
//		else { 
			if (rmeta_out != (uint8_t) spec.out_rmeta) {
				fprintf(stderr, "\tspec[%d].rmeta: have:%d expect:%d\n", n, rmeta_out, spec.out_rmeta);
				emit_op_spec(spec);
				testrep = failed (name, "test result not per spec");
				goto done;
			}
//		}
skip_check:		n++;
	}

	testrep = passed (name);

done: free(p);
	  return testrep;
}

/* --------------------------------------------------------------------- */
/* clc_cache Policy Tests  */

///////////////////////////////////////////////////////////////////////////
/// LRU Test Sequence /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// For all tests, we'll use the following mask.
static uint64_t const mask = 0xff0000;

/* note that we use v0 for GET/DEL whereas API docs call it 'val' */
const op_spec_t test_sequence_LRU [] = {
// seqnum            rec,      mask,     rec0,       rmeta, stat 
// TEST zero value record - error CLC_ERECORD
/*    0 */ { PUT,  { 0x0     , 0    }, { 0        }, 0  , CLC_ERECORD   }, 
// TEST it is OK to PUT the same record repeatedly
/*    1 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 6  , CLC_OK        }, 
/*    2 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 5  , CLC_OK        }, 
/*    3 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 4  , CLC_OK        }, 
/*    4 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 3  , CLC_OK        }, 
/*    5 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 2  , CLC_OK        }, 
/*    6 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 1  , CLC_OK        }, 
/*    7 */ { PUT,  { 0x0a00a0, 0    }, { 0        }, 0  , CLC_OK        }, 
// TEST we should be at full capacity at this point
/*    8 */ { PUT,  { 0x0a00a0, 0    }, { 0x0a00a0 }, 6  , CLC_REMOVED   }, 
// TEST ok DEL the rest but first check for CLC_ESELECTOR on zero key
//      + spot check force NOTFOUND with a wrong selector
/*    9 */ { DEL  ,{ 0,        0    }, { 0        }, 0  , CLC_ESELECTOR },
/*   10 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 6  , CLC_OK        },
/*   11 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 0  , CLC_OK        },
/*   12 */ { DEL  ,{ 0xcc0000, mask }, { 0        }, 0  , CLC_NOTFOUND  },
/*   13 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 1  , CLC_OK        },
/*   14 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 2  , CLC_OK        },
/*   15 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 3  , CLC_OK        },
/*   16 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 4  , CLC_OK        },
/*   17 */ { DEL  ,{ 0x0a0000, mask }, { 0x0a00a0 }, 5  , CLC_OK        },
// TEST verify LEN is 0
/*   18 */ { LEN  ,{ 0,        0    }, { 0        }, 0  , CLC_OK        },
/*   19 */ { PUTX, { 0xa10000, mask }, { 0        }, 5  , CLC_OK        }, 
// TEST verify PUTX same key diff rec is DUP
/*   20 */ { PUTX, { 0xa100a0, mask }, { 0xa10000 }, 5  , CLC_DUPLICATE }, 
/*   21 */ { PUTX, { 0xa20001, mask }, { 0        }, 4  , CLC_OK        }, 
/*   22 */ { PUTX, { 0xa30002, mask }, { 0        }, 3  , CLC_OK        }, 
/*   23 */ { PUTX, { 0xa40003, mask }, { 0        }, 2  , CLC_OK        }, 
/*   24 */ { PUTX, { 0xa300a0, mask }, { 0xa30002 }, 3  , CLC_DUPLICATE }, 
/*   25 */ { PUTX, { 0xa50004, mask }, { 0        }, 1  , CLC_OK        }, 
/*   26 */ { PUTX, { 0xa60005, mask }, { 0        }, 0  , CLC_OK        }, 
/*   27 */ { PUTX, { 0xa70006, mask }, { 0        }, 6  , CLC_OK        }, 
// GET a record (promoted to LRU) then check remove order
/*   28 */ { GET,  { 0xa70006, mask }, { 0xa70006 }, 6  , CLC_OK        }, 
// GET a record (promoted to LRU) then check remove order
/*   29 */ { GET,  { 0xa40000, mask }, { 0xa40003 }, 2  , CLC_OK        }, 
/*   30 */ { PUT,  { 0xcc00a0, 0    }, { 0xa10000 }, 5  , CLC_REMOVED   }, 
/*   31 */ { PUT,  { 0xcc00a0, 0    }, { 0xa20001 }, 4  , CLC_REMOVED   }, 
/*   32 */ { PUT,  { 0xcc00a0, 0    }, { 0xa30002 }, 3  , CLC_REMOVED   }, 
/*   33 */ { PUT,  { 0xcc00a0, 0    }, { 0xa50004 }, 1  , CLC_REMOVED   }, 
/*   34 */ { PUT,  { 0xcc00a0, 0    }, { 0xa60005 }, 0  , CLC_REMOVED   }, 
/*   35 */ { PUT,  { 0xcc00a0, 0    }, { 0xa70006 }, 6  , CLC_REMOVED   }, 
/*   36 */ { PUT,  { 0xcc00a0, 0    }, { 0xa40003 }, 2  , CLC_REMOVED   }, 
/*   -- */ { _ORD, { 0       , 0    }, { 0        }, 0  , 0             },
/*   37 */ { GET,  { 0xa70006, mask }, { 0        }, 0  , CLC_NOTFOUND  }, 
/*   -- */ { _ORD, { 0       , 0    }, { 0        }, 0  , 0             },
#if DONE
/*    1 */ { PUT,  { 0x0b, 0x00b0 }, { 0,    0      }, CLC_OK },
/*    2 */ { PUT,  { 0x0c, 0x00c0 }, { 0,    0      }, CLC_OK },
/*    3 */ { PUTX, { 0x0c, 0x00cc }, { 0,    0x00c0 }, CLC_EDUPKEY },
/*    4 */ { GET,  { 0x0f },         { 0,    0      }, CLC_NOTFOUND },
/*    5 */ { PUT,  { 0x0d, 0x00d0 }, { 0,    0      }, CLC_OK },
/*    6 */ { PUT,  { 0x0e, 0x00e0 }, { 0,    0      }, CLC_OK },
/*    7 */ { PUT,  { 0x0f, 0x00f0 }, { 0,    0      }, CLC_OK },
/*    8 */ { PUT,  { 0x1a, 0x01a0 }, { 0,    0      }, CLC_OK },
/*    9 */ { GET,  { 0x0f },         { 0,    0x00f0 }, CLC_OK },       
/*   10 */ { PUT,  { 0x1b, 0x01b0 }, { 0x0a, 0x00a0 }, CLC_REMOVED },
/*   11 */ { GET,  { 0x0c },         { 0,    0x00c0 }, CLC_OK },       
/*   12 */ { PUT,  { 0x1c, 0x01c0 }, { 0x0b, }, CLC_REMOVED },
/*   13 */ { PUT,  { 0x1d, 0x01d0 }, { 0x0d, 0x00d0 }, CLC_REMOVED },
/*   14 */ { GET,  { 0x0e },         { 0,    0x00e0 }, CLC_OK },       
/*   15 */ { PUT,  { 0x1e, 0x01e0 }, { 0x1a, 0x01a0 }, CLC_REMOVED },
/*   16 */ { PUT,  { 0,    0x01e0 }, { 0,    0      }, CLC_EINVKEY }, // once is enough
/*   17 */ { DEL,  { 0x0f},          { 0,    0x00f0 }, CLC_OK },  
/*   18 */ { PUT,  { 0x1f, 0x01f0 }, { 0,    0      }, CLC_OK },     
/*   19 */ { DEL,  { 0x1f},          { 0,    0x01f0 }, CLC_OK },  
/* note: if ok to this point its working but let's check dup put and get results */
/*       just put same key with different vals and do a fianl get.               */
/*   20 */ { PUT,  { 0xaa, 0x01aa }, { 0,    0      }, CLC_OK },     
/*   21 */ { PUT,  { 0xaa, 0x02aa }, { 0x1b, 0x1b0  }, CLC_REMOVED },
/*   22 */ { PUT,  { 0xaa, 0x03aa }, { 0x0c, 0x0c0  }, CLC_REMOVED },
/*   23 */ { PUT,  { 0xaa, 0x04aa }, { 0x1c, 0x1c0  }, CLC_REMOVED },
/*   24 */ { PUT,  { 0xaa, 0x05aa }, { 0x1d, 0x1d0  }, CLC_REMOVED },
/*   25 */ { PUT,  { 0xaa, 0x06aa }, { 0x0e, 0x0e0  }, CLC_REMOVED },
/*   26 */ { PUT,  { 0xaa, 0x07aa }, { 0x1e, 0x1e0  }, CLC_REMOVED },
/*   27 */ { GET,  { 0xaa },         { 0,    0x07aa }, CLC_OK },       
/* REVU: PUT semantics are still problematic as subsequent PUT of (any) keys will*/
/*       removed stale values for 0xaa and that is not good. should we keep PUT? */
#endif
/*   -- */ { _ORD, { 0       , 0    }, { 0        }, 0  , 0             },
/*   -- */ { _END, { 0       , 0    }, { 0        }, 0  , 0             }
};

///////////////////////////////////////////////////////////////////////////
/// FIFO Test Sequence ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

const op_spec_t test_sequence_FIFO [] = {
// seqnum            key   val         k0    v0        stat
/* check: fifo order                                        */
#if DONE
/*    1 */ { PUT,  { 0x01, 0x00a1 }, { 0,    0      }, CLC_OK },
/*    2 */ { PUT,  { 0x02, 0x00a2 }, { 0,    0      }, CLC_OK },
/*    3 */ { GET,  { 0x01 },         { 0,    0x00a1 }, CLC_OK },       
/*    4 */ { PUT,  { 0x03, 0x00a3 }, { 0,    0      }, CLC_OK },
/*    5 */ { GET,  { 0x01 },         { 0,    0x00a1 }, CLC_OK },       
/*    6 */ { PUT,  { 0x04, 0x00a4 }, { 0,    0      }, CLC_OK },
/*    7 */ { PUT,  { 0x05, 0x00a5 }, { 0,    0      }, CLC_OK },
/*    8 */ { GET,  { 0x04 },         { 0,    0x00a4 }, CLC_OK },       
/*    0 */ { PUT,  { 0x06, 0x00a6 }, { 0,    0      }, CLC_OK },
/*    9 */ { GET,  { 0x02 },         { 0,    0x00a2 }, CLC_OK },       
/*   10 */ { PUT,  { 0x07, 0x00a7 }, { 0,    0      }, CLC_OK },
/*   11 */ { GET,  { 0x01 },         { 0,    0x00a1 }, CLC_OK },       
/*   12 */ { PUT,  { 0xa1, 0x02a1 }, { 0x01, 0x0a1  }, CLC_REMOVED },
/*   13 */ { PUT,  { 0xa2, 0x02a2 }, { 0x02, 0x0a2  }, CLC_REMOVED },
/*   14 */ { PUT,  { 0xa3, 0x02a3 }, { 0x03, 0x0a3  }, CLC_REMOVED },
/*   15 */ { PUT,  { 0xa4, 0x02a4 }, { 0x04, 0x0a4  }, CLC_REMOVED },
/*   16 */ { PUT,  { 0xa5, 0x02a5 }, { 0x05, 0x0a5  }, CLC_REMOVED },
/*   17 */ { PUT,  { 0xa6, 0x02a6 }, { 0x06, 0x0a6  }, CLC_REMOVED },
/*   18 */ { PUT,  { 0xa7, 0x02a7 }, { 0x07, 0x0a7  }, CLC_REMOVED },
/* check: return values for not found, dup, and invalid key */
/*   19 */ { PUT,  { 0,    0x01e0 }, { 0,    0      }, CLC_EINVKEY },  // once is enough
/*   20 */ { PUTX, { 0xa1, 0x00cc }, { 0,    0x02a1 }, CLC_EDUPKEY },  // once is enough
/*   21 */ { GET,  { 0x01 },         { 0,    0      }, CLC_NOTFOUND }, // once is enough
/* check: put same key 7 time and do some gets to get last. */
/*  note: we should get the current set of values back fifo.*/
/*   22 */ { PUT,  { 0xff, 0x01ff }, { 0xa1, 0x2a1  }, CLC_REMOVED },
/*   23 */ { PUT,  { 0xff, 0x02ff }, { 0xa2, 0x2a2  }, CLC_REMOVED },
/*   24 */ { PUT,  { 0xff, 0x03ff }, { 0xa3, 0x2a3  }, CLC_REMOVED },
/*   25 */ { PUT,  { 0xff, 0x04ff }, { 0xa4, 0x2a4  }, CLC_REMOVED },
/*   26 */ { PUT,  { 0xff, 0x05ff }, { 0xa5, 0x2a5  }, CLC_REMOVED },
/*   27 */ { PUT,  { 0xff, 0x06ff }, { 0xa6, 0x2a6  }, CLC_REMOVED },
/*   28 */ { PUT,  { 0xff, 0x07ff }, { 0xa7, 0x2a7  }, CLC_REMOVED },
/*   29 */ { GET,  { 0xff },         { 0,    0x07ff }, CLC_OK },       
/*   30 */ { GET,  { 0xff },         { 0,    0x07ff }, CLC_OK },       
/* check: delete - delete in reverse order w/ get followups */
/*   31 */ { DEL,  { 0xff},          { 0,    0x07ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x06ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x06ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x05ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x05ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x04ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x04ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x03ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x03ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x02ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x02ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0x01ff }, CLC_OK },       
/*   31 */ { DEL,  { 0xff},          { 0,    0x01ff }, CLC_OK },  
/*   32 */ { GET,  { 0xff },         { 0,    0      }, CLC_NOTFOUND }, 
#endif
/*   -- */ { _ORD, { 0       , 0    }, { 0        }, 0  , 0             },
/*   -- */ { _END, { 0       , 0    }, { 0        }, 0  , 0             }
};

///////////////////////////////////////////////////////////////////////////
/// 2Q Test Sequence //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

const op_spec_t test_sequence_2Q [] = {
/*   -- */ { _ORD, { 0       , 0    }, { 0        }, 0  , 0             },
/*   -- */ { _END, { 0       , 0    }, { 0        }, 0  , 0             }
};

/* --------------------------------------------------------------------- */
/* clc_0x7m Policy Tests  */

static testrep_t const * test_cache_lru         (void);
static testrep_t const * test_cache_lru_inline  (void);
#if DONE
static testrep_t const * test_cache_fifo        (void);
static testrep_t const * test_cache_fifo_inline (void);
static testrep_t const * test_cache_2q          (void);
static testrep_t const * test_cache_2q_inline   (void);
#endif
static testrep_t const * test_cache_lru_inline (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <LRU> inline", __clc.cache.lru, test_sequence_LRU);
}
static testrep_t const * test_cache_lru (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <LRU>", clc.cache.lru, test_sequence_LRU);
}
#if DONE
static testrep_t const * test_cache_fifo_inline (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <FIFO> inline", __clc.cache.fifo, test_sequence_FIFO);
}
static testrep_t const * test_cache_fifo (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <FIFO>", clc.cache.fifo, test_sequence_FIFO);
}
static testrep_t const * test_cache_2q_inline (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <2Q> inline", __clc.cache.q2, test_sequence_2Q);
}
static testrep_t const * test_cache_2q (void) 
{
	/* NOTE uses clc objects */
	return test_container ("clc_cache <2Q>", clc.cache.q2, test_sequence_2Q);
}
#endif
test_fn test_cache_policies[] = {
	test_cache_lru_inline,
	test_cache_lru,
	0,
};

/* --------------------------------------------------------------------- */
/* runner options ------- */

typedef struct option_struct {
	unsigned int iters;
} option_t;

/* --------------------------------------------------------------------- */
/* run the test suites -- */

int main (int argc, char **argv) {
	printf("Salaam!\n");

	runsuite("clc_0x7m policies", test_cache_policies);

	return 0;
}

