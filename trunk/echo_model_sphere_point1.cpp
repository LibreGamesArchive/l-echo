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


void draw_sphere_point1()
{
#ifndef ECHO_NDS
	glutSolidSphere(0.100f, 8, 8);
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(156, 0, 378);
		glVertex3v16(144, 59, 378);
		glVertex3v16(289, 0, 289);
		glVertex3v16(267, 110, 289);
		glVertex3v16(378, 0, 156);
		glVertex3v16(349, 144, 156);
		glVertex3v16(409, 0, 0);
		glVertex3v16(378, 156, 0);
		glVertex3v16(378, 0, -156);
		glVertex3v16(349, 144, -156);
		glVertex3v16(289, 0, -289);
		glVertex3v16(267, 110, -289);
		glVertex3v16(156, 0, -378);
		glVertex3v16(144, 59, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(144, 59, 378);
		glVertex3v16(110, 110, 378);
		glVertex3v16(267, 110, 289);
		glVertex3v16(204, 204, 289);
		glVertex3v16(349, 144, 156);
		glVertex3v16(267, 267, 156);
		glVertex3v16(378, 156, 0);
		glVertex3v16(289, 289, 0);
		glVertex3v16(349, 144, -156);
		glVertex3v16(267, 267, -156);
		glVertex3v16(267, 110, -289);
		glVertex3v16(204, 204, -289);
		glVertex3v16(144, 59, -378);
		glVertex3v16(110, 110, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(110, 110, 378);
		glVertex3v16(59, 144, 378);
		glVertex3v16(204, 204, 289);
		glVertex3v16(110, 267, 289);
		glVertex3v16(267, 267, 156);
		glVertex3v16(144, 349, 156);
		glVertex3v16(289, 289, 0);
		glVertex3v16(156, 378, 0);
		glVertex3v16(267, 267, -156);
		glVertex3v16(144, 349, -156);
		glVertex3v16(204, 204, -289);
		glVertex3v16(110, 267, -289);
		glVertex3v16(110, 110, -378);
		glVertex3v16(59, 144, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(59, 144, 378);
		glVertex3v16(0, 156, 378);
		glVertex3v16(110, 267, 289);
		glVertex3v16(0, 289, 289);
		glVertex3v16(144, 349, 156);
		glVertex3v16(0, 378, 156);
		glVertex3v16(156, 378, 0);
		glVertex3v16(0, 409, 0);
		glVertex3v16(144, 349, -156);
		glVertex3v16(0, 378, -156);
		glVertex3v16(110, 267, -289);
		glVertex3v16(0, 289, -289);
		glVertex3v16(59, 144, -378);
		glVertex3v16(0, 156, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 156, 378);
		glVertex3v16(-59, 144, 378);
		glVertex3v16(0, 289, 289);
		glVertex3v16(-110, 267, 289);
		glVertex3v16(0, 378, 156);
		glVertex3v16(-144, 349, 156);
		glVertex3v16(0, 409, 0);
		glVertex3v16(-156, 378, 0);
		glVertex3v16(0, 378, -156);
		glVertex3v16(-144, 349, -156);
		glVertex3v16(0, 289, -289);
		glVertex3v16(-110, 267, -289);
		glVertex3v16(0, 156, -378);
		glVertex3v16(-59, 144, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-59, 144, 378);
		glVertex3v16(-110, 110, 378);
		glVertex3v16(-110, 267, 289);
		glVertex3v16(-204, 204, 289);
		glVertex3v16(-144, 349, 156);
		glVertex3v16(-267, 267, 156);
		glVertex3v16(-156, 378, 0);
		glVertex3v16(-289, 289, 0);
		glVertex3v16(-144, 349, -156);
		glVertex3v16(-267, 267, -156);
		glVertex3v16(-110, 267, -289);
		glVertex3v16(-204, 204, -289);
		glVertex3v16(-59, 144, -378);
		glVertex3v16(-110, 110, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-110, 110, 378);
		glVertex3v16(-144, 59, 378);
		glVertex3v16(-204, 204, 289);
		glVertex3v16(-267, 110, 289);
		glVertex3v16(-267, 267, 156);
		glVertex3v16(-349, 144, 156);
		glVertex3v16(-289, 289, 0);
		glVertex3v16(-378, 156, 0);
		glVertex3v16(-267, 267, -156);
		glVertex3v16(-349, 144, -156);
		glVertex3v16(-204, 204, -289);
		glVertex3v16(-267, 110, -289);
		glVertex3v16(-110, 110, -378);
		glVertex3v16(-144, 59, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-144, 59, 378);
		glVertex3v16(-156, 0, 378);
		glVertex3v16(-267, 110, 289);
		glVertex3v16(-289, 0, 289);
		glVertex3v16(-349, 144, 156);
		glVertex3v16(-378, 0, 156);
		glVertex3v16(-378, 156, 0);
		glVertex3v16(-409, 0, 0);
		glVertex3v16(-349, 144, -156);
		glVertex3v16(-378, 0, -156);
		glVertex3v16(-267, 110, -289);
		glVertex3v16(-289, 0, -289);
		glVertex3v16(-144, 59, -378);
		glVertex3v16(-156, 0, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-156, 0, 378);
		glVertex3v16(-144, -59, 378);
		glVertex3v16(-289, 0, 289);
		glVertex3v16(-267, -110, 289);
		glVertex3v16(-378, 0, 156);
		glVertex3v16(-349, -144, 156);
		glVertex3v16(-409, 0, 0);
		glVertex3v16(-378, -156, 0);
		glVertex3v16(-378, 0, -156);
		glVertex3v16(-349, -144, -156);
		glVertex3v16(-289, 0, -289);
		glVertex3v16(-267, -110, -289);
		glVertex3v16(-156, 0, -378);
		glVertex3v16(-144, -59, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-144, -59, 378);
		glVertex3v16(-110, -110, 378);
		glVertex3v16(-267, -110, 289);
		glVertex3v16(-204, -204, 289);
		glVertex3v16(-349, -144, 156);
		glVertex3v16(-267, -267, 156);
		glVertex3v16(-378, -156, 0);
		glVertex3v16(-289, -289, 0);
		glVertex3v16(-349, -144, -156);
		glVertex3v16(-267, -267, -156);
		glVertex3v16(-267, -110, -289);
		glVertex3v16(-204, -204, -289);
		glVertex3v16(-144, -59, -378);
		glVertex3v16(-110, -110, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-110, -110, 378);
		glVertex3v16(-59, -144, 378);
		glVertex3v16(-204, -204, 289);
		glVertex3v16(-110, -267, 289);
		glVertex3v16(-267, -267, 156);
		glVertex3v16(-144, -349, 156);
		glVertex3v16(-289, -289, 0);
		glVertex3v16(-156, -378, 0);
		glVertex3v16(-267, -267, -156);
		glVertex3v16(-144, -349, -156);
		glVertex3v16(-204, -204, -289);
		glVertex3v16(-110, -267, -289);
		glVertex3v16(-110, -110, -378);
		glVertex3v16(-59, -144, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(-59, -144, 378);
		glVertex3v16(0, -156, 378);
		glVertex3v16(-110, -267, 289);
		glVertex3v16(0, -289, 289);
		glVertex3v16(-144, -349, 156);
		glVertex3v16(0, -378, 156);
		glVertex3v16(-156, -378, 0);
		glVertex3v16(0, -409, 0);
		glVertex3v16(-144, -349, -156);
		glVertex3v16(0, -378, -156);
		glVertex3v16(-110, -267, -289);
		glVertex3v16(0, -289, -289);
		glVertex3v16(-59, -144, -378);
		glVertex3v16(0, -156, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, -156, 378);
		glVertex3v16(59, -144, 378);
		glVertex3v16(0, -289, 289);
		glVertex3v16(110, -267, 289);
		glVertex3v16(0, -378, 156);
		glVertex3v16(144, -349, 156);
		glVertex3v16(0, -409, 0);
		glVertex3v16(156, -378, 0);
		glVertex3v16(0, -378, -156);
		glVertex3v16(144, -349, -156);
		glVertex3v16(0, -289, -289);
		glVertex3v16(110, -267, -289);
		glVertex3v16(0, -156, -378);
		glVertex3v16(59, -144, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(59, -144, 378);
		glVertex3v16(110, -110, 378);
		glVertex3v16(110, -267, 289);
		glVertex3v16(204, -204, 289);
		glVertex3v16(144, -349, 156);
		glVertex3v16(267, -267, 156);
		glVertex3v16(156, -378, 0);
		glVertex3v16(289, -289, 0);
		glVertex3v16(144, -349, -156);
		glVertex3v16(267, -267, -156);
		glVertex3v16(110, -267, -289);
		glVertex3v16(204, -204, -289);
		glVertex3v16(59, -144, -378);
		glVertex3v16(110, -110, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(110, -110, 378);
		glVertex3v16(144, -59, 378);
		glVertex3v16(204, -204, 289);
		glVertex3v16(267, -110, 289);
		glVertex3v16(267, -267, 156);
		glVertex3v16(349, -144, 156);
		glVertex3v16(289, -289, 0);
		glVertex3v16(378, -156, 0);
		glVertex3v16(267, -267, -156);
		glVertex3v16(349, -144, -156);
		glVertex3v16(204, -204, -289);
		glVertex3v16(267, -110, -289);
		glVertex3v16(110, -110, -378);
		glVertex3v16(144, -59, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 0, 409);
		glVertex3v16(0, 0, 409);
		glVertex3v16(144, -59, 378);
		glVertex3v16(156, 0, 378);
		glVertex3v16(267, -110, 289);
		glVertex3v16(289, 0, 289);
		glVertex3v16(349, -144, 156);
		glVertex3v16(378, 0, 156);
		glVertex3v16(378, -156, 0);
		glVertex3v16(409, 0, 0);
		glVertex3v16(349, -144, -156);
		glVertex3v16(378, 0, -156);
		glVertex3v16(267, -110, -289);
		glVertex3v16(289, 0, -289);
		glVertex3v16(144, -59, -378);
		glVertex3v16(156, 0, -378);
		glVertex3v16(0, 0, -409);
		glVertex3v16(0, 0, -409);
	glEnd();
#endif
}

