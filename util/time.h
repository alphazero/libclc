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

#ifndef _CLC_UTIL_TIME_H_
#define  _CLC_UTIL_TIME_H_

#include <stdlib.h>
#include <sys/time.h>

/* --------------------------------------------------------------------- */
/* time & bench related ---- */

/* unit conversion */

#define USECS_PER_SEC   ((long long)(1000000))
#define NSECS_PER_SEC   ((long long)(1000000000))
#define NSECS_PER_USEC  ((long long)(1000))

#define USEC_TO_NSEC(_v_) \
	((long long)(_v_)*(NSECS_PER_USEC))

#define NSECS_PER_OP(_cnt_, _nst_)  \
	((double)(_nst_) / ((double)(_cnt_)))

/* prototypes */

/* Returns the current system time in usecs.
 * NOTE: will exit(errno) if system time func faults */
long long time_usec (void);

/* Returns the difference in usecs between the current system 
 * time and the argument 'd0'. It is expected that 'd0' is a
 * usec measure.
 * NOTE: will exit(errno) if system time func faults */
long long dt_usec (long long const t0);

/* prints out a 2 line timed operations repot. first line is the info string. */
#define TIME_OPS2(_info_, _t0_, _opcnt)\
    {\
	const long long dt = dt_usec(_t0_);\
	const long long dt_nsec = USEC_TO_NSEC(dt);\
	const double nsperop = NSECS_PER_OP(_opcnt, dt_nsec);\
	printf("-- timed-ops: %s -- ops:%zd nsec/op:%f usec:%llu\n", _info_, _opcnt, nsperop, dt);\
	}
 
#define TIME_OPS(_t0_, _opcnt)\
    {\
	const long long dt = dt_usec(_t0_);\
	const long long dt_nsec = USEC_TO_NSEC(dt);\
	const double nsperop = NSECS_PER_OP(_opcnt, dt_nsec);\
	printf("-- timed-ops: ops:%zd | nsec/op:%9.3f | usec:%6llu\n", _opcnt, nsperop, dt);\
	}

/* for use with R, etc */
#define TIME_OPS_CSV(_t0_, _opcnt)\
    {\
	const long long dt = dt_usec(_t0_);\
	const long long dt_nsec = USEC_TO_NSEC(dt);\
	const double nsperop = NSECS_PER_OP(_opcnt, dt_nsec);\
	printf("%f, %llu\n", nsperop, dt);\
	}

#define TIME_OPS_MINIMAL(_t0_, _opcnt)\
    {\
	const long long dt = dt_usec(_t0_);\
	const long long dt_nsec = USEC_TO_NSEC(dt);\
	const double nsperop = NSECS_PER_OP(_opcnt, dt_nsec);\
	printf("%f\n", nsperop);\
	}
#endif // _CLC_UTIL_TIME_H_

