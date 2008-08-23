// echo_model_body.cpp

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


void draw_body()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.141f, 0.283f, 0.000f);
		glVertex3f(0.131f, 0.283f, 0.041f);
		glVertex3f(0.185f, 0.153f, 0.000f);
		glVertex3f(0.171f, 0.153f, 0.053f);
		glVertex3f(0.200f, 0.000f, 0.000f);
		glVertex3f(0.185f, 0.000f, 0.057f);
		glVertex3f(0.185f, -0.153f, 0.000f);
		glVertex3f(0.171f, -0.153f, 0.053f);
		glVertex3f(0.141f, -0.283f, 0.000f);
		glVertex3f(0.131f, -0.283f, 0.041f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.131f, 0.283f, 0.041f);
		glVertex3f(0.100f, 0.283f, 0.075f);
		glVertex3f(0.171f, 0.153f, 0.053f);
		glVertex3f(0.131f, 0.153f, 0.098f);
		glVertex3f(0.185f, 0.000f, 0.057f);
		glVertex3f(0.141f, 0.000f, 0.106f);
		glVertex3f(0.171f, -0.153f, 0.053f);
		glVertex3f(0.131f, -0.153f, 0.098f);
		glVertex3f(0.131f, -0.283f, 0.041f);
		glVertex3f(0.100f, -0.283f, 0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.100f, 0.283f, 0.075f);
		glVertex3f(0.054f, 0.283f, 0.098f);
		glVertex3f(0.131f, 0.153f, 0.098f);
		glVertex3f(0.071f, 0.153f, 0.128f);
		glVertex3f(0.141f, 0.000f, 0.106f);
		glVertex3f(0.077f, 0.000f, 0.139f);
		glVertex3f(0.131f, -0.153f, 0.098f);
		glVertex3f(0.071f, -0.153f, 0.128f);
		glVertex3f(0.100f, -0.283f, 0.075f);
		glVertex3f(0.054f, -0.283f, 0.098f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.054f, 0.283f, 0.098f);
		glVertex3f(0.000f, 0.283f, 0.106f);
		glVertex3f(0.071f, 0.153f, 0.128f);
		glVertex3f(0.000f, 0.153f, 0.139f);
		glVertex3f(0.077f, 0.000f, 0.139f);
		glVertex3f(0.000f, 0.000f, 0.150f);
		glVertex3f(0.071f, -0.153f, 0.128f);
		glVertex3f(0.000f, -0.153f, 0.139f);
		glVertex3f(0.054f, -0.283f, 0.098f);
		glVertex3f(0.000f, -0.283f, 0.106f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.283f, 0.106f);
		glVertex3f(-0.054f, 0.283f, 0.098f);
		glVertex3f(0.000f, 0.153f, 0.139f);
		glVertex3f(-0.071f, 0.153f, 0.128f);
		glVertex3f(0.000f, 0.000f, 0.150f);
		glVertex3f(-0.077f, 0.000f, 0.139f);
		glVertex3f(0.000f, -0.153f, 0.139f);
		glVertex3f(-0.071f, -0.153f, 0.128f);
		glVertex3f(0.000f, -0.283f, 0.106f);
		glVertex3f(-0.054f, -0.283f, 0.098f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.054f, 0.283f, 0.098f);
		glVertex3f(-0.100f, 0.283f, 0.075f);
		glVertex3f(-0.071f, 0.153f, 0.128f);
		glVertex3f(-0.131f, 0.153f, 0.098f);
		glVertex3f(-0.077f, 0.000f, 0.139f);
		glVertex3f(-0.141f, 0.000f, 0.106f);
		glVertex3f(-0.071f, -0.153f, 0.128f);
		glVertex3f(-0.131f, -0.153f, 0.098f);
		glVertex3f(-0.054f, -0.283f, 0.098f);
		glVertex3f(-0.100f, -0.283f, 0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.100f, 0.283f, 0.075f);
		glVertex3f(-0.131f, 0.283f, 0.041f);
		glVertex3f(-0.131f, 0.153f, 0.098f);
		glVertex3f(-0.171f, 0.153f, 0.053f);
		glVertex3f(-0.141f, 0.000f, 0.106f);
		glVertex3f(-0.185f, 0.000f, 0.057f);
		glVertex3f(-0.131f, -0.153f, 0.098f);
		glVertex3f(-0.171f, -0.153f, 0.053f);
		glVertex3f(-0.100f, -0.283f, 0.075f);
		glVertex3f(-0.131f, -0.283f, 0.041f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.131f, 0.283f, 0.041f);
		glVertex3f(-0.141f, 0.283f, 0.000f);
		glVertex3f(-0.171f, 0.153f, 0.053f);
		glVertex3f(-0.185f, 0.153f, 0.000f);
		glVertex3f(-0.185f, 0.000f, 0.057f);
		glVertex3f(-0.200f, 0.000f, 0.000f);
		glVertex3f(-0.171f, -0.153f, 0.053f);
		glVertex3f(-0.185f, -0.153f, 0.000f);
		glVertex3f(-0.131f, -0.283f, 0.041f);
		glVertex3f(-0.141f, -0.283f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.141f, 0.283f, 0.000f);
		glVertex3f(-0.131f, 0.283f, -0.041f);
		glVertex3f(-0.185f, 0.153f, 0.000f);
		glVertex3f(-0.171f, 0.153f, -0.053f);
		glVertex3f(-0.200f, 0.000f, 0.000f);
		glVertex3f(-0.185f, 0.000f, -0.057f);
		glVertex3f(-0.185f, -0.153f, 0.000f);
		glVertex3f(-0.171f, -0.153f, -0.053f);
		glVertex3f(-0.141f, -0.283f, 0.000f);
		glVertex3f(-0.131f, -0.283f, -0.041f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.131f, 0.283f, -0.041f);
		glVertex3f(-0.100f, 0.283f, -0.075f);
		glVertex3f(-0.171f, 0.153f, -0.053f);
		glVertex3f(-0.131f, 0.153f, -0.098f);
		glVertex3f(-0.185f, 0.000f, -0.057f);
		glVertex3f(-0.141f, 0.000f, -0.106f);
		glVertex3f(-0.171f, -0.153f, -0.053f);
		glVertex3f(-0.131f, -0.153f, -0.098f);
		glVertex3f(-0.131f, -0.283f, -0.041f);
		glVertex3f(-0.100f, -0.283f, -0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.100f, 0.283f, -0.075f);
		glVertex3f(-0.054f, 0.283f, -0.098f);
		glVertex3f(-0.131f, 0.153f, -0.098f);
		glVertex3f(-0.071f, 0.153f, -0.128f);
		glVertex3f(-0.141f, 0.000f, -0.106f);
		glVertex3f(-0.077f, 0.000f, -0.139f);
		glVertex3f(-0.131f, -0.153f, -0.098f);
		glVertex3f(-0.071f, -0.153f, -0.128f);
		glVertex3f(-0.100f, -0.283f, -0.075f);
		glVertex3f(-0.054f, -0.283f, -0.098f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.054f, 0.283f, -0.098f);
		glVertex3f(-0.000f, 0.283f, -0.106f);
		glVertex3f(-0.071f, 0.153f, -0.128f);
		glVertex3f(-0.000f, 0.153f, -0.139f);
		glVertex3f(-0.077f, 0.000f, -0.139f);
		glVertex3f(-0.000f, 0.000f, -0.150f);
		glVertex3f(-0.071f, -0.153f, -0.128f);
		glVertex3f(-0.000f, -0.153f, -0.139f);
		glVertex3f(-0.054f, -0.283f, -0.098f);
		glVertex3f(-0.000f, -0.283f, -0.106f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.283f, -0.106f);
		glVertex3f(0.054f, 0.283f, -0.098f);
		glVertex3f(-0.000f, 0.153f, -0.139f);
		glVertex3f(0.071f, 0.153f, -0.128f);
		glVertex3f(-0.000f, 0.000f, -0.150f);
		glVertex3f(0.077f, 0.000f, -0.139f);
		glVertex3f(-0.000f, -0.153f, -0.139f);
		glVertex3f(0.071f, -0.153f, -0.128f);
		glVertex3f(-0.000f, -0.283f, -0.106f);
		glVertex3f(0.054f, -0.283f, -0.098f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.054f, 0.283f, -0.098f);
		glVertex3f(0.100f, 0.283f, -0.075f);
		glVertex3f(0.071f, 0.153f, -0.128f);
		glVertex3f(0.131f, 0.153f, -0.098f);
		glVertex3f(0.077f, 0.000f, -0.139f);
		glVertex3f(0.141f, 0.000f, -0.106f);
		glVertex3f(0.071f, -0.153f, -0.128f);
		glVertex3f(0.131f, -0.153f, -0.098f);
		glVertex3f(0.054f, -0.283f, -0.098f);
		glVertex3f(0.100f, -0.283f, -0.075f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.100f, 0.283f, -0.075f);
		glVertex3f(0.131f, 0.283f, -0.041f);
		glVertex3f(0.131f, 0.153f, -0.098f);
		glVertex3f(0.171f, 0.153f, -0.053f);
		glVertex3f(0.141f, 0.000f, -0.106f);
		glVertex3f(0.185f, 0.000f, -0.057f);
		glVertex3f(0.131f, -0.153f, -0.098f);
		glVertex3f(0.171f, -0.153f, -0.053f);
		glVertex3f(0.100f, -0.283f, -0.075f);
		glVertex3f(0.131f, -0.283f, -0.041f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.131f, 0.283f, -0.041f);
		glVertex3f(0.141f, 0.283f, -0.000f);
		glVertex3f(0.171f, 0.153f, -0.053f);
		glVertex3f(0.185f, 0.153f, -0.000f);
		glVertex3f(0.185f, 0.000f, -0.057f);
		glVertex3f(0.200f, 0.000f, -0.000f);
		glVertex3f(0.171f, -0.153f, -0.053f);
		glVertex3f(0.185f, -0.153f, -0.000f);
		glVertex3f(0.131f, -0.283f, -0.041f);
		glVertex3f(0.141f, -0.283f, -0.000f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(579, 1158, 0);
		glVertex3v16(535, 1158, 166);
		glVertex3v16(756, 626, 0);
		glVertex3v16(699, 626, 217);
		glVertex3v16(819, 0, 0);
		glVertex3v16(756, 0, 235);
		glVertex3v16(756, -626, 0);
		glVertex3v16(699, -626, 217);
		glVertex3v16(579, -1158, 0);
		glVertex3v16(535, -1158, 166);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(535, 1158, 166);
		glVertex3v16(409, 1158, 307);
		glVertex3v16(699, 626, 217);
		glVertex3v16(535, 626, 401);
		glVertex3v16(756, 0, 235);
		glVertex3v16(579, 0, 434);
		glVertex3v16(699, -626, 217);
		glVertex3v16(535, -626, 401);
		glVertex3v16(535, -1158, 166);
		glVertex3v16(409, -1158, 307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(409, 1158, 307);
		glVertex3v16(221, 1158, 401);
		glVertex3v16(535, 626, 401);
		glVertex3v16(289, 626, 524);
		glVertex3v16(579, 0, 434);
		glVertex3v16(313, 0, 567);
		glVertex3v16(535, -626, 401);
		glVertex3v16(289, -626, 524);
		glVertex3v16(409, -1158, 307);
		glVertex3v16(221, -1158, 401);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(221, 1158, 401);
		glVertex3v16(0, 1158, 434);
		glVertex3v16(289, 626, 524);
		glVertex3v16(0, 626, 567);
		glVertex3v16(313, 0, 567);
		glVertex3v16(0, 0, 614);
		glVertex3v16(289, -626, 524);
		glVertex3v16(0, -626, 567);
		glVertex3v16(221, -1158, 401);
		glVertex3v16(0, -1158, 434);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1158, 434);
		glVertex3v16(-221, 1158, 401);
		glVertex3v16(0, 626, 567);
		glVertex3v16(-289, 626, 524);
		glVertex3v16(0, 0, 614);
		glVertex3v16(-313, 0, 567);
		glVertex3v16(0, -626, 567);
		glVertex3v16(-289, -626, 524);
		glVertex3v16(0, -1158, 434);
		glVertex3v16(-221, -1158, 401);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-221, 1158, 401);
		glVertex3v16(-409, 1158, 307);
		glVertex3v16(-289, 626, 524);
		glVertex3v16(-535, 626, 401);
		glVertex3v16(-313, 0, 567);
		glVertex3v16(-579, 0, 434);
		glVertex3v16(-289, -626, 524);
		glVertex3v16(-535, -626, 401);
		glVertex3v16(-221, -1158, 401);
		glVertex3v16(-409, -1158, 307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-409, 1158, 307);
		glVertex3v16(-535, 1158, 166);
		glVertex3v16(-535, 626, 401);
		glVertex3v16(-699, 626, 217);
		glVertex3v16(-579, 0, 434);
		glVertex3v16(-756, 0, 235);
		glVertex3v16(-535, -626, 401);
		glVertex3v16(-699, -626, 217);
		glVertex3v16(-409, -1158, 307);
		glVertex3v16(-535, -1158, 166);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-535, 1158, 166);
		glVertex3v16(-579, 1158, 0);
		glVertex3v16(-699, 626, 217);
		glVertex3v16(-756, 626, 0);
		glVertex3v16(-756, 0, 235);
		glVertex3v16(-819, 0, 0);
		glVertex3v16(-699, -626, 217);
		glVertex3v16(-756, -626, 0);
		glVertex3v16(-535, -1158, 166);
		glVertex3v16(-579, -1158, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-579, 1158, 0);
		glVertex3v16(-535, 1158, -166);
		glVertex3v16(-756, 626, 0);
		glVertex3v16(-699, 626, -217);
		glVertex3v16(-819, 0, 0);
		glVertex3v16(-756, 0, -235);
		glVertex3v16(-756, -626, 0);
		glVertex3v16(-699, -626, -217);
		glVertex3v16(-579, -1158, 0);
		glVertex3v16(-535, -1158, -166);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-535, 1158, -166);
		glVertex3v16(-409, 1158, -307);
		glVertex3v16(-699, 626, -217);
		glVertex3v16(-535, 626, -401);
		glVertex3v16(-756, 0, -235);
		glVertex3v16(-579, 0, -434);
		glVertex3v16(-699, -626, -217);
		glVertex3v16(-535, -626, -401);
		glVertex3v16(-535, -1158, -166);
		glVertex3v16(-409, -1158, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-409, 1158, -307);
		glVertex3v16(-221, 1158, -401);
		glVertex3v16(-535, 626, -401);
		glVertex3v16(-289, 626, -524);
		glVertex3v16(-579, 0, -434);
		glVertex3v16(-313, 0, -567);
		glVertex3v16(-535, -626, -401);
		glVertex3v16(-289, -626, -524);
		glVertex3v16(-409, -1158, -307);
		glVertex3v16(-221, -1158, -401);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-221, 1158, -401);
		glVertex3v16(0, 1158, -434);
		glVertex3v16(-289, 626, -524);
		glVertex3v16(0, 626, -567);
		glVertex3v16(-313, 0, -567);
		glVertex3v16(0, 0, -614);
		glVertex3v16(-289, -626, -524);
		glVertex3v16(0, -626, -567);
		glVertex3v16(-221, -1158, -401);
		glVertex3v16(0, -1158, -434);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1158, -434);
		glVertex3v16(221, 1158, -401);
		glVertex3v16(0, 626, -567);
		glVertex3v16(289, 626, -524);
		glVertex3v16(0, 0, -614);
		glVertex3v16(313, 0, -567);
		glVertex3v16(0, -626, -567);
		glVertex3v16(289, -626, -524);
		glVertex3v16(0, -1158, -434);
		glVertex3v16(221, -1158, -401);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(221, 1158, -401);
		glVertex3v16(409, 1158, -307);
		glVertex3v16(289, 626, -524);
		glVertex3v16(535, 626, -401);
		glVertex3v16(313, 0, -567);
		glVertex3v16(579, 0, -434);
		glVertex3v16(289, -626, -524);
		glVertex3v16(535, -626, -401);
		glVertex3v16(221, -1158, -401);
		glVertex3v16(409, -1158, -307);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(409, 1158, -307);
		glVertex3v16(535, 1158, -166);
		glVertex3v16(535, 626, -401);
		glVertex3v16(699, 626, -217);
		glVertex3v16(579, 0, -434);
		glVertex3v16(756, 0, -235);
		glVertex3v16(535, -626, -401);
		glVertex3v16(699, -626, -217);
		glVertex3v16(409, -1158, -307);
		glVertex3v16(535, -1158, -166);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(535, 1158, -166);
		glVertex3v16(579, 1158, 0);
		glVertex3v16(699, 626, -217);
		glVertex3v16(756, 626, 0);
		glVertex3v16(756, 0, -235);
		glVertex3v16(819, 0, 0);
		glVertex3v16(699, -626, -217);
		glVertex3v16(756, -626, 0);
		glVertex3v16(535, -1158, -166);
		glVertex3v16(579, -1158, 0);
	glEnd();
#endif
}

