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

#ifndef _LIBCLC_H_
#define _LIBCLC_H_

#include <stdlib.h>
#include <stdio.h>

/* ------------------------------------------------------------------------- */
/* parametric settings ------- */

// NOTE: this section is a placeholder.
//
// REVU: 128b CLCs would require 4bits for the record index in the associated
// rmeta element vs the 3bits for canonical 64b version. Further, the brute-
// force approach to searching a unit-container may not be as performant. 
// 
// TODO consider support for 128 byte cache lines. 
// options:
// - 2x clc-0x7-64bit : potential issue is false sharing. can be addressed.
// - 1x clc-0xE-64bit : O(n) concerns. bench it.

#define CLC_SIZE 0x40

/* ------------------------------------------------------------------------- */
/* structure ----------------- */

// CLC records are viewed either as (value representation) images of 8 bytes,
// or a container and record-set metadata (cmeta & rmeta, respectively) 
// array of bytes.
union clc_record {
	uint64_t  image;
	uint8_t   b[ 8 ];
};

// Unit Container. 
//
// The initial record of all CLCs, regardless of container semantics, is
// the 8 byte meta-data record. The remaining 7 records are user-data.
struct clc_unit {
	union clc_record meta;
	union clc_record rec[ 7 ];
};

// R-META : unit container physical record metadata bits
//
// rmeta bit semantics & ascii art

/* physical record index */
#define clc_rmeta_idx_mask         ((uint8_t)0x07) /* -----### */
/* extended record metadata */
#define clc_rmeta_ext_mask         ((uint8_t)0x38) /* --###--- */
/* reserved rmeta bit */
#define clc_rmeta_dirty_mask       ((uint8_t)0x40) /* -#------ */
/* reserved rmeta bit */
#define clc_rmeta_lock_mask        ((uint8_t)0x80) /* #------- */
/* use to set physical record index */
#define clc_rmeta_idx_mask_inv     ((uint8_t)0xf8) /* #####--- */
/* use to set extended record metadata */
#define clc_rmeta_ext_mask_inv     ((uint8_t)0xc7) /* ##---### */
/* use to set reserved rmeta bit - 1 -> dirty */
#define clc_rmeta_dirty_mask_inv   ((uint8_t)0xbf) /* #-###### */
/* use to set reserved rmeta bit - 1 -> locked */
#define clc_rmeta_lock_mask_inv    ((uint8_t)0x7f) /* -####### */

// C-META : unit container metadata bits
//
#define clc_cmeta_offset 7

// cmeta bit semantics

/* physical record count */
#define clc_cmeta_len_mask         ((uint8_t)0x07) /* -----### */
/* extended record metadata */
#define clc_cmeta_ext_mask         ((uint8_t)0x38) /* --###--- */
/* reserved rmeta bit */
#define clc_cmeta_dirty_mask       ((uint8_t)0x40) /* -#------ */
/* reserved rmeta bit */
#define clc_cmeta_lock_mask        ((uint8_t)0x80) /* #------- */
/* use to set physical record count */
#define clc_cmeta_len_mask_inv     ((uint8_t)0xf8) /* #####--- */
/* use to set extended unit-c metadata */
#define clc_cmeta_idx_mask_inv     ((uint8_t)0xc7) /* ##---### */
/* use to set reserved cmeta bit - 1 -> dirty */
#define clc_cmeta_dirty_mask_inv   ((uint8_t)0xbf) /* #-###### */
/* use to set reserved cmeta bit - 1 -> locked */
#define clc_cmeta_lock_mask_inv    ((uint8_t)0x7f) /* -####### */

/* ------------------------------------------------------------------------- */
/* convenience macros -------- */

/* NOTE:
   It is perfectly fine to use these macros directly, if
   (a) you know what you are doing
   (b) your pointers are correct & you don't need asserts.

   That said, the inline functions (libclc/inline.h) are quite performant
   and will provide the same. Ideally we wouldn't want to export these
   macros but it is not clear how to keep them 'private' (as they are 
   required for the inline functions and can't be folded into the ref-impl
   sources.) 
 */

// unit container pointer.
// convert (expected) void* pointer to a CLC unit pointer.
#define clc_unitptr_m(_p_) ((struct clc_unit*)(_p_))

// unit container's metadata. 
#define clc_unit_cmeta_m(_p_)    (clc_unitptr_m(_p_)->meta.b[clc_cmeta_offset])

// unit container's length (e.g. number of records).
#define clc_unitlen_m(_p_)       (clc_unit_cmeta_m(_p_) & clc_cmeta_len_mask)

// record metadata for a specific record of the unit-container.
// NOTE that record order in RMETA reflects the ordering of records per
// specific container type semantics, and is NOT a direct index into the
// clc_unit.rec[] array.
#define clc_rmeta_at_m(_p_,_r_)  (clc_unitptr_m(_p_)->meta.b[(_r_)])

// Index of the indicated record's physical posiiton in the clc_unit.rec[].
#define clc_rec_idx_m(_p_,_r_)   (clc_rmeta_at_m(_p_,_r_) & clc_rmeta_idx_mask)

// REVU: deprecated. TODO cleanup.
#define clc_rec_ord_m(_p_,_r_)   (clc_rmeta_at_m(_p_,_r_) & clc_rmeta_ord_tag)

// Maps semantic record index _r_ to corresponding physical record.
#define clc_record_at_m(_p_,_r_) (clc_unitptr_m(_p_)->rec[clc_rec_idx_m (_p_,_r_)])

#define clc_R0_m(_p_)            (clc_unitptr_m(_p_)->meta)
#define clc_R0img_m(_p_)         (clc_unitptr_m(_p_)->meta.image)

// sets the physical index for the given semantic order record.
#define clc_rmeta_set_idx_m(_p_,_r_,_v_)\
	(clc_rmeta_at_m(_p_,_r_) = (clc_rmeta_at_m(_p_,_r_)&0xf8)|(_v_))

// sets the container length.
#define clc_cmeta_set_len_m(_p_,_v_)\
	(clc_unit_cmeta_m(_p_) = (clc_unit_cmeta_m(_p_)&0xf8)|(_v_))

// REVU: TODO iterator macros.


/* ------------------------------------------------------------------------- */
/* func. call stat codes ----- */

// REVU: would prefer a smaller unsigned stat code. TODO bench it.

// CLC's (function) API uniformly conforms to the pattern of IN/OUT args and 
// a function call return status. There is a bit of semantic conflation in that 
// non-zero positive return codes can convey function specific (semantic) 
// information.
typedef int32_t clc_stat;

// The following status codes all represent a non-faulty function applications.
// The API systemically treats possible (non-faulty) function results as 
// error free usage, even if the result indicates a non-application of the
// invoked function. 
#define CLC_OK              ((clc_stat) 0)
#define CLC_FULL            ((clc_stat) 1)
#define CLC_EMPTY           ((clc_stat) 2)
#define CLC_REMOVED         ((clc_stat) 3)
#define CLC_NOTFOUND        ((clc_stat) 4)
#define CLC_DUPLICATE       ((clc_stat) 5)

// A negative functional return indicates an error.
#define clc_is_error_m(_s_) ((_s_) < CLC_OK)

// error stat codes are uniformly of form CLC_E<specific-error>
// and cover both user error (e.g. invalid arguments) and 
// system faults (e.g. out-of-memory).
#define CLC_ESTATE          ((clc_stat) -1)
#define CLC_EALIGNMENT      ((clc_stat) -2)
#define CLC_EPOINTER        ((clc_stat) -3)
#define CLC_EARG            ((clc_stat) -4) // REVU: really CLC_ERECORD 
#define CLC_EKEY            ((clc_stat) -5) // REVU: deprecated?
#define CLC_EINDEX          ((clc_stat) -6)
#define CLC_ERECORD         ((clc_stat) -7) 
#define CLC_ENOTIMPL        ((clc_stat) -255) // NOTE if bumping down stat size

// error reporting support
#define clc_error_fprintln_m(_fd_, _stat_, _info_)\
{\
	switch((_stat_)){\
		case CLC_ESTATE:\
			fprintf((_fd_), "ERR - CLC_ESTATE - invalid container state - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_EALIGNMENT:\
			fprintf((_fd_), "ERR - CLC_EALIGHMENT - container not cache line aigned - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_EPOINTER:\
			fprintf((_fd_), "ERR - CLC_EPOINTER - null pointer - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_EARG:\
			fprintf((_fd_), "ERR - CLC_EARG - invalid argument - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_EKEY:\
			fprintf((_fd_), "ERR - CLC_EKEY - invalid key - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_EINDEX:\
			fprintf((_fd_), "ERR - CLC_EINDEX - invalid record index - info: %s\n",\
				   	(_info_));\
			break;\
		case CLC_ENOTIMPL:\
			fprintf((_fd_), "ERR - CLC_ENOTIMPL - function not implemented - info: %s\n",\
				   	(_info_));\
			break;\
		default:\
			fprintf((_fd_), "BUG - CLC_E? <%d> - unknown error code - info: %s\n",\
					(_stat_), (_info_));\
	}\
}

#define clc_error_println_m(_stat_,_info_)\
	clc_error_fprintln_m(stderr, (_stat_), (_info_))

/* ------------------------------------------------------------------------- */
/* systolic shift ops -------- */

/* impl artifact - internal use only */
struct clc_rshift {
	uint64_t m0, m1, m2, shft;
} ;

// shift-up op masks for pivot records (0-6).
// rmask-up[7] record is pad only and not used. 
static const struct clc_rshift rmask_up [8] = {
	{ 0xffffffffffffff00, 0x0000000000000000, 0x00000000000000ff, 0,  }, // nop
	{ 0xffffffffffff0000, 0x00000000000000ff, 0x000000000000ff00, 8,  },
	{ 0xffffffffff000000, 0x000000000000ffff, 0x0000000000ff0000, 16, },
	{ 0xffffffff00000000, 0x0000000000ffffff, 0x00000000ff000000, 24, },
	{ 0xffffff0000000000, 0x00000000ffffffff, 0x000000ff00000000, 32, },
	{ 0xffff000000000000, 0x000000ffffffffff, 0x0000ff0000000000, 40, },
	{ 0xff00000000000000, 0x0000ffffffffffff, 0x00ff000000000000, 48, },
	{ 0,0,0,0 },
};
/* systolic shift - promote RS[r] to RS[0] */
#define clc_shift_up_m(_p_, _r_)\
{\
	clc_R0img_m(_p_) =                                                             \
	        ( (clc_R0img_m((_p_)) & rmask_up[(_r_)].m0                         ) | \
	        ( (clc_R0img_m((_p_)) & rmask_up[(_r_)].m1)) << 8                  ) | \
	        ( (clc_R0img_m((_p_)) & rmask_up[(_r_)].m2 ) >> rmask_up[_r_].shft );  \
}

// shift-dn op masks for pivot records (0-6).
// rmask-up[7] record is pad only and not used. 
static const struct clc_rshift rmask_dn [8] = {
	{ 0xff00000000000000, 0x00ffffffffffff00, 0x00000000000000ff, 48, },
	{ 0xff000000000000ff, 0x00ffffffffff0000, 0x000000000000ff00, 40, },
	{ 0xff0000000000ffff, 0x00ffffffff000000, 0x0000000000ff0000, 32, },
	{ 0xff00000000ffffff, 0x00ffffff00000000, 0x00000000ff000000, 24, },
	{ 0xff000000ffffffff, 0x00ffff0000000000, 0x000000ff00000000, 16, },
	{ 0xff0000ffffffffff, 0x00ff000000000000, 0x0000ff0000000000, 8,  },
	{ 0xff00ffffffffffff, 0x0000000000000000, 0x00ff000000000000, 0,  }, // nop
	{ 0,0,0,0 },
};
/* systolic shift - demote RS[r] to RS[6] */ 
#define clc_shift_dn_m(_p_, _r_)\
{\
	clc_R0img_m(_p_) =                                                         \
	        ( (clc_R0img_m(_p_) & rmask_dn[_r_].m0                         ) | \
	        ( (clc_R0img_m(_p_) & rmask_dn[_r_].m1)) >> 8                  ) | \
	        ( (clc_R0img_m(_p_) & rmask_dn[_r_].m2 ) << rmask_dn[_r_].shft );  \
}

static const struct clc_rshift rmask_r6_to [8] = {
	{ 0xff00000000000000, 0x0000ffffffffffff, 0x00ff000000000000, 48, },
	{ 0xff000000000000ff, 0x0000ffffffffff00, 0x00ff000000000000, 40, },
	{ 0xff0000000000ffff, 0x0000ffffffff0000, 0x00ff000000000000, 32, },
	{ 0xff00000000ffffff, 0x0000ffffff000000, 0x00ff000000000000, 24, },
	{ 0xff000000ffffffff, 0x0000ffff00000000, 0x00ff000000000000, 16, },
	{ 0xff0000ffffffffff, 0x0000ff0000000000, 0x00ff000000000000, 8,  },
	{ 0xff00ffffffffffff, 0x0000000000000000, 0x00ff000000000000, 0,  }, // nop
	{ 0,0,0,0 },
};
/* systolic shift - promote RS[6] to RS[0] */
#define clc_shift_r6_to_m(_p_, _r_)\
{\
	clc_R0img_m(_p_) =                                                                   \
   	        ( (clc_R0img_m((_p_)) & rmask_r6_to[(_r_)].m0                            ) | \
	        ( (clc_R0img_m((_p_)) & rmask_r6_to[(_r_)].m1)) << 8                     ) | \
	        ( (clc_R0img_m((_p_)) & rmask_r6_to[(_r_)].m2 ) >> rmask_r6_to[_r_].shft );  \
}

/* ------------------------------------------------------------------------- */
/* concurrency support ------- */

/* NOTE: 
   These macros are *just* place-holders. GCC _sync_ CAS spin-locks are the 
   baseline likely impls. but TODO is to try CK or liburcu or something..
 */

#define clc_unit_lock_m(_p_)\
	while((clc_unitptr_m(_p_)->meta.b[7]&clc_cmeta_lock_mask) == 0)\
		clc_unitptr_m(_p_)->meta.b[7] |= clc_cmeta_lock_mask;

#define clc_unit_unlock_m(_p_)\
	while((clc_unitptr_m(_p_)->meta.b[7]&clc_cmeta_lock_mask) != 0)\
		clc_unitptr_m(_p_)->meta.b[7] ^= clc_cmeta_lock_mask;

#define clc_sync_op_m(_p_, _opcall_)\
	clc_unit_lock_m (p);\
	clc_stat const r = (_opcall_);\
	clc_unit_unlock_m (p);

/* ------------------------------------------------------------------------- */
/* assert macros ------------- */

/* NOTE:
   Use compiler directive -DCLC_ASSERT_IN_ARGS to enable IN args asserts.
 */

#if defined(CLC_ASSERT_IN_ARGS)
	// -------------------------------
	#define clc_assert_in_ptr_m(_p_)\
	    if((_p_)==NULL)\
	        return CLC_EPOINTER ;

	#define clc_assert_alignment_m(_p_)\
		if(((size_t)(_p_) % CLC_SIZE) != 0)\
	        return CLC_EALIGNMENT ;

	#define clc_assert_recval_m(_rec_)\
		if((uint64_t)(_rec_) == 0ULL)\
	        return CLC_ERECORD ;	// was CLC_EARG
	// -------------------------------
#else
	#define clc_assert_in_ptr_m(a1)        /* nop */
	#define clc_assert_alignment_m(a1)     /* nop */
	#define clc_assert_recval_m(a1)        /* nop */
#endif

/* NOTE: 
   Use compiler directive -DCLC_ASSERT_OUT_ARGS to enable OUT args asserts.
 */
 
#if defined(CLC_ASSERT_OUT_ARGS)
	// -------------------------------
	#define clc_assert_out_ptr_m(_p_)\
	    if((_p_)==NULL)\
	        return CLC_EPOINTER ;
	// -------------------------------
#else
	#define clc_assert_out_ptr_m(a1)       /* nop */
#endif
/* ------------------------------------------------------------------------- */
/* debug macros -------------- */

// TODO: INARGS, etc. -DCLC_DEBUG to flip emit to stderr.

/* ------------------------------------------------------------------------- */
/* api ----------------------- */

/* -------------------------------------------------------------------------*/
/* generic unit ops ----------*/

extern clc_stat clc_reset (void *const);
extern clc_stat clc_init (void *const);
extern clc_stat clc_get (void const*const, uint8_t, uint64_t*, uint8_t*);
extern clc_stat clc_del (void *const, uint8_t, uint64_t*, uint8_t*);
extern clc_stat clc_len (void const*const, uint8_t*);
extern clc_stat clc_cmeta (void const*const, uint8_t*);
extern clc_stat clc_rmeta (void const*const, uint8_t, uint8_t*);

extern void     clc_dump (FILE *restrict, void const*const);
extern void     clc_debug (void const*const);
extern void     clc_dump_inorder (FILE *restrict, void const*const);
extern void     clc_debug_inorder (void const*const);

/// STORES ///////////////////////////////////////////////////////////////////

/* -------------------------------------------------------------------------*/
/* container : stack ---------*/

extern clc_stat clc_stack_push (void*const, uint64_t, uint8_t*);
extern clc_stat clc_stack_pop (void*const, uint64_t*, uint8_t*);

extern clc_stat clc_stack_push_sync (void*const, uint64_t, uint8_t*);
extern clc_stat clc_stack_pop_sync (void*const, uint64_t*, uint8_t*);

/* -------------------------------------------------------------------------*/
/* container : queue ---------*/

extern clc_stat clc_queue_enqueue (void*const, uint64_t, uint8_t*);
extern clc_stat clc_queue_dequeue (void*const, uint64_t*, uint8_t*);

extern clc_stat clc_queue_enqueue_sync (void*const, uint64_t, uint8_t*);
extern clc_stat clc_queue_dequeue_sync (void*const, uint64_t*, uint8_t*);

/* -------------------------------------------------------------------------*/
/* container : list ----------*/

extern clc_stat clc_list_add     (void*const, uint64_t, uint8_t*);
extern clc_stat clc_list_append  (void*const, uint64_t, uint8_t*);
extern clc_stat clc_list_insert  (void*const, uint8_t, uint64_t, uint8_t*);
extern clc_stat clc_list_get     (void*const, uint8_t, uint64_t*, uint8_t*);
extern clc_stat clc_list_del     (void*const, uint8_t, uint64_t*, uint8_t*);

extern clc_stat clc_list_add_sync    (void*const, uint64_t, uint8_t*);
extern clc_stat clc_list_append_sync (void*const, uint64_t, uint8_t*);
extern clc_stat clc_list_insert_sync (void*const, uint8_t, uint64_t, uint8_t*);
extern clc_stat clc_list_get_sync    (void*const, uint8_t, uint64_t*, uint8_t*);
extern clc_stat clc_list_del_sync    (void*const, uint8_t, uint64_t*, uint8_t*);

#ifdef CONCEPTUAL_RESOLVED // REVU: these belong to higher level constructs
/* -------------------------------------------------------------------------*/
/* container : set -----------*/

extern clc_stat clc_set_add    ();
extern clc_stat clc_set_update ();
extern clc_stat clc_set_get    (void*const, uint64_t*, uint64_t, uint8_t*);
extern clc_stat clc_set_put    (void*const, uint64_t, uint64_t, uint8_t*);
extern clc_stat clc_set_putr   (void*const, uint64_t, uint64_t, uint8_t*);

/* -------------------------------------------------------------------------*/
/* container : map -----------*/

extern clc_stat clc_map_get  (void*const, uint64_t*, uint64_t, uint8_t*);
extern clc_stat clc_map_put  (void*const, uint64_t, uint8_t*);
extern clc_stat clc_map_putx (void*const, uint64_t, uint64_t, uint8_t*);
extern clc_stat clc_map_putr (void*const, uint64_t, uint64_t, uint8_t*);

extern clc_stat clc_map_get_sync  (void*const, uint64_t*, uint64_t, uint8_t*);
extern clc_stat clc_map_put_sync  (void*const, uint64_t, uint8_t*);
extern clc_stat clc_map_putr_sync (void*const, uint64_t, uint64_t, uint8_t*);
extern clc_stat clc_map_putx_sync (void*const, uint64_t, uint64_t, uint8_t*);
#endif // CONCEPTUAL_RESOLVED

/// CACHES ///////////////////////////////////////////////////////////////////

/* -------------------------------------------------------------------------*/
/* cache : lru ---------------*/

extern clc_stat clc_cache_get_lru  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_lru  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_lru  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_lru (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putr_lru (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

extern clc_stat clc_cache_putr_lru_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_lru_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_lru_sync  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_get_lru_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_lru_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

/* -------------------------------------------------------------------------*/
/* cache : fifo ---------------*/

extern clc_stat clc_cache_get_fifo  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_fifo  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_fifo  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_fifo (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putr_fifo (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

extern clc_stat clc_cache_putr_fifo_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_fifo_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_fifo_sync  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_get_fifo_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_fifo_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

/* -------------------------------------------------------------------------*/
/* cache : 2Q ---------------*/

extern clc_stat clc_cache_get_2q  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_2q  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_2q  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_2q (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putr_2q (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

extern clc_stat clc_cache_putr_2q_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_putx_2q_sync (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_put_2q_sync  (void*const, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_get_2q_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);
extern clc_stat clc_cache_del_2q_sync  (void*const, uint64_t, uint64_t, uint64_t*, uint8_t*);

#endif //_LIBCLC_H_ 
