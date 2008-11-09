// echo_loader.cpp

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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>

#include <vector>
#include <map>

#include <filter.h>
#include <trigger.h>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_stage.h>
#include <echo_platform.h>

#include <launcher.h>
#include <freeform_grid.h>
#include <t_grid.h>
#include <escgrid.h>
#include <hole.h>
#include <grid.h>
#include <stair.h>

#include <echo_xml.h>

//#define LOAD_DEBUG

#ifdef LOAD_DEBUG
	#define LD_PRINT(...)	ECHO_PRINT(__VA_ARGS__)
#else
	#define LD_PRINT(...)
#endif

class functor
{
public:
        grid* obj;
        void (grid::*funcp)(grid*);
        functor()
	{
		obj = NULL;
		funcp = NULL;
	}
	virtual ~functor(){}
        functor(grid* my_obj, void (grid::*my_funcp)(grid*))
        {
		obj = my_obj;
		funcp = my_funcp;
        }
        virtual void call(grid* ptr) const
        {
		(obj->*funcp)(ptr);
        }
};

class t_functor : public functor
{
public:
        t_grid* t_obj;
        void (t_grid::*t_funcp)(grid*);
	t_functor()
	{
		t_obj = NULL;
		t_funcp = NULL;
	}
	virtual ~t_functor(){}
        t_functor(t_grid* my_obj, void (t_grid::*my_funcp)(grid*))
        {
		t_obj = my_obj;
		t_funcp = my_funcp;
        }
        virtual void call(grid* ptr) const
        {
		(t_obj->*t_funcp)(ptr);
        }
};
class filter_functor : public functor
{
public:
        filter* f_obj;
        void (filter::*f_funcp)(grid*);
	filter_functor()
	{
		f_obj = NULL;
		f_funcp = NULL;
	}
	virtual ~filter_functor(){}
        filter_functor(filter* my_obj, void (filter::*my_funcp)(grid*))
        {
		f_obj = my_obj;
		f_funcp = my_funcp;
        }
        virtual void call(grid* ptr) const
        {
		(f_obj->*f_funcp)(ptr);
        }
};
class trigger_functor : public functor
{
public:
        trigger* t_obj;
        void (trigger::*t_funcp)(grid*);
	trigger_functor()
	{
		t_obj = NULL;
		t_funcp = NULL;
	}
	virtual ~trigger_functor(){}
        trigger_functor(trigger* my_obj, void (trigger::*my_funcp)(grid*))
        {
		t_obj = my_obj;
		t_funcp = my_funcp;
        }
        virtual void call(grid* ptr) const
        {
		(t_obj->*t_funcp)(ptr);
        }
};

typedef std::vector<functor*> FUNCTOR_VEC;
typedef std::map<std::string, FUNCTOR_VEC*> DEPENDENCY_MAP;

#ifdef ECHO_NDS
	static grid* parse_grid(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot
			, LEVEL_MAP* nonffgrids, LEVEL_MAP* ffgrids);
#else
	static grid* parse_grid(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot);
#endif

void delete_functors(FUNCTOR_VEC* vec)
{
	FUNCTOR_VEC::iterator it = vec->begin();
	while(it != vec->end())
	{
		functor* func = *it;
		if(func != NULL)
			delete func;
		it++;
	}
	delete vec;
}

void delete_dependencies(DEPENDENCY_MAP* map)
{
	DEPENDENCY_MAP::iterator it = map->begin();
	while(it != map->end())
	{
		FUNCTOR_VEC* vec = it->second;
		if(vec != NULL)
			delete_functors(vec);
		it++;
	}
	delete map;
}

stage* load_stage(char* file_name)
{
	ECHO_PRINT("at loading\n");
	echo_xml** doc = new(echo_xml*);
	CHKPTR(doc);
	ECHO_PRINT("loading.................\n");
	if(echo_xml_load_file(doc, file_name) == WIN)
	{
		//-------------------------------------------------------------prepare stuff
		ECHO_PRINT("loaded file\n");
		DEPENDENCY_MAP* map = new DEPENDENCY_MAP();
		//ECHO_PRINT("map init\n");
		LD_CHKPTR(map);
		//ECHO_PRINT("map not null\n");
		stage* ret = new stage();
		LD_CHKPTR(ret);
#ifdef ECHO_NDS
		LEVEL_MAP* nonffgrids = new LEVEL_MAP();
		LD_CHKPTR(nonffgrids);
		LEVEL_MAP* ffgrids = new LEVEL_MAP();
		LD_CHKPTR(ffgrids);
#endif
		//(*doc)->document->print(std::cout);
		//-------------------------------------------------------------get root
		echo_xml_element** root = new(echo_xml_element*);
		
		CHKPTR(root);
		if(echo_xml_get_root(*doc, root) == FAIL)
		{
			lderr("cannot find root element!");
			delete root;
			echo_xml_delete_file(*doc);
			delete doc;
			delete_dependencies(map);
			delete ret;
#ifdef ECHO_NDS
			delete nonffgrids;
			delete ffgrids;
#endif
			return(NULL);
		}
		//-------------------------------------------------------------parse all grids
		echo_xml_node** child = new(echo_xml_node*);
		CHKPTR(child);
		if(echo_xml_get_first_child(*root, child) == WIN)
		{
			echo_xml_element** e = new(echo_xml_element*);
			CHKPTR(e);
			echo_xml_type type = ECHO_XML_TYPE_UNKNOWN;
			do
			{
				if(echo_xml_get_node_type(*child, &type) == WIN)
				{
					if(type == ECHO_XML_TYPE_ELEMENT)
					{
#ifdef ECHO_NDS
						if(echo_xml_to_element(*child, e) != WIN || parse_grid(*e, ret, map, NULL, nonffgrids, ffgrids) == NULL)
#else
						if(echo_xml_to_element(*child, e) != WIN || parse_grid(*e, ret, map, NULL) == NULL)
#endif
						{
							lderr("parse not successful!");
							delete child;
							delete e;
							echo_xml_delete_file(*doc);
							delete doc;
							delete_dependencies(map);
							delete ret;
#ifdef ECHO_NDS
							delete nonffgrids;
							delete ffgrids;
#endif
							return(NULL);
						}
					}
					else if(type != ECHO_XML_TYPE_COMMENT)
					{
						
						lderr("unknown node type!");
						delete child;
						delete e;
						echo_xml_delete_file(*doc);
						delete doc;
						delete_dependencies(map);
						delete ret;
#ifdef ECHO_NDS
						delete nonffgrids;
						delete ffgrids;
#endif
						return(NULL);
					}
				}
				else
				{
					lderr("couldn't get node type!\n");
					delete child;
					delete e;
					echo_xml_delete_file(*doc);
					delete doc;
					delete_dependencies(map);
					delete ret;
#ifdef ECHO_NDS
					delete nonffgrids;
					delete ffgrids;
#endif
					return(NULL);
				}
			}
			while(echo_xml_next_sibling(*child , child));
			delete e;
		}
		delete child;
		//-------------------------------------------------------------get starting point string
		char** start = new(char*);
		CHKPTR(start);
		if(echo_xml_get_attribute(*root, "start", start) == FAIL)
		{
			lderr("no starting point specified!\n");
			delete start;
			echo_xml_delete_file(*doc);
			delete doc;
			delete_dependencies(map);
			delete ret;
#ifdef ECHO_NDS
			delete nonffgrids;
			delete ffgrids;
#endif
			return(NULL);
		}
		else
			LD_PRINT("start: %s\n", start);
		//-------------------------------------------------------------get grid, set to stage
		grid* start_grid = ret->get(*start);
		if(start_grid == NULL)
		{
			lderr("start grid not found...\n");
			delete start;
			echo_xml_delete_file(*doc);
			delete doc;
			delete_dependencies(map);
			delete ret;
#ifdef ECHO_NDS
			delete nonffgrids;
			delete ffgrids;
#endif
			return(NULL);
		}
		ret->set_start(start_grid);
		delete start;
		//-------------------------------------------------------------get/set name
		char** name = new(char*);
		CHKPTR(name);
		if(echo_xml_get_attribute(*root, "name", name) == FAIL)
		{
			lderr("name of stage not specified!\n");
			delete name;
			echo_xml_delete_file(*doc);
			delete doc;
			delete_dependencies(map);
			delete ret;
#ifdef ECHO_NDS
			delete nonffgrids;
			delete ffgrids;
#endif
			return(NULL);
		}
		ret->set_name(new std::string(*name));
		delete name;
		//-------------------------------------------------------------get num goals
		int num_goals = 0;
		if(echo_xml_get_int_attribute(*root, "goals", &num_goals) == FAIL)
		{
			lderr("cannot find number of goals!\n");
			echo_xml_delete_file(*doc);
			delete doc;
			delete_dependencies(map);
			delete ret;
#ifdef ECHO_NDS
			delete nonffgrids;
			delete ffgrids;
#endif
			return(NULL);
		}
		ret->set_num_goals(num_goals);
		//-------------------------------------------------------------delete docs and dependencies
		if(!map->empty())
			ldwarn("dependencies not satisfied...\n");
		delete_dependencies(map);
		echo_xml_delete_file(*doc);
		delete doc;
		delete root;
		//-------------------------------------------------------------hand out the polyIDs
#ifdef ECHO_NDS
#define GRID_POLYID_START	19
		unsigned int polyID = GRID_POLYID_START;
		LEVEL_MAP::iterator it = nonffgrids->begin(), end = nonffgrids->end();
		GRID_PTR_SET::iterator git, gend;
		while(it != end)
		{
			git = it->second->begin();
			gend = it->second->end();
			while(git != gend)
			{
				(*git)->set_polyID(polyID);
				git++;
			}
			ECHO_PRINT("polyID: %i (height: %f)\n", polyID, it->first);
			polyID++;
			if(polyID >= 64)
				polyID = GRID_POLYID_START;	//er...no better way.
			it++;
		}
		it = ffgrids->begin();
		end = ffgrids->end();
		while(it != end)	//NOTE: NOT COMPLETE!  Only ffgrids with the same or opposing normals can have the same polyID!
		{
			git = it->second->begin();
			gend = it->second->end();
			while(git != gend)
			{
				(*git)->set_polyID(polyID);
				git++;
			}
			ECHO_PRINT("polyID: %i (height: %f)\n", polyID, it->first);
			polyID++;
			if(polyID >= 64)
				polyID = GRID_POLYID_START;	//er...no better way.
			it++;
		}
		
		delete nonffgrids;
		delete ffgrids;
#endif
		//ret->dump_levels();
		return(ret);
	}
	else
	{
		lderr("cannot open file! (might not be correct xml file): ", file_name);
		echo_xml_delete_file(*doc);
		return(NULL);
	}
	return(NULL);
}

static FUNCTOR_VEC* dep_set(DEPENDENCY_MAP* map, char* id)
{
	std::string* s1 = new std::string(id);
	LD_CHKPTR(s1);
	DEPENDENCY_MAP::iterator it = map->find(*s1);
	delete s1;
	return(it == map->end() ? NULL : it->second);
}

static void add(DEPENDENCY_MAP* map, char* id, functor*  f)
{
	FUNCTOR_VEC* set = dep_set(map, id);
	if(set)
	{
		LD_PRINT("dep set for %s found, adding\n", id);
		set->push_back(f);
	}
	else
	{
		LD_PRINT("dep set for %s NOT found, adding\n", id);
		set = new FUNCTOR_VEC();
		LD_CHKPTR(set);
		set->push_back(f);
		map->insert(DEPENDENCY_MAP::value_type(id, set));
	}
}

static void add(DEPENDENCY_MAP* map, char* id, t_grid* obj, void (t_grid::*funcp)(grid*))
{
#ifdef STRICT_MEM
	t_functor* f = new t_functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, f);
#else
	add(map, id, new t_functor(obj, funcp));
#endif
}

static void add(DEPENDENCY_MAP* map, char* id, grid* obj, void (grid::*funcp)(grid*))
{
#ifdef STRICT_MEM
	functor* f = new functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, f);
#else
	add(map, id, new t_functor(obj, funcp));
#endif
}

static void add(DEPENDENCY_MAP* map, char* id, filter* obj, void (filter::*funcp)(grid*))
{
#ifdef STRICT_MEM
	filter_functor* f = new filter_functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, f);
#else
	add(map, id, new filter_functor(obj, funcp));
#endif
}

static void add(DEPENDENCY_MAP* map, char* id, trigger* obj, void (trigger::*funcp)(grid*))
{
#ifdef STRICT_MEM
	trigger_functor* f = new trigger_functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, f);
#else
	add(map, id, new trigger_functor(obj, funcp));
#endif
}

/*
static int get_float(TiXmlElement* element, const char* attr, float* save_to)
{
	int result = element->QueryFloatAttribute(attr, save_to);
	if(result == TIXML_NO_ATTRIBUTE)
	{
		lderr("cannot find float attribute: ", attr);
		return(0);
	}
	else if(result == TIXML_WRONG_TYPE)
	{
		lderr("attribute should be float: ", attr);
		return(0);
	}
	return(1);
	
}
// */

static int get_vec(echo_xml_element* txe, vector3f* vec, stage* st = NULL)
{
	//std::cout << "get_vec: " << *txe << std::endl;
	const int result = echo_xml_get_float_attribute(txe, "x", &vec->x) == WIN
		&& echo_xml_get_float_attribute(txe, "y", &vec->y) == WIN
		&& echo_xml_get_float_attribute(txe, "z", &vec->z) == WIN;
	if(st)
		st->set_farthest(vec->length());
	return(result);
}

static int get_angle(echo_xml_element* txe, vector3f* vec)
{
	return(echo_xml_get_float_attribute(txe, "x", &vec->x) == WIN
			&& echo_xml_get_float_attribute(txe, "y", &vec->y) == WIN);
}

#ifdef ECHO_NDS
static int add_esc(echo_xml_element* child, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* egrid
			, LEVEL_MAP* nonffgrids, LEVEL_MAP* ffgrids)
#else
static int add_esc(echo_xml_element* child, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* egrid)
#endif
{
	char** type = new(char*);
	CHKPTR(type);
	if(echo_xml_get_tagname(child, type) == WIN)
	{
		if(!strcmp(*type, "angle"))
		{
			vector3f* each_angle = new vector3f();
			LD_CHKPTR(each_angle);
			if(get_angle(child, each_angle) == WIN)
			{
				echo_xml_node** first = new(echo_xml_node*);
				CHKPTR(first);
				if(echo_xml_get_first_child(child, first) == WIN)
				{
					echo_xml_element** e = new(echo_xml_element*);
					CHKPTR(e);
					if(echo_xml_to_element(*first, e) == WIN)
					{
#ifdef ECHO_NDS
						grid* g = parse_grid(*e, st, map, escroot ? escroot : egrid, nonffgrids, ffgrids);
#else
						grid* g = parse_grid(*e, st, map, escroot ? escroot : egrid);
#endif
						if(g != NULL)
						{
							egrid->add(each_angle, g);
							delete first;
							delete e;
							delete type;
							return(WIN);
						}
						else
							lderr("parse grid error in add_esc!");
					}
					else
						lderr("couldn't convert to element error in add_esc!");
					delete e;
				}
				else
					lderr("no esc in angle!");
				delete first;
			}
			else
				lderr("couldn't get angle in add_esc!");
			delete each_angle;
		}
		else if(!strcmp(*type, "range"))
		{
			vector3f* v1 = new vector3f();
			LD_CHKPTR(v1);
			if(echo_xml_get_float_attribute(child, "x_min", &(v1->x)) == WIN
				&& echo_xml_get_float_attribute(child, "y_min", &(v1->y)) == WIN)
			{
				vector3f* v2 = new vector3f();
				LD_CHKPTR(v2);
				if(echo_xml_get_float_attribute(child, "x_max", &(v2->x)) == WIN
					&& echo_xml_get_float_attribute(child, "y_max", &(v2->y)) == WIN)
				{
					echo_xml_node** first = new(echo_xml_node*);
					LD_CHKPTR(first);
					if(echo_xml_get_first_child(child, first) == WIN)
					{
						echo_xml_element** e = new(echo_xml_element*);
						LD_CHKPTR(e);
						if(echo_xml_to_element(*first, e) == WIN)
						{
#ifdef ECHO_NDS
							grid* g = parse_grid(*e, st, map, escroot ? escroot : egrid, nonffgrids, ffgrids);
#else
							grid* g = parse_grid(*e, st, map, escroot ? escroot : egrid);
#endif
							if(g != NULL)
							{
								angle_range* range = new angle_range(v1, v2);
								//ECHO_PRINT("range added\n");
								CHKPTR(range);
								egrid->add(range, g);
								delete type;
								delete e;
								delete first;
								return(WIN);
							}
							else
								lderr("parse grid error in add_esc!");
						}
						else
							lderr("couldn't convert to element error in add_esc!");
						delete e;
					}
					else
						lderr("no esc in angle range!");
					delete first;
				}
				else
					lderr("couldn't get max for angle range!");
				delete v2;
			}
			else
				lderr("couldn't get min for angle range!");
			delete v1;
			
		}
		else
			lderr("child of escgrid, hole or launcher is not an angle or range!");
	}
	delete type;
	return(FAIL);
}

#ifdef ECHO_NDS
static int add_escs(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid
			, LEVEL_MAP* nonffgrids, LEVEL_MAP* ffgrids)
#else
static int add_escs(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid)
#endif
{
	echo_xml_node** first = new(echo_xml_node*);
	CHKPTR(first);
	if(echo_xml_get_first_child(txe, first) == WIN)
	{
		echo_xml_type type = ECHO_XML_TYPE_UNKNOWN;
		char** tag = new(char*);
		CHKPTR(tag);
		echo_xml_element** e = new(echo_xml_element*);
		CHKPTR(e);
		do
		{
			if(echo_xml_get_node_type(*first, &type) == WIN)
			{
				if(type == ECHO_XML_TYPE_ELEMENT)
				{
					if(echo_xml_to_element(*first, e) == WIN)
					{
						//std::cout << "Escs Element: "<< **e << std::endl;
						if(echo_xml_get_tagname(*e, tag) == WIN && strcmp(*tag, "triggers"))
#ifdef ECHO_NDS
							add_esc(*e, st, map, escroot, grid, nonffgrids, ffgrids);
#else
							add_esc(*e, st, map, escroot, grid);
#endif
						else
						{
							lderr("couldn't get tag name in escgrid!");
							delete first;
							delete e;
							delete tag;
							return(FAIL);
						}
					}
					else
					{
						lderr("couldn't convert to element in escgrid!");
						delete first;
						delete e;
						delete tag;
						return(FAIL);
					}
				}
				else if(type != ECHO_XML_TYPE_COMMENT)
				{
					lderr("unknown node in escgrid!");
					delete first;
					delete e;
					delete tag;
					return(FAIL);
				}
			}
			else
			{
				lderr("couldn't get node type in escgrid!");
				delete first;
				delete e;
				delete tag;
				return(FAIL);
			}
		}
		while(echo_xml_next_sibling(*first, first) == WIN);
		delete tag;
		delete e;
	}
	delete first;
	return(WIN);
}

static char* get_attribute(echo_xml_element* txe, const char* attr, const char* errmsg1, const char* errmsg2)
{
	char** retp = new(char*);
	if(echo_xml_get_attribute(txe, attr, retp) == FAIL)
	{
		lderr(errmsg1, errmsg2);
		return(NULL);
	}
	char* ret = *retp;
	delete retp;
	return(ret);
}

static char* get_attribute(echo_xml_element* txe, const char* attr, const char* errmsg)
{
	char** retp = new(char*);
	if(echo_xml_get_attribute(txe, attr, retp) == FAIL)
	{
		lderr(errmsg);
		return(NULL);
	}
	char* ret = *retp;
	delete retp;
	return(ret);
}

static filter* get_filter(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, char* type = NULL)
{
	if(type == NULL)
	{
		if(echo_xml_get_tagname(txe, &type) == FAIL)
		{
			lderr("type unknown for filter...");
			return(NULL);
		}
	}
	//std::cout << "filter:"<< *txe << std::endl;
	ECHO_PRINT("type of filter:%s\n", type);
	if(!strcmp(type, "goal"))
	{
		char* name = get_attribute(txe, "id", "no id for goal filter");
		if(name != NULL)
		{
			filter* ret = new filter();
			LD_CHKPTR(ret);
			grid* g = st->get(name);
			if(g)
				ret->set_target(g);
			else
			{
				LD_PRINT("filter target (%s) is null, adding to dep map: ", name);
				add(map, name, ret, &filter::set_target);
			}
			return(ret);
		}
	}
	else if(!strcmp(type, "not"))
	{
		echo_xml_node** first = new(echo_xml_node*);
		LD_CHKPTR(first);
		if(echo_xml_get_first_child(txe, first) == WIN)
		{
			echo_xml_type type = ECHO_XML_TYPE_UNKNOWN;
			//while(child != NULL && child->Type() != TiXmlNode::ELEMENT)
			//	child = child->NextSibling();
			while(echo_xml_get_node_type(*first, &type) == WIN 
				&& type != ECHO_XML_TYPE_ELEMENT
				&& echo_xml_next_sibling(*first, first) == WIN);
			
			if(*first != NULL)
			{
				echo_xml_element** e = new(echo_xml_element*);
				LD_CHKPTR(e);
				if(echo_xml_to_element(*first, e) == WIN)
				{
					not_filter* ret = new not_filter(get_filter(*e, st, map));
					LD_CHKPTR(ret);
					delete e;
					delete first;
					return(ret);
				}
				else
					lderr("couldn't convert to element in \"not\" filter!");
				delete e;
			}
			else
				lderr("no filter element in \"not\" filter!");
		}
		else
			lderr("\"not\" filter has no item inside");
		delete first;
	}
	else if(!strcmp(type, "or") || !strcmp(type, "and"))
	{
		multi_filter* ret = (!strcmp(type, "or")) ?
				dynamic_cast<multi_filter*>(new or_filter()) : 
				dynamic_cast<multi_filter*>(new and_filter());
		LD_CHKPTR(ret);
		int error = false;
		echo_xml_node** first = new(echo_xml_node*);
		LD_CHKPTR(first);
		if(echo_xml_get_first_child(txe, first) == WIN)
		{
			echo_xml_type type = ECHO_XML_TYPE_UNKNOWN;
			echo_xml_element** e = new(echo_xml_element*);
			LD_CHKPTR(e);
			do
			{
				if(echo_xml_get_node_type(*first, &type) == WIN)
				{
					if(type == ECHO_XML_TYPE_ELEMENT)
					{
						if(echo_xml_to_element(*first, e) == WIN)
						{
							ret->add_filter(get_filter(*e, st, map));
						}
						else
						{
							error = true;
							lderr("couldn't convert to element in \"or\" filter\n");
						}
					}
					else if(type != ECHO_XML_TYPE_COMMENT)
					{
						error = true;
						lderr("unknown node in \"or\" filter\n");
					}
				}
				else
				{
					error = true;
					lderr("couldn't get node type for \"or\" filter\n");
				}
			}
			while(echo_xml_next_sibling(*first, first) == WIN && error == false);
			delete e;
		}
		else
		{
			error = true;
			lderr("\"or\" filter has no item inside\n");
		}
		delete first;
		if(error == true)	delete ret;
		else				return(ret);
	}
	else
		lderr("filter type unknown\n");
	return(NULL);
}

static trigger* get_trigger(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map)
{
	filter* f = NULL;
	echo_xml_node** first = new(echo_xml_node*);
	LD_CHKPTR(first);
	if(echo_xml_get_first_child(txe, first) == WIN)
	{
		echo_xml_element** e = new(echo_xml_element*);
		LD_CHKPTR(e);
		if(echo_xml_to_element(*first, e) == WIN)
		{
			f = get_filter(*e, st, map, "and");
			if(f == NULL)
			{
				delete e;
				delete first;
				return(NULL);
			}
		}
		delete e;
	}
	delete first;
	char* name = get_attribute(txe, "id", "no id for trigger");
	if(name != NULL)
	{
		grid* g = st->get(name);
		if(g)
		{
			trigger* ret = new trigger(f, g);
			LD_CHKPTR(ret);
			return(ret);
		}
		else
		{
			trigger* ret = new trigger(f);
			LD_CHKPTR(ret);
			LD_PRINT("trigger target (%s) is null, adding to dep map\n", name);
			add(map, name, ret, &trigger::set_target);
			return(ret);
		}
	}
	return(NULL);
}

static int add_triggers(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, grid* g)
{
	echo_xml_node** first = new(echo_xml_node*);
	LD_CHKPTR(first);
	int error = false;
	if(echo_xml_get_first_child(txe, first) == WIN)
	{
		//std::cout << "add_triggers: " << *txe << std::endl;
		echo_xml_type type = ECHO_XML_TYPE_UNKNOWN;
		echo_xml_element** e = new(echo_xml_element*);
		LD_CHKPTR(e);
		do
		{
			//std::cout << "add_triggers: child: " << (*first)->name() << "; " << **first << std::endl;
			if(echo_xml_get_node_type(*first, &type) == WIN)
			{
				if(type == ECHO_XML_TYPE_ELEMENT)
				{
					if(echo_xml_to_element(*first, e) == WIN)
					{
						g->add_trigger(get_trigger(*e, st, map));
					}
					else
					{
						error = true;
						lderr("couldn't convert to element in trigger\n");
					}
				}
				else if(type != ECHO_XML_TYPE_COMMENT)
				{
					error = true;
					lderr("unknown node in trigger\n");
				}
			}
			else
			{
				error = true;
				lderr("couldn't get node type for trigger!\n");
			}
		}
		while(echo_xml_next_sibling(*first, first) == WIN);
		delete e;
	}
	else
	{
		lderr("triggers element has no triggers!");
		error = true;
	}
	delete first;
	return(error == true ? FAIL : WIN);
}

#ifdef ECHO_NDS
static grid* parse_grid(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot
			, LEVEL_MAP* nonffgrids, LEVEL_MAP* ffgrids)
#else
static grid* parse_grid(echo_xml_element* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot)
#endif
{
	LD_PRINT("\n");
	char* name = get_attribute(txe, "id", "unnamed grid!");
	if(name != NULL)
	{
		char** type = new(char*);
		LD_CHKPTR(type);
		if(echo_xml_get_tagname(txe, type) == WIN)
		{
			grid_info_t* info = new(grid_info_t);
			LD_CHKPTR(info);
			if(get_vec(txe, &(info->pos), st) == WIN)
			{
				char* prev_id = get_attribute(txe, "prev", "no previous for grid: ", name);
				if(prev_id != NULL)
				{
					grid* prev = st->get(prev_id);
					char* next_id = get_attribute(txe, "next", "no next for grid: ", name);
					if(next_id != NULL)
					{
						grid* next = st->get(next_id);
						grid* new_grid = NULL;
						if(!strcmp(*type, "grid"))
						{
							LD_PRINT("%s is a grid!\n", name);
							new_grid = new grid(info, prev, next);
							LD_CHKPTR(new_grid);
						}
						else if(!strcmp(*type, "t_grid"))
						{
							LD_PRINT("%s is a t_grid!\n", name);
							char* next2_id = get_attribute(txe, "next2", "no next2 for t_grid: ", name);
							if(next2_id != NULL)
							{
								grid* next2 = st->get(next2_id);
								new_grid = new t_grid(info, prev, next, next2);
								LD_CHKPTR(new_grid);
								if(!next2 && strcmp(next2_id, "NONE"))
								{
									LD_PRINT("next2 (%s) is null, adding to dep map\n", next2_id);
									add(map, next2_id, (t_grid*)new_grid, &t_grid::set_real_next2);
								}
							}							
						}
						else if(!strcmp(*type, "escgrid"))
						{
							LD_PRINT("%s is a escgrid!\n", name);
							new_grid = new escgrid(info, prev, next);
							LD_CHKPTR(new_grid);
#ifdef ECHO_NDS
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid, nonffgrids, ffgrids))
#else
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid))
#endif
							{
								delete new_grid;
								new_grid = NULL;
							}
						}
						else if(!strcmp(*type, "hole"))
						{
							LD_PRINT("%s is an hole!\n", name);
							new_grid = new hole(info, prev, next);
							LD_CHKPTR(new_grid);
#ifdef ECHO_NDS
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid, nonffgrids, ffgrids))
#else
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid))
#endif
							{
								delete new_grid;
								new_grid = NULL;
							}
						}
						else if(!strcmp(*type, "launcher"))
						{
							LD_PRINT("%s is a launcher!\n", name);
							new_grid = new launcher(info, prev, next);
							LD_CHKPTR(new_grid);
#ifdef ECHO_NDS
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid, nonffgrids, ffgrids))
#else
							if(!add_escs(txe, st, map, escroot, (escgrid*)new_grid))
#endif
							{
								delete new_grid;
								new_grid = NULL;
							}
						}
						else if(!strcmp(*type, "freeform_grid"))
						{
							LD_PRINT("%s is a freeform_grid!\n", name);
							echo_xml_node** first = new(echo_xml_node*);
							LD_CHKPTR(first);
							if(echo_xml_get_first_child(txe, first) == WIN)
							{
								echo_xml_type type2 = ECHO_XML_TYPE_UNKNOWN;
								while(echo_xml_get_node_type(*first, &type2) == WIN 
									&& type2 != ECHO_XML_TYPE_ELEMENT
									&& echo_xml_next_sibling(*first, first) == WIN);
								if(*first != NULL)
								{
									vector3f* dir_angle = new vector3f();
									LD_CHKPTR(dir_angle);
									echo_xml_element** e = new(echo_xml_element*);
									LD_CHKPTR(e);
									if(echo_xml_to_element(*first, e) == WIN 
										&& get_vec(*e, dir_angle) == WIN)
									{
										if(echo_xml_next_sibling(*first, first) == WIN)
										{
											while(echo_xml_get_node_type(*first, &type2) == WIN 
												&& type2 != ECHO_XML_TYPE_ELEMENT
												&& echo_xml_next_sibling(*first, first) == WIN);
											if(*first != NULL)
											{
												vector3f* width_angle = new vector3f();
												LD_CHKPTR(width_angle);
												if(echo_xml_to_element(*first, e) == WIN 
													&& get_vec(*e, width_angle) == WIN)
												{
													new_grid = new freeform_grid(info, prev, next, dir_angle, width_angle);
													LD_CHKPTR(new_grid);
												}
												else
												{
													lderr("couldn't get width of freeform_grid!", name);
													delete width_angle;
													delete dir_angle;
												}
											}
											else
											{
												lderr("cannot find width element of freeform_grid: " , name);
												delete dir_angle;
											}
										}
										else
										{
											lderr("cannot find width element of freeform_grid: " , name);
											delete dir_angle;
										}
										delete e;
									}
									else
									{
										lderr("couldn't get direction of freeform grid!:", name);
										delete dir_angle;
									}
								}
								else
									lderr("cannot find direction element of freeform_grid: " , name);
							}
							else
								lderr("couldn't get first child in freeform_grid: ", name);
							delete first;
						}
						else if(!strcmp(*type, "stair"))
						{
							float angle = 45;
							if(echo_xml_get_float_attribute(txe, "direction", &angle) == WIN)
							{
								LD_PRINT("angle: %f\n", angle);
								new_grid = new stair(info, prev, next, angle);
								LD_CHKPTR(new_grid);
							}
							else
								lderr("couldn't get start direction!");
						}
						else
						{
							lderr("grid type not known!: ", *type);
						}
						if(new_grid != NULL)
						{
#ifdef ECHO_NDS
							if(!strcmp(*type, "freeform_grid"))
								map_add_pos(ffgrids, info->pos, new_grid);
							else if(strcmp(*type, "stair"))	//NOT EQUAL TO STAIRS ; no '!'
								map_add_pos(nonffgrids, info->pos, new_grid);
#endif
							//previous check
							if(!prev && strcmp(prev_id, "NONE"))
							{
								LD_PRINT("prev (%s) is null, adding to dep map\n", prev_id);
								add(map, prev_id, new_grid, &grid::set_real_prev);
							}
							//next check
							if(!next && strcmp(next_id, "NONE"))
							{
								LD_PRINT("next (%s) is null, adding to dep map\n", next_id);
								add(map, next_id, new_grid, &grid::set_real_next);
							}
							//goal check
							int is_goal = 0;
							if(echo_xml_get_int_attribute(txe, "goal", &is_goal) == WIN)
							{
								if(is_goal)
								{
									new_grid->set_as_goal();
									LD_PRINT("it's a goal!\n");
								}
							}
							//nodraw check
							int nodraw = 0;
							if(echo_xml_get_int_attribute(txe, "nodraw", &nodraw) == WIN)
							{
								if(nodraw)
								{
									new_grid->set_draw(0);
									LD_PRINT("it's invisible!\n");
								}
							}
							//noland check
							int noland = 0;
							if(echo_xml_get_int_attribute(txe, "noland", &noland) == WIN)
							{
								if(noland)
									LD_PRINT("it's not land-able!\n");
							}
							if(!escroot)
							{
								st->add(name, new_grid);
								if(!noland)
									st->add_pos(info->pos, new_grid);
							}
							else if(!noland)
							{
								st->add_pos(info->pos, escroot);
							}
							//trigger check
							echo_xml_node** first = new(echo_xml_node*);
							LD_CHKPTR(first);
							if(echo_xml_get_first_child(txe, first) == WIN)
							{
								echo_xml_type type2 = ECHO_XML_TYPE_UNKNOWN;
								while(echo_xml_get_node_type(*first, &type2) == WIN 
									&& type2 != ECHO_XML_TYPE_ELEMENT
									&& echo_xml_next_sibling(*first, first) == WIN);
								if(*first != NULL)
								{
									echo_xml_element** e = new(echo_xml_element*);
									LD_CHKPTR(e);
									if(echo_xml_to_element(*first, e) == WIN)
									{
										char** tag = new(char*);
										LD_CHKPTR(tag);
										if(echo_xml_get_tagname(*e, tag) == WIN 
											&& !strcmp(*tag, "triggers"))
										{
											LD_PRINT("adding triggers\n");
											add_triggers(*e, st, map, new_grid);
										}
										delete tag;
									}
									delete e;
								}
							}
							delete first;
							//dep check
							FUNCTOR_VEC* deps = dep_set(map, name);
							if(deps)
							{
								LD_PRINT("deps found for: %s\n", name);
								FUNCTOR_VEC::iterator it = deps->begin(), end = deps->end();
								while(it != end)
								{
									(*it)->call(new_grid);
									LD_PRINT("dep called for %s (%s) by %s", it->obj, typeid(*(it->obj)).name(), name);
									it++;
								}
								delete_functors(deps);
								map->erase(name);
							}
							else
								LD_PRINT("deps not found for: %s\n", name);
							return(new_grid);
							
						}
						if(new_grid != NULL)
							delete new_grid;
					}
					//no next specified
				}
				//no prev specified
			}
			else
				lderr("couldn't get position!");
			delete info;
		}
		else
			lderr("type not known for grid: " , name);
		delete type;
	}
	else
		lderr("couldn't get name of grid!\n");
	return(NULL);
	
}
