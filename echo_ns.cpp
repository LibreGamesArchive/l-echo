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
#include <set>

#include <echo_platform.h>
#include <echo_error.h>
#include <echo_character.h>
#include <echo_math.h>
#include <echo_ns.h>
#include <echo_gfx.h>

#include <grid.h>
#include <hole.h>

/// The minimum opacity of the "stand-in mannequin" 
#define NULL_CHAR_OPACITY_MIN   0.25f

/// Holds important stuff
namespace echo_ns
{
	/** Opacity of the "stand-in" mannequin, or the mannequin that marks
	 * where the character initially lands.
	 */
	float null_char_opacity = NULL_CHAR_OPACITY_MIN;
	/// Are we increasing the character's opacity or decreasing?
	int opacity_incr = true;
	/// The world's rotation angle.  _VERY_ important variable
	vector3f angle;
	/// The main character; the protagonist, the one the player controls
	echo_char* main_char = NULL;
	/// The current stage
	stage* current_stage = NULL;
	/// Has the game started yet?
	int started = false;
	/// Deallocate everything: stage and character
	void deallocate()
	{
		if(current_stage != NULL)
			delete current_stage;
		if(main_char != NULL)
			delete main_char;
	}
	/// Initialize everything with the stage (which will be delete if deallocate is called)
	void init(stage* st)
	{
		if(current_stage != NULL)
			delete current_stage;
		current_stage = st;
		if(st != NULL)
		{
			main_char = new echo_char(st->get_start());
			CHKPTR(main_char);
		}
		else
			main_char = NULL;
	}
	/// Get the ball rolling!
	void start()
	{
		started = true;
	}
	/// Get the lowest level in the current stage
	float get_lowest_level()
	{
		return(current_stage->get_lowest_level());
	}
	/// Draws the stage and the character, or a "stand-in" mannequin
	void draw()
	{
		if(current_stage != NULL)
		{
			current_stage->draw(angle);
			if(started)
			{
				main_char->step();
			}
			/// Need a stand-in mannequin
			else
			{
				/// Get the starting grid's info
				grid* g = current_stage->get_start();
				if(g)
				{
					grid_info_t* info = g->get_info(echo_ns::angle);
					if(info)
					{
						gfx_push_matrix();
						gfx_translatef(info->pos->x, info->pos->y, info->pos->z);
#ifndef ECHO_NDS
						gfx_outline_start();
						draw_character(NULL);
						gfx_outline_mid();
#endif
						gfx_color3f(null_char_opacity, null_char_opacity, null_char_opacity);
						draw_character(NULL);
#ifndef ECHO_NDS
						gfx_outline_end();
#endif
						gfx_pop_matrix();
						
						/// Change the opacity
						/// If we're increasing the opacity
						if(opacity_incr)
						{
							/// Increase the opacity slightly
							null_char_opacity += 0.05f;
							/// If the opacity is greater than (or equal to) 1
							if(null_char_opacity >= 1)
							{
								/// Change it back to one
								null_char_opacity = 1;
								/// Start decreasing the opacity
								opacity_incr = false;
							}
						}
						/// Else, we're decreasing...
						else
						{
							/// Decrease the opacity slightly
							null_char_opacity -= 0.05f;
							/// If the opacity is less than the minimum
							if(null_char_opacity <= NULL_CHAR_OPACITY_MIN)
								/// Start increasing the opacity
								/// (don't need to change to NULL_CHAR_OPACITY_MIN because it's OK to cross the threshold)
								opacity_incr = true;
						}
					}
				}
			}
		}
	}
	/// Pause or unpause the game
	void toggle_pause()
	{
		if(main_char != NULL)
			main_char->toggle_pause();
	}
	/// Is the game paused?
	int is_paused()
	{
		return(main_char->is_paused());
	}
	/// How many goals are there on this stage?
	int num_goals()
	{
		return(current_stage->get_num_goals());
	}
	/// How goals has the many character reached?
	int num_goals_reached()
	{
		return(main_char->num_goals_reached());
	}
	/// How many goals are left?
	int goals_left()
	{
		return(current_stage->get_num_goals() - num_goals_reached());
	}
	/// Get the speed of the character (see echo_char#speed)
	float get_speed()
	{
		return(main_char->get_speed());
	}
	/// Change speed to running if we can
	void start_run()
	{
		main_char->start_run();
	}
	/// Change speed to walking if we can
	void start_step()
	{
		main_char->start_step();
	}
	/// Toggle running
	void toggle_run()
	{
		main_char->toggle_run();
	}
};
