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

#include <echo_math.h>
#include <iostream>
#include <cmath>

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
