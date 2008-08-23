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

#include <echo_platform.h>
#include <echo_gfx.h>

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


void draw_right_hand()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.000f, 0.075f);
		glVertex3f(-0.029f, 0.000f, 0.069f);
		glVertex3f(0.000f, -0.038f, 0.069f);
		glVertex3f(-0.027f, -0.038f, 0.064f);
		glVertex3f(0.000f, -0.071f, 0.053f);
		glVertex3f(-0.020f, -0.071f, 0.049f);
		glVertex3f(0.000f, -0.092f, 0.029f);
		glVertex3f(-0.011f, -0.092f, 0.027f);
		glVertex3f(0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.029f, 0.000f, 0.069f);
		glVertex3f(-0.053f, 0.000f, 0.053f);
		glVertex3f(-0.027f, -0.038f, 0.064f);
		glVertex3f(-0.049f, -0.038f, 0.049f);
		glVertex3f(-0.020f, -0.071f, 0.049f);
		glVertex3f(-0.038f, -0.071f, 0.038f);
		glVertex3f(-0.011f, -0.092f, 0.027f);
		glVertex3f(-0.020f, -0.092f, 0.020f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.053f, 0.000f, 0.053f);
		glVertex3f(-0.069f, 0.000f, 0.029f);
		glVertex3f(-0.049f, -0.038f, 0.049f);
		glVertex3f(-0.064f, -0.038f, 0.027f);
		glVertex3f(-0.038f, -0.071f, 0.038f);
		glVertex3f(-0.049f, -0.071f, 0.020f);
		glVertex3f(-0.020f, -0.092f, 0.020f);
		glVertex3f(-0.027f, -0.092f, 0.011f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.069f, 0.000f, 0.029f);
		glVertex3f(-0.075f, 0.000f, 0.000f);
		glVertex3f(-0.064f, -0.038f, 0.027f);
		glVertex3f(-0.069f, -0.038f, 0.000f);
		glVertex3f(-0.049f, -0.071f, 0.020f);
		glVertex3f(-0.053f, -0.071f, 0.000f);
		glVertex3f(-0.027f, -0.092f, 0.011f);
		glVertex3f(-0.029f, -0.092f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.075f, 0.000f, 0.000f);
		glVertex3f(-0.069f, 0.000f, -0.029f);
		glVertex3f(-0.069f, -0.038f, 0.000f);
		glVertex3f(-0.064f, -0.038f, -0.027f);
		glVertex3f(-0.053f, -0.071f, 0.000f);
		glVertex3f(-0.049f, -0.071f, -0.020f);
		glVertex3f(-0.029f, -0.092f, 0.000f);
		glVertex3f(-0.027f, -0.092f, -0.011f);
		glVertex3f(-0.000f, -0.100f, 0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.069f, 0.000f, -0.029f);
		glVertex3f(-0.053f, 0.000f, -0.053f);
		glVertex3f(-0.064f, -0.038f, -0.027f);
		glVertex3f(-0.049f, -0.038f, -0.049f);
		glVertex3f(-0.049f, -0.071f, -0.020f);
		glVertex3f(-0.038f, -0.071f, -0.037f);
		glVertex3f(-0.027f, -0.092f, -0.011f);
		glVertex3f(-0.020f, -0.092f, -0.020f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.053f, 0.000f, -0.053f);
		glVertex3f(-0.029f, 0.000f, -0.069f);
		glVertex3f(-0.049f, -0.038f, -0.049f);
		glVertex3f(-0.027f, -0.038f, -0.064f);
		glVertex3f(-0.038f, -0.071f, -0.037f);
		glVertex3f(-0.020f, -0.071f, -0.049f);
		glVertex3f(-0.020f, -0.092f, -0.020f);
		glVertex3f(-0.011f, -0.092f, -0.027f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.029f, 0.000f, -0.069f);
		glVertex3f(-0.000f, 0.000f, -0.075f);
		glVertex3f(-0.027f, -0.038f, -0.064f);
		glVertex3f(-0.000f, -0.038f, -0.069f);
		glVertex3f(-0.020f, -0.071f, -0.049f);
		glVertex3f(-0.000f, -0.071f, -0.053f);
		glVertex3f(-0.011f, -0.092f, -0.027f);
		glVertex3f(-0.000f, -0.092f, -0.029f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
		glVertex3f(-0.000f, -0.100f, -0.000f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-117, 0, 283);
		glVertex3v16(0, -156, 283);
		glVertex3v16(-108, -156, 262);
		glVertex3v16(0, -289, 217);
		glVertex3v16(-83, -289, 200);
		glVertex3v16(0, -378, 117);
		glVertex3v16(-44, -378, 108);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-117, 0, 283);
		glVertex3v16(-217, 0, 217);
		glVertex3v16(-108, -156, 262);
		glVertex3v16(-200, -156, 200);
		glVertex3v16(-83, -289, 200);
		glVertex3v16(-153, -289, 153);
		glVertex3v16(-44, -378, 108);
		glVertex3v16(-83, -378, 83);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-217, 0, 217);
		glVertex3v16(-283, 0, 117);
		glVertex3v16(-200, -156, 200);
		glVertex3v16(-262, -156, 108);
		glVertex3v16(-153, -289, 153);
		glVertex3v16(-200, -289, 83);
		glVertex3v16(-83, -378, 83);
		glVertex3v16(-108, -378, 44);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-283, 0, 117);
		glVertex3v16(-307, 0, 0);
		glVertex3v16(-262, -156, 108);
		glVertex3v16(-283, -156, 0);
		glVertex3v16(-200, -289, 83);
		glVertex3v16(-217, -289, 0);
		glVertex3v16(-108, -378, 44);
		glVertex3v16(-117, -378, 0);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-307, 0, 0);
		glVertex3v16(-283, 0, -117);
		glVertex3v16(-283, -156, 0);
		glVertex3v16(-262, -156, -108);
		glVertex3v16(-217, -289, 0);
		glVertex3v16(-200, -289, -83);
		glVertex3v16(-117, -378, 0);
		glVertex3v16(-108, -378, -44);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-283, 0, -117);
		glVertex3v16(-217, 0, -217);
		glVertex3v16(-262, -156, -108);
		glVertex3v16(-200, -156, -200);
		glVertex3v16(-200, -289, -83);
		glVertex3v16(-153, -289, -153);
		glVertex3v16(-108, -378, -44);
		glVertex3v16(-83, -378, -83);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-217, 0, -217);
		glVertex3v16(-117, 0, -283);
		glVertex3v16(-200, -156, -200);
		glVertex3v16(-108, -156, -262);
		glVertex3v16(-153, -289, -153);
		glVertex3v16(-83, -289, -200);
		glVertex3v16(-83, -378, -83);
		glVertex3v16(-44, -378, -108);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-117, 0, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(-108, -156, -262);
		glVertex3v16(0, -156, -283);
		glVertex3v16(-83, -289, -200);
		glVertex3v16(0, -289, -217);
		glVertex3v16(-44, -378, -108);
		glVertex3v16(0, -378, -117);
		glVertex3v16(0, -409, 0);
		glVertex3v16(0, -409, 0);
	glEnd();
#endif
}

