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

#include "libclc.h"
#include "libclc/__clc_stack.h"

/* ------------------------------------------------------------------------- */
/* container : stack --------- */

clc_stat 
clc_stack_push (void * const p, uint64_t rec, uint8_t * rmeta) {
	return __clc_stack_push (p, rec, rmeta);
}

clc_stat 
clc_stack_pop (void *const p, uint64_t *rec, uint8_t *rmeta) {
	return __clc_stack_pop (p, rec, rmeta);
}

/* REVU (typical for all container _syncs)
   these sh/could directly invoke their inline peer
 */

clc_stat 
clc_stack_push_sync (void*const p, uint64_t rec, uint8_t* rmeta) {
	clc_sync_op_m (p, __clc_stack_push (p, rec, rmeta));
	return r;
}
clc_stat 
clc_stack_pop_sync (void*const p, uint64_t *rec, uint8_t* rmeta) {
	clc_sync_op_m (p, __clc_stack_pop (p, rec, rmeta));
	return r;
}

