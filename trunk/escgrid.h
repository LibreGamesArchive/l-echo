// escgrid.h

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

#include <grid.h>
#include <echo_math.h>

#ifndef __ECHO_CLASS_ESCGRID__
#define __ECHO_CLASS_ESCGRID__
class escgrid : public grid
{
	protected:
		int num_esc;
		int delete_at_deconstruct;
		vector3f** vecs;
		grid** escs;

	public:
		escgrid();
		escgrid(grid_info_t* my_info, grid* my_prev, grid* my_next);
		escgrid(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
			, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next);
		void init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
			, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f** my_escvecs, grid** my_escs, int my_num_escs);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next);

		void add(vector3f* vec, grid* esc);

		virtual ~escgrid();
		virtual grid_info_t* get_info(vector3f angle);
		virtual grid* get_next(vector3f angles, grid* current);
		virtual line3f* get_lines(vector3f angle);
		virtual void draw(vector3f angle);
		virtual int equals(grid* g, vector3f angle);

		grid* get_esc(vector3f angle);
	protected:
		virtual grid** get_neighbors(vector3f angle);
		virtual int num_neighbors(vector3f angle);
		void delete_table();

};
#endif

