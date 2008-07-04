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



#include <echo_debug.h>
#include <echo_error.h>
#include <echo_math.h>
#include <echo_gfx.h>

#include <grid.h>
#include <stair.h>

stair::stair() : grid()
{
	init(NULL, NULL, NULL, 0);
}

stair::stair(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle) : grid(0)
{
	init(my_info, my_prev, my_next, my_angle);
}

void stair::init(grid_info_t* my_info, grid* my_prev, grid* my_next, float my_angle)
{
	angle = my_angle;
	//ECHO_PRINT("stair angle 1: %f\n", angle);
	grid::init(my_info, my_prev, my_next);
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
	
	ECHO_PRINT("stair angle: %f\n", angle);
	
#ifdef STRICT_MEM
	vector3f* p1 = new vector3f(0.5, 0.5, 0.5);
	CHKPTR(p1);
	p1->self_rotate_about_y(angle);
	p1->add(my_info.pos);
	vector3f* p2 = new vector3f(-0.5, 0.5, 0.5);
	CHKPTR(p2);
	p2->self_rotate_about_y(angle);
	p2->add(my_info.pos);
	
	vector3f* p3 = new vector3f(0.5f, -0.5f, -0.5f);
	CHKPTR(p3);
	p3->self_rotate_about_y(angle);
	p3->add(my_info.pos);
	vector3f* p4 = new vector3f(-0.5f, -0.5f, -0.5f);
	CHKPTR(p4);
	p4->self_rotate_about_y(angle);
	p4->add(my_info.pos);
	
	ret[0].p1 = *p1;	ret[0].p2 = *p2;
	ret[1].p1 = *p3;	ret[1].p2 = *p4;
#else
	vector3f p1(0.5, 0.5, 0.5);
	p1.self_rotate_about_y(angle);
	p1.add(my_info.pos);
	vector3f p2(-0.5, 0.5, 0.5);
	p2.self_rotate_about_y(angle);
	p2.add(my_info.pos);
	
	vector3f p3(0.5, -0.5, -0.5);
	p3.self_rotate_about_y(angle);
	p3.add(my_info.pos);
	vector3f p4(-0.5, -0.5, -0.5);
	p4.self_rotate_about_y(angle);
	p4.add(my_info.pos);
	
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
	int eachi = 0;
	while(eachi < 2)
	{
		if( ( next_lines == NULL || !has_line(next_lines, my_lines[eachi]) )
			&& ( prev_lines == NULL || !has_line(prev_lines, my_lines[eachi]) ) )
		draw_line(my_lines[eachi]);
		eachi++;
	}
	
	gfx_push_matrix();
	gfx_translatef(ginfo->pos.x, ginfo->pos.y, ginfo->pos.z);
	gfx_rotatef(this->angle, 0, 1, 0);
	float each = -2 * 0.166f;
	while(each <= 0.5f)
	{
		if(each < 0.5f)
			draw_line(0.5f, each, each, -0.5f, each, each);			//length
		draw_line(0.5f, each - 0.166f, each, -0.5f, each - 0.166f, each);
		
		draw_rect(0.5f, each, each, 0.5f, each - 0.166f, each
			, -0.5f, each - 0.166f, each, -0.5f, each, each);
		
		draw_line(0.5f, each, each, 0.5f, each - 0.166f, each);		//side-down
		draw_line(-0.5f, each, each, -0.5f, each - 0.166f, each);
		
		draw_line(0.5f, each - 0.166f, each - 0.166f, 0.5f, each - 0.166f, each);	//side-across
		draw_line(-0.5f, each - 0.166f, each - 0.166f, -0.5f, each - 0.166f, each);
		
		draw_rect(0.5f, each - 0.166f, each - 0.166f, 0.5f, each - 0.166f, each
			, -0.5f, each - 0.166f, each, -0.5f, each - 0.166f, each - 0.166f);
		
		each += 0.166f;
	}
	gfx_pop_matrix();
}

