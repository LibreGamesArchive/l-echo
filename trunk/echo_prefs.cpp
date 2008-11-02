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
#ifdef ECHO_NDS
	#include <tinyxml.h>
#else
	#include <tinyxml/tinyxml.h>
#endif

#ifdef ECHO_NDS
	#ifndef __ECHO_NDS_PREFS__
	#define __ECHO_NDS_PREFS__
	
	#define HAND_ATTR_NAME 		"handedness"
	#define HAND_LEFT_VALUE 	"left"
	#define HAND_RIGHT_VALUE 	"right"
	#define PREFS_FILE			"/apps/n-echo/prefs.xml"
	#include <tinyxml.h>
	int open_prefs(TiXmlDocument** document)
	{
		*document = new TiXmlDocument(PREFS_FILE);
		return((*document)->LoadFile() ? WIN : FAIL);
	}
	int get_hand(TiXmlDocument* document, HAND* handedness)
	{
		ECHO_PRINT("at get_hand\n");
		if(document != NULL)
		{
			ECHO_PRINT("after document != NULL\n");
			TiXmlElement* root = document->RootElement();
			ECHO_PRINT("after get root\n");
			if(root != NULL)
			{
				ECHO_PRINT("after root != NULL\n");
				const char* hand_str = root->Attribute(HAND_ATTR_NAME);
				ECHO_PRINT("after get hand_str\n");
				if(hand_str != NULL)
				{
					ECHO_PRINT("after hand_str != NULL\n");
					if(!strcmp(hand_str, HAND_LEFT_VALUE))
					{
						ECHO_PRINT("after hand_str == left\n");
						*handedness = LEFT_HAND;
						ECHO_PRINT("left hand\n");
						return(WIN);
					}
					else if(!strcmp(hand_str, HAND_RIGHT_VALUE))
					{
						ECHO_PRINT("after hand_str == right\n");
						*handedness = RIGHT_HAND;
						ECHO_PRINT("right hand\n");
						return(WIN);
					}
				}
				else
					ECHO_PRINT("hand_str is null?\n");
			}
			else
				ECHO_PRINT("Root element is null?\n");
		}
		else
			ECHO_PRINT("Doc is null?\n");
		return(FAIL);
	}
	int set_hand(TiXmlDocument* document, HAND handedness)
	{
		if(document)
		{
			TiXmlElement* root = document->RootElement();
			if(root)
			{
				root->SetAttribute(HAND_ATTR_NAME, handedness == LEFT_HAND ? HAND_LEFT_VALUE : HAND_RIGHT_VALUE);
				return(WIN);
			}
		}
		return(FAIL);
	}
	int close_prefs(TiXmlDocument* document)
	{
		if(document->SaveFile(PREFS_FILE) == FALSE)
			return(FAIL);
		delete document;
		return(WIN);
	}
	#endif
#endif


