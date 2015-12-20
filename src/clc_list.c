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
#include "libclc/inline.h"

/* ------------------------------------------------------------------------- */
/* container : list ---------- */

clc_stat
clc_list_add (void *const p, uint64_t rec, uint8_t *rmeta) {
	return __clc_list_add (p, rec, rmeta); 
}

clc_stat
clc_list_append (void *const p, uint64_t rec, uint8_t *rmeta) {
	return __clc_list_append (p, rec, rmeta);
}

clc_stat
clc_list_insert (void *const p, uint8_t n, uint64_t rec, uint8_t *rmeta) {
	return __clc_list_insert (p, n, rec, rmeta);
}

// REVU: this is actually clc.unit.get
// REVU: minus the asserts and checks, this is record_at & rmeta_at
clc_stat
clc_list_get (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	return __clc_list_get (p, n, rec, rmeta);
}

// REVU: this is actually clc.unit.del
// REVU: minus the asserts and checks, this is record_at & rmeta_at + shift
clc_stat
clc_list_del (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	return __clc_list_del (p, n, rec, rmeta);
}
clc_stat 
clc_list_add_sync (void *const p,  uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_add (p, rec, rmeta));
	return r;
}
clc_stat 
clc_list_append_sync (void *const p,  uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_append (p, rec, rmeta));
	return r;
}
clc_stat 
clc_list_insert_sync (void *const p, uint8_t n, uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_insert (p, n, rec, rmeta));
	return r;
}
clc_stat 
clc_list_get_sync (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_get (p, n, rec, rmeta));
	return r;
}
clc_stat 
clc_list_del_sync (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_del (p, n, rec, rmeta));
	return r;
}

