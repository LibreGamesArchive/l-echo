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
	escgrid::init(my_escangle, my_normal_info, my_esc_info, NULL, my_esc_prev, NULL, my_esc_next);
}

void hole::init(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs)
{
	escgrid::init(my_info, NULL, NULL, my_escranges, my_escs, my_num_escs);
}

void hole::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	hole::init(my_info, my_prev, my_next, NULL, NULL, 0);
}

hole::~hole()
{
}
void hole::draw(vector3f angle)
{
	escgrid::draw(angle);
	draw_hole(get_info(angle)->pos);
}
grid* hole::get_next(vector3f angle, grid* current)
{
	grid* esc = get_esc(angle);
	if(esc != NULL)
		return(esc->get_next(angle, current));
	return(NULL);
}

