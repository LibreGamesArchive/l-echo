// echo_xml.cpp

#include <cstdlib>
#include <cstddef>

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_xml.h>

#ifdef USE_TIXML
	#ifdef ECHO_NDS
		#include <tinyxml.h>
	#else
		#include <tinyxml/tinyxml.h>
	#endif	
	STATUS echo_xml_load_file(echo_xml** doc, char* filename)
	{
		if(doc != NULL && filename != NULL)
		{
			*doc = new(echo_xml);
			(*doc)->file_name = filename;
			(*doc)->document = new TiXmlDocument(filename);
			if((*doc)->document->LoadFile())
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_save_file(echo_xml* doc)
	{
		if(doc == NULL)
		{
			if(doc->document->SaveFile(doc->file_name) == true)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_root(echo_xml* doc, echo_xml_element** root)
	{
		if(doc != NULL && root != NULL)
		{
			*root = doc->document->RootElement();
			if(*root != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_to_element(echo_xml_node* node, echo_xml_element** e)
	{
		//ECHO_PRINT("to element\n");
		if(node != NULL && e != NULL)
		{
			//ECHO_PRINT("to element NOT null\n");
			*e = node->ToElement();
			if(*e != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_attribute(echo_xml_element* e, const char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			*value = const_cast<char*>(e->Attribute(key));
			if(*value != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_int_attribute(echo_xml_element* e, const char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->QueryIntAttribute(key, value) == TIXML_SUCCESS)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_float_attribute(echo_xml_element* e, const char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->QueryFloatAttribute(key, value) == TIXML_SUCCESS)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child)
	{
		if(node != NULL && child != NULL)
		{
			*child = node->FirstChild();
			if(*child != NULL)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling)
	{
		if(node != NULL && sibling != NULL)
		{
			*sibling = node->NextSibling();
			if(*sibling != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_set_attribute(echo_xml_element* e, const char* key, const char* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			e->SetAttribute(key, value);
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_node_type(echo_xml_node* node, echo_xml_type* t)
	{
		if(node != NULL && t != NULL)
		{
			*t = node->Type();
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_tagname(echo_xml_element* e, char** tag)
	{
		if(e != NULL && tag != NULL)
		{
			*tag = const_cast<char*>(e->Value());
			if(*tag != NULL)
				return(WIN);
		}
		return(FAIL);
	}
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
	#include <iostream>
	#include <fstream>
	#include <cstring>
	#include <cmath>
	STATUS echo_xml_load_file(echo_xml** doc, char* filename)
	{
		if(doc != NULL && filename != NULL)
		{
			*doc = NULL;
			rapidxml::file<>* f = NULL;
			try
			{
				f = new rapidxml::file<>(filename);
				CHKPTR(f);
				char* data = f->data();
				if(data != NULL)
				{
					try
					{
						*doc = new(echo_xml);
						(*doc)->file_name = filename;
						(*doc)->document = new rapidxml::xml_document<>();
						CHKPTR((*doc)->document);
						(*doc)->document->parse<0>(data);
						
						delete f;
						return(WIN);
					}
					catch(rapidxml::parse_error e)
					{
						ECHO_PRINT("xml file parse error\n");
					}
					if((*doc)->document != NULL)
						delete ((*doc)->document);
				}
				else
					ECHO_PRINT("no content found in xml file\n");
			}
			catch(std::runtime_error re)
			{
				ECHO_PRINT("xml file could not be opened!\n");
			}
			if(f != NULL)
				delete f;
		}
		return(FAIL);
	}
	STATUS echo_xml_save_file(echo_xml* doc)
	{
		if(doc != NULL)
		{
			std::ofstream file;
			file.open(doc->file_name);
			if(file)
			{
				file << *(doc->document);
				file.close();
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_root(echo_xml* doc, echo_xml_element** root)
	{
		if(doc != NULL && root != NULL) 
		{
			*root = doc->document->first_node();
			if(*root != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_to_element(echo_xml_node* node, echo_xml_element** e)
	{
		if(node != NULL && e != NULL)
		{
			*e = node;
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_attribute(echo_xml_element* e, const char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			rapidxml::xml_attribute<>* attr = e->first_attribute();
			while(attr != NULL)
			{
				if(!strcmp(attr->name(), key))
				{
					*value = attr->value();
					if(*value != NULL)
						return(WIN);
				}
				attr = attr->next_attribute();
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_int_attribute(echo_xml_element* e, const char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atoi(*attr_save);
				delete attr_save;
				return(WIN);
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_float_attribute(echo_xml_element* e, const char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atof(*attr_save);
				delete attr_save;
				return(WIN);
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child)
	{
		if(node != NULL && child != NULL)
		{
			*child = node->first_node();
			if(*child != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling)
	{
		if(node != NULL && sibling != NULL)
		{
			*sibling = node->next_sibling();
			if(*sibling != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_set_attribute(echo_xml_element* e, const char* key, const char* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			rapidxml::xml_attribute<>* attr = e->first_attribute();
			while(attr != NULL)
			{
				if(!strcmp(attr->name(), key))
				{
					attr->value(value);
					return(WIN);
				}
				attr = attr->next_attribute();
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_node_type(echo_xml_node* node, echo_xml_type* t)
	{
		if(node != NULL && t != NULL)
		{
			*t = node->type();
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_tagname(echo_xml_element* e, char** tag)
	{
		if(e != NULL && tag != NULL)
		{
			*tag = e->name();
			if(*tag != NULL)
				return(WIN);
		}
		return(FAIL);
	}
#elif defined(USE_PUGIXML)
	#ifdef ECHO_NDS
		#include <pugixml.hpp>
	#else
		#include <pugixml/pugixml.hpp>
	#endif
	STATUS echo_xml_load_file(echo_xml** doc, char* filename)
	{
		if(doc != NULL && filename != NULL)
		{
			*doc = new(echo_xml);
			CHKPTR(*doc);
			(*doc)->file_name = filename;
			(*doc)->document = new pugi::xml_document();
			CHKPTR((*doc)->document);
			if((*doc)->document->load_file(filename) == true)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_save_file(echo_xml* doc)
	{
		if(doc != NULL && doc->document != NULL)
		{
			if(doc->document->save_file(doc->file_name) == true)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_root(echo_xml* doc, echo_xml_element** root)
	{
		if(doc != NULL && root != NULL) 
		{
			*root = doc->document->first_child_pointer();
			if(*root != NULL)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_to_element(echo_xml_node* node, echo_xml_element** e)
	{
		if(node != NULL && e != NULL)
		{
			*e = node;
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_attribute(echo_xml_element* e, const char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			*value = const_cast<char*>(e->attribute(key).value());
			if(*value != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_int_attribute(echo_xml_element* e, const char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atoi(*attr_save);
				if(*value != 0 || !strcmp(*attr_save, "0")) //make sure it isn't equal to zero, or it has to be equal to zero
				{
					delete attr_save;
					return(WIN);
				}
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_float_attribute(echo_xml_element* e, const char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atof(*attr_save);
				if(*value != 0 || !strcmp(*attr_save, "0")) //make sure it isn't equal to zero, or it has to be equal to zero
				{
					delete attr_save;
					return(WIN);
				}
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child)
	{
		if(node != NULL && child != NULL)
		{
			*child = node->first_child_pointer();
			if(*child != NULL)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling)
	{
		if(node != NULL && sibling != NULL)
		{
			echo_xml_node* sib_prev = *sibling;
			*sibling = node->next_sibling_pointer();
			if(sib_prev == node)
				delete node;
			if(*sibling != NULL)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_set_attribute(echo_xml_element* e, const char* key, const char* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->attribute(key).set_value(value) == true);
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_node_type(echo_xml_node* node, echo_xml_type* t)
	{
		if(node != NULL && t != NULL)
		{
			*t = node->type();
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_tagname(echo_xml_element* e, char** tag)
	{
		if(e != NULL && tag != NULL)
		{
			*tag = const_cast<char*>(e->name());
			if(*tag != NULL)
				return(WIN);
		}
		return(FAIL);
	}
#else
	#include <pugxml.h>
	STATUS echo_xml_load_file(echo_xml** doc, char* filename)
	{
		if(doc != NULL && filename != NULL)
		{
			*doc = new(echo_xml);
			CHKPTR(*doc);
			(*doc)->file_name = filename;
			(*doc)->document = new pug::xml_document();
			CHKPTR((*doc)->document);
			if((*doc)->document->load_file(filename) == true)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_save_file(echo_xml* doc)
	{
		if(doc != NULL && doc->document != NULL)
		{
			if(doc->document->save_file(doc->file_name) == true)
			{
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_root(echo_xml* doc, echo_xml_element** root)
	{
		if(doc != NULL && root != NULL) 
		{
			echo_xml_element e = doc->document->first_child();
			if(e.empty() == false)
			{
				*root = &e;
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_to_element(echo_xml_node* node, echo_xml_element** e)
	{
		if(node != NULL && e != NULL)
		{
			*e = node;
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_attribute(echo_xml_element* e, const char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			*value = const_cast<char*>(e->attribute(key).value());
			if(*value != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_int_attribute(echo_xml_element* e, const char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atoi(*attr_save);
				if(*value != 0 || !strcmp(*attr_save, "0")) //make sure it isn't equal to zero, or it has to be equal to zero
				{
					delete attr_save;
					return(WIN);
				}
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_float_attribute(echo_xml_element* e, const char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atof(*attr_save);
				if(*value != 0 || !strcmp(*attr_save, "0")) //make sure it isn't equal to zero, or it has to be equal to zero
				{
					delete attr_save;
					return(WIN);
				}
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child)
	{
		if(node != NULL && child != NULL)
		{
			echo_xml_node n = node->first_child();
			
			if(n.empty() == false)
			{
				std::cout << "<node>" << n;
				//n.print(std::cout);
				std::cout << "</node>" << std::endl;
				*child = &n;
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling)
	{
		if(node != NULL && sibling != NULL)
		{
			echo_xml_node n = node->next_sibling();
			if(n.empty() == false)
			{
				if(*sibling == node)
					delete *sibling;
				*sibling = &n;
				return(WIN);
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_set_attribute(echo_xml_element* e, const char* key, const char* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->attribute(key).set_value(value) == true);
				return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_node_type(echo_xml_node* node, echo_xml_type* t)
	{
		if(node != NULL && t != NULL)
		{
			*t = node->type();
			return(WIN);
		}
		return(FAIL);
	}
	STATUS echo_xml_get_tagname(echo_xml_element* e, char** tag)
	{
		if(e != NULL && tag != NULL)
		{
			*tag = const_cast<char*>(e->name());
			if(*tag != NULL)
				return(WIN);
		}
		return(FAIL);
	}
#endif

STATUS echo_xml_delete_file(echo_xml* doc)
{
	if(doc != NULL)
	{
		//leave doc->file_name alone...
		delete doc->document;
		delete doc;
		return(WIN);
	}
	return(FAIL);
}
