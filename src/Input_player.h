#ifndef INPUT_PLAYER_H
#define INPUT_PLAYER_H

#include<iostream>
#include"Action.h"

class Input_player
{
public:
	Input_player() : next_action(nullptr) { }
	~Input_player() { delete next_action; }

	Action* get_next_action() const { return next_action; }

	Action* next_action;
};

std::istream& operator>> (std::istream& is, Input_player& rhs);



#endif
