// freeform_grid.h

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

#ifndef __ECHO_CLASS_FREEFORM_GRID__
#define __ECHO_CLASS_FREEFORM_GRID__
/// A parallelogram-shaped grid
class freeform_grid : public grid
{
	protected:
		/// A side vector; it forms half of a side
    		vector3f* dir;
		/// The other side vector; it forms half of a side
		vector3f* width;
	public:
		/// Initializes an empty freeform grid sloping right and up
		freeform_grid();
		/// Initialize the freeform grid with the info, neighbors, and side vectors
		freeform_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f* my_dir, vector3f* my_width);
		/// Re-Initialize the freeform grid with the info, neighbors, and side vectors
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f* my_dir, vector3f* my_width);
		/// Makes the side vectors null
		virtual void init_to_null();
		/// Generate points that makes the grid shaped like a parallelogram
		virtual vector3f** generate_points(grid_info_t* my_info);
};
#endif
