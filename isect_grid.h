// isect_grid.h

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
#include <static_grid.h>
#include <echo_stage.h>

#ifndef __ECHO_CLASS_ISECT_GRID__
#define __ECHO_CLASS_ISECT_GRID__
class isect_grid : public static_grid
{
	protected:
		GRID_PTR_SET* level;
		float level_y;
		grid* cam_grid;
	public:
		isect_grid();
		isect_grid(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f camera, GRID_PTR_SET* my_level);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, vector3f camera, GRID_PTR_SET* my_level);
		
		virtual grid* get_next(vector3f angle, grid* current);
		virtual void force_refresh(vector3f camera);
		virtual grid_info_t* get_info(vector3f angle);
		virtual void init_to_null();
};
#endif


