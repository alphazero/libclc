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

#ifndef __INLINE_CLC_CACHE_H_
#define __INLINE_CLC_CACHE_H_

#include "libclc.h"

#include <stdio.h> // TEMP

/* ------------------------------------------------------------------------- */
/* cache : lru --------------- */

// TODO
static inline clc_stat 
__clc_cache_del_lru (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	return CLC_ENOTIMPL;
}

static inline clc_stat 
__clc_cache_get_lru (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{

	// REVU: allow all-zero key bit pattern.
	// We only care about zero-value records (which are not valid).
	register uint64_t const key = selector & mask;
//	if (key == 0ULL) {
//		return CLC_EKEY; // REVU: careful to only return < 0 errors
//	}

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );
	clc_assert_out_ptr_m   ( rec );
	clc_assert_out_ptr_m   ( rmeta );

	// fast track the MRU
	if( (clc_record_at_m (p, 0).image & mask) == key) {
		*rec = clc_record_at_m (p, 0).image;
		*rmeta = clc_rmeta_at_m (p, 0);
		return CLC_OK;
	}
	for(unsigned r=1; r<clc_unitlen_m(p); r++){
		if( (clc_record_at_m(p, r).image & mask) == key) {
			*rec = clc_record_at_m (p, r).image;
			*rmeta = clc_rmeta_at_m (p, r);
			clc_shift_up_m (p, r);
			return CLC_OK;
		}
	}
	return CLC_NOTFOUND;
}

static inline clc_stat 
__clc_cache_put_lru (void *const p, uint64_t rec, uint64_t *rec0, uint8_t *rmeta) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );
	clc_assert_recval_m    ( rec );
	clc_assert_out_ptr_m   ( rec0 );
	clc_assert_out_ptr_m   ( rmeta );

	*rmeta = clc_rmeta_at_m (p, 6);
	*rec0 = clc_record_at_m (p, 6).image;
	clc_record_at_m (p, 6).image = rec;

	clc_shift_up_m (p, 6);
//	*rmeta = clc_rmeta_at_m (p, 0); // NOTE here its a bit slower ..

	if (*rec0 != 0ULL) {
		return CLC_REMOVED;
	}

	clc_cmeta_set_len_m (p, clc_unitlen_m (p) + 1);
	return CLC_OK;
}

static inline clc_stat 
__clc_cache_putx_lru (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );
	clc_assert_recval_m    ( rec );

	// REVU: zero-value KEY is OK - we only care about zero-value records
	uint64_t const key = rec & mask;

	clc_assert_out_ptr_m   ( rec0 );
	clc_assert_out_ptr_m   ( rmeta );

	for(unsigned r=0; r<clc_unitlen_m(p); r++){
		if( (clc_record_at_m(p, r).image & mask) == key) {
			*rec0 = clc_record_at_m (p, r).image;
			*rmeta = clc_rmeta_at_m (p, r);
			return CLC_DUPLICATE;
		}
	}
	
	return __clc_cache_put_lru (p, rec, rec0, rmeta);	
}

static inline clc_stat 
__clc_cache_putr_lru (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {

	// REVU: all below are asserted in the put call below.
//	clc_assert_in_ptr_m    ( p );
//	clc_assert_alignment_m ( p );
//	clc_assert_out_ptr_m   ( rec0 );
//	clc_assert_out_ptr_m   ( rmeta );

//	*rec0 = rec;
	clc_stat const s = __clc_cache_get_lru (p, rec, mask, rec0, rmeta);
	if (s < 0)  {
		return s;
	}

	clc_record_at_m (p, 0).image = rec;
	*rmeta = clc_rmeta_at_m (p, 0);

	return CLC_OK;	
}

static inline clc_stat
__clc_cache_put_lru_sync (void *const p, uint64_t rec, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_put_lru (p, rec, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_putx_lru_sync (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_putx_lru (p, rec, mask, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_putr_lru_sync (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_putr_lru (p, rec, mask, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_get_lru_sync (void *const p, uint64_t key, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	clc_sync_op_m (p,  __clc_cache_get_lru (p, key, mask, rec, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_del_lru_sync (void *const p, uint64_t key, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	clc_sync_op_m (p,  __clc_cache_del_lru (p, key, mask, rec, rmeta) );
	return r;
}
#endif // __INLINE_CLC_CACHE_H_
