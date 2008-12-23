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

escgrid::escgrid() : grid()
{
	init_to_null();
	delete_at_deconstruct = 1;
	init(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

escgrid::escgrid(grid_info_t* my_info, grid* my_prev, grid* my_next) : grid()
{
	init_to_null();
	delete_at_deconstruct = 1;
	init(my_info, my_prev, my_next);
}

escgrid::escgrid(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next) : grid()
{
	init_to_null();
	delete_at_deconstruct = 1;
	init(my_escangle, my_normal_info, my_esc_info, my_normal_prev, my_esc_prev, my_normal_next, my_esc_next);
}

void escgrid::delete_table(int delete_every)
{
	//ECHO_PRINT("deleting table for escgrid\n");
	if(delete_at_deconstruct)
	{
		//ECHO_PRINT("delete_at_deconstruct is true\n");
		if(escs)
		{
			//ECHO_PRINT("deleting escs\n");
			//std::cout << "lol1";
			if(delete_every == 1)
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
			//ECHO_PRINT("deleting ranges\n");
			//std::cout << "lol2";
			if(delete_every == 1)
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

void escgrid::init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next)
{
	grid::init(my_normal_info, my_normal_prev, my_normal_next);
	num_esc = 1;
	delete_table(1);
	ranges = new angle_range*[1];
	CHKPTR(ranges);
	escs = new grid*[1];
	CHKPTR(escs);
	delete_at_deconstruct = 1;
	ranges[0] = VECPTR_TO_RANGE(my_escangle);
	escs[0] = new grid(my_esc_info, my_esc_prev, my_esc_next);
}

void escgrid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
	//ECHO_PRINT("\nINIT ESCGRID\n\n");
	grid::init(my_info, my_prev, my_next);
	//ECHO_PRINT("\nEND INIT ESCGRID: GRID\n\n");
	delete_table(1);
	ranges = NULL;
	escs = NULL;
	delete_at_deconstruct = 1;
	num_esc = 0;
}

void escgrid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs)
{
	grid::init(my_info, my_prev, my_next);
	delete_table(1);
	ranges = my_escranges;
	escs = my_escs;
	delete_at_deconstruct = 0;
	num_esc = my_num_escs;
}

escgrid::~escgrid()
{
	delete_table(1);
}

void escgrid::init_to_null()
{
	grid::init_to_null();
	ranges = NULL;
	escs = NULL;
}

void escgrid::add(vector3f* vec, grid* esc)
{
	add(VECPTR_TO_RANGE(vec), esc);
}

void escgrid::add(angle_range* range, grid* esc)
{
	//delete_table();
	//*
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
	// */
	if(am_goal)
		esc->set_as_goal();
	ranges[num_esc] = range;
	escs[num_esc] = esc;
	num_esc++;
}

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

int escgrid::equals(grid* g, vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->equals(g, angle) : grid::equals(g, angle));
}

void escgrid::draw(vector3f angle)
{
	grid* esc = get_esc(angle);
	if(esc) esc->draw(angle);
	else    grid::draw(angle);
}

grid_info_t* escgrid::get_info(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_info(angle) : grid::get_info(angle));
}

/*
line3f* escgrid::get_lines(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_lines(angle) : grid::get_lines(angle));
}
// */

grid* escgrid::get_next(vector3f angle, grid* current)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_next(angle, current) : grid::get_next(angle, current));
}

grid** escgrid::get_neighbors(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_neighbors(angle) : grid::get_neighbors(angle));
}

int escgrid::num_neighbors(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->num_neighbors(angle) : grid::num_neighbors(angle));
}

#ifdef ECHO_NDS
unsigned int escgrid::get_polyID(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_polyID(angle) : grid::get_polyID(angle));
}
#endif

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

int escgrid::is_goal(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->is_goal(angle) : grid::is_goal(angle));
}
int escgrid::should_land(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->should_land(angle) : grid::should_land(angle));
}

int escgrid::projected_line_intersect(vector3f* p1, vector3f* p2, vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->projected_line_intersect(p1, p2, angle) : grid::projected_line_intersect(p1, p2, angle));
}
