// stair.h

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

#include "echo_math.h"
#include "grid.h"

#ifndef __ECHO_CLASS_STAIR__
#define __ECHO_CLASS_STAIR__
/** @brief Displays a stair instead of the typical square. */
class stair : public grid
{
	protected:
		float angle;
	public:
		/// Init info and neighbors to null and angle to 0
		stair();
		/// Init the stair with the info, neighbors, and angle
		stair(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle);
		/// Re-Init the stair with the info, neighbors, and angle
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle);
		/// Inits the angle to 0
		virtual void init_to_null();
		/// Draws the stairs with angle given around the y-axis
		virtual void draw(vector3f angle);
};
#endif

