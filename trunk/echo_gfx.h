// echo_gfx.h

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

#include <grid.h>
#include <echo_math.h>

void draw_n_lines(line3f* my_lines, vector3f angle, grid** others, int num_lines);
void draw_line(vector3f p1, vector3f p2);
void draw_line(line3f ln);
void draw_rect(vector3f p1, vector3f p2, vector3f p3, vector3f p4);
void draw_circle(vector3f pos);
void draw_hole(vector3f pos);
