// !!! FRIEND !!!
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
 * copies of the Software, subject to the following conditions:
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

#include <stdlib.h> 
#include "hash.h"

/* --------------------------------------------------------------------- */
/* hash functions  --------- */

/* ---------------------------------------------------------------------
 * tw_hash_<t> funcs.
 *
 * Thomas Wang's hash. 
 * Source (use archive.org) http://www.cris.com/~Ttwang/tech/inthash.htm
 * Note it sometimes emits 0 value hash 
 * --------------------------------------------------------------------- */

/* hash -> 32b -> 32b */
uint32_t clc_tw_hash_u32 (uint32_t key) {
	key += ~(key << 15);
	key ^=  (key >> 10);
	key +=  (key << 3);
	key ^=  (key >> 6);
	key += ~(key << 11);
	key ^=  (key >> 16);

	return key;
}

/* hash -> 16b -> 32b */
uint32_t clc_tw_hash_u16 (uint16_t k16) {
	uint32_t key = k16;
	key += ~(key << 15);
	key ^=  (key >> 10);
	key +=  (key << 3);
	key ^=  (key >> 6);
	key += ~(key << 11);
	key ^=  (key >> 16);

	return key;
}

/* hash ->  8b -> 32b */
uint32_t clc_tw_hash_u8 (uint8_t k8) {
	uint32_t key = k8;
	key += ~(key << 15);
	key ^=  (key >> 10);
	key +=  (key << 3);
	key ^=  (key >> 6);
	key += ~(key << 11);
	key ^=  (key >> 16);

	return key;
}

/* hash -> 64b -> 64b */
uint64_t clc_tw_hash_u64 (uint64_t key) {
	key += ~(key << 32);
	key ^=  (key >> 22);
	key += ~(key << 13);
	key ^=  (key >> 8);
	key +=  (key << 3);
	key ^=  (key >> 15);
	key += ~(key << 27);
	key ^=  (key >> 31);

	return  key;
}
								 
