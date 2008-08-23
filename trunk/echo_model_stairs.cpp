// echo_model_stairs.cpp

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


#ifdef ARM9
	#define SET_ID(id)	gfx_set_polyID(id)
#else
	#define SET_ID(id)
#endif

void draw_stairs()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.5f, -0.332f, -0.332f);
		glVertex3f(-0.5f, -0.332f, -0.332f);
		glVertex3f(0.5f, -0.498f, -0.332f);
		glVertex3f(-0.5f, -0.498f, -0.332f);
		glVertex3f(0.5f, -0.166f, -0.166f);
		glVertex3f(-0.5f, -0.166f, -0.166f);
		glVertex3f(0.5f, -0.332f, -0.166f);
		glVertex3f(-0.5f, -0.332f, -0.166f);
		glVertex3f(0.5f, 0.000f, 0.000f);
		glVertex3f(-0.5f, 0.000f, 0.000f);
		glVertex3f(0.5f, -0.166f, 0.000f);
		glVertex3f(-0.5f, -0.166f, 0.000f);
		glVertex3f(0.5f, 0.166f, 0.166f);
		glVertex3f(-0.5f, 0.166f, 0.166f);
		glVertex3f(0.5f, 0.000f, 0.166f);
		glVertex3f(-0.5f, 0.000f, 0.166f);
		glVertex3f(0.5f, 0.332f, 0.332f);
		glVertex3f(-0.5f, 0.332f, 0.332f);
		glVertex3f(0.5f, 0.166f, 0.332f);
		glVertex3f(-0.5f, 0.166f, 0.332f);
		glVertex3f(0.5f, 0.498f, 0.498f);
		glVertex3f(-0.5f, 0.498f, 0.498f);
		glVertex3f(0.5f, 0.332f, 0.498f);
		glVertex3f(-0.5f, 0.332f, 0.498f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
#else
	SET_ID(16);
	draw_rectv16(2048, -1359, -1359, 2048, -2039, -1359
		, -2048, -2039, -1359, -2048, -1359, -1359);
	SET_ID(17);
	draw_rectv16(2048, -2039, -2039, 2048, -2039, -1359
		, -2048, -2039, -1359, -2048, -2039, -2039);
	SET_ID(18);
	draw_rectv16(2048, -679, -679, 2048, -1359, -679
		, -2048, -1359, -679, -2048, -679, -679);
	SET_ID(16);
	draw_rectv16(2048, -1359, -1359, 2048, -1359, -679
		, -2048, -1359, -679, -2048, -1359, -1359);
	SET_ID(17);
	draw_rectv16(2048, 0, 0, 2048, -679, 0
		, -2048, -679, 0, -2048, 0, 0);
	SET_ID(18);
	draw_rectv16(2048, -679, -679, 2048, -679, 0
		, -2048, -679, 0, -2048, -679, -679);
	SET_ID(16);
	draw_rectv16(2048, 679, 679, 2048, 0, 679
		, -2048, 0, 679, -2048, 679, 679);
	SET_ID(17);
	draw_rectv16(2048, 0, 0, 2048, 0, 679
		, -2048, 0, 679, -2048, 0, 0);
	SET_ID(18);
	draw_rectv16(2048, 1359, 1359, 2048, 679, 1359
		, -2048, 679, 1359, -2048, 1359, 1359);
	SET_ID(16);
	draw_rectv16(2048, 679, 679, 2048, 679, 1359
		, -2048, 679, 1359, -2048, 679, 679);
	SET_ID(17);
	draw_rectv16(2048, 2039, 2039, 2048, 1359, 2039
		, -2048, 1359, 2039, -2048, 2039, 2039);
	SET_ID(18);
	draw_rectv16(2048, 1359, 1359, 2048, 1359, 2039
		, -2048, 1359, 2039, -2048, 1359, 1359);
#endif
}

