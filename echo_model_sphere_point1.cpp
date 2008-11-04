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


//stats: LATS: 6, ANGLE_INCR: 0.523599, NDS_LATS: 2, NDS_ANGLE_INCR: 1.570796
void draw_sphere_point1()
{
#ifndef ECHO_NDS
	glutSolidSphere(0.100f, 6, 6);
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-409, 409, -409);
		glVertex3v16(-409, -409, -409);
		glVertex3v16(409, 409, -409);
		glVertex3v16(409, -409, -409);
		glVertex3v16(409, 409, 409);
		glVertex3v16(409, -409, 409);
		glVertex3v16(-409, 409, 409);
		glVertex3v16(-409, -409, 409);
		glVertex3v16(-409, 409, -409);
		glVertex3v16(-409, -409, -409);
	glEnd();
#endif
}

