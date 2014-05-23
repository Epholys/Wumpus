#ifndef COMPONENT_ALL_H
#define COMPONENT_ALL_H

#include"Component_base.h"
#include"Input_player.h"
#include"Action.h"

struct Position : Component
{
	unsigned int x;
	unsigned int y;

	Position(unsigned int xx=0, unsigned int yy=0) : x(xx), y(yy) { }

	ComponentType get_component_type () const { return C_Position; }
};

struct Moveable : Component
{
	int dx;
	int dy;

	Moveable(int ddx=0, int ddy=0) : dx(ddx), dy(ddy) { }

	ComponentType get_component_type () const { return C_Moveable; }
};

struct Wall : Component
{
	ComponentType get_component_type () const { return C_Wall; }
};

struct Damageable : Component
{
	int life;
	bool fly;

	Damageable(int l=1, bool f=false) : life(l), fly(f) { }

	ComponentType get_component_type () const { return C_Damageable; }
};

struct Deal_Damage : Component
{
	unsigned int damage;
	bool ground;
	bool noisy;
	int duration;

	Deal_Damage(unsigned int d=1, bool g=false, bool n=false, int dur=-1)
		:damage(d), ground(g), noisy(n), duration(dur) { }

	ComponentType get_component_type () const { return C_Deal_Damage; }
};

struct IA : Component
{
	bool awake;
	bool sensible;

	IA(bool a=true, bool s=false) : awake(a), sensible(s) { }

	ComponentType get_component_type () const { return C_IA; }
};

struct Controllable : Component
{
	Input_player* pInputPlayer;
	Action* pNextAction;

	int nArrow;

	Controllable(Input_player* pip, Action* pa) : pInputPlayer(pip), pNextAction(pa), nArrow(10) { }
	~Controllable() { }

	ComponentType get_component_type () const { return C_Controllable; }
};

struct Displayable : Component
{
	char repres;

	Displayable(char c='X') : repres(c) { }

	ComponentType get_component_type () const { return C_Displayable; }
};

struct Faction : Component
{
	int faction;

	Faction(int f=0) : faction(f) { }

	ComponentType get_component_type () const { return C_Faction; }
};

struct Vital : Component
{
	bool player;

	Vital(bool p=false) : player(p)  { }

	ComponentType get_component_type () const { return C_Vital; }
};

struct Numbered : Component
{
	int number;

	Numbered(int n=0) : number(n) { }

	ComponentType get_component_type () const { return C_Numbered; }
};

struct Trap : Component
{
	enum TrapType { Teleport=0 };
	TrapType type;

	Trap(TrapType ts=Teleport) : type(ts) {	}

	ComponentType get_component_type () const { return C_Trap; }
};

#endif
