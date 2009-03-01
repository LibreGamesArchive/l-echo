// echo_char_joints.cpp

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

#include "echo_char_joints.h"
#include "echo_debug.h"
#include "echo_error.h"

/** Set all joints to zero
 * @param joints Joints to set to zero
 */
int reset_joints(echo_char_joints* joints)
{
	/// Have to have the joints as non-null, eh?
	if(joints == NULL)
		return(FAIL);
	/// Optimized reverse loop
	for(int each = NUM_VALUES; --each >= 0;)
		joints->value[each] = 0;
	return(WIN);
}
