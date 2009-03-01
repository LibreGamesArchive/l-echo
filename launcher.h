// launcher.h

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
#include "escgrid.h"

#ifndef __ECHO_CLASS_LAUNCHER__
#define __ECHO_CLASS_LAUNCHER__
/** @brief A launcher; it's basically a special escgrid that serves as a marker for 
 * the character to realize that it's being launched.
 */
class launcher : public escgrid
{
	public:
		/// Initializes an empty launcher with no info or neighbors
		launcher();
		/// Initializes a launcher with info and no neighbors (it doesn't need them)
		launcher(grid_info_t* my_info);
		/// Re-Initializes a launcher with info and no neighbors (it doesn't need them)
		void init(grid_info_t* my_info);
		/// Deconstructor; does nothing
		virtual ~launcher();
		/** Gets the next grid; it's either the next grid of the current esc,
		 * or null, which tells the character to launch itself (this grid certainly
		 * isn't going to do that for him.
		 */
		virtual grid* get_next(vector3f angle, grid* current);
		/// Draws the launcher
		virtual void draw(vector3f angle);
};
#endif
