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

#include <grid.h>
#include <echo_stage.h>

stage::stage()
{
    grids = new STAGE_MAP();
}

stage::stage(grid* my_start, std::string my_name, int my_num_goals)
{
    grids = new STAGE_MAP();
    start = my_start;
    name = my_name;
    num_goals = my_num_goals;
}

void stage::add(std::string id, grid* ptr)
{
    grids->insert(STAGE_MAP::value_type(id, ptr));
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
    while(it != end)
    {
        it->second->draw(angle);
        ++it;
    }
}

void stage::set_start(grid* g)
{
    start = g;
}

void stage::set_name(std::string my_name)
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

std::string stage::get_name()
{
    return(name);
}

int stage::get_num_goals()
{
    return(num_goals);
}
