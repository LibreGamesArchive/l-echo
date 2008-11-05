// echo_xml.cpp

#include <cstdlib>

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
	int echo_xml_load_file(echo_xml** doc, char* filename)
	{
		if(doc != NULL && filname != NULL)
		{
			*doc = new(echo_xml);
			(*doc)->file_name = filename;
			(*doc)->document = new TiXmlDocument(filename);
			if((*doc)->document->LoadFile())
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_save_file(echo_xml* doc)
	{
		if(doc == NULL)
		{
			if(doc->document->SaveFile(doc->file_name) == TRUE)
			{
				delete document;
				return(WIN);
			}
			delete document;
		}
		return(FAIL);
	}
	int echo_xml_get_root(echo_xml* doc, echo_xml_element** root)
	{
		if(doc != NULL && root != NULL)
		{
			*root = doc->document->RootElement();
			if(*root != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_to_element(echo_xml_node* node, echo_xml_element** e)
	{
		if(node != NULL && e != NULL)
		{
			*e = node->ToElement();
			if(*e != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_get_attribute(echo_xml_element* e, char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			*value = e->Attribute(key);
			if(*value != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_get_int_attribute(echo_xml_element* e, char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->QueryIntAttribute(key, value) == TIXML_SUCCESS)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_get_float_attribute(echo_xml_element* e, char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			if(e->QueryFloatAttribute(key, value) == TIXML_SUCCESS)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_get_first_child(echo_xml_node* node, echo_xml_node** child)
	{
		if(node != NULL && child != NULL)
		{
			*child = node->FirstChild();
			if(*child != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_next_sibling(echo_xml_node* node, echo_xml_node** sibling)
	{
		if(node != NULL && sibling != NULL)
		{
			*sibling = node->NextSibling();
			if(*sibling != NULL)
				return(WIN);
		}
		return(FAIL);
	}
	int echo_xml_set_attribute(echo_xml_element* e, char* key, const char* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			e->SetAttribute(key, value);
		}
		return(FAIL);
	}
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
						ECHO_PRINT("prefs file parse error\n");
					}
					if((*doc)->document != NULL)
						delete ((*doc)->document);
				}
				else
					ECHO_PRINT("no content found in prefs file\n");
			}
			catch(std::runtime_error re)
			{
				ECHO_PRINT("prefs file could not be opened!\n");
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
	STATUS echo_xml_get_attribute(echo_xml_element* e, char* key, char** value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			rapidxml::xml_attribute<>* attr = e->first_attribute();
			while(attr != NULL)
			{
				if(!strcmp(attr->name(), key))
				{
					*value = attr->value();
					return(WIN);
				}
				attr = attr->next_attribute();
			}
		}
		return(FAIL);
	}
	STATUS echo_xml_get_int_attribute(echo_xml_element* e, char* key, int* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atoi(*attr_save);
			}
			delete attr_save;
		}
		return(FAIL);
	}
	STATUS echo_xml_get_float_attribute(echo_xml_element* e, char* key, float* value)
	{
		if(e != NULL && key != NULL && value != NULL)
		{
			char** attr_save = new(char*);
			CHKPTR(attr_save);
			if(echo_xml_get_attribute(e, key, attr_save) == WIN)
			{
				*value = atof(*attr_save);
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
	STATUS echo_xml_set_attribute(echo_xml_element* e, char* key, const char* value)
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
#endif

STATUS echo_xml_delete_file(echo_xml* doc)
{
	//leave doc->file_name alone...
	delete doc->document;
	delete doc;
	return(WIN);
}
