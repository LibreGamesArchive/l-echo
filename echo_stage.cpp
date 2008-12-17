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
	levels = new LEVEL_MAP();
	CHKPTR(levels);
	
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
	LEVEL_MAP::iterator level_it = levels->begin();
	LEVEL_MAP::iterator level_end = levels->end();
	while(level_it != level_end)
	{
		if(level_it->second != NULL)
			delete level_it->second;
	    ++level_it;
	}
	delete levels;
}

void stage::add(std::string id, grid* ptr)
{
    grids->insert(STAGE_MAP::value_type(id, ptr));
}

GRID_PTR_SET* map_get_level(LEVEL_MAP* levels, vector3f* pos)
{
	LEVEL_MAP::iterator it = levels->find(pos->y);
	if(it == levels->end())
		return(NULL);
	return(it->second);
}

void map_add_pos(LEVEL_MAP* levels, vector3f* pos, grid* g)
{
	GRID_PTR_SET* set = map_get_level(levels, pos);
	if(set)
	{
		set->insert(g);
	}
	else
	{
		set = new GRID_PTR_SET();
		CHKPTR(set);
		set->insert(g);
		levels->insert(LEVEL_MAP::value_type(pos->y, set));
	}
}

GRID_PTR_SET* stage::get_level(vector3f* pos)
{
	return(map_get_level(levels, pos));
}

void stage::add_pos(vector3f* pos, grid* g)
{
	if(pos->y < lowest)
		lowest = pos->y;
	map_add_pos(levels, pos, g);
}

float stage::get_lowest_level()
{
	return(lowest);
}

void stage::dump_levels()
{
	LEVEL_MAP::iterator it = levels->begin(), end = levels->end();
	ECHO_PRINT("levels: \n");
	while(it != end)
	{
		ECHO_PRINT("%f: size: %i\n", it->first, it->second->size());
		it++;
	}
}

grid* stage::get(std::string id)
{
    STAGE_MAP::iterator pos = grids->find(id);
    if(pos == grids->end())
        return(NULL);
    return(pos->second);
}

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
	//draw_character();
	gfx_outline_mid();
	it = grids->begin();
	while(it != end)
	{
	    if(it->second->should_draw())
		    it->second->draw(angle);
	    ++it;
	}
	//draw_character();
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

void stage::set_start(grid* g)
{
    start = g;
}

void stage::set_name(std::string* my_name)
{
    name = my_name;
}

void stage::set_num_goals(int my_num_goals)
{
    num_goals = my_num_goals;
}

grid* stage::get_start()
{
    return(start);
}

std::string* stage::get_name()
{
    return(name);
}

int stage::get_num_goals()
{
    return(num_goals);
}

LEVEL_MAP* stage::get_levels_lower_than(float y)
{
	LEVEL_MAP::iterator it = levels->begin(), end = levels->end();
	LEVEL_MAP* ret = new LEVEL_MAP();
	CHKPTR(ret);
	while(it != end)
	{
		if(it->first >= y)
			goto RET;
		ret->insert(LEVEL_MAP::value_type(it->first, it->second));
		it++;
	}
	RET:
	return(ret);
}

LEVEL_MAP* stage::get_levels_higher_than(float y)
{
	if(!levels->size())
		return(new LEVEL_MAP());
	LEVEL_MAP::iterator it = levels->end(), end = levels->begin();
	LEVEL_MAP* ret = new LEVEL_MAP();
	do
	{
		it--;
		if(it->first < y)
			goto RET;
		ret->insert(LEVEL_MAP::value_type(it->first, it->second));
	}
	while(it != end);
	RET:
	return(ret);
}

void stage::set_farthest(float new_far)
{
	if(farthest < new_far)
		farthest = new_far;
}

float stage::get_farthest()
{
	return(farthest);
}

