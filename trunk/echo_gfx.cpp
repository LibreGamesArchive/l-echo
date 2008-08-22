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

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <grid.h>
#include <echo_gfx.h>
#include <echo_math.h>
#include <cstdlib>
#include <iostream>

#ifdef ARM9
	#define POP_MATRIX glPopMatrix(1)
#else
	#define POP_MATRIX glPopMatrix()
#endif

#ifndef ARM9
	static int drawing_outline = 0;
#endif


void draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
{
	gfx_color3f(0, 0, 0);
#ifdef ARM9
	glBegin(GL_TRIANGLES);
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glVertex3f(x2, y2, z2);
        glEnd();
#else
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
#endif
}

void draw_line(vector3f p1, vector3f p2)
{
	gfx_color3f(0, 0, 0);
#ifdef ARM9
	glBegin(GL_TRIANGLES);
                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
                glVertex3f(p2.x, p2.y, p2.z);
        glEnd();
#else
	glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
#endif
}

void draw_hole(vector3f pos)
{
	gfx_color3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.03, pos.z);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, HALF_GRID);
		glVertex3f(HALF_GRID, 0, 0);
		glVertex3f(0, 0, -HALF_GRID);
		glVertex3f(-HALF_GRID, 0, 0);
	glEnd();
	POP_MATRIX;
}

void draw_launcher(vector3f pos)
{
	gfx_color3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.03, pos.z);
#ifdef ARM9
	glBegin(GL_TRIANGLE_STRIP);
                        glVertex3f(0, 0, HALF_GRID);
                        glVertex3f(HALF_GRID, 0, 0);
                        glVertex3f(0, 0, -HALF_GRID);
                        glVertex3f(-HALF_GRID, 0, 0);
	glEnd();
#else
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, HALF_GRID);
		glVertex3f(HALF_GRID, 0, 0);
		glVertex3f(0, 0, -HALF_GRID);
		glVertex3f(-HALF_GRID, 0, 0);
	glEnd();
#endif
	POP_MATRIX;
}

void draw_rect(vector3f p1, vector3f p2, vector3f p3, vector3f p4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void draw_rect(float x1, float y1, float z1
		, float x2, float y2, float z2
		, float x3, float y3, float z3
		, float x4, float y4, float z4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);
		glVertex3f(x4, y4, z4);
	glEnd();
}

#ifdef ARM9
void draw_rectv16(v16 x1, v16 y1, v16 z1
		, v16 x2, v16 y2, v16 z2
		, v16 x3, v16 y3, v16 z3
		, v16 x4, v16 y4, v16 z4)
{
	gfx_color3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3v16(x1, y1, z1);
		glVertex3v16(x2, y2, z2);
		glVertex3v16(x3, y3, z3);
		glVertex3v16(x4, y4, z4);
	glEnd();
}
#endif

void draw_goal_gfx(vector3f pos, float goal_angle)
{
#ifndef ARM9
	if(!drawing_outline)
#endif
	{
		gfx_color3f(0.25f, 0.25f, 0.25f);
		glPushMatrix();
		glTranslatef(pos.x, pos.y + 0.5f, pos.z);
		glRotatef(goal_angle, 0, 1, 0);
#ifdef ARM9
		glBegin(GL_QUADS);
		glVertex3f(0, HALF_GRID, 0);
		glVertex3f(HALF_GRID, 0, 0);
		glVertex3f(0, -HALF_GRID, 0);
		glVertex3f(-HALF_GRID, 0, 0);
		glEnd();
#else
		glutSolidTorus(0.05f, 0.4f, 8, 8);
#endif
		POP_MATRIX;
	}
}

void draw_character()
{
	gfx_translatef(0, 2, 0);
	//draw_sphere_point1();
	draw_body();
	gfx_translatef(0, 1.1f, 0);
	draw_head();
}

void gfx_rotatef(float angle, float x, float y, float z)
{
#ifdef ECHO_NDS
	glRotatef(-angle, x, y, z);
#else
	glRotatef(angle, x, y, z);
#endif
}

void gfx_translatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void gfx_push_matrix()
{
	glPushMatrix();
}

void gfx_pop_matrix()
{
	POP_MATRIX;
}

void gfx_identity()
{
	glLoadIdentity();
}

void gfx_color3f(float r, float g, float b)
{
#ifndef	ARM9
	if(!drawing_outline)
#endif
		glColor3f(r, g, b);
}

//From http://www.codeproject.com/KB/openGL/Outline_Mode.aspx
#ifndef ARM9
#define OUTLINE_POLYOFFSET
void gfx_outline_start()
{
	drawing_outline = 1;
#ifdef OUTLINE_POLYOFFSET
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
#else
	//Method 2
	// Push the GL attribute bits so that we don't wreck any settings
	
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	// Set the clear value for the stencil buffer, then clear it
	
	glClearStencil(0);
	glClear( GL_STENCIL_BUFFER_BIT );
	glEnable( GL_STENCIL_TEST );
	// Set the stencil buffer to write a 1 in every time
	
	// a pixel is written to the screen
	
	glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	// Render the object in black
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glColor3f( 1.0f, 1.0f, 1.0f );
#endif
}
void gfx_outline_mid()
{
	drawing_outline = 0;
#ifdef OUTLINE_POLYOFFSET
	//Method 1
	// Set the polygon mode to be filled triangles 
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	// Set the colour to the background
	
	glColor3f( 1.0f, 1.0f, 1.0f );
#else
	//Method 2
	// Set the stencil buffer to only allow writing
	
	// to the screen when the value of the
	
	// stencil buffer is not 1
	
	glStencilFunc( GL_NOTEQUAL, 1, 0xFFFF );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	// Draw the object with thick lines
	
	glLineWidth( 3.0f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f( 0.0f, 0.0f, 0.0f );
#endif
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
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(polyID));
}
#endif

