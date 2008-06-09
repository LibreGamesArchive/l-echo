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
	init(my_info, NULL, NULL);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	init_to_null();
	init(my_info, my_prev, my_next);
}

grid::grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor)
{
	init_to_null();
	init(my_info, my_prev, my_next, num_neighbor);
}

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	init_to_null();
	init(my_info, my_prev, my_next, 2);
}

void grid::init_to_null()
{
	lines = NULL;
	ginfo = NULL;
	neighbors = NULL;
	triggers = NULL;
}

void grid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, int my_num_neighbors)
{
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
	if(lines)
		delete[] lines;
	lines = ginfo != NULL ? generate_lines(*ginfo) : NULL;
}

static int has_line(line3f* ptr, line3f line)
{
	return(ptr[0] == line || ptr[1] == line || ptr[2] == line || ptr[3] == line);
}

static void dump_lines(line3f* ptr)
{
	dump_line3f(ptr[0]);
	ECHO_PRINT("\n");
	dump_line3f(ptr[1]);
	ECHO_PRINT("\n");
	dump_line3f(ptr[2]);
	ECHO_PRINT("\n");
	dump_line3f(ptr[3]);
	ECHO_PRINT("\n");
}

grid::~grid()
{
	//*
	if(neighbors)
		delete[] neighbors;
	if(lines)
		delete[] lines;
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
	
	line3f* my_lines = get_lines(angle);
	draw_rect(my_lines[0].p1, my_lines[1].p1, my_lines[2].p1, my_lines[3].p1);
	grid* my_next = get_real_next();
	line3f* next_lines = my_next != NULL ? my_next->get_lines(angle) : NULL;
	grid* my_prev = get_real_prev();
	line3f* prev_lines = my_prev != NULL ? my_prev->get_lines(angle) : NULL;
	int each = 0;
	while(each < 4)
	{
		if( ( next_lines == NULL || !has_line(next_lines, my_lines[each]) )
			&& ( prev_lines == NULL || !has_line(prev_lines, my_lines[each]) ) )
		draw_line(my_lines[each]);
		each++;
	}
}

line3f* grid::generate_lines(grid_info_t my_info)
{
	line3f* ret = new line3f[4];
	CHKPTR(ret);
	
	vector3f pos = my_info.pos;
	vector3f p1(pos.x - HALF_GRID, pos.y, pos.z - HALF_GRID);
	vector3f p2(pos.x - HALF_GRID, pos.y, pos.z + HALF_GRID);
	vector3f p3(pos.x + HALF_GRID, pos.y, pos.z + HALF_GRID);
	vector3f p4(pos.x + HALF_GRID, pos.y, pos.z - HALF_GRID);
	
	ret[0].p1 = p1;	ret[0].p2 = p2;
	ret[1].p1 = p2;	ret[1].p2 = p3;
	ret[2].p1 = p3;	ret[2].p2 = p4;
	ret[3].p1 = p4;	ret[3].p2 = p1;
	
	return(ret);
}

line3f* grid::get_lines(vector3f angle)
{
	return(lines);
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

