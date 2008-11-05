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
#include <echo_xml.h>
#ifdef ECHO_NDS
	enum HAND { LEFT_HAND, RIGHT_HAND };
	STATUS open_prefs(echo_xml** document);
	STATUS get_hand(echo_xml* document, HAND* handedness);
	STATUS set_hand(echo_xml* document, HAND handedness);
	STATUS close_prefs(echo_xml* document);
#endif
