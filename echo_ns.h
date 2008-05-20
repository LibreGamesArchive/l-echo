// echo_ns.h

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

#include <echo_math.h>
#include <grid.h>

#ifndef __ECHO_NS__
#define __ECHO_NS__
namespace echo_ns
{
	extern grid* hole_grid;
	extern vector3f angle;
	extern grid* start;
	extern grid* grid1;
	extern grid* grid2;
	extern int paused;
	extern int num_goals;
	extern float grid1per, grid2per, startper;

	void toggle_pause();
	void kill_char();
	void setup_char(grid* g1);
	vector3f* step_char();
};
#endif

