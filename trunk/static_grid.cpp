// static_grid.cpp

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

#include <set>
#include <map>
#include <iostream>
#include <typeinfo>

#include <echo_ns.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_gfx.h>
#include <echo_math.h>
#include <grid.h>
#include <static_grid.h>
#include <isect_grid.h>

static_grid::static_grid() : grid()
{
	echo_ns::add_static_grid(this);
}
static_grid::static_grid(grid_info_t* my_info, grid* my_prev
			, grid* my_next, vector3f camera
			, vector3f* my_path_orig) : grid()
{
	init(my_info, my_prev, my_next, camera, my_path_orig, this);
	echo_ns::add_static_grid(this);
}
static_grid::static_grid(grid_info_t* my_info, grid* my_prev
			, grid* my_next, vector3f camera
			, vector3f* my_path_orig, static_grid* my_root) : grid()
{
	init(my_info, my_prev, my_next, camera, my_path_orig, my_root == NULL ? this : my_root);
	echo_ns::add_static_grid(this);
}
void static_grid::init(grid_info_t* my_info, grid* my_prev
			, grid* my_next, vector3f camera
			, vector3f* my_path_orig, static_grid* my_root)
{
#ifdef STRICT_MEM
	grid_info_t* init_info = new(grid_info_t);
	CHKPTR(init_info);
	grid::init(init_info, my_prev, my_next);
#else
	grid::init(new(grid_info_t), my_prev, my_next);
#endif
	real_vec = my_info->pos.neg_rotate_yx(camera);
	root = my_root;
	force_refresh(camera);
}
void static_grid::refresh(vector3f camera)
{
	if(camera != prev_angle)
	{
		force_refresh(camera);
	}
}

static_grid::~static_grid()
{
	echo_ns::remove_static_grid(this);
}

void static_grid::init_to_null()
{
	grid::init_to_null();
	real_vec = NULL;
}

void static_grid::force_refresh(vector3f camera)
{
	ginfo->pos = *(real_vec->rotate_xy(camera));
	grid* cam_grid = where_is_cam_grid();
	if(cam_grid)
	{
		grid_info_t* real_info = ((static_grid*)cam_grid)->get_ginfo();
		grid_info_t* cam_info = ((isect_grid*)cam_grid)->get_cam_grid()->get_info(camera);
		vector3f cam_vec = real_info->pos - cam_info->pos;
		ginfo->pos = ginfo->pos + cam_vec;
	}
	prev_angle.set(camera);
}

grid_info_t* static_grid::get_info(vector3f angle)
{
	refresh(angle);
	return(ginfo);
}

void static_grid::draw(vector3f angle)
{
	refresh(angle);
	grid::draw(angle);
}

grid* static_grid::get_next(vector3f angle, grid* current)
{
	refresh(angle);
	return(get_real_next());
}

grid* static_grid::where_is_cam_grid()
{
	grid* next = get_real_next();
	if(next && (typeid(*next) == typeid(static_grid) 
			|| typeid(*next) == typeid(isect_grid)))
	{
		return(((static_grid*)next)->where_is_cam_grid());
	}
	return(NULL);
}

grid_info_t* static_grid::get_ginfo()
{
	return(ginfo);
}

