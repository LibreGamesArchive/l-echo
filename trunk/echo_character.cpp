// echo_character.cpp

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

#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <cmath>

#include <echo_platform.h>
#include <echo_gfx.h>
#include <echo_debug.h>
#include <echo_math.h>
#include <echo_ns.h>
#include <echo_character.h>

#include <launcher.h>
#include <grid.h>
#include <hole.h>
#include <isect_grid.h>

//how high aboce the start grid does the character start?
#define STARTY		30

//various character speeds
#define SPEED_STEP 	0.08f
#define SPEED_FALL 	0.50f
#define SPEED_LAUNCH	0.30f

echo_char::echo_char()
{
	num_goals = 0;
	init(NULL);
}
echo_char::echo_char(grid* g1)
{
	num_goals = 0;
	init(g1);
}

int echo_char::is_paused()
{
	return(paused);
} 

int echo_char::num_goals_reached()
{
	return(num_goals);
}

void echo_char::change_speed()
{
	if(grid1 && grid2)
	{
		//first grid is an hole, and second is an isect_grid or just falling nowhere
		if(typeid(*grid1) == typeid(hole) && (typeid(*grid2) == typeid(isect_grid)
			|| grid2 == echo_ns::hole_grid))
		{
			ECHO_PRINT("falling into hole...\n");
			speed = SPEED_FALL;
		}
		//first grid is a launcher, and second is an static_grid or just falling nowhere
		else if(typeid(*grid1) == typeid(launcher) && (typeid(*grid2) == typeid(static_grid)
			|| grid2 == echo_ns::hole_grid))
		{
			ECHO_PRINT("being launched!\n");
			speed = SPEED_LAUNCH;	
		}
		//if this character isn't in midair
		else if(typeid(*grid1) == typeid(isect_grid)
			&& (typeid(*grid2) != typeid(isect_grid) 
				&& typeid(*grid2) != typeid(static_grid)))
		{
			ECHO_PRINT("normal speed\n");
			speed = SPEED_STEP;
		}
	}
}
void echo_char::init(grid * g1)
{
	start = grid1 = g1;
	grid2 = g1 ? grid1->get_next(echo_ns::angle, grid1) : NULL;
	
	paused = 0;
	grid1per = 1;
	grid2per = 0;
	//haven't started falling out of the sky
	startper = 1;
	speed = SPEED_STEP;
	dist = 1;
	change_speed();
}

void echo_char::toggle_pause()
{
	paused = !paused;
}

void echo_char::kill()
{
	//start falling
	startper = -0.05;
}

void echo_char::reset()
{
	init(start);
}

void echo_char::next_grid()
{
	if(grid1->is_goal(echo_ns::angle))
	{
		grid1->toggle_goal(echo_ns::angle);
		num_goals++;
	}
	//falling to nowhere
	if(grid1 == echo_ns::hole_grid)
		kill();
	else if(grid2)
	{
		//replace grid1 with grid2, grid2 with the next grid
		grid* temp = grid2;
		grid2 = grid2->get_next(echo_ns::angle, grid1);
		// if the  next grid is nowhere, die.
		if(grid2 == echo_ns::hole_grid)	kill();
		grid1 = temp;
		change_speed();
	}
	else
		grid2 = NULL;
	//reset
	grid1per = 1;
	grid2per = 0;
}

void echo_char::step()
{
	gfx_color3f(0.5f, 0.5f, 0.5f);
	if(startper > 0)
	{
		vector3f pos1 = start->get_info(echo_ns::angle)->pos;
		startper -= 0.05;
		if (startper < 0)
			startper = 0;
		draw(pos1.x, pos1.y + STARTY * startper, pos1.z);
		return;
	}
	else if(startper < 0)
	{
		vector3f pos1 = grid1->get_info(echo_ns::angle)->pos;
		startper -= 0.05;
		if(startper < -1)
			init(start);
		draw(pos1.x, pos1.y + STARTY * startper, pos1.z);
		return;
	} 
	else if(grid1)
	{
		if (grid2 == echo_ns::hole_grid)
			kill();
		else
		{
			if(grid1->is_goal(echo_ns::angle))
			{
				grid1->toggle_goal(echo_ns::angle);
				num_goals++;
			}
			if(grid1 && grid2 && !paused)	//if both grids are there
			{
				/*
				if(speed == SPEED_LAUNCH)
				{
					grid1per -= speed;
					grid2per += speed;
				}
				else
				// */
				{
					grid1per -= speed / dist;	//step thru it
					grid2per += speed / dist;
				}
				if(grid1per <= 0)	//if we've reached the end of this cycle
				{
					next_grid();	//on to the next cycle
				}
			}
			if(grid1 && !grid2)	//if there isn't a second grid...
			{
				grid2 = grid1->get_next(echo_ns::angle, grid1);	//try to get one
				change_speed();
				if(!grid2)	//if there still isn't a second grid...
				{
					draw(grid1->get_info(echo_ns::angle)->pos);	//return grid1's position
					return;
				}
			}
			grid_info_t* i1 = grid1->get_info(echo_ns::angle);
			if(i1)
			{
				vector3f pos1 = i1->pos;
				grid_info_t* i2 = grid2->get_info(echo_ns::angle);
				if(i2)
				{
					vector3f pos2 = i2->pos;
					dist = pos1.dist(pos2);
					draw(pos1.x * grid1per + pos2.x * grid2per,
								pos1.y * grid1per + pos2.y * grid2per,
								pos1.z * grid1per + pos2.z * grid2per);
					return;
				}
				else
				{
					draw(i1->pos);	//there isn't a second grid?  but wait, wouldn't this be return already?
					return;
				}
			}
		}
	}
}

void echo_char::draw(vector3f vec)
{
	draw(vec.x, vec.y, vec.z);
}

void echo_char::draw(float x, float y, float z)
{
	//*
	gfx_push_matrix();
	gfx_translatef(x, y, z);
	if(grid1 && grid2)
	{
		grid_info_t* i1 = grid1->get_info(echo_ns::angle);
		grid_info_t* i2 = grid2->get_info(echo_ns::angle);
		if(i1 && i2)
		{
			gfx_rotatef(90 - TO_DEG(atan2(i2->pos.z - i1->pos.z, i2->pos.x - i1->pos.x))
				, 0, 1, 0);
		}
	}
#ifndef ECHO_NDS
	//gfx_colorf
	gfx_outline_start();
	draw_character();
	gfx_outline_mid();
	draw_character();
	gfx_outline_end();
#else
	gfx_set_polyID(1);
	draw_character();
#endif
	gfx_pop_matrix();
	// */
}
