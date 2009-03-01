// echo_model_lower_body.cpp

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

#include "echo_platform.h"
#include "echo_gfx.h"

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


//stats: LATS: 6, ANGLE_INCR: 0.523599, NDS_LATS: 2, NDS_ANGLE_INCR: 1.570796
void draw_lower_body()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.100f, 0.260f, 0.000f);
		glVertex3f(0.087f, 0.260f, 0.037f);
		glVertex3f(0.173f, 0.150f, 0.000f);
		glVertex3f(0.150f, 0.150f, 0.065f);
		glVertex3f(0.200f, 0.000f, 0.000f);
		glVertex3f(0.173f, 0.000f, 0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.087f, 0.260f, 0.037f);
		glVertex3f(0.050f, 0.260f, 0.065f);
		glVertex3f(0.150f, 0.150f, 0.065f);
		glVertex3f(0.087f, 0.150f, 0.112f);
		glVertex3f(0.173f, 0.000f, 0.075f);
		glVertex3f(0.100f, 0.000f, 0.130f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.050f, 0.260f, 0.065f);
		glVertex3f(0.000f, 0.260f, 0.075f);
		glVertex3f(0.087f, 0.150f, 0.112f);
		glVertex3f(0.000f, 0.150f, 0.130f);
		glVertex3f(0.100f, 0.000f, 0.130f);
		glVertex3f(0.000f, 0.000f, 0.150f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.260f, 0.075f);
		glVertex3f(-0.050f, 0.260f, 0.065f);
		glVertex3f(0.000f, 0.150f, 0.130f);
		glVertex3f(-0.087f, 0.150f, 0.113f);
		glVertex3f(0.000f, 0.000f, 0.150f);
		glVertex3f(-0.100f, 0.000f, 0.130f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.050f, 0.260f, 0.065f);
		glVertex3f(-0.087f, 0.260f, 0.038f);
		glVertex3f(-0.087f, 0.150f, 0.113f);
		glVertex3f(-0.150f, 0.150f, 0.065f);
		glVertex3f(-0.100f, 0.000f, 0.130f);
		glVertex3f(-0.173f, 0.000f, 0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.087f, 0.260f, 0.038f);
		glVertex3f(-0.100f, 0.260f, 0.000f);
		glVertex3f(-0.150f, 0.150f, 0.065f);
		glVertex3f(-0.173f, 0.150f, 0.000f);
		glVertex3f(-0.173f, 0.000f, 0.075f);
		glVertex3f(-0.200f, 0.000f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.100f, 0.260f, 0.000f);
		glVertex3f(-0.087f, 0.260f, -0.037f);
		glVertex3f(-0.173f, 0.150f, 0.000f);
		glVertex3f(-0.150f, 0.150f, -0.065f);
		glVertex3f(-0.200f, 0.000f, 0.000f);
		glVertex3f(-0.173f, 0.000f, -0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.087f, 0.260f, -0.037f);
		glVertex3f(-0.050f, 0.260f, -0.065f);
		glVertex3f(-0.150f, 0.150f, -0.065f);
		glVertex3f(-0.087f, 0.150f, -0.112f);
		glVertex3f(-0.173f, 0.000f, -0.075f);
		glVertex3f(-0.100f, 0.000f, -0.130f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.050f, 0.260f, -0.065f);
		glVertex3f(-0.000f, 0.260f, -0.075f);
		glVertex3f(-0.087f, 0.150f, -0.112f);
		glVertex3f(-0.000f, 0.150f, -0.130f);
		glVertex3f(-0.100f, 0.000f, -0.130f);
		glVertex3f(-0.000f, 0.000f, -0.150f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.260f, -0.075f);
		glVertex3f(0.050f, 0.260f, -0.065f);
		glVertex3f(-0.000f, 0.150f, -0.130f);
		glVertex3f(0.087f, 0.150f, -0.113f);
		glVertex3f(-0.000f, 0.000f, -0.150f);
		glVertex3f(0.100f, 0.000f, -0.130f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.050f, 0.260f, -0.065f);
		glVertex3f(0.087f, 0.260f, -0.038f);
		glVertex3f(0.087f, 0.150f, -0.113f);
		glVertex3f(0.150f, 0.150f, -0.065f);
		glVertex3f(0.100f, 0.000f, -0.130f);
		glVertex3f(0.173f, 0.000f, -0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.087f, 0.260f, -0.038f);
		glVertex3f(0.100f, 0.260f, -0.000f);
		glVertex3f(0.150f, 0.150f, -0.065f);
		glVertex3f(0.173f, 0.150f, -0.000f);
		glVertex3f(0.173f, 0.000f, -0.075f);
		glVertex3f(0.200f, 0.000f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.100f, 0.260f, -0.000f);
		glVertex3f(0.087f, 0.260f, 0.037f);
		glVertex3f(0.173f, 0.150f, -0.000f);
		glVertex3f(0.150f, 0.150f, 0.065f);
		glVertex3f(0.200f, 0.000f, -0.000f);
		glVertex3f(0.173f, 0.000f, 0.075f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-819, -614, 614);
		glVertex3v16(-819, 1213, 614);
		glVertex3v16(819, -614, 614);
		glVertex3v16(819, 1213, 614);
		glVertex3v16(819, -614, -614);
		glVertex3v16(819, 1213, -614);
		glVertex3v16(-819, -614, -614);
		glVertex3v16(-819, 1213, -614);
		glVertex3v16(-819, -614, 614);
		glVertex3v16(-819, 1213, 614);
	glEnd();
#endif
}

