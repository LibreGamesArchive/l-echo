// launcher.cpp

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

#include <cmath>
#include <iostream>

#include <echo_error.h>
#include <echo_ns.h>
#include <echo_gfx.h>
#include <launcher.h>
#include <echo_math.h>
#include <grid.h>
#include <isect_grid.h>

launcher::launcher() : escgrid()
{

}

launcher::launcher(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next) : escgrid()
{
	init(my_escangle, my_normal_info, my_esc_info, my_normal_prev, my_esc_prev, my_normal_next, my_esc_next);
}

launcher::launcher(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs) : escgrid()
{
	init(my_info, my_prev, my_next, my_escranges, my_escs, my_num_escs);
}

launcher::launcher(grid_info_t* my_info, grid* my_prev, grid* my_next) : escgrid()
{
    init(my_info, my_prev, my_next);
}

void launcher::init(vector3f* my_escangle, grid_info_t* my_normal_info, grid_info_t* my_esc_info
	, grid* my_normal_prev, grid* my_esc_prev, grid* my_normal_next, grid* my_esc_next)
{
	escgrid::init(my_escangle, my_normal_info, my_esc_info, echo_ns::hole_grid, my_esc_prev, echo_ns::hole_grid, my_esc_next);
	real_prev = my_normal_prev;
	real_next = my_normal_next;
}

void launcher::init(grid_info_t* my_info, grid* my_prev, grid* my_next, angle_range** my_escranges, grid** my_escs, int my_num_escs)
{
	escgrid::init(my_info, echo_ns::hole_grid, echo_ns::hole_grid, my_escranges, my_escs, my_num_escs);
	real_prev = my_prev;
	real_next = my_next;
}

void launcher::init(grid_info_t* my_info, grid* my_prev, grid* my_next)
{
    launcher::init(my_info, my_prev, my_next, NULL, NULL, 0);
}

launcher::~launcher()
{
}

void launcher::init_to_null()
{
	escgrid::init_to_null();
	real_prev = NULL;
	real_next = NULL;
}

void launcher::draw(vector3f angle)
{
	escgrid::draw(angle);
	draw_launcher(get_info(angle)->pos);
}

void launcher::set_real_next(grid* g)
{
	real_next = g;
}

void launcher::set_real_prev(grid* g)
{
	real_prev = g;
}

grid* launcher::get_real_next()
{
	return(real_next);
}

grid* launcher::get_real_prev()
{
	return(real_prev);
}

#define VERTEX_Z   				2
#define VERTEX_Y				4
#define	INTERCEPT_Z				4
#define STATIC_STEP				0.25f
#define GET_Z(y)				(2 + sqrtf(4 - (y)))
#ifdef WIN32
	#define GET_Y(z)				(4 - ((z) - 2) * ((z) - 2))
#else
	#define GET_Y(z)				(4 - powf((z) - 2, 2))	//a bug in mingw prevents linking with powf...
#endif
#define TRANS_TO_LAUNCH(vec, angle, pos)	(*(vec.rotate_about_y(angle)) + pos)
#define TRANS_PTR_TO_LAUNCH(vec, angle, pos)	(*(vec->rotate_about_y(angle)) + pos)

grid* launcher::get_next(vector3f angle, grid* current)
{
	grid* esc = get_esc(angle);
	if(esc)
		return(esc->get_next(angle, current));
	vector3f pos = get_info(angle)->pos;
	
	grid* my_prev = get_real_prev();
	vector3f* direction;
	if(my_prev)
	{
		grid_info_t* info = my_prev->get_info(angle);
		direction = info ? &(pos - info->pos) : new vector3f(0, 0, 1);
	}
	else
		direction = new vector3f(0, 0, 1);
	CHKPTR(direction);
	
	float launch_angle = TO_DEG(atan2f(direction->x, direction->z));
	std::cout << "angle of launcher: " << launch_angle << std::endl;
	
	/*
	vector3f* vertex = new vector3f(0, VERTEX_Y, VERTEX_Z);
	vector3f rotated_vertex = TRANS_TO_LAUNCH(vertex, launch_angle, pos);
	std::cout << "translated vertex: ";
	rotated_vertex.dump();
	std::cout << std::endl;
	// */
	
	grid* begin = NULL;
	grid* temp1 = this;
	grid* temp2 = NULL;
	
	float z = STATIC_STEP;
	while(z <= VERTEX_Z)
	{
		grid_info_t* info = new(grid_info_t);
		CHKPTR(info);
		info->pos.set(0, GET_Y(z), z);
		info->pos = TRANS_TO_LAUNCH(info->pos, launch_angle, pos);
		/*
		std::cout << "info->pos: ";
		info->pos.dump();
		std::cout << std::endl;
		// */
		
		temp2 = new static_grid(info, temp1, echo_ns::hole_grid, angle);
		CHKPTR(temp2);
		if(temp1 != this)
			temp1->set_real_next(temp2);
		temp1 = temp2;
		if(!begin)
			begin = temp1;
		
		//std::cout << "z: " << z << std::endl;
		z += STATIC_STEP;
	}
	//return(begin);
	
	//*
	LEVEL_MAP* levels = echo_ns::current_stage->get_levels_lower_than(pos.y + VERTEX_Y);
	//std::cout << "levels->size(): " << levels->size() <<std::endl;
	if(levels->size() > 0)
	{
		LEVEL_MAP::iterator it = levels->end(), end = levels->begin();
		it--;
		int past_end = 0, dup_static = 0;
		std::cout << "last y: " << end->first << std::endl;
		float last_z = (int)(GET_Z(end->first - pos.y) / STATIC_STEP) * STATIC_STEP, this_y;
		if(last_z < INTERCEPT_Z)
			last_z = INTERCEPT_Z;
		std::cout << "last z: " << last_z << std::endl;
		while(z <= last_z)
		{
			this_y = GET_Y(z) + pos.y;
			while(!past_end && it->first >= this_y)
			{
				//std::cout << "LEVEL: " << it->first << ", " << GET_Y(z) << ", " << pos.y << std::endl;
				grid_info_t* info = new(grid_info_t);
				CHKPTR(info);
				info->pos.set(0, it->first - pos.y, GET_Z(it->first - pos.y));
				info->pos.dump();
				std::cout << std::endl;
				info->pos = TRANS_TO_LAUNCH(info->pos, launch_angle, pos);
				info->pos.dump();
				std::cout << std::endl;
				temp2 = new isect_grid(info, temp1, echo_ns::hole_grid, angle, it->second);
				CHKPTR(temp2);
				temp1->set_real_next(temp2);
				temp1 = temp2;
				
				if(it->first == this_y)
				{
					std::cout << "dup_static..." << std::endl;
					dup_static = 1;
				}
				if(it == end)
					past_end = 1;
				else
					it--;
			}
			//std::cout << "STEP: " << z<< std::endl;
			if(!dup_static)
			{
				grid_info_t* info = new(grid_info_t);
				CHKPTR(info);
				info->pos.set(0, GET_Y(z), z);
				info->pos = TRANS_TO_LAUNCH(info->pos, launch_angle, pos);
				temp2 = new static_grid(info, temp1, echo_ns::hole_grid, angle);
				CHKPTR(temp2);
				temp1->set_real_next(temp2);
				temp1 = temp2;
			}
			else
				dup_static = 0;
			z += STATIC_STEP;
		}
		/*
		do
		{
			it--;
			grid_info_t* info = new(grid_info_t);
			info->pos.set(0, it->first - pos.y, GET_Z(it->first - pos.y));
			info->pos.dump();
			std::cout << std::endl;
			info->pos = TRANS_TO_LAUNCH(info->pos, launch_angle, pos);
			info->pos.dump();
			std::cout << std::endl;
			temp2 = new isect_grid(info, temp1, echo_ns::hole_grid, angle, it->second);
			temp1->set_real_next(temp2);
			temp1 = temp2;
		}
		while(it != end);
		// */
		//std::cout << "begin: " << end->second << std::endl;
	}
	return(begin ? begin : echo_ns::hole_grid);
	// */
}

