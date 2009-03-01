// grid.h

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

#include <set>
#include "filter.h"
#include "trigger.h"
#include "echo_math.h"
#include "echo_platform.h"

/// The size of a grid
#define GRID_SIZE	1
/// Half the size of a grid
#define HALF_GRID	0.5f

#ifndef __ECHO_GRID_INFO_T__
#define __ECHO_GRID_INFO_T__
/** @brief Contains information on the grid, which is just its position
 * as of this release.
 */
typedef struct
{
	/// The position of the grid
	vector3f* pos;
}
grid_info_t;

/// Prettyprints the info's data
void dump_grid_info(grid_info_t ginfo);
#endif

#define LINE_PT(ln, pt)	((ln) * 4 + (pt))
#define LINE(ptr, ln)	(ptr)[LINE_PT(ln, 0)], (ptr)[LINE_PT(ln, 1)]

#ifndef __ECHO_CLASS_GRID__
#define __ECHO_CLASS_GRID__

class grid;

typedef std::set<grid*> GRID_PTR_SET;
/** @brief The "nodes" in Grid Mode; the character just traverses through the grids like a
 * linked list, and grids are the nodes.
 * 
 * The basic grid can only have a previous node and a next node.  See TGrid for
 * a grid with 3 neighbors (too lazy to make a XGrid for 4 neighbors).
 */
class grid
{
	protected:
		/// Contains basic info about the grid, which is just its positions
		grid_info_t* ginfo;
		/// The grid's neighbors
		grid** neighbors;
		/// The number of neighbors it has
		int n_neighbors;
		/// Is there a goal on this grid?
		int am_goal;
		/// Should this grid be drawn?
		int draw_me;
		/// Can the character land on this grid?
		int landable;
		/// Is this grid already initialized?
		int already_init;
		/// This grid's triggers; if this grid is toggled, then it will attempt to toggle the triggers in this list
		TRIGGER_SET* triggers;
		/** Cached points of this grid; THERE MUST BE 4, OR THE SUBCLASS MUST ALSO
		 * MODIFY generate_points _AND_ draw _AND_ projected_line_intersect!
		 */
		vector3f** points;
#ifdef ECHO_NDS
		/// The polyID; see echo_gfx for explanation
		unsigned int polyID;
#endif
	public:
		/// Initialize a grid with no info and no previous or next
		grid();
		/** Initialize a grid with the info, previous and next given
		 * @param my_info The new grid's info
		 * @param my_prev The new grid's previous
		 * @param my_next The new grid's next
		 */
		grid(grid_info_t* my_info, grid* my_prev, grid* my_next);
		/** Initialize a grid with the info, previous and next given
		 * @param my_info The new grid's info
		 * @param my_prev The new grid's previous
		 * @param my_next The new grid's next
		 * @param num_neighbors The new grid's number of neighbors
		 */
		grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor);
		/** Initialize a grid with the info given and no previous or next
		 * @param my_info The new grid's info
		 */
		grid(grid_info_t* my_info);
		/// Initialize everything to be dynamically allocated to null
		virtual void init_to_null();
		/** Initialize this grid with the info, previous and next given
		 * @param my_info The grid's new info
		 * @param my_prev The grid's new previous
		 * @param my_next The grid's new next
		 */
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next);
		/** Initialize this grid with the info, previous and next given
		 * @param my_info The grid's new info
		 * @param my_prev The grid's new previous
		 * @param my_next The grid's new next
		 * @param num_neighbors The grid's new nuber of neighbors
		 */
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor);
		/// Delete everything
		virtual ~grid();
		/// Clear the grid's points
		void delete_points();
		/// Clear the triggers
		void delete_triggers();
		/// Clear the list of neighbors, not the neighbors themselves
		void delete_neighbors();
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
		virtual grid* get_next(vector3f angle, grid* current);
		/// Prettyprints the grid
		virtual void dump();
		/** Checks grids for equality, which just requires the positions to be
		 * same.  The rationale is that one of the grids is a EscGrid, and thus
		 * just comparing references won't necessarily detect equality.
		 * @param g The other grid to compare to.
		 * @param angle The current camera angle
		 */
		virtual int equals(grid* g, vector3f angle);
		/** Sets the next grid; used mainly by the loader
		 * @param g The new next grid
		 */
		virtual void set_real_next(grid* g);
		/** Sets the previous grid; used mainly by the loader
		 * @param g The new previous grid
		 */
		virtual void set_real_prev(grid* g);
		/** Adds the trigger
		 * @param trig Trigger to be added
		 */
		virtual void add_trigger(trigger* trig);
		/** Toggles the grid; if this grid is a goal before calling this, then
		 * triggers will be toggled.
		 * @param angle Current camera angle
		 */
		virtual void toggle_goal(vector3f angle);
		/// Asks if this grid is a goal
		virtual int is_goal(vector3f angle);
		/// Sets the grid to be a goal; used mainly by the loader
		virtual void set_as_goal();
		/** Generates the set of points that outline the grid\n
		 * Used for drawing, and deciding if a character lands on this grid\n
		 * Default behavior is to return a square with width GRID_SIZE that is centered at the grid's position
		 * @param my_info The grid's info, so that this method doesn't have to get it
		 * @return The points
		 */
		virtual vector3f** generate_points(grid_info_t* my_info);
		/// Should this grid be drawn?
		virtual int should_draw();
		/// Sets if this grid should be drawn
		virtual void set_draw(int draw);
		/** Draws the grid; default behavior is to draw a quad with attribute "points",
		 * and draw a goal if this is a goal.
		 * @param angle The current camera angle
		 */
		virtual void draw(vector3f angle);
		/** Sets the grid's land flag; can the character land on this grid?
		 * @param land New land flag
		 */
		virtual void set_land(int land);
		/// Can a character land on this grid at that angle?
		virtual int should_land(vector3f angle);
		/** Is the given point on this grid?  Default behavior is to make sure
		 * that the y is similar and that the x and z is within a HALF_GRID
		 * distance.
		 * @param angle Current camera angle
		 * @param pt Point to check
		 */
		virtual int is_pt_on(vector3f angle, vector3f* pt);
		/** Checks for an intersection between a line of Screen Position points and
		 * any of the edges formed by the points; used by holes and launchers.
		 * @param p1 The first point of the line
		 * @param p2 The second point of the line
		 * @param angle Current camera angle
		 */
		virtual int projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle);
		/** If this grid is a goal, then it draws a goal above the position of this grid.
		 * @param angle Current camera angle
		 */
		void draw_goal(vector3f angle);
#ifdef ECHO_NDS
		/** Gets the grid's polyID (see echo_gfx for more info on polyID)
		 * @param angle Current camera angle
		 */
		virtual unsigned int get_polyID(vector3f angle);
		/** Sets this grid's polyID
		 * @param my_polyID The grid's new polyID
		 */
		void set_polyID(unsigned int my_polyID);
#endif
};
#endif

