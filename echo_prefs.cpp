// echo_prefs.cpp

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

#include <echo_prefs.h>
#include <echo_error.h>
#include <echo_debug.h>
#include <echo_platform.h>
#include <echo_xml.h>

#ifdef ECHO_NDS
	#ifndef __ECHO_NDS_PREFS__
	#define __ECHO_NDS_PREFS__
	
	#define HAND_ATTR_NAME 		"handedness"
	#define HAND_LEFT_VALUE 	"left"
	#define HAND_RIGHT_VALUE 	"right"
	#define PREFS_FILE			"/apps/n-echo/prefs.xml"
	
	STATUS open_prefs(echo_xml** document)
	{
		return(echo_xml_load_file(document, PREFS_FILE));
	}
	STATUS get_hand(echo_xml* document, HAND* handedness)
	{
		char** attr = new(char*);
		CHKPTR(attr);
		echo_xml_element** root = new(echo_xml_element*);
		CHKPTR(root);
		if(echo_xml_get_root(document, root) == WIN &&
			echo_xml_get_attribute(*root, HAND_ATTR_NAME, attr) == WIN)
		{
			if(!strcmp(*attr, HAND_LEFT_VALUE))
			{
				*handedness = LEFT_HAND;
				delete root;
				delete attr;
				return(WIN);
			}
			else if(!strcmp(*attr, HAND_RIGHT_VALUE))
			{
				*handedness = RIGHT_HAND;
				delete root;
				delete attr;
				return(WIN);
			}
		}
		delete root;
		delete attr;
		return(FAIL);
	}
	STATUS set_hand(echo_xml* document, HAND handedness)
	{
		echo_xml_element** root = new(echo_xml_element*);
		CHKPTR(root);
		if(echo_xml_get_root(document, root) == WIN &&
			echo_xml_set_attribute(*root, HAND_ATTR_NAME
				, handedness == LEFT_HAND ? HAND_LEFT_VALUE : HAND_RIGHT_VALUE))
		{
			delete root;
			return(WIN);
		}
		delete root;
		return(FAIL);
	}
	STATUS close_prefs(echo_xml* document)
	{
		if(echo_xml_save_file(document) == WIN &&
			echo_xml_delete_file(document) == WIN)
				return(WIN);
		return(FAIL);
	}
	#endif
#endif


