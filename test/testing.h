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

#ifndef _CLC_TESTING_H_CLC_
#define _CLC_TESTING_H_CLC_

#include <stdlib.h>

/*************************************************************************/
/* a very basic test suite                                               */ 
/*************************************************************************/

/* ------------------------------------------------------------------------- */
/* types --------------------- */

typedef unsigned testres_t;
#define PASS ((testres_t)1)
#define FAIL ((testres_t)0)

typedef char const * string_t;
typedef struct testspec {
	unsigned foo;
} testspec_t;

typedef struct testrep {
	string_t   name;
	string_t   info;
 	testres_t  result;	
} testrep_t;

/* suitefp_t is pointer to a testsuit function */
typedef void (*testsuite_fn) (void);

/* testfp_t is pointer to a test function */
typedef testrep_t const * (*test_fn) (void);

/* ------------------------------------------------------------------------- */
/* api ----------------------- */

/* note: function will free the testrep_t allocations */
const testres_t emit_report(testrep_t const * tr);

/* emits suite name and runs it.
 * arg 'tests' should be a 0/NULL terminated array */
void runsuite(char const * name, test_fn const tests[]);

/* allocates and inits a new tsetrep_t struct from args */
testrep_t * report(char const * name, const testres_t result, char const * info) ;

/* allocates failed case tsetrep_t struct from args */
testrep_t * failed(char const * name, char const * info) ;

/* allocates passed case tsetrep_t struct from args */
testrep_t * passed(char const * name) ;

#endif //_CLC_TESTING_H_CLC_
