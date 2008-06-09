// trigger.h

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


#ifndef __ECHO_CLASS_TRIGGER__
#define __ECHO_CLASS_TRIGGER__

class trigger;

#include <grid.h>
#include <filter.h>

class trigger
{
	protected:
		filter* my_filter;
		grid* target;
	public:
		trigger();
		trigger(filter* filter);
		trigger(filter* filter, grid* my_target);
		void toggle(vector3f angle);
		void set_target(grid* my_target);
};

#endif

typedef std::set<trigger*> TRIGGER_SET;

