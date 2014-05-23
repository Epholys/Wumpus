#include<iostream>
#include<limits>
#include"Input_player.h"
#include"Action_look.h"
#include"Action_move.h"
#include"Action_shoot.h"


std::istream& operator>> (std::istream& is, Input_player& rhs)
{
	char ch1 = ' ';
	char ch2 = ' ';

	if (is >> ch1) {
		switch (ch1)
		{
		case 'w':
			rhs.next_action = new Action();
			break;
		case 'l':
			rhs.next_action = new Action_look();
			break;
		case 'm':
		{	if (is >> ch2) {
				switch (ch2)
				{
				case'n':
					rhs.next_action = new Action_move(0,-1);
					break;
				case'e':
					rhs.next_action = new Action_move(1,0);
					break;
				case'w':
					rhs.next_action = new Action_move(-1,0);
					break;
				case's':
					rhs.next_action = new Action_move(0,1);
					break;
				default:
					is.putback(ch2);
					is.clear(std::ios::failbit);
					return is;
					break;

				}
			}
			break;
		}
		case 's':
		{
			if (is >> ch2) {
				switch (ch2)
				{
				case'n':
					rhs.next_action = new Action_shoot(0,-1);
					break;
				case'e':
					rhs.next_action = new Action_shoot(1,0);
					break;
				case'w':
					rhs.next_action = new Action_shoot(-1,0);
					break;
				case's':
					rhs.next_action = new Action_shoot(0,1);
					break;
				default:
					is.putback(ch2);
					is.clear(std::ios::failbit);
					return is;
					break;

				}
			}
			break;
		}
		default:
			is.clear(std::ios::failbit);
			return is;
			break;
		}
	}
	is.clear();
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return is;
}

