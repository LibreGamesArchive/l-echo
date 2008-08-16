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

#include <echo_platform.h>

#define PI			    3.1415926f
#define TWOPI			6.2831853f
#define EPSILON 		5e-2f
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
		void set(float my_x, float my_y, float my_z);
		int operator ==(vector3f v);
		int operator !=(vector3f v);
		int angle_similar(vector3f v);
		void dump();
		float length();
		
		vector3f* angle_xy();
		vector3f* rotate_xy(vector3f rot);
		vector3f* neg_rotate_yx(vector3f rot);
		vector3f* rotate_about_y(float angle);
		void self_rotate_about_y(float angle);
		vector3f* angle_to_real();
		
		vector3f normalize_angle();
		vector3f negate();
		float dist(vector3f other);
		
		void add(vector3f vec);
		
		vector3f operator *(float f);
		vector3f operator +(vector3f vec);
		vector3f operator -(vector3f vec);
};
#endif

#ifndef __ECHO_ANGLE_RANGE__
#define __ECHO_ANGLE_RANGE__
class angle_range
{
	protected:
		vector3f* v1;
		vector3f* v2;
	public:
		angle_range(vector3f* my_v1, vector3f* my_v2);
		int is_vec_in(vector3f v);
};
#endif

#define VECPTR_TO_RANGE(v)    (new angle_range(v, v))

float echo_sin(int deg);
float echo_cos(int deg);
#ifndef ECHO_NDS
	void init_math();
#endif

