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

#ifndef __INLINE_CLC_UNIT_H_
#define __INLINE_CLC_UNIT_H_ /* unit-container inline functions */

#include "libclc.h"

/* ------------------------------------------------------------------------- */
/* generic unit ops ---------- */

static inline clc_stat 
__clc_len (void const * const p, uint8_t * len) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );
	
	*len = clc_unitlen_m(p);
	
	return CLC_OK;	
}

static inline clc_stat 
__clc_reset(void *const p) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	// don't modify cmeta 
	for(uint8_t b=0; b<7; b++) {
		clc_unitptr_m(p)->meta.b[b] = b;
	}

	return CLC_OK;
}

static inline clc_stat 
__clc_init (void *const p) {

	clc_assert_in_ptr_m    ( p );
	clc_assert_alignment_m ( p );

	clc_stat const r = __clc_reset(p);
	if (clc_is_error_m(r)) 
		return r;

	for(unsigned i=0; i<7; i++) {
		clc_unitptr_m(p)->rec[i].image = 0ULL;
	}

	// cmeta 
	clc_unitptr_m(p)->meta.b[7] = 0;

	return CLC_OK;
}

/* aka init */
static inline clc_stat 
__clc_clear (void *const p) { 
	return __clc_init (p); 
}

/* ------------------------------------------------------------------------- */
/* debug support ------------- */

/* internal use only */
#define clc_pp_rs_m(p, rec)\
		for(uint8_t i=0; i<8 ; i++) fprintf(out, "%02x ", rec.b[i]);\
		fprintf(out, "| len:%d f: %d %d %d %d %d\n", clc_unitlen_m(p), 0, 0, 0, 0, 0);

/* internal use only */
#define clc_pp_rec0_m(p, rec)\
		for(uint8_t i=0; i<8 ; i++) fprintf(out, "%02x ", rec.b[i]);\
		fprintf(out, "| 0x%016llx\n", rec.image);

/* internal use only */
#define clc_pp_meta_m(p, rec)\
		fprintf(out, "~R : +00 : ");\
		clc_pp_rs_m(p, rec);

/* internal use only */
#define clc_pp_rec_m(p, rec, r)\
		fprintf(out, "R%d : +%02d : ", r, (r+1)<<3);\
		clc_pp_rec0_m(p, rec);

/* internal use only */
#define pp_preamble_m(p, rec)\
	printf("-----------------------------------[ %16p ]\n", p);\
	clc_pp_meta_m(p, rec);

static inline void
__clc_dump (FILE * restrict out, void const * const p) {

	union clc_record rec;
	rec.image = clc_unitptr_m(p)->meta.image;
	pp_preamble_m (p, rec);
	for(uint8_t r = 0; r < 7; r++) {
		rec.image = clc_unitptr_m(p)->rec[r].image;
		clc_pp_rec_m (p, rec, r);
	}
	fprintf(out, "\n");
	return;
}

static inline void
__clc_dump_inorder (FILE * restrict out, void const * const p) {

	union clc_record rec;
	rec.image = clc_unitptr_m(p)->meta.image;
	pp_preamble_m (p, rec);
	for(uint8_t r = 0; r < 7; r++) {
		rec.image = clc_record_at_m (p, r).image;
		clc_pp_rec_m (p, rec, clc_rec_idx_m (p, r));
	}
	fprintf(out, "\n");
	return;
}

static inline void
__clc_debug (void const * const p) {
	__clc_dump (stderr, p);
	return;
}

static inline void
__clc_debug_inorder (void const * const p) {
	__clc_dump_inorder (stderr, p);
	return;
}

#endif // __INLINE_CLC_UNIT_H_
