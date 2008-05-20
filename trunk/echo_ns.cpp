// echo_ns.h

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

#define STARTY	30
#define CHAR_STEP 0.1f

namespace echo_ns
{
	grid* hole_grid = new grid();
	vector3f angle;
	grid* start;
	grid* grid1;
	grid* grid2;
	float grid1per, grid2per, startper;
	int paused = 1;
	int num_goals = 0;

	void setup_char(grid* g1)
	{
		//std::cout << "setupchar" << std::endl;
		start = g1;
		startper = 1;
		grid1 = g1;
		if(g1)  grid2 = grid1->get_next(angle, grid1);
		else    grid2 = NULL;
		grid1per = 1;
		grid2per = 0;
	}
	static void next_grid()
	{
	    if(grid1->is_goal(angle))
	    {
	        grid1->toggle_goal(angle);
	        num_goals++;
	    }
		if(grid1 == hole_grid)
			kill_char();
		else if(grid2)
		{
			//std::cout << "hole_char" << std::endl;
			grid* temp = grid2;
			grid2 = grid2->get_next(angle, grid1);
			if(grid2 == hole_grid)
				kill_char();
			grid1 = temp;
		}
		else
			grid2 = NULL;
		grid1per = 1;
		grid2per = 0;
	}
	void kill_char()
	{
		//std::cout << "killchar" << std::endl;
		startper = -0.05;
	}
	void toggle_pause()
	{
	    paused = !paused;
	}
	vector3f* step_char() //CHANGE FOR NORMALS
	{
		if(startper > 0)
		{
			vector3f pos1 = start->get_info(angle)->pos;
			startper -= 0.05;
			if(startper < 0)
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
		else
		{
			/*
			std::cout << "startper == 0: " << grid1per << "," << grid2per << std::endl;
			grid1->dump();
			std::cout << std::endl;
			grid2->dump();
			std::cout << std::endl;
			std::cout << grid2 << ", " << hole_grid << std::endl;
			// */
			if(grid2 == hole_grid)
				kill_char();
			else
			{
				if(grid1->is_goal(angle))
				{
					grid1->toggle_goal(angle);
					num_goals++;
				}
				if(grid1 && grid2 && !paused)  //if both grids are there
				{
					grid1per -= CHAR_STEP;  //step thru it
					grid2per += CHAR_STEP;
					if(grid1per <= 0)       //if we've reached the end of this cycle
					{
						next_grid();        //on to the next cycle
					}
				}
				if(grid1 && !grid2) //if there isn't a second grid...
				{
					grid2 = grid1->get_next(angle, grid1);      //try to get one
					if(!grid2)      //if there still isn't a second grid...
						return(&(grid1->get_info(angle)->pos)); //return grid1's position
				}
				grid_info_t* i1 = grid1->get_info(angle);
				if(i1)
				{
					vector3f pos1 = i1->pos;
					grid_info_t* i2 = grid2->get_info(angle);
					if(i2)
					{
						vector3f pos2 = i2->pos;
						/*
						ret->dump();
						std::cout <<std::endl;
						// */
						return(new vector3f(pos1.x * grid1per + pos2.x * grid2per
							, pos1.y * grid1per + pos2.y * grid2per
							, pos1.z * grid1per + pos2.z * grid2per));
					}
					return(&(i1->pos));  //there isn't a second grid?  but wait, wouldn't this be return already?
				}
			}
			return(NULL);       //i dunno why this would happen, but sure...
		}
	}
};

