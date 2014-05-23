#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include"Entity_base.h"
#include"Component_base.h"
#include"Component_all.h"
#include<stdexcept>
#include<vector>
#include<map>


struct Entity_manager
{
private:
	unsigned int nextEntity;

	std::map<unsigned int, unsigned long> entities;
	std::map<unsigned int, std::vector<Component*>> components;

public:
	Component* get_component (unsigned int, ComponentType);
	template<class T> T get_component(unsigned int e, ComponentType ct)
	{
		Component* comp = get_component(e, ct);
		if (!comp)
		{
			throw std::runtime_error("Entity_manager::get_component<>(): can't find T of "+e);
		}
		T component = dynamic_cast<T>(comp);
		if (!component)
		{
			throw std::runtime_error("Entity_manager()::get_component<>(): can't cast Component* into T");
		}
		return component;
	}

	void fill_components_type (std::vector<Component*>&, ComponentType);
	void fill_entities_componentType (std::vector<unsigned int>&, unsigned long);
	template<class T>
		void fill_components_of_entities (std::vector<T>& vT, const std::vector<unsigned int>& vE, ComponentType ct)
	{
		for (auto e : vE)
		{
			Component* comp = get_component(e, ct);
			if (!comp)
			{
				throw std::runtime_error("Entity_manager::fill_component_of_entities<>(): can't find T of "+e);
			}
			T component = dynamic_cast<T>(comp);
			if (!component)
			{
				throw std::runtime_error("Entity_manager::fill_component_of_entities<>(): can't cast Component* into T");
			}
			vT.push_back(component);
		}
	}

	void add_component (unsigned int, Component*);

	template<class T> int create_entity (T et)
	{
		entities.emplace(nextEntity, 0);
		components.emplace(nextEntity, std::vector<Component*>());
		et(nextEntity, this);
		nextEntity++;
		return nextEntity-1;
	}
	int create_entity();

	void kill_entity (unsigned int);


	Entity_manager() : nextEntity(1) { }
	~Entity_manager()
	{
		for (auto& e : components)
			for (auto& ee : e.second)
				delete ee;
	}

	const int SIZE_MAP = 8;
	const int N_ROOM = 15;
	const int PROB_PIT = 5;
	const int N_BAT = 1;
	const int N_WUMPUS = 1;
};



#endif
