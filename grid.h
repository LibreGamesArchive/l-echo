// grid.h

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
#include <filter.h>
#include <trigger.h>
#include <echo_math.h>

#define GRID_SIZE	1
#define HALF_GRID	0.5f

#ifndef __ECHO_GRID_INFO_T__
#define __ECHO_GRID_INFO_T__
typedef struct
{
	vector3f pos; //TODO add normal capabilities
}
grid_info_t;

void dump_grid_info(grid_info_t ginfo);
#endif

#define LINE_PT(ln, pt)	((ln) * 4 + (pt))
#define LINE(ptr, ln)	(ptr)[LINE_PT(ln, 0)], (ptr)[LINE_PT(ln, 1)]

#ifndef __ECHO_CLASS_GRID__
#define __ECHO_CLASS_GRID__

class grid;

typedef std::set<grid*> GRID_PTR_SET;

class grid
{
	protected:
		line3f* lines;
		grid_info_t* ginfo;
		grid** neighbors;
		int n_neighbors;

		int am_goal;
		float goal_angle;
		int draw_me;
		
		TRIGGER_SET* triggers;
	public:
		grid();
		grid(int is_generate_lines);
		grid(grid_info_t* my_info, grid* my_prev, grid* my_next);
		grid(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor);
		grid(grid_info_t* my_info);
		virtual void init_to_null();
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next);
		//void init(grid_info_t* my_info, grid* my_prev, grid* my_next, int is_generate_lines);
		void init(grid_info_t* my_info, grid* my_prev, grid* my_next, int num_neighbor, int is_generate_lines);

		virtual ~grid();
		virtual grid_info_t* get_info(vector3f angle);
		virtual grid* get_next(vector3f angle, grid* current);
		virtual void dump();

		virtual grid** get_neighbors(vector3f angle);
		int num_neighbors(vector3f angle);
		virtual int equals(grid* g, vector3f angle);

		virtual grid* get_real_next();
		virtual grid* get_real_prev();
		virtual grid_info_t* get_real_info();
		virtual void set_real_next(grid* g);
		virtual void set_real_prev(grid* g);

		virtual void add_trigger(trigger* trig);
		virtual void toggle_goal(vector3f angle);
		virtual int is_goal(vector3f angle);
		virtual void set_as_goal();

		virtual line3f* generate_lines(grid_info_t my_info);
		virtual line3f* get_lines(vector3f angle);
		
		virtual int should_draw();
		virtual void set_draw(int draw);
		virtual void draw(vector3f angle);

		virtual int is_pt_on(vector3f angle, vector3f pt);

		void draw_goal(vector3f angle);
};
#endif

