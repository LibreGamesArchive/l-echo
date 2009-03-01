// launcher.cpp

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

#include <cmath>
#include <iostream>

#include "echo_debug.h"
#include "echo_error.h"
#include "echo_ns.h"
#include "echo_gfx.h"
#include "launcher.h"
#include "echo_math.h"
#include "grid.h"

/// Initializes an empty launcher with no info or neighbors
launcher::launcher() : escgrid()
{

}
/// Initializes a launcher with info and no neighbors (it doesn't need them)
launcher::launcher(grid_info_t* my_info) : escgrid()
{
	init(my_info);
}
/// Re-Initializes a launcher with info and no neighbors (it doesn't need them)
void launcher::init(grid_info_t* my_info)
{
	escgrid::init(my_info, NULL, NULL);
}
/// Deconstructor; does nothing
launcher::~launcher()
{
}
/// Draws the launcher
void launcher::draw(vector3f angle)
{
	escgrid::draw(angle);
	draw_launcher(get_info(angle)->pos);
}
/** Gets the next grid; it's either the next grid of the current esc,
 * or null, which tells the character to launch itself (this grid certainly
 * isn't going to do that for him.
 */
grid* launcher::get_next(vector3f angle, grid* current)
{
	grid* esc = get_esc(angle);
	if(esc != NULL)
		return(esc->get_next(angle, current));
	return(NULL);
}

