// hole.cpp

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

#include <iostream>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_ns.h>
#include <echo_gfx.h>
#include <hole.h>
#include <echo_math.h>
#include <grid.h>
//#include <isect_grid.h>

hole::hole() : escgrid()
{

}

hole::hole(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next) : escgrid()
{
	init(my_escangle, my_normal_info, my_esc_info, my_normal_prev, my_esc_prev, my_normal_next, my_esc_next);
}

hole::hole(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs) : escgrid()
{
	init(my_info, my_prev, my_next, my_escranges, my_escs, my_num_escs);
}

hole::hole(grid_info_t* my_info, grid* my_prev, grid* my_next) : escgrid()
{
    init(my_info, my_prev, my_next);
}

void hole::init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next)
{
	escgrid::init(my_escangle, my_normal_info, my_esc_info, echo_ns::hole_grid, my_esc_prev, echo_ns::hole_grid, my_esc_next);
	real_prev = my_normal_prev;
	real_next = my_normal_next;
}

void hole::init(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs)
{
	escgrid::init(my_info, echo_ns::hole_grid, echo_ns::hole_grid, my_escranges, my_escs, my_num_escs);
	real_prev = my_prev;
	real_next = my_next;
}

void hole::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
    hole::init(my_info, my_prev, my_next, NULL, NULL, 0);
}

hole::~hole()
{
}

void hole::init_to_null()
{
	escgrid::init_to_null();
	real_prev = NULL;
	real_next = NULL;
}

void hole::draw(vector3f angle)
{
	escgrid::draw(angle);
	draw_hole(get_info(angle)->pos);
}

void hole::set_real_next(grid* g)
{
	real_next = g;
}

void hole::set_real_prev(grid* g)
{
	real_prev = g;
}

grid* hole::get_real_next()
{
	return(real_next);
}

grid* hole::get_real_prev()
{
	return(real_prev);
}
grid* hole::get_next(vector3f angle, grid* current)
{
	/*
	grid* esc = get_esc(angle);
	if(esc)
		return(esc->get_next(angle, current));
	vector3f pos = get_info(angle)->pos;
	LEVEL_MAP* levels = echo_ns::current_stage->get_levels_lower_than(pos.y);
	if(levels->size() > 0)
	{
		//CAM_GRID_MAP* map = new CAM_GRID_MAP();
		//CHKPTR(map);
		LEVEL_MAP::iterator it = levels->begin(), end = levels->end();
		isect_grid* begin = NULL;
		isect_grid* real_begin = NULL;
		grid* temp = echo_ns::hole_grid;
		while(it != end)
		{
			grid_info_t* info = new(grid_info_t);
			CHKPTR(info);
			info->pos.set(pos.x, it->first, pos.z);
			//init_level(map, it->first);
			begin = new isect_grid(info, NULL, temp, angle, &pos, real_begin, it->second);
			CHKPTR(begin);
			temp->set_real_prev(begin);
			//echo_ns::current_stage->add("add", begin);
			temp = begin;
			it++;
		}
		//std::cout << "end: " << end->first <<std::endl;
		if(begin)
		{
			begin->set_real_prev(this);
			return(begin);
		}
	}
	return(echo_ns::hole_grid);
	// */
	return(NULL);
}

