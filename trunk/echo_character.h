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
#include <echo_char_joints.h>

#ifndef __ECHO_CHARACTER__
#define __ECHO_CHARACTER__
/** @brief echo_char represent an active mannequin (i.e., not a goal, or an "echo")
 * Usually the main character, echo_chars can also be antagonist characters
 * that sap a bit of the character's health if they collide.
 * 
 * @todo Character health
 * @todo Antagonist echo_char (does not toggle goals, drawn with gray hue)
 * 
 * echo_chars have two categories of modes: \n
 *
 * Grid Mode: When the character is walking and running between grids \n
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The position of the character is determined by the percentage weight \n
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; between grid1 and grid2. \n
 *
 * Falling Mode: When the character is falling or flying.  Describe in more detail \n
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; under fall_position. \n
 */
class echo_char
{
	protected:
		/// Where the character should be spawned
		grid* start;
		/// The first grid, the one this character is "departing" from
		grid* grid1;
		/// The second grid, the one this character is "arriving" to
		grid* grid2;
		/// Is this character paused?
		int paused;
		/// The number of goals ("echoes") reached by this character
		int num_goals;
		/// Is this character running (Grid Mode).
		int is_running;
		/*
			
			
			dist is the distance between the two grids.
			
			weight +=/-= (speed / dist) each step;
			
			
		*/
		///The percentage weight on the first grid (Grid Mode)
		float grid1per;
		/** The speed of the character.  Depends on the Mode this character is\n
		 * in (see class description).\n
		 * Various speeds are defined in echo_character.cpp\n
		 * Used mainly as a mode indicator, but also used in Grid Mode\n
		 * to increment grid1per and dist_traveled.\n
		 */
		float speed;
		
		/// Cached distance between grid1 and grid2 (Grid Mode)
		float dist;
		
		/// A cyclical measure of the distance travelled (actually multiplied by 2 to simplify animation); range: [0, 4)
		float dist_traveled;
		
		/// dist_traveled * 90; A convience variable; range: [0, 360)
		float dist_traveled_cyclic;
		
		/// The joint values
		echo_char_joints joints;
		
		/// The actual speed of the character; changed by acceleration (Falling Mode)
		float actual_speed;
		
		/** fall_position is used as:\n
		 * 
		 * -If falling from a hole or a launcher: the position if the camera angle is (0, 0, 0)\n
		 *      The character will be drawn at the fall_position rotated by\n
		 *      the new camera angle, then the velocity is integrated into\n
		 *      the absolute position, and finally the absolute position is\n
		 *      rotated back into the new fall_positon.\n
		 *      This is so the character is not affected by the rotation of\n
		 *      the world; he is in the air, after all.\n
		 * 
		 * -If falling from the sky: the absolute position of the character\n
		 *      The character needs to land on the target, so he has to\n
		 *      be attached to the world's rotation.\n
		 */
		vector3f* fall_position;
		
		/// Horizontal component of the character's flight (after launched by a launcher)
		vector3f* fly_direction;
		
		/// The y of the target grid (used if falling from the sky)
		float target_y;
	public:
		
		/** Initialize, and prepare to fall to that grid.
		 * @param start The initial grid on which to spawn
		 */
		echo_char(grid* start);
		/// Destructor
		virtual ~echo_char();
		/** Reinitializes the grid; spawns on g1
		 * @param g1 The new initial grid on which to spawn
		 */
		void init(grid* g1);
		/** Makes the character land on the grid
		 * @param g1 Where to land on
		 * @param do_change_speed Should this grid change the 
		 */
		void land(grid* g1, int do_change_speed);
		/// Respawns; same as init(start); 
		void reset();
		
		/// Take one step in animation and movement; call each frame
		void step();
		/// Forces the character to go the next grid (and trigger the goal there, if any)
		void next_grid();
		/// Changes the mode and speed of the character according to the grids it's at.
		void change_speed();
		
		//change speed to running if we can
		void start_run();
		//change speed to walking if we can
		void start_step();
		
		void toggle_run();
		
		vector3f* get_direction();
		
		//pause or unpause
		void toggle_pause();
		/** @return If the character is paused; should actually relegate to echo_ns.
		 */
		int is_paused();
		/** @return How many goals (echos, in echochrome-speak) the character has reached.
		 */
		int num_goals_reached();
		float get_speed();
		
		void draw(vector3f* vec);
		void draw(float x, float y, float z);
		
		/** Start falling from the given position, or where grid1 is.
		 * \param pos An arbitrary position to fall from.  If this is NULL, then grid1's position will be used
		 */
		void initialize_falling(vector3f* pos);
		/** Falling from the sky, at the start of the stage.
 */
		void initialize_fall_from_sky();
};
#endif

