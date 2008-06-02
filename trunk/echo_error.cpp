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

#include <echo_error.h>

void lderr(const char* msg)
{
	std::cout << "Load Error: " << msg << std::endl;
	std::exit(1);
}
void ldmemerr()
{
	lderr("cannot allocate memory!");
}
void lderr(const char* msg1, const char* msg2)
{
	char* msg = new char[strlen(msg1) + strlen(msg2) + 1];
	LD_CHKPTR(msg);
	lderr(strcat(strcpy(msg, msg1), msg2));
}

void ldwarn(const char* msg)
{
	std::cout << "Load Warning: " << msg << std::endl;
}

void genmemerr()
{
	std::cout << "Cannot allocate memory!" << std::endl;
}

