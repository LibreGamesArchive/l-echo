// echo_gfx.cpp

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

#include <echo_platform.h>
#include <echo_math.h>
#include <echo_debug.h>
#include <echo_char_joints.h>
#include <echo_error.h>
#include <echo_gfx.h>
#include <echo_math.h>

#include <cstdlib>
#include <iostream>

#ifdef ECHO_NDS
	#include <nds.h>
#elif ECHO_OSX	//OS X
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include <grid.h>

#ifdef ECHO_NDS
	#define POP_MATRIX 		glPopMatrix(1)
	#define SET_ID(id)		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(id));
#else
	/// Are we currently drawing the outline?
	static int drawing_outline = 0;
	#define SET_ID(id)
	#define POP_MATRIX		glPopMatrix()
#endif

/** Draw a hole (colored black) the size of grid at pos.
 * It's a filled square that is formed from the midpoints of the edges of the grid.
 * @param pos Where to draw the hole
 */
void draw_hole(vector3f* pos)
{
	gfx_color3f(0, 0, 0);
	gfx_push_matrix();
	{
		/// Translate slightly above the grid so there is no z-fighting
		glTranslatef(pos->x, pos->y + 0.05f, pos->z);
		glBegin(GL_QUADS);
		{
			glVertex3f(0, 0, HALF_GRID);
			glVertex3f(HALF_GRID, 0, 0);
			glVertex3f(0, 0, -HALF_GRID);
			glVertex3f(-HALF_GRID, 0, 0);
		}
		glEnd();
	}
	POP_MATRIX;
}
/** Draw a launcher the size of grid at pos.
 * It's a non-filled square that is formed from the midpoints of the edges of the grid.
 * @param pos Where to draw the launcher
 */
void draw_launcher(vector3f* pos)
{
	gfx_color3f(0, 0, 0);
	gfx_push_matrix();
	{
		/// Translate slightly above the grid so there is no z-fighting
		glTranslatef(pos->x, pos->y + 0.05f, pos->z);
#ifdef ECHO_NDS
		/// A strip of thin quads
		glBegin(GL_QUAD_STRIP);
		{
			glVertex3f(0, 0, HALF_GRID);
			glVertex3f(0, 0, HALF_GRID - 0.05f);
			
			glVertex3f(HALF_GRID - 0.05f, 0, 0);
			glVertex3f(HALF_GRID, 0, 0);
			
			glVertex3f(0, 0, -HALF_GRID);
			glVertex3f(0, 0, -HALF_GRID + 0.05f);
			
			glVertex3f(-HALF_GRID, 0, 0);
			glVertex3f(-HALF_GRID + 0.05f, 0, 0);
			
			glVertex3f(0, 0, HALF_GRID);
			glVertex3f(0, 0, HALF_GRID - 0.05f);
		}
		glEnd();
#else
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(0, 0, HALF_GRID);
			glVertex3f(HALF_GRID, 0, 0);
			glVertex3f(0, 0, -HALF_GRID);
			glVertex3f(-HALF_GRID, 0, 0);
		}
		glEnd();
#endif
	}
	POP_MATRIX;
}
/** Draws a WHITE rectangle (actually, a quadrilateral) at the specified points
 * @param p1 The first point
 * @param p2 The second point
 * @param p3 The third point
 * @param p4 The fourth point
 */
void draw_rect(vector3f* p1, vector3f* p2, vector3f* p3, vector3f* p4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
	{
		glVertex3f(p1->x, p1->y, p1->z);
		glVertex3f(p2->x, p2->y, p2->z);
		glVertex3f(p3->x, p3->y, p3->z);
		glVertex3f(p4->x, p4->y, p4->z);
	}
	glEnd();
}
/// Draws a WHITE rectangle (actually, a quadrilateral) at the specified coordinates
void draw_rect(float x1, float y1, float z1
		, float x2, float y2, float z2
		, float x3, float y3, float z3
		, float x4, float y4, float z4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
	{
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);
		glVertex3f(x4, y4, z4);
	}
	glEnd();
}

#ifdef ECHO_NDS
/** Draws a WHITE rectangle (actually, a quadrilateral) at the specified coordinates
 * Coordinates are in v16 (use floattof32 and f32tofloat to convert to and from)
 */
void draw_rectv16(v16 x1, v16 y1, v16 z1
		, v16 x2, v16 y2, v16 z2
		, v16 x3, v16 y3, v16 z3
		, v16 x4, v16 y4, v16 z4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
	{
		glVertex3v16(x1, y1, z1);
		glVertex3v16(x2, y2, z2);
		glVertex3v16(x3, y3, z3);
		glVertex3v16(x4, y4, z4);
	}
	glEnd();
}
#endif

/** Draw a goal ("echo") at pos
 * @param pos Where to draw the goal
 */
void draw_goal_gfx(vector3f* pos)
{
	/// Don't draw the goal if the outline is currently being drawn
#ifndef ECHO_NDS
	if(!drawing_outline)
#endif
	{
		gfx_color3f(0.25f, 0.25f, 0.25f);
		gfx_push_matrix();
			glTranslatef(pos->x, pos->y, pos->z);
			draw_character(NULL);
		POP_MATRIX;
	}
}
/** Draw a character with joint values as specified
 * @param joints Joint values of the character
 */
void draw_character(echo_char_joints* joints)
{
	gfx_push_matrix();
	{
		if(joints != NULL)
		{
			/// Body
			glTranslatef(0, 1.875f, 0);
			SET_ID(1);
			draw_body();
			
			gfx_push_matrix();
			{
				/// Neck
				glTranslatef(0, 0.3f, 0);
				SET_ID(12);
				draw_sphere_pointzero75();
				
				/// Head
				glRotatef(joints->head_x, 1, 0, 0);
				glRotatef(joints->head_y, 0, 1, 0);
				glRotatef(joints->head_z, 0, 0, 1);
				glTranslatef(0, 0.3f, 0);
				SET_ID(0);
				draw_head();
			}
			POP_MATRIX;
			gfx_push_matrix();
			{
				/// Left Shoulder
				glTranslatef(0.2f, 0.2f, 0);
				SET_ID(13);
				draw_sphere_pointzero75();
				
				/// Upper Left Arm
				glRotatef(joints->lshoulder_x, 1, 0, 0);
				glRotatef(joints->lshoulder_y, 0, 1, 0);
				glRotatef(joints->lshoulder_z + 12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(2);
				draw_limb();
				
				/// Left Elbow
				glTranslatef(0, -0.45f, 0);
				SET_ID(15);
				draw_sphere_pointzero75();
				
				/// Left Arm
				glRotatef(joints->larm_x, 1, 0, 0);
				glRotatef(joints->larm_y, 0, 1, 0);
				glRotatef(-12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(4);
				draw_limb();
				
				/// Left Wrist
				glTranslatef(0, -0.45f, 0);
				SET_ID(14);
				draw_sphere_pointzero75();
				
				/// Left Hand
				glRotatef(joints->lhand_x, 1, 0, 0);
				glTranslatef(-0.03f, -0.06f, 0);
				SET_ID(6);
				draw_left_hand();
			}
			POP_MATRIX;
			gfx_push_matrix();
			{
				/// Right Shoulder
				glTranslatef(-0.2f, 0.2f, 0);
				SET_ID(13);
				draw_sphere_pointzero75();
				
				/// Upper Right Arm
				glRotatef(joints->rshoulder_x, 1, 0, 0);
				glRotatef(joints->rshoulder_y, 0, 1, 0);
				glRotatef(joints->rshoulder_z - 12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(3);
				draw_limb();
				
				/// Right Elbow
				glTranslatef(0, -0.45f, 0);
				SET_ID(15);
				draw_sphere_pointzero75();
				
				/// Right Arm
				glRotatef(joints->rarm_x, 1, 0, 0);
				glRotatef(joints->rarm_y, 0, 1, 0);
				glRotatef(12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(5);
				draw_limb();
				
				/// Right Wrist
				glTranslatef(0, -0.45f, 0);
				SET_ID(14);
				draw_sphere_pointzero75();
				
				/// Right Hand
				glRotatef(joints->rhand_x, 1, 0, 0);
				glTranslatef(0.03f, -0.06f, 0);
				SET_ID(6);
				draw_right_hand();
			}
			POP_MATRIX;
			gfx_push_matrix();
			{
				/// Hip
				SET_ID(13);
				glTranslatef(0, -0.32f, 0);
				draw_sphere_point1();
				
				/// Lower Body
				glRotatef(joints->waist_y, 0, 1, 0);
				glRotatef(joints->waist_z, 0, 0, 1);
				glTranslatef(0, -0.32f, 0);
				SET_ID(1);
				draw_lower_body();
				
				gfx_push_matrix();
				{
					/// Left...um...Buttocks
					glTranslatef(0.1f, -0.06f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					
					/// Left Thigh
					glRotatef(joints->lthigh_x, 1, 0, 0);
					glRotatef(joints->lthigh_y, 0, 1, 0);
					glRotatef(joints->lthigh_z, 0, 0, 1);
					glTranslatef(0, -0.05f, 0);
					SET_ID(7);
					draw_limb();
					
					/// Left Knee
					glTranslatef(0, -0.45f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					
					/// Left Leg
					glRotatef(joints->lleg_x, 1, 0, 0);
					glTranslatef(0, -0.05f, 0);
					SET_ID(9);
					draw_limb();
					
					/// Left Ankle
					glTranslatef(0, -0.45f, 0);
					SET_ID(13);
					draw_sphere_pointzero75();
					
					/// Left Foot
					glRotatef(joints->lfoot_x, 1, 0, 0);
					glTranslatef(0.01f, -0.15f, 0.025f);
					SET_ID(11);
					draw_foot();
				}
				POP_MATRIX;
				gfx_push_matrix();
				{
					/// Right Buttocks
					glTranslatef(-0.1f, -0.06f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					
					/// Right Thigh
					glRotatef(joints->rthigh_x, 1, 0, 0);
					glRotatef(joints->rthigh_y, 0, 1, 0);
					glRotatef(joints->rthigh_z, 0, 0, 1);
					glTranslatef(0, -0.05f, 0);
					SET_ID(8);
					draw_limb();
					
					/// Right Knee
					glTranslatef(0, -0.45f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					
					/// Right Leg
					glRotatef(joints->rleg_x, 1, 0, 0);
					glTranslatef(0, -0.05f, 0);
					SET_ID(10);
					draw_limb();
					
					/// Right Ankle
					glTranslatef(0, -0.45f, 0);
					SET_ID(13);
					draw_sphere_pointzero75();
					
					/// Right Foot
					glRotatef(joints->rfoot_x, 1, 0, 0);
					glTranslatef(-0.01f, -0.15f, 0.025f);
					SET_ID(11);
					draw_foot();
				}
				POP_MATRIX;
			}
			POP_MATRIX;
		}
		/// If the given joints was NULL...
		else
		{
			glTranslatef(0, 1.875f, 0);
			SET_ID(1);
			draw_body();
			gfx_push_matrix();	//neck, head
			{
				glTranslatef(0, 0.3f, 0);
				SET_ID(12);
				draw_sphere_pointzero75();
				glTranslatef(0, 0.3f, 0);
				SET_ID(0);
				draw_head();
			}
			POP_MATRIX; //end neck, hand
			gfx_push_matrix();	//left hand
			{
				glTranslatef(0.2f, 0.2f, 0);
				SET_ID(13);
				draw_sphere_pointzero75();
				glRotatef(12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(2);
				draw_limb();
				glTranslatef(0, -0.45f, 0);
				SET_ID(15);
				draw_sphere_pointzero75();
				glRotatef(-12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(4);
				draw_limb();
				glTranslatef(0, -0.45f, 0);
				SET_ID(14);
				draw_sphere_pointzero75();
				glTranslatef(-0.03f, -0.06f, 0);
				SET_ID(6);
				draw_left_hand();
			}
			POP_MATRIX;	//end left hand
			gfx_push_matrix();	//right hand
			{
				glTranslatef(-0.2f, 0.2f, 0);
				SET_ID(13);
				draw_sphere_pointzero75();
					glRotatef(-12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(3);
				draw_limb();
				glTranslatef(0, -0.45f, 0);
				SET_ID(15);
				draw_sphere_pointzero75();
				glRotatef(12, 0, 0, 1);
				glTranslatef(0, -0.05f, 0);
				SET_ID(5);
				draw_limb();
				glTranslatef(0, -0.45f, 0);
				SET_ID(14);
				draw_sphere_pointzero75();
				glTranslatef(0.03f, -0.06f, 0);
				SET_ID(6);
				draw_right_hand();
			}
			POP_MATRIX;	//end right hand
			gfx_push_matrix();	//waist, lower body
			{
				SET_ID(13);
				glTranslatef(0, -0.32f, 0);
				draw_sphere_point1();
				glTranslatef(0, -0.32f, 0);
				SET_ID(1);
				draw_lower_body();
				gfx_push_matrix();	//left leg
				{
					glTranslatef(0.1f, -0.06f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					glTranslatef(0, -0.05f, 0);
					SET_ID(7);
					draw_limb();
					glTranslatef(0, -0.45f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					glTranslatef(0, -0.05f, 0);
					SET_ID(9);
					draw_limb();
					glTranslatef(0, -0.45f, 0);
					SET_ID(13);
					draw_sphere_pointzero75();
					glTranslatef(0.01f, -0.15f, 0.025f);
					SET_ID(11);
					draw_foot();
				}
				POP_MATRIX;	//end left leg
				gfx_push_matrix();	//right leg
				{
					glTranslatef(-0.1f, -0.06f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					glTranslatef(0, -0.05f, 0);
					SET_ID(8);
					draw_limb();
					glTranslatef(0, -0.45f, 0);
					SET_ID(12);
					draw_sphere_pointzero75();
					glTranslatef(0, -0.05f, 0);
					SET_ID(10);
					draw_limb();
					glTranslatef(0, -0.45f, 0);
					SET_ID(13);
					draw_sphere_pointzero75();
					glTranslatef(-0.01f, -0.15f, 0.025f);
					SET_ID(11);
					draw_foot();
				}
				POP_MATRIX;	//end right leg
			}
			POP_MATRIX; //end lower body
		}
	}
	POP_MATRIX;	//end body
}

/** Rotates the current matrix by angle "angle" at vector <x, y, z> (follows the right-hand rule)
 * @param angle The angle to rotate by (DEGREES!)
 * @param x The X-Coordinate of the vector to rotate around
 * @param y The Y-Coordinate of the vector to rotate around
 * @param z The Z-Coordinate of the vector to rotate around
 */
void gfx_rotatef(float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}
/** Translates the current matrix by vector <x, y, z>
 * @param x The X-Coordinate of the vector to translate to
 * @param y The Y-Coordinate of the vector to translate to
 * @param z The Z-Coordinate of the vector to translate to
 */
void gfx_translatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}
/// Pushes a matrix
void gfx_push_matrix()
{
	glPushMatrix();
}
/// Pops a matrix
void gfx_pop_matrix()
{
	POP_MATRIX;
}
/// Sets the current matrix to the identity matrix
void gfx_identity()
{
	glLoadIdentity();
}
/** Set the color
 * @param r The intensity of red from 0 to 1.0f, 1.0f being the brightest
 * @param g The intensity of green from 0 to 1.0f, 1.0f being the brightest
 * @param b The intensity of blue from 0 to 1.0f, 1.0f being the brightest
 */
void gfx_color3f(float r, float g, float b)
{
#ifndef	ECHO_NDS
	if(!drawing_outline)
#endif
		glColor3f(r, g, b);
}

#ifndef ECHO_NDS
	/// Adapted from http://www.codeproject.com/KB/openGL/Outline_Mode.aspx
	void gfx_outline_start()
	{
		drawing_outline = 1;
		//Method 1
		// Push the GL attribute bits so that we don't wreck any settings
		
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( -2.5f, -2.5f );
		// Set the render mode to be line rendering with a thick line width
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glLineWidth( 4.0f );
		// Set the colour to be white
		
		glColor3f( 0.0f, 0.0f, 0.0f );
	}
	void gfx_outline_mid()
	{
		drawing_outline = 0;
		//Method 1
		// Set the polygon mode to be filled triangles 
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		// Set the colour to the background
		
		glColor3f( 1.0f, 1.0f, 1.0f );
	}
	void gfx_outline_end()
	{
		// Pop the state changes off the attribute stack
		// to set things back how they were
		
		glPopAttrib();
	}
#else
	void gfx_set_polyID(unsigned int polyID)
	{
		SET_ID(polyID);
	}
#endif

