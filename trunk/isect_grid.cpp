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
#include <typeinfo>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_ns.h>
#include <echo_math.h>
#include <grid.h>
#include <static_grid.h>
#include <isect_grid.h>
#include <echo_stage.h>

isect_grid::isect_grid() : static_grid()
{
}
isect_grid::isect_grid(grid_info_t* my_info, grid* my_prev, grid* my_next
			, vector3f camera, vector3f* my_path_orig, static_grid* my_root, GRID_PTR_SET* my_level) : static_grid()
{
	init(my_info, my_prev, my_next, camera, my_path_orig, my_root, my_level);
}
void isect_grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next
			, vector3f camera, vector3f* my_path_orig, static_grid* my_root, GRID_PTR_SET* my_level)
{
	level_y = my_info->pos.y;
	static_grid::init(my_info, my_prev, my_next, camera, my_path_orig, my_root);
	level = my_level;
}

isect_grid::~isect_grid()
{
}

static vector3f* end_pt(vector3f* prev_pos, vector3f* vec, float level_y)
{
	if(vec->y != 0)
	{
		float delta_y = level_y - prev_pos->y;
		if((delta_y > 0 && vec->y < 0) || (delta_y < 0 && vec->y > 0))
			return(NULL);
		vec = (*vec) * (delta_y / vec->y);
	}
#ifdef STRICT_MEM
	vector3f* ret = *prev_pos + vec;
	CHKPTR(ret);
	return(ret);
#else
	return(new vector3f(prev_pos + vec));
#endif
}

static grid* check_level(GRID_PTR_SET* level, vector3f* my_end_pt, vector3f angle)
{
	GRID_PTR_SET::iterator it = level->begin(), end = level->end();
	grid* g = NULL;
	while(it != end)
	{
		g = *it;
		if(g->is_pt_on(angle, *my_end_pt))
		{
			return(g);
		}
		it++;
	}
	return(NULL);
}

static grid* check_levels_above(LEVEL_MAP* levels_above, vector3f* pos, vector3f* vec, vector3f angle)
{
	if(levels_above->size() < 1)
		return(NULL);
	LEVEL_MAP::iterator it = levels_above->begin(), end = levels_above->end();
	vector3f* pt = NULL;
	grid* g = NULL;
	while(it != end)
	{
		pt = end_pt(pos, vec, it->first);
		if(pt)
			g = check_level(it->second, pt, angle);
		if(g)
			return(g);
		it++;
	}
	return(NULL);
}

static grid* check_levels_below(LEVEL_MAP* levels_below, vector3f* pos, vector3f* vec, vector3f angle)
{
	if(levels_below->size() < 1)
		return(NULL);
	LEVEL_MAP::iterator it = levels_below->end(), end = levels_below->begin();
	vector3f* pt = NULL;
	grid* g = NULL;
	do
	{
		it--;
		pt = end_pt(pos, vec, it->first);
		if(pt)
			g = check_level(it->second, pt, angle);
		if(g)
			return(g);
	}
	while(it != end);
	return(NULL);
}

grid* isect_grid::get_next(vector3f angle, grid* current)
{
	refresh(angle);
	if(cam_grid)
		return(cam_grid);
	//*
	vector3f* my_end_pt = &ginfo->pos;
	grid* prev = get_real_prev();
	if(prev)
	{
		grid_info_t* prev_info = prev->get_info(angle);
		if(prev_info)
		{
			vector3f* cam_vec = ginfo->pos - &(prev_info->pos);
			my_end_pt = end_pt(&(prev_info->pos), cam_vec, level_y);
			delete cam_vec;
			if(!my_end_pt)
				return(grid::get_next(angle, current));
		}
	}
	grid* ret = check_level(level, my_end_pt, angle);
	delete my_end_pt;
#ifdef PATH_GRID
	set_as_path_grid(angle);
#endif
	return(ret ? ret : grid::get_next(angle, current));
	// */
}

grid* isect_grid::where_is_cam_grid()
{
	if(cam_grid)
		return(this);
	grid* next = get_real_next();
	if(next && (typeid(*next) == typeid(static_grid) 
			|| typeid(*next) == typeid(isect_grid)))
	{
		return(((static_grid*)next)->where_is_cam_grid());
	}
	return(NULL);
}

grid* isect_grid::get_cam_grid()
{
	return(cam_grid);
}

void isect_grid::init_to_null()
{
	static_grid::init_to_null();
	level = NULL;
	cam_grid = NULL;
}

grid_info_t* isect_grid::get_info(vector3f angle)
{
	return(cam_grid ? cam_grid->get_info(angle) : static_grid::get_info(angle));
}

void isect_grid::force_refresh(vector3f camera)
{
	cam_grid = NULL;
	
	static_grid::force_refresh(camera);
	
	if(ABS(camera.x) >= 35 && ABS(camera.x) <= 50)
	{
		vector3f* cam_real = camera.angle_to_real();
		vector3f* cam_vec = ginfo->pos - cam_real;
		vector3f* neg_cam_vec = cam_vec->negate();
		if(cam_real->y > 0) //viewing downwards
		{
			cam_grid = check_levels_above(echo_ns::current_stage->get_levels_higher_than(level_y)
							, &(ginfo->pos), neg_cam_vec, camera);
			if(!cam_grid)
				cam_grid = check_levels_below(echo_ns::current_stage->get_levels_lower_than(level_y)
							, &(ginfo->pos), neg_cam_vec, camera);
		}
		else //viewing up
		{
			cam_grid = check_levels_below(echo_ns::current_stage->get_levels_lower_than(level_y)
							, &(ginfo->pos), neg_cam_vec, camera);
			if(!cam_grid)
				cam_grid = check_levels_above(echo_ns::current_stage->get_levels_higher_than(level_y)
							, &(ginfo->pos), neg_cam_vec, camera);
		}
		delete cam_vec;
		delete neg_cam_vec;
	}
}

