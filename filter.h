// filter.h

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


#include <echo_math.h>

#ifndef __ECHO_CLASS_FILTER__
#define __ECHO_CLASS_FILTER__
class filter;

#include <grid.h>
#include <echo_platform.h>
#include <echo_debug.h>

class filter
{
	protected:
		grid* target;
	public:
		filter();
		filter(grid* my_target);
		virtual ~filter();
		virtual int is_true(vector3f angle);
		void set_target(grid* my_target);
};

class not_filter : public filter
{
	protected:
		filter* f;
	public:
		not_filter(filter* f);
		virtual ~not_filter();
		int is_true(vector3f angle);
};

class or_filter : public filter
{
	protected:
		FILTER_SET* filters;
	public:
		or_filter();
		virtual ~or_filter();
		void add_filter(filter* f);
		int is_true(vector3f angle);
};

class and_filter : public filter
{
	protected:
		FILTER_SET* filters;
	public:
		and_filter();
		virtual ~and_filter();
		void add_filter(filter* f);
		int is_true(vector3f angle);
};

#endif

typedef std::set<filter*> FILTER_SET;
