#ifndef ASSEMBLAGE_H
#define ASSEMBLAGE_H

#include"Entity_manager.h"

struct Walll
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Wall());
		em->add_component(i, new Displayable('X'));
	}
};
struct Pit
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Deal_Damage(1, true, false, -1));
		em->add_component(i, new Faction(0));
		em->add_component(i, new Displayable('O'));
	}
};
struct Bat
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Moveable());
		em->add_component(i, new Damageable(1,true));
//		em->add_component(i, new Deal_Damage(0, false, false, -1));
		em->add_component(i, new Trap(Trap::Teleport));
		em->add_component(i, new Faction(1));
		em->add_component(i, new IA(true, false));
		em->add_component(i, new Displayable('B'));
	}
};
struct Wumpus
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Moveable());
		em->add_component(i, new Damageable(1,true));
		em->add_component(i, new Deal_Damage(1,false,false,-1));
		em->add_component(i, new Faction(1));
		em->add_component(i, new Vital(false));
		em->add_component(i, new IA(false, true));
		em->add_component(i, new Displayable('W'));
	}
};

struct Player
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Moveable());
		em->add_component(i, new Damageable(1,false));
		em->add_component(i, new Faction(2));
		em->add_component(i, new Controllable(new Input_player(), new Action()));
		em->add_component(i, new Vital(true));
		em->add_component(i, new Displayable('@'));
	}
};

struct Arrow
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Deal_Damage(1,false,true,1));
		em->add_component(i, new Faction(2));
	}
};

struct Room
{
	void operator() (unsigned int i, Entity_manager* em)
	{
		em->add_component(i, new Position());
		em->add_component(i, new Numbered());
	}
};


#endif
