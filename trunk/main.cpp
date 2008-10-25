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
#include <cmath>

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_gfx.h>
#include <echo_math.h>
#include <echo_loader.h>
#include <echo_ns.h>
#include <echo_sys.h>
#include <echo_stage.h>
#include <echo_ingame_loader.h>
#include <echo_prefs.h>
#include <echo_char_joints.h>

#include <hole.h>
#include <grid.h>
#include <escgrid.h>
#include <static_grid.h>
#include <isect_grid.h>
#include <t_grid.h>

//for Windows, or something
#define _STDCALL_SUPPORTED

#ifdef ECHO_NDS
	#include <nds.h>
	#include <nds/arm9/video.h>
	#include <fat.h>
	
	#include "font.h"
	#include "topscreen.h"
	#include "menu.h"
	
	//header of the counter
	#define COUNTER_HEAD    	"goals: "	//# of goals on next line
	//number of files displayed by the in-game loader
	#define NUM_FILES_DISPLAYED	22
	
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
#else
	#ifdef ECHO_PC
		#ifdef ECHO_OSX	//OS X
			#include <OpenGL/gl.h>
			#include <OpenGL/glu.h>
			#include <GLUT/glut.h>
		#else
			#include <GL/glut.h>
			#include <GL/gl.h>
			#include <GL/glu.h>
		#endif
		#include <libgen.h>
		
		//Key code for keyboard 
		#define ESCAPE 27
		#define ENTER  13
	#elif ECHO_GCN || ECHO_WII
		//using gl2gx to save code and time and pain and blood and suffering and...
		#include <GL/glut.h>
		#include <GL/gl.h>
		#include <GL/glu.h>
	#endif
	
	//format of the counter
	#define COUNTER_HEAD    	"goals: %i"	//# of goals appended
	//number of files displayed by the in-game loader
	#define NUM_FILES_DISPLAYED     31
	//the number of frames the loader can scroll
	#define LOAD_MAX		10.0f
	//multiply by real_width to get the width of the loader screen
	#define LOADER_WIDTH		1.5f
#endif

//status messages
#define MSG_READY 		"ready"
#define MSG_START 		"start"
#define MSG_PAUSE 		"thinking"
#define MSG_BLANK 		""

//...yeah...
#define SUCCESS         	"success"
#define	NO_GOALS		"no goals"

//the number of frames the name of the stage fades out after starting (no fading in n-echo)
#define NAME_DISPLAY_MAX    	30
//multiply by depth to get the row spacing of the text in the loader
#define FILE_SPACE_PER_DEPTH	0.06f
//the number of frames the status "start" fades out after starting
#define START_MAX           	50
//rotational interval in degrees
#define ROTATE_ANG	5.0f

//--VARIABLES

#ifdef ECHO_NDS
	//mode of the top screen (subscreen)
	static int sub_mode = NDS_START_MODE;
	//address of the "second console", used by the info and loader screens
	static u16* string_map = NULL;
	//basic flags of the top screen
	static const uint32 basic_modes = MODE_0_2D | DISPLAY_BG_EXT_PALETTE;
	//the temp address of the counter (holds number of goals)
	static char counter[32];
#else
	#ifdef ECHO_PC
		//ID of the window
		static int window;
	#endif
	//is the loader toggled?; which frame is the loader in?
	static int loading = 0, load_frame = 0;
	//the temp address of the counter (holds number of goals)
	static char* counter = NULL;
	//was this paused before the loader was toggled?
	static int was_paused = 0;
#endif

//are we in menu mode or playing mode
static int menu_mode = 1;

//dimensions of window or screen (256x192 in NDS)
static int my_width = 0, my_height = 0;
//dimensions of viewing field (important to text)
static float real_width = 0, real_height = 0;

//depth; FILE_SPACE_PER_DEPTH * depth; dividend of character width = 150 / depth
static float depth = 0, file_space = 0, font_div = 0;

//on which file does the loader load?; what file is the top of the loader?
static int file_index = 0, file_start = 0;

//frame of the fading "start" status; frame of the fading name display
static int start_frame = 0, name_display = NAME_DISPLAY_MAX;

//the status message
static char* message = MSG_READY;
//cache of the name of the stage
static char* name_cache = NULL;

//did the dragging start already?; where did the dragging start?
static int touch_started = 0, start_x = 0, start_y = 0;
//the angle when the dragging started
static vector3f real_angle(0, 0, 0);
//the current directory
static echo_files* files = NULL;

#ifdef ECHO_NDS
	static int left, right, up, down;
#endif

//--METHODS

#ifdef ECHO_NDS
	//controller
	static void get_key();
	
	//change the subscreen mode according to sub_mode
	static void refresh_sub_mode();
	
	//draw str at (x, y)
	static void console2_draw_string(int x, int y, char* str);
	//draw num characters (or until end of string) of str at (x, y)
	static void console2_draw_string(int x, int y, char* str, int num);
	//clear the screen
	static void console2_clear();
	//clear row y
	static void console2_clear_row(int y);
	
	//update console2 with the loader
	static void update_loader();
	
	//start info mode
	static void toggle_info();
	//update console2 with # of goals
	static void update_num_goals();
	//update console2 with status
	static void update_char_state();
#elif ECHO_PC
	//mouse event, calls pressed or handles releases.
	static void mouse(int button, int state, int x, int y);
	//normal alphanumeric keys, such as p, l, a
	static void key(unsigned char key, int x, int y);
	//special keys esp. arrow keys
	static void spec_key(int key, int x, int y);
	//draw a string on screen
	static void draw_string(float x, float y, const char *string);
	static void draw_string(float x, float y, char *string);
	//draw a file name (using size 12 font instead of size 12)
	static void draw_fname_string(float x, float y, char *string);
	//draw the status (twice as spaced out)
	static void draw_message_string(float x, float y, char *string);
#endif
//dragged
static void pointer(int x, int y);
//pressed down
static void pressed(int x, int y);
//load the file
static void load(const char* fname);
//initialize
static void init(int argc, char **argv, int w, int h);
//resize the screen
static void resize(int w, int h);
//view
static void display();
//directional control
static void up();
static void down();
static void left();
static void right();
//toggle pause
static void echo_pause();
//starts or pauses (the 'p' key on pc/mac/linux, shoulder button on nds)
static void start_or_pause();

int main(int argc, char **argv)
{
#ifdef ECHO_NDS
	//initialize the file system
	fatInitDefault();
	
	TiXmlDocument* doc = NULL;
	if(open_pref(&doc) == WIN)
	{
		HAND* hand = NULL;
		if(get_hand(doc, hand) == WIN)
		{
			if(*hand == RIGHT_HAND)
			{
				left = KEY_LEFT;
				right = KEY_RIGHT;
				up = KEY_UP;
				down = KEY_DOWN;
			}
			else
			{
				left = KEY_Y;
				right = KEY_A;
				up = KEY_X;
				down = KEY_B;
			}
		}
		close_pref(doc);
	}
	
	//get the files
	files = get_files("/app/nds");
	//initialize the screens
	init(argc, argv, 255, 191);
	//load the menu
	load(NULL);
	//infinite loop
	while(1)
	{
		//get the key presses and touch screen, and refresh topscreen if in info or loader mode
        get_key();
		//otherwise the 3D will paint anyways, so don't paint over.
		if(!menu_mode)
		{
			//display bottom screen
			display();
			//draw the image
			glFlush(0);
		}
		//otherwise we won't sync
        swiWaitForVBlank();
	}
#elif ECHO_PC
	echo_char_joints* joints = new(echo_char_joints);
	joints->rwrist = 10;
	ECHO_PRINT("joints.rwrist: %f\n", joints->rwrist);
	ECHO_PRINT("joints.rhand_x: %f\n", joints->rhand_x);
	ECHO_PRINT("joints.lhand_x: %f\n", joints->lhand_x);
	joints->lwrist = 20;
	ECHO_PRINT("joints.rwrist: %f\n", joints->rwrist);
	ECHO_PRINT("joints.rhand_x: %f\n", joints->rhand_x);
	ECHO_PRINT("joints.lhand_x: %f\n", joints->lhand_x);
	reset_joints(joints);
	ECHO_PRINT("joints.rwrist: %f\n", joints->rwrist);
	ECHO_PRINT("joints.rhand_x: %f\n", joints->rhand_x);
	ECHO_PRINT("joints.lhand_x: %f\n", joints->lhand_x);
	delete joints;
	
	//fill lookup tables
	init_math();
	#ifdef ECHO_WIN //goddammmit windows, adhere to POSIX!
		TCHAR buffer[MAX_PATH] = "";
		GetCurrentDirectory(MAX_PATH, buffer);
		files = get_files(buffer);
	#else
		files = get_files(getenv("PWD"));
	#endif
	
	//if there is a cli argument
	if(argc >= 2)
	{
		//if it is -h
		if(!strcmp(argv[1], "-h"))
		{
			//print usage and exit gracefully
			ECHO_PRINT("Usage: %s [-h | -t] [stage file name]\n", argv[0]);
			ECHO_PRINT("\t-h\tprints this help message\n");
			ECHO_PRINT("\t-t\tjust tests the stage file\n");
			ECHO_PRINT("if no stage is specified, sample1.xml is loaded.\n");
			std::exit(0);
		}
		//if it  is -t
		else if(!strcmp(argv[1], "-t"))
		{
			//test the file
			if(load_stage(argv[2]))
			{
				//ok
				ECHO_PRINT("stage file OK\n");
				std::exit(0);
			}
			else
			{
				//no
				ECHO_PRINT("stage file has errors...\n");
				std::exit(1);
			}
		}
		//else, just load the stage
		else
			load(argv[1]);
	}
	//else, open up with menu
	else
	{
		load(NULL);
	}
	//initialize opengl
	init(argc, argv, 640, 480);
	//start main loop
	glutMainLoop();
#elif ECHO_GCN || ECHO_WII
	//TODO: Wii/GCN port?
#endif
	//never gonna reach here
	return(1);
}

static void load(const char* fname)
{
	//load stage
	stage* s = (fname ? load_stage(echo_merge(files->current_dir, fname)) : NULL);
	//if the stage file is bad (fname != NULL && s == NULL), fuhgeddaboutit!
	if(s || !fname)
	{
		//if stage is valid
		if(s)
		{
			//initialize to that stage
			echo_ns::init(s);
#ifdef ECHO_NDS
			//set menu off (though the 3d buffer seems to have priority anyways)
			videoSetMode(MODE_0_3D);
#endif
			//not in menu mode
			menu_mode = 0;
			//the distance to the farthest point plus 2.5 for good measure.
			depth = echo_ns::current_stage->get_farthest() + 2.5f;
			//so i don't have to call it all the time
			name_cache = const_cast<char*>(echo_ns::current_stage->get_name().c_str());
		}
		//if we want our menu
		else
		{
			//init to null
			echo_ns::init(NULL);
#ifdef ECHO_NDS
			//display the menu
			videoSetMode(MODE_0_3D | DISPLAY_BG1_ACTIVE);
#endif
			//in menu mode
			menu_mode = 1;
			//the distance to the farthest point, but there is no points!
			depth = 5;
			name_cache = NULL;
		}
		//reset start and name frames
		start_frame = 0;
		name_display = NAME_DISPLAY_MAX;
		//set status to ready
		message = MSG_READY;
		//set loader display variables
		file_space = FILE_SPACE_PER_DEPTH * depth;
		font_div = 150 / depth;
		//resize, since we change the depth of the stage
		resize(my_width, my_height);
	}
}

#ifdef ECHO_NDS
	//refresh the subscreen mode
	static void refresh_sub_mode()
	{
		//if out of bounds, correct the mode.
		if(sub_mode > NDS_MODE_MAX)		sub_mode = NDS_MODE_MIN;
		else if(sub_mode < NDS_MODE_MIN)	sub_mode = NDS_MODE_MAX;
		switch(sub_mode)
		{
			case NDS_LOAD_MODE:		videoSetModeSub(basic_modes | LOAD_BG);	update_loader();	break;
			case NDS_INFO_MODE:		videoSetModeSub(basic_modes | INFO_BG);	toggle_info();		break;
			case NDS_START_MODE:		videoSetModeSub(basic_modes | START_BG);			break;
			case NDS_DEBUG_MODE:		videoSetModeSub(basic_modes | DEBUG_BG);			break;
			default:
			break;
		}
	}
#endif

static void init(int argc, char **argv, int w, int h)
{
#ifdef ECHO_NDS
	//power on
        powerON(POWER_ALL);
        
	//initialize hardware interrupts for updating our screens
        irqInit();
	//yea, vblank
        irqSet(IRQ_VBLANK, 0);
        
	//assign banks (H is use for ext. palette)
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
        vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_LCD);
	vramSetBankE(VRAM_E_LCD);
	vramSetBankF(VRAM_F_LCD);
	vramSetBankG(VRAM_G_LCD);
	vramSetBankI(VRAM_I_SUB_BG);
	
	//Main Screen
	
	//set mode
	videoSetMode(MODE_0_3D | DISPLAY_BG1_ACTIVE);
	
	//init
	glInit();
	//i heard it's not good for aa + outline, but going with it anyways...
        glEnable(GL_ANTIALIAS);
	//hardware-based outlining
	glEnable(GL_OUTLINE);
	//set outline colors for all groups
	glSetOutlineColor(0, RGB15(0, 0, 0));	//unrolled loops ftw
	glSetOutlineColor(1, RGB15(0, 0, 0));
	glSetOutlineColor(2, RGB15(0, 0, 0));
	glSetOutlineColor(3, RGB15(0, 0, 0));
	glSetOutlineColor(4, RGB15(0, 0, 0));
	glSetOutlineColor(5, RGB15(0, 0, 0));
	glSetOutlineColor(6, RGB15(0, 0, 0));
	glSetOutlineColor(7, RGB15(0, 0, 0));
        glClearColor(31,31,31,31); 		//BG must be opaque for AA to work
        glClearPolyID(63); 			//BG must have a unique polygon ID for AA to work
        glClearDepth(0x7FFF);
	
	// the menu
	BG1_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(5) | BG_TILE_BASE(1) | BG_PRIORITY(0);
	memcpy((u16*)BG_TILE_RAM(1), menuTiles, menuTilesLen);
	memcpy((u16*)BG_MAP_RAM(5), menuMap, menuMapLen);
	memcpy(BG_PALETTE, menuPal, menuPalLen);
	
	//Sub Screen
	
	//set sub screen mode first
	refresh_sub_mode();
	
	//the bg the game starts out with; the one with all the controls
	SUB_BG0_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(5) | BG_TILE_BASE(6) | BG_PRIORITY(2);
	memcpy((u16*)BG_TILE_RAM_SUB(6), topscreenTiles, topscreenTilesLen);
	memcpy((u16*)BG_MAP_RAM_SUB(5), topscreenMap, topscreenMapLen);
	
	//console2 for info, and loader
	SUB_BG1_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(4) | BG_PRIORITY(0);
	u16* text1_tile = (u16*)BG_TILE_RAM_SUB(4);
        string_map = (u16*)BG_MAP_RAM_SUB(1);
	memcpy(text1_tile, fontTiles, fontTilesLen);
	
	//the debug console
	SUB_BG3_CR = (0 << 14) | BG_COLOR_256 | BG_MAP_BASE(3) | BG_TILE_BASE(2) | BG_PRIORITY(3);
	u16* text_tile = (u16*)CHAR_BASE_BLOCK_SUB(2);
        u16* text_map = (u16*)SCREEN_BASE_BLOCK_SUB(3);
	consoleInit((u16*)fontTiles, text_tile, 95, 32, text_map, CONSOLE_USE_COLOR255, 8);
	memcpy(text_tile, fontTiles, fontTilesLen);
	
	//copy the palette
	vramSetBankH(VRAM_H_LCD);
	memcpy(VRAM_H_EXT_PALETTE[3], fontPal, fontPalLen);
	memcpy(VRAM_H_EXT_PALETTE[1], fontPal, fontPalLen);
	memcpy(VRAM_H_EXT_PALETTE[0], topscreenPal, topscreenPalLen);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	
	//otherwise the topscreen would be the mainscreen
	lcdSwap();
	//make sure console works
	ECHO_PRINT("console init\n");
	//set height to local cache (even though it's always 256x192)
	my_width = w;
	my_height = h;
#elif ECHO_PC
	//init
	glutInit(&argc, argv);
	//even thought no color is use, alpha is used for fading fonts and character guy
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	//init window
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("L-Echo");
	//assign handlers
	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&key);
	glutSpecialFunc(&spec_key);
	glutMouseFunc(&mouse);
	glutMotionFunc(&pointer);
	//basic stuff
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	//set our line
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.5);
	
	//blending for fading effects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	resize(w, h);
#elif ECHO_GCN || ECHO_WII
	//TODO: Wii/GCN port?
#endif
}

static void resize(int w, int h)
{
	//can't have div by 0
	if(h == 0)	h = 1;
	
	//change our viewport (window)
	glViewport(0, 0, w, h);
	
	my_width = w;
	my_height = h;
	
	//figure out the dimensions of our projection
	if(w <= h)
        {
                real_width = depth;
                real_height = depth * w / h;
        }
        else
        {
                real_width = depth * w /  h;
                real_height = depth;
        }
	
	//change to projection mode
	glMatrixMode(GL_PROJECTION);
	//load an identity matrix
	glLoadIdentity();
	//orthogonal projection or else it would look weird and you couldn't play this game
        glOrtho(-real_width, real_width, -real_height, real_height, -depth, depth);
	//shift back
	glMatrixMode(GL_MODELVIEW);
}

// ----DRAW MAIN----

#ifdef ECHO_NDS
	static void console2_draw_string(int x, int y, char* str)
	{
		while(*str != '\0')
		{
			string_map[POS2IDX(x, y)] = CHAR2TILE(*str);
			x++;
			str++;
		}
	}
	static void console2_draw_string(int x, int y, char* str, int num)
	{
		int i = 0;
		while(*str != '\0' && i < num)
		{
			string_map[POS2IDX(x, y)] = CHAR2TILE(*str);
			x++;
			str++;
			i++;
		}
	}
	static void console2_clear()
	{
		//32 rows (only 24 are displayed without shifting it) by 32 columns
		memset((void*)string_map, '\0', 1024 * sizeof(u16));
	}
	static void console2_clear_row(int y)
	{
		memset((void*)(string_map + (y * 32)), '\0', 32 * sizeof(u16));
	}
	
	static void update_loader()
	{
		console2_clear();
		//draw the current directory on top
		console2_draw_string(0, 0, files->current_dir, 16);
		
		int each_file = 0;
		while(each_file < NUM_FILES_DISPLAYED)
		{
			//display an arrow next to the currently selected file
			if(file_start + each_file == file_index)
				console2_draw_string(0, 2 + each_file, "->");
			console2_draw_string(2, 2 + each_file, files->file_names[file_start + each_file], 30);
			each_file++;
		}
	}
	
	static void toggle_info()
	{
		console2_clear();
		//draw the stage name, since it's not going to change unless you switch to the loader
		if(echo_ns::current_stage)
		{
			console2_draw_string(0, 0, "stage: ");
			console2_draw_string(7, 0, name_cache, 25);
			update_num_goals();
			update_char_state();
		}
		else
		{
			console2_draw_string(0, 0, "no stage loaded");
		}
	}
	static void update_num_goals()
	{
		console2_clear_row(10);	console2_clear_row(11);
		console2_clear_row(12);	console2_clear_row(13);
		if(echo_ns::current_stage)
		{
			int goals_left = echo_ns::goals_left();
			if(goals_left > 0)
			{
				//hmms, unlikely to buffer overflow, but what can I do about it anyways?
				//and who the hell will have more than 10^32 goals?
				//can that even fit in an int?
				sprintf(counter, "%i", goals_left);
				console2_draw_string(0, 10, COUNTER_HEAD);
				console2_draw_string(0, 12, counter, 32);
			}
			//yay you finished!
			else if(echo_ns::num_goals())
				console2_draw_string(0, 11, SUCCESS);
			//the stage had no goals to begin with
			else
				console2_draw_string(0, 11, NO_GOALS);
		}
		else
			console2_draw_string(0, 11, NO_GOALS);
	}
	static void update_char_state()
	{
		console2_clear_row(22);	console2_clear_row(23);
		if(echo_ns::current_stage)
			console2_draw_string(0, 22, message);
		else
			console2_draw_string(0, 22, "please load a stage");
	}
#elif ECHO_PC
	//copied from http://lighthouse3d.com/opengl/glut/index.php?bmpfontortho
	static void draw_message_string(float x, float y, char *string)
	{
		char *c = string;
		while(*c != '\0')
		{
			glRasterPos2f(x, y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
			//twice as spaced out as the other strings
			x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) / font_div * 2;
			c++;
		}
	}
	
	static void draw_string(float x, float y, const char *string)
	{
		int i = 0;
		while(string[i] != '\0')
		{
			glRasterPos2f(x, y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
			x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, string[i]) / font_div;
			i++;
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
	
	static void draw_HUD()
	{
		//status
		
		if(message == MSG_START)
		{
			//fade...
			if(start_frame > START_MAX)
				message = MSG_BLANK;
			else
			{
				glColor4f(0, 0, 0, 1.0f - start_frame * 1.0f / START_MAX);
				start_frame++;
			}
		}
		else if(message == MSG_PAUSE)
		{
			//make it pulsate, reusing variables ftw!
			glColor4f(0, 0, 0, 0.4f * sin(TO_RAD(start_frame)) + 0.6f);
			start_frame += 10;
			//prevent overflow
			if(start_frame == 36)
				start_frame = 0;
		}
		
		//near the bottom left of the stage
		draw_message_string(-0.8f * real_width, -0.9f * real_height, message);
		
		//name of the stage
		
		if(name_display > 0)
		{
			glColor3f(0, 0, 0);
			
			if(name_display < NAME_DISPLAY_MAX)
			{
				//fading
				glColor4f(0, 0, 0, name_display * 1.0f / NAME_DISPLAY_MAX);
				name_display--;
			}
			
			//left side
			draw_string(-0.9f * real_width, 0, name_cache);
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
		//bottom left, above status
		draw_string(-0.6f * real_width, -0.8f * real_height, counter);
		if(goals_left > 0)
			delete counter;
	}
	static void draw_loader()
	{
		//if loading or the loader just isn't fully tucked away yet
		if(loading || load_frame > 0)
		{
			//loader is being tucked away
			if(!loading)
				load_frame--;
			//loader is coming out
			else if(load_frame < LOAD_MAX)
				load_frame++;
			
			glLoadIdentity();
			//translate the whole thing
			glTranslatef(real_width * (1 - LOADER_WIDTH * load_frame / LOAD_MAX)
					, 0, depth - 0.1f);
			//the background of the loader
			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex3f(0, real_height, 0);
			glVertex3f(real_width * LOADER_WIDTH, real_height, 0);
			glVertex3f(real_width * LOADER_WIDTH, -real_height, 0);
			glVertex3f(0, -real_height, 0);
			glEnd();
			
			//slightly in front
			glTranslatef(0, 0, 0.05f);
			//the current directory
			glColor3f(1, 1, 1);
			draw_string(0, real_height - 0.4f, files->current_dir);
			
			//grey for files that are not selected
			glColor3f(0.5f, 0.5f, 0.5f);
			int each_file = 0;
			//start 3 spaces from the top.
			float each_y = real_height - 3 * file_space;
			//while the each_y is less than the bottom,
			//each_file less than the number of files supposed to be displayed on the screen
			//and less than the number of files we actually have
			while(each_y >= -real_height && each_file < NUM_FILES_DISPLAYED 
				&& each_file < files->num_files)
			{
				//highlight the selected
				if(file_start + each_file == file_index)
					glColor3f(1, 1, 1);
				draw_fname_string(0, each_y, files->file_names[file_start + each_file]);
				//shift the color back
				if(file_start + each_file == file_index)
					glColor3f(0.5f, 0.5f, 0.5f);
				each_y -= file_space;
				each_file++;
			}
		}
	}
#endif

static void display()
{
#ifndef ECHO_NDS
	//clear color and depth buffer, nds does this automatically at glFlush(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//load identity
	glLoadIdentity();
	//draw hud if not in menu mode
	if(!menu_mode)
		draw_HUD();
#else
	//just keep the 
	if(message == MSG_START)
	{
		if(start_frame > START_MAX)
			message = MSG_BLANK;
		else
			start_frame++;
	}
#endif
	//reset anain
	glLoadIdentity();
	//rotate to the angle (forgot why negative)
	gfx_rotatef(-echo_ns::angle.x, 1, 0, 0);
	gfx_rotatef(-echo_ns::angle.y, 0, 1, 0);
	
	//echo_ns::angle.dump();
	
	//draw the world
	if(!menu_mode)
		echo_ns::draw();
#ifndef ECHO_NDS
	//draw the menu
	else
	{
		glColor3f(0, 0, 0);
		draw_message_string(-2, 3, "L-Echo");
		draw_string(-3, -0.5, "Please load a stage.");
		draw_string(-6, -1.5, "Press L To Toggle Loader.");
		draw_string(-6, -2, "Press P To Start/Pause/Resume.");
		draw_string(-6, -2.5, "Press W To Walk.");
		draw_string(-6, -3, "Press R to Run.");
		draw_string(-6, -3.5, "Press Arrow Keys or use Mouse to ");
		draw_string(-3, -4, "Rotate World.");
		draw_string(-6, -4.5, "Press Esc To Quit.");
	}
	
	//draw the loader
	draw_loader();
	
	//display
	glutSwapBuffers();
	//for 30fps (need real fps mechanism here)
	ECHO_SLEEP(16);
#endif
}

// ----CONTROLS---

#ifdef ECHO_NDS
	static void get_key()
	{
		//make libnds update its cache
		scanKeys();
		//get keys
		const u16 key = keysDown();
		
		//because we'd be just wasting time on atan2
		if(!menu_mode)
		{
			//handle touch
			if(keysHeld() & KEY_TOUCH)
			{
				touchPosition t_pos = touchReadXY();
				if(touch_started)	pointer(t_pos.px, t_pos.py);
				else			pressed(t_pos.px, t_pos.py);
			}
			else if(touch_started)
				touch_started = 0;
			if(key & KEY_LID && !echo_ns::is_paused())
			{
				message = MSG_PAUSE;
				echo_ns::toggle_pause();
			}
		}
		
		//if loading, don't care if menu_mode or not (because otherwise we would be able to load)
		if(sub_mode == NDS_LOAD_MODE)
		{
			//load or open directory
			if((key & KEY_L) || (key & KEY_R))
			{
				//load stage file
				if(!is_dir(files, file_index))
					load(files->file_names[file_index]);
				//open the directory
				else
				{
					//previous dir
					if(!strcmp(files->file_names[file_index], ".."))
					{
						//if not root
						if(strcmp(files->current_dir, "/"))
						{
							//index of last '/' character
							int len =  strrchr(files->current_dir, '/') - files->current_dir;
							//not root
							if(len > 0)
							{
								char* dir = new char[len + 1];
								CHKPTR(dir);
								//cut off last '/' and after
								strncpy(dir, files->current_dir, len);
								delete files;
								files = get_files(dir);
							}
							//going to root
							else if(len == 0)
							{
								delete files;
								files = get_files("/");
							}
						}
					}
					//normal dir
					else
					{
						const char* new_dir = echo_merge(files->current_dir, files->file_names[file_index]);
						delete files;
						files = get_files(new_dir);
					}
					//reset loader
					//if the user tries to go to parent dir from root, his file_index and file_start are both 0 anyways, so no prob
					file_index = 0;
					file_start = 0;
				}
			}
			if(((key & KEY_DOWN)	|| (key & KEY_B)) && file_index < files->num_files - 1)
				file_index++;
			if(((key & KEY_UP)	|| (key & KEY_X)) && file_index > 0)
				file_index--;
			file_start = file_index - 21;
			if(file_start < 0)
				file_start = 0;
		}
		else if(!menu_mode)
		{
			if((key & KEY_L) || (key & KEY_R))
							start_or_pause();
			if(key & right)	right();
			if(key & left)	left();
			if(key & down)	down();
			if(key & up)	up();
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
			//no more keys on nds = sharing
			ECHO_PRINT("angle: %f, %f\n" , echo_ns::angle.x, echo_ns::angle.y);
		}
		if(sub_mode == NDS_INFO_MODE)
		{
			update_num_goals();
			update_char_state();
		}
		else if(sub_mode == NDS_LOAD_MODE)
			update_loader();
		//The DS doesn't have a FPU, so it isn't very precise.  Maybe I should switch angle to int?
		echo_ns::angle.x = (int)echo_ns::angle.x;
		echo_ns::angle.y = (int)echo_ns::angle.y;
	}
#elif defined(ECHO_PC)
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
	static void key(unsigned char key, int x, int y)
	{
		if(key == ESCAPE)
		{
			glutDestroyWindow(window);
			std::exit(0);
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
					load(file);
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
				//if not started or already paused
				if(message != MSG_READY && !echo_ns::is_paused())
				{
					echo_ns::toggle_pause();
					was_paused = 1;
				}
				else
					was_paused = 0;
				loading = 1;
			}
			else
			{
				//because unexpectedly unpausing would be really bad for someone pausing
				//to load then deciding to finish the level
				if(was_paused)
					echo_ns::toggle_pause();
				loading = 0;
			}
		}
		else if(key == 'r' || key == 'R')
			echo_ns::start_run();
		else if(key == 'w' || key == 'W')
			echo_ns::start_step();
		else if(key == 's' || key == 'S')
			ECHO_PRINT("speed: %f\n", echo_ns::get_speed());
		else if(key == 'a' || key == 'A')
		{
			//dump the angle
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
				if(key == GLUT_KEY_RIGHT)	right();
				else if(key == GLUT_KEY_LEFT)	left();
				else if(key == GLUT_KEY_DOWN)	down();
				else if(key == GLUT_KEY_UP)	up();
			}
		}
		else
		{
			if(key == GLUT_KEY_UP && file_index > 0)
				file_index--;
			else if(key == GLUT_KEY_DOWN && file_index < files->num_files - 1)
				file_index++;
			file_start = file_index - NUM_FILES_DISPLAYED + 1;
			if(file_start < 0)
				file_start = 0;
		}
	}
#endif

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
	start_frame = 0;
	if(message == MSG_READY)
	{
		message = MSG_START;
		echo_ns::start();
#ifndef ECHO_NDS
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

