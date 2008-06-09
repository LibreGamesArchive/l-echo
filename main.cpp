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
//#include <iostream>
#include <cmath>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_math.h>
#include <echo_loader.h>
#include <echo_ns.h>
#include <echo_sys.h>
#include <echo_stage.h>
#include <echo_ingame_loader.h>

#include <hole.h>
#include <grid.h>
#include <escgrid.h>
#include <static_grid.h>
#include <isect_grid.h>
#include <t_grid.h>

#define _STDCALL_SUPPORTED

#ifdef ARM9

	#include <nds.h>
	#include <fat.h>

#else

	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <libgen.h>

#endif

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
#define FILE_SPACE_PER_DEPTH	0.06f
#define NUM_FILES_DISPLAYED     31
#define NULL_CHAR_OPACITY_MIN   0.25f

#ifndef ARM9
	static int window;
#endif

static int my_width, my_height;
static float depth, file_space, font_div;
static float real_width, real_height;
static int loading = 0, load_frame = 0, file_index = 0, file_start = 0;

static int start_frame = 0, name_display = NAME_DISPLAY_MAX;
static char* counter;
static char* message = MSG_READY;

static float null_char_opacity;
static int opacity_incr;

static echo_files* files;

static void load(const char* fname);
static void init(int argc, char **argv, int w, int h);

static void resize(int w, int h);
static void set_proj(int w, int h);

static void display();

#ifdef ARM9
	static void get_key();
#else
	static void key(unsigned char key, int x, int y);
	static void spec_key(int key, int x, int y);
#endif

int main(int argc, char **argv)
{
#ifdef ARM9
	init(argc, argv, 255, 191);
	
	vector3f* vec = new vector3f(1, 2, 2);
	CHKPTR(vec);
	vector3f* angle = vec->angle_xy();
	angle->dump();
	ECHO_PRINT("\n");
	vector3f* norm = vec->neg_rotate_yx(*angle);
	norm->dump();
	ECHO_PRINT("\n");
	vector3f* back = norm->rotate_xy(*angle);
	back->dump();
	ECHO_PRINT("\n");
	
	fatInitDefault();
	init_math();
	files = get_files("/");
	load("fat:/sample1.xml");
	resize(255, 191);
	ECHO_PRINT("is stage null?: %i\n", echo_ns::current_stage == NULL);
	ECHO_PRINT("is stage start null?: %i\n", echo_ns::current_stage->get_start() == NULL);
	while (1)
        {
                get_key();

                display();

                glFlush(0);

                swiWaitForVBlank();
        }
#else
	init_math();
	
	/*
	vector3f* orig = new vector3f(0, 0, 0);
	CHKPTR(orig);
	vector3f* old_goal = new vector3f(2, 0, 2);
	CHKPTR(old_goal);
	vector3f* pos = new vector3f(1, 1.414, 1);
	CHKPTR(pos);
	
	vector3f* angle = (*old_goal - *orig).angle_xy();
	ECHO_PRINT("angle: ");
	angle->dump();
	ECHO_PRINT("\n");
	vector3f* trans_pos = (*pos - *orig).neg_rotate_yx(*angle);
	ECHO_PRINT("trans_pos: ");
	trans_pos->dump();
	ECHO_PRINT("\n");
	
	vector3f* new_goal = new vector3f(2, 2.828, 2);
	CHKPTR(new_goal);
	vector3f* new_angle = (*new_goal - *orig).angle_xy();
	ECHO_PRINT("new_angle: ");
	new_angle->dump();
	ECHO_PRINT("\n");
	vector3f new_pos = *(trans_pos->rotate_xy(*new_angle)) + *orig;
	new_pos = new_pos * (new_goal->length() / old_goal->length());
	ECHO_PRINT("new_pos: ");
	new_pos.dump();
	ECHO_PRINT("\n");
	// */
	
	if(argc >= 2)
	{
		if(!strcmp(argv[1], "-h"))
		{
			ECHO_PRINT("Usage: %s [-h | -t] [stage file name]\n", argv[0]);
			ECHO_PRINT("\t-h\tprints this help message\n");
			ECHO_PRINT("\t-t\tjust tests the stage file\n");
			ECHO_PRINT("if no stage is specified, sample1.xml is loaded.\n");
			std::exit(0);
		}
		else if(!strcmp(argv[1], "-t"))
		{
			if(load_stage(argv[2]))
			{
				ECHO_PRINT("stage file OK\n");
				std::exit(0);
			}
			else
			{
				ECHO_PRINT("stage file has errors...\n");
				std::exit(1);
			}
		}
		else
			load(argv[1]);
	}
	else
	{
		load("sample1.xml");
	}
	
	char* pwd = new char[4096];
	CHKPTR(pwd);
	files = get_files(getcwd(pwd, 4096));
	init(argc, argv, 640, 480);
	glutMainLoop();
#endif
	return(1);
}

static void load(const char* fname)
{
	start_frame = 0;
	name_display = NAME_DISPLAY_MAX;
	message = MSG_READY;
	
	opacity_incr = 1;
	null_char_opacity = NULL_CHAR_OPACITY_MIN;
	
	echo_ns::init(load_stage(fname));
	
	depth = echo_ns::current_stage->get_farthest() + 1;
	file_space = FILE_SPACE_PER_DEPTH * depth;
	font_div = 150 / depth;
	ECHO_PRINT("depth: %f\n", depth);
	resize(my_width, my_height);
}

static void init(int argc, char **argv, int w, int h)
{
#ifdef ARM9
	// Turn on everything
        powerON(POWER_ALL);

        // Setup the Main screen for 3D
        videoSetMode(MODE_0_3D);

        // IRQ basic setup
        irqInit();
        irqSet(IRQ_VBLANK, 0);

        // initialize the geometry engine
        glInit();

        // enable antialiasing
        glEnable(GL_ANTIALIAS);
	
	videoSetModeSub(MODE_1_2D | DISPLAY_BG0_ACTIVE);        //sub bg 0 will be used to print text
        vramSetBankC(VRAM_C_SUB_BG);

        SUB_BG0_CR = BG_MAP_BASE(31);

        BG_PALETTE_SUB[255] = RGB15(31,31,31);  //by default font will be rendered with color 255

        //consoleInit() is a lot more flexible but this gets you up and running quick
        consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
	
	lcdSwap();
	
	ECHO_PRINT("console init\n", 0, 0);
	
        // setup the rear plane
        glClearColor(31,31,31,31); // BG must be opaque for AA to work
        glClearPolyID(63); // BG must have a unique polygon ID for AA to work
        glClearDepth(0x7FFF);
	
        //resize(w, h);
	
	ECHO_PRINT("finished init\n");
#else
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
	set_proj(w, h);
#endif
}

static void resize(int w, int h)
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
                real_width = depth;
                real_height = depth * w / h;
        }
        else
        {
                real_width = depth * w /  h;
                real_height = depth;
        }
        glOrtho(-real_width, real_width, -real_height, real_height, -depth, depth);
	
#ifdef ARM9
	//ds specific, several attributes can be set here
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
#endif
	
	glMatrixMode(GL_MODELVIEW);
}

// ----DISPLAY STRING----

#ifndef ARM9
//copied from http://lighthouse3d.com/opengl/glut/index.php?bmpfontortho
static void draw_message_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) / font_div * 2;
		c++;
	}
}

static void draw_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) / font_div;
		c++;
	}
}

static void draw_fname_string(float x, float y, char *string)
{
	char *c = string;
	while(*c != '\0')
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, *c) / font_div;
		c++;
	}
}
#endif

// ----DRAW MAIN----

#ifndef ARM9
static void draw_HUD()
{
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
	
	draw_message_string(-0.8f * real_width, -0.9f * real_height, message);
	
	if(name_display > 0)
	{
		glColor3f(0, 0, 0);
		
		if(name_display < NAME_DISPLAY_MAX)
		{
			glColor4f(0, 0, 0, name_display * 1.0f / NAME_DISPLAY_MAX);
			name_display--;
		}
		//std::cout << my_stage->get_name() << std::endl;
		draw_string(-0.9f * real_width, 0, const_cast<char*>(echo_ns::current_stage->get_name().c_str()));
	}
	
	int goals_left = echo_ns::goals_left();
	if(goals_left > 0)
	{
		//not very precise, but oh well
		counter = new char[(int)log(goals_left) + 10];
		CHKPTR(counter);
		sprintf(counter, COUNTER_HEAD, goals_left);
	}
	else if(echo_ns::num_goals())
		counter = SUCCESS;
	else
		counter = NO_GOALS;
	
	glColor3f(0, 0, 0);
	draw_string(-0.6f * real_width, -0.8f * real_height, counter);
}
#endif

#ifndef ARM9
static void draw_loader()
{
	if(loading || load_frame > 0)
	{
		if(!loading)
			load_frame--;
		else if(load_frame < LOAD_MAX)
			load_frame++;
		
		float side_x = real_width * (1 - 1.5f * load_frame / LOAD_MAX);
		
		glLoadIdentity();
		glTranslatef(0, 0, depth - 0.1f);
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		glVertex3f(side_x, real_height, 0);
		glVertex3f(real_width, real_height, 0);
		glVertex3f(real_width, -real_height, 0);
		glVertex3f(side_x, -real_height, 0);
		glEnd();
		
		glTranslatef(0, 0, 0.05f);
		glColor3f(1, 1, 1);

		draw_string(side_x, real_height - 0.4f, files->current_dir);
		
		glColor3f(0.5f, 0.5f, 0.5f);
		
		int each_file = 0;
		float each_y = real_height - 3 * file_space;
		while(each_y >= -real_height && each_file < NUM_FILES_DISPLAYED && each_file < files->num_files)
		{
			if(file_start + each_file == file_index)
			{
				glColor3f(1, 1, 1);
			}
			draw_fname_string(side_x, each_y, files->file_names[file_start + each_file]);
			if(file_start + each_file == file_index)
			{
				glColor3f(0.5f, 0.5f, 0.5f);
			}
			each_y -= file_space;
			each_file++;
		}
	}
}
#endif

static void draw_character()
{
#ifdef ARM9
		glBegin(GL_QUADS);
			glVertex3f(0, HALF_GRID, 0);
			glVertex3f(HALF_GRID, 0, 0);
			glVertex3f(0, -HALF_GRID, 0);
			glVertex3f(-HALF_GRID, 0, 0);
		glEnd();
#else
		glutSolidSphere(0.1, 8, 8);
#endif
}

static void display()
{
#ifndef ARM9
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	glLoadIdentity();
	
	glColor3f(0, 0, 0);
	
#ifndef ARM9
	draw_HUD();
#endif

	glLoadIdentity();
	
#ifdef ARM9
	glRotatef(echo_ns::angle.x, 1, 0, 0);
	glRotatef(echo_ns::angle.y, 0, 1, 0);
#else
	glRotatef(-echo_ns::angle.x, 1, 0, 0);
	glRotatef(-echo_ns::angle.y, 0, 1, 0);
#endif
	
	echo_ns::draw();
	
	glColor3f(0.5f, 0.5f, 0.5f);
	vector3f* vec = echo_ns::step_char();
	//ECHO_PRINT("is vec null?: %i\n", vec == NULL);
	if(vec)
	{
		glTranslatef(vec->x, vec->y + 0.25, vec->z);
		draw_character();
		delete vec;
	}
	else
	{
		grid* g = echo_ns::current_stage->get_start();
		if(g)
		{
			grid_info_t* info = g->get_info(echo_ns::angle);
			if(info)
			{
				glColor3f(null_char_opacity, null_char_opacity, null_char_opacity);
				glTranslatef(info->pos.x, info->pos.y + 0.25, info->pos.z);
				draw_character();
				if(opacity_incr)
				{
					null_char_opacity += 0.05f;
					if(null_char_opacity >= 1)
						opacity_incr = 0;
				}
				else
				{
					null_char_opacity -= 0.05f;
					if(null_char_opacity <= NULL_CHAR_OPACITY_MIN)
						opacity_incr = 1;
				}
			}
		}
	}
	
	
#ifndef ARM9
	draw_loader();
	
	glutSwapBuffers();
	echo_sleep(30000);
#endif
}

// ----CONTROLS---

#define ROTATE_ANG	5.0f

static void up()
{
	if(echo_ns::angle.x > -60)
		echo_ns::angle.x -= ROTATE_ANG;
}

static void down()
{
	if(echo_ns::angle.x < 60)
		echo_ns::angle.x += ROTATE_ANG;
}

static void left()
{
	echo_ns::angle.y -= ROTATE_ANG;
	if(echo_ns::angle.y < -180)	echo_ns::angle.y += 360;
}

static void right()
{
	echo_ns::angle.y += ROTATE_ANG;
	if(echo_ns::angle.y > 180)	echo_ns::angle.y -= 360;
}

static void echo_pause()
{
	if(!echo_ns::is_paused())
		message = MSG_PAUSE;
	else
		message = MSG_BLANK;
	echo_ns::toggle_pause();
}

static void start_or_pause()
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
		echo_pause();
	}
}

#ifdef ARM9
static void get_key()
{
	scanKeys();
	
	u16 key = keysDown();
	//echo_sleep(100);
	if(key & KEY_A)
		start_or_pause();
	else if(key & KEY_SELECT)
	{
		ECHO_PRINT("angle * 100: %i, %i\n" , (int)(echo_ns::angle.x * 100), (int)(echo_ns::angle.y* 100));
	}
	else if(key & KEY_LID)
	{
		if(!echo_ns::is_paused())
		{
			message = MSG_PAUSE;
			echo_ns::toggle_pause();
		}
	}
	else if(key  & KEY_RIGHT)
		right();
	else if(key & KEY_LEFT)
		left();
	else if(key & KEY_DOWN)
		down();
	else if(key & KEY_UP)
		up();
	echo_ns::angle.x = (int)echo_ns::angle.x;
	echo_ns::angle.y = (int)echo_ns::angle.y;
}
#else

static void key(unsigned char key, int x, int y)
{
	if(key == ESCAPE)
	{
		glutDestroyWindow(window);
		exit(0);
	}
	else if(key == 'p' || key == 'P')
		start_or_pause();
	else if(key == ENTER)
	{
		if(loading)
		{
			const char* file = files->file_names[file_index];
			if(!is_dir(files->current_dir, file))
			{
				load(file);
			}
			else
			{
				char* dir;
				if(!strcmp(file, ".."))
				{
					dir = new char[strlen(files->current_dir)];
					CHKPTR(dir);
					strcpy(dir, dirname(files->current_dir));
				}
				else
				{
					dir = echo_merge(files->current_dir, file);
				}
				delete files;
				files = get_files(dir);
				file_index = 0;
				file_start = 0;
			}
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
		}
	}
	else if(key == 'a' || key == 'A')
	{
		ECHO_PRINT("ang: ");
		echo_ns::angle.dump();
		ECHO_PRINT("\n");
	}
}

static void spec_key(int key, int x, int y)
{
	if(!loading)
	{
		if(key == GLUT_KEY_RIGHT)
			right();
		else if(key == GLUT_KEY_LEFT)
			left();
		else if(key == GLUT_KEY_DOWN)
			down();
		else if(key == GLUT_KEY_UP)
			up();
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

#endif

