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

#include <cstdlib>
#include <iostream>
#include <grid.h>
#include <escgrid.h>
#include <echo_math.h>
#include <echo_gfx.h>

escgrid::escgrid()
{
	delete_at_deconstruct = 1;
	init(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

escgrid::escgrid(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
    delete_at_deconstruct = 1;
    init(my_info, my_prev, my_next);
}

escgrid::escgrid(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next)
{
	delete_at_deconstruct = 1;
	init(my_escangle, my_normal_info, my_esc_info, my_normal_prev, my_esc_prev, my_normal_next, my_esc_next);
}

void escgrid::delete_table()
{
	if(delete_at_deconstruct)
	{
		if(escs)
		{
			//std::cout << "lol1";
			delete[] escs;
		}
		if(vecs)
		{
			//std::cout << "lol2";
			delete[] vecs;
		}
	}
}

void escgrid::init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next)
{
	grid::init(my_normal_info, my_normal_prev, my_normal_next);
	num_esc = 1;
	vecs = new vector3f*[1];
	escs = new grid*[1];
	delete_at_deconstruct = 1;
	vecs[0] = my_escangle;
	escs[0] = new grid(my_esc_info, my_esc_prev, my_esc_next);
}

void escgrid::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
    grid::init(my_info, my_prev, my_next);
	vecs = NULL;
	escs = NULL;
	delete_at_deconstruct = 1;
	num_esc = 0;
}

void escgrid::init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f** my_escvecs, grid** my_escs, int my_num_escs)
{
	grid::init(my_info, my_prev, my_next);
	vecs = my_escvecs;
	escs = my_escs;
	delete_at_deconstruct = 0;
	num_esc = my_num_escs;
}

escgrid::~escgrid()
{
	delete_table();
}

void escgrid::add(vector3f* vec, grid* esc)
{
	//delete_table();
	//*
	vector3f** new_vecs = new vector3f*[num_esc + 1];
	grid** new_escs = new grid*[num_esc + 1];
	if(escs)
	{
		int each = 0;
		while(each < num_esc)
		{
			new_vecs[each] = vecs[each];
			new_escs[each] = escs[each];
			each++;
		}
		delete_table();
	}
	new_vecs[num_esc] = vec;
	new_escs[num_esc] = esc;
	escs = new_escs;
   	vecs = new_vecs;
	// */
	vecs[num_esc] = vec;
	escs[num_esc] = esc;
	num_esc++;
}

grid* escgrid::get_esc(vector3f angle)
{
	int each = 0;
	while(each < num_esc)
	{
		if(escs[each] && vecs[each]->angle_similar(angle))
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

line3f* escgrid::get_lines(vector3f angle)
{
	grid* esc = get_esc(angle);
	return(esc ? esc->get_lines(angle) : grid::get_lines(angle));
}

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
