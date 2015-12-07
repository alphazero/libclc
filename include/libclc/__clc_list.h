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

#ifndef __CLC_LIST_H_
#define __CLC_LIST_H_

#include "libclc.h"
#include "libclc/__clc_stack.h"

/* ------------------------------------------------------------------------- */
/* container : list ---------- */

static inline clc_stat
__clc_list_add (void *const p, uint64_t rec, uint8_t *rmeta) {
	return __clc_stack_push (p, rec, rmeta);
}

static inline clc_stat
__clc_list_append (void *const p, uint64_t rec, uint8_t *rmeta) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	if (clc_unitlen_m(p) == 7) {
		return CLC_FULL;
	}

	clc_assert_recval_m    ( rec );
	clc_assert_out_ptr_m   ( rmeta );

	clc_record_at_m (p, clc_unitlen_m(p)).image = rec;
	*rmeta = clc_rmeta_at_m(p, clc_unitlen_m(p));

	clc_cmeta_set_len_m(p, clc_unitlen_m(p) + 1);

	return CLC_OK;
}

static inline clc_stat
__clc_list_insert (void *const p, uint8_t n, uint64_t rec, uint8_t *rmeta) {

	if (n == 0) { 
		return __clc_stack_push (p, rec, rmeta);
	}

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	if (clc_unitlen_m(p) == 7) {
		return CLC_FULL;
	}

	if (n > clc_unitlen_m(p)) {
		return CLC_EINDEX;
	}

	clc_assert_recval_m    ( rec );
	clc_assert_out_ptr_m   ( rmeta );

	clc_record_at_m (p, 6).image = rec;
	if (n < 6) {
		clc_shift_r6_to_m (p, n);
	}

	*rmeta = clc_rmeta_at_m(p, n);

	clc_cmeta_set_len_m(p, clc_unitlen_m(p) + 1);

	return CLC_OK;
}

// REVU: this is actually clc.unit.get
// REVU: minus the asserts and checks, this is record_at & rmeta_at
static inline clc_stat
__clc_list_get (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	if(clc_unitlen_m(p) == 0) {
		return CLC_NOTFOUND; // REVU: CLC_EMPTY is possibly more informative
	}

	if(n > clc_unitlen_m(p)) {
		return CLC_EINDEX;
	}

	clc_assert_out_ptr_m   ( rec );
	clc_assert_out_ptr_m   ( rmeta );

	*rec = clc_record_at_m(p, n).image;
	*rmeta = clc_rmeta_at_m(p, n);

	return CLC_OK;
}

// REVU: this is actually clc.unit.del
// REVU: minus the asserts and checks, this is record_at & rmeta_at + shift
static inline clc_stat
__clc_list_del (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {

	if (n == 0) { 
		return __clc_stack_pop (p, rec, rmeta);
	}

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	if (n > clc_unitlen_m(p)) {
		return CLC_EINDEX;
	}

	clc_assert_out_ptr_m   ( rec );
	clc_assert_out_ptr_m   ( rmeta );

	*rec = clc_record_at_m(p, n).image;
	clc_record_at_m(p, n).image = 0ULL;

	clc_shift_dn_m (p, n);
	*rmeta = clc_rmeta_at_m(p, 6);

	clc_cmeta_set_len_m(p, clc_unitlen_m(p) - 1);
	return CLC_OK;
}

static inline clc_stat 
__clc_list_add_sync (void *const p,  uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_add (p, rec, rmeta));
	return r;
}
static inline clc_stat 
__clc_list_append_sync (void *const p,  uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_append (p, rec, rmeta));
	return r;
}
static inline clc_stat 
__clc_list_insert_sync (void *const p, uint8_t n, uint64_t rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_insert (p, n, rec, rmeta));
	return r;
}
static inline clc_stat 
__clc_list_get_sync (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_get (p, n, rec, rmeta));
	return r;
}
static inline clc_stat 
__clc_list_del_sync (void *const p, uint8_t n, uint64_t *rec, uint8_t *rmeta) {
	clc_sync_op_m (p, __clc_list_del (p, n, rec, rmeta));
	return r;
}

#endif //__CLC_LIST_H_
