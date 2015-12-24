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

#ifndef _CLC_OPTION_H_
#define _CLC_OPTION_H_

#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* cmdline options util ---- */

#define OPT_TYPE_NIL     0 // for convenient null terminating the option spec arrays
#define OPT_TYPE_SIZE_T  1
#define OPT_TYPE_BOOL    2
#define OPT_TYPE_CHAR    3
#define OPT_TYPE_STRING  4
#define OPT_TYPE_UINT   10
#define OPT_TYPE_UINT8  11
#define OPT_TYPE_UINT16 12
#define OPT_TYPE_UINT32 13
#define OPT_TYPE_UINT64 14
#define OPT_TYPE_INT    20
#define OPT_TYPE_INT8   21
#define OPT_TYPE_INT16  22
#define OPT_TYPE_INT32  23
#define OPT_TYPE_INT64  24

/* specifies the option */
typedef struct {
	char * const  name;
	const bool    is_flag;
	char * const  usage;
	char * const  fmt;
	uint8_t const ctype;
	void *        vptr;
	bool const    required;
	bool          provided;
} option_spec;

/* general option definition macro */
#define DEFINE_OPTION(_name, _usage, _fmt, _ctype, _vptr, _required) \
	{ _name, false, _usage, _fmt, _ctype, &(_vptr), _required, false }

/* NULL terminate the option spec array */
#define END_OPTIONSPEC() \
	{ NULL, false, NULL, 0, OPT_TYPE_NIL, NULL, false }

/* flag definition (e.g. --my-flag) */
/* flags by definition are optional */
#define DEFINE_FLAG(_name, _usage, _vptr) \
	{ _name, true, _usage, 0, OPT_TYPE_BOOL, &(_vptr), false }

/* BOOL option definition (e.g. -my-option <val>) */
/* accept {t[rue], f[alse]} */ 
#define BOOL_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%hhc", OPT_TYPE_BOOL, (_vptr), false)

#define BOOL_REQ_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%hhc", OPT_TYPE_BOOL, (_vptr), true)

/* UINT option definition (e.g. -my-option <val>) */
#define UINT_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%d", OPT_TYPE_UINT, (_vptr), false)

#define UINT_REQ_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%d", OPT_TYPE_UINT, (_vptr), true)

/* option definition (e.g. -my-option <val>) */
#define CHAR_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%hhc", OPT_TYPE_CHAR, (_vptr), false)

#define CHAR_REQ_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%hhc", OPT_TYPE_CHAR, (_vptr), true)

/* option definition (e.g. -my-option <val>) */
#define STRING_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%s", OPT_TYPE_STRING, (_vptr), false)

#define STRING_REQ_OPTION(_name, _usage, _vptr) \
	DEFINE_OPTION(_name, _usage, "%s", OPT_TYPE_STRING, (_vptr), true)

/* emits a somewhat pretty print usage given an
 * array of option_spec */
extern void emit_usage(option_spec spec[]) ;

/* parses specified options from the cmdline argc/argv */
extern int parse_options (int const argc, char ** argv, option_spec options[]);

#endif // _CLC_OPTION_H_
