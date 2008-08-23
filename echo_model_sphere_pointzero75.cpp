// echo_model_sphere.cpp

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


void draw_sphere_pointzero75()
{
#ifndef ECHO_NDS
	glutSolidSphere(0.075f, 8, 8);
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(117, 0, 283);
		glVertex3v16(108, 44, 283);
		glVertex3v16(217, 0, 217);
		glVertex3v16(200, 83, 217);
		glVertex3v16(283, 0, 117);
		glVertex3v16(262, 108, 117);
		glVertex3v16(307, 0, 0);
		glVertex3v16(283, 117, 0);
		glVertex3v16(283, 0, -117);
		glVertex3v16(262, 108, -117);
		glVertex3v16(217, 0, -217);
		glVertex3v16(200, 83, -217);
		glVertex3v16(117, 0, -283);
		glVertex3v16(108, 44, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(108, 44, 283);
		glVertex3v16(83, 83, 283);
		glVertex3v16(200, 83, 217);
		glVertex3v16(153, 153, 217);
		glVertex3v16(262, 108, 117);
		glVertex3v16(200, 200, 117);
		glVertex3v16(283, 117, 0);
		glVertex3v16(217, 217, 0);
		glVertex3v16(262, 108, -117);
		glVertex3v16(200, 200, -117);
		glVertex3v16(200, 83, -217);
		glVertex3v16(153, 153, -217);
		glVertex3v16(108, 44, -283);
		glVertex3v16(83, 83, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(83, 83, 283);
		glVertex3v16(44, 108, 283);
		glVertex3v16(153, 153, 217);
		glVertex3v16(83, 200, 217);
		glVertex3v16(200, 200, 117);
		glVertex3v16(108, 262, 117);
		glVertex3v16(217, 217, 0);
		glVertex3v16(117, 283, 0);
		glVertex3v16(200, 200, -117);
		glVertex3v16(108, 262, -117);
		glVertex3v16(153, 153, -217);
		glVertex3v16(83, 200, -217);
		glVertex3v16(83, 83, -283);
		glVertex3v16(44, 108, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(44, 108, 283);
		glVertex3v16(0, 117, 283);
		glVertex3v16(83, 200, 217);
		glVertex3v16(0, 217, 217);
		glVertex3v16(108, 262, 117);
		glVertex3v16(0, 283, 117);
		glVertex3v16(117, 283, 0);
		glVertex3v16(0, 307, 0);
		glVertex3v16(108, 262, -117);
		glVertex3v16(0, 283, -117);
		glVertex3v16(83, 200, -217);
		glVertex3v16(0, 217, -217);
		glVertex3v16(44, 108, -283);
		glVertex3v16(0, 117, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 117, 283);
		glVertex3v16(-44, 108, 283);
		glVertex3v16(0, 217, 217);
		glVertex3v16(-83, 200, 217);
		glVertex3v16(0, 283, 117);
		glVertex3v16(-108, 262, 117);
		glVertex3v16(0, 307, 0);
		glVertex3v16(-117, 283, 0);
		glVertex3v16(0, 283, -117);
		glVertex3v16(-108, 262, -117);
		glVertex3v16(0, 217, -217);
		glVertex3v16(-83, 200, -217);
		glVertex3v16(0, 117, -283);
		glVertex3v16(-44, 108, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-44, 108, 283);
		glVertex3v16(-83, 83, 283);
		glVertex3v16(-83, 200, 217);
		glVertex3v16(-153, 153, 217);
		glVertex3v16(-108, 262, 117);
		glVertex3v16(-200, 200, 117);
		glVertex3v16(-117, 283, 0);
		glVertex3v16(-217, 217, 0);
		glVertex3v16(-108, 262, -117);
		glVertex3v16(-200, 200, -117);
		glVertex3v16(-83, 200, -217);
		glVertex3v16(-153, 153, -217);
		glVertex3v16(-44, 108, -283);
		glVertex3v16(-83, 83, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-83, 83, 283);
		glVertex3v16(-108, 44, 283);
		glVertex3v16(-153, 153, 217);
		glVertex3v16(-200, 83, 217);
		glVertex3v16(-200, 200, 117);
		glVertex3v16(-262, 108, 117);
		glVertex3v16(-217, 217, 0);
		glVertex3v16(-283, 117, 0);
		glVertex3v16(-200, 200, -117);
		glVertex3v16(-262, 108, -117);
		glVertex3v16(-153, 153, -217);
		glVertex3v16(-200, 83, -217);
		glVertex3v16(-83, 83, -283);
		glVertex3v16(-108, 44, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-108, 44, 283);
		glVertex3v16(-117, 0, 283);
		glVertex3v16(-200, 83, 217);
		glVertex3v16(-217, 0, 217);
		glVertex3v16(-262, 108, 117);
		glVertex3v16(-283, 0, 117);
		glVertex3v16(-283, 117, 0);
		glVertex3v16(-307, 0, 0);
		glVertex3v16(-262, 108, -117);
		glVertex3v16(-283, 0, -117);
		glVertex3v16(-200, 83, -217);
		glVertex3v16(-217, 0, -217);
		glVertex3v16(-108, 44, -283);
		glVertex3v16(-117, 0, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-117, 0, 283);
		glVertex3v16(-108, -44, 283);
		glVertex3v16(-217, 0, 217);
		glVertex3v16(-200, -83, 217);
		glVertex3v16(-283, 0, 117);
		glVertex3v16(-262, -108, 117);
		glVertex3v16(-307, 0, 0);
		glVertex3v16(-283, -117, 0);
		glVertex3v16(-283, 0, -117);
		glVertex3v16(-262, -108, -117);
		glVertex3v16(-217, 0, -217);
		glVertex3v16(-200, -83, -217);
		glVertex3v16(-117, 0, -283);
		glVertex3v16(-108, -44, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-108, -44, 283);
		glVertex3v16(-83, -83, 283);
		glVertex3v16(-200, -83, 217);
		glVertex3v16(-153, -153, 217);
		glVertex3v16(-262, -108, 117);
		glVertex3v16(-200, -200, 117);
		glVertex3v16(-283, -117, 0);
		glVertex3v16(-217, -217, 0);
		glVertex3v16(-262, -108, -117);
		glVertex3v16(-200, -200, -117);
		glVertex3v16(-200, -83, -217);
		glVertex3v16(-153, -153, -217);
		glVertex3v16(-108, -44, -283);
		glVertex3v16(-83, -83, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-83, -83, 283);
		glVertex3v16(-44, -108, 283);
		glVertex3v16(-153, -153, 217);
		glVertex3v16(-83, -200, 217);
		glVertex3v16(-200, -200, 117);
		glVertex3v16(-108, -262, 117);
		glVertex3v16(-217, -217, 0);
		glVertex3v16(-117, -283, 0);
		glVertex3v16(-200, -200, -117);
		glVertex3v16(-108, -262, -117);
		glVertex3v16(-153, -153, -217);
		glVertex3v16(-83, -200, -217);
		glVertex3v16(-83, -83, -283);
		glVertex3v16(-44, -108, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(-44, -108, 283);
		glVertex3v16(0, -117, 283);
		glVertex3v16(-83, -200, 217);
		glVertex3v16(0, -217, 217);
		glVertex3v16(-108, -262, 117);
		glVertex3v16(0, -283, 117);
		glVertex3v16(-117, -283, 0);
		glVertex3v16(0, -307, 0);
		glVertex3v16(-108, -262, -117);
		glVertex3v16(0, -283, -117);
		glVertex3v16(-83, -200, -217);
		glVertex3v16(0, -217, -217);
		glVertex3v16(-44, -108, -283);
		glVertex3v16(0, -117, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, -117, 283);
		glVertex3v16(44, -108, 283);
		glVertex3v16(0, -217, 217);
		glVertex3v16(83, -200, 217);
		glVertex3v16(0, -283, 117);
		glVertex3v16(108, -262, 117);
		glVertex3v16(0, -307, 0);
		glVertex3v16(117, -283, 0);
		glVertex3v16(0, -283, -117);
		glVertex3v16(108, -262, -117);
		glVertex3v16(0, -217, -217);
		glVertex3v16(83, -200, -217);
		glVertex3v16(0, -117, -283);
		glVertex3v16(44, -108, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(44, -108, 283);
		glVertex3v16(83, -83, 283);
		glVertex3v16(83, -200, 217);
		glVertex3v16(153, -153, 217);
		glVertex3v16(108, -262, 117);
		glVertex3v16(200, -200, 117);
		glVertex3v16(117, -283, 0);
		glVertex3v16(217, -217, 0);
		glVertex3v16(108, -262, -117);
		glVertex3v16(200, -200, -117);
		glVertex3v16(83, -200, -217);
		glVertex3v16(153, -153, -217);
		glVertex3v16(44, -108, -283);
		glVertex3v16(83, -83, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(83, -83, 283);
		glVertex3v16(108, -44, 283);
		glVertex3v16(153, -153, 217);
		glVertex3v16(200, -83, 217);
		glVertex3v16(200, -200, 117);
		glVertex3v16(262, -108, 117);
		glVertex3v16(217, -217, 0);
		glVertex3v16(283, -117, 0);
		glVertex3v16(200, -200, -117);
		glVertex3v16(262, -108, -117);
		glVertex3v16(153, -153, -217);
		glVertex3v16(200, -83, -217);
		glVertex3v16(83, -83, -283);
		glVertex3v16(108, -44, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 307);
		glVertex3v16(0, 0, 307);
		glVertex3v16(108, -44, 283);
		glVertex3v16(117, 0, 283);
		glVertex3v16(200, -83, 217);
		glVertex3v16(217, 0, 217);
		glVertex3v16(262, -108, 117);
		glVertex3v16(283, 0, 117);
		glVertex3v16(283, -117, 0);
		glVertex3v16(307, 0, 0);
		glVertex3v16(262, -108, -117);
		glVertex3v16(283, 0, -117);
		glVertex3v16(200, -83, -217);
		glVertex3v16(217, 0, -217);
		glVertex3v16(108, -44, -283);
		glVertex3v16(117, 0, -283);
		glVertex3v16(0, 0, -307);
		glVertex3v16(0, 0, -307);
	glEnd();
#endif
}

