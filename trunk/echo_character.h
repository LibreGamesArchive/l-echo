// echo_character.h

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
#include <echo_stage.h>

#ifndef __ECHO_CHARACTER__
#define __ECHO_CHARACTER__
class echo_char
{
	protected:
		grid* start;
		grid* grid1;
		grid* grid2;
		int paused;
		int num_goals;
		float grid1per, grid2per, startper, speed, dist;
	
	public:
		echo_char();
		echo_char(grid* start);
		void init(grid* g1);
		void reset();
		
		vector3f* step();
		void kill();
		void toggle_pause();
		void next_grid();
		void change_speed();
		int is_paused();
		int num_goals_reached();
};
#endif

