// isect_grid.cpp

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

#include <echo_ns.h>
#include <echo_math.h>
#include <grid.h>
#include <static_grid.h>
#include <isect_grid.h>
#include <echo_stage.h>

isect_grid::isect_grid() : static_grid()
{
}
isect_grid::isect_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f camera, GRID_PTR_SET* my_level) : static_grid()
{
	init(my_info, my_prev, my_next, camera, my_level);
}
void isect_grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f camera, GRID_PTR_SET* my_level)
{
	level_y = my_info->pos.y;
	static_grid::init(my_info, my_prev, my_next, camera);
	level = my_level;
}

grid* isect_grid::get_next(vector3f angle, grid* current)
{
	refresh(angle);
	GRID_PTR_SET::iterator it = level->begin(), end = level->end();
	vector3f prev_pos = get_real_prev()->get_info(angle)->pos;
	vector3f vec = ginfo->pos - prev_pos;
	float delta_y = level_y - prev_pos.y;
	//std::cout << "delta_y: " << delta_y << std::endl;
	if((delta_y > 0 && vec.y < 0) || (delta_y < 0 && vec.y > 0))
		return(grid::get_next(angle, current));
	if(vec.y != 0)
		vec = vec * (delta_y / vec.y);
	vector3f end_pt = prev_pos + vec;
	/*
	std::cout << "end_pt: " << std::endl;
	end_pt.dump();
	std::cout << std::endl;
	// */
	while(it != end)
	{
		grid* g = *it;
		if(g->is_pt_on(angle,end_pt))
		{
			return(g);
		}
		it++;
	}
	return(grid::get_next(angle, current));
}

void isect_grid::init_to_null()
{
	static_grid::init_to_null();
	level = NULL;
}

void isect_grid::force_refresh(vector3f camera)
{
	static_grid::force_refresh(camera);
}
