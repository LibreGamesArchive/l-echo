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


//stats: LATS: 6, ANGLE_INCR: 0.523599, NDS_LATS: 2, NDS_ANGLE_INCR: 1.570796
void draw_body()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.118f, 0.324f, 0.000f);
		glVertex3f(0.102f, 0.324f, 0.044f);
		glVertex3f(0.183f, 0.163f, 0.000f);
		glVertex3f(0.158f, 0.163f, 0.069f);
		glVertex3f(0.199f, -0.042f, 0.000f);
		glVertex3f(0.172f, -0.042f, 0.075f);
		glVertex3f(0.162f, -0.235f, 0.000f);
		glVertex3f(0.140f, -0.235f, 0.061f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.102f, 0.324f, 0.044f);
		glVertex3f(0.059f, 0.324f, 0.076f);
		glVertex3f(0.158f, 0.163f, 0.069f);
		glVertex3f(0.091f, 0.163f, 0.119f);
		glVertex3f(0.172f, -0.042f, 0.075f);
		glVertex3f(0.099f, -0.042f, 0.129f);
		glVertex3f(0.140f, -0.235f, 0.061f);
		glVertex3f(0.081f, -0.235f, 0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.059f, 0.324f, 0.076f);
		glVertex3f(0.000f, 0.324f, 0.088f);
		glVertex3f(0.091f, 0.163f, 0.119f);
		glVertex3f(0.000f, 0.163f, 0.137f);
		glVertex3f(0.099f, -0.042f, 0.129f);
		glVertex3f(0.000f, -0.042f, 0.149f);
		glVertex3f(0.081f, -0.235f, 0.105f);
		glVertex3f(0.000f, -0.235f, 0.121f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.324f, 0.088f);
		glVertex3f(-0.059f, 0.324f, 0.076f);
		glVertex3f(0.000f, 0.163f, 0.137f);
		glVertex3f(-0.091f, 0.163f, 0.119f);
		glVertex3f(0.000f, -0.042f, 0.149f);
		glVertex3f(-0.099f, -0.042f, 0.129f);
		glVertex3f(0.000f, -0.235f, 0.121f);
		glVertex3f(-0.081f, -0.235f, 0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.059f, 0.324f, 0.076f);
		glVertex3f(-0.102f, 0.324f, 0.044f);
		glVertex3f(-0.091f, 0.163f, 0.119f);
		glVertex3f(-0.158f, 0.163f, 0.069f);
		glVertex3f(-0.099f, -0.042f, 0.129f);
		glVertex3f(-0.172f, -0.042f, 0.075f);
		glVertex3f(-0.081f, -0.235f, 0.105f);
		glVertex3f(-0.140f, -0.235f, 0.061f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.102f, 0.324f, 0.044f);
		glVertex3f(-0.118f, 0.324f, 0.000f);
		glVertex3f(-0.158f, 0.163f, 0.069f);
		glVertex3f(-0.183f, 0.163f, 0.000f);
		glVertex3f(-0.172f, -0.042f, 0.075f);
		glVertex3f(-0.199f, -0.042f, 0.000f);
		glVertex3f(-0.140f, -0.235f, 0.061f);
		glVertex3f(-0.162f, -0.235f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.118f, 0.324f, 0.000f);
		glVertex3f(-0.102f, 0.324f, -0.044f);
		glVertex3f(-0.183f, 0.163f, 0.000f);
		glVertex3f(-0.158f, 0.163f, -0.069f);
		glVertex3f(-0.199f, -0.042f, 0.000f);
		glVertex3f(-0.172f, -0.042f, -0.075f);
		glVertex3f(-0.162f, -0.235f, 0.000f);
		glVertex3f(-0.140f, -0.235f, -0.061f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.102f, 0.324f, -0.044f);
		glVertex3f(-0.059f, 0.324f, -0.076f);
		glVertex3f(-0.158f, 0.163f, -0.069f);
		glVertex3f(-0.091f, 0.163f, -0.119f);
		glVertex3f(-0.172f, -0.042f, -0.075f);
		glVertex3f(-0.099f, -0.042f, -0.129f);
		glVertex3f(-0.140f, -0.235f, -0.061f);
		glVertex3f(-0.081f, -0.235f, -0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.059f, 0.324f, -0.076f);
		glVertex3f(-0.000f, 0.324f, -0.088f);
		glVertex3f(-0.091f, 0.163f, -0.119f);
		glVertex3f(-0.000f, 0.163f, -0.137f);
		glVertex3f(-0.099f, -0.042f, -0.129f);
		glVertex3f(-0.000f, -0.042f, -0.149f);
		glVertex3f(-0.081f, -0.235f, -0.105f);
		glVertex3f(-0.000f, -0.235f, -0.121f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.324f, -0.088f);
		glVertex3f(0.059f, 0.324f, -0.076f);
		glVertex3f(-0.000f, 0.163f, -0.137f);
		glVertex3f(0.091f, 0.163f, -0.119f);
		glVertex3f(-0.000f, -0.042f, -0.149f);
		glVertex3f(0.099f, -0.042f, -0.129f);
		glVertex3f(-0.000f, -0.235f, -0.121f);
		glVertex3f(0.081f, -0.235f, -0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.059f, 0.324f, -0.076f);
		glVertex3f(0.102f, 0.324f, -0.044f);
		glVertex3f(0.091f, 0.163f, -0.119f);
		glVertex3f(0.158f, 0.163f, -0.069f);
		glVertex3f(0.099f, -0.042f, -0.129f);
		glVertex3f(0.172f, -0.042f, -0.075f);
		glVertex3f(0.081f, -0.235f, -0.105f);
		glVertex3f(0.140f, -0.235f, -0.061f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.102f, 0.324f, -0.044f);
		glVertex3f(0.118f, 0.324f, -0.000f);
		glVertex3f(0.158f, 0.163f, -0.069f);
		glVertex3f(0.183f, 0.163f, -0.000f);
		glVertex3f(0.172f, -0.042f, -0.075f);
		glVertex3f(0.199f, -0.042f, -0.000f);
		glVertex3f(0.140f, -0.235f, -0.061f);
		glVertex3f(0.162f, -0.235f, -0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.118f, 0.324f, -0.000f);
		glVertex3f(0.102f, 0.324f, 0.044f);
		glVertex3f(0.183f, 0.163f, -0.000f);
		glVertex3f(0.158f, 0.163f, 0.069f);
		glVertex3f(0.199f, -0.042f, -0.000f);
		glVertex3f(0.172f, -0.042f, 0.075f);
		glVertex3f(0.162f, -0.235f, -0.000f);
		glVertex3f(0.140f, -0.235f, 0.061f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(481, 1325, 0);
		glVertex3v16(0, 1325, 361);
		glVertex3v16(662, -963, 0);
		glVertex3v16(0, -963, 497);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1325, 361);
		glVertex3v16(-481, 1325, 0);
		glVertex3v16(0, -963, 497);
		glVertex3v16(-662, -963, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-481, 1325, 0);
		glVertex3v16(0, 1325, -361);
		glVertex3v16(-662, -963, 0);
		glVertex3v16(0, -963, -497);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1325, -361);
		glVertex3v16(481, 1325, 0);
		glVertex3v16(0, -963, -497);
		glVertex3v16(662, -963, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(481, 1325, 0);
		glVertex3v16(0, 1325, 361);
		glVertex3v16(662, -963, 0);
		glVertex3v16(0, -963, 497);
	glEnd();
#endif
}

