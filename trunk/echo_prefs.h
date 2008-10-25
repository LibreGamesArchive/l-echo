// echo_prefs.h

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

#ifdef ECHO_NDS
	enum { LEFT_HAND, RIGHT_HAND } HAND;

	int open_prefs(TiXmlDocument** document);
	int get_hand(TiXmlDocument* document, enum HAND* handedness);
	int set_hand(TiXmlDocument* document, enum HAND handedness);
	int close_prefs(TiXmlDocument* document);
#endif

