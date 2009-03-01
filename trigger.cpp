// trigger.cpp

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
#include "echo_platform.h"
#include "echo_debug.h"

#include "filter.h"
#include "trigger.h"
#include "grid.h"

/** Makes a new Trigger with the filter given, and a null target (MUST SET LATER!)
 * @param filter The new trigger's filter; WILL BE DELETED!!!!
 */
trigger::trigger(filter* filter)
{
	my_filter = filter;
	target = NULL;
}
/** Makes a new Trigger with the filter and target given
 * @param filter The new trigger's filter; WILL BE DELETED!!!!
 * @param my_target The new trigger's target
 */
trigger::trigger(filter* filter, grid* my_target)
{
	target = my_target;
	my_filter = filter;
}
/// Deletes the filter
trigger::~trigger()
{
	delete my_filter;
}
/** Triggers the target's goal if the trigger's filter is null or is true
 * for that camera angle.
 * @param angle The current camera angle
 */
void trigger::toggle(vector3f angle)
{
	if(my_filter == NULL || my_filter->is_true(angle))
		target->toggle_goal(angle);
}
/** Sets the trigger's target to the one given
 * @param my_target The trigger's new target
 */
void trigger::set_target(grid* my_target)
{
	target = my_target;
}

