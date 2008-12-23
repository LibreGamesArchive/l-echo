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

/// Needed for quitting
#include <cstdlib>

/// Needed for printing
#include <echo_debug.h>
#include <echo_error.h>

/** Report an error while loading
 * @param msg The error message
 */
void lderr(const char* msg)
{
	ECHO_PRINT("Load Error: %s\n", msg);
}
/** Reports a memory error (usually the inability to allocate memory), and quits\n
 * Used by the loader
 */
void ldmemerr()
{
	lderr("cannot allocate memory!");
	std::exit(1);
}
/** Report an error while loading
 * @param msg1 The first error message
 * @param msg2 The second error message, concatenated onto this first.
 */
void lderr(const char* msg1, const char* msg2)
{
	ECHO_PRINT("Load Error: %s%s\n", msg1, msg2);
}
/** Report a warning while loading
 * @param msg The warning message
 */
void ldwarn(const char* msg)
{
	ECHO_PRINT("Load Warning: %s\n", msg);
}
/** Stands for GENeral MEMory ERRor;\n
 * Reports a memory error (usually the inability to allocate memory), and quits
 */
void genmemerr()
{
	ECHO_PRINT("Cannot allocate memory!\n");
	std::exit(1);
}
/** Report a generic error, and quits
 * @param msg The error message
 */
void echo_error(const char* msg)
{
	ECHO_PRINT(msg);
	std::exit(1);
}
