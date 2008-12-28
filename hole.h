// hole.h

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
#include <escgrid.h>

#ifndef __ECHO_CLASS_HOLE__
#define __ECHO_CLASS_HOLE__
class hole : public escgrid
{
	public:
		hole();
		hole(grid_info_t* my_info, grid* my_prev, grid* my_next);
		hole(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
			, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next);
		hole(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs);
		void init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
			, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next);
		
		virtual ~hole();
		virtual grid* get_next(vector3f angle, grid* current);
		virtual void draw(vector3f angle);
};
#endif
