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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option.h"

/* --------------------------------------------------------------------- */
/* cmdline options util ----- */

int option_cnt(option_spec spec[]) {
	int n = 0;
	while(spec[n].ctype != OPT_TYPE_NIL) {
		n++;
	}
	return n;
}

void emit_usage(option_spec spec[]) {

	int const n = option_cnt(spec);

	char * typ[2] = {"-", "--"};
	char * req[2] = {"     ", "(req)"};
	fprintf(stderr, "usage: <app> [flag] [option]\n");
	for(unsigned i=0; i<n; i++) {
		char * _typ = typ[spec[i].is_flag];
		char * _req = req[spec[i].required];
		char * _nam = spec[i].name;

		// flag|option decoration + name and required signifier
		fprintf(stderr, " %s%2s%-14s", _req, _typ, _nam);
		// then usage on same line.
		fprintf(stderr, " %s\n", spec[i].usage);

		// Finally default value, if applicable.
		// required options by definition can't have default
		// values or even if they do, we ignore that.
		//
		// flags are a special case, since we are using flags
		// for bool type options.
		if (!spec[i].required && !spec[i].is_flag) {
			fprintf(stderr, "                             * default => ");
			void const * const vptr = spec[i].vptr;
			char const * const fmt = spec[i].fmt;
			switch (spec[i].ctype) {
				case OPT_TYPE_BOOL:
					// for now, assume only flag options are used for bool type.
					// (self-reminder: this is not a general purpose cmdline lib.)
					if (*(bool*)vptr == true) 
						fprintf(stderr, "true");
					else
						fprintf(stderr, "false");
					break;
				case OPT_TYPE_CHAR:
					fprintf(stderr, fmt, *((char*)vptr));
					break;
				case OPT_TYPE_STRING:
					fprintf(stderr, fmt, *((char**)vptr));  // REVU: fix this BUG
					break;
				case OPT_TYPE_UINT:
					fprintf(stderr, fmt, *((unsigned int*)spec[i].vptr));
					break;
				case OPT_TYPE_UINT8:
					fprintf(stderr, fmt, *((uint8_t *)spec[i].vptr));
					break;
				case OPT_TYPE_UINT16:
					fprintf(stderr, fmt, *((uint16_t *)spec[i].vptr));
					break;
				case OPT_TYPE_UINT32:
					fprintf(stderr, fmt, *((uint32_t *)spec[i].vptr));
					break;
				case OPT_TYPE_UINT64:
					fprintf(stderr, fmt, *((uint64_t *)spec[i].vptr));
					break;
				case OPT_TYPE_INT:
					fprintf(stderr, fmt, *((int*)spec[i].vptr));
					break;
				case OPT_TYPE_INT8:
					fprintf(stderr, fmt, *((int8_t *)spec[i].vptr));
					break;
				case OPT_TYPE_INT16:
					fprintf(stderr, fmt, *((int16_t *)spec[i].vptr));
					break;
				case OPT_TYPE_INT32:
					fprintf(stderr, fmt, *((int32_t *)spec[i].vptr));
					break;
				case OPT_TYPE_INT64:
					fprintf(stderr, fmt, *((int64_t *)spec[i].vptr));
					break;
				default:
					fprintf(stderr, "\n** BUG ** unknown option c-type:%d\n", spec[i].ctype);
					return;
					/* bug */
			}
			fprintf(stderr, "\n");
		}
	}
}

// REVU: this can be tightended up.
int parse_options (int const argc, char ** argv, option_spec options[]) {

	// note number of spec'd options
	int const optcnt = option_cnt(options);

	if (optcnt == 0) {
		return 0; // REVU: possibly better non-zero here?
	}

	// note number of required options
	int reqcnt = 0;
	for(int n=0; n<optcnt; n++) 
		if (options[optcnt].required == true) 
			reqcnt++;

	if (reqcnt > (argc-1)) {
		goto required_options;
	}

	// parse 'em
	char const * const prefix = "error parsing commandline options:";
	for(int n=1; n<argc; ) {
		// expect -opt val or --flag
		char const * const arg = argv[n];
		size_t const arglen = strlen(arg);
		if ( arglen < 2 || arg[0] != '-') {
			fprintf(stderr, "%s unexpected ... %s ...\n", prefix, arg);
			return -1;
		}
		else if (arglen > 2 && arg[1] == '-') {
			if(strncmp(arg+2, "help", 4) == 0) {
				return -1;
			}
			n += 1;
			bool found = false;
			for(unsigned i=0; i<optcnt; i++) {
				if (! options[i].is_flag) continue;
				if(strcmp(arg+2, options[i].name) == 0) {
					bool * _v = options[i].vptr;
					*_v = 1;
					options[i].provided = true;
					found = true;
					break;
				}
			}
			if (!found) {
				fprintf(stderr, "%s unknown flag .. %s\n", prefix, arg);
				return -1;
			}
		}
		else if (arglen >= 2 && arg[0] == '-') {
			if(strncmp(arg+1, "h", 1) == 0) {
				return -1;
			}
			n += 1;
			if (n == argc) {
				fprintf(stderr, "%s expected value for option .. %s\n", prefix, arg);
				return -1;
			}
			char const * const optval = argv[n];
			n += 1;

			bool found = false;
			for(unsigned i=0; i<optcnt; i++) {
				if (options[i].is_flag) continue;
				if(strcmp(arg+1, options[i].name) == 0) {
					if(options[i].ctype != OPT_TYPE_BOOL) {
						int const r = sscanf (optval, options[i].fmt, options[i].vptr);
						if (r != 1) {
							// TODO add pprint for OPT TYPES
							fprintf(stderr, "%s: expected ctype(%d) val for option .. %s %s ..\n", 
									prefix, options[i].ctype, arg, optval);
							return -1;
						}
					} else {
						unsigned char _v = 0;
						int const r = sscanf (optval, options[i].fmt, &_v);
						if (r != 1 || (_v != 't' && _v != 'f')) {
							fprintf(stderr, "%s: expected either 't' or 'f' for bool option .. %s %s ..\n", 
									prefix, arg, optval);
							return -1;
						}
						*(bool*)options[i].vptr = _v == 't';
					}
					options[i].provided = true;
					found = true;
					break;
				}
			}
			if (!found) {
				fprintf(stderr, "%s unknown flag .. %s\n", prefix, arg);
				return -1;
			}
		} 
	}

required_options: /* verify we have required options */;
	char * typ[] = {"option", "flag"};
	char * dec[] = {"-", "--"};
	bool usage_error = false;
	for(int n=0; n<optcnt; n++) {
		char * const _typ = typ[options[n].is_flag];
		char * const _dec = dec[options[n].is_flag];
		if (options[n].required && !options[n].provided) {
			fprintf(stderr, "*** usage-error: %s %s%s is required\n", 
					_typ, _dec, options[n].name);
			usage_error = true;
		}
	}
	if (usage_error) return -1;

	return 0;
}


