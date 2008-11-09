// filter.cpp

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

#include <grid.h>
#include <filter.h>
#include <echo_math.h>
#include <echo_error.h>

filter::filter()
{
	target = NULL;
}
filter::~filter()
{
	//ECHO_PRINT("deleting filter\n");
}

filter::filter(grid* my_target)
{
	target = my_target;
}

int filter::is_true(vector3f angle)
{
	return(target->is_goal(angle));
}

void filter::set_target(grid* my_target)
{
	target = my_target;
}

not_filter::not_filter(filter* my_filter)
{
	f = my_filter;
}
not_filter::~not_filter()
{
	//ECHO_PRINT("deleting not_filter\n");
	delete f;
}
int not_filter::is_true(vector3f angle)
{
	return(!f->is_true(angle));
}
int or_filter::is_true(vector3f angle)
{
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		if((*it)->is_true(angle))
			return(1);
		it++;
	}
	return(0);
}
int and_filter::is_true(vector3f angle)
{
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		if(!((*it)->is_true(angle)))
			return(0);
		it++;
	}
	return(1);
}

multi_filter::multi_filter()
{
	filters = new FILTER_SET();
	CHKPTR(filters);
}

multi_filter::~multi_filter()
{
	//ECHO_PRINT("deleting multi_filter\n");
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		filter* del = *it;
		if(del != NULL)
			delete del;
		it++;
	}
	delete filters;
}
void multi_filter::add_filter(filter* f)
{
	filters->insert(f);
}
