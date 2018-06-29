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

#ifndef _CLC_UTIL_HASH_H_
#define _CLC_UTIL_HASH_H_

#include <stdbool.h> 
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* hash functions ---------- */

/* ---------------------------------------------------------------------
 * FNV-1a hash functions.
 * --------------------------------------------------------------------- */

/* FNV-1a 64bit offset-basis */
#define clc_fnv1a_u64_xof ((uint64_t)0xcbf29ce484222325)

/* FNV-1a 64bit prime */
#define clc_fnv1a_u64_prime ((uint64_t)0x100000001b3)

/* hash unsigned 64bit key to unsigned 64bit hash */
extern uint64_t clc_fnv1a_hash_u64 (uint64_t k);

/* ---------------------------------------------------------------------
 * tw_hash_<t> funcs.
 *
 * Thomas Wang's hash. 
 * Source (use archive.org) http://www.cris.com/~Ttwang/tech/inthash.htm
 * Note it sometimes emits 0 value hash 
 * --------------------------------------------------------------------- */

/* hash unsigned 32bit key to unsigned 32bit hash. */
extern uint32_t tw_hash_u32 (uint32_t k) ;

/* hash unsigned 16bit key to unsigned 32bit hash. */
extern uint32_t tw_hash_u16 (uint16_t k16) ;

/* hash unsigned 08bit key to unsigned 32bit hash. */
extern uint32_t tw_hash_u8  (uint8_t k8) ;

/* hash unsigned 64bit key to unsigned 64bit hash. */
extern uint64_t tw_hash_u64 (uint64_t k) ;

/* a bit faster as macro but also included as a function.
 * usage example (typical):
 * 	uint32_t key;
 * 	uint32_t hash = 0;
 * 	TW_HASH_U32 (key, hash);
 */ 
#define TW_HASH_U32(_k_, _h_) \
	{                         \
	_h_ = _k_;                \
	_h_ += ~(_h_ << 15);      \
	_h_ ^=  (_h_ >> 10);      \
	_h_ +=  (_h_ << 3);       \
	_h_ ^=  (_h_ >> 6);       \
	_h_ += ~(_h_ << 11);      \
	_h_ ^=  (_h_ >> 16);      \
	}

#define TW_HASH_U16(_k_16, _h_) \
	{                           \
	TW_HASH_U32 ((uint32_t)_k_16, _h_); \
	}

#define TW_HASH_U8(_k_8, _h_)   \
	{                           \
	TW_HASH_U32 ((uint32_t)_k_8, _h_); \
	}

#define TW_HASH_U64(_k_, _h_) \
	{                         \
	_h_ += ~(_k_ << 32);      \
	_h_ ^=  (_h_ >> 22);      \
	_h_ += ~(_h_ << 13);      \
	_h_ ^=  (_h_ >> 8);       \
	_h_ +=  (_h_ << 3);       \
	_h_ ^=  (_h_ >> 15);      \
	_h_ += ~(_h_ << 27);      \
	_h_ ^=  (_h_ >> 31);      \
	}

#endif //_CLC_UTIL_HASH_H_
