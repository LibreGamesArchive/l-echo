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

#include <libgen.h>
#include <unistd.h>

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
#include <echo_ingame_loader.h>

#define _STDCALL_SUPPORTED

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define ESCAPE 27
#define ENTER  13

#define MSG_READY "ready"
#define MSG_START "start"
#define MSG_PAUSE "thinking"
#define MSG_BLANK ""

#define COUNTER_HEAD    "goals: %i"
#define SUCCESS         "success"
#define	NO_GOALS	"no goals"

#define LOAD_MAX		10.0f
#define START_MAX           	50
#define NAME_DISPLAY_MAX    	30
#define FILE_SPACE		0.3f
#define NUM_FILES_DISPLAYED     31

static int window;
static int my_width, my_height;
static float real_width;
static int start_frame = 0, name_display = NAME_DISPLAY_MAX;
static int loading = 0, load_frame = 0, file_index = 0, file_start = 0;
static vector3f esc_angle1(-45, 90, 0), esc_angle2(-45, 180, 0);
static char* message = MSG_READY;
static char* counter;
static echo_files* files;

void load(const char* fname);
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
	load(fname);
	
	char* pwd = new char[4096];
	files = get_files(getcwd(pwd, 4096));
	//dump_files(files);
	
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

void load(const char* fname)
{
	start_frame = 0;
	name_display = NAME_DISPLAY_MAX;
	message = MSG_READY;
	echo_ns::init(load_stage(fname));
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
	
	if (w <= h)
	{
		glOrtho(-5.0, 5.0, -5.0 * (GLfloat) h / (GLfloat) w, 5.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
		real_width = 5.0f;
	}
	else
	{
		glOrtho(-5.0 * (GLfloat) w / (GLfloat) h, 5.0 * (GLfloat) w / (GLfloat) h, -5.0, 5.0, -10.0, 10.0);
		real_width = 5.0f * (GLfloat) w / (GLfloat) h;
	}
	
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

void draw_fname_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, *c) / 36.0f;
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
		//not very precise, but oh well
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
	
	if(loading || load_frame > 0)
	{
		//std::cout << "loading? " << std::endl;
		if(!loading)
			load_frame--;
		else if(load_frame < LOAD_MAX)
			load_frame++;
		
		float side_x = real_width * (1 - 1.5f * load_frame / LOAD_MAX);
		
		glLoadIdentity();
		glTranslatef(0, 0, 9.8f);
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		glVertex3f(side_x, 5, 0);
		glVertex3f(real_width, 5, 0);
		glVertex3f(real_width, -5, 0);
		glVertex3f(side_x, -5, 0);
		glEnd();
		
		glTranslatef(0, 0, 0.1f);
		glColor3f(1, 1, 1);
		
		draw_string(side_x
				, 5 - 0.4f, files->current_dir);
		
		glColor3f(0.5f, 0.5f, 0.5f);
		
		int each_file = 0;
		float each_y = 5 - 3 * FILE_SPACE;
		while(each_y >= -5 && each_file < NUM_FILES_DISPLAYED && each_file < files->num_files)
		{
			if(file_start + each_file == file_index)
			{
				glColor3f(1, 1, 1);
			}
			draw_fname_string(side_x
				, each_y, files->file_names[file_start + each_file]);
			if(file_start + each_file == file_index)
			{
				glColor3f(0.5f, 0.5f, 0.5f);
			}
			each_y -= FILE_SPACE;
			each_file++;
		}
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
	else if(key == ENTER)
	{
		const char* file = files->file_names[file_index];
		if(!is_dir(files->current_dir, file))
		{
			load(file);
		}
		else
		{
			//std::cout << "file is dir: " << file << ", " << files->current_dir << std::endl;
			char* dir;
			if(!strcmp(file, ".."))
			{
				dir = new char[strlen(files->current_dir)];
				strcpy(dir, dirname(files->current_dir));
			}
			else
			{
				dir = echo_merge(files->current_dir, file);
			}
			//std::cout << "newdir: " << dir << std::endl;
			delete files;
			files = get_files(dir);
			file_index = 0;
			file_start = 0;
		}
	}
	else if(key == 'l' || key == 'L')
	{
		if(!loading)
		{
			if(message != MSG_READY && !echo_ns::is_paused())
				echo_ns::toggle_pause();
			loading = 1;
		}
		else
		{
			echo_ns::toggle_pause();
			loading = 0;
			//load_frame = 0;
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
	if(!loading)
	{
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
		}
		else if(key == GLUT_KEY_UP)
		{
			if(echo_ns::angle.x > -60)
				echo_ns::angle.x -= ROTATE_ANG;
		}
	}
	else
	{
		if(key == GLUT_KEY_UP && file_index > 0)
		{
			file_index--;
		}
		else if(key == GLUT_KEY_DOWN && file_index < files->num_files - 1)
		{
			file_index++;
		}
		file_start = file_index - NUM_FILES_DISPLAYED + 1;
		if(file_start < 0)
			file_start = 0;
	}
}

