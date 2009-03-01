// filter.h

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


#include "echo_math.h"

#ifndef __ECHO_CLASS_FILTER__
#define __ECHO_CLASS_FILTER__
class filter;

#include "grid.h"
#include "echo_platform.h"
#include "echo_debug.h"
/** @brief When a grid that has triggers is triggered, filters in each trigger determine
 * if the trigger's targets should be triggered too.
 */
class filter
{
	protected:
		/** The target grid; if the target is a goal when this filter is "asked",
		 * it will return true.  Has NOTHING TO DO with trigger targets?
		 */
		grid* target;
	public:
		/// Initializes a filter with no target; MUST SET BEFORE CALLING is_true!
		filter();
		/** Initializes a filter with the target given
		 * @param my_target The new filter's target
		 */
		filter(grid* my_target);
		/// Destructor; does nothing
		virtual ~filter();
		/** Should the trigger's target be triggered?  That is, turning into a goal.
		 * The default behavior is to see if the filter's target is a goal.
		 * @param angle Current camera angle
		 * @return If the (trigger's) target should be triggered.
		 */
		virtual int is_true(vector3f angle);
		/** Sets the filter's target (DOES NOT HAVE ANYTHING TO DO WITH THE *TRIGGER'S*
		 * TARGETS!).
		 * @param my_target The filter's new target
		 */
		void set_target(grid* my_target);
};
/// Negates the target filter's is_true
class not_filter : public filter
{
	protected:
		/// The target filter; its is_true result will be negated then returned in the new is_true
		filter* f;
	public:
		/** Creates a not_filter with the designated filter
		 * @param my_filter The new not_filter's target filter; WILL BE DELETED!!!
		 */
		not_filter(filter* my_filter);
		/// Destructor; DELETES TARGET FILTER!
		virtual ~not_filter();
		/** Should the trigger's target be triggered?  That is, turning into a goal.
		 * Returns the opposite of what the target filter returns
		 * @param angle Current camera angle
		 * @return If the (trigger's) target should be triggered.
		 */
		int is_true(vector3f angle);
};
/// Filters that have multiple target filters, such as or_filters and and_filters
class multi_filter: public filter
{
	protected:
		/// Filters that are used by the child class
		FILTER_SET* filters;
	public:
		/// Default constructor; initializes the list of filters
		multi_filter();
		/// Destructor; DELETES ALL FILTERS!
		virtual ~multi_filter();
		/** Adds the filter given into the set.
		 * @param f Filter to add; WILL DELETE THE FILTER GIVEN!
		 */
		void add_filter(filter* f);
		/// Must override!
		virtual int is_true(vector3f angle) = 0;
};
/// If one target filter returns true, then this filter will return true
class or_filter : public multi_filter
{
	public:
		/// Default constructor; does nothing
		or_filter(){};
		/// destructor; does nothing
		virtual ~or_filter(){};
		/** If one target filter returns true, then this filter will return true
		 * @param angle Current camera angle
		 * @return If the (trigger's) target should be triggered.
		 */
		int is_true(vector3f angle);
};
/// If one target filter returns false, then this filter will return false
class and_filter : public multi_filter
{
	public:
		/// Default constructor; does nothing
		and_filter(){};
		/// destructor; does nothing
		virtual ~and_filter(){};
		/** If one target filter returns false, then this filter will return false
		 * @param angle Current camera angle
		 * @return If the (trigger's) target should be triggered.
		 */
		int is_true(vector3f angle);
};

#endif

typedef std::set<filter*> FILTER_SET;
