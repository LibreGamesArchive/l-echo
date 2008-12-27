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

void dump_grid_info(grid_info_t ginfo)
{
	ECHO_PRINT("grid_info_t: [");
	ginfo.pos->dump();
	ECHO_PRINT("]");
}

grid::grid()
{
	already_init = 0;
	init_to_null();
	init(NULL, NULL, NULL);
}
grid::grid(grid_info_t* my_info)
{
	already_init = 0;
	init_to_null();
	init(my_info, NULL, NULL);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	already_init = 0;
	init_to_null();
	init(my_info, my_prev, my_next);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor)
{
	already_init = 0;
	init_to_null();
	init(my_info, my_prev, my_next, num_neighbor);
}

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	init(my_info, my_prev, my_next, 2);
}

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

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, int my_num_neighbors)
{
	//ECHO_PRINT("6: %i\n", is_generate_lines);
	am_goal = 0;
	//goal_angle = 0;
	draw_me = 1;
	delete_triggers();
	triggers = new TRIGGER_SET();
	CHKPTR(triggers);
	
	ginfo = my_info;
	//ECHO_PRINT("ginfo: %p\n", ginfo);
	n_neighbors = my_num_neighbors;
	delete_neighbors();
	neighbors = new grid*[my_num_neighbors < 2 ? 2 : my_num_neighbors];
	CHKPTR(neighbors);
	neighbors[0] = my_prev;
	neighbors[1] = my_next;
	delete_points();
	points = ginfo != NULL ? generate_points(ginfo) : NULL;
	//ECHO_PRINT("points generated: %p\n", points);
}

void grid::delete_points()
{
	//ECHO_PRINT("deleting points\n");
	if(points != NULL)
	{
		//ECHO_PRINT("\tpoints not equal to NULL\n");
		delete points[0];
		delete points[1];
		delete points[2];
		delete points[3];
		delete[] points;
		points = NULL;
	}
}

void grid::delete_triggers()
{
	//ECHO_PRINT("deleting triggers\n");
	if(triggers != NULL)
	{
		//ECHO_PRINT("\ttriggers is NO NULL\n");
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

void grid::delete_neighbors()
{
	//ECHO_PRINT("deleting neighbors\n");
	if(neighbors != NULL)
	{
		//ECHO_PRINT("\tneighbors is NOT NULL\n");
		delete[] neighbors;
		neighbors = NULL;
	}
}

grid::~grid()
{
	//ECHO_PRINT("deallocating grid\n");
	if(ginfo != NULL)
	{
		if(ginfo->pos != NULL)
		{
			delete ginfo->pos;
			ginfo->pos = NULL;
		}
		delete ginfo;
		ginfo = NULL;
	}
	//*
	delete_neighbors();
	delete_points();
	delete_triggers();
	// */triggers
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
	//ECHO_PRINT("points generated: %p\n", points);
	draw_rect(points[0], points[1], points[2], points[3]);
	draw_goal(angle);	
}

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
	//ECHO_PRINT("points generated: %p, %p\n", ret, ret[0]);
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
		draw_goal_gfx(get_info(angle)->pos);
		/*
		goal_angle += 5;
		if(goal_angle == 360)
			goal_angle = 0;
		// */
	}
}

int grid::is_pt_on(vector3f angle, vector3f* pt)
{
	vector3f* pos = get_info(angle)->pos;
	return(ABS(pos->y - pt->y) < EPSILON && ABS(pos->x - pt->x) < HALF_GRID
				&& ABS(pos->z - pt->z) < HALF_GRID);
}

float grid::vert_shift(float percent_in)
{
	//return(-0.05f);
	return(0.05f * echo_cos(360 * percent_in) - 0.05f);
}

int grid::projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle)
{
	vector3f* proj_pt0 = points[0]->neg_rotate_xy(angle);
	vector3f* proj_pt1 = points[1]->neg_rotate_xy(angle);
	vector3f* proj_pt2 = points[2]->neg_rotate_xy(angle);
	vector3f* proj_pt3 = points[3]->neg_rotate_xy(angle);
	
	/*
	ECHO_PRINT("points of grid: ");
	ginfo->pos->dump();
	ECHO_PRINT("\n");
	points[0]->dump();
	proj_pt0->dump();
	ECHO_PRINT("\n");
	proj_pt1->dump();
	ECHO_PRINT("\n");
	proj_pt2->dump();
	ECHO_PRINT("\n");
	proj_pt3->dump();
	ECHO_PRINT("\n");
	ECHO_PRINT("\npoints: ");
	p1->dump();
	ECHO_PRINT("\n");
	p2->dump();
	ECHO_PRINT("\n");
	ECHO_PRINT("\n");
	// */
	int ret = lineSeg_intersect(p1, p2, proj_pt0, proj_pt1) 
		|| lineSeg_intersect(p1, p2, proj_pt1, proj_pt2)
		|| lineSeg_intersect(p1, p2, proj_pt2, proj_pt3)
		|| lineSeg_intersect(p1, p2, proj_pt3, proj_pt0);
	delete proj_pt0, proj_pt1, proj_pt2, proj_pt3;
	return(ret);
}

void grid::set_land(int land)
{
	landable = land;
}
int grid::should_land(vector3f angle)
{
	return(landable);
}

#ifdef ECHO_NDS
unsigned int grid::get_polyID(vector3f angle)
{
	return(polyID);
}
void grid::set_polyID(unsigned int my_polyID)
{
	polyID = my_polyID;
}
#endif

