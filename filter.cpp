// filter.cpp

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

#include <grid.h>
#include <filter.h>
#include <echo_math.h>
#include <echo_error.h>

/// Initializes a filter with no target; MUST SET BEFORE CALLING is_true!
filter::filter()
{
	target = NULL;
}
/// Destructor; does nothing
filter::~filter()
{
}
/** Initializes a filter with the target given
 * @param my_target The new filter's target
 */
filter::filter(grid* my_target)
{
	target = my_target;
}
/** Should the trigger's target be triggered?  That is, turning into a goal.
 * The default behavior is to see if the filter's target is a goal.
 * @param angle Current camera angle
 * @return If the (trigger's) target should be triggered.
 */
int filter::is_true(vector3f angle)
{
	return(target->is_goal(angle));
}
/** Sets the filter's target (DOES NOT HAVE ANYTHING TO DO WITH THE *TRIGGER'S*
 * TARGETS!).
 * @param my_target The filter's new target
 */
void filter::set_target(grid* my_target)
{
	target = my_target;
}
/** Creates a not_filter with the designated filter
 * @param my_filter The new not_filter's target filter; WILL BE DELETED!!!
 */
not_filter::not_filter(filter* my_filter)
{
	f = my_filter;
}
/// Destructor; DELETES TARGET FILTER!
not_filter::~not_filter()
{
	delete f;
}
/** Should the trigger's target be triggered?  That is, turning into a goal.
 * Returns the opposite of what the target filter returns
 * @param angle Current camera angle
 * @return If the (trigger's) target should be triggered.
 */
int not_filter::is_true(vector3f angle)
{
	return(!f->is_true(angle));
}
/** If one target filter returns true, then this filter will return true
 * @param angle Current camera angle
 * @return If the (trigger's) target should be triggered.
 */
int or_filter::is_true(vector3f angle)
{
	if(filters->empty())
		return(1);
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		if((*it)->is_true(angle))
			return(1);
		it++;
	}
	return(0);
}
/** If one target filter returns false, then this filter will return false
 * @param angle Current camera angle
 * @return If the (trigger's) target should be triggered.
 */
int and_filter::is_true(vector3f angle)
{
	if(filters->empty())
		return(1);
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		if(!((*it)->is_true(angle)))
			return(0);
		it++;
	}
	return(1);
}
/// Default constructor; initializes the list of filters
multi_filter::multi_filter()
{
	filters = new FILTER_SET();
	CHKPTR(filters);
}
/// Destructor; DELETES ALL FILTERS!
multi_filter::~multi_filter()
{
	FILTER_SET::iterator it = filters->begin(), end = filters->end();
	while(it != end)
	{
		filter* del = *it;
		if(del != NULL)
			delete del;
		it++;
	}
	delete filters;
}
/** Adds the filter given into the set.
 * @param f Filter to add; WILL DELETE THE FILTER GIVEN!
 */
void multi_filter::add_filter(filter* f)
{
	filters->insert(f);
}
