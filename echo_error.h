// echo_error.h

/*
    This file is part of L-Echo.

    L-Echo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    L-Echo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with L-Echo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <iostream>

#ifndef __ECHO_STATUS__
	#define __ECHO_STATUS__
	/** The Internet is serious business! And so is statuses */
	enum STATUS { FAIL = 0, WIN = 1};
#endif

/// All of these functions are used by the loader
/** Report an error while loading
 * @param msg The error message
 */
void lderr(const char* msg);
/** Report an error while loading
 * @param msg1 The first error message
 * @param msg2 The second error message, concatenated onto this first.
 */
void lderr(const char* msg1, const char* msg2);
/** Reports a memory error (usually the inability to allocate memory), and quits
 * Used by the loader
 */
void ldmemerr();
/** Report a warning while loading
 * @param msg The warning message
 */
void ldwarn(const char* msg);

/** Stands for GENeral MEMory ERRor;
 * Reports a memory error (usually the inability to allocate memory), and quits
 */
void genmemerr();
/** Report a generic error, and quits
 * @param msg The error message
 */
void echo_error(const char* msg);

#ifdef STRICT_MEM
	/// Check the pointer, and if it's NULL, just quit
	#define CHKPTR(ptr) 	if((ptr) == NULL) genmemerr();
	/// Same as LD_CHKPTR, but used by the loader
	#define LD_CHKPTR(ptr)	if((ptr) == NULL) ldmemerr();
#else
	/// Eat the line
	#define CHKPTR(ptr)
	#define LD_CHKPTR(ptr)
#endif
