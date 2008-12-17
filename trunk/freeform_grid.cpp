// freeform_grid.cpp

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

#include <echo_error.h>
#include <freeform_grid.h>
#include <echo_math.h>
#include <grid.h>

freeform_grid::freeform_grid() : grid()
{
    init(NULL, NULL, NULL, new vector3f(0.5f, 0.5f, 0), new vector3f(0, 0, 0.5f));
}

freeform_grid::freeform_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f* my_dir, vector3f* my_width) : grid()
{
    dir = my_dir;
    width = my_width;
    init(my_info, my_prev, my_next, my_dir, my_width);
}

void freeform_grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f* my_dir, vector3f* my_width)
{
    dir = my_dir;
    width = my_width;
    grid::init(my_info, my_prev, my_next);
}

void freeform_grid::init_to_null()
{
	grid::init_to_null();
	dir = NULL;
	width = NULL;
}

vector3f** freeform_grid::generate_points(grid_info_t* my_info)
{
	vector3f** ret = new vector3f*[4];
	CHKPTR(ret);
	
	vector3f* pos = my_info->pos;
	ret[0] = new vector3f(pos->x + dir->x + width->x
				, pos->y + dir->y + width->y
				, pos->z + dir->z + width->z);
	CHKPTR(ret[0]);
	ret[1] = new vector3f(pos->x - dir->x + width->x
				, pos->y - dir->y + width->y
				, pos->z - dir->z + width->z);
	CHKPTR(ret[1]);
	ret[2] = new vector3f(pos->x - dir->x - width->x
				, pos->y - dir->y - width->y
				, pos->z - dir->z - width->z);
	CHKPTR(ret[2]);
	ret[3] = new vector3f(pos->x + dir->x - width->x
				, pos->y + dir->y - width->y
				, pos->z + dir->z - width->z);
	CHKPTR(ret[3]);
	
	return(ret);
}


