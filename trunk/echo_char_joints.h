// echo_char_joints.h

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

#include "echo_debug.h"

#ifndef __ECHO_CHAR_JOINTS__
#define __ECHO_CHAR_JOINTS__
/// Number of values in echo_char_joints...Just count them
#define NUM_VALUES	30

/** @brief Joint values of a mannequin
 * 
 * This union represents the joints and possible ways of bending them (at least, based on my body)\n
 * This is setup so that there are two ways to access a value:\n
 * 1) joints->[joint/body part]_[axis of rotation] (front is negative z, up is positive y, right is negative x)\n
 * (positive is clockwise)\n
 * 2) joints->[joint/body part (may be different)]_[action]\n
 * For example: lhand_x is the same as lwrist\n
 **/
union echo_char_joints
{
	/// An array of the values, so I can iteratively clear the joint values
	float value[NUM_VALUES];
	struct
	{
		float body_x;	/// Body turning forward or backwards
		float body_y;	/// Body turning left or right
		float body_z;	/// Body doing cartwheeels
		
		float head_x; 	/// nodding
		float head_y;	/// turning head
		float head_z;	/// tilting head
		float waist_y;	/// turning waist
		float waist_x;	/// bowing
		
		float lthigh_x;	/// lifting leg
		float lthigh_y;	/// twisting leg (roll)
		float lthigh_z;	/// opening legs
		
		float lleg_x;	/// bending leg
		float lfoot_x;	/// flexing foot
		
		float rthigh_x;	/// lifting leg
		float rthigh_y;	/// twisting leg (roll)
		float rthigh_z;	/// opening legs
		
		float rleg_x;	/// bending leg
		float rfoot_x;	/// flexing foot
		
		float lshoulder_z;	/// flapping arms
		float lshoulder_y; 	/// pushing out
		float lshoulder_x;	/// swinging arms
		
		float larm_x; 		/// bending elbow
		float larm_y; 		/// twisting arm
		float lhand_x;		/// wrist
		
		float rshoulder_z;	/// flapping arms
		float rshoulder_y; 	/// pushing out
		float rshoulder_x;	/// swinging arms
		
		float rarm_x; 		/// bending elbow
		float rarm_y; 		/// twisting arm
		float rhand_x;		/// wrist
	};
	struct
	{
		float body_pitch;	/// Body turning forward or backwards
		float body_turn;	/// Body turning left or right
		float body_tilt;	/// Body doing cartwheeels
		
		float head_nod; 	/// nodding
		float head_turn;	/// turning head
		float head_tilt;	/// tilting head
		float waist_turn;	/// turning waist
		float waist_bow;	/// bowing
		
		float lthigh_lift;	/// lifting leg
		float lthigh_twist;	/// twisting leg (roll)
		float lthigh_open;	/// opening legs
		
		float lleg_bend;	/// bending leg
		float lfoot_bend;	/// flexing foot
		
		float rthigh_lift;	/// lifting leg
		float rthigh_twist;	/// twisting leg (roll)
		float rthigh_open;	/// opening legs
		
		float rleg_bend;	/// bending leg
		float rfoot_bend;	/// flexing foot
		
		float lshoulder_flap;	/// flapping arms
		float lshoulder_push; 	/// pushing out
		float lshoulder_swing;	/// swinging arms
		
		float larm_bend; 		/// bending elbow
		float larm_twist; 		/// twisting arm
		float lwrist;		/// wrist
		
		float rshoulder_flap;	/// flapping arms
		float rshoulder_push; 	/// pushing out
		float rshoulder_swing;	/// swinging arms
		
		float rarm_bend; 		/// bending elbow
		float rarm_twist; 		/// twisting arm
		float rwrist;		/// wrist
	};
};

/** Set all joints to zero
 * @param joints Joints to set to zero
 */
int reset_joints(echo_char_joints* joints);
#endif
