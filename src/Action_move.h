#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

class Action_move : public Action
{
public:
	Action_move(int xx, int yy) : Action(Move), dx(xx), dy(yy) { }

	int dx;
	int dy;
};


#endif
