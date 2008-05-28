// echo_ns.cpp

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

#include <echo_character.h>
#include <echo_math.h>
#include <grid.h>
#include <echo_ns.h>
#include <hole.h>
#include <isect_grid.h>

namespace echo_ns
{
	grid *hole_grid = new grid();
	vector3f angle;
	echo_char* main_char;
	stage* current_stage;
	int started;
	
	void init(stage* st)
	{
		started = 0;
		current_stage = st;
		main_char = new echo_char(st->get_start());
	}
	void start()
	{
		started = 1;
		main_char->init(current_stage->get_start());
	}
	void draw()
	{
		current_stage->draw(angle);
	}
	void kill_char()
	{
		main_char->kill();
	}
	void toggle_pause()
	{
		main_char->toggle_pause();
	}
	vector3f *step_char()	//CHANGE FOR NORMALS
	{
		return(started ? main_char->step() : NULL);
	}
	void reset()
	{
		main_char->reset();
	}
	int is_paused()
	{
		return(main_char->is_paused());
	}
	int num_goals()
	{
		return(current_stage->get_num_goals());
	}
	int num_goals_reached()
	{
		return(main_char->num_goals_reached());
	}
	int goals_left()
	{
		return(current_stage->get_num_goals() - num_goals_reached());
	}
};
