// echo_stage.cpp

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

#include <iostream>
#include <typeinfo>
#include <string>
#include <cfloat>

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <grid.h>
#include <echo_stage.h>
#include <echo_gfx.h>

stage::stage()
{
	init(NULL, NULL, 0);
}

stage::stage(grid* my_start, std::string* my_name, int my_num_goals) 
{
	init(my_start, my_name, my_num_goals);
}

void stage::init(grid* my_start, std::string* my_name, int my_num_goals)
{
	farthest = 0;
	lowest = FLT_MAX;
	grids = new STAGE_MAP();
	CHKPTR(grids);
	
	start = my_start;
	name = my_name;
	num_goals = my_num_goals;
}

stage::~stage()
{
	if(name != NULL)
		delete name;
	
	STAGE_MAP::iterator it = grids->begin();
	STAGE_MAP::iterator end = grids->end();
	while(it != end)
	{
		if(it->second != NULL)
			delete it->second;
	    ++it;
	}
	delete grids;
}
/** Adds the grid with the id.
 * @param id The id of the grid to add
 * @param ptr The grid to add
 */
void stage::add(std::string id, grid* ptr)
{
    grids->insert(STAGE_MAP::value_type(id, ptr));
}
/** Adds the position of the grid.  It was used in the old holes and launchers
 * implementation, but used right now just to calculate the lowest position.
 * @param pos The position of the grid.  Just has to be the position of the grid at some angle.
 * @param g The grid (isn't used now)
 */
void stage::add_pos(vector3f* pos, grid* g)
{
	if(pos->y < lowest)
		lowest = pos->y;
}
/// Gets the lowest y-coordinate that has a grid that can be landed on
float stage::get_lowest_level()
{
	return(lowest);
}
/// Gets a grid with the given string id
grid* stage::get(std::string id)
{
    STAGE_MAP::iterator pos = grids->find(id);
    if(pos == grids->end())
        return(NULL);
    return(pos->second);
}
/// Draws all the grids
void stage::draw(vector3f angle)
{
	STAGE_MAP::iterator it = grids->begin();
	STAGE_MAP::iterator end = grids->end();
#ifndef ECHO_NDS
	gfx_outline_start();
	while(it != end)
	{
	    if(it->second->should_draw())
		    it->second->draw(angle);
	    ++it;
	}
	gfx_outline_mid();
	it = grids->begin();
	while(it != end)
	{
	    if(it->second->should_draw())
		    it->second->draw(angle);
	    ++it;
	}
	gfx_outline_end();
#else
	while(it != end)
	{
	    if(it->second->should_draw())
	    {
		    gfx_set_polyID(it->second->get_polyID(angle));
		    it->second->draw(angle);
	    }
	    ++it;
	}
#endif
}
/// Sets the initial starting point of the stage
void stage::set_start(grid* g)
{
    start = g;
}
/// Sets the name of the stage
void stage::set_name(std::string* my_name)
{
    name = my_name;
}
/// Sets the number of goals of the stage
void stage::set_num_goals(int my_num_goals)
{
    num_goals = my_num_goals;
}
/// Gets the initial starting point of this stage
grid* stage::get_start()
{
    return(start);
}
/// Gets the name of the stage
std::string* stage::get_name()
{
    return(name);
}
/// Gets the total number of goals of the stage
int stage::get_num_goals()
{
    return(num_goals);
}
/** Tests if any grid is intersected by the line represented by the
 * Screen Position points
 * @param p1 Screen Position; point 1 of the line
 * @param p2 Screen Position; point 2 of the line
 * @param angle Current camera angle
 */
grid* stage::get_grid_intersection(vector3f* p1, vector3f* p2, vector3f angle)
{
	grid* ret = NULL;
	float shortest_dist = FLT_MAX;
	STAGE_MAP::iterator it = grids->begin();
	STAGE_MAP::iterator end = grids->end();
	while(it != end)
	{
		/// Short-circuit if you can't land on the line anyways
		if(it->second->should_land(angle) && it->second->projected_line_intersect(p1, p2, angle))
		{
			/// The line intersects, but we want the nearest grid
			grid_info_t* info = it->second->get_info(angle);
			if(info != NULL)
			{
				/// Get the distance from the first point
				vector3f* rot = info->pos->rotate_xy(angle);
				float dist = p1->dist(rot);
				if(dist < shortest_dist)
				{
					ret = it->second;
					shortest_dist = dist;
				}
				delete rot;
			}
		}
		it++;
	}
	return(ret);
}
/// Set the farthest position
void stage::set_farthest(float new_far)
{
	if(farthest < new_far)
		farthest = new_far;
}
/// Get the farthest position
float stage::get_farthest()
{
	return(farthest);
}

