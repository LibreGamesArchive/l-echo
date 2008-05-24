// echo_ns.cpp

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

#include <echo_math.h>
#include <grid.h>
#include <echo_ns.h>
#include <hole.h>
#include <isect_grid.h>

#define STARTY		30
#define SPEED_STEP 	0.01f
#define SPEED_FALL 	0.10f

namespace echo_ns
{
	grid *hole_grid = new grid();
	vector3f angle;
	grid *start;
	grid *grid1;
	grid *grid2;
	float grid1per, grid2per, startper, dist, speed;
	int paused = 1;
	int num_goals = 0;
	
	stage *current_stage;
	
	static void change_speed()
	{
		if(typeid(*grid1) == typeid(hole))
		{
			std::cout << "falling into hole..." << std::endl;
			speed = SPEED_FALL;
		}
		if(grid2 && typeid(*grid1) == typeid(isect_grid)
			&& typeid(*grid2) != typeid(isect_grid))
		{
			std::cout << "normal speed" << std::endl;
			speed = SPEED_STEP;
		}
	}
	void setup_char(grid * g1)
	{
		start = g1;
		startper = 1;
		grid1 = g1;
		if (g1)
			grid2 = grid1->get_next(angle, grid1);
		else
			grid2 = NULL;
		grid1per = 1;
		grid2per = 0;
		dist = 1;
		speed = SPEED_STEP;
		change_speed();
	}
	void init ()
	{
		setup_char(current_stage->get_start());
	}
	void draw()
	{
		current_stage->draw(angle);
	}
	static void next_grid()
	{
		if(grid1->is_goal (angle))
		{
			grid1->toggle_goal(angle);
			num_goals++;
		}
		if(grid1 == hole_grid)
			kill_char ();
		else if(grid2)
		{
			grid *temp = grid2;
			grid2 = grid2->get_next(angle, grid1);
			if (grid2 == hole_grid)
				kill_char();
			grid1 = temp;
			change_speed();
		}
		else
			grid2 = NULL;
		grid1per = 1;
		grid2per = 0;
	}
	void kill_char()
	{
		startper = -0.05;
	}
	void toggle_pause()
	{
		paused = !paused;
	}
	vector3f *step_char()	//CHANGE FOR NORMALS
	{
		if(startper > 0)
		{
			vector3f pos1 = start->get_info(angle)->pos;
			startper -= 0.05;
			if (startper < 0)
				startper = 0;
			return(new vector3f(pos1.x, pos1.y + STARTY * startper, pos1.z));
		}
		else if(startper < 0)
		{
			vector3f pos1 = grid1->get_info(angle)->pos;
			startper -= 0.05;
			if(startper < -1)
				setup_char(start);
			return(new vector3f(pos1.x, pos1.y + STARTY * startper, pos1.z));
		}
		else if(grid1)
		{
			if (grid2 == hole_grid)
				kill_char();
			else
			{
				if(grid1->is_goal(angle))
				{
					grid1->toggle_goal(angle);
					num_goals++;
				}
				if(grid1 && grid2 && !paused)	//if both grids are there
				{
					grid1per -= speed / dist;	//step thru it
					grid2per += speed / dist;
					if (grid1per <= 0)	//if we've reached the end of this cycle
					{
						next_grid();	//on to the next cycle
					}
				}
				if(grid1 && !grid2)	//if there isn't a second grid...
				{
					grid2 = grid1->get_next(angle, grid1);	//try to get one
					change_speed();
					if(!grid2)	//if there still isn't a second grid...
						return(&(grid1->get_info(angle)->pos));	//return grid1's position
				}
				grid_info_t *i1 = grid1->get_info(angle);
				if(i1)
				{
					vector3f pos1 = i1->pos;
					grid_info_t *i2 = grid2->get_info(angle);
					if(i2)
					{
						vector3f pos2 = i2->pos;
						dist = pos1.dist(pos2);
						/*
						ret->dump();
						std::cout <<std::endl;
						// */
						return(new vector3f(pos1.x * grid1per + pos2.x * grid2per,
									pos1.y * grid1per + pos2.y * grid2per,
									pos1.z * grid1per + pos2.z * grid2per));
					}
					return(&(i1->pos));	//there isn't a second grid?  but wait, wouldn't this be return already?
				}
			}
		}
		return(NULL);		//i dunno why this would happen, but sure...
	}
};
