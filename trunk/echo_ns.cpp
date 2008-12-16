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
#include <set>

#include <echo_platform.h>
#include <echo_error.h>
#include <echo_character.h>
#include <echo_math.h>
#include <echo_ns.h>
#include <echo_gfx.h>

#include <grid.h>
#include <hole.h>
#include <isect_grid.h>

#define NULL_CHAR_OPACITY_MIN   0.25f

namespace echo_ns
{
	grid* hole_grid = new grid();
	//CHKPTR(hole_grid);
	
	float null_char_opacity = NULL_CHAR_OPACITY_MIN;
	int opacity_incr = 1;
	
	vector3f angle;
	echo_char* main_char = NULL;
	stage* current_stage = NULL;
	int started;
	//CAM_MAPS* maps = new CAM_MAPS();
	STATIC_SET* statics = new STATIC_SET();
	
	void deallocate()
	{
		if(current_stage != NULL)
			delete current_stage;
		if(main_char != NULL)
			delete main_char;
		delete hole_grid;
		delete statics;
	}
	void init(stage* st)
	{
		if(current_stage != NULL)
			delete current_stage;
		started = 0;
		current_stage = st;
		if(st != NULL)
		{
			main_char = new echo_char();
			CHKPTR(main_char);
		}
		else
			main_char = NULL;
	}
	void start()
	{
		started = 1;
		main_char->init(current_stage->get_start());
	}
	void draw()
	{
		if(current_stage != NULL)
		{
			STATIC_SET::iterator it = statics->begin(), end = statics->end();
			while(it != end)
			{
				(*it)->refresh(angle);
				it++;
			}
			current_stage->draw(angle);
			if(started)
			{
				//gfx_color3f(0.5f, 0.5f, 0.5f);
				main_char->step();
			}
			else
			{
				grid* g = current_stage->get_start();
				if(g)
				{
					grid_info_t* info = g->get_info(echo_ns::angle);
					if(info)
					{
						gfx_push_matrix();
						gfx_translatef(info->pos.x, info->pos.y, info->pos.z);
#ifndef ECHO_NDS
						gfx_outline_start();
						draw_character(NULL);
						gfx_outline_mid();
#endif
						gfx_color3f(null_char_opacity, null_char_opacity, null_char_opacity);
						draw_character(NULL);
#ifndef ECHO_NDS
						gfx_outline_end();
#endif
						gfx_pop_matrix();
						if(opacity_incr)
						{
							null_char_opacity += 0.05f;
							if(null_char_opacity >= 1)
								opacity_incr = 0;
						}
						else
						{
							null_char_opacity -= 0.05f;
							if(null_char_opacity <= NULL_CHAR_OPACITY_MIN)
								opacity_incr = 1;
						}
					}
				}
			}
		}
	}
	void kill_char()
	{
		main_char->kill();
	}
	void toggle_pause()
	{
		if(current_stage != NULL)
			main_char->toggle_pause();
	}
	void step_char()	//CHANGE FOR NORMALS
	{
		if(started)	main_char->step();
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
	float get_speed()
	{
		return(main_char->get_speed());
	}
	//change speed to running if we can
	void start_run()
	{
		main_char->start_run();
	}
	//change speed to walking if we can
	void start_step()
	{
		main_char->start_step();
	}
	void toggle_run()
	{
		main_char->toggle_run();
	}
	
	void add_static_grid(static_grid* sg)
	{
		statics->insert(sg);
	}
	
	void remove_static_grid(static_grid* sg)
	{
		STATIC_SET::iterator it = statics->find(sg);
		if(it != statics->end())
			statics->erase(it);
	}
};
