#include<iostream>
#include"Component_base.h"
#include"Assemblage.h"
#include"Entity_manager.h"

//----------------------------------------------------------------------------------

Component* Entity_manager::get_component (unsigned int id, ComponentType ct)
{
	auto ite = components.find(id);
	if (ite == components.end()) return nullptr;

	std::vector<Component*> v = (*ite).second;
	for (auto it = v.begin(); it!=v.end(); ++it) {
		if ((*it)->get_component_type() == ct) return *it;
	}
	return nullptr;
}

void Entity_manager::fill_components_type (std::vector<Component*>& v, ComponentType et)
{
	v.erase(v.begin(), v.end());

	for (auto it1=components.begin(); it1!=components.end(); ++it1) {
		for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2) {
			if ((*it2)->get_component_type() == et) v.push_back(*it2);
		}
	}
}

void Entity_manager::fill_entities_componentType (std::vector<unsigned int>& v, unsigned long et)
{
	v.erase(v.begin(), v.end());

	for (auto it=entities.begin(); it!=entities.end(); ++it) {
		if ((it->second & et) == et) {
			v.push_back(it->first);
		}
	}
}

void Entity_manager::add_component (unsigned int id, Component* pc)
{
	auto it1 = entities.find(id);
	if (it1==entities.end()) return;
	it1->second = it1->second | pc->get_component_type();

	auto it2 = components.find(id);
	it2->second.push_back(pc);
}

int Entity_manager::create_entity()
{
	entities.emplace(nextEntity, 0);
	components.emplace(nextEntity, std::vector<Component*>());
	nextEntity++;
	return nextEntity-1;
}

/* PUT IN THE HEADER FILE
template<class T>
int Entity_manager::create_entity(T et)
{
	entities.emplace(nextEntity, 0);
	components.emplace(nextEntity, std::vector<Component*>());
	et(nextEntity, this);
	nextEntity++;
	return nextEntity-1;
}
*/

void Entity_manager::kill_entity (unsigned int id)
{
	entities.erase(id);
	components.erase(id);
}


/*
int main()
{
	Entity_manager world;

	unsigned int wall = world.create_entity(Walll());

	world.create_entity(Pit());

	world.create_entity(Wumpus());


	Component* pc = world.get_component(wall, C_Position);
	char x = !pc ? '0' : '1';
	std::cout << x << '\n';
	if (pc) {
		Position* pp = dynamic_cast<Position*>(pc);
		if (pp) {
			pp->x = 10;
			std::cout << pp->x << '\n';
		}
	}


	std::vector<Component*> vpc;
	world.fill_components_type(vpc, C_Deal_Damage);
	for (const auto& e : vpc) {
		std::cout << e->get_component_type() << ' ';
	}
	std::cout << '\n';


	std::vector<unsigned int> ve;
	world.fill_entities_componentType(ve, C_Moveable);
	for (const auto& e : ve) {
		std::cout << e << ' ';
	}
	std::cout << '\n';

	world.kill_entity (2);
	std::vector<Component*> vpc2;
	world.fill_components_type(vpc2, C_Deal_Damage);
	for (const auto& e : vpc2) {
		std::cout << e->get_component_type() << ' ';
	}
	std::cout << '\n';

}


*/
