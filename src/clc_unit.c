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

#include <stdio.h>

#include "libclc.h"
#include "libclc/inline.h"

/* ------------------------------------------------------------------------- */
/* generic unit ops ---------- */

void     
clc_dump (FILE * restrict out, void const * const p) {
	__clc_dump (out, p);
	return;
}

void
clc_debug (void const * const p) {
	__clc_debug (p);
	return;
}

void     
clc_dump_inorder (FILE * restrict out, void const * const p) {
	__clc_dump_inorder (out, p);
	return;
}

void
clc_debug_inorder (void const * const p) {
	__clc_debug_inorder (p);
	return;
}

clc_stat
clc_len (void const * const p, uint8_t * len) {
	return __clc_len (p, len);
}

clc_stat
clc_reset(void *const p) {
	return __clc_reset (p);
}

clc_stat
clc_clear(void *const p) {
	return __clc_clear (p);
}

clc_stat 
clc_init(void *const p) {
	return __clc_init (p);
}

