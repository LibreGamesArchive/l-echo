// escgrid.cpp

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

#include <echo_error.h>
#include <cstdlib>
#include <iostream>
#include <grid.h>
#include <escgrid.h>
#include <echo_math.h>
#include <echo_gfx.h>

/// Initializes an empty EscGrid, with no info and neighbors
escgrid::escgrid() : grid()
{
	init_to_null();
	delete_at_deconstruct = 1;
	init(NULL, NULL, NULL);
}
/// Initializes an EscGrid with the info and neighbors given
escgrid::escgrid(grid_info_t* my_info, grid* my_prev, grid* my_next) : grid()
{
	init_to_null();
	delete_at_deconstruct = 1;
	init(my_info, my_prev, my_next);
}
/** Deletes the list of ranges and escs, and, if instructed, the ranges and escs themselves
 * @param delete_every Where to delete the ranges and escs.
 */
void escgrid::delete_table(int delete_every)
{
	if(delete_at_deconstruct)
	{
		if(escs)
		{
			if(delete_every == true)
			{
				int each = 0;
				while(each < num_esc)
				{
					grid* del = escs[each];
					delete del;
					each++;
				}
			}
			delete[] escs;
		}
		if(ranges)
		{
			if(delete_every == true)
			{
				int each = 0;
				while(each < num_esc)
				{
					angle_range* del = ranges[each];
					delete del;
					each++;
				}
			}
			delete[] ranges;
		}
	}
}
/// Re-initializes the EscGrid with the info and neighbors
void escgrid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	grid::init(my_info, my_prev, my_next);
	delete_table(1);
	ranges = NULL;
	escs = NULL;
	delete_at_deconstruct = 1;
	num_esc = 0;
}
/// Deletes the table (see delete_table)
escgrid::~escgrid()
{
	delete_table(1);
}
/// Initializes the angles and escs to null
void escgrid::init_to_null()
{
	grid::init_to_null();
	ranges = NULL;
	escs = NULL;
}
/** Adds the vector as an angle range and maps the grid given to it
 * @param vec Angle vector to add as an angle range
 * @param esc Grid to map to that angle range
 */
void escgrid::add(vector3f* vec, grid* esc)
{
	add(VECPTR_TO_RANGE(vec), esc);
}
/// Maps the angle range to the esc
void escgrid::add(angle_range* range, grid* esc)
{
	angle_range** new_ranges = new angle_range*[num_esc + 1];
	CHKPTR(new_ranges);
	grid** new_escs = new grid*[num_esc + 1];
	CHKPTR(new_escs);
	if(escs)
	{
		int each = 0;
		while(each < num_esc)
		{
			new_ranges[each] = ranges[each];
			new_escs[each] = escs[each];
			each++;
		}
		delete_table(0);
	}
	delete_at_deconstruct = 1;
	escs = new_escs;
	ranges = new_ranges;
	/// If this escgrid itself is a goal, make the esc a goal too.
	if(am_goal)
		esc->set_as_goal();
	ranges[num_esc] = range;
	escs[num_esc] = esc;
	num_esc++;
}
/// Get the profile for this escgrid at that camera angle
grid* escgrid::get_esc(vector3f angle)
{
	int each = 0;
	while(each < num_esc)
	{
		if(escs[each] && ranges[each]->is_vec_in(angle))
		{
			return(escs[each]);
		}
		each++;
	}
	return(NULL);
}
/// Checks grids for equality.
int escgrid::equals(grid* g, vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->equals(g, angle) : grid::equals(g, angle));
}
/** Draws the grid; default behavior is to draw a quad with attribute "points",
 * and draw a goal if this is a goal.
 * @param angle The current camera angle
 */
void escgrid::draw(vector3f angle)
{
	grid* esc = get_esc(angle);
	if(esc) esc->draw(angle);
	else    grid::draw(angle);
}
/** Gets the info of the grid; override for awesomeness
 * @param angle Current camera angle
 */
grid_info_t* escgrid::get_info(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_info(angle) : grid::get_info(angle));
}
/** Directs the character to the next grid.  Note that this have to be
 * bidirectional; the grid has to return the grid in the right direction
 * the character is traversing in this "linked list".
 * @param angle Current camera angle
 * @param current Current grid (actually, a grid next to this grid)
 * @return The next grid the character should go to
 */
grid* escgrid::get_next(vector3f angle, grid* current)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_next(angle, current) : grid::get_next(angle, current));
}

#ifdef ECHO_NDS
	/// Get the assigned polyID of this escgrid; changes between escs
	unsigned int escgrid::get_polyID(vector3f angle)
	{
		grid* esc = get_esc(angle);
		return(esc ? esc->get_polyID(angle) : grid::get_polyID(angle));
	}
#endif

/// Sets the grid to be a goal; escgrids must set all of its escs as grids too
void escgrid::set_as_goal()
{
	int each = 0;
	while(each < num_esc)
	{
		escs[each]->set_as_goal();
		each++;
	}
	grid::set_as_goal();
}
/** Toggles the grid; all escs will be toggled too
 * @param angle Current camera angle
 */
void escgrid::toggle_goal(vector3f angle)
{
	int each = 0;
	while(each < num_esc)
	{
		escs[each]->toggle_goal(angle);
		each++;
	}
	grid::toggle_goal(angle);
}
/// Asks if this grid is a goal
int escgrid::is_goal(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->is_goal(angle) : grid::is_goal(angle));
}
/// Can a character land on this grid at that angle?
int escgrid::should_land(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->should_land(angle) : grid::should_land(angle));
}
/** Checks for an intersection between a line of Screen Position points and
 * any of the edges formed by the points; used by holes and launchers.
 * @param p1 The first point of the line
 * @param p2 The second point of the line
 * @param angle Current camera angle
 */
int escgrid::projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->projected_line_intersect(p1, p2, angle) : grid::projected_line_intersect(p1, p2, angle));
}
