// trigger.h

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


#ifndef __ECHO_CLASS_TRIGGER__
#define __ECHO_CLASS_TRIGGER__

class trigger;

#include "grid.h"
#include "filter.h"

/** @brief Trigger structure that contains a filter and a target that is triggered if
 * the filter is null or is true.
 */
class trigger
{
	protected:
		/// Filter to ask if the the target should be triggered.
		filter* my_filter;
		/// Target grid
		grid* target;
	public:
		/** Makes a new Trigger with the filter given, and a null target (MUST SET LATER!)
		 * @param filter The new trigger's filter; WILL BE DELETED!!!!
		 */
		trigger(filter* filter);
		/** Makes a new Trigger with the filter and target given
		 * @param filter The new trigger's filter; WILL BE DELETED!!!!
		 * @param my_target The new trigger's target
		 */
		trigger(filter* filter, grid* my_target);
		/// Deletes the filter
		~trigger();
		/** Triggers the target's goal if the trigger's filter is null or is true
		 * for that camera angle.
		 * @param angle The current camera angle
		 */
		void toggle(vector3f angle);
		/** Sets the trigger's target to the one given
		 * @param my_target The trigger's new target
		 */
		void set_target(grid* my_target);
};

#endif

typedef std::set<trigger*> TRIGGER_SET;

