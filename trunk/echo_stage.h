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

typedef std::map<std::string, grid*> STAGE_MAP;
typedef std::vector<GRID_PTR_SET*> GPS_VEC;
typedef std::map<float, GRID_PTR_SET*> LEVEL_MAP;

class stage
{
    protected:
        STAGE_MAP* grids;
        grid* start;
        std::string name;
        int num_goals;
	
	LEVEL_MAP* levels;
	
    public:
        stage();
        stage(grid* my_start, std::string my_name, int my_num_goals);
        void add(std::string id, grid* ptr);
	void add_pos(vector3f pos, grid* g);
	GRID_PTR_SET* get_level(vector3f pos);
	LEVEL_MAP* get_levels_lower_than(float y);
	LEVEL_MAP* get_levels_higher_than(float y);
	void dump_levels();
        grid* get(std::string id);
        void draw(vector3f angle);

        void set_start(grid* g);
        void set_name(std::string name);
        void set_num_goals(int num_goals);

        grid* get_start();
        std::string get_name();
        int get_num_goals();
};
#endif
