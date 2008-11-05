// echo_xml.h

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

#ifndef __ECHO_XML__
#define __ECHO_XML__
	#ifdef USE_TIXML
		#ifdef ECHO_NDS
			#include <tinyxml.h>
		#else
			#include <tinyxml/tinyxml.h>
		#endif	
		typedef struct {
			TiXmlDocument* document;
			char* file_name;
		} echo_xml;
		typedef TiXmlNode 		echo_xml_node;
		typedef TiXmlElement	echo_xml_element;
	#else
		#ifdef ECHO_NDS
			#include <rapidxml.hpp>
			#include <rapidxml_print.hpp>
			#include <rapidxml_utils.hpp>
		#else
			#include <rapidxml/rapidxml.hpp>
			#include <rapidxml/rapidxml_print.hpp>
			#include <rapidxml/rapidxml_utils.hpp>
		#endif
		typedef struct {
			rapidxml::xml_document<>* document;
			char* file_name;
		} echo_xml;
		typedef rapidxml::xml_node<> 	echo_xml_node;
		typedef rapidxml::xml_node<>	echo_xml_element;
	#endif
#endif

int echo_xml_load_file(echo_xml** doc, char* filename);
int echo_xml_save_file(echo_xml* doc);

int echo_xml_get_root(echo_xml* doc, echo_xml_element** root);
int echo_xml_to_element(echo_xml_node* node, echo_xml_element** e);

int echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child);
int echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling);

int echo_xml_get_attribute(echo_xml_element* e, char* key, char** value);
int echo_xml_get_int_attribute(echo_xml_element* e, char* key, int* value);
int echo_xml_get_float_attribute(echo_xml_element* e, char* key, float* value);

int echo_xml_set_attribute(echo_xml_element* e, char* key, const char* value);

int echo_xml_delete_file(echo_xml* doc);
