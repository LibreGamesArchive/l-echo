// hole.h

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

#include <echo_math.h>
#include <grid.h>
#include <escgrid.h>

#ifndef __ECHO_CLASS_HOLE__
#define __ECHO_CLASS_HOLE__
class hole : public escgrid
{
	public:
		/// Initializes an empty hole with no info or neighbors
		hole();
		/// Initializes a hole with info and no neighbors (it doesn't need them)
		hole(grid_info_t* my_info);
		/// Re-Initializes a hole with info and no neighbors (it doesn't need them)
		void init(grid_info_t* my_info);
		/// Descontructor; does nothing
		virtual ~hole();
		/** Gets the next grid; it's either the next grid of the current esc,
		 * or null, which tells the character to fall into the hole itself (this grid certainly
		 * isn't going to do that for him)
		 */
		virtual grid* get_next(vector3f angle, grid* current);
		/// Draws the hole
		virtual void draw(vector3f angle);
};
#endif
