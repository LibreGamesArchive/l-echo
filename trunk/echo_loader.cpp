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

#include <launcher.h>
#include <freeform_grid.h>
#include <t_grid.h>
#include <escgrid.h>
#include <hole.h>
#include <grid.h>
#include <stair.h>

#ifdef ARM9
	#include <tinyxml.h>
#else
	#include <tinyxml/tinyxml.h>
#endif

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
		LD_PRINT("start: %s\n", start);
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

static void get_float(TiXmlElement* element, const char* attr, float* save_to)
{
	int result = element->QueryFloatAttribute(attr, save_to);
	if(result == TIXML_NO_ATTRIBUTE)
		lderr("cannot find float attribute: ", attr);
	else if(result == TIXML_WRONG_TYPE)
		lderr("attribute should be float: ", attr);
}

static void get_vec(TiXmlElement* txe, vector3f* vec, stage* st = NULL)
{
	get_float(txe, "x", &vec->x);
	get_float(txe, "y", &vec->y);
	get_float(txe, "z", &vec->z);
	if(st)
		st->set_farthest(vec->length());
}

static void get_angle(TiXmlElement* txe, vector3f* vec)
{
	get_float(txe, "x", &vec->x);
	get_float(txe, "y", &vec->y);
}

static void add_esc(TiXmlElement* child, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid)
{
	const char* type = child->Value();
	if(!strcmp(type, "angle"))
	{
		vector3f* each_angle = new vector3f();
		LD_CHKPTR(each_angle);
		get_angle(child, each_angle);
		grid->add(each_angle
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
	else if(!strcmp(type, "range"))
	{
		vector3f* v1 = new vector3f();
		LD_CHKPTR(v1);
		get_float(child, "x_min", &v1->x);
		get_float(child, "y_min", &v1->y);
		vector3f* v2 = new vector3f();
		LD_CHKPTR(v2);
		get_float(child, "x_max", &v2->x);
		get_float(child, "y_max", &v2->y);
		grid->add(new angle_range(v1, v2)
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
	else
		lderr("child of escgrid, hole or launcher is not an angle or range!");
}

static void add_escs(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid)
{
	if(txe->FirstChild())
	{
		TiXmlNode* child = txe->FirstChild();
		while(child)
		{
			if(child->Type() == TiXmlNode::ELEMENT && strcmp(child->Value(), "triggers"))
				add_esc(child->ToElement(), st, map, escroot, grid);
			else if(child->Type() != TiXmlNode::COMMENT)
				lderr("unknown node in escgrid!");
			child = child->NextSibling();
		}
	}
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

static filter* get_filter(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, const char* type = NULL)
{
	if(!type)
	{
		type = txe->Value();
		if(!type)
			lderr("type unknown for filter...");
	}
	if(!strcmp(type, "goal"))
	{
		filter* ret = new filter();
		LD_CHKPTR(ret);
		char* name = get_attribute(txe, "id", "no id for goal filter");
		grid* g = st->get(name);
		if(g)
			ret->set_target(g);
		else
		{
			LD_PRINT("filter target (%s) is null, adding to dep map\n", name);
			add(map, name, ret, &filter::set_target);
		}
		return(ret);
	}
	else if(!strcmp(type, "not"))
	{
		if(txe->FirstChild())
		{
			TiXmlNode* child = txe->FirstChild();
			while(child != NULL && child->Type() != TiXmlNode::ELEMENT)
				child = child->NextSibling();
			if(child == NULL)
				lderr("no filter element in \"not\" filter!");
			return(new not_filter(get_filter(child->ToElement(), st, map)));
		}
		else
			lderr("\"not\" filter has no item inside");
	}
	else if(!strcmp(type, "or"))
	{
		or_filter* ret = new or_filter();
		if(txe->FirstChild())
		{
			TiXmlNode* child = txe->FirstChild();
			while(child)
			{
				if(child->Type() == TiXmlNode::ELEMENT)
					ret->add_filter(get_filter(child->ToElement(), st, map));
				else if(child->Type() != TiXmlNode::COMMENT)
					lderr("unknown node in \"or\" filter!");
				child = child->NextSibling();
			}
		}
		else
			lderr("\"or\" filter has no item inside");
		return(ret);
	}
	else if(!strcmp(type, "and"))
	{
		and_filter* ret = new and_filter();
		if(txe->FirstChild())
		{
			TiXmlNode* child = txe->FirstChild();
			while(child)
			{
				if(child->Type() == TiXmlNode::ELEMENT)
					ret->add_filter(get_filter(child->ToElement(), st, map));
				else if(child->Type() != TiXmlNode::COMMENT)
					lderr("unknown node in \"and\" filter!");
				child = child->NextSibling();
			}
		}
		else
			lderr("\"and\" filter has no item inside");
		return(ret);
	}
	else
		lderr("filter type unknown");
	return(NULL);
}

static trigger* get_trigger(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map)
{
	filter* f = NULL;
	if(txe->FirstChild())
		f = get_filter(txe, st, map, "and");
	char* name = get_attribute(txe, "id", "no id for trigger");
	grid* g = st->get(name);
	if(g)
		return(new trigger(f, g));
	else
	{
		trigger* ret = new trigger(f);
		LD_PRINT("trigger target (%s) is null, adding to dep map\n", name);
		add(map, name, ret, &trigger::set_target);
		return(ret);
	}
}

static void add_triggers(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, grid* g)
{
	if(txe->FirstChild())
	{
		TiXmlNode* child = txe->FirstChild();
		while(child)
		{
			if(child->Type() == TiXmlNode::ELEMENT)
				g->add_trigger(get_trigger(child->ToElement(), st, map));
			else if(child->Type() != TiXmlNode::COMMENT)
				lderr("unknown node in triggers!");
			child = child->NextSibling();
		}
	}
	else
		lderr("triggers element has no triggers!");
}

static grid* parse_grid(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot)
{
	LD_PRINT("\n");
	char* name = get_attribute(txe, "id", "unnamed grid!");
	const char* type = txe->Value();
	if(!type)
		lderr("type not known for grid: " , name);
	grid_info_t* info = new(grid_info_t);
	LD_CHKPTR(info);
	get_vec(txe, &(info->pos), st);
	char* prev_id = get_attribute(txe, "prev", "no previous for grid: ", name);
	grid* prev = st->get(prev_id);
	char* next_id = get_attribute(txe, "next", "no next for grid: ", name);
	grid* next = st->get(next_id);
	
	grid* new_grid = NULL;
	if(!strcmp(type, "grid"))
	{
		LD_PRINT("%s is a grid!\n", name);
		new_grid = new grid(info, prev, next);
		LD_CHKPTR(new_grid);
	}
	else if(!strcmp(type, "t_grid"))
	{
		LD_PRINT("%s is a t_grid!\n", name);
		char* next2_id = get_attribute(txe, "next2", "no next2 for t_grid: ", name);
		grid* next2 = st->get(next2_id);
		new_grid = new t_grid(info, prev, next, next2);
		LD_CHKPTR(new_grid);
		if(!next2 && strcmp(next2_id, "NONE"))
		{
			LD_PRINT("next2 (%s) is null, adding to dep map\n", next2_id);
			add(map, next2_id, (t_grid*)new_grid, &t_grid::set_real_next2);
		}
	}
	else if(!strcmp(type, "escgrid"))
	{
		LD_PRINT("%s is a escgrid!\n", name);
		new_grid = new escgrid(info, prev, next);
		LD_CHKPTR(new_grid);
		add_escs(txe, st, map, escroot, (escgrid*)new_grid);
	}
	else if(!strcmp(type, "hole"))
	{
		LD_PRINT("%s is an hole!\n", name);
		new_grid = new hole(info, prev, next);
		LD_CHKPTR(new_grid);
		add_escs(txe, st, map, escroot, (escgrid*)new_grid);
	}
	else if(!strcmp(type, "launcher"))
	{
		LD_PRINT("%s is a launcher!\n", name);
		new_grid = new launcher(info, prev, next);
		LD_CHKPTR(new_grid);
		add_escs(txe, st, map, escroot, (escgrid*)new_grid);
	}
	else if(!strcmp(type, "freeform_grid"))
	{
		LD_PRINT("%s is a freeform_grid!\n", name);
		TiXmlNode* vec_element = txe->FirstChild();
		while(vec_element != NULL && vec_element->Type() != TiXmlNode::ELEMENT)
			vec_element = vec_element->NextSiblingElement();
		if(!vec_element)
			lderr("cannot find direction element of freeform_grid: " , name);
		vector3f* dir_angle = new vector3f();
		LD_CHKPTR(dir_angle);
		get_vec(vec_element->ToElement(), dir_angle);
		vec_element = vec_element->NextSibling();
		while(vec_element != NULL && vec_element->Type() != TiXmlNode::ELEMENT)
			vec_element = vec_element->NextSiblingElement();
		if(!vec_element)
			lderr("cannot find width element of freeform_grid: " , name);
		vector3f* width_angle = new vector3f();
		LD_CHKPTR(width_angle);
		get_vec(vec_element->ToElement(), width_angle);
		new_grid = new freeform_grid(info, prev, next, dir_angle, width_angle);
		LD_CHKPTR(new_grid);
	}
	else if(!strcmp(type, "stair"))
	{
		float angle = 45;
		get_float(txe, "direction", &angle);
		ECHO_PRINT("angle: %f\n", angle);
		new_grid = new stair(info, prev, next, angle);
		LD_CHKPTR(new_grid);
	}
	else
		lderr("grid type not known!: ", type);
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
	if(txe->Attribute("goal"))
	{
		int is_goal = 0;
		if(txe->QueryIntAttribute("goal", &is_goal) == TIXML_WRONG_TYPE)
			lderr("goal attribute contains wrong type!: ", name);
		if(is_goal)
		{
			new_grid->set_as_goal();
			LD_PRINT("it's a goal!\n");
		}
	}
	//nodraw check
	if(txe->Attribute("nodraw"))
	{
		int nodraw = 0;
		if(txe->QueryIntAttribute("nodraw", &nodraw) == TIXML_WRONG_TYPE)
			lderr("nodraw attribute contains wrong type!: ", name);
		if(nodraw)
		{
			new_grid->set_draw(0);
			LD_PRINT("it's invisible!\n");
		}
	}
	//noland check
	int noland = 0;
	if(txe->Attribute("noland"))
	{
		if(txe->QueryIntAttribute("noland", &noland) == TIXML_WRONG_TYPE)
			lderr("noland attribute contains wrong type!: ", name);
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
	TiXmlNode* child = txe->FirstChild();
	while(child != NULL && child->Type() != TiXmlNode::ELEMENT)
		child = child->NextSibling();
	if(child != NULL && !strcmp(child->Value(), "triggers"))
	{
		LD_PRINT("adding triggers\n");
		add_triggers(child->ToElement(), st, map, new_grid);
	}
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
		map->erase(name);
	}
	else
		LD_PRINT("deps not found for: %s\n", name);
	return(new_grid);
}
