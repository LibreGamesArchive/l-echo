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

#include <set>

#include <echo_character.h>
#include <echo_math.h>
#include <grid.h>
#include <echo_stage.h>

#ifndef __ECHO_NS__
#define __ECHO_NS__

/// Holds important stuff
namespace echo_ns
{
	/// The world's rotation angle.  _VERY_ important variable
	extern vector3f angle;
	/// The current stage
	extern stage* current_stage;
	/// The main character; the protagonist, the one the player controls
	extern echo_char* main_char;
	
	/// Deallocate everything: stage and character
	void deallocate();
	/// Initialize everything with the stage (which will be delete if deallocate is called)
	void init(stage* st);
	/// Get the ball rolling!
	void start();
	/// Pause or unpause the game
	void toggle_pause();
	/// Is the game paused?
	int is_paused();
	void setup_char(grid* g1);
	/// Draws the stage and the character, or a "stand-in" mannequin
	void draw();
	/// How many goals are there on this stage?
	int num_goals();
	/// How goals has the many character reached?
	int num_goals_reached();
	/// How many goals are left?
	int goals_left();
	/// Get the speed of the character (see echo_char#speed)
	float get_speed();
	/// Get the lowest level in the current stage
	float get_lowest_level();
	/// Change speed to running if we can
	void start_run();
	/// Change speed to walking if we can
	void start_step();
	/// Toggle running
	void toggle_run();
};
#endif

