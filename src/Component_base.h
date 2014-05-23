#ifndef COMPONENT_H
#define COMPONENT_H



enum ComponentType : unsigned long {
	C_Null = 0,
	C_Position = 1,
	C_Moveable = 1 << 1,
	C_Wall = 1 << 2,
	C_Damageable = 1 << 3,
	C_Deal_Damage = 1 << 4,
	C_IA = 1 << 5,
	C_Controllable = 1 << 6,
	C_Displayable = 1 << 7,
	C_Faction = 1 << 8,
	C_Vital = 1 << 9,
	C_Numbered = 1 << 10,
	C_Trap = 1 << 11
};


struct Component
{
	virtual ~Component() { }

	virtual ComponentType get_component_type() const =0;
};


#endif
