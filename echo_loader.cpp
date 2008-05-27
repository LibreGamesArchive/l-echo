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

#include <launcher.h>
#include <stair.h>
#include <t_grid.h>
#include <escgrid.h>
#include <hole.h>
#include <grid.h>
#include <echo_stage.h>
#include <tinyxml/tinyxml.h>

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
	TiXmlDocument doc(file_name);
	if(doc.LoadFile())
	{
		DEPENDENCY_MAP* map = new DEPENDENCY_MAP();
		stage* ret = new stage();
		TiXmlElement* root = doc.RootElement();
		TiXmlNode* child = NULL;
		while(child = root->IterateChildren(child))
		{
			parse_grid(child->ToElement(), ret, map, NULL);
		}
		ret->set_start(ret->get(root->Attribute("start")));
		ret->set_name(*(new std::string(root->Attribute("name"))));
		int num_goals = 0;
		root->QueryIntAttribute("goals", &num_goals);
		ret->set_num_goals(num_goals);
		
		std::cout << "is map empty?: " << map->empty() << std::endl;
		ret->dump_levels();
		return(ret);
	}
	return(NULL);
}

static FUNCTOR_VEC* dep_set(DEPENDENCY_MAP* map, char* id)
{
	std::string s1(id);
	DEPENDENCY_MAP::iterator it = map->find(s1);
	if(it == map->end())
	{
		return(NULL);
	}
	return(it->second);
}

static void add(DEPENDENCY_MAP* map, char* id, functor f)
{
	FUNCTOR_VEC* set = dep_set(map, id);
	if(set)
	{
		std::cout << "dep set for " << id << " found, adding"<< std::endl;
		set->push_back(f);
	}
	else
	{
		std::cout << "dep set for " << id << " NOT found, adding"<< std::endl;
		set = new FUNCTOR_VEC();
		set->push_back(f);
		map->insert(DEPENDENCY_MAP::value_type(id, set));
	}
}

static void add(DEPENDENCY_MAP* map, char* id, t_grid* obj, void (t_grid::*funcp)(grid*))
{
	t_functor f(obj, funcp);
	add(map, id, f);
}

static void add(DEPENDENCY_MAP* map, char* id, grid* obj, void (grid::*funcp)(grid*))
{
	functor f(obj, funcp);
	add(map, id, f);
}

static void add_esc(TiXmlElement* child, stage* st, DEPENDENCY_MAP* map, escgrid* escroot, escgrid* grid)
{
	const char* type = child->Value();
	if(!strcmp(type, "angle"))
	{
		vector3f* each_angle = new vector3f();
		child->QueryFloatAttribute("x", &each_angle->x);
		child->QueryFloatAttribute("y", &each_angle->y);
		child->QueryFloatAttribute("z", &each_angle->z);
		grid->add(each_angle
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
	else if(!strcmp(type, "range"))
	{
		vector3f* v1 = new vector3f();
		child->QueryFloatAttribute("x_min", &v1->x);
		child->QueryFloatAttribute("y_min", &v1->y);
		child->QueryFloatAttribute("z_min", &v1->z);
		vector3f* v2 = new vector3f();
		child->QueryFloatAttribute("x_max", &v2->x);
		child->QueryFloatAttribute("y_max", &v2->y);
		child->QueryFloatAttribute("z_max", &v2->z);
		grid->add(new angle_range(v1, v2)
			, parse_grid(child->FirstChild()->ToElement(), st, map, escroot ? escroot : grid));
	}
}

static grid* parse_grid(TiXmlElement* txe, stage* st, DEPENDENCY_MAP* map, escgrid* escroot)
{
	std::cout << std::endl;
	const char* type = txe->Value();
	//*
	grid_info_t* info = new(grid_info_t);
	txe->QueryFloatAttribute("x", &info->pos.x);
	txe->QueryFloatAttribute("y", &info->pos.y);
	txe->QueryFloatAttribute("z", &info->pos.z);
	
	char* name = const_cast<char*>(txe->Attribute("id"));
	char* prev_id = const_cast<char*>(txe->Attribute("prev"));
	grid* prev = st->get(prev_id);
	char* next_id = const_cast<char*>(txe->Attribute("next"));
	grid* next = st->get(next_id);
	
	grid* new_grid = NULL;
	// */
	if(!strcmp(type, "grid"))
	{
		std::cout << name << " is a grid!" << std::endl;
		new_grid = new grid(info, prev, next);
	}
	else if(!strcmp(type, "t_grid"))
	{
		std::cout << name << " is a t_grid!" << std::endl;
		char* next2_id = const_cast<char*>(txe->Attribute("next2"));
		grid* next2 = st->get(next2_id);
		new_grid = new t_grid(info, prev, next, next2);
		if(!next2 && strcmp(next2_id, "NONE"))
		{
			std::cout << "next2 (" << next2_id << ") is null, adding to dep map" << std::endl;
			add(map, next2_id, (t_grid*)new_grid, &t_grid::set_real_next2);
		}
	}
	else if(!strcmp(type, "escgrid"))
	{
		std::cout << name << " is an escgrid!" << std::endl;
		new_grid = new escgrid(info, prev, next);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				add_esc(child, st, map, escroot, (escgrid*)new_grid);
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "hole"))
	{
		std::cout << name << " is an hole!" << std::endl;
		new_grid = new hole(info, prev, next);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				add_esc(child, st, map, escroot, (escgrid*)new_grid);
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "launcher"))
	{
		std::cout << name << " is an launcher!" << std::endl;
		new_grid = new launcher(info, prev, next);
		if(txe->FirstChild())
		{
			TiXmlElement* child = txe->FirstChild()->ToElement();
			while(child)
			{
				add_esc(child, st, map, escroot, (escgrid*)new_grid);
				child = child->NextSiblingElement();
			}
		}
	}
	else if(!strcmp(type, "stair"))
	{
		std::cout << name << " is a stair!" << std::endl;
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
		new_grid = new stair(info, prev, next, dir_angle, width_angle);
	}
	if(!prev && strcmp(prev_id, "NONE"))
	{
		std::cout << "prev (" << prev_id << ") is null, adding to dep map" << std::endl;
		add(map, prev_id, new_grid, &grid::set_real_prev);
	}
	if(!next && strcmp(next_id, "NONE"))
	{
		std::cout << "next (" << next_id << ") is null, adding to dep map" << std::endl;
		add(map, next_id, new_grid, &grid::set_real_next);
	}
	if(txe->Attribute("goal"))
	{
		int is_goal = 0;
		txe->QueryIntAttribute("goal", &is_goal);
		if(is_goal)
		{
			new_grid->set_as_goal();
			std::cout << "it's a goal!" <<std::endl;
		}
	}
	if(txe->Attribute("nodraw"))
	{
		int nodraw = 0;
		txe->QueryIntAttribute("nodraw", &nodraw);
		if(nodraw)
		{
			new_grid->set_draw(0);
			std::cout << "it's invisible!" <<std::endl;
		}
	}
	FUNCTOR_VEC* deps = dep_set(map, name);
	if(deps)
	{
		std::cout << "deps found for: " << name << std::endl;
		FUNCTOR_VEC::iterator it = deps->begin(), end = deps->end();
		while(it != end)
		{
			it->call(new_grid);
			std::cout << "dep called for " << it->obj << " (" << typeid(*(it->obj)).name() << ") by " << name << std::endl;
			it++;
		}
		map->erase(name);
	}
	else
		std::cout << "deps not found for: " << name << std::endl;
	int noland = 0;
	if(txe->Attribute("noland"))
	{
		txe->QueryIntAttribute("noland", &noland);
		if(noland)
			std::cout << "it's not land-able!" <<std::endl;
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
