// echo_model_right_hand.cpp

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
void draw_right_hand()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.000f, 0.075f);
		glVertex3f(-0.037f, 0.000f, 0.065f);
		glVertex3f(0.000f, -0.050f, 0.065f);
		glVertex3f(-0.032f, -0.050f, 0.056f);
		glVertex3f(0.000f, -0.087f, 0.038f);
		glVertex3f(-0.019f, -0.087f, 0.032f);
		glVertex3f(0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.037f, 0.000f, 0.065f);
		glVertex3f(-0.065f, 0.000f, 0.038f);
		glVertex3f(-0.032f, -0.050f, 0.056f);
		glVertex3f(-0.056f, -0.050f, 0.032f);
		glVertex3f(-0.019f, -0.087f, 0.032f);
		glVertex3f(-0.032f, -0.087f, 0.019f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.065f, 0.000f, 0.038f);
		glVertex3f(-0.075f, 0.000f, 0.000f);
		glVertex3f(-0.056f, -0.050f, 0.032f);
		glVertex3f(-0.065f, -0.050f, 0.000f);
		glVertex3f(-0.032f, -0.087f, 0.019f);
		glVertex3f(-0.038f, -0.087f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.075f, 0.000f, 0.000f);
		glVertex3f(-0.065f, 0.000f, -0.037f);
		glVertex3f(-0.065f, -0.050f, 0.000f);
		glVertex3f(-0.056f, -0.050f, -0.032f);
		glVertex3f(-0.038f, -0.087f, 0.000f);
		glVertex3f(-0.032f, -0.087f, -0.019f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.065f, 0.000f, -0.037f);
		glVertex3f(-0.038f, 0.000f, -0.065f);
		glVertex3f(-0.056f, -0.050f, -0.032f);
		glVertex3f(-0.032f, -0.050f, -0.056f);
		glVertex3f(-0.032f, -0.087f, -0.019f);
		glVertex3f(-0.019f, -0.087f, -0.032f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.038f, 0.000f, -0.065f);
		glVertex3f(-0.000f, 0.000f, -0.075f);
		glVertex3f(-0.032f, -0.050f, -0.056f);
		glVertex3f(-0.000f, -0.050f, -0.065f);
		glVertex3f(-0.019f, -0.087f, -0.032f);
		glVertex3f(-0.000f, -0.087f, -0.038f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.000f, -0.075f);
		glVertex3f(0.037f, 0.000f, -0.065f);
		glVertex3f(-0.000f, -0.050f, -0.065f);
		glVertex3f(0.032f, -0.050f, -0.056f);
		glVertex3f(-0.000f, -0.087f, -0.038f);
		glVertex3f(0.019f, -0.087f, -0.032f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(0.000f, -0.100f, -0.000f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-307, 0, 307);
		glVertex3v16(0, -409, 307);
		glVertex3v16(-307, -409, 307);
		glVertex3v16(0, -409, -307);
		glVertex3v16(-307, -409, -307);
		glVertex3v16(0, 0, -307);
		glVertex3v16(-307, 0, -307);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3v16(-307, 0, 307);
		glVertex3v16(-307, -409, 307);
		glVertex3v16(-307, -409, -307);
		glVertex3v16(-307, 0, -307);
	glEnd();
#endif
}

