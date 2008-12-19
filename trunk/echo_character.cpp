// echo_character.cpp

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

/* Your standard libraries
 * typeinfo is used to find if the grid is a hole, so the character can fall
 * through the hole if the first grid is one.
 */
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <cmath>
#include <cfloat>

/* L-Echo libraries (not grids)
 */
#include <echo_platform.h>
#include <echo_sys.h>
#include <echo_gfx.h>
#include <echo_debug.h>
#include <echo_math.h>
#include <echo_ns.h>
#include <echo_character.h>
#include <echo_char_joints.h>
#include <echo_stage.h>

/* grids; just launchers and holes are discriminated against, because
 * the character is responsible for flying and falling
 */
#include <launcher.h>
#include <grid.h>
#include <hole.h>

//need to measure the body sizes in order to do IK correctly
#include <gen/gen.h>

//how high aboce the start grid does the character start?
#define STARTY		10

//the acceleration constant
#define ACCEL		15.0f

//various character speeds
//percentage (we change the weight from grid1 to grid2 to make the character go in grid mode)
#define SPEED_STEP 			0.07f
#define SPEED_RUN			0.25f
//actual speeds (in flying/falling mode)
#define SPEED_FALL 			0.00f
#define SPEED_LAUNCH		0.30f
#define SPEED_FALL_FROM_SKY	0.50f


echo_char::echo_char(grid* g1)
{
	/// num_goals is the total number of goals this character has passed; shouldn't be reset
	num_goals = 0;
	/// initialize
	init(g1);
}

echo_char::~echo_char()
{
	/// fall_position is the only dynamically constructed member
	if(fall_position != NULL)
	{
		delete fall_position;
		fall_position = NULL;
	}
}

/** @return If the character is paused; should actually relegate to echo_ns.
 */
int echo_char::is_paused()
{
	return(paused);
} 

/** @return How many goals (echos, in echochrome-speak) the character has reached.
 */
int echo_char::num_goals_reached()
{
	return(num_goals);
}

/** Start falling from the given position, or where grid1 is.
 * @param pos An arbitrary position to fall from.  If this is NULL, then grid1's position will be used
 */
void echo_char::initialize_falling(vector3f* pos)
{
	ECHO_PRINT("initialize falling...%p, %p\n", grid1, grid2);
	if(pos != NULL)
	{
		///fall_position is used as the position if the camera angle is (0, 0, 0)
		fall_position = pos->neg_rotate_yx(echo_ns::angle);
	}
	else if(grid1 != NULL)
	{
		grid_info_t* i1 = grid1->get_info(echo_ns::angle);
		if(i1 != NULL)
		{
			fall_position = i1->pos->neg_rotate_yx(echo_ns::angle);
		}
	}
	else
		echo_error("Cannot find where the character is falling from; quitting\n");
	speed = SPEED_FALL;
	if(actual_speed == 0)
		actual_speed = SPEED_FALL;
}

/** Falling from the sky, at the start of the stage.
 */
void echo_char::initialize_fall_from_sky()
{
	ECHO_PRINT("initialize falling from sky...\n");
	if(grid1 != NULL)
	{
		grid_info_t* i1 = grid1->get_info(echo_ns::angle);
		if(i1 != NULL)
		{
			target_y = i1->pos->y;
			fall_position = new vector3f();
			fall_position->set(i1->pos);
			fall_position->y += STARTY;
		}
	}
	else
		echo_error("Cannot find where the character is falling from; quitting\n");
	speed = SPEED_FALL_FROM_SKY;
	actual_speed = SPEED_FALL_FROM_SKY;
}

void echo_char::change_speed()
{
	if(grid1 != NULL)
	{
		//first grid is an hole, and second is an isect_grid or just falling nowhere
		if(typeid(*grid1) == typeid(hole) && grid2 == NULL)
		{
			ECHO_PRINT("falling into hole...\n");
			initialize_falling(NULL);
		}
		//first grid is a launcher, and second is an static_grid or just falling nowhere
		else if(typeid(*grid1) == typeid(launcher) && grid2 == NULL)
		{
			ECHO_PRINT("being launched!\n");
			speed = SPEED_LAUNCH;
			actual_speed = SPEED_LAUNCH;
		}
		//if this character isn't in midair
		else if(speed == SPEED_FALL || speed == SPEED_LAUNCH || speed == SPEED_FALL_FROM_SKY)
		{
			ECHO_PRINT("normal speed\n");
			speed = is_running ? SPEED_RUN : SPEED_STEP;
			actual_speed = 0;
		}
	}
}

void echo_char::start_run()
{
	if(speed == SPEED_STEP)
	{
		is_running = 1;
		speed = SPEED_RUN;
	}
}

void echo_char::start_step()
{
	if(speed == SPEED_RUN)
	{
		is_running = 0;
		speed = SPEED_STEP;
	}
}

void echo_char::toggle_run()
{
	if(speed == SPEED_RUN)
		start_step();
	else
		start_run();
}

void echo_char::init(grid* g1)
{
	start = g1;
	
	//are or were we running?
	is_running = 0;
	paused = 0;
	
	land(g1, false);
	initialize_fall_from_sky();
	
	reset_joints(&joints);
}

void echo_char::land(grid* g1, int do_change_speed)
{
	grid1 = g1;
	grid2 = g1 ? grid1->get_next(echo_ns::angle, grid1) : NULL;
	fall_position = NULL;
	
	//haven't started falling out of the sky
	grid1per = 1;
	//grid2per = 0;
	
	//distance between grids
	dist = 1;
	
	//stuff for walking
	dist_traveled = 0;
	dist_traveled_cyclic = 0;
	
	//if(g1 != NULL)
	if(do_change_speed == true)
	{
		change_speed();
	}
}

void echo_char::toggle_pause()
{
	paused = !paused;
}

void echo_char::reset()
{
	init(start);
}

void echo_char::next_grid()
{
	if(grid1->is_goal(echo_ns::angle))
	{
		grid1->toggle_goal(echo_ns::angle);
		num_goals++;
	}
	if(grid2 != NULL)
	{
		//replace grid1 with grid2, grid2 with the next grid
		grid* temp = grid2;
		grid2 = grid2->get_next(echo_ns::angle, grid1);
		grid1 = temp;
		change_speed();
	}
	else
		grid2 = NULL;
	//reset
	grid1per = 1;
	//grid2per = 0;
}

static vector3f* end_pt(vector3f* prev_pos, vector3f* vec, float level_y)
{
	if(vec->y != 0)
	{
		float delta_y = level_y - prev_pos->y;
		if((delta_y > 0 && vec->y < 0) || (delta_y < 0 && vec->y > 0))
			return(NULL);
		vec = (*vec) * (delta_y / vec->y);
	}
	vector3f* ret = *prev_pos + vec;
	delete vec;
	CHKPTR(ret);
	return(ret);
}

static grid* check_level(GRID_PTR_SET* level, vector3f* my_end_pt, vector3f angle)
{
	GRID_PTR_SET::iterator it = level->begin(), end = level->end();
	grid* g = NULL;
	while(it != end)
	{
		g = *it;
		if(g->is_pt_on(angle, my_end_pt))
		{
			return(g);
		}
		it++;
	}
	return(NULL);
}

static grid* check_levels_above(LEVEL_MAP* levels_above, vector3f* pos, vector3f* vec, vector3f angle)
{
	if(levels_above->size() < 1)
		return(NULL);
	LEVEL_MAP::iterator it = levels_above->begin(), end = levels_above->end();
	vector3f* pt = NULL;
	grid* g = NULL;
	while(it != end)
	{
		pt = end_pt(pos, vec, it->first);
		if(pt != NULL)
		{
			g = check_level(it->second, pt, angle);
			delete pt;
		}
		if(g != NULL)
			return(g);
		it++;
	}
	return(NULL);
}

static grid* check_levels_below(LEVEL_MAP* levels_below, vector3f* pos, vector3f* vec, vector3f angle)
{
	if(levels_below->size() < 1)
		return(NULL);
	LEVEL_MAP::iterator it = levels_below->end(), end = levels_below->begin();
	vector3f* pt = NULL;
	grid* g = NULL;
	do
	{
		it--;
		pt = end_pt(pos, vec, it->first);
		if(pt != NULL)
		{
			g = check_level(it->second, pt, angle);
			delete pt;
		}
		if(g != NULL)
			return(g);
	}
	while(it != end);
	return(NULL);
}

void echo_char::step()
{
	gfx_color3f(0.5f, 0.5f, 0.5f);
	if(speed == SPEED_FALL_FROM_SKY)
	{
		draw(fall_position);
		if(fall_position->y < target_y)
		{
			vector3f* fall_pos_ptr = fall_position;
			if(typeid(*grid1) == typeid(hole))
				initialize_falling(fall_position);
			else
				land(grid1, true);
			delete fall_pos_ptr;
		}
		else
		{
			if(!paused)
				///Do the falling...
				fall_position->y -= actual_speed * WAIT / 1000;
		}
		if(!paused)
			actual_speed += ACCEL * WAIT / 1000;
	}
	else if(speed == SPEED_FALL)
	{
		vector3f* relative_pos = fall_position->rotate_xy(echo_ns::angle);
		draw(relative_pos);
		if(relative_pos->y < echo_ns::get_lowest_level() - 5)
		{
			reset();
		}
		else
		{
			//check for available places to fall onto
			grid* cam_grid = NULL;
			vector3f* cam_real = echo_ns::angle.angle_to_real();
			vector3f* cam_vec = (*relative_pos) - cam_real;
			vector3f* neg_cam_vec = cam_vec->negate();
			if(cam_real->y > 0) //viewing downwards
			{
				LEVEL_MAP* levels_below = echo_ns::current_stage->get_levels_lower_than(relative_pos->y);
				cam_grid = check_levels_below(levels_below, relative_pos, cam_vec, echo_ns::angle);
				delete levels_below;
				if(cam_grid == NULL || typeid(*cam_grid) == typeid(hole))
				{
					LEVEL_MAP* levels_above = echo_ns::current_stage->get_levels_higher_than(relative_pos->y);
					cam_grid = check_levels_above(levels_above, relative_pos, neg_cam_vec, echo_ns::angle);
					delete levels_above;
				}
			}
			else //viewing up
			{
				LEVEL_MAP* levels_above = echo_ns::current_stage->get_levels_higher_than(relative_pos->y);
				cam_grid = check_levels_above(levels_above, relative_pos, cam_vec, echo_ns::angle);
				delete levels_above;
				if(cam_grid == NULL || typeid(*cam_grid) == typeid(hole))
				{
					LEVEL_MAP* levels_below = echo_ns::current_stage->get_levels_lower_than(relative_pos->y);
					cam_grid = check_levels_below(levels_below, relative_pos, neg_cam_vec, echo_ns::angle);
					delete levels_below;
				}
			}
			delete cam_real;
			delete neg_cam_vec;
			delete cam_vec;
			if(cam_grid != NULL && typeid(*cam_grid) != typeid(hole))
			{
				
				delete fall_position;
				fall_position = NULL;
				land(cam_grid, true);
			}
			else if(!paused)
			{
				//do the falling...
				relative_pos->y -= actual_speed * WAIT / 1000;
				actual_speed += ACCEL * WAIT / 1000;
				//renew the fall_position
				delete fall_position;
				fall_position = relative_pos->neg_rotate_yx(echo_ns::angle);
			}
		}
		//delete the vector
		delete relative_pos;
	}
	else if(grid1 != NULL)
	{
		if(grid1->is_goal(echo_ns::angle))
		{
			start = grid1;
			grid1->toggle_goal(echo_ns::angle);
			num_goals++;
		}
		if(grid2 != NULL)	//if both grids are there
		{
			grid_info_t* i1 = grid1->get_info(echo_ns::angle);
			if(i1 != NULL)
			{
				vector3f* pos1 = i1->pos;
				if(pos1 != NULL)
				{
					grid_info_t* i2 = grid2->get_info(echo_ns::angle);
					if(i2 != NULL)
					{
						vector3f* pos2 = i2->pos;
						if(pos2 != NULL)
						{
							if(!paused)
							{
								dist_traveled += speed * 2;	//inflate it a little
								dist_traveled_cyclic += speed * 180;
								if(dist_traveled_cyclic > 360)
								{
									dist_traveled -= 4;	
									dist_traveled_cyclic -= 360;	
								}
								dist = pos1->dist(pos2);
								if(dist_traveled > 0.5f && dist_traveled <= 1)
								{
									grid1per -= (1 + 1 * echo_cos(90 * dist_traveled - 22.5f)) * speed / dist;	//step thru it
								}
								else if(dist_traveled > 2.5f && dist_traveled <= 3)
								{
									grid1per -= (1 + 1 * echo_cos(90 * dist_traveled + 67.5f)) * speed / dist;	//step thru it
								}
								else
								{
									grid1per -= speed / dist;	//step thru it
								}
								
								if(grid1per <= 0)	//if we've reached the end of this cycle
								{
									next_grid();	//on to the next cycle
									pos1 = pos2;
									if(grid2)
									{
										i2 = grid2->get_info(echo_ns::angle);
										if(i2)
										{
											pos2 = i2->pos;
										}
									}
								}
							}
							draw(pos1->x * grid1per + pos2->x * (1 - grid1per),
									pos1->y * grid1per + pos2->y * (1 - grid1per),
									pos1->z * grid1per + pos2->z * (1 - grid1per));
						}
						else
							echo_error("Position of the second grid is NULL!\n");
					}
					else
						draw(i1->pos);
				}
				else
					echo_error("Position of the first grid is NULL!\n");
			}
			else
				echo_error("Info of the first grid is NULL!\n");
		}
		else	//if there isn't a second grid...
		{
			grid2 = grid1->get_next(echo_ns::angle, grid1);	//try to get one
			change_speed();
			if(!grid2)	//if there still isn't a second grid...
			{
				draw(grid1->get_info(echo_ns::angle)->pos);	//return grid1's position
			}
		}
	}
}

vector3f* echo_char::get_direction()
{
	if(grid2 != NULL)	//if both grids are there
	{
		grid_info_t* i1 = grid1->get_info(echo_ns::angle);
		if(i1 != NULL)
		{
			grid_info_t* i2 = grid2->get_info(echo_ns::angle);
			if(i2 != NULL)
				return(i2->pos->sub_new(i1->pos));
			else
				echo_error("Info of the second grid is NULL!\n");
			
		}
		else
			echo_error("Info of the first grid is NULL!\n");
	}
	return(NULL);
}

void echo_char::draw(vector3f* vec)
{
	draw(vec->x, vec->y, vec->z);
}

void echo_char::draw(float x, float y, float z)
{
	//*
	gfx_push_matrix();
	{
		
		float main_per = 0;
		grid* main_grid = NULL;
		if(grid1 != NULL && grid1per >= 0.5f)
		{
			main_per = grid1per;
			main_grid = grid1;
		}
		else if(grid2 != NULL)
		{
			main_per = 1 - grid1per;
			main_grid = grid2;
		}
		if(main_grid != NULL)
		{
			float vshift = main_grid->vert_shift(main_per);
			//ECHO_PRINT("vshift: %f, %f\n", vshift, y);
			y += vshift;
			//gfx_translatef(0, grid2->vert_shift(main_per), 0);
			joints.rshoulder_swing = -20 * echo_sin(dist_traveled_cyclic);
			joints.lshoulder_swing = 20 * echo_sin(dist_traveled_cyclic);
			joints.rarm_bend = -10 * echo_sin(dist_traveled_cyclic) - 20;
			joints.larm_bend = 10 * echo_sin(dist_traveled_cyclic) - 20;
			joints.rthigh_lift = 35 * echo_sin(dist_traveled_cyclic) - 15;
			joints.lthigh_lift = -35 * echo_sin(dist_traveled_cyclic) - 15;
			
#ifdef USE_IK
			vector3f* foot_vec = get_direction();
			
			if(foot_vec != NULL)
			{
				vector3f* up = new vector3f(0, 1, 0);
				CHKPTR(up);
				
				float left_dir_angle = 0;
				foot_vec->scalar_angle(up, &left_dir_angle);
				ECHO_PRINT("lda: %f\n", left_dir_angle);
				const float left_dist_foot = (vshift + 0.825f) * echo_sin(abs(joints.lthigh_lift)) 
											/ echo_sin(left_dir_angle);
				
				float right_dir_angle = 0;
				foot_vec->scalar_angle(up, &right_dir_angle);
				const float right_dist_foot = (vshift + 0.825f) * echo_sin(abs(joints.rthigh_lift)) 
										/ echo_sin(right_dir_angle);
				//ECHO_PRINT("lrdf: %f, %f\n", left_dist_foot, right_dist_foot);
				float temp = joints.rleg_bend;
				if(IK_angle(0.5f, 0.65f, right_dist_foot, &joints.rleg_bend) == WIN)
				{
					//ECHO_PRINT("joints.rleg_bend: %f\n", joints.rleg_bend);
					if(joints.rleg_bend == 0 || joints.rleg_bend != joints.rleg_bend)
						joints.rleg_bend = temp;
					else if(joints.rleg_bend > 90)
						joints.rleg_bend -= 90;
					//ECHO_PRINT("joints.rleg_bend: %f\n", joints.rleg_bend);
					temp = joints.lleg_bend;
					if(IK_angle(0.5f, 0.65f, left_dist_foot, &joints.lleg_bend) == WIN)
					{
						if(joints.lleg_bend == 0 || joints.lleg_bend != joints.lleg_bend)
							joints.lleg_bend = temp;
						else if(joints.lleg_bend > 90)
							joints.lleg_bend -= 90;
					}
					else
						echo_error("Inverse Kinematics failed?\n");
				}
				else
					echo_error("Inverse Kinematics failed?\n");
				
				delete up;
			}
			else
			{
				float temp = joints.rleg_bend;
				if(IK_angle(0.5f, 0.65f, (vshift + 1.175f) 
						/ echo_cos(joints.rthigh_lift), &joints.rleg_bend) == WIN)
				{
					if(joints.rleg_bend == 0)
						joints.rleg_bend = temp;
					temp = joints.lleg_bend;
					if(IK_angle(0.5f, 0.65f, (vshift + 1.175f) 
							/ echo_cos(joints.lthigh_lift), &joints.lleg_bend) == WIN)
					{
						if(joints.lleg_bend == 0)
							joints.lleg_bend = temp;
					}
					else
						echo_error("Inverse Kinematics failed?\n");
				}
				else
					echo_error("Inverse Kinematics failed?\n");
			}
			delete foot_vec;
#else
			#define LEG_BEND_MAX	30
			
			if(dist_traveled > 1 && dist_traveled <= 1.5f)
				joints.rleg_bend = LEG_BEND_MAX * echo_cos(dist_traveled * 180 + 180);
			else if(dist_traveled > 3.0f || dist_traveled <= 1)
				joints.rleg_bend = LEG_BEND_MAX * echo_sin(dist_traveled * 45);
			else
				joints.rleg_bend = 0; 
			if(dist_traveled > 3 && dist_traveled <= 3.5f)
				joints.lleg_bend = LEG_BEND_MAX * echo_cos(dist_traveled * 180 + 180);
			else if(dist_traveled > 1.0f && dist_traveled <= 3)
				joints.lleg_bend = LEG_BEND_MAX * echo_sin(dist_traveled * 45 - 90);
			else
				joints.lleg_bend = 0;
#endif
		}
		gfx_translatef(x, y, z);
		//ECHO_PRINT("pos: %f, %f, %f\n", x, y, z);
		//direction he is facing.
		if(grid1 != NULL && grid2 != NULL)
		{
			grid_info_t* i1 = grid1->get_info(echo_ns::angle);
			grid_info_t* i2 = grid2->get_info(echo_ns::angle);
			if(i1 && i2)
			{
				gfx_rotatef(90 - TO_DEG(atan2(i2->pos->z - i1->pos->z, i2->pos->x - i1->pos->x))
					, 0, 1, 0);
			}
		}
		//gfx_translatef(x, y, z);
#ifndef ECHO_NDS
		gfx_outline_start();
		draw_character(&joints);
		//gfx_identity();
		gfx_outline_mid();
		draw_character(&joints);
		gfx_outline_end();
#else
		//gfx_set_polyID(1);
		draw_character(&joints);
#endif
	}
	gfx_pop_matrix();
	// */
}

float echo_char::get_speed()
{
	return(speed);
}
