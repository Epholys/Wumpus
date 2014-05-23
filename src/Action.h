#ifndef ACTION_H
#define ACTION_H

class Action
{
public:
	enum Action_type { Wait, Look, Shoot, Move };

	Action(Action_type at = Wait) : type(at) { }
	virtual ~Action() { }

	Action_type get_Action_type() { return type; }

protected:
	Action_type type;
};


#endif
