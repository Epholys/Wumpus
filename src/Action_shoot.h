#ifndef ACTION_SHOOT_H
#define ACTION_SHOOT_H

class Action_shoot : public Action
{
public:
	Action_shoot(int xx, int yy) : Action(Look), dx(xx), dy(yy) { }

	int dx;
	int dy;
};


#endif
