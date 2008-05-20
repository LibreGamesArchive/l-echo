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

#include <map>
#include <string>

#include <grid.h>
#include <echo_math.h>
#include <string>

#ifndef __ECHO_CLASS_STAGE__
#define __ECHO_CLASS_STAGE__

typedef std::map<std::string, grid*> STAGE_MAP;

class stage
{
    protected:
        STAGE_MAP* grids;
        grid* start;
        std::string name;
        int num_goals;
    public:
        stage();
        stage(grid* my_start, std::string my_name, int my_num_goals);
        void add(std::string id, grid* ptr);
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
