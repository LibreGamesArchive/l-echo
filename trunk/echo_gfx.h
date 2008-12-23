// echo_gfx.h

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

/// Various L-Echo libraries
#include <echo_char_joints.h>
#include <echo_math.h>
#include <echo_platform.h>

/// Need NDS library for the definition of v16
#ifdef ECHO_NDS
	#include <nds.h>
#endif

/** Draws a WHITE rectangle (actually, a quadrilateral) at the specified points
 * @param p1 The first point
 * @param p2 The second point
 * @param p3 The third point
 * @param p4 The fourth point
 */
void draw_rect(vector3f* p1, vector3f* p2, vector3f* p3, vector3f* p4);
/// Draws a WHITE rectangle (actually, a quadrilateral) at the specified coordinates
void draw_rect(float x1, float y1, float z1
		, float x2, float y2, float z2
		, float x3, float y3, float z3
		, float x4, float y4, float z4);
#ifdef ECHO_NDS
/** Draws a WHITE rectangle (actually, a quadrilateral) at the specified coordinates
 * Coordinates are in v16 (use floattof32 and f32tofloat to convert to and from)
 */
void draw_rectv16(v16 x1, v16 y1, v16 z1
		, v16 x2, v16 y2, v16 z2
		, v16 x3, v16 y3, v16 z3
		, v16 x4, v16 y4, v16 z4);
#endif

/** Draw a hole (colored black) the size of grid at pos
 * It's a filled square that is formed from the midpoints of the edges of the grid.
 * @param pos Where to draw the hole
 */
void draw_hole(vector3f* pos);
/** Draw a launcher the size of grid at pos
 * It's a non-filled square that is formed from the midpoints of the edges of the grid.
 * @param pos Where to draw the launcher
 */
void draw_launcher(vector3f* pos);
/** Draw a goal ("echo") at pos
 * @param pos Where to draw the goal
 */
void draw_goal_gfx(vector3f* pos);
/** Draw a character with joint values as specified
 * @param joints Joint values of the character
 */
void draw_character(echo_char_joints* joints);

/// These functions are NOT defined in echo_gfx.cpp, but generated
/// Draw the head of the character (does not translate)
void draw_head();
/// Draw the body of the character (does not translate)
void draw_body();
/// Draw the lower body of the character (does not translate)
void draw_lower_body();
/// Draw a limb (arm, leg, thigh) of the character (does not translate)
void draw_limb();
/// Draw a set stairs (does not translate)
void draw_stairs();
/// Draw the left hand of the character (does not translate)
void draw_left_hand();
/// Draw the right hand of the character (does not translate)
void draw_right_hand();
/// Draw a foot (does not translate)
void draw_foot();
/// Draw a sphere with radius 0.1f (Used for the hip) (does not translate)
void draw_sphere_point1();
/// Draw a sphere with radius 0.075f (Used for joints) (does not translate)
void draw_sphere_pointzero75();

/** These functions encapsulate the functionality of their GL equivalents;
 * used by other files that do not include GL
 */
/// Sets the current matrix to the identity matrix
void gfx_identity();
/// Pushes a matrix
void gfx_push_matrix();
/// Pops a matrix
void gfx_pop_matrix();
/** Rotates the current matrix by angle "angle" at vector <x, y, z> (follows the right-hand rule)
 * @param angle The angle to rotate by (DEGREES!)
 * @param x The X-Coordinate of the vector to rotate around
 * @param y The Y-Coordinate of the vector to rotate around
 * @param z The Z-Coordinate of the vector to rotate around
 */
void gfx_rotatef(float angle, float x, float y, float z);
/** Translates the current matrix by vector <x, y, z>
 * @param x The X-Coordinate of the vector to translate to
 * @param y The Y-Coordinate of the vector to translate to
 * @param z The Z-Coordinate of the vector to translate to
 */
void gfx_translatef(float x, float y, float z);
/** Set the color
 * @param r The intensity of red from 0 to 1.0f, 1.0f being the brightest
 * @param g The intensity of green from 0 to 1.0f, 1.0f being the brightest
 * @param b The intensity of blue from 0 to 1.0f, 1.0f being the brightest
 */
void gfx_color3f(float r, float g, float b);

#ifndef ECHO_NDS
	/// Call before 1st render
	void gfx_outline_start();
	/// Call between the 1st and 2nd renders
	void gfx_outline_mid();
	/// Call after the second render
	void gfx_outline_end();
#else
	/** Sets the polygon ID of the following polygons to polyID.
	 * This is used to force polygons to have different outlines.
	 * As there is a limited number of polyIDs, they are split up like so:
	 * 0 - 15: The character's various parts
	 * 16 - 18: Stairs
	 * 19 - 45: Grids
	 * PolyIDs of grids are split up among freeform_grids and non-freeform_grids,
	 * and they in turn are split up among each level.
	 * @param polyID The polyID from 0 to 63.
	 */
	void gfx_set_polyID(unsigned int polyID);
#endif

