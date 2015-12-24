/* The Incomparably Gracious! */
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


#ifndef __PRNG_H__
#define __PRNG_H__

#include <stdlib.h>
#include <stdint.h>

/* ///////////////////////////////////////////////////////////////////// */
/* /// XOR Shift Star 1024 PRNG //////////////////////////////////////// */
/* ///////////////////////////////////////////////////////////////////// */

/* --------------------------------------------------------------------- */
/* forward declarations ---- */

/* private.
 * struct xoss defines the internal structural of the prng.
 * internal structure is private. */
struct xoss;

/* private
 * c.f. struct xoss */
typedef struct xoss xoss;

/* private.
 * main interal ops func for the prng, returns the next 
 * (seed-)deterministic ULL pseudo-random number in the
 * sequence. */
uint64_t xoss_next(xoss* const p);

/* --------------------------------------------------------------------- */
/* api --------------------- */

/* public.
 * New XOR-Shift-Star 1024 prng.
 * Heap allocates a new instance of the xoss object and 
 * initializes it with the default seed.
 * Use dispose() to free instance. */
xoss* const xoss_new (void);

/* public.
 * New XOR-Shift-Star 1024 prng using seed.
 * See xoss_new() for other details.*/
xoss* const xoss_newseed(const uint64_t seed);

/* public.
 * Dispose frees the memory allocated for the prng and
 * sets the instance pointer to null/(void*) 0. */
void xoss_dispose (xoss ** const ptr);

/* public.
 * Returns the next uint64_t in the pseudo-random number 
 * sequence */
uint64_t xoss_uint64(xoss* const p);

/* public.
 * Returns the next uint32_t in the pseudo-random number 
 * sequence */
uint32_t xoss_uint32(xoss* const p);

#endif /* __PRNG_H__ */
