// grid.cpp

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

#include <cstdlib>
#include <iostream>

#include <echo_debug.h>
#include <echo_platform.h>
#include <echo_error.h>
#include <echo_gfx.h>
#include <trigger.h>
#include <grid.h>
#include <echo_math.h>

/// Prettyprints the info's data
void dump_grid_info(grid_info_t ginfo)
{
	ECHO_PRINT("grid_info_t: [");
	ginfo.pos->dump();
	ECHO_PRINT("]");
}
/// Initialize a grid with no info and no previous or next
grid::grid()
{
	already_init = 0;
	init_to_null();
	init(NULL, NULL, NULL);
}
/** Initialize a grid with the info given and no previous or next
 * @param my_info The new grid's info
 */
grid::grid(grid_info_t* my_info)
{
	already_init = 0;
	init_to_null();
	init(my_info, NULL, NULL);
}
/** Initialize a grid with the info, previous and next given
 * @param my_info The new grid's info
 * @param my_prev The new grid's previous
 * @param my_next The new grid's next
 */
grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	already_init = 0;
	init_to_null();
	init(my_info, my_prev, my_next);
}
/** Initialize a grid with the info, previous and next given
 * @param my_info The new grid's info
 * @param my_prev The new grid's previous
 * @param my_next The new grid's next
 * @param num_neighbors The new grid's number of neighbors
 */
grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor)
{
	already_init = 0;
	init_to_null();
	init(my_info, my_prev, my_next, num_neighbor);
}
/** Initialize this grid with the info, previous and next given
 * @param my_info The grid's new info
 * @param my_prev The grid's new previous
 * @param my_next The grid's new next
 */
void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	init(my_info, my_prev, my_next, 2);
}
/// Initialize everything to be dynamically allocated to null
void grid::init_to_null()
{
	if(already_init == 1)
	{
		delete_points();
		delete_triggers();
		delete_neighbors();
	}
	ginfo = NULL;
	neighbors = NULL;
	triggers = NULL;
	points = NULL;
#ifdef ECHO_NDS
	polyID = 0;
#endif
	already_init = 1;
	landable = 1;
}
/** Initialize this grid with the info, previous and next given
 * @param my_info The grid's new info
 * @param my_prev The grid's new previous
 * @param my_next The grid's new next
 * @param num_neighbors The grid's new nuber of neighbors
 */
void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, int my_num_neighbors)
{
	am_goal = 0;
	draw_me = 1;
	
	delete_triggers();
	triggers = new TRIGGER_SET();
	CHKPTR(triggers);
	
	ginfo = my_info;
	
	n_neighbors = my_num_neighbors;
	delete_neighbors();
	neighbors = new grid*[my_num_neighbors < 2 ? 2 : my_num_neighbors];
	CHKPTR(neighbors);
	neighbors[0] = my_prev;
	neighbors[1] = my_next;
	
	delete_points();
	points = ginfo != NULL ? generate_points(ginfo) : NULL;
}
/// Clear the grid's points
void grid::delete_points()
{
	if(points != NULL)
	{
		delete points[0];
		delete points[1];
		delete points[2];
		delete points[3];
		delete[] points;
		points = NULL;
	}
}
/// Clear the triggers
void grid::delete_triggers()
{
	if(triggers != NULL)
	{
		TRIGGER_SET::iterator it = triggers->begin();
		while(it != triggers->end())
		{
			trigger* del = *it;
			if(del != NULL)
				delete del;
			it++;
		}
		delete triggers;
		triggers = NULL;
	}
}
/// Clear the list of neighbors, not the neighbors themselves
void grid::delete_neighbors()
{
	if(neighbors != NULL)
	{
		/// Just delete the list, not the elements
		delete[] neighbors;
		neighbors = NULL;
	}
}
/// Delete everything
grid::~grid()
{
	/// If this grid's info is not null
	if(ginfo != NULL)
	{
		/// If the info's position is not null
		if(ginfo->pos != NULL)
		{
			/// Clear the position
			delete ginfo->pos;
			ginfo->pos = NULL;
		}
		/// Clear the info
		delete ginfo;
		ginfo = NULL;
	}
	/// Delete everything else
	delete_neighbors();
	delete_points();
	delete_triggers();
}
/** Sets the next grid; used mainly by the loader
 * @param g The new next grid
 */
void grid::set_real_next(grid* g)
{
	neighbors[1] = g;
}
/** Sets the previous grid; used mainly by the loader
 * @param g The new previous grid
 */
void grid::set_real_prev(grid* g)
{
	neighbors[0] = g;
}
/** Draws the grid; default behavior is to draw a quad with attribute "points",
 * and draw a goal if this is a goal.
 * @param angle The current camera angle
 */
void grid::draw(vector3f angle)
{
	draw_rect(points[0], points[1], points[2], points[3]);
	draw_goal(angle);
}
/** Generates the set of points that outline the grid\n
 * Used for drawing, and deciding if a character lands on this grid\n
 * Default behavior is to return a square with width GRID_SIZE that is centered at the grid's position
 * @param my_info The grid's info, so that this method doesn't have to get it
 * @return The points
 */
vector3f** grid::generate_points(grid_info_t* my_info)
{
	vector3f** ret = new vector3f*[4];
	CHKPTR(ret);
	
	ret[0] = new vector3f(my_info->pos->x - HALF_GRID, my_info->pos->y, my_info->pos->z - HALF_GRID);
	CHKPTR(ret[0]);
	ret[1] = new vector3f(my_info->pos->x - HALF_GRID, my_info->pos->y, my_info->pos->z + HALF_GRID);
	CHKPTR(ret[1]);
	ret[2] = new vector3f(my_info->pos->x + HALF_GRID, my_info->pos->y, my_info->pos->z + HALF_GRID);
	CHKPTR(ret[2]);
	ret[3] = new vector3f(my_info->pos->x + HALF_GRID, my_info->pos->y, my_info->pos->z - HALF_GRID);
	CHKPTR(ret[3]);
	return(ret);
}
/** Gets the info of the grid; override for awesomeness
 * @param angle Current camera angle
 */
grid_info_t* grid::get_info(vector3f angle)
{
	return(ginfo);
}
/** Checks grids for equality, which just requires the positions to be
 * same.  The rationale is that one of the grids is a EscGrid, and thus
 * just comparing references won't necessarily detect equality.
 * @param g The other grid to compare to.
 * @param angle The current camera angle
 */
int grid::equals(grid* g, vector3f angle)
{
	/// Although, short-circuiting is ideal
	if(this == g)
		return(true);
	grid_info_t* i1 = get_info(angle);
	grid_info_t* i2 = g->get_info(angle);
	return(i1 && i2 && i1->pos == i2->pos);
}
/** Directs the character to the next grid.  Note that this have to be
 * bidirectional; the grid has to return the grid in the right direction
 * the character is traversing in this "linked list".
 * @param angle Current camera angle
 * @param current Current grid (actually, a grid next to this grid)
 * @return The next grid the character should go to
 */
grid* grid::get_next(vector3f angle, grid* current)
{
	if(current && neighbors[1] && current->equals(neighbors[1], angle))
		return(neighbors[0]);
	return(neighbors[1]);
}
/// Prettyprints the grid
void grid::dump()
{
	ECHO_PRINT("grid: [");
	if(ginfo)    dump_grid_info(*ginfo);
	else        ECHO_PRINT("NULL grid_info_t?");
	ECHO_PRINT("]");
}
/** Adds the trigger
 * @param trig Trigger to be added
 */
void grid::add_trigger(trigger* trig)
{
	triggers->insert(trig);
}
/// Sets the grid to be a goal; used mainly by the loader
void grid::set_as_goal()
{
	am_goal = 1;
}
/** Toggles the grid; if this grid is a goal before calling this, then
 * triggers will be toggled.
 * @param angle Current camera angle
 */
void grid::toggle_goal(vector3f angle)
{
	if(am_goal)	//triggers
	{
		TRIGGER_SET::iterator it = triggers->begin()
					, end = triggers->end();
		while(it != end)
		{
			(*it)->toggle(angle);
			it++;
		}
	}
	am_goal = !am_goal;
}
/** Is this grid a goal?
 * @param angle Current camera angle
 */
int grid::is_goal(vector3f angle)
{
	return(am_goal);
}
/// Should this grid be drawn?
int grid::should_draw()
{
	return(draw_me);
}
/// Sets the grid's draw property; should this grid be visible?
void grid::set_draw(int draw)
{
	draw_me = draw;
}
/** If this grid is a goal, then it draws a goal above the position of this grid.
 * @param angle Current camera angle
 */
void grid::draw_goal(vector3f angle)
{
	if(is_goal(angle))
	{
		draw_goal_gfx(get_info(angle)->pos);
	}
}
/** Is the given point on this grid?  Default behavior is to make sure
 * that the y is similar and that the x and z is within a HALF_GRID
 * distance.
 * @param angle Current camera angle
 * @param pt Point to check
 */
int grid::is_pt_on(vector3f angle, vector3f* pt)
{
	vector3f* pos = get_info(angle)->pos;
	return(ABS(pos->y - pt->y) < EPSILON && ABS(pos->x - pt->x) < HALF_GRID
				&& ABS(pos->z - pt->z) < HALF_GRID);
}
/** Checks for an intersection between a line of Screen Position points and
 * any of the edges formed by the points; used by holes and launchers.
 * @param p1 The first point of the line
 * @param p2 The second point of the line
 * @param angle Current camera angle
 */
int grid::projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle)
{
	vector3f* proj_pt0 = points[0]->neg_rotate_xy(angle);
	vector3f* proj_pt1 = points[1]->neg_rotate_xy(angle);
	vector3f* proj_pt2 = points[2]->neg_rotate_xy(angle);
	vector3f* proj_pt3 = points[3]->neg_rotate_xy(angle);
	
	int ret = lineSeg_intersect(p1, p2, proj_pt0, proj_pt1) 
		|| lineSeg_intersect(p1, p2, proj_pt1, proj_pt2)
		|| lineSeg_intersect(p1, p2, proj_pt2, proj_pt3)
		|| lineSeg_intersect(p1, p2, proj_pt3, proj_pt0);
	delete proj_pt0, proj_pt1, proj_pt2, proj_pt3;
	return(ret);
}
/** Sets the grid's land flag; can the character land on this grid?
 * @param land New land flag
 */
void grid::set_land(int land)
{
	landable = land;
}
/** Can the character land on this grid?
 * @param angle Current camera angle
 */
int grid::should_land(vector3f angle)
{
	return(landable);
}
#ifdef ECHO_NDS
	/** Gets the grid's polyID (see echo_gfx for more info on polyID)
	 * @param angle Current camera angle
	 */
	unsigned int grid::get_polyID(vector3f angle)
	{
		return(polyID);
	}
	/** Sets this grid's polyID
	 * @param my_polyID The grid's new polyID
	 */
	void grid::set_polyID(unsigned int my_polyID)
	{
		polyID = my_polyID;
	}
#endif

