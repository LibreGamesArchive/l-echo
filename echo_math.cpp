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
	
	#define ECHO_COSI(deg)			((int)f32tofloat(cosLerp(degreesToAngle(deg))))
	#define ECHO_COSF(deg)			(f32tofloat(cosLerp(degreesToAngle(deg))))
	#define ECHO_SINI(deg)			((int)f32tofloat(sinLerp(degreesToAngle(deg))))
	#define ECHO_SINF(deg)			(f32tofloat(sinLerp(degreesToAngle(deg))))
	
	#define ECHO_ACOSF_DEG(ratio)	(angleToDegrees(acosLerp(floattof32(ratio))))
#else
	/// Cosine lookup table (index in degrees)
	static float cos_table[360];
	/// Initialize the lookup table
	void init_math()
	{
	    int deg = 0;
	    while(deg < 360)
	    {
			cos_table[deg] = cosf(TO_RAD(deg));
			deg++;
	    }
	}
	
	#define ECHO_COSI(deg)			(cos_table[ABS((deg)) % 360])
	#define ECHO_COSF(deg)			(ECHO_COSI((int)(deg)))
	#define ECHO_SINI(deg)			(ECHO_COSI(90 - (deg)))
	#define ECHO_SINF(deg)			(ECHO_SINI((int)(deg)))
	
	#define	ECHO_ACOSF_DEG(ratio)	(TO_DEG(acos(ratio)))
#endif

float echo_sin(int deg){ return(ECHO_SINI(deg)); }
float echo_sin(float deg){ return(ECHO_SINF(deg)); }
float echo_cos(int deg){ return(ECHO_COSI(deg)); }
float echo_cos(float deg){ return(ECHO_COSF(deg)); }

/// Default constructor
vector3f::vector3f()
{
	x = 0;
	y = 0;
	z = 0;
}
/** Sets elements as specified
 * @param myX The new vector's x-coordinate
 * @param myY The new vector's y-coordinate
 * @param myZ The new vector's z-coordinate
 */
vector3f::vector3f(float my_x, float my_y, float my_z)
{
	x = my_x;
	y = my_y;
	z = my_z;
}
/** Test if the vectors are unequal, or different outside of the range of +/- EPSILON.
 * @param v The other vector to check against.
 */
int vector3f::operator !=(vector3f* v)
{
    return(!(*this == v));
}
/** Test if the vectors are equivalent, or similar within the range of
 * +/- EPSILON.
 * @param v The other vector to check against
 */
int vector3f::operator ==(vector3f* v)
{
	return(ABS(x - v->x) < EPSILON &&
		ABS(y - v->y) < EPSILON &&
		ABS(z - v->z) < EPSILON);
}

/** Get the length of this vector from the origin.\n
 * Needless to say, the result of this function only makes sense if the
 * vector is used as a point or direction.
 * @return The length of this vector
 */
float vector3f::length()
{
	return(sqrt(x * x + y * y + z * z));
}

/// Print out the elements
void vector3f::dump()
{
	ECHO_PRINT("vector3f: [%f,%f,%f]", x, y, z);
}
/** Sets the vector to exactly the same as the vector given.
 * @param copy_from The vector to copy from.
 */
void vector3f::set(vector3f* copy_from)
{
	x = copy_from->x;
	y = copy_from->y;
	z = copy_from->z;
}
/** Gets a new vector that is opposite to this vector.
 * @return A new vector that is opposite to this vector
 */
vector3f* vector3f::negate()
{
	vector3f* ret = new vector3f(-x, -y, -z);
	CHKPTR(ret);
	return(ret);
}
/** Gets the resultant vector from the addition of this vector and the one given.
 * @param vec The other vector to add to to form the resultant vector.
 * @return The resultant vector.
 */
vector3f* vector3f::operator +(vector3f* vec)
{
    vector3f* ret = new vector3f(x + vec->x, y + vec->y, z + vec->z);
    CHKPTR(ret);
    return(ret);
}
/** Gets the resultant vector from the subtraction of the vector given from this vector.
 * @param vec The other vector to subtract this vector to form the resultant vector.
 * @return The resultant vector.
 */
vector3f* vector3f::operator -(vector3f* vec)
{
    vector3f* ret = new vector3f(x - vec->x, y - vec->y, z - vec->z);
    CHKPTR(ret);
    return(ret);
}
/** Gets a new vector that is this vector times the scalar given.
 * @param f Factor to multiply each element by.
 * @return This vector times the scalar given.
 */
vector3f* vector3f::operator *(float f)
{
	vector3f* ret = new vector3f(x * f, y * f, z * f);
	CHKPTR(ret);
	return(ret);
}
/** Gets a new vector representing the angle between this vector and <0, 0, 1>
 * @return Vector with the x and y rotation angles needed to rotate this vector to the positive z-axis.
 */
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
/** Gets the distance between this point vector and the other point vector given.
 * @param other The other point.
 * @return The distance between the two points.
 */
float vector3f::dist(vector3f* other)
{
	return(sqrt((x - other->x) * (x - other->x)
		+ (y - other->y) * (y - other->y) 
		+ (z - other->z) * (z - other->z)));
}
/** Gets the distance between this point vector and <0, 1, 0>.
 * Used to accelerate IK calculations
 * @return Distance between this point and <0, 1, 0>
 */
float vector3f::dist_with_up()
{
	return(sqrt(x * x + y * y - 2 * y + 1 + z * z));
}
/** Rotates from Absolute Position to World Position
 * @param rot Current camera angle
 * @return New vector containing a World Position if this vector is an Absolute Position
 */
vector3f* vector3f::rotate_xy(vector3f rot)
{
	/// If there is no rotation, just give a copy of this vector
	if(rot.x == 0 && rot.y == 0)
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
/** Rotates from World Position to Screen Position
 * @param rot Current camera angle
 * @return New vector containing a Screen Position if this vector is a World Position
 */
vector3f* vector3f::neg_rotate_xy(vector3f rot)
{
	if(rot.x == 0 && rot.y == 0)
	{
		vector3f* ret = new vector3f(x, y, z);
		CHKPTR(ret);
		return(ret);
	}
	vector3f* ret = new vector3f(x
			, y * ECHO_COSF(-rot.x) - z * ECHO_SINF(-rot.x)
			, y * ECHO_SINF(-rot.x) + z * ECHO_COSF(-rot.x));
	CHKPTR(ret);
	float z_save = ret->z;
	ret->z = ret->z * ECHO_COSF(-rot.y) - ret->x * ECHO_SINF(-rot.y);
	ret->x = z_save * ECHO_SINF(-rot.y) + ret->x * ECHO_COSF(-rot.y);
	return(ret);
}
/** Sets the vector to the components given.
 * @param my_x The new x component of this vector
 * @param my_y The new y component of this vector
 * @param my_z The new z component of this vector
 */
void vector3f::set(float my_x, float my_y, float my_z)
{
	x = my_x;
	y = my_y;
	z = my_z;
}
/** Rotates from Screen Position to World Position
 * @param rot Current camera angle
 * @return New vector containing a World Position if this vector is a Screen Position
 */
vector3f* vector3f::rotate_yx(vector3f rot)
{
	if(rot.x == 0 && rot.y == 0)
	{
		vector3f* ret = new vector3f(x, y, z);
		CHKPTR(ret);
		return(ret);
	}
	vector3f* ret = new vector3f(z * ECHO_SINF(rot.y) + x * ECHO_COSF(rot.y), y
                                , z * ECHO_COSF(rot.y) - x * ECHO_SINF(rot.y));
	CHKPTR(ret);
	float y_save = ret->y;
	ret->y = ret->y * ECHO_COSF(rot.x) - ret->z * ECHO_SINF(rot.x);
	ret->z = y_save * ECHO_SINF(rot.x) + ret->z * ECHO_COSF(rot.x);
	return(ret);
}
/** Rotates from World Position to Absolute Position
 * @param rot Current camera angle
 * @return New vector containing an Absolute Position if this vector is a World Position
 */
vector3f* vector3f::neg_rotate_yx(vector3f rot)
{
	if(rot.x == 0 && rot.y == 0)
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
/** Returns the angle this vector has with up as a single scalar in degrees
 * @return Angle this vector has with up; note that this is on an interval of [0, 180]
 */
float vector3f::scalar_angle_with_up()
{
	const float distance = dist_with_up();
	const float length1 = length();
	//const float length2 = vec->length();
	return(ECHO_ACOSF_DEG( ( (length1 * length1) + 1
			- (distance * distance) ) / (2 * length1) ));
}
/** Get the correct angle between the two lengths that would make the third leg the same length as distance
 * @param length1 One of the shorter lengths of the triangle
 * @param length2 Another short length
 * @param distance Longest length
 * @return Angle between length1 and length2
 */
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
		*angle = 180 - ECHO_ACOSF_DEG( ( (length1 * length1) + (length2 * length2) 
			- (distance * distance) ) / (2 * length1 * length2) );
	}
	return(WIN);
}
/** Tests for line segment intersection
 * Adapted from http://www.idevgames.com/forum/showthread.php?t=7458
 * @param a1 One of the points of line segment A
 * @param a2 One of the points of line segment A
 * @param b1 One of the points of line segment B
 * @param b2 One of the points of line segment B
 * @return If line segments A and B intersect.
 */
int lineSeg_intersect(vector3f* a1, vector3f* a2, vector3f* b1, vector3f* b2)
{
    float a1yb1y = a1->y - b1->y;
    float a1xb1x = a1->x - b1->x;
    float a2xa1x = a2->x - a1->x;
    float a2ya1y = a2->y - a1->y;
	
    //----------------------------------------------------------------------
	
    float crossa = a1yb1y * (b2->x - b1->x) - a1xb1x * (b2->y - b1->y);
    float crossb = a2xa1x * (b2->y - b1->y) - a2ya1y * (b2->x - b1->x);
	
    //----------------------------------------------------------------------
    
    if(crossb == 0)
		return(false);
    else if(fabs(crossa) > fabs(crossb) || crossa * crossb < 0)
		return(false);
    else
    {
		crossa = a1yb1y * a2xa1x - a1xb1x * a2ya1y;
		if(fabs(crossa) > fabs(crossb) || crossa * crossb < 0)
			return(false);
    }
    
    //----------------------------------------------------------------------
    
    return(true);
}

//-----------ANGLE_RANGE----------
/** Initialize an AngleRange with the two bounds given
 * @param my_v1 First bound
 * @param my_v2 Second bound
 */
angle_range::angle_range(vector3f* my_v1, vector3f* my_v2)
{
	v1 = my_v1;
	v2 = my_v2;
}

/// is b in between a and c?
#define IN_BETWEEN(a,b,c) (((a) <= (b) && (b) <= (c)) || ((c) <= (b) && (b) <= (a)))

/** Is the given vector inside the bounds.
 * @param v Vector to check.
 * @return If the vector is within bounds
 */
int angle_range::is_vec_in(vector3f v)
{
	return(IN_BETWEEN(v1->x, v.x, v2->x) 
		&& IN_BETWEEN(v1->y, v.y, v2->y));
}
/// Destructor; DELETES BOTH VECTORS IF POSSIBLE!
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

