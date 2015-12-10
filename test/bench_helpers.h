// ~!!! Ya NASIR !!!~

#ifndef __CLC_BENCH_HELPERS_H_
#define __CLC_BENCH_HELPERS_H_

// REVU: temp alloc large cnt TODO use aligned alloc
#define bench_alloc_and_init_m(p, n)\
{\
	p = calloc (n, sizeof (struct clc_unit));\
	struct clc_unit * pcu = clc_unitptr_m(p);\
	for (unsigned i=0; i<n; i++) {\
		clc_stat s = __clc_init (pcu + i);\
		if(clc_is_error_m(s)) {\
			clc_error_println_m(s, "init");\
			return s;\
		}\
	}\
}

// NOTE barfs on dots (.) so just used for the (_) variant
#define bench_join_m(a,b,c) a ##b ##c

#endif //__CLC_BENCH_HELPERS_H_
