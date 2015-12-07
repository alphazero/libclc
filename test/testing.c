/* FRIEND */
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

#include <libc.h>
#include "testing.h"
 
testrep_t * report(char const * name, const testres_t result, char const * info) {
	testrep_t * r = (testrep_t *) calloc(1, sizeof(testrep_t));
	r->name = name;
	r->info = info;
	r->result = result;
	return r;
}

testrep_t * failed(char const * name, char const * info) {
	return report(name, FAIL, info);
}

testrep_t * passed(char const * name) {
	return report(name, PASS, 0);
}

void runsuite(char const * name, test_fn const tests[]) {
	printf("\n-- testing: %s --\n", name);
	unsigned n = 0;
	while (tests[n] != 0) {
		emit_report( tests[n]() );
		n++;
	}
	return;
}

const testres_t emit_report(testrep_t const * report) {

	const testres_t result = report->result;
	if (result != PASS) 
		printf("FAIL");
	else 
		printf("PASS");
	
	printf(": %s", report->name);

	if(report->info != 0 && strlen(report->info) > 0) 
		printf("\n\t\t%s\n", report->info);
	else
		printf("\n");

	free ((void *)report);

	return result;
}

