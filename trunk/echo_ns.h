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

#include <echo_character.h>
#include <echo_math.h>
#include <grid.h>
#include <echo_stage.h>

#ifndef __ECHO_NS__
#define __ECHO_NS__
namespace echo_ns
{
	extern grid* hole_grid;
	extern vector3f angle;
	extern stage* current_stage;
	extern echo_char* main_char;

	void init(stage* st);
	void start();
	
	void toggle_pause();
	int is_paused();
	void kill_char();
	void setup_char(grid* g1);
	vector3f* step_char();
	void draw();
	void reset();
	int num_goals_reached();
	int goals_left();
};
#endif

