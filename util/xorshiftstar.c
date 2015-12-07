/* DOOST */
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


#include <errno.h>
#include "xorshiftstar.h"

/* --------------------------------------------------------------------- */
/* private ----------------- */

/* prng state */
struct xoss {
	uint64_t	s[16];
	uint8_t 	p;
	uint64_t (*next)(void);
};

/* The Seed */
const uint64_t _s0[16] = {
	0xEA221D1E5C8DB483,
	0xF89369282348A220,
	0x7022326276090608,
	0x1618FCC12E583E30,
	0xF7E7C005F85EFC69,
	0x132B746F9C2FF047,
	0x338324A69CBDC6B5,
	0x2B91B21FAAB58FE0,
	0x85CB192105B8B12B,
	0x5A9EC4772C1B642D,
	0xFEA936CB4A0CEA20,
	0x6865516844ED9BBD,
	0x16C12E5DCC3D7365,
	0x0BBDE3B5363FA6E9,
	0xD454C1D29E450885,
	0x480EB817028DB197
};

/* ULL.MAX-VALUE */
const uint64_t xoss_k = 1181783497276652981;

/* --------------------------------------------------------------------- */
/* prototypes -------------- */

xoss* const xoss_new_from_seeds (uint64_t const s[16]); 

// copied from clc_util.h - rather not have mutual deps in the util files.
// REVU TODO check Thomas W.'s original copyright notices and add it here.
#define tw_hash_u64(_k_, _h_) \
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

/* --------------------------------------------------------------------- */
/* public api -------------- */

/* NOTE: refer to the header file for api docs */

xoss* const xoss_newseed(const uint64_t seed) {
	// use seed to generate 16 ULLs
	uint64_t s[16] = { seed, }; // use seed as first element
	for(int i=1; i<16; i++) {
		tw_hash_u64	(s[i-1], s[i]);
	}
	return xoss_new_from_seeds(s);
}

// use hardcoded _s0[] to init
xoss* const xoss_new (void) {
	return xoss_new_from_seeds(_s0);
}

xoss* const xoss_new_from_seeds (uint64_t const s[16]) {
	xoss* p = (xoss* const) calloc( (size_t) 1, sizeof (xoss) );
	if (p == 0) {
		exit(errno);
	}
	// initialize it
	auto int i;
	for (i=0; i<16; i++) {
		p->s[i] = s[i]; 
	};
	p->p = 0;
	
	return p;
}

void xoss_dispose (xoss ** const pp) {
	free ((void*) *pp);
	*pp = (void*)0;

	return;
}

inline
uint64_t xoss_uint64 (xoss * const p) {
	return xoss_next(p);
}

inline
uint32_t xoss_uint32 (xoss * const p) {
	// return the hi bits 
	return (uint32_t) (xoss_next(p) >> 32); 
}

/* --------------------------------------------------------------------- */
/* internal ops ------------ */

/* returns the next pseudo-random number in the
 * (seed-)deterministic sequence. private. */
inline 
uint64_t xoss_next (xoss* const x) {

	register uint64_t s0 = x->s[x->p];
	x->p = (x->p + 1) & 0xF;
	register uint64_t s1 = x->s[x->p];

	s1 ^= s1 << 31; // A
	s1 ^= s1 >> 11; // B
	s0 ^= s0 >> 30; // C

	/* update state */
	x->s[x->p] = s0 ^ s1;
	return x->s[x->p] * xoss_k;
}

