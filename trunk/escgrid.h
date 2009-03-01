// escgrid.h

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

#include "grid.h"
#include "echo_math.h"

#ifndef __ECHO_CLASS_ESCGRID__
#define __ECHO_CLASS_ESCGRID__
/** @brief EscherGrids act like different grids under different camera angles.
 * This grid is behind most of the optical illusions, by having different "escs",
 * or profiles under certain AngleRanges.\n
 * Most of the functions in this class are just conditionals calling the esc's
 * function or the grid's function.
 */
class escgrid : public grid
{
	protected:
		/// Should the table be deleted at deconstruct?
		int delete_at_deconstruct;
		/// Rudimentary map structure; theory is that it's slightly faster than a STL map
		int num_esc;
		angle_range** ranges;
		grid** escs;
	public:
		/// Initializes an empty EscGrid, with no info and neighbors
		escgrid();
		/// Initializes an EscGrid with the info and neighbors given
		escgrid(grid_info_t* my_info, grid* my_prev, grid* my_next);
		/// Re-initializes the EscGrid with the info and neighbors
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next);
		/** Adds the vector as an angle range and maps the grid given to it
		 * @param vec Angle vector to add as an angle range
		 * @param esc Grid to map to that angle range
		 */
		void add(vector3f* vec, grid* esc);
		/// Maps the angle range to the esc
		void add(angle_range* range, grid* esc);
		/// Attempts to deletes the table (see delete_table)
		virtual ~escgrid();
		/** Gets the info of the grid; override for awesomeness
		 * @param angle Current camera angle
		 */
		virtual grid_info_t* get_info(vector3f angle);
		/** Directs the character to the next grid.  Note that this have to be
		 * bidirectional; the grid has to return the grid in the right direction
		 * the character is traversing in this "linked list".
		 * @param angle Current camera angle
		 * @param current Current grid (actually, a grid next to this grid)
		 * @return The next grid the character should go to
		 */
		virtual grid* get_next(vector3f angles, grid* current);
		/** Draws the grid; default behavior is to draw a quad with attribute "points",
		 * and draw a goal if this is a goal.
		 * @param angle The current camera angle
		 */
		virtual void draw(vector3f angle);
		/// Checks grids for equality.
		virtual int equals(grid* g, vector3f angle);
		/// Can a character land on this grid at that angle?
		virtual int should_land(vector3f angle);
		/// Sets the grid to be a goal; escgrids must set all of its escs as grids too
		virtual void set_as_goal();
		/** Checks for an intersection between a line of Screen Position points and
		 * any of the edges formed by the points; used by holes and launchers.
		 * @param p1 The first point of the line
		 * @param p2 The second point of the line
		 * @param angle Current camera angle
		 */
		virtual int projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle);
		/** Toggles the grid; all escs will be toggled too
		 * @param angle Current camera angle
		 */
		virtual void toggle_goal(vector3f angle);
		/// Asks if this grid is a goal
		virtual int is_goal(vector3f angle);
		/// Initializes the angles and escs to null
		virtual void init_to_null();
		/// Get the profile for this escgrid at that camera angle
		grid* get_esc(vector3f angle);
#ifdef ECHO_NDS
		/// Get the assigned polyID of this escgrid; changes between escs
		virtual unsigned int get_polyID(vector3f angle);
#endif
	protected:
		/** Deletes the list of ranges and escs, and, if instructed, the ranges and escs themselves
		 * @param delete_every Where to delete the ranges and escs.
		 */
		void delete_table(int delete_every);
};
#endif

