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
#include <cstring>
//#include <iostream>
#include <cmath>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_gfx.h>
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
	#include <nds/arm9/video.h>
	#include <fat.h>
	
	#include "freeserif16.h"
	#include "font.h"
	#include "topscreen.h"
	#include "menu.h"
#else
	#ifdef __MACH__	//OS X
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <GLUT/glut.h>
	#else
		#include <GL/glut.h>
		#include <GL/gl.h>
		#include <GL/glu.h>
	#endif
	#include <libgen.h>
#endif

#define ESCAPE 27
#define ENTER  13

#define MSG_READY "ready"
#define MSG_START "start"
#define MSG_PAUSE "thinking"
#define MSG_BLANK ""

#ifdef ARM9
	#define COUNTER_HEAD    "goals: "
#else
	#define COUNTER_HEAD    "goals: %i"
#endif
#define SUCCESS         "success"
#define	NO_GOALS	"no goals"

#define LOAD_MAX		10.0f
#define START_MAX           	50
#define NAME_DISPLAY_MAX    	30
#define FILE_SPACE_PER_DEPTH	0.06f
#define NUM_FILES_DISPLAYED     31


#ifdef ARM9
	//From LiraNuna =D
	// Macro to translate 2D coord into 1D map array
	#define POS2IDX(x, y)		((x) + ((y) * 32))
	
	// Set of defines for tile flipping
	#define TILE_FLIP_NONE		(0<<10)
	#define TILE_FLIP_X		(1<<10)
	#define TILE_FLIP_Y		(2<<10)
	#define TILE_FLIP_XY		(TILE_FLIP_X | TILE_FLIP_Y)
	
	// Macro for palette selecting
	#define TILE_PALETTE(n)		((n)<<12)

	#define CHAR2TILE(c)		((c) - 32)

	#define WRITE16(map, x, y, c) 	(map)[POS2IDX((x)	, (y))] 	= CHAR2TILE((c)) * 4; \
					(map)[POS2IDX((x) + 1	, (y))] 	= CHAR2TILE((c)) * 4 + 1; \
					(map)[POS2IDX((x)	, (y) + 1)] 	= CHAR2TILE((c)) * 4 + 2; \
					(map)[POS2IDX((x) + 1	, (y) + 1)] 	= CHAR2TILE((c)) * 4 + 3;
	
	#define NDS_INFO_MODE		-2
	#define NDS_LOAD_MODE		-1
	#define NDS_START_MODE		0
	#define NDS_DEBUG_MODE		1
	
	#define NDS_MODE_MIN		-2
	#define NDS_MODE_MAX		1
	
	
	
	#define LOAD_BG			DISPLAY_BG1_ACTIVE
	#define INFO_BG			DISPLAY_BG1_ACTIVE
	#define START_BG		DISPLAY_BG0_ACTIVE
	#define DEBUG_BG		DISPLAY_BG3_ACTIVE
#endif

//--VARIABLES

#ifndef ARM9
	static int window;
#endif

static int menu_mode = 1;

static int my_width, my_height;
static float depth, file_space, font_div;
static float real_width, real_height;
static int loading = 0, load_frame = 0, file_index = 0, file_start = 0;

static int start_frame = 0, name_display = NAME_DISPLAY_MAX;
static char* counter;
static char* message = MSG_READY;

static int touch_started = 0, start_x = 0, start_y = 0;
static vector3f real_angle(0, 0, 0);
static echo_files* files = NULL;

#ifdef ARM9
	static int sub_mode = NDS_START_MODE;
	static u16* string_map = NULL;
	static uint32 basic_modes = MODE_0_2D | DISPLAY_BG_EXT_PALETTE;
#endif

//--METHODS

static void load(const char* fname);

static void init(int argc, char **argv, int w, int h);

static void resize(int w, int h);
static void set_proj(int w, int h);

static void display();

#ifdef ARM9
	static void get_key();
	static void refresh_sub_mode();
	
	static void serif16_draw_string(int x, int y, const char* str);
	static void serif16_draw_string(int x, int y, const char* str, int num);
	static void serif16_clear();
	static void serif16_clear_row(int y);
	
	//static void toggle_loader();
	static void update_loader();
	
	static void toggle_info();
	static void update_num_goals();
	static void update_char_state();
#else
	static void mouse(int button, int state, int x, int y);
	static void key(unsigned char key, int x, int y);
	static void spec_key(int key, int x, int y);
	static void pointer(int x, int y);
#endif

int main(int argc, char **argv)
{
#ifdef ARM9
	init(argc, argv, 255, 191);
	
	fatInitDefault();
	init_math();
	files = get_files("/");
	//dump_files(files);
	load(NULL);
	resize(255, 191);
	ECHO_PRINT("is stage null?: %i\n", echo_ns::current_stage == NULL);
	if(echo_ns::current_stage)
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
#ifdef WIN32
	TCHAR buffer[MAX_PATH] = "";
	GetCurrentDirectory(MAX_PATH, buffer);
	ECHO_PRINT("current dir : %s\n", buffer);
	files = get_files(buffer);
#else
	files = get_files(getenv("PWD"));
#endif
	
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
	//*
	else
	{
		//load("sample1.xml");
		load(NULL);
	}
	// */
	
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
	
	if(fname != NULL)
	{
		echo_ns::init(load_stage(echo_merge(files->current_dir, fname)));
#ifdef ARM9
		videoSetMode(MODE_0_3D);
#endif
		menu_mode = 0;
		depth = echo_ns::current_stage->get_farthest() + 1;
	}
	else
	{
		echo_ns::init(NULL);
#ifdef ARM9
		videoSetMode(MODE_0_3D | DISPLAY_BG1_ACTIVE);
#endif
		menu_mode = 1;
		depth = 5;
	}
	
	file_space = FILE_SPACE_PER_DEPTH * depth;
	font_div = 150 / depth;
	resize(my_width, my_height);
}

#ifdef ARM9
	static void refresh_sub_mode()
	{
		//videoSetMode(MODE_0_3D | DISPLAY_BG1_ACTIVE);
		if(sub_mode > NDS_MODE_MAX)		sub_mode = NDS_MODE_MIN;
		else if(sub_mode < NDS_MODE_MIN)	sub_mode = NDS_MODE_MAX;
		switch(sub_mode)
		{
			case NDS_LOAD_MODE:		videoSetModeSub(basic_modes | LOAD_BG);	update_loader();	break;
			case NDS_INFO_MODE:		videoSetModeSub(basic_modes | INFO_BG);	toggle_info();		break;
			case NDS_START_MODE:		videoSetModeSub(basic_modes | START_BG);					break;
			case NDS_DEBUG_MODE:	videoSetModeSub(basic_modes | DEBUG_BG);				break;
			default:
			break;
		}
	}
#endif

static void init(int argc, char **argv, int w, int h)
{
#ifdef ARM9
        powerON(POWER_ALL);
        
        irqInit();
        irqSet(IRQ_VBLANK, 0);
        
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
        vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_LCD);
	vramSetBankE(VRAM_E_LCD);
	vramSetBankF(VRAM_F_LCD);
	vramSetBankG(VRAM_G_LCD);
	//vramSetBankH(VRAM_H_LCD);
	vramSetBankI(VRAM_I_SUB_BG);
	
	//Main Screen
	
	videoSetMode(MODE_0_3D | DISPLAY_BG1_ACTIVE);
	
	glInit();
        glEnable(GL_ANTIALIAS);
	glEnable(GL_OUTLINE);
	glSetOutlineColor(0, RGB15(0, 0, 0));	//unrolled loops ftw
	glSetOutlineColor(1, RGB15(0, 0, 0));
	glSetOutlineColor(2, RGB15(0, 0, 0));
	glSetOutlineColor(3, RGB15(0, 0, 0));
	glSetOutlineColor(4, RGB15(0, 0, 0));
	glSetOutlineColor(5, RGB15(0, 0, 0));
	glSetOutlineColor(6, RGB15(0, 0, 0));
	glSetOutlineColor(7, RGB15(0, 0, 0));
        glClearColor(31,31,31,31); // BG must be opaque for AA to work
        glClearPolyID(63); // BG must have a unique polygon ID for AA to work
        glClearDepth(0x7FFF);
	
	BG1_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(5) | BG_TILE_BASE(1) | BG_PRIORITY(0);
	memcpy((u16*)BG_TILE_RAM(1), menuTiles, menuTilesLen);
	memcpy((u16*)BG_MAP_RAM(5), menuMap, menuMapLen);
	memcpy(BG_PALETTE, menuPal, menuPalLen);
	
	//Sub Screen
	
	refresh_sub_mode();
	
	SUB_BG0_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(2) | BG_PRIORITY(2);
	memcpy((u16*)BG_TILE_RAM_SUB(2), topscreenTiles, topscreenTilesLen);
	memcpy((u16*)BG_MAP_RAM_SUB(0), topscreenMap, topscreenMapLen);
	
	SUB_BG1_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(2) | BG_TILE_BASE(4) | BG_PRIORITY(0);
	u16* text1_tile = (u16*)BG_TILE_RAM_SUB(4);
        string_map = (u16*)BG_MAP_RAM_SUB(2);
	memcpy(text1_tile, freeserif16Tiles, freeserif16TilesLen);
	
	SUB_BG3_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(4) | BG_TILE_BASE(6) | BG_PRIORITY(3);
	u16* text_tile = (u16*)CHAR_BASE_BLOCK_SUB(6);
        u16* text_map = (u16*)SCREEN_BASE_BLOCK_SUB(4);
	consoleInit((u16*)fontTiles, text_tile, 95, 32, text_map, CONSOLE_USE_COLOR255, 8);
	memcpy(text_tile, fontTiles, fontTilesLen);
	
	vramSetBankH(VRAM_H_LCD);
	memcpy(VRAM_H_EXT_PALETTE[3], fontPal, fontPalLen);
	memcpy(VRAM_H_EXT_PALETTE[1], freeserif16Pal, freeserif16PalLen);
	memcpy(VRAM_H_EXT_PALETTE[0], topscreenPal, topscreenPalLen);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	
	text_map[POS2IDX(1, 1)] = CHAR2TILE('A');
	
	lcdSwap();
	
	ECHO_PRINT("\x1b[2;2Hconsole init\n");
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
	glutMouseFunc(&mouse);
	glutMotionFunc(&pointer);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_LINE_SMOOTH);
	
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

#ifdef ARM9
	static void serif16_draw_string(int x, int y, const char* str)
	{
		while(*str != '\0')
		{
			WRITE16(string_map, x, y, *str);
			x += 2;
			str++;
		}
	}
	static void serif16_draw_string(int x, int y, const char* str, int num)
	{
		int i = 0;
		while(*str != '\0' && i < num)
		{
			WRITE16(string_map, x, y, *str);
			x += 2;
			str++;
			i++;
		}
	}
	static void serif16_clear()
	{
		memset(string_map, 0, 1024 * sizeof(u16));
	}
	static void serif16_clear_row(int y)
	{
		memset(string_map + y * 32, 0, 32 * sizeof(u16));
	}
#else
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

#ifdef ARM9
	static void update_loader()
	{
		serif16_clear();
		serif16_clear_row(18);	serif16_clear_row(19);
		serif16_clear_row(20);	serif16_clear_row(21);
		serif16_clear_row(22);	serif16_clear_row(23);
		serif16_draw_string(0, 0, files->current_dir, 16);
		
		int each_file = 0;
		while(each_file < 10)
		{
			if(file_start + each_file == file_index)
				serif16_draw_string(0, 4 + each_file * 2, "->");
			serif16_draw_string(4, 4 + each_file * 2, files->file_names[file_start + each_file], 14);
			each_file++;
		}
	}
	
	static void toggle_info()
	{
		serif16_clear();
		if(echo_ns::current_stage)
		{
			serif16_draw_string(0, 0, "stage: ");
			serif16_draw_string(14, 0, echo_ns::current_stage->get_name().c_str(), 9);
			update_num_goals();
			update_char_state();
		}
		else
		{
			serif16_draw_string(0, 0, "no stage loaded");
		}
	}
	static void update_num_goals()
	{
		serif16_clear_row(10);	serif16_clear_row(11);
		serif16_clear_row(12);	serif16_clear_row(13);
		if(echo_ns::current_stage)
		{
			int goals_left = echo_ns::goals_left();
			if(goals_left > 0)
			{
				char* counter = new char[(int)log(goals_left) + 1];
				CHKPTR(counter);
				sprintf(counter, "%i", goals_left);
				serif16_draw_string(0, 10, COUNTER_HEAD);
				serif16_draw_string(0, 12, counter, 16);
				delete counter;
			}
			else if(echo_ns::num_goals())
				serif16_draw_string(0, 11, SUCCESS);
			else
				serif16_draw_string(0, 11, NO_GOALS);
		}
		else
			serif16_draw_string(0, 11, NO_GOALS);
	}
	static void update_char_state()
	{
		serif16_clear_row(22);	serif16_clear_row(23);
		if(echo_ns::current_stage)
			serif16_draw_string(0, 22, message);
		else
			serif16_draw_string(0, 22, "pls load a stage");
	}
#else
	static void draw_HUD()
	{
		//ready, start, or thinking
		
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
		
		//name of the stage
		
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
		
		//num goals left
		
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
		if(goals_left > 0)
			delete counter;
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

		draw_string(side_x, real_height - 0.4f, const_cast<char*>(files->current_dir));
		
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

static void display()
{
#ifndef ARM9
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glColor3f(0, 0, 0);
	
	if(!menu_mode)
		draw_HUD();
#else
	if(message == MSG_START)
	{
		if(start_frame > START_MAX)
			message = MSG_BLANK;
		else
			start_frame++;
	}
	
	if(sub_mode == NDS_INFO_MODE)
	{
		update_num_goals();
		update_char_state();
	}
	else if(sub_mode == NDS_LOAD_MODE)
	{
		update_loader();
	}
#endif
	
	glLoadIdentity();
	
	gfx_rotatef(-echo_ns::angle.x, 1, 0, 0);
	gfx_rotatef(-echo_ns::angle.y, 0, 1, 0);
	
	if(!menu_mode)
	{
		echo_ns::draw();
	}
#ifndef ARM9
	else
	{
		draw_message_string(-2, 3, "L-Echo");
		draw_string(-3, 0, "Please load a stage.");
		draw_string(-6, -2, "Press L To Toggle Loader.");
		draw_string(-6, -2.5, "Press P To Start/Pause/Resume.");
		draw_string(-6, -3, "Press Arrow Keys or use Mouse to ");
		draw_string(-3, -3.5, "Rotate World.");
		draw_string(-6, -4, "Press Esc To Quit.");
	}
#endif
	
#ifndef ARM9
	draw_loader();
	
	glutSwapBuffers();
	ECHO_SLEEP(30);
#endif
}

// ----CONTROLS---

#define ROTATE_ANG	5.0f

static void up()
{
	if(echo_ns::angle.x > -60)
	{
		if(touch_started) real_angle.x -= ROTATE_ANG;
		echo_ns::angle.x -= ROTATE_ANG;
	}
}

static void down()
{
	if(echo_ns::angle.x < 60)
	{
		if(touch_started) real_angle.x += ROTATE_ANG;
		echo_ns::angle.x += ROTATE_ANG;
	}
}

static void left()
{
	if(touch_started)
		real_angle.y -= ROTATE_ANG;
	echo_ns::angle.y -= ROTATE_ANG;
	if(echo_ns::angle.y < -180)		echo_ns::angle.y += 360;
}

static void right()
{
	if(touch_started)
		real_angle.y += ROTATE_ANG;
	echo_ns::angle.y += ROTATE_ANG;
	if(echo_ns::angle.y > 180)		echo_ns::angle.y -= 360;
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
	//ECHO_PRINT("s or p\n");
	start_frame = 0;
	if(message == MSG_READY)
	{
		message = MSG_START;
		echo_ns::start();
#ifndef ARM9
		name_display--;
#endif
	}
	else
	{
		echo_pause();
	}
}

static void pointer(int x, int y)
{
	echo_ns::angle = real_angle;
	echo_ns::angle.x += -(int)TO_DEG(atanf(4.0f * (y - start_y) / my_height)) / 5 * 5;
	if(echo_ns::angle.x < -60)		echo_ns::angle.x = -60;
	else if(echo_ns::angle.x > 60)		echo_ns::angle.x = 60;
	echo_ns::angle.y += -(int)TO_DEG(atanf(4.0f * (x - start_x) / my_width)) / 5 * 5;
	if(echo_ns::angle.y < -180)		echo_ns::angle.y += 360;
	else if(echo_ns::angle.y > 180)		echo_ns::angle.y -= 360;
}

static void pressed(int x, int y)
{
	touch_started = 1;
	start_x = x;
	start_y = y;
	real_angle = echo_ns::angle;
}

#ifndef ARM9
static void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			pressed(x, y);
		else
			touch_started = 0;
	}
}
#endif

#ifdef ARM9
static void get_key()
{
	scanKeys();
	
	if(!menu_mode)
	{
		if(keysHeld() & KEY_TOUCH)
		{
			touchPosition t_pos = touchReadXY();
			if(touch_started)	pointer(t_pos.px, t_pos.py);
			else			pressed(t_pos.px, t_pos.py);
		}
		else if(touch_started)
			touch_started = 0;
	}
	
	u16 key = keysDown();
	if(sub_mode == NDS_LOAD_MODE)
	{
		if((key & KEY_L) || (key & KEY_R))
		{
			if(!is_dir(files, file_index))
			{
				//ECHO_PRINT("trying to load:%s\n", echo_merge(files->current_dir, files->file_names[file_index]));
				load(files->file_names[file_index]);
			}
			else
			{
				const char* file = files->file_names[file_index];
				if(!strcmp(file, ".."))
				{
					if(strcmp(files->current_dir, "/"))
					{
						int len =  strrchr(files->current_dir, '/') - files->current_dir;
						if(len > 0)
						{
							char* dir = new char[len + 1];
							CHKPTR(dir);
							memset(dir, '\0', len + 1);
							strncpy(dir, files->current_dir, len);
							delete files;
							files = get_files(dir);
						}
						else if(len == 0) //going to root
						{
							delete files;
							files = get_files("/");
						}
					}
				}
				else
				{
					const char* current_dir = files->current_dir;
					delete files;
					files = get_files(echo_merge(current_dir, file));
				}
				ECHO_PRINT("<new dir>%s</new dir>\n", files->current_dir);
				file_index = 0;
				file_start = 0;
			}
		}
		if(((key & KEY_DOWN)	|| (key & KEY_B)) && file_index < files->num_files - 1)	file_index++;
		if(((key & KEY_UP)		|| (key & KEY_X)) && file_index > 0)					file_index--;
		file_start = file_index - 9;
		if(file_start < 0)
			file_start = 0;
	}
	else if(!menu_mode)
	{
		if((key & KEY_L) || (key & KEY_R))
			start_or_pause();
		if((key & KEY_RIGHT)	|| (key & KEY_A))	right();
		if((key & KEY_LEFT)		|| (key & KEY_Y))	left();
		if((key & KEY_DOWN)	|| (key & KEY_B))	down();
		if((key & KEY_UP)		|| (key & KEY_X))	up();
	}
	if(key & KEY_START)
	{
		sub_mode++;
		refresh_sub_mode();
	}
	if(key & KEY_SELECT)
	{
		sub_mode--;
		refresh_sub_mode();
		ECHO_PRINT("angle: %f, %f\n" , echo_ns::angle.x, echo_ns::angle.y);
	}
	if(key & KEY_LID && !menu_mode)
	{
		if(!echo_ns::is_paused())
		{
			message = MSG_PAUSE;
			echo_ns::toggle_pause();
		}
	}
	//The DS doesn't have a FPU, so it isn't very precise
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
	{
		if(!menu_mode)
			start_or_pause();
	}
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
				//std::cout << "file is dir: " << file << ", " << files->current_dir << std::endl;
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
				//std::cout << "newdir: " << dir << std::endl;
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
		if(!menu_mode)
		{
			ECHO_PRINT("ang: ");
			echo_ns::angle.dump();
			ECHO_PRINT("\n");
		}
	}
}

static void spec_key(int key, int x, int y)
{
	if(!loading)
	{
		if(!menu_mode)
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


