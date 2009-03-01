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

#include "echo_platform.h"
#include "echo_debug.h"
#include "echo_error.h"

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
		typedef int				echo_xml_type;
		
		const echo_xml_type ECHO_XML_TYPE_ELEMENT = TiXmlNode::ELEMENT;
		const echo_xml_type ECHO_XML_TYPE_COMMENT = TiXmlNode::COMMENT;
		const echo_xml_type ECHO_XML_TYPE_NULL = TiXmlNode::UNKNOWN;
	#elif defined(USE_RAPIDXML)
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
		typedef rapidxml::node_type		echo_xml_type;
		
		const echo_xml_type ECHO_XML_TYPE_ELEMENT = rapidxml::node_element;
		const echo_xml_type ECHO_XML_TYPE_COMMENT = rapidxml::node_comment;
		const echo_xml_type ECHO_XML_TYPE_NULL = rapidxml::node_data;
	#elif defined(USE_PUGIXML)
		#ifdef ECHO_NDS
			#include <pugixml.hpp>
		#else
			#include <pugixml/pugixml.hpp>
		#endif
		typedef struct {
			pugi::xml_document* document;
			char* file_name;
		} echo_xml;
		typedef pugi::xml_node 		echo_xml_node;
		typedef pugi::xml_node		echo_xml_element;
		typedef pugi::xml_node_type		echo_xml_type;
		
		const echo_xml_type ECHO_XML_TYPE_ELEMENT = pugi::node_element;
		const echo_xml_type ECHO_XML_TYPE_COMMENT = pugi::node_comment;
		const echo_xml_type ECHO_XML_TYPE_NULL = pugi::node_null;
	#endif
#endif

/** Load the document from the file
 * @param filename File name of the XML file
 * @return The document if it exists, or NULL if not
 */
STATUS echo_xml_load_file(echo_xml** doc, char* filename);
/// Saves the document
STATUS echo_xml_save_file(echo_xml* doc);
/// Get the root of the document
STATUS echo_xml_get_root(echo_xml* doc, echo_xml_element** root);
/** Convert the node to an element
 * @param node The node to convert
 * @param e Where to store the element
 */
STATUS echo_xml_to_element(echo_xml_node* node, echo_xml_element** e);
/// Get the type of the node
STATUS echo_xml_get_node_type(echo_xml_node* node, echo_xml_type* t);
/// Get the tag name of the element
STATUS echo_xml_get_tagname(echo_xml_element* e, char** tag);
/// Get the first child node of the given node
STATUS echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child);
/// Get the next sibling of this node
STATUS echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling);
/// Get the attribute in string form
STATUS echo_xml_get_attribute(echo_xml_element* e, const char* key, char** value);
/// Get the attribute in int form
STATUS echo_xml_get_int_attribute(echo_xml_element* e, const char* key, int* value);
/// Get the attribute in float form
STATUS echo_xml_get_float_attribute(echo_xml_element* e, const char* key, float* value);
/// Set the attribute in string form
STATUS echo_xml_set_attribute(echo_xml_element* e, const char* key, const char* value);
/// Delete and clear the document
STATUS echo_xml_delete_file(echo_xml* doc);

