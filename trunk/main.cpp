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

//standard libraries
#include <cstdlib>
#include <cstring>
#include <cmath>

//most of the headers
#include <echo_platform.h>
#include <echo_xml.h>
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
//#include <echo_mp3.h>
#include <echo_char_joints.h>
//various grids
#include <hole.h>
#include <grid.h>
#include <escgrid.h>
//#include <static_grid.h>
//#include <isect_grid.h>
#include <t_grid.h>

//for Windows, or something
#define _STDCALL_SUPPORTED

#ifdef ECHO_NDS
	//basic nds stuff
	#include <nds.h>
	//the video stuff for opengl
	#include <nds/arm9/video.h>
	//to initialize FAT for file reading
	#include <fat.h>
	//aslib: Advanced Sound LIBrary, for mp3 and sfx playback
	//#include <as_lib9.h>
	
	//resources: the console font, topscreen display, and the menu
	//#include "font.h"
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
	
	//*
	//modes for the topscreen
	#define NDS_INFO_MODE		-2
	#define NDS_LOAD_MODE		-1
	#define NDS_START_MODE		0
	//#define NDS_DEBUG_MODE		1
	
	//the range of the modes
	#define NDS_MODE_MIN		-2
	#define NDS_MODE_MAX		0
	
	//the background values for the vaious topscreen modes
	//load and info both use the console (BG1)
	#define LOAD_BG			DISPLAY_BG0_ACTIVE
	#define INFO_BG			DISPLAY_BG0_ACTIVE
	#define START_BG		DISPLAY_BG3_ACTIVE
	//#define DEBUG_BG		DISPLAY_BG0_ACTIVE
	// */
#else
	#ifdef ECHO_PC
		//opengl
		#ifdef ECHO_OSX	//OS X
			#include <OpenGL/gl.h>
			#include <OpenGL/glu.h>
			#include <GLUT/glut.h>
		#else
			#include <GL/glut.h>
			#include <GL/gl.h>
			#include <GL/glu.h>
		#endif
		
		//used to attach the signal handler
		#include <signal.h>
		//for directory stuff
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
#define SUCCESS         "success"
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
	//static u16* string_map = NULL;
	
	static int controls_bg = -1;
	static PrintConsole console, info;
	
	//basic flags of the top screen
	static const uint32 basic_modes = MODE_5_2D;
	//the temp address of the counter (holds number of goals)
	static char counter[32];
	//what are the directional buttons?  defaults to right-scheme
	static int left_key = KEY_LEFT, right_key = KEY_RIGHT, up_key = KEY_UP, down_key = KEY_DOWN, b_key = KEY_B;
#else
	#ifdef ECHO_PC
		//ID of the window
		static int window;
	#endif
	//the previous time "display" was called
	static int prev_time = 0;
	//is the loader toggled?; which frame is the loader in?
	static int loading = 0, load_frame = 0;
	//the temp address of the counter (holds number of goals)
	static char* counter = NULL;
	//was the counter allocated?
	static int counter_alloc = 0;
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
	
	//tries to refresh the handedness
	static void refresh_hand(echo_xml* doc);
#elif ECHO_PC
	//mouse event, calls pressed or handles releases.
	static void mouse(int button, int state, int x, int y);
	//normal alphanumeric keys, such as p, l, a
	static void key(unsigned char key, int x, int y);
	//special keys esp. arrow keys
	static void spec_key(int key, int x, int y);
	//draw a string on screen
	static int draw_string(float x, float y, const char *string);
	static int draw_string(float x, float y, char *string);
	//draw a file name (using size 12 font instead of size 12)
	static int draw_fname_string(float x, float y, char *string);
	//draw the status (twice as spaced out)
	static int draw_message_string(float x, float y, char *string);
#endif
//mouse dragged
static void pointer(int x, int y);
//mouse pressed down
static void pressed(int x, int y);
//load the file
static void load(const char* fname);
//initialize
static void init(int argc, char **argv, int w, int h);
//resize the screen
static void resize(int w, int h);
//display the screen
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

static void main_deallocate();
static void signal_handler(int signal);

int main(int argc, char **argv)
{
	atexit(main_deallocate);
#ifdef ECHO_NDS
	srand(time(0));
	//initialize the file system
	fatInitDefault();
	char* root = NULL;
	if(echo_genroot(&root) == FAIL)
		echo_error("Cannot allocate path string!");
	//get the files
	files = get_files(root);
	//initialize the screens
	init(argc, argv, 255, 191);
	
	//*
	ECHO_PRINT("trying to load prefs...\n");
	echo_xml** doc = new(echo_xml*);
	CHKPTR(doc);
	if(open_prefs(doc) == WIN)
	{
		ECHO_PRINT("loaded prefs...\n");
		refresh_hand(*doc);
		close_prefs(*doc);
	}
	else
		ECHO_PRINT("couldn't load prefs!\n");
	delete doc;
	// */
	
	//load the menu
	load(NULL);
	defaultExceptionHandler();
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
        //for 30 fps, I need to skip a frame
        swiWaitForVBlank();
	}
#elif ECHO_PC
	//fill lookup tables
	init_math();
	
	char* dir = NULL;
	if(echo_execdir(&dir) == FAIL)
		echo_error("couldn't get executable directory!!!\n");
	files = get_files(dir);
	
	//attach the signal handler
	signal(SIGINT, signal_handler);
	
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
			const stage* st = load_stage(argv[2]);
			//test the file
			if(st != NULL)
			{
				//ok
				ECHO_PRINT("stage file OK\n");
				delete st;
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
	//initialize prev time
	prev_time = glutGet(GLUT_ELAPSED_TIME);
	//start main loop
	glutMainLoop();
#elif ECHO_GCN || ECHO_WII
	//TODO: Wii/GCN port?
#endif
	//never gonna reach here
	return(1);
}

void main_deallocate()
{
	ECHO_PRINT("main_deallocate: deallocating echo_ns\n");
	echo_ns::deallocate();
	ECHO_PRINT("main_deallocate: finished deallocating echo_ns\n");
#ifndef ECHO_NDS
	if(counter_alloc == 1 && counter != NULL)
		delete[] counter;
#endif
	ECHO_PRINT("main_deallocate: deallocating files\n");
	delete_echo_files(files);
	ECHO_PRINT("main_deallocate: exiting...\n");
}

void signal_handler(int signal)
{
	//we have already called atexit, so we don't need to call main_deallocate again
	std::exit(signal);
}

#ifdef ECHO_NDS
void refresh_hand(echo_xml* doc)
{
	HAND hand = RIGHT_HAND;
	ECHO_PRINT("get hand started\n");
	if(get_hand(doc, &hand) == WIN)
	{
		ECHO_PRINT("get hand ended\n");
		if(hand == RIGHT_HAND)
		{
			ECHO_PRINT("right handed\n");
			left_key = KEY_LEFT;
			right_key = KEY_RIGHT;
			up_key = KEY_UP;
			down_key = KEY_DOWN;
			b_key = KEY_B;
		}
		else
		{
			ECHO_PRINT("left handed\n");
			left_key = KEY_Y;
			right_key = KEY_A;
			up_key = KEY_X;
			down_key = KEY_B;
			b_key = KEY_DOWN;
		}
	}
	else
		ECHO_PRINT("couldn't get handedness!\n");
}
#endif

static void load(const char* fname)
{
	ECHO_PRINT("start of load\n");
	stage* s = NULL;
	if(fname != NULL)
	{
		ECHO_PRINT("before load_stage:%s,%s\n", files->current_dir, fname);
		char* abs_path = echo_merge(files->current_dir, fname);
		//load stage
		ECHO_PRINT("starting load\n");
		s = load_stage(abs_path);
		delete[] abs_path;
		ECHO_PRINT("after load_stage\n");
	}
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
			videoSetMode(MODE_5_3D);
#endif
			//not in menu mode
			menu_mode = 0;
			//the distance to the farthest point plus 2.5 for good measure.
			depth = echo_ns::current_stage->get_farthest() + 5;
			//so i don't have to call it all the time
			name_cache = const_cast<char*>(echo_ns::current_stage->get_name()->c_str());
		}
		//if we want our menu
		else
		{
			//init to null
			echo_ns::init(NULL);
#ifdef ECHO_NDS
			//display the menu
			videoSetMode(MODE_5_3D | DISPLAY_BG3_ACTIVE);
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
			case NDS_LOAD_MODE:		videoSetModeSub(MODE_5_2D | DISPLAY_BG1_ACTIVE);	bgHide(controls_bg);		update_loader();	break;
			case NDS_INFO_MODE:		videoSetModeSub(MODE_5_2D | DISPLAY_BG1_ACTIVE);	bgHide(controls_bg);		toggle_info();		break;
			case NDS_START_MODE:	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);	bgShow(controls_bg);							break;
			default:
			break;
		}
	}
#endif

static void init(int argc, char **argv, int w, int h)
{
#ifdef ECHO_NDS
        
	//assign banks (H is use for ext. palette)
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_LCD);
	vramSetBankE(VRAM_E_LCD);
	vramSetBankF(VRAM_F_LCD);
	vramSetBankG(VRAM_G_LCD);
	vramSetBankH(VRAM_H_LCD);
	vramSetBankI(VRAM_I_LCD);
	
	//otherwise the topscreen would be the mainscreen
	lcdSwap();
	
	//Main Screen
	
	//set mode
	videoSetMode(MODE_5_3D | DISPLAY_BG3_ACTIVE);
	
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
	
	int menu_bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 5, 0);
	dmaCopy(menuBitmap, bgGetGfxPtr(menu_bg), menuBitmapLen);
	dmaCopy(menuPal, BG_PALETTE, menuPalLen);
	bgSetPriority(menu_bg, 3);
	
	//Sub Screen
	
	videoSetModeSub(MODE_5_2D);
	
	controls_bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 5, 0);
	bgSetPriority(controls_bg, 3);
	dmaCopy(topscreenBitmap, bgGetGfxPtr(controls_bg), topscreenBitmapLen);
	dmaCopy(topscreenPal, BG_PALETTE_SUB, topscreenPalLen);
	
	console = *consoleInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 2, false);
	info = *consoleInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false);
	
	consoleSetWindow(&info,		16, 0, 	16, 24);
    consoleSetWindow(&console,	0, 0, 	16, 24);

	
    char *border = 
	"----------------"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"|              |"
	"----------------";
	
    consoleSelect(&info);
    iprintf(border);
    consoleSelect(&console);
    iprintf(border);
    consoleSetWindow(&info,		17, 1,	14, 22);
    consoleSetWindow(&console,	1, 1,	14, 22);

	refresh_sub_mode();
	
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
	//*
	static void console2_draw_string(int x, int y, char* str)
	{
		ECHO_PRINT("\x1b[%d;%dH%s", y, x, str);
	}
	static void console2_draw_string(int x, int y, char* str, int num)
	{
		int len = strlen(str);
		if(len > num)
			len = num;
		char* s_tmp = new char[len + 1];
		memset(s_tmp, 0, sizeof(char) * (len + 1));
		memcpy(s_tmp, str, sizeof(char) * len);
		//ECHO_PRINT("%s\n", fmt);
		ECHO_PRINT("\x1b[%d;%dH%s", y, x, s_tmp);
		delete s_tmp;
	}
	static void console2_clear()
	{
		//32 rows (only 24 are displayed without shifting it) by 32 columns
		//memset((void*)string_map, '\0', 1024 * sizeof(u16));
		ECHO_PRINT("\x1b[2J");
	}
	static void console2_clear_row(int y)
	{
		//memset((void*)(string_map + (y * 32)), '\0', 32 * sizeof(u16));
		ECHO_PRINT("\x1b[%d;0H          ", y);
	}
	// */
	static void update_loader()
	{
		consoleSelect(&info);
		console2_clear();
		//draw the current directory on top
		console2_draw_string(0, 0, files->current_dir, 14);
		
		int each_file = 0;
		while(each_file < files->num_files && each_file < 20)
		{
			//display an arrow next to the currently selected file
			if(file_start + each_file == file_index)
				console2_draw_string(0, 2 + each_file, "->");
			console2_draw_string(2, 2 + each_file, files->file_names[file_start + each_file], 12);
			each_file++;
		}
		consoleSelect(&console);
	}
	
	static void toggle_info()
	{
		consoleSelect(&info);
		console2_clear();
		//draw the stage name, since it's not going to change unless you switch to the loader
		if(echo_ns::current_stage != NULL)
		{
			console2_draw_string(0, 0, "stage: ");
			console2_draw_string(0, 1, name_cache, 14);
			update_num_goals();
			update_char_state();
		}
		else
		{
			console2_draw_string(0, 0, "no stage loaded");
		}
		consoleSelect(&console);
	}
	static void update_num_goals()
	{
		//console2_clear_row(10);	console2_clear_row(11);
		//console2_clear_row(12);	console2_clear_row(13);
		if(echo_ns::current_stage != NULL)
		{
			int goals_left = echo_ns::goals_left();
			if(goals_left > 0)
			{
				//hmms, unlikely to buffer overflow, but what can I do about it anyways?
				//and who the hell will have more than 10^32 goals?
				//can that even fit in an int?
				sprintf(counter, "%i", goals_left);
				console2_draw_string(0, 10, COUNTER_HEAD);
				console2_draw_string(0, 12, counter, 14);
			}
			//yay you finished!
			else if(echo_ns::num_goals() > 0)
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
		//console2_clear_row(21);	//console2_clear_row(22);
		if(echo_ns::current_stage != NULL)
			console2_draw_string(0, 21, message);
		else
			console2_draw_string(0, 21, "please load a stage");
	}
#elif ECHO_PC
	//copied from http://lighthouse3d.com/opengl/glut/index.php?bmpfontortho
	static int draw_message_string(float x, float y, char *string)
	{
		if(string)
		{
			while(*string != '\0')
			{
				glRasterPos2f(x, y);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string);
				//twice as spaced out as the other strings
				x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *string) / font_div * 2;
				string++;
			}
			return(WIN);
		}
		return(FAIL);
	}
	
	static int draw_string(float x, float y, const char *string)
	{
		if(string)
		{
			int i = 0;
			while(string[i] != '\0')
			{
				glRasterPos2f(x, y);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
				x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, string[i]) / font_div;
				i++;
			}
			return(WIN);
		}
		return(FAIL);
	}
	
	static int draw_string(float x, float y, char *string)
	{
		if(string)
		{
			while(*string != '\0')
			{
				glRasterPos2f(x, y);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string);
				//twice as spaced out as the other strings
				x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *string) / font_div;
				string++;
			}
			return(WIN);
		}
		return(FAIL);
	}
	
	static int draw_fname_string(float x, float y, char *string)
	{
		if(string)
		{
			while(*string != '\0')
			{
				glRasterPos2f(x, y);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string);
				//twice as spaced out as the other strings
				x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, *string) / font_div;
				string++;
			}
			return(WIN);
		}
		return(FAIL);
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
			counter_alloc = 1;
			CHKPTR(counter);
			sprintf(counter, COUNTER_HEAD, goals_left);
		}
		else if(echo_ns::num_goals())
		{
			counter = SUCCESS;
			counter_alloc = 0;
		}
		else
		{
			counter = NO_GOALS;
			counter_alloc = 0;
		}
		
		glColor3f(0, 0, 0);
		//bottom left, above status
		draw_string(-0.6f * real_width, -0.8f * real_height, counter);
		if(goals_left > 0)
		{
			if(counter_alloc == 1)
				delete[] counter;
			counter = NULL;
		}
	}
	static void draw_loader()
	{
		//if loading or the loader just isn't fully tucked away yet
		if(loading || load_frame > 0)
		{
			//loader is being tucked away
			if(loading == 0)
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
			{
				glVertex3f(0, real_height, 0);
				glVertex3f(real_width * LOADER_WIDTH, real_height, 0);
				glVertex3f(real_width * LOADER_WIDTH, -real_height, 0);
				glVertex3f(0, -real_height, 0);
			}
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
	
	int elapsed = glutGet(GLUT_ELAPSED_TIME) - prev_time;
	if(elapsed < WAIT)
	{
		ECHO_SLEEP(WAIT - elapsed);
		//ECHO_PRINT("not fskip\n");
	}
	//frameskip =(
	//else
	//	ECHO_PRINT("fskip: %d\n", elapsed);
	prev_time = glutGet(GLUT_ELAPSED_TIME);
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
				{
					load(files->file_names[file_index]);
				}
				//open the directory
				else
				{
					//*
					//previous dir
					if(!strcmp(files->file_names[file_index], ".."))
					{
						char* dir = NULL;
						if(echo_parentdir(files->current_dir, &dir) == WIN)
						{
							delete_echo_files(files);
							files = get_files(dir);
						}
						else
						{
							echo_error("File parentdir error\n");
						}
					}
					//normal dir
					else
					{
						const char* new_dir = echo_merge(files->current_dir, files->file_names[file_index]);
						delete_echo_files(files);
						files = get_files(new_dir);
					}
					//reset loader
					//if the user tries to go to parent dir from root, his file_index and file_start are both 0 anyways, so no prob
					file_index = 0;
					file_start = 0;
					// */
				}
			}
			if((key & down_key) && file_index < files->num_files - 1)
				file_index++;
			if((key & up_key) && file_index > 0)
				file_index--;
			file_start = file_index - 19;
			if(file_start < 0)
				file_start = 0;
		}
		else if(!menu_mode)
		{
			if((key & KEY_L) || (key & KEY_R))
							start_or_pause();
			if(key & right_key)	right();
			if(key & left_key)	left();
			if(key & down_key)	down();
			if(key & up_key)	up();
			if(key & b_key)		echo_ns::toggle_run();
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
			toggle_info();
			/*
			update_num_goals();
			update_char_state();
			*/
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
					char* dir = NULL;
					if(!strcmp(file, ".."))
					{
						if(echo_parentdir(files->current_dir, &dir) == FAIL)
						{
							dir = new char[strlen(files->current_dir) + 1];
							CHKPTR(dir);
							strcpy(dir, files->current_dir);
						}
					}
					else
					{
						dir = echo_merge(files->current_dir, file);
					}
					delete_echo_files(files);
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
		else if(key == 't' || key == 'T')
			echo_ns::toggle_run();
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

