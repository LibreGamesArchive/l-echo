// echo_math.h

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

#define PI			    3.1415926f
#define TWOPI			6.2831853f
#define EPSILON 		8e-3f
#define ABS(x)			((x) >= 0 ? (x) : -(x))
#define TO_RAD(x)		((x) / 180.0f * PI)
#define TO_DEG(x)		((x) / PI * 180.0f)

#ifndef __ECHO_VECTOR_3F__
#define __ECHO_VECTOR_3F__
/*
typedef struct
{
	float x, y, z;
}
vector3f;
*/
class vector3f
{
	public:
		float x, y, z;

		vector3f();
		vector3f(float my_x, float my_y, float my_z);
		~vector3f(){}
		void set(vector3f copy_from);
		int operator ==(vector3f v);
		int angle_similar(vector3f v);
		void dump();
		float length();
		vector3f normalize_angle();
		vector3f negate();
};
#endif

#ifndef __ECHO_LINE_3F__
#define __ECHO_LINE_3F__
typedef struct
{
	vector3f p1;
	vector3f p2;
}
line3f;
#endif

void dump_line3f(line3f ln);
int operator ==(line3f ln1, line3f ln2);

