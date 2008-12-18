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

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_math.h>
//#include <iostream>
#include <cmath>

#ifdef ECHO_NDS
	#include <nds.h>
	#include <nds/ndstypes.h>
	
	#define ECHO_COSI(deg) (cosLerp(degreesToAngle(deg)))
	#define ECHO_COSF(deg) (cosLerp(degreesToAngle(deg)))
	#define ECHO_SINI(deg) (sinLerp(degreesToAngle(deg)))
	#define ECHO_SINF(deg) (sinLerp(degreesToAngle(deg)))
#else
	static float cos_table[360];
	
	void init_math()
	{
	    int deg = 0;
	    while(deg < 360)
	    {
			cos_table[deg] = cosf(TO_RAD(deg));
			deg++;
	    }
	}
	
	#define ECHO_COSI(deg) (cos_table[ABS((deg)) % 360])
	#define ECHO_COSF(deg) (ECHO_COSI((int)(deg)))
	#define ECHO_SINI(deg) (ECHO_COSI(90 - (deg)))
	#define ECHO_SINF(deg) (ECHO_SINI((int)(deg)))
#endif

float echo_sin(int deg){ return(ECHO_SINI(deg)); }
float echo_cos(int deg){ return(ECHO_COSI(deg)); }

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

int vector3f::angle_similar(vector3f* v)
{
	return(ABS(x - v->x) < EPSILON &&
		ABS(y - v->y) < EPSILON);
}

int vector3f::operator !=(vector3f* v)
{
    return(!(*this == v));
}

int vector3f::operator ==(vector3f* v)
{
	return(ABS(x - v->x) < EPSILON &&
		ABS(y - v->y) < EPSILON &&
		ABS(z - v->z) < EPSILON);
}

float vector3f::length()
{
	return(sqrt(x * x + y * y + z * z));
}

void vector3f::dump()
{
	ECHO_PRINT("vector3f: [%f,%f,%f]", x, y, z);
}

void vector3f::set(vector3f* copy_from)
{
	x = copy_from->x;
	y = copy_from->y;
	z = copy_from->z;
}

vector3f* vector3f::negate()
{
	vector3f* ret = new vector3f(-x, -y, -z);
	CHKPTR(ret);
	return(ret);
}

vector3f* vector3f::operator +(vector3f* vec)
{
    vector3f* ret = new vector3f(x + vec->x, y + vec->y, z + vec->z);
    CHKPTR(ret);
    return(ret);
}

vector3f* vector3f::operator -(vector3f* vec)
{
    vector3f* ret = new vector3f(x - vec->x, y - vec->y, z - vec->z);
    CHKPTR(ret);
    return(ret);
}

vector3f* vector3f::operator *(float f)
{
	vector3f* ret = new vector3f(x * f, y * f, z * f);
	CHKPTR(ret);
	return(ret);
}

vector3f* vector3f::angle_xy()
{
	vector3f* ret = NULL;
	if(z == 0)
		ret = new vector3f(0, TO_DEG(atan2f(x, z)), 0);
	else
	{
		ret = new vector3f(-TO_DEG(atan2f(y, z > 0 
				? sqrt(x * x  + z * z) : -sqrt(x * x  + z * z))),
					TO_DEG(atan2f(x, z)), 0);
	}
        CHKPTR(ret);
        return(ret);

}

float vector3f::dist(vector3f* other)
{
	return(sqrt(pow(x - other->x, 2) + pow(y - other->y, 2) + pow(z - other->z, 2)));
}

vector3f* vector3f::rotate_xy(vector3f rot)
{
	if(rot.x == 0 && rot.y == 0 && rot.z == 0)
	{
		vector3f* ret = new vector3f(x, y, z);
		CHKPTR(ret);
		return(ret);
	}
	vector3f* ret = new vector3f(x
			, y * ECHO_COSF(rot.x) - z * ECHO_SINF(rot.x)
			, y * ECHO_SINF(rot.x) + z * ECHO_COSF(rot.x));
	CHKPTR(ret);
	float z_save = ret->z;
	ret->z = ret->z * ECHO_COSF(rot.y) - ret->x * ECHO_SINF(rot.y);
	ret->x = z_save * ECHO_SINF(rot.y) + ret->x * ECHO_COSF(rot.y);
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
	if(rot.x == 0 && rot.y == 0 && rot.z == 0)
	{
		vector3f* ret = new vector3f(x, y, z);
		CHKPTR(ret);
		return(ret);
	}
	vector3f* ret = new vector3f(z * ECHO_SINF(-rot.y) + x * ECHO_COSF(-rot.y), y
                                , z * ECHO_COSF(-rot.y) - x * ECHO_SINF(-rot.y));
	CHKPTR(ret);
	float y_save = ret->y;
	ret->y = ret->y * ECHO_COSF(-rot.x) - ret->z * ECHO_SINF(-rot.x);
	ret->z = y_save * ECHO_SINF(-rot.x) + ret->z * ECHO_COSF(-rot.x);
	return(ret);
}
vector3f* vector3f::rotate_about_y(float angle)
{
#ifdef STRICT_MEM
	vector3f* ret = new vector3f(z * ECHO_SINF(angle) + x * ECHO_COSF(angle)
			, y, z * ECHO_COSF(angle) - x * ECHO_SINF(angle));
	CHKPTR(ret);
	return(ret);
#else
	return(new vector3f(z * ECHO_SINF(angle) + x * ECHO_COSF(angle)
			, y, z * ECHO_COSF(angle) - x * ECHO_SINF(angle)));
#endif
}

void vector3f::self_rotate_about_y(float angle)
{
	float x_save = x;
	x = z * ECHO_SINF(angle) + x_save * ECHO_COSF(angle);
	z = z * ECHO_COSF(angle) - x_save * ECHO_SINF(angle);
}

void vector3f::add(vector3f* vec)
{
	x += vec->x;
	y += vec->y;
	z += vec->z;
}

static vector3f cam(0, 0, 10);

vector3f* vector3f::angle_to_real()
{
	return(cam.rotate_xy(*this));
}

STATUS vector3f::scalar_angle(vector3f* vec, float* angle)
{
	const float distance = dist(vec);
	const float length1 = length();
	const float length2 = vec->length();
	*angle = TO_DEG( acos( ( (length1 * length1) + (length2 * length2) 
			- (distance * distance) ) / (2 * length1 * length2) ) );
	return(WIN);
}

vector3f* vector3f::normalize_new(float new_length)
{
	const float mult = new_length / length();
	vector3f* ret = new vector3f(x * mult, y * mult, z * mult);
	CHKPTR(ret);
	return(ret);
}

vector3f* vector3f::add_new(vector3f* vec)
{
	vector3f* ret = new vector3f(x + vec->x, y + vec->y, z + vec->z);
	CHKPTR(ret);
	return(ret);
}

vector3f* vector3f::sub_new(vector3f* vec)
{
	vector3f* ret = new vector3f(x - vec->x, y - vec->y, z - vec->z);
	CHKPTR(ret);
	return(ret);
}

void vector3f::normalize(float new_length)
{
	const float mult = new_length / length();
	x *= mult;
	y *= mult;
	z *= mult;
}

STATUS IK_angle(float length1, float length2, float distance, float* angle)
{
	/*
	 * Where C = distance, A = length1, B = length2
	 * C^2 = A^2 + B^2 - 2ABcosTHETA
	 * C^2 - A^2 - B^2 = - 2ABcosTHETA
	 * (A^2 + B^2 - C^2) / 2AB = cosTHETA
	 * acos((A^2 + B^2 - C^2) / 2AB) = THETA
	 */
	//ECHO_PRINT("l1, l2, d: %f, %f, %f\n", length1, length2, distance);
	if(length1 + length2 < distance)
		*angle = 0;
	else
	{
		*angle = 180 - TO_DEG( acos( ( (length1 * length1) + (length2 * length2) 
			- (distance * distance) ) / (2 * length1 * length2) ) );
		
	}
	return(WIN);
}

//-----------ANGLE_RANGE----------

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

angle_range::~angle_range()
{
	if(v1 != NULL)
	{
		if(v2 != NULL && v1 != v2)
		{
			delete v1;
			delete v2;
		}
		else
			delete v1;
	}
}

