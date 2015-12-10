// !!! DOOST !!!
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

#ifndef _CLC_OBJECTS_H_
#define _CLC_OBJECTS_H_

#include "libclc.h"
#include "libclc/inline.h"

/* NOTE:
   Certain use-cases (e.g. testing) that require a more generic (pseudo object
   oriented) access to the functionality of libclc are facilitated by the use
   of the structures and static storage instances provided in this header file.

   For general use, note that a possibly measurable memory pressure may be placed
   (in terms of instruction cache in L1) on the system. 

   See usage example below at storage declaration of the following structures. 
 */

/* ------------------------------------------------------------------------- */
/* container : base unit ----- */

struct clc_unit_op {
	clc_stat (* init ) (void * const p);
	clc_stat (* reset ) (void * const p) ;
	clc_stat (* clear ) (void * const p) ;
	clc_stat (* find ) (void * const p, uint64_t rec, uint8_t * rmeta); 
	clc_stat (* select ) (void * const p, uint64_t rec, uint64_t mask, uint64_t *iter);
	clc_stat (* len ) (void const * const p, uint8_t * len) ;
	clc_stat (* cmeta ) (void const * const p, uint8_t * cmeta) ;
	clc_stat (* rmeta ) (void const * const p, uint8_t r, uint8_t * rmeta) ;
	void     (* dump ) (FILE * restrict out, void const * const p);
	void     (* debug ) (void const * const p);
	void     (* dump_inorder ) (FILE * restrict out, void const * const p);
	void     (* debug_inorder ) (void const * const p);
};

/* ------------------------------------------------------------------------- */
/* container : stack --------- */

struct clc_stack_op {
	clc_stat (*push) (void * const p, uint64_t rec, uint8_t * rmeta);
	clc_stat (*pop) (void * const p, uint64_t * rec, uint8_t * remeta);
};

/* ------------------------------------------------------------------------- */
/* container : list ---------- */

struct clc_list_op { 
	clc_stat (*add) (void*, uint64_t, uint8_t*);
	clc_stat (*append) (void*, uint64_t, uint8_t*);
	clc_stat (*insert) (void*, uint8_t, uint64_t, uint8_t*);
	clc_stat (*get) (void*, uint8_t, uint64_t*, uint8_t*);
	clc_stat (*del) (void*, uint8_t, uint64_t*, uint8_t*);
};

#ifdef CONCEPTUAL_RESOLVED
/* ------------------------------------------------------------------------- */
/* container : map ----------- */

struct clc_map_op { 
	clc_stat (*get)  (void*const p, uint64_t* rec, uint64_t mask, uint8_t* rmeta);
	clc_stat (*put)  (void*const p, uint64_t rec, uint8_t* rmeta);
	clc_stat (*putx) (void*const p, uint64_t rec, uint64_t mask, uint8_t* rmeta);
	clc_stat (*putr) (void*const p, uint64_t rec, uint64_t mask, uint8_t* rmeta);
};
#endif

/* ------------------------------------------------------------------------- */
/* container : queue --------- */

struct clc_queue_op {
	clc_stat (*enqueue) (void*, uint64_t, uint8_t*);
	clc_stat (*dequeue) (void*, uint64_t*, uint8_t*);
};

/* ------------------------------------------------------------------------- */
/* container : cache --------- */

// intrinsic, policy-invariant, caching container semantics.
struct clc_cache_op {
	clc_stat (*get)  (void*, uint64_t, uint64_t, uint64_t*, uint8_t*);
	clc_stat (*del)  (void*, uint64_t, uint64_t, uint64_t*, uint8_t*);
	clc_stat (*put)  (void*, uint64_t, uint64_t*, uint8_t*);
	clc_stat (*putx) (void*, uint64_t, uint64_t, uint64_t*, uint8_t*);
	clc_stat (*putr) (void*, uint64_t, uint64_t, uint64_t*, uint8_t*);
};

/* ------------------------------------------------------------------------- */
/* container : cache : policy  */

// policy specific instances
struct clc_cache_policy {
	struct clc_cache_op fifo;
	struct clc_cache_op lru;
	struct clc_cache_op q2;		// aka 2Q
	struct clc_cache_op mfu;
};

/* ------------------------------------------------------------------------- */
/* libclc : api -------------- */

struct libclc {
	struct clc_unit_op unit;
	struct clc_list_op list;
	struct clc_stack_op stack;
	struct clc_queue_op queue;
	struct clc_cache_policy cache;
	struct {
		struct clc_unit_op unit;
		struct clc_list_op list;
		struct clc_stack_op stack;
		struct clc_queue_op queue;
		struct clc_cache_policy cache;
	} sync;
};

/* ------------------------------------------------------------------------- */
/* libclc : api : providers -- */

/* using inline functions */
static struct libclc const __clc = {
	.unit.init = __clc_init,
	.unit.reset = __clc_reset,
	.unit.clear = __clc_clear,
	.unit.len = __clc_len,
	.unit.dump = __clc_dump,
	.unit.debug = __clc_debug,
	.unit.dump_inorder = __clc_dump_inorder,
	.unit.debug_inorder = __clc_debug_inorder,
	/* stack */
	.stack.push = __clc_stack_push,
	.stack.pop = __clc_stack_pop,
	.sync.stack.push = __clc_stack_push_sync,
	.sync.stack.pop = __clc_stack_pop_sync,
	/* list */
	.list.append = __clc_list_append,
	.list.add = __clc_list_add,
	.list.insert = __clc_list_insert,
	.list.get = __clc_list_get,
	.list.del = __clc_list_del,
	.sync.list.append = __clc_list_append_sync,
	.sync.list.add = __clc_list_add_sync,
	.sync.list.insert = __clc_list_insert_sync,
	.sync.list.get = __clc_list_get_sync,
	.sync.list.del = __clc_list_del_sync,
	/* queue */
	.queue.enqueue = __clc_queue_enqueue,
	.queue.dequeue = __clc_queue_dequeue,
	.sync.queue.enqueue = __clc_queue_enqueue_sync,
	.sync.queue.dequeue = __clc_queue_dequeue_sync,
	/* store: map & set */
	// TODO pending REVU
	/* cache:lru */
	.cache.lru.get = __clc_cache_get_lru,
	.cache.lru.del = __clc_cache_del_lru,
	.cache.lru.put = __clc_cache_put_lru,
	.cache.lru.putr = __clc_cache_putr_lru,
	.cache.lru.putx = __clc_cache_putx_lru,
	.sync.cache.lru.get = __clc_cache_get_lru_sync,
	.sync.cache.lru.del = __clc_cache_del_lru_sync,
	.sync.cache.lru.put = __clc_cache_put_lru_sync,
	.sync.cache.lru.putr = __clc_cache_putr_lru_sync,
	.sync.cache.lru.putx = __clc_cache_putx_lru_sync,
	/* cache:fifo */
	.cache.fifo.get = __clc_cache_get_fifo,
	.cache.fifo.del = __clc_cache_del_fifo,
	.cache.fifo.put = __clc_cache_put_fifo,
	.cache.fifo.putr = __clc_cache_putr_fifo,
	.cache.fifo.putx = __clc_cache_putx_fifo,
	.sync.cache.fifo.get = __clc_cache_get_fifo_sync,
	.sync.cache.fifo.del = __clc_cache_del_fifo_sync,
	.sync.cache.fifo.put = __clc_cache_put_fifo_sync,
	.sync.cache.fifo.putr = __clc_cache_putr_fifo_sync,
	.sync.cache.fifo.putx = __clc_cache_putx_fifo_sync,
	/* cache:fifo */
	.cache.q2.get = __clc_cache_get_2q,
	.cache.q2.del = __clc_cache_del_2q,
	.cache.q2.put = __clc_cache_put_2q,
	.cache.q2.putr = __clc_cache_putr_2q,
	.cache.q2.putx = __clc_cache_putx_2q,
	.sync.cache.q2.get = __clc_cache_get_2q_sync,
	.sync.cache.q2.del = __clc_cache_del_2q_sync,
	.sync.cache.q2.put = __clc_cache_put_2q_sync,
	.sync.cache.q2.putr = __clc_cache_putr_2q_sync,
	.sync.cache.q2.putx = __clc_cache_putx_2q_sync,
};

/* using inline functions */
static struct libclc const clc = {
	.unit.init = clc_init,
	.unit.reset = clc_reset,
	.unit.clear = clc_clear,
	.unit.len = clc_len,
	.unit.dump = clc_dump,
	.unit.debug = clc_debug,
	.unit.dump_inorder = clc_dump_inorder,
	.unit.debug_inorder = clc_debug_inorder,
	/* stack */
	.stack.push = clc_stack_push,
	.stack.pop = clc_stack_pop,
	.sync.stack.push = clc_stack_push_sync,
	.sync.stack.pop = clc_stack_pop_sync,
	/* list */
	.list.append = clc_list_append,
	.list.add = clc_list_add,
	.list.insert = clc_list_insert,
	.list.get = clc_list_get,
	.list.del = clc_list_del,
	.sync.list.append = clc_list_append_sync,
	.sync.list.add = clc_list_add_sync,
	.sync.list.insert = clc_list_insert_sync,
	.sync.list.get = clc_list_get_sync,
	.sync.list.del = clc_list_del_sync,
	/* queue */
	.queue.enqueue = clc_queue_enqueue,
	.queue.dequeue = clc_queue_dequeue,
	.sync.queue.enqueue = clc_queue_enqueue_sync,
	.sync.queue.dequeue = clc_queue_dequeue_sync,
	/* store: map & set */
	// TODO pending REVU
	/* cache:lru */
	.cache.lru.get = clc_cache_get_lru,
	.cache.lru.del = clc_cache_del_lru,
	.cache.lru.put = clc_cache_put_lru,
	.cache.lru.putr = clc_cache_putr_lru,
	.cache.lru.putx = clc_cache_putx_lru,
	.sync.cache.lru.get = clc_cache_get_lru_sync,
	.sync.cache.lru.del = clc_cache_del_lru_sync,
	.sync.cache.lru.put = clc_cache_put_lru_sync,
	.sync.cache.lru.putr = clc_cache_putr_lru_sync,
	.sync.cache.lru.putx = clc_cache_putx_lru_sync,
	/* cache:fifo */
	.cache.fifo.get = __clc_cache_get_fifo,
	.cache.fifo.del = __clc_cache_del_fifo,
	.cache.fifo.put = __clc_cache_put_fifo,
	.cache.fifo.putr = __clc_cache_putr_fifo,
	.cache.fifo.putx = __clc_cache_putx_fifo,
	.sync.cache.fifo.get = __clc_cache_get_fifo_sync,
	.sync.cache.fifo.del = __clc_cache_del_fifo_sync,
	.sync.cache.fifo.put = __clc_cache_put_fifo_sync,
	.sync.cache.fifo.putr = __clc_cache_putr_fifo_sync,
	.sync.cache.fifo.putx = __clc_cache_putx_fifo_sync,
	/* cache:fifo */
	.cache.q2.get = __clc_cache_get_2q,
	.cache.q2.del = __clc_cache_del_2q,
	.cache.q2.put = __clc_cache_put_2q,
	.cache.q2.putr = __clc_cache_putr_2q,
	.cache.q2.putx = __clc_cache_putx_2q,
	.sync.cache.q2.get = __clc_cache_get_2q_sync,
	.sync.cache.q2.del = __clc_cache_del_2q_sync,
	.sync.cache.q2.put = __clc_cache_put_2q_sync,
	.sync.cache.q2.putr = __clc_cache_putr_2q_sync,
	.sync.cache.q2.putx = __clc_cache_putx_2q_sync,
};

#endif //_CLC_OBJECTS_H_
