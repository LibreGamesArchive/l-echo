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

#include <echo_error.h>
#include <launcher.h>
#include <stair.h>
#include <t_grid.h>
#include <escgrid.h>
#include <hole.h>
#include <grid.h>
#include <echo_stage.h>
#include <tinyxml/tinyxml.h>

//#define LOAD_DEBUG

class functor
{
public:
        grid* obj;
        void (grid::*funcp)(grid*);
        functor(){}
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

typedef std::vector<functor> FUNCTOR_VEC;
typedef std::map<std::string, FUNCTOR_VEC*> DEPENDENCY_MAP;

static grid* parse_grid(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot);

stage* load_stage(const char* file_name)
{
	TiXmlDocument* doc = new TiXmlDocument(file_name);
	LD_CHKPTR(doc);
	if(doc->LoadFile())
	{
		DEPENDENCY_MAP* map = new DEPENDENCY_MAP();
		LD_CHKPTR(map);
		stage* ret = new stage();
		LD_CHKPTR(ret);
		TiXmlElement* root = doc->RootElement();
		if(!root)
			lderr("cannot find root element!");
		TiXmlNode* child = NULL;
		while((child = root->IterateChildren(child)) != NULL)
		{
			if(child->Type() == TiXmlNode::ELEMENT)
				parse_grid(child->ToElement(), ret, map, NULL);
			else if(child->Type() != TiXmlNode::COMMENT)
				lderr("unknown node type!");
		}
		const char* start = root->Attribute("start");
		if(!start)
			lderr("no starting point specified!");
		grid* start_grid = ret->get(start);
		if(!start_grid)
			lderr("start grid not found...");
		ret->set_start(start_grid);
		const char* name = root->Attribute("name");
		if(!name)
			lderr("name of stage not specified!");
		ret->set_name(*(new std::string(name)));
		int num_goals = 0;
		if(root->QueryIntAttribute("goals", &num_goals) != TIXML_SUCCESS)
			lderr("cannot find number of goals!");
		ret->set_num_goals(num_goals);
		
		if(!map->empty())
			ldwarn("dependencies not satisfied...");
		//ret->dump_levels();
		return(ret);
	}
	else
		lderr("cannot open file!: ", file_name);
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

static void add(DEPENDENCY_MAP* map, char* id, functor f)
{
	FUNCTOR_VEC* set = dep_set(map, id);
	if(set)
	{
#ifdef LOAD_DEBUG
		std::cout << "dep set for " << id << " found, adding"<< std::endl;
#endif
		set->push_back(f);
	}
	else
	{
#ifdef LOAD_DEBUG
		std::cout << "dep set for " << id << " NOT found, adding"<< std::endl;
#endif
		set = new FUNCTOR_VEC();
		LD_CHKPTR(set);
		set->push_back(f);
		map->insert(DEPENDENCY_MAP::value_type(id, set));
	}
}

static void add(DEPENDENCY_MAP* map, char* id, t_grid* obj, void (t_grid::*funcp)(grid*))
{
	t_functor* f = new t_functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, *f);
}

static void add(DEPENDENCY_MAP* map, char* id, grid* obj, void (grid::*funcp)(grid*))
{
	functor* f = new functor(obj, funcp);
	LD_CHKPTR(f);
	add(map, id, *f);
}

static void get_float(TiXmlElement* element, const char* attr, float* save_to)
{
	int result = element->QueryFloatAttribute(attr, save_to);
	if(result == TIXML_NO_ATTRIBUTE)
		lderr("cannot find float attribute: ", attr);
	else if(result == TIXML_WRONG_TYPE)
		lderr("attribute should be float: ", attr);
}

static void get_vec(TiXmlElement* txe, vector3f* vec)
{
	get_float(txe, "x", &vec->x);
	get_float(txe, "y", &vec->y);
	get_float(txe, "z", &vec->z);
}

static void add_esc(TiXmlElement* child, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid)
{
	const char* type = child->Value();
	if(!strcmp(type, "angle"))
	{
		vector3f* each_angle = new vector3f();
		LD_CHKPTR(each_angle);
		get_vec(child, each_angle);
		grid->add(each_angle
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
	else if(!strcmp(type, "range"))
	{
		vector3f* v1 = new vector3f();
		LD_CHKPTR(v1);
		get_float(child, "x_min", &v1->x);
		get_float(child, "y_min", &v1->y);
		get_float(child, "z_min", &v1->z);
		vector3f* v2 = new vector3f();
		LD_CHKPTR(v2);
		get_float(child, "x_max", &v2->x);
		get_float(child, "y_max", &v2->y);
		get_float(child, "z_max", &v2->z);
		grid->add(new angle_range(v1, v2)
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
	else
		lderr("child of escgrid, hole or launcher is not an angle or range!");
}

static char* get_attribute(TiXmlElement* txe, const char* attr, const char* errmsg1, const char* errmsg2)
{
	const char* ret_const = txe->Attribute(attr);
	if(!ret_const)
		lderr(errmsg1, errmsg2);
	return(const_cast<char*>(ret_const));
}

static char* get_attribute(TiXmlElement* txe, const char* attr, const char* errmsg)
{
	const char* ret_const = txe->Attribute(attr);
	if(!ret_const)
		lderr(errmsg);
	return(const_cast<char*>(ret_const));
}

static grid* parse_grid(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot)
{
#ifdef LOAD_DEBUG
	std::cout << std::endl;
#endif
	char* name = get_attribute(txe, "id", "unnamed grid!");
	const char* type = txe->Value();
	if(!type)
		lderr("type not known for grid: " , name);
	grid_info_t* info = new(grid_info_t);
	LD_CHKPTR(info);
	get_vec(txe, &(info->pos));
	char* prev_id = get_attribute(txe, "prev", "no previous for grid: ", name);
	grid* prev = st->get(prev_id);
	char* next_id = get_attribute(txe, "next", "no next for grid: ", name);
	grid* next = st->get(next_id);
	const char* trig_id = txe->Attribute("trig");
	grid* trig = trig_id ? st->get(const_cast<char*>(trig_id)) : NULL;
	
	grid* new_grid = NULL;
	if(!strcmp(type, "grid"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is a grid!" << std::endl;
#endif
		new_grid = new grid(info, prev, next);
		LD_CHKPTR(new_grid);
	}
	else if(!strcmp(type, "t_grid"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is a t_grid!" << std::endl;
#endif
		char* next2_id = get_attribute(txe, "next2", "no next2 for t_grid: ", name);
		grid* next2 = st->get(next2_id);
		new_grid = new t_grid(info, prev, next, next2);
		LD_CHKPTR(new_grid);
		if(!next2 && strcmp(next2_id, "NONE"))
		{
#ifdef LOAD_DEBUG
			std::cout << "next2 (" << next2_id << ") is null, adding to dep map" << std::endl;
#endif
			add(map, next2_id, (t_grid*)new_grid, &t_grid::set_real_next2);
		}
	}
	else if(!strcmp(type, "escgrid"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is an escgrid!" << std::endl;
#endif
		new_grid = new escgrid(info, prev, next);
		LD_CHKPTR(new_grid);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				if(child->Type() == TiXmlNode::ELEMENT)
					add_esc(child, st, map, escroot, (escgrid*)new_grid);
				else if(child->Type() != TiXmlNode::COMMENT)
					lderr("unknown node in escgrid!");
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "hole"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is an hole!" << std::endl;
#endif
		new_grid = new hole(info, prev, next);
		LD_CHKPTR(new_grid);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				if(child->Type() == TiXmlNode::ELEMENT)
					add_esc(child, st, map, escroot, (escgrid*)new_grid);
				else if(child->Type() != TiXmlNode::COMMENT)
					lderr("unknown node in escgrid!");
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "launcher"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is an launcher!" << std::endl;
#endif
		new_grid = new launcher(info, prev, next);
		LD_CHKPTR(new_grid);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				if(child->Type() == TiXmlNode::ELEMENT)
					add_esc(child, st, map, escroot, (escgrid*)new_grid);
				else if(child->Type() != TiXmlNode::COMMENT)
					lderr("unknown node in escgrid!");
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "stair"))
	{
#ifdef LOAD_DEBUG
		std::cout << name << " is a stair!" << std::endl;
#endif
		TiXmlNode* vec_element = txe->FirstChild();
		while(vec_element != NULL && vec_element->Type() != TiXmlNode::ELEMENT)
			vec_element = vec_element->NextSiblingElement();
		if(!vec_element)
			lderr("cannot find direction element of stair: " , name);
		vector3f* dir_angle = new vector3f();
		LD_CHKPTR(dir_angle);
		get_vec(vec_element->ToElement(), dir_angle);
		vec_element = vec_element->NextSibling();
		while(vec_element != NULL && vec_element->Type() != TiXmlNode::ELEMENT)
			vec_element = vec_element->NextSiblingElement();
		if(!vec_element)
			lderr("cannot find width element of stair: " , name);
		vector3f* width_angle = new vector3f();
		LD_CHKPTR(width_angle);
		get_vec(vec_element->ToElement(), width_angle);
		/*
		TiXmlElement* dir_element = txe->FirstChild()->ToElement();
		vector3f* dir_angle = new vector3f();
		dir_element->QueryFloatAttribute("x", &dir_angle->x);
		dir_element->QueryFloatAttribute("y", &dir_angle->y);
		dir_element->QueryFloatAttribute("z", &dir_angle->z);
		TiXmlElement* width_element = dir_element->NextSiblingElement();
		vector3f* width_angle = new vector3f();
		width_element->QueryFloatAttribute("x", &width_angle->x);
		width_element->QueryFloatAttribute("y", &width_angle->y);
		width_element->QueryFloatAttribute("z", &width_angle->z);
		// */
		new_grid = new stair(info, prev, next, dir_angle, width_angle);
		LD_CHKPTR(new_grid);
	}
	else
		lderr("grid type not known!: ", type);
	if(!prev && strcmp(prev_id, "NONE"))
	{
#ifdef LOAD_DEBUG
		std::cout << "prev (" << prev_id << ") is null, adding to dep map" << std::endl;
#endif
		add(map, prev_id, new_grid, &grid::set_real_prev);
	}
	if(!next && strcmp(next_id, "NONE"))
	{
#ifdef LOAD_DEBUG
		std::cout << "next (" << next_id << ") is null, adding to dep map" << std::endl;
#endif
		add(map, next_id, new_grid, &grid::set_real_next);
	}
	if(trig_id && !trig && strcmp(trig_id, "NONE"))
	{
#ifdef LOAD_DEBUG
		std::cout << "trig (" << trig_id << ") is null, adding to dep map" << std::endl;
#endif
		add(map, const_cast<char*>(trig_id), new_grid, &grid::add_trigger);
	}
	if(txe->Attribute("goal"))
	{
		int is_goal = 0;
		if(txe->QueryIntAttribute("goal", &is_goal) == TIXML_WRONG_TYPE)
			lderr("goal attribute contains wrong type!: ", name);
		if(is_goal)
		{
			new_grid->set_as_goal();
#ifdef LOAD_DEBUG
			std::cout << "it's a goal!" <<std::endl;
#endif
		}
	}
	if(txe->Attribute("nodraw"))
	{
		int nodraw = 0;
		if(txe->QueryIntAttribute("nodraw", &nodraw) == TIXML_WRONG_TYPE)
			lderr("nodraw attribute contains wrong type!: ", name);
		if(nodraw)
		{
			new_grid->set_draw(0);
#ifdef LOAD_DEBUG
			std::cout << "it's invisible!" <<std::endl;
#endif
		}
	}
	FUNCTOR_VEC* deps = dep_set(map, name);
	if(deps)
	{
#ifdef LOAD_DEBUG
		std::cout << "deps found for: " << name << std::endl;
#endif
		FUNCTOR_VEC::iterator it = deps->begin(), end = deps->end();
		while(it != end)
		{
			it->call(new_grid);
#ifdef LOAD_DEBUG
			std::cout << "dep called for " << it->obj << " (" << typeid(*(it->obj)).name() << ") by " << name << std::endl;
#endif
			it++;
		}
		map->erase(name);
	}
#ifdef LOAD_DEBUG
	else
		std::cout << "deps not found for: " << name << std::endl;
#endif
	int noland = 0;
	if(txe->Attribute("noland"))
	{
		if(txe->QueryIntAttribute("noland", &noland) == TIXML_WRONG_TYPE)
			lderr("noland attribute contains wrong type!: ", name);
#ifdef LOAD_DEBUG
		if(noland)
			std::cout << "it's not land-able!" <<std::endl;
#endif
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
	return(new_grid);
}
