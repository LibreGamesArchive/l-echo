// main.cpp

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

#if defined(ARM9) || defined(ARM7)
	#define ECHO_NDS		1
#elif defined(HW_RVL)
	#define ECHO_WII		1
#elif defined(HW_DOL)
	#define ECHO_GCN		1
#else
	#define ECHO_PC			1
	#ifdef __MACH__
		#define ECHO_OSX	1
	#elif defined(WIN32)
		#define ECHO_WIN	1
	#else
		#define ECHO_UNIX	1
	#endif
#endif

