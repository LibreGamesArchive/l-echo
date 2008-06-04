// echo_character.h

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
#include <echo_stage.h>

#ifndef __ECHO_CHARACTER__
#define __ECHO_CHARACTER__
class echo_char
{
	protected:
		//where the character was initialized.
		grid* start;
		//the first grid
		grid* grid1;
		//the second grid
		grid* grid2;
		//is this character paused?
		int paused;
		//the number of goals reached
		int num_goals;
		/*
			grid1per is the weight of the first grid
			grid2per is the weight of the second grid
			
			startper is the percent that the grid is spawning or is dying.
			
			speed is the raw speed
			dist is the distance between the two grids.
			
			weight +=/-= (speed / dist) each step;
		*/
		float grid1per, grid2per, startper, speed, dist;
	
	public:
		//default constructor
		echo_char();
		//init to that grid
		echo_char(grid* start);
		//if you used the default, or reinitializing...
		void init(grid* g1);
		//restart from the last starting point
		void reset();
		
		//take one step
		vector3f* step();
		//kill the character
		void kill();
		//go to the next grid
		void next_grid();
		//check the type of the grids, and change the speed of the character accordingly
		void change_speed();
		
		//pause or unpause
		void toggle_pause();
		//is this character paused?
		int is_paused();
		//number of goals/echoes reached
		int num_goals_reached();
};
#endif

