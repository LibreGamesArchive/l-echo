// echo_stage.h

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

#include <set>
#include <map>
#include <string>
#include <vector>

#include <grid.h>
#include <echo_math.h>

#ifndef __ECHO_CLASS_STAGE__
#define __ECHO_CLASS_STAGE__

/// A map from the id of the grid to the grid
typedef std::map<std::string, grid*> STAGE_MAP;

/// @brief Holds a list of grids and basic info about the grid (number of goals, name, etc.)
class stage
{
    protected:
    	/// The list of grids in the stage; needed for drawing
        STAGE_MAP* grids;
	/// The initial starting point (changes as the mannequin acquires goals)
        grid* start;
	/// The name od the stage
        std::string* name;
	/// The total number of goals in the stage
        int num_goals;
	/// Distance from the origin of the farthest grid
	float farthest;
	/// Y-Coordinate of the lowest grid THAT CAN BE LANDED ON!!!!!!!!!!!!!!
	float lowest;
	/** Internal initialization function
	 * @param my_start Initial starting point
	 * @param my_name The stage's name
	 * @param my_num_goals Total number of goals this stage has
	 */
	void init(grid* my_start, std::string* my_name, int my_num_goals);
    public:
    	/// Default constructor; initializes an empty stage
        stage();
	/** Makes a new grid with the following parameters
	 * @param my_start Initial starting point
	 * @param my_name The stage's name; WILL BE DELETED AT DECONSTRUCTION!!!
	 * @param my_num_goals Total number of goals this stage has
	 */
        stage(grid* my_start, std::string* my_name, int my_num_goals);
	/// Deletes the stage's name and the all the grids in it
        ~stage();
        /** Adds the grid with the id.
	 * @param id The id of the grid to add
	 * @param ptr The grid to add
	 */
        void add(std::string id, grid* ptr);
	/** Adds the position of the grid.  It was used in the old holes and launchers
	 * implementation, but used right now just to calculate the lowest position.
	 * @param pos The position of the grid.  Just has to be the position of the grid at some angle.
	 * @param g The grid (isn't used now)
	 */
	void add_pos(vector3f* pos, grid* g);
	/// Gets a grid with the given string id
        grid* get(std::string id);
	/// Draws all the grids
        void draw(vector3f angle);
	/// Sets the initial starting point of the stage
        void set_start(grid* g);
	/// Sets the name of the stage
        void set_name(std::string* name);
	/// Sets the number of goals of the stage
        void set_num_goals(int num_goals);
	/** Tests if any grid is intersected by the line represented by the
	 * Screen Position points
	 * @param p1 Screen Position; point 1 of the line
	 * @param p2 Screen Position; point 2 of the line
	 * @param angle Current camera angle
	 */
	grid* get_grid_intersection(vector3f* p1, vector3f* p2, vector3f angle);
	/// Gets the initial starting point of this stage
        grid* get_start();
	/// Gets the name of the stage
        std::string* get_name();
	/// Gets the total number of goals of the stage
        int get_num_goals();
	/// Set the farthest position
	void set_farthest(float new_far);
	/// Get the farthest position
	float get_farthest();
	/// Gets the lowest y-coordinate that has a grid that can be landed on
	float get_lowest_level();
};
#endif

