// echo_math.cpp

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

#include <echo_math.h>
#include <iostream>
#include <cmath>

static float sin_table[360];

void init_math()
{
    int deg = 0;
    while(deg < 360)
    {
        sin_table[deg] = sinf(TO_RAD(deg));
        deg++;
    }
}

float echo_sin(int deg)
{
    deg = deg % 360;
    if(deg < 0)
        deg += 360;
    //std::cout << "deg: " << deg << std::endl;
    return(sin_table[deg]);
}

float echo_cos(int deg)
{
    return(echo_sin(90 - deg));
}

vector3f::vector3f()
{
	x = 0;
	y = 0;
	z = 0;
}

vector3f::vector3f(float my_x, float my_y, float my_z)
{
	x = my_x;
	y = my_y;
	z = my_z;
}

int vector3f::angle_similar(vector3f v)
{
	/*
	std::cout << "angle similar: ";
	dump();
	std::cout << std::endl;
	// */
	return(ABS(x - v.x) < EPSILON &&
		(ABS(y - v.y) < EPSILON || ABS(ABS(y - v.y) - 360) < EPSILON) &&
		ABS(z - v.z) < EPSILON);
}

int vector3f::operator !=(vector3f v)
{
    return(!(*this == v));
}

int vector3f::operator ==(vector3f v)
{
	return(ABS(x - v.x) < EPSILON &&
		ABS(y - v.y) < EPSILON &&
		ABS(z - v.z) < EPSILON);
}

float vector3f::length()
{
	return(sqrt(x * x + y * y + z * z));
}

void vector3f::dump()
{
	std::cout << "vector3f(" << this << "): [" << x << "," << y << "," << z << "]";
}

void vector3f::set(vector3f copy_from)
{
	x = copy_from.x;
	y = copy_from.y;
	z = copy_from.z;
}

vector3f vector3f::negate()
{
	vector3f ret(-x, -y, -z);
	return(ret);
}

void dump_line3f(line3f ln)
{
	std::cout << "lines3f: [";
	ln.p1.dump();
	std::cout << ",";
	ln.p2.dump();
	std::cout << "]";
}

int operator ==(line3f ln1, line3f ln2)
{
	return( ( (ln1.p1 == ln2.p1) && (ln1.p2 == ln2.p2) )
		|| ( (ln1.p1 == ln2.p2) && (ln1.p2 == ln2.p1) ) );
}

vector3f vector3f::operator +(vector3f vec)
{
    vector3f* ret = new vector3f(x + vec.x, y + vec.y, z + vec.z);
    return(*ret);
}

vector3f vector3f::operator -(vector3f vec)
{
    vector3f* ret = new vector3f(x - vec.x, y - vec.y, z - vec.z);
    return(*ret);
}

vector3f vector3f::operator *(float f)
{
	vector3f* ret = new vector3f(x * f, y * f, z * f);
	return(*ret);
}

vector3f* vector3f::angle_xy()
{
	float temp = sqrt(x * x  + z * z);
	vector3f* ret = new vector3f(-TO_DEG(atan2f(y, z > 0 ? temp : -temp)),
				TO_DEG(atan2f(x, z)), 0);
	return(ret);
}

float vector3f::dist(vector3f other)
{
	return(sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2)));
}

vector3f* vector3f::rotate_xy(vector3f rot)
{
	if(rot.x == 0 && rot.y == 0 && rot.z == 0)
		return(this);
	vector3f* ret = new vector3f(x
			, y * echo_cos(rot.x) - z * echo_sin(rot.x)
			, y * echo_sin(rot.x) + z * echo_cos(rot.x));
	float z_save = ret->z;
	ret->z = ret->z * echo_cos(rot.y) - ret->x * echo_sin(rot.y);
	ret->x = z_save * echo_sin(rot.y) + ret->x * echo_cos(rot.y);
	return(ret);
}

void vector3f::set(float my_x, float my_y, float my_z)
{
	x = my_x;
	y = my_y;
	z = my_z;
}

vector3f* vector3f::neg_rotate_yx(vector3f rot)
{
	//float rad_x = -TO_RAD(rot.x), rad_y = -TO_RAD(rot.y);
	vector3f* ret = new vector3f(z * echo_sin(-rot.y) + x * echo_cos(-rot.y), y
                                , z * echo_cos(-rot.y) - x * echo_sin(-rot.y));
	float y_save = ret->y;
	ret->y = ret->y * echo_cos(-rot.x) - ret->z * echo_sin(-rot.x);
	ret->z = y_save * echo_sin(-rot.x) + ret->z * echo_cos(-rot.x);
	return(ret);
}

angle_range::angle_range(vector3f* my_v1, vector3f* my_v2)
{
	v1 = my_v1;
	v2 = my_v2;
}

//is b in between a and c?
#define IN_BETWEEN(a,b,c) (((a) <= (b) && (b) <= (c)) || ((c) <= (b) && (b) <= (a)))

int angle_range::is_vec_in(vector3f v)
{
	return(IN_BETWEEN(v1->x, v.x, v2->x) 
		&& IN_BETWEEN(v1->y, v.y, v2->y)
		&& IN_BETWEEN(v1->z, v.z, v2->z));
}

vector3f* vector3f::rotate_about_y(float angle)
{
	return(new vector3f(z * echo_sin(angle) + x * echo_cos(angle)
			, y, z * echo_cos(angle) - x * echo_sin(angle)));
}

