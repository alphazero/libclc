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

#ifndef __INLINE_CLC_CACHE_2Q_H_
#define __INLINE_CLC_CACHE_2Q_H_

#ifndef _LIBCLC_INLINE_H_
#error Do not directly include this header. Only include libclc/inline.h
#endif

#include "libclc.h"

#include <stdio.h> // TEMP

/* ------------------------------------------------------------------------- */
/* cache : 2q --------------- */

// REVU: this is precisely SET.delete 
static inline clc_stat 
__clc_cache_del_2q (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	return CLC_ENOTIMPL;
}

// this should be clc_unit
static inline clc_stat 
__clc_cache_get_2q (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	register uint64_t const key = selector & mask;
	clc_assert_key_m ( key );

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
			return CLC_OK;
		}
	}
	return CLC_NOTFOUND;
}

static inline clc_stat 
__clc_cache_put_2q (void *const p, uint64_t rec, uint64_t *rec0, uint8_t *rmeta) {
	return CLC_ENOTIMPL;
}

static inline clc_stat 
__clc_cache_putx_2q (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	return CLC_ENOTIMPL;
}

static inline clc_stat 
__clc_cache_putr_2q (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	return CLC_ENOTIMPL;
}

static inline clc_stat
__clc_cache_put_2q_sync (void *const p, uint64_t rec, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_put_2q (p, rec, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_putx_2q_sync (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_putx_2q (p, rec, mask, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_putr_2q_sync (void *const p, uint64_t rec, uint64_t mask, uint64_t *rec0, uint8_t *rmeta) {
	clc_sync_op_m (p,  __clc_cache_putr_2q (p, rec, mask, rec0, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_get_2q_sync (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	clc_sync_op_m (p,  __clc_cache_get_2q (p, selector, mask, rec, rmeta) );
	return r;
}
static inline clc_stat
__clc_cache_del_2q_sync (void *const p, uint64_t selector, uint64_t mask, 
		uint64_t* rec, uint8_t *rmeta) 
{
	clc_sync_op_m (p,  __clc_cache_del_2q (p, selector, mask, rec, rmeta) );
	return r;
}
#endif // __INLINE_CLC_CACHE_2Q_H_
