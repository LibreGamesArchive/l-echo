// t_grid.h

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

#include <echo_math.h>
#include <grid.h>

#ifndef __ECHO_CLASS_T_GRID__
#define __ECHO_CLASS_T_GRID__
class t_grid : public grid
{
	public:
		t_grid();
		t_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, grid* my_next2);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, grid* my_next2);

		virtual ~t_grid();
		virtual grid* get_next(vector3f angle, grid* current);
		virtual void dump();
		virtual void draw(vector3f angle);

		virtual grid* get_real_next2();
		virtual void set_real_next2(grid* g);
};
#endif


