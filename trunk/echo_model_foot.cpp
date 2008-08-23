// echo_model_foot.cpp

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


void draw_foot()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.029f, 0.139f, 0.000f);
		glVertex3f(0.027f, 0.139f, 0.029f);
		glVertex3f(0.053f, 0.106f, 0.000f);
		glVertex3f(0.049f, 0.106f, 0.054f);
		glVertex3f(0.069f, 0.057f, 0.000f);
		glVertex3f(0.064f, 0.057f, 0.071f);
		glVertex3f(0.075f, 0.000f, 0.000f);
		glVertex3f(0.069f, 0.000f, 0.077f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.027f, 0.139f, 0.029f);
		glVertex3f(0.020f, 0.139f, 0.054f);
		glVertex3f(0.049f, 0.106f, 0.054f);
		glVertex3f(0.038f, 0.106f, 0.100f);
		glVertex3f(0.064f, 0.057f, 0.071f);
		glVertex3f(0.049f, 0.057f, 0.131f);
		glVertex3f(0.069f, 0.000f, 0.077f);
		glVertex3f(0.053f, 0.000f, 0.141f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.020f, 0.139f, 0.054f);
		glVertex3f(0.011f, 0.139f, 0.071f);
		glVertex3f(0.038f, 0.106f, 0.100f);
		glVertex3f(0.020f, 0.106f, 0.131f);
		glVertex3f(0.049f, 0.057f, 0.131f);
		glVertex3f(0.027f, 0.057f, 0.171f);
		glVertex3f(0.053f, 0.000f, 0.141f);
		glVertex3f(0.029f, 0.000f, 0.185f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(0.011f, 0.139f, 0.071f);
		glVertex3f(0.000f, 0.139f, 0.077f);
		glVertex3f(0.020f, 0.106f, 0.131f);
		glVertex3f(0.000f, 0.106f, 0.141f);
		glVertex3f(0.027f, 0.057f, 0.171f);
		glVertex3f(0.000f, 0.057f, 0.185f);
		glVertex3f(0.029f, 0.000f, 0.185f);
		glVertex3f(0.000f, 0.000f, 0.200f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.150f, 0.000f);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(0.000f, 0.139f, 0.077f);
		glVertex3f(-0.011f, 0.139f, 0.071f);
		glVertex3f(0.000f, 0.106f, 0.141f);
		glVertex3f(-0.020f, 0.106f, 0.131f);
		glVertex3f(0.000f, 0.057f, 0.185f);
		glVertex3f(-0.027f, 0.057f, 0.171f);
		glVertex3f(0.000f, 0.000f, 0.200f);
		glVertex3f(-0.029f, 0.000f, 0.185f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.011f, 0.139f, 0.071f);
		glVertex3f(-0.020f, 0.139f, 0.054f);
		glVertex3f(-0.020f, 0.106f, 0.131f);
		glVertex3f(-0.037f, 0.106f, 0.100f);
		glVertex3f(-0.027f, 0.057f, 0.171f);
		glVertex3f(-0.049f, 0.057f, 0.131f);
		glVertex3f(-0.029f, 0.000f, 0.185f);
		glVertex3f(-0.053f, 0.000f, 0.141f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.020f, 0.139f, 0.054f);
		glVertex3f(-0.027f, 0.139f, 0.029f);
		glVertex3f(-0.037f, 0.106f, 0.100f);
		glVertex3f(-0.049f, 0.106f, 0.054f);
		glVertex3f(-0.049f, 0.057f, 0.131f);
		glVertex3f(-0.064f, 0.057f, 0.071f);
		glVertex3f(-0.053f, 0.000f, 0.141f);
		glVertex3f(-0.069f, 0.000f, 0.077f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.000f, 0.150f, 0.000f);
		glVertex3f(-0.027f, 0.139f, 0.029f);
		glVertex3f(-0.029f, 0.139f, 0.000f);
		glVertex3f(-0.049f, 0.106f, 0.054f);
		glVertex3f(-0.053f, 0.106f, 0.000f);
		glVertex3f(-0.064f, 0.057f, 0.071f);
		glVertex3f(-0.069f, 0.057f, 0.000f);
		glVertex3f(-0.069f, 0.000f, 0.077f);
		glVertex3f(-0.075f, 0.000f, 0.000f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 567, 313);
		glVertex3v16(-44, 567, 289);
		glVertex3v16(0, 434, 579);
		glVertex3v16(-83, 434, 535);
		glVertex3v16(0, 235, 756);
		glVertex3v16(-108, 235, 699);
		glVertex3v16(0, 0, 819);
		glVertex3v16(-117, 0, 756);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-44, 567, 289);
		glVertex3v16(-83, 567, 221);
		glVertex3v16(-83, 434, 535);
		glVertex3v16(-153, 434, 409);
		glVertex3v16(-108, 235, 699);
		glVertex3v16(-200, 235, 535);
		glVertex3v16(-117, 0, 756);
		glVertex3v16(-217, 0, 579);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-83, 567, 221);
		glVertex3v16(-108, 567, 119);
		glVertex3v16(-153, 434, 409);
		glVertex3v16(-200, 434, 221);
		glVertex3v16(-200, 235, 535);
		glVertex3v16(-262, 235, 289);
		glVertex3v16(-217, 0, 579);
		glVertex3v16(-283, 0, 313);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-108, 567, 119);
		glVertex3v16(-117, 567, 0);
		glVertex3v16(-200, 434, 221);
		glVertex3v16(-217, 434, 0);
		glVertex3v16(-262, 235, 289);
		glVertex3v16(-283, 235, 0);
		glVertex3v16(-283, 0, 313);
		glVertex3v16(-307, 0, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-117, 567, 0);
		glVertex3v16(-108, 567, -119);
		glVertex3v16(-217, 434, 0);
		glVertex3v16(-200, 434, -221);
		glVertex3v16(-283, 235, 0);
		glVertex3v16(-262, 235, -289);
		glVertex3v16(-307, 0, 0);
		glVertex3v16(-283, 0, -313);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-108, 567, -119);
		glVertex3v16(-83, 567, -221);
		glVertex3v16(-200, 434, -221);
		glVertex3v16(-153, 434, -409);
		glVertex3v16(-262, 235, -289);
		glVertex3v16(-200, 235, -535);
		glVertex3v16(-283, 0, -313);
		glVertex3v16(-217, 0, -579);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-83, 567, -221);
		glVertex3v16(-44, 567, -289);
		glVertex3v16(-153, 434, -409);
		glVertex3v16(-83, 434, -535);
		glVertex3v16(-200, 235, -535);
		glVertex3v16(-108, 235, -699);
		glVertex3v16(-217, 0, -579);
		glVertex3v16(-117, 0, -756);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 614, 0);
		glVertex3v16(0, 614, 0);
		glVertex3v16(-44, 567, -289);
		glVertex3v16(0, 567, -313);
		glVertex3v16(-83, 434, -535);
		glVertex3v16(0, 434, -579);
		glVertex3v16(-108, 235, -699);
		glVertex3v16(0, 235, -756);
		glVertex3v16(-117, 0, -756);
		glVertex3v16(0, 0, -819);
	glEnd();
#endif
}

