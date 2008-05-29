// main.cpp

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

#include <cstdlib>
#include <iostream>
#include <cmath>

#include <hole.h>
#include <grid.h>
#include <escgrid.h>
#include <static_grid.h>
#include <isect_grid.h>
#include <echo_math.h>
#include <echo_loader.h>
#include <echo_ns.h>
#include <echo_sys.h>
#include <t_grid.h>
#include <echo_stage.h>

#define _STDCALL_SUPPORTED

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define ESCAPE 27

#define MSG_READY "ready"
#define MSG_START "start"
#define MSG_PAUSE "thinking"
#define MSG_BLANK ""

#define COUNTER_HEAD    "goals: %i"
#define SUCCESS         "success"
#define	NO_GOALS	"no goals"

#define START_MAX           50
#define NAME_DISPLAY_MAX    30

static int window;
static int my_width, my_height;
static int start_frame, name_display = NAME_DISPLAY_MAX;
static vector3f esc_angle1(-45, 90, 0), esc_angle2(-45, 180, 0);
static char* message = MSG_READY;
static char* counter;

void init(int argc, char **argv, int w, int h);
void resize(int w, int h);
void display();
void key(unsigned char key, int x, int y);
void spec_key(int key, int x, int y);
static void set_proj(int w, int h);

int main(int argc, char **argv)
{
	init_math();
	
	char* fname = argc >= 2 ? argv[1] : const_cast<char*>("sample1.xml");
	echo_ns::init(load_stage(fname));
	
	/*
	vector3f* vec = new vector3f(1, 2, 2);
	vector3f* angle = vec->angle_xy();
	angle->dump();
	std::cout << std::endl;
	vector3f* norm = vec->neg_rotate_yx(*angle);
	norm->dump();
	std::cout << std::endl;
	vector3f* back = norm->rotate_xy(*angle);
	back->dump();
	std::cout << std::endl;
	// */
	
	init(argc, argv, 640, 480);
	glutMainLoop();
	return(1);
}

void init(int argc, char **argv, int w, int h)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("L-Echo");
	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&key);
	glutSpecialFunc(&spec_key);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glLineWidth(2.5);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
	glEnable(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_COLOR);
	glShadeModel(GL_FLAT);
	// */
	
	set_proj(w, h);
}

void resize(int w, int h)
{
	if(h == 0)	h = 1;
	
	glViewport(0, 0, w, h);
	
	set_proj(w, h);
}

static void set_proj(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	my_width = w;
	my_height = h;
	
	if (w <= h)	glOrtho(-5.0, 5.0, -5.0 * (GLfloat) h / (GLfloat) w, 5.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else		glOrtho(-5.0 * (GLfloat) w / (GLfloat) h, 5.0 * (GLfloat) w / (GLfloat) h, -5.0, 5.0, -10.0, 10.0);
	
	glMatrixMode(GL_MODELVIEW);
}

//copied from http://lighthouse3d.com/opengl/glut/index.php?bmpfontortho
void draw_message_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) / 18.0f;
		c++;
	}
}

void draw_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) / 36.0f;
		c++;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glColor3f(0, 0, 0);
	
	if(message == MSG_START)
	{
		glColor4f(0, 0, 0, 1.0f - start_frame * 1.0f / START_MAX);
		if(start_frame > START_MAX)
			message = MSG_BLANK;
		else
			start_frame++;
	}
	else if(message == MSG_PAUSE)
	{
		glColor4f(0, 0, 0, 0.4f * sin(TO_RAD(start_frame)) + 0.6f);
		start_frame += 10;
	}
	
	draw_message_string(-4, -4.5f, message);
	
	if(name_display > 0)
	{
		glColor3f(0, 0, 0);
		
		if(name_display < NAME_DISPLAY_MAX)
		{
			glColor4f(0, 0, 0, name_display * 1.0f / NAME_DISPLAY_MAX);
			name_display--;
		}
		//std::cout << my_stage->get_name() << std::endl;
		draw_string(-5, 0, const_cast<char*>(echo_ns::current_stage->get_name().c_str()));
	}
	
	int goals_left = echo_ns::goals_left();
	if(goals_left > 0)
	{
		//Yeah, I really should check the number of chars I need...
		counter = new char[(int)log(goals_left) + 10];
		sprintf(counter, COUNTER_HEAD, goals_left);
	}
	else if(echo_ns::num_goals())
		counter = SUCCESS;
	else
		counter = NO_GOALS;
	
	glColor3f(0, 0, 0);
	draw_string(-3, -4, counter);
	
	glRotatef(-echo_ns::angle.x, 1, 0, 0);
	glRotatef(-echo_ns::angle.y, 0, 1, 0);
	
	echo_ns::draw();
	
	vector3f* vec = echo_ns::step_char();
	if(vec)
	{
		/*
		vec->dump();
		std::cout << std::endl;
		// */
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(vec->x, vec->y + 0.25, vec->z);
		glutSolidSphere(0.1, 8, 8);
	}
	
	glutSwapBuffers();
	
	echo_sleep(30000);
}

void key(unsigned char key, int x, int y)
{
	//echo_sleep(100);
	if(key == ESCAPE)
	{
		glutDestroyWindow(window);
		exit(0);
	}
	else if(key == 'p' || key == 'P')
	{
		start_frame = 0;
		if(message == MSG_READY)
		{
			message = MSG_START;
			echo_ns::start();
			name_display--;
		}
		else
		{
			if(!echo_ns::is_paused())
				message = MSG_PAUSE;
			else
				message = MSG_BLANK;
			echo_ns::toggle_pause();
		}
	}
	else if(key == 'a' || key == 'A')
	{
		std::cout << "ang: ";
		echo_ns::angle.dump();
		std::cout << std::endl;
	}
}

#define ROTATE_ANG	5.0f

void spec_key(int key, int x, int y)
{
	//echo_sleep(100);
	if(key == GLUT_KEY_RIGHT)
	{
		echo_ns::angle.y += ROTATE_ANG;
		if(echo_ns::angle.y > 360)	echo_ns::angle.y -= 360;
	}
	else if(key == GLUT_KEY_LEFT)
	{
		echo_ns::angle.y -= ROTATE_ANG;
		if(echo_ns::angle.y < -360)	echo_ns::angle.y += 360;
	}
	else if(key == GLUT_KEY_DOWN)
	{
		if(echo_ns::angle.x < 60)
			echo_ns::angle.x += ROTATE_ANG;
		//if(echo_ns::angle.x > 360)	echo_ns::angle.x -= 360;
	}
	else if(key == GLUT_KEY_UP)
	{
		if(echo_ns::angle.x > -60)
			echo_ns::angle.x -= ROTATE_ANG;
		//if(echo_ns::angle.x < -360)	echo_ns::angle.x += 360;
	}
}

