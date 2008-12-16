// stair.cpp

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



#include <echo_debug.h>
#include <echo_error.h>
#include <echo_math.h>
#include <echo_gfx.h>

#include <grid.h>
#include <stair.h>

stair::stair() : grid()
{
	init(NULL, NULL, NULL, 0);
}

stair::stair(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle) : grid(0)
{
	init(my_info, my_prev, my_next, my_angle);
}

void stair::init(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle)
{
	angle = my_angle;
	//ECHO_PRINT("stair angle 1: %f\n", angle);
	grid::init(my_info, my_prev, my_next);
}

void stair::init_to_null()
{
	grid::init_to_null();
	angle = 0;
}

void stair::draw(vector3f angle)
{
	gfx_push_matrix();
		gfx_color3f(1, 1, 1);
		gfx_translatef(ginfo->pos->x, ginfo->pos->y, ginfo->pos->z);
		gfx_rotatef(this->angle, 0, 1, 0);
		draw_stairs();
	gfx_pop_matrix();
	
	draw_goal(angle);
}
