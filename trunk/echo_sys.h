// echo_sys.h

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

//void echo_sleep(int millis);

#ifndef ARM9
	#ifdef WIN32
		#include <windows.h>
		#define ECHO_SLEEP(arg)	Sleep(arg)
	#else		//Assume it's Unix
		#warning "Assuming system is Unix-based, using usleep"
		#include <unistd.h>
		#define ECHO_SLEEP(arg) usleep((arg) * 1000)
	#endif
#endif

