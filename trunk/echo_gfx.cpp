// echo_gfx.cpp

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

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <echo_error.h>
#include <grid.h>
#include <echo_gfx.h>
#include <echo_math.h>
#include <cstdlib>
#include <iostream>

void draw_line(vector3f p1, vector3f p2)
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}

void draw_line(line3f ln)
{
	draw_line(ln.p1, ln.p2);
}

static int has_line(line3f* ptr, line3f line)
{
	return(ptr[0] == line || ptr[1] == line || ptr[2] == line || ptr[3] == line);
}

void draw_hole(vector3f pos)
{
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.03, pos.z);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, HALF_GRID);
	glVertex3f(HALF_GRID, 0, 0);
	glVertex3f(0, 0, -HALF_GRID);
	glVertex3f(-HALF_GRID, 0, 0);
	glEnd();
	glPopMatrix();
}

void draw_launcher(vector3f pos)
{
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.03, pos.z);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, HALF_GRID);
	glVertex3f(HALF_GRID, 0, 0);
	glVertex3f(0, 0, -HALF_GRID);
	glVertex3f(-HALF_GRID, 0, 0);
	glEnd();
	glPopMatrix();
}

void draw_circle(vector3f pos)
{
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.03, pos.z);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.4, 0.01, 8, 8);
	glPopMatrix();
}

void draw_n_lines(line3f* my_lines, vector3f angle, grid** others, int num_others)
{
	line3f** lines = new line3f*[num_others];
	CHKPTR(lines);
	int each = 0, line_count = 0;
	while(each < num_others)
	{
		if(others[each] && (lines[line_count] = others[each]->get_lines(angle)))
			line_count++;
		each++;
	}
	each = 0;
	int each_lines = 0;
	while(each < 4)
	{
		each_lines = 0;
		while(each_lines < line_count)
		{
			if(has_line(lines[each_lines], my_lines[each]))
				goto NODRAW;
			each_lines++;
		}
		draw_line(my_lines[each]);
	NODRAW:
		each++;
	}
	delete[] lines;
}

void draw_rect(vector3f p1, vector3f p2, vector3f p3, vector3f p4)
{
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void draw_goal_gfx(vector3f pos, float goal_angle)
{
	glColor3f(0.25f, 0.25f, 0.25f);
	glPushMatrix();
	glTranslatef(pos.x, pos.y + 0.5f, pos.z);
	glRotatef(goal_angle, 0, 1, 0);
	glutSolidTorus(0.05f, 0.4f, 8, 8);
	glPopMatrix();
}
