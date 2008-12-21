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

#include <echo_debug.h>
#include <echo_error.h>

void lderr(const char* msg)
{
	ECHO_PRINT("Load Error: %s\n", msg);
	//std::exit(1);
}
void ldmemerr()
{
	lderr("cannot allocate memory!");
	std::exit(1);
}
void lderr(const char* msg1, const char* msg2)
{
	ECHO_PRINT("Load Error: %s%s\n", msg1, msg2);
	//std::exit(1);
}

void ldwarn(const char* msg)
{
	ECHO_PRINT("Load Warning: %s\n", msg);
}

void genmemerr()
{
	ECHO_PRINT("Cannot allocate memory!\n");
	std::exit(1);
}

void echo_error(const char* msg)
{
	ECHO_PRINT(msg);
	std::exit(1);
}
