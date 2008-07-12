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
#include <echo_error.h>
#include <echo_gfx.h>
#include <trigger.h>
#include <grid.h>
#include <echo_math.h>

void dump_grid_info(grid_info_t ginfo)
{
	ECHO_PRINT("grid_info_t: [");
	ginfo.pos.dump();
	ECHO_PRINT("]");
}

grid::grid()
{
	init_to_null();
	init(NULL, NULL, NULL);
}
grid::grid(grid_info_t* my_info)
{
	init_to_null();
	//ECHO_PRINT("2\n");
	init(my_info, NULL, NULL);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	init_to_null();
	//ECHO_PRINT("3\n");
	init(my_info, my_prev, my_next);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor)
{
	init_to_null();
	//ECHO_PRINT("4\n");
	init(my_info, my_prev, my_next, num_neighbor);
}

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	//ECHO_PRINT("5\n");
	init(my_info, my_prev, my_next, 2);
}

void grid::init_to_null()
{
	ginfo = NULL;
	neighbors = NULL;
	triggers = NULL;
	points = NULL;
#ifdef ARM9
	polyID = 0;
#endif
}

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, int my_num_neighbors)
{
	//ECHO_PRINT("6: %i\n", is_generate_lines);
	am_goal = 0;
	goal_angle = 0;
	draw_me = 1;
	if(triggers)
		delete triggers;
	triggers = new TRIGGER_SET();
	CHKPTR(triggers);
	
	ginfo = my_info;
	n_neighbors = my_num_neighbors;
	if(neighbors)
		delete[] neighbors;
	neighbors = new grid*[my_num_neighbors < 2 ? 2 : my_num_neighbors];
	CHKPTR(neighbors);
	neighbors[0] = my_prev;
	neighbors[1] = my_next;
	if(points)
		delete[] points;
	points = ginfo != NULL ? generate_points(*ginfo) : NULL;
}

grid::~grid()
{
	//*
	if(neighbors)
		delete[] neighbors;
	if(points)
		delete[] points;
	if(triggers)
		delete triggers;
	// */
}

grid* grid::get_real_next()
{
	return(neighbors[1]);
}

grid* grid::get_real_prev()
{
	return(neighbors[0]);
}

grid_info_t* grid::get_real_info()
{
	return(ginfo);
}

void grid::set_real_next(grid* g)
{
	neighbors[1] = g;
}

void grid::set_real_prev(grid* g)
{
	neighbors[0] = g;
}

void grid::draw(vector3f angle)	//TODO CHANGE FOR NORMALS
{
	draw_goal(angle);
	
	draw_rect(*(points[0]), *(points[1]), *(points[2]), *(points[3]));
}

vector3f** grid::generate_points(grid_info_t my_info)
{
	vector3f** ret = new vector3f*[4];
	CHKPTR(ret);
	
	ret[0] = new vector3f(my_info.pos.x - HALF_GRID, my_info.pos.y, my_info.pos.z - HALF_GRID);
	CHKPTR(ret[0]);
	ret[1] = new vector3f(my_info.pos.x - HALF_GRID, my_info.pos.y, my_info.pos.z + HALF_GRID);
	CHKPTR(ret[1]);
	ret[2] = new vector3f(my_info.pos.x + HALF_GRID, my_info.pos.y, my_info.pos.z + HALF_GRID);
	CHKPTR(ret[2]);
	ret[3] = new vector3f(my_info.pos.x + HALF_GRID, my_info.pos.y, my_info.pos.z - HALF_GRID);
	CHKPTR(ret[3]);
	return(ret);
}

grid_info_t* grid::get_info(vector3f angle)
{
	return(ginfo);
}

int grid::equals(grid* g, vector3f angle)
{
	grid_info_t* i1 = get_info(angle);
	grid_info_t* i2 = g->get_info(angle);
	return(i1 && i2 && i1->pos == i2->pos);
}

grid* grid::get_next(vector3f angle, grid* current)
{
	if(current && neighbors[1] && current->equals(neighbors[1], angle))
		return(neighbors[0]);
	return(neighbors[1]);
}

grid** grid::get_neighbors(vector3f angle)
{
	return(neighbors);
}

int grid::num_neighbors(vector3f angle)
{
	return(n_neighbors);
}

void grid::dump()
{
	ECHO_PRINT("grid: [");
	if(ginfo)    dump_grid_info(*ginfo);
	else        ECHO_PRINT("NULL grid_info_t?");
	ECHO_PRINT("]");
	//std::cout << "," << neighbors[0] << "," << neighbors[1] << "]";
}

void grid::add_trigger(trigger* trig)
{
	triggers->insert(trig);
}

void grid::set_as_goal()
{
	am_goal = 1;
}

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

int grid::is_goal(vector3f angle)
{
	return(am_goal);
}

int grid::should_draw()
{
	return(draw_me);
}
void grid::set_draw(int draw)
{
	draw_me = draw;
}

void grid::draw_goal(vector3f angle)
{
	if(is_goal(angle))
	{
		draw_goal_gfx(get_info(angle)->pos, goal_angle);
		goal_angle += 5;
		if(goal_angle == 360)
			goal_angle = 0;
	}
}

int grid::is_pt_on(vector3f angle, vector3f pt)
{
	vector3f pos = get_info(angle)->pos;
	//(pos - pt).dump();
	//std::cout << std::endl;
	return(ABS(pos.y - pt.y) < EPSILON && ABS(pos.x - pt.x) < HALF_GRID
				&& ABS(pos.z - pt.z) < HALF_GRID);
}

#ifdef ARM9
unsigned int grid::get_polyID(vector3f angle)
{
	return(polyID);
}
void grid::set_polyID(unsigned int my_polyID)
{
	polyID = my_polyID;
}
#endif

