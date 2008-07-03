// stair.cpp

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

#include <echo_error.h>
#include <echo_math.h>
#include <echo_gfx.h>

#include <grid.h>
#include <stair.h>

stair::stair() : grid()
{
	init(NULL, NULL, NULL, 0);
}

stair::stair(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle) : grid()
{
	init(my_info, my_prev, my_next, my_angle);
}

void stair::init(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle)
{
	grid::init(my_info, my_prev, my_next);
	angle = my_angle;
}

void stair::init_to_null()
{
	grid::init_to_null();
	angle = 0;
}

line3f* stair::generate_lines(grid_info_t my_info)
{
	line3f* ret = new line3f[2];
	CHKPTR(ret);

#ifdef STRICT_MEM
	vector3f* p1 = new vector3f(0.5, 0.5, 0.5);
	CHKPTR(p1);
	p1->self_rotate_about_y(angle);
	vector3f* p2 = new vector3f(-0.5, 0.5, 0.5);
	CHKPTR(p2);
	p2->self_rotate_about_y(angle);
	
	vector3f* p3 = new vector3f(0.5, -0.5, -0.5);
	CHKPTR(p3);
	p3->self_rotate_about_y(angle);
	vector3f* p4 = new vector3f(-0.5, -0.5, -0.5);
	CHKPTR(p4);
	p4->self_rotate_about_y(angle);
	
	ret[0].p1 = *p1;	ret[0].p2 = *p2;
	ret[1].p1 = *p3;	ret[1].p2 = *p4;
#else
	vector3f p1(0.5, 0.5, 0.5);
	p1.self_rotate_about_y(angle);
	vector3f p2(-0.5, 0.5, 0.5);
	p2.self_rotate_about_y(angle);
	
	vector3f p3(0.5, -0.5, -0.5);
	p3.self_rotate_about_y(angle);
	vector3f p4(-0.5, -0.5, -0.5);
	p4.self_rotate_about_y(angle);
	
	ret[0].p1 = p1;	ret[0].p2 = p2;
	ret[1].p1 = p3;	ret[1].p2 = p4;
#endif
	
	return(ret);
}

void stair::draw(vector3f angle)
{
	draw_goal(angle);
	
	line3f* my_lines = get_lines(angle);
	//draw_rect(my_lines[0].p1, my_lines[1].p1, my_lines[2].p1, my_lines[3].p1);
	grid* my_next = get_real_next();
	line3f* next_lines = my_next != NULL ? my_next->get_lines(angle) : NULL;
	grid* my_prev = get_real_prev();
	line3f* prev_lines = my_prev != NULL ? my_prev->get_lines(angle) : NULL;
	int each = 0;
	while(each < 2)
	{
		if( ( next_lines == NULL || !has_line(next_lines, my_lines[each]) )
			&& ( prev_lines == NULL || !has_line(prev_lines, my_lines[each]) ) )
		draw_line(my_lines[each]);
		each++;
	}
}

