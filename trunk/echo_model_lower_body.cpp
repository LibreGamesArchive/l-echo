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


void draw_lower_body()
{
#ifndef ECHO_NDS
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.100f, 0.260f, 0.000f);
		glVertex3f(0.092f, 0.260f, 0.029f);
		glVertex3f(0.159f, 0.183f, 0.000f);
		glVertex3f(0.147f, 0.183f, 0.046f);
		glVertex3f(0.193f, 0.078f, 0.000f);
		glVertex3f(0.178f, 0.078f, 0.055f);
		glVertex3f(0.198f, -0.039f, 0.000f);
		glVertex3f(0.183f, -0.039f, 0.057f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.092f, 0.260f, 0.029f);
		glVertex3f(0.071f, 0.260f, 0.053f);
		glVertex3f(0.147f, 0.183f, 0.046f);
		glVertex3f(0.112f, 0.183f, 0.084f);
		glVertex3f(0.178f, 0.078f, 0.055f);
		glVertex3f(0.137f, 0.078f, 0.102f);
		glVertex3f(0.183f, -0.039f, 0.057f);
		glVertex3f(0.140f, -0.039f, 0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.071f, 0.260f, 0.053f);
		glVertex3f(0.038f, 0.260f, 0.069f);
		glVertex3f(0.112f, 0.183f, 0.084f);
		glVertex3f(0.061f, 0.183f, 0.110f);
		glVertex3f(0.137f, 0.078f, 0.102f);
		glVertex3f(0.074f, 0.078f, 0.134f);
		glVertex3f(0.140f, -0.039f, 0.105f);
		glVertex3f(0.076f, -0.039f, 0.137f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.038f, 0.260f, 0.069f);
		glVertex3f(0.000f, 0.260f, 0.075f);
		glVertex3f(0.061f, 0.183f, 0.110f);
		glVertex3f(0.000f, 0.183f, 0.119f);
		glVertex3f(0.074f, 0.078f, 0.134f);
		glVertex3f(0.000f, 0.078f, 0.145f);
		glVertex3f(0.076f, -0.039f, 0.137f);
		glVertex3f(0.000f, -0.039f, 0.149f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.000f, 0.260f, 0.075f);
		glVertex3f(-0.038f, 0.260f, 0.069f);
		glVertex3f(0.000f, 0.183f, 0.119f);
		glVertex3f(-0.061f, 0.183f, 0.110f);
		glVertex3f(0.000f, 0.078f, 0.145f);
		glVertex3f(-0.074f, 0.078f, 0.134f);
		glVertex3f(0.000f, -0.039f, 0.149f);
		glVertex3f(-0.076f, -0.039f, 0.137f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.038f, 0.260f, 0.069f);
		glVertex3f(-0.071f, 0.260f, 0.053f);
		glVertex3f(-0.061f, 0.183f, 0.110f);
		glVertex3f(-0.112f, 0.183f, 0.084f);
		glVertex3f(-0.074f, 0.078f, 0.134f);
		glVertex3f(-0.137f, 0.078f, 0.102f);
		glVertex3f(-0.076f, -0.039f, 0.137f);
		glVertex3f(-0.140f, -0.039f, 0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.071f, 0.260f, 0.053f);
		glVertex3f(-0.092f, 0.260f, 0.029f);
		glVertex3f(-0.112f, 0.183f, 0.084f);
		glVertex3f(-0.147f, 0.183f, 0.046f);
		glVertex3f(-0.137f, 0.078f, 0.102f);
		glVertex3f(-0.178f, 0.078f, 0.055f);
		glVertex3f(-0.140f, -0.039f, 0.105f);
		glVertex3f(-0.183f, -0.039f, 0.057f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.092f, 0.260f, 0.029f);
		glVertex3f(-0.100f, 0.260f, 0.000f);
		glVertex3f(-0.147f, 0.183f, 0.046f);
		glVertex3f(-0.159f, 0.183f, 0.000f);
		glVertex3f(-0.178f, 0.078f, 0.055f);
		glVertex3f(-0.193f, 0.078f, 0.000f);
		glVertex3f(-0.183f, -0.039f, 0.057f);
		glVertex3f(-0.198f, -0.039f, 0.000f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.100f, 0.260f, 0.000f);
		glVertex3f(-0.092f, 0.260f, -0.029f);
		glVertex3f(-0.159f, 0.183f, 0.000f);
		glVertex3f(-0.147f, 0.183f, -0.046f);
		glVertex3f(-0.193f, 0.078f, 0.000f);
		glVertex3f(-0.178f, 0.078f, -0.055f);
		glVertex3f(-0.198f, -0.039f, 0.000f);
		glVertex3f(-0.183f, -0.039f, -0.057f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.092f, 0.260f, -0.029f);
		glVertex3f(-0.071f, 0.260f, -0.053f);
		glVertex3f(-0.147f, 0.183f, -0.046f);
		glVertex3f(-0.112f, 0.183f, -0.084f);
		glVertex3f(-0.178f, 0.078f, -0.055f);
		glVertex3f(-0.137f, 0.078f, -0.102f);
		glVertex3f(-0.183f, -0.039f, -0.057f);
		glVertex3f(-0.140f, -0.039f, -0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.071f, 0.260f, -0.053f);
		glVertex3f(-0.038f, 0.260f, -0.069f);
		glVertex3f(-0.112f, 0.183f, -0.084f);
		glVertex3f(-0.061f, 0.183f, -0.110f);
		glVertex3f(-0.137f, 0.078f, -0.102f);
		glVertex3f(-0.074f, 0.078f, -0.134f);
		glVertex3f(-0.140f, -0.039f, -0.105f);
		glVertex3f(-0.076f, -0.039f, -0.137f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.038f, 0.260f, -0.069f);
		glVertex3f(-0.000f, 0.260f, -0.075f);
		glVertex3f(-0.061f, 0.183f, -0.110f);
		glVertex3f(-0.000f, 0.183f, -0.119f);
		glVertex3f(-0.074f, 0.078f, -0.134f);
		glVertex3f(-0.000f, 0.078f, -0.145f);
		glVertex3f(-0.076f, -0.039f, -0.137f);
		glVertex3f(-0.000f, -0.039f, -0.149f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-0.000f, 0.260f, -0.075f);
		glVertex3f(0.038f, 0.260f, -0.069f);
		glVertex3f(-0.000f, 0.183f, -0.119f);
		glVertex3f(0.061f, 0.183f, -0.110f);
		glVertex3f(-0.000f, 0.078f, -0.145f);
		glVertex3f(0.074f, 0.078f, -0.134f);
		glVertex3f(-0.000f, -0.039f, -0.149f);
		glVertex3f(0.076f, -0.039f, -0.137f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.038f, 0.260f, -0.069f);
		glVertex3f(0.071f, 0.260f, -0.053f);
		glVertex3f(0.061f, 0.183f, -0.110f);
		glVertex3f(0.112f, 0.183f, -0.084f);
		glVertex3f(0.074f, 0.078f, -0.134f);
		glVertex3f(0.137f, 0.078f, -0.102f);
		glVertex3f(0.076f, -0.039f, -0.137f);
		glVertex3f(0.140f, -0.039f, -0.105f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.071f, 0.260f, -0.053f);
		glVertex3f(0.092f, 0.260f, -0.029f);
		glVertex3f(0.112f, 0.183f, -0.084f);
		glVertex3f(0.147f, 0.183f, -0.046f);
		glVertex3f(0.137f, 0.078f, -0.102f);
		glVertex3f(0.178f, 0.078f, -0.055f);
		glVertex3f(0.140f, -0.039f, -0.105f);
		glVertex3f(0.183f, -0.039f, -0.057f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.092f, 0.260f, -0.029f);
		glVertex3f(0.100f, 0.260f, -0.000f);
		glVertex3f(0.147f, 0.183f, -0.046f);
		glVertex3f(0.159f, 0.183f, -0.000f);
		glVertex3f(0.178f, 0.078f, -0.055f);
		glVertex3f(0.193f, 0.078f, -0.000f);
		glVertex3f(0.183f, -0.039f, -0.057f);
		glVertex3f(0.198f, -0.039f, -0.000f);
	glEnd();
#else
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(409, 1064, 0);
		glVertex3v16(378, 1064, 117);
		glVertex3v16(649, 748, 0);
		glVertex3v16(600, 748, 186);
		glVertex3v16(791, 318, 0);
		glVertex3v16(731, 318, 227);
		glVertex3v16(812, -160, 0);
		glVertex3v16(750, -160, 233);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(378, 1064, 117);
		glVertex3v16(289, 1064, 217);
		glVertex3v16(600, 748, 186);
		glVertex3v16(459, 748, 344);
		glVertex3v16(731, 318, 227);
		glVertex3v16(559, 318, 419);
		glVertex3v16(750, -160, 233);
		glVertex3v16(574, -160, 430);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(289, 1064, 217);
		glVertex3v16(156, 1064, 283);
		glVertex3v16(459, 748, 344);
		glVertex3v16(248, 748, 450);
		glVertex3v16(559, 318, 419);
		glVertex3v16(302, 318, 548);
		glVertex3v16(574, -160, 430);
		glVertex3v16(310, -160, 562);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(156, 1064, 283);
		glVertex3v16(0, 1064, 307);
		glVertex3v16(248, 748, 450);
		glVertex3v16(0, 748, 487);
		glVertex3v16(302, 318, 548);
		glVertex3v16(0, 318, 593);
		glVertex3v16(310, -160, 562);
		glVertex3v16(0, -160, 609);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1064, 307);
		glVertex3v16(-156, 1064, 283);
		glVertex3v16(0, 748, 487);
		glVertex3v16(-248, 748, 450);
		glVertex3v16(0, 318, 593);
		glVertex3v16(-302, 318, 548);
		glVertex3v16(0, -160, 609);
		glVertex3v16(-310, -160, 562);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-156, 1064, 283);
		glVertex3v16(-289, 1064, 217);
		glVertex3v16(-248, 748, 450);
		glVertex3v16(-459, 748, 344);
		glVertex3v16(-302, 318, 548);
		glVertex3v16(-559, 318, 419);
		glVertex3v16(-310, -160, 562);
		glVertex3v16(-574, -160, 430);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-289, 1064, 217);
		glVertex3v16(-378, 1064, 117);
		glVertex3v16(-459, 748, 344);
		glVertex3v16(-600, 748, 186);
		glVertex3v16(-559, 318, 419);
		glVertex3v16(-731, 318, 227);
		glVertex3v16(-574, -160, 430);
		glVertex3v16(-750, -160, 233);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-378, 1064, 117);
		glVertex3v16(-409, 1064, 0);
		glVertex3v16(-600, 748, 186);
		glVertex3v16(-649, 748, 0);
		glVertex3v16(-731, 318, 227);
		glVertex3v16(-791, 318, 0);
		glVertex3v16(-750, -160, 233);
		glVertex3v16(-812, -160, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-409, 1064, 0);
		glVertex3v16(-378, 1064, -117);
		glVertex3v16(-649, 748, 0);
		glVertex3v16(-600, 748, -186);
		glVertex3v16(-791, 318, 0);
		glVertex3v16(-731, 318, -227);
		glVertex3v16(-812, -160, 0);
		glVertex3v16(-750, -160, -233);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-378, 1064, -117);
		glVertex3v16(-289, 1064, -217);
		glVertex3v16(-600, 748, -186);
		glVertex3v16(-459, 748, -344);
		glVertex3v16(-731, 318, -227);
		glVertex3v16(-559, 318, -419);
		glVertex3v16(-750, -160, -233);
		glVertex3v16(-574, -160, -430);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-289, 1064, -217);
		glVertex3v16(-156, 1064, -283);
		glVertex3v16(-459, 748, -344);
		glVertex3v16(-248, 748, -450);
		glVertex3v16(-559, 318, -419);
		glVertex3v16(-302, 318, -548);
		glVertex3v16(-574, -160, -430);
		glVertex3v16(-310, -160, -562);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(-156, 1064, -283);
		glVertex3v16(0, 1064, -307);
		glVertex3v16(-248, 748, -450);
		glVertex3v16(0, 748, -487);
		glVertex3v16(-302, 318, -548);
		glVertex3v16(0, 318, -593);
		glVertex3v16(-310, -160, -562);
		glVertex3v16(0, -160, -609);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(0, 1064, -307);
		glVertex3v16(156, 1064, -283);
		glVertex3v16(0, 748, -487);
		glVertex3v16(248, 748, -450);
		glVertex3v16(0, 318, -593);
		glVertex3v16(302, 318, -548);
		glVertex3v16(0, -160, -609);
		glVertex3v16(310, -160, -562);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(156, 1064, -283);
		glVertex3v16(289, 1064, -217);
		glVertex3v16(248, 748, -450);
		glVertex3v16(459, 748, -344);
		glVertex3v16(302, 318, -548);
		glVertex3v16(559, 318, -419);
		glVertex3v16(310, -160, -562);
		glVertex3v16(574, -160, -430);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(289, 1064, -217);
		glVertex3v16(378, 1064, -117);
		glVertex3v16(459, 748, -344);
		glVertex3v16(600, 748, -186);
		glVertex3v16(559, 318, -419);
		glVertex3v16(731, 318, -227);
		glVertex3v16(574, -160, -430);
		glVertex3v16(750, -160, -233);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3v16(378, 1064, -117);
		glVertex3v16(409, 1064, 0);
		glVertex3v16(600, 748, -186);
		glVertex3v16(649, 748, 0);
		glVertex3v16(731, 318, -227);
		glVertex3v16(791, 318, 0);
		glVertex3v16(750, -160, -233);
		glVertex3v16(812, -160, 0);
	glEnd();
#endif
}

