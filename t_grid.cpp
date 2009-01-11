// t_grid.cpp

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

#include <t_grid.h>
#include <iostream>
#include <echo_debug.h>
#include <echo_gfx.h>
#include <echo_math.h>
#include <grid.h>

/// Init an empty t_grid with no neighbors and info
t_grid::t_grid() : grid()
{
	init(NULL, NULL, NULL, NULL);
}
/// Init a t_grid with the info and neighbors
t_grid::t_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, grid* my_next2) : grid()
{
	init(my_info, my_prev, my_next, my_next2);
}
/// Re-initializes the t_grid with the info and neighbors
void t_grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, grid* my_next2)
{
	grid::init(my_info, my_prev, my_next, 3);
	neighbors[2] = my_next2;
}
/// Deconstructor; doesn't do anything
t_grid::~t_grid()
{
}
/// Gets next grid. It goes in a cycle: neighbors[0] -> neighbors[1] -> neighbors[2]
grid* t_grid::get_next(vector3f angle, grid* current)
{
	if(current && neighbors[0] && current->equals(neighbors[0], angle))
		return(neighbors[1]);
	if(current && neighbors[2] && current->equals(neighbors[2], angle))
		return(neighbors[0]);
	return(neighbors[2]);
}
/// Set neighbors[2].  Used by the loader
void t_grid::set_real_next2(grid* g)
{
	neighbors[2] = g;
}
/// Prettyprints the t_grid
void t_grid::dump()
{
	ECHO_PRINT("t_grid: [");
	if(ginfo)    dump_grid_info(*ginfo);
	else        ECHO_PRINT("NULL grid_info_t?");
	ECHO_PRINT("]");
	//std::cout << "," << neighbors[0] << "," << neighbors[1];
	//std::cout << "," << neighbors[2] << "]";
}

