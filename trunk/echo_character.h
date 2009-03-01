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

#include "echo_math.h"
#include "grid.h"
#include "echo_stage.h"
#include "echo_char_joints.h"

#ifndef __ECHO_CHARACTER_SPEED__
	#define __ECHO_CHARACTER_SPEED__
	/** Speeds/modes of the character.  STEP and RUN are percentage speeds (%/sec),
	 * and FALL, FALL_FROM_SKY, LAUNCH are "real" speeds (Units/sec)
	 */
	enum CHARACTER_SPEED{ STEP = 0, RUN = 1, FALL = 2, FALL_FROM_SKY = 3, LAUNCH = 4, LANDING = 5, STANDING_UP = 6 };
#endif

#ifndef __ECHO_CHARACTER__
#define __ECHO_CHARACTER__
/** @brief echo_char represent an active mannequin (i.e., not a goal, or an "echo")\n
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
		///The percentage weight on the first grid (Grid Mode)
		float grid1per;
		/** Defines which mode the character is in
		 */
		enum CHARACTER_SPEED mode;
		
		/// Cached distance between grid1 and grid2 (Grid Mode)
		float dist;
		
		/// A cyclical measure of the distance travelled (actually multiplied by 2 to simplify animation); range: [0, 4)
		float dist_traveled;
		
		/// dist_traveled * 90; A convience variable; range: [0, 360)
		float dist_traveled_cyclic;
		
		/// The joint values
		echo_char_joints joints;
		
		/// The actual speed of the character; changed by acceleration (Falling Mode)
		float speed;
		
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
		
		/** Save the direction of the character before changing grids, so
		 * that if the new grid1 is a launcher it can access the last direction.
		 */
		vector3f* fly_direction;
		
		/// Lateral component of the character's flight (after launched by a launcher)
		float x_speed;
		/// Lateral component of the character's flight (after launched by a launcher)
		float z_speed;
		
		/// The y of the target grid (used if falling from the sky)
		float target_y;
	public:
		
		/** Initialize, and prepare to fall to that grid.
		 * @param g1 The initial grid on which to spawn
		 */
		echo_char(grid* g1);
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
		
		/// If the character is walking, start running
		void start_run();
		/// If the character is running, start walking
		void start_step();
		
		/// Start running if walking, or start walking if running
		void toggle_run();
		
		/** Get the current direction of the character.
		 * @return The current direction of the character, or grid2's position - grid1's
		 */
		vector3f* get_direction();
		
		/// Pause if running, or unpause if paused
		void toggle_pause();
		/** Returns if the character is paused or not 
		 * @return If the character is paused or not
		 */
		int is_paused();
		/** Returns how many echos the character has reached. 
		 * @return How many goals (echos, in echochrome-speak) the character has reached.
		 */
		int num_goals_reached();
		/** Returns the "speed" variable (see speed attribute)
		 * @return "speed" (see speed attribute)
		 */
		float get_speed();
	
	protected:
		/** Checks if the grid given is a goal, and if it is, the character
		 * will toggle the goal, set it as its new spawn spot, and add to the goal count
		 * @param g Grid to check
		 */
		void check_goal(grid* g);
		/** Draws the character at (x,y,z)
		 * @param x X-coordinate of the character
		 * @param y Y-coordinate of the character
		 * @param z Z-coordinate of the character
		 */
		void draw(float x, float y, float z);
		
		/** Start falling from the given position, or where grid1 is.
		 * @param pos An arbitrary position to fall from.  If this is NULL, then grid1's position will be used
		 */
		void initialize_falling(vector3f* pos);
		/** Falling from the sky, at the start of the stage.
		*/
		void initialize_fall_from_sky();
		/** Start launching from the given position and direction, or where grid1 and grid2 are.
		 * The launching works like this: If the character is launched from and lands on the same 
		 * level, then he'll go up 7 units and right 4 units (tested in the real game).
		 * So we can derive some constants from that...
		 * 
		 * When y = 7, v_fy = 0:
		 * {v_oy}^2 + 2{a_y}x = {v_fy}^2
		 * {v_oy}^2 + 14{a_y} = 0
		 * {v_oy} = sqrt(-14{a_y})  (a is already negative)
		 * 
		 * When y = 0 (when he launches and lands):
		 * y = {v_oy}t + a{t^2} / 2
		 * 0 = t * sqrt(-14{a_y}) + a{t^2} / 2
		 * 
		 * Since a_y = -sqrt(-a_y) * sqrt(-a_y):
		 * 0 = t * sqrt(-a_y) * ( sqrt(14) - t * sqrt(-a_y) / 2 )
		 * 
		 * So: 
		 * t * sqrt(-a_y) / 2 = sqrt(14)
		 * t = 2 * sqrt(14) / sqrt(-a_y)   (when he lands)
		 * 
		 * Plug it into the x equation:
		 * x = v_ox * t
		 * 4 = v_ox * 2 * sqrt(14) / sqrt(-a_y)
		 * 2 * sqrt(-a_y) / sqrt(14) = v_ox
		 * sqrt(-14{a_y}) / 7 = v_ox
		 * v_oy / 7 = v_ox
		 * 
		 * @param pos An arbitrary position to launch from.  If this is NULL, then grid1's position will be used
		 * @param direction Direction to launch towards laterally (on the xz-plane).  If this is NULL, then get_direction will be used.  WILL BE DELETED!!!
		 */
		void initialize_launching(vector3f* pos, vector3f* direction);
		/** Initialize the landing sequence; doesn't actually call
		 * land.  The step method will call land after the landing and 
		 * standing up sequences have been executed.
		 */
		void initialize_landing();
		/// Calculate joint values for a character in the air (Falling Mode)
		void falling_mode_joints();
		/// Joint calculation for a character just landing
		void landing_mode_joints();
		/// Joint calculation for a character standing up right after a landing
		void standing_up_joints();
		/// Step through joint calculations for walking (used in Grid Mode)
		void grid_mode_joints(float y);
		/// Initializes the joints for falling mode
		void initialize_falling_mode();
};
#endif

