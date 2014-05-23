#include<iostream>
#include<vector>
#include<stdexcept>
#include<string>
#include"System_all.h"
#include"System_helper.h"
#include"Action.h"
#include"Action_look.h"
#include"Action_move.h"
#include"Action_shoot.h"
#include"Assemblage.h"

using std::vector;
using std::runtime_error;

void S_Player::process (long previousFrameTime)
{
	//retrieve all the entites with C_Controllable and others
	vector<unsigned int> entitiesControl;
	em->fill_entities_componentType(entitiesControl, C_Controllable | C_Position | C_Moveable | C_Vital);

	vector<Controllable*> vControlCon;
	em->fill_components_of_entities<Controllable*> (vControlCon, entitiesControl, C_Controllable);

	vector<Position*> vControlPos;
	em->fill_components_of_entities<Position*> (vControlPos, entitiesControl, C_Position);

	vector<Moveable*> vControlMov;
	em->fill_components_of_entities<Moveable*> (vControlMov, entitiesControl, C_Moveable);

	vector<Vital*> vControlVit;
	em->fill_components_of_entities<Vital*> (vControlVit, entitiesControl, C_Vital);


	// display the texts
	display_pre_feedback(vControlPos);


	// get the next action via std::cin
	for(size_t i=0; i<vControlCon.size(); ++i)
	{
		get_input(vControlCon[i]);
	}


	// execute the action
	for (size_t i=0; i<vControlCon.size(); ++i)
	{
		Action* pCom = vControlCon[i]->pNextAction;
		if (dynamic_cast<Action_look*>(pCom))
		{ /*do nothing*/ }
		else if (dynamic_cast<Action_move*>(pCom))
		{
			Action_move* pActMov = dynamic_cast<Action_move*>(pCom);
			vControlMov[i]->dx = pActMov->dx;
			vControlMov[i]->dy = pActMov->dy;
		}
		else if (dynamic_cast<Action_shoot*>(pCom))
		{
			Action_shoot* pActSho = dynamic_cast<Action_shoot*>(pCom);

			if (vControlCon[i]->nArrow < 1)
			{
				std::cout << "You can't shoot. You don't have any arrows.\n";
				std::cout << "Without any arrows, you can't survive in the cave...\n";
				if (vControlVit[i]->player)
				{
					em->kill_entity(entitiesControl[i]);
				}
			}
			else
			{
				--(vControlCon[i]->nArrow);

				for (int j=1; j<4; ++j)
				{
					unsigned int arrow = em->create_entity(Arrow());
					Position* position = em->get_component<Position*>(arrow, C_Position);

					// for the warp-around
					int arrowX = vControlPos[i]->x + j * pActSho->dx;
					int arrowXBelowMin = 0 - arrowX;
					int arrowXAboveMax = arrowX - (em->SIZE_MAP-1);
					arrowX = (arrowXBelowMin>0) ? (em->SIZE_MAP - arrowXBelowMin) : arrowX;
					arrowX = (arrowXAboveMax>0) ? arrowXAboveMax - 1 : arrowX;

					int arrowY = vControlPos[i]->y + j * pActSho->dy;
					int arrowYBelowMin = 0 - arrowY;
					int arrowYAboveMax = arrowY - (em->SIZE_MAP-1);
					arrowY = (arrowYBelowMin>0) ? (em->SIZE_MAP - arrowYBelowMin) : arrowY;
					arrowY = (arrowYAboveMax>0) ? arrowYAboveMax - 1 : arrowY;

					if (!collision_with_wall(em, arrowX, arrowY))
					{
		                position->x = arrowX;
		                position->y = arrowY;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void S_Player::get_input (Controllable* con)
{
	std::cout << "> ";
	while (!(std::cin >> *(con->pInputPlayer))) {
		std::cin.clear();
		std::cout << "> ";
	}
	std::cout << '\n';
	con->pNextAction = con->pInputPlayer->get_next_action();
}

void S_Player::display_pre_feedback(const vector<Position*>& vControlPos)
{
	// retrieve some datas about the rooms
	vector<unsigned int> entitiesRoom;
	em->fill_entities_componentType(entitiesRoom, C_Numbered | C_Position);
	vector<Position*> vRoomPos;
	em->fill_components_of_entities<Position*>(vRoomPos, entitiesRoom, C_Position);

	vector<Numbered*> vRoomNum;
	em->fill_components_of_entities<Numbered*>(vRoomNum, entitiesRoom, C_Numbered);

	// write the current position
	for(size_t i=0; i<vControlPos.size(); ++i)
	{
		for(size_t j=0; j<vRoomPos.size(); ++j)
		{
			if ((vControlPos[i]->x == vRoomPos[j]->x)
			&& (vControlPos[i]->y == vRoomPos[j]->y))
			{
				std::cout << "You are in room " << vRoomNum[j]->number << ".\n";
			}
		}
	}

	// write all about the corridors
	for (size_t i=0; i<vControlPos.size(); ++i)
	{

		int nCorridor = 0;
		bool north=false, south=false, east=false, west=false;
		int nNorth=0, nSouth=0, nEast=0, nWest=0;

		for (size_t j=0; j<vRoomPos.size(); ++j)
		{
			int dx = vControlPos[i]->x - vRoomPos[j]->x;
			int dy = vControlPos[i]->y - vRoomPos[j]->y;

			// for the "warp around":
			dx = dx==(em->SIZE_MAP-1) ? -1 : dx;
			dx = dx==(-(em->SIZE_MAP-1)) ? 1 : dx;
			dy = dy==(em->SIZE_MAP-1) ? -1 : dy;
			dy = dy==(-(em->SIZE_MAP-1)) ? 1 : dy;

			if (dx == 0 && dy == 1)
			{
				north = true;
				nNorth = vRoomNum[j]->number;
				++nCorridor;
			}
			else if (dx == 0 && dy == -1)
			{
				south = true;
				nSouth = vRoomNum[j]->number;
				++nCorridor;
			}
			else if (dx == -1 && dy == 0)
			{
				east=true;
				nEast = vRoomNum[j]->number;
				++nCorridor;
			}
			else if (dx == 1 && dy == 0)
			{
				west = true;
				nWest = vRoomNum[j]->number;
				++nCorridor;
			}
		}

		if (!nCorridor)
		{
			std::cout << "There isn't any corridors. You're stuck. That's weird, that souldn't happen. Please contact the author.\n";
		}
		else
		{
			std::string be = (nCorridor==1) ? "is" : "are";
			std::string plural = (nCorridor==1) ? "" : "s";
			std::cout << "There " << be << ' ' << nCorridor << " corridors:\n";
			if (north)
			{
				std::cout << "  -One to the north and room " << nNorth << ".\n";
			}
			if (east)
			{
				std::cout << "  -One to the east and room " << nEast << ".\n";
			}
			if (south)
			{
				std::cout << "  -One to the south and room " << nSouth << ".\n";
			}
			if (west)
			{
				std::cout << "  -One to the west and room " << nWest << ".\n";
			}
		}

	}


	// retrieve some datas about the neigboors
	vector<unsigned int> entitiesDisplay;
	em->fill_entities_componentType(entitiesDisplay, C_Displayable | C_Position);

	vector<Position*> vDisplayPos;
	em->fill_components_of_entities<Position*>(vDisplayPos, entitiesDisplay, C_Position);

	vector<Displayable*> vDisplayDis;
	em->fill_components_of_entities<Displayable*>(vDisplayDis, entitiesDisplay, C_Displayable);

	for (size_t i=0; i<vControlPos.size(); ++i) {

		int nBats=0;
		std::string plural;

		for (size_t j=0; j<vDisplayPos.size(); ++j)
		{

			int dx = vControlPos[i]->x - vDisplayPos[j]->x;
			int dy = vControlPos[i]->y - vDisplayPos[j]->y;

			// for the "warp around":
			dx = dx==(em->SIZE_MAP-1) ? -1 : dx;
			dx = dx==(-(em->SIZE_MAP-1)) ? 1 : dx;
			dy = dy==(em->SIZE_MAP-1) ? -1 : dy;
			dy = dy==(-(em->SIZE_MAP-1)) ? 1 : dy;

			switch (vDisplayDis[j]->repres)
			{
			case 'B':
				{
					if (dx == 0 && dy == 1)
					{
						++nBats;
					}
					if (dx == 0 && dy == -1)
					{
						++nBats;
					}
					if (dx == -1 && dy == 0)
					{
						++nBats;
					}
					if (dx == 1 && dy == 0)
					{
						++nBats;
					}

					if (nBats)
					{
						plural = nBats==1 ? "" : "s";
					}
					break;
				}
			case 'W':
				{
					if ((dx == 0 && dy == 1)
					|| (dx == 0 && dy == -1)
					|| (dx == -1 && dy == 0)
					|| (dx == 1 && dy == 0))
					{
						std::cout << "You smell the Wumpus.\n";
					}
					break;
				}
			case 'O':
				{
					if ((dx == 0 && dy == 1)
					|| (dx == 0 && dy == -1)
					|| (dx == -1 && dy == 0)
					|| (dx == 1 && dy == 0))
					{
						std::cout << "You feel a breeze.\n";
					}
					break;
				}
			default:
				break;
			}
		}
		if (nBats)
		{
			std::cout << "You hear " << nBats << " bat" << plural << ".\n";
		}
	}
}


void S_Feedback::process (long previousFrameTime)
{
	// Retrieve some datas about the player(s?)
	vector<unsigned int> entitiesControl;
	em->fill_entities_componentType(entitiesControl, C_Controllable | C_Position | C_Moveable);
	vector<Position*> vControlPos;
	em->fill_components_of_entities<Position*>(vControlPos, entitiesControl, C_Position);


	// Retrieve some datas about the monsters and the trap
	vector<unsigned int> entitiesTrap;
	em->fill_entities_componentType(entitiesTrap, C_Position | C_Displayable);

	vector<Position*> vTrapPos;
	em->fill_components_of_entities<Position*>(vTrapPos, entitiesTrap, C_Position);

	vector<Displayable*> vTrapDis;
	em->fill_components_of_entities<Displayable*>(vTrapDis, entitiesTrap, C_Displayable);

	for (size_t i=0; i<vControlPos.size(); ++i)
	{
		for (size_t j=0; j<entitiesTrap.size(); ++j)
		{
			if ((vControlPos[i]->x == vTrapPos[j]->x)
			&& (vControlPos[i]->y == vTrapPos[j]->y))
			{
				switch (vTrapDis[j]->repres)
				{
				case 'W':
					std::cout << "You suddenly see the Wumpus! He jumps and devours you!\n";
					break;
				case 'O':
					std::cout << "You enter in a dark room carefully... but don't see the trap and fell into a bottomless pit!\n";
					break;
				case 'B':
					std::cout << "A Giant Bat fell from the ceiling and takes you into another room!\n";
					break;
				default:
					break;
				}
			}
		}
	}
}

