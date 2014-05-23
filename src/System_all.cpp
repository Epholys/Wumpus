#include<iostream>
#include<stdexcept>
#include<vector>
#include"System_all.h"
#include"System_helper.h"
#include"Assemblage.h"
#include"Entity_manager.h"
#include"math.h"

using std::vector;
using std::runtime_error;


//----------------------------------------------------------------------------------------------

void S_Movement::process(long previousFrameTime)
// Update the Position of all Moveable entities
{
	// retrieve all the C_Position and C_Moveable of the entities which have both
	vector<unsigned int> entities;
	em->fill_entities_componentType(entities, C_Position | C_Moveable);

	vector<Position*> vPos;
	em->fill_components_of_entities<Position*>(vPos, entities, C_Position);

	vector<Moveable*> vMov;
	em->fill_components_of_entities<Moveable*>(vMov, entities, C_Moveable);


	// if there isn't a wall, update the position
	for (size_t i=0; i<vPos.size(); ++i)
	{
		int newX = vPos[i]->x + vMov[i]->dx;
		newX = (newX==em->SIZE_MAP) ? 0 : newX;
		newX = (newX==-1) ? em->SIZE_MAP-1 : newX;

		int newY = vPos[i]->y + vMov[i]->dy;
		newY = (newY==em->SIZE_MAP) ? 0 : newY;
		newY = (newY==-1) ? em->SIZE_MAP-1 : newY;

		bool moveOk = !(collision_with_wall(em, newX, newY));

		if (moveOk)
		{
			vPos[i]->x = newX;
			vPos[i]->y = newY;
			vMov[i]->dx = 0;
			vMov[i]->dy = 0;
		}
	}
}


//----------------------------------------------------------------------------------------------

void S_Damage::process(long previousFrameTime)
// Update all about the Deal_Damage and Damageable Components
{
	// Take the position, the type and the faction of the damages
	vector<unsigned int> dealDamageEntities;
	em->fill_entities_componentType(dealDamageEntities, C_Position | C_Deal_Damage | C_Faction);

	vector<Position*> vDamagePos;
	em->fill_components_of_entities<Position*>(vDamagePos, dealDamageEntities, C_Position);

	vector<Deal_Damage*> vDamageDam;
	em->fill_components_of_entities<Deal_Damage*>(vDamageDam, dealDamageEntities, C_Deal_Damage);

	vector<Faction*> vDamageFac;
	em->fill_components_of_entities<Faction*>(vDamageFac, dealDamageEntities, C_Faction);


	// Take the units the Position, the type and the faction of units that can be damaged
	vector<unsigned int> damageableEntities;
	em->fill_entities_componentType(damageableEntities, C_Position | C_Damageable | C_Faction);

	vector<Position*> vDamageablePos;
	em->fill_components_of_entities<Position*>(vDamageablePos, damageableEntities, C_Position);

	vector<Damageable*> vDamageableDam;
	em->fill_components_of_entities<Damageable*>(vDamageableDam, damageableEntities, C_Damageable);

	vector<Faction*> vDamageableFac;
	em->fill_components_of_entities<Faction*>(vDamageableFac, damageableEntities, C_Faction);


	for (size_t i=0; i<vDamagePos.size(); ++i)
	{

		// Deal with the duration stuff
		if (vDamageDam[i]->duration > 0)
		{
			--(vDamageDam[i]->duration);
		}
		else if (vDamageDam[i]->duration == 0)
		{
			em->kill_entity(dealDamageEntities[i]);
		}

		// Deal with the position, the faction, and the type of damages
		for (size_t j=0; j<vDamageablePos.size(); ++j)
		{

			if ((vDamagePos[i]->x == vDamageablePos[j]->x) &&
				(vDamagePos[i]->y == vDamageablePos[j]->y))
			{

				if (vDamageDam[i]->ground && vDamageableDam[j]->fly)
				{ /*do nothing*/ }

				else if (vDamageFac[i]->faction != vDamageableFac[j]->faction)
				{
					vDamageableDam[j]->life -= vDamageDam[i]->damage;
				}
			}
		}

	}


	// kill all damageable entities with null life
	for (size_t i=0; i<vDamageableDam.size(); ++i)
	{
		if (vDamageableDam[i]->life <= 0)
		{
			em->kill_entity(damageableEntities[i]);
		}
	}
}


//----------------------------------------------------------------------------------------------

void S_Trap::process(long previousFrameTime)
// Active all the traps
{
	// Retrieve datas about the traps
	vector<unsigned int> trapEntities;
	em->fill_entities_componentType(trapEntities, C_Position | C_Trap | C_Faction);

	vector<Position*> vTrapPos;
	em->fill_components_of_entities<Position*>(vTrapPos, trapEntities, C_Position);

	vector<Trap*> vTrapTrap;
	em->fill_components_of_entities<Trap*>(vTrapTrap, trapEntities, C_Trap);

	vector<Faction*> vTrapFac;
	em->fill_components_of_entities<Faction*>(vTrapFac, trapEntities, C_Faction);


	// Retrieve datas about the victims
	vector<unsigned int> victEntities;
	em->fill_entities_componentType(victEntities, C_Position | C_Faction);

	vector<Position*> vVictPos;
	em->fill_components_of_entities<Position*>(vVictPos, victEntities, C_Position);

	vector<Faction*> vVictFac;
	em->fill_components_of_entities<Faction*>(vVictFac, victEntities, C_Faction);


	// Active the Traps (yeah, I know, spaghetti code, like everything else)
	for (size_t i=0; i<vTrapPos.size(); ++i)
	{
		for (size_t j=0; j<vVictPos.size(); ++j)
		{
			if ((vTrapPos[i]->x == vVictPos[j]->x)
			&& (vTrapPos[i]->y == vVictPos[j]->y)
			&& (vTrapFac[i]->faction != vVictFac[j]->faction))
			{
				switch(vTrapTrap[i]->type)
				{
				case Trap::Teleport:
					{
						int rx=0;
						int ry=0;
						do {
							rx = randint(0,em->SIZE_MAP);
							ry = randint(0,em->SIZE_MAP);
						} while (collision_with_wall(em, rx, ry));

						vVictPos[j]->x = rx;
						vVictPos[j]->y = ry;
					}
				default:
					break;
				}
			}
		}
	}
}


//----------------------------------------------------------------------------------------------

void S_IA::process(long previousFrameTime)
// Update the Moveable of all AI Entities, with the help of Deal_Damage for the noise aspect
{
	// Retrieve datas about AI Entities
	vector<unsigned int> IAEntities;
	em->fill_entities_componentType(IAEntities, C_Moveable | C_IA | C_Position);

	vector<Moveable*> vIAMov;
	em->fill_components_of_entities<Moveable*>(vIAMov, IAEntities, C_Moveable);

	vector<IA*> vIAIA;
	em->fill_components_of_entities<IA*>(vIAIA, IAEntities, C_IA);

	vector<Position*> vIAPos;
	em->fill_components_of_entities<Position*>(vIAPos, IAEntities, C_Position);


	// Retrieve datas about the noise aspect
	vector<unsigned int> damageEntities;
	em->fill_entities_componentType(damageEntities, C_Deal_Damage);

	vector<Deal_Damage*> vDamageDam;
	em->fill_components_of_entities(vDamageDam, damageEntities, C_Deal_Damage);


	// deal with the awake/sensible/noise things
	for (size_t i=0; i<vIAIA.size(); ++i)
	{
		bool noise=false;
		for (size_t j=0; j<vDamageDam.size(); ++j)
		{
			if (vDamageDam[j]->noisy)
			{
				noise=true;
			}
		}
		if (noise && vIAIA[i]->sensible)
		{
			vIAIA[i]->awake=true;
		}
	}

	// deal with the movement and the other part a part of the noise things
	for (size_t i=0; i<vIAMov.size(); ++i)
	{
		if (vIAIA[i]->awake)
		{
			while (true)
			{
				int dx=randint(-1,2);
				int dy=randint(-1,2);

				int newX = vIAPos[i]->x + dx;
				newX = (newX==-1) ? em->SIZE_MAP-1 : newX;
				newX = (newX==em->SIZE_MAP) ? 0 : newX;

				int newY = vIAPos[i]->y + dy;
				newY = (newY==-1) ? em->SIZE_MAP-1 : newY;
				newY = (newY==em->SIZE_MAP) ? 0 : newY;

				if ((dx+dy==1 || dx+dy==-1)
				&& !collision_with_wall(em, newX, newY))
				{
					vIAMov[i]->dx = dx;
					vIAMov[i]->dy = dy;
					break;
				}
			}
		}

		if (vIAIA[i]->awake && vIAIA[i]->sensible)
		{
			vIAIA[i]->awake=false;
		}
	}

}


//----------------------------------------------------------------------------------------------

void S_Display::process(long previousFrameTime)
// Display all the displayable entities
// For Debugging
{
	vector<unsigned int> displayEntities;
	em->fill_entities_componentType(displayEntities, C_Position | C_Displayable);

	vector<Position*> vDisplayPos;
	em->fill_components_of_entities<Position*>(vDisplayPos, displayEntities, C_Position);

	vector<Displayable*> vDisplayDis;
	em->fill_components_of_entities<Displayable*>(vDisplayDis, displayEntities, C_Displayable);


	for (size_t i=0; i< em->SIZE_MAP; ++i)
	{
		for (size_t j=0; j< em->SIZE_MAP; ++j)
		{
			bool entityFound=false;
			for (size_t k=0; k<vDisplayPos.size(); ++k)
			{
				if ( (vDisplayPos[k]->x == i) && (vDisplayPos[k]->y == j) )
				{
					std::cout << vDisplayDis[k]->repres;
					entityFound=true;
					break;
				}
			}
			if (!entityFound)
			{
				std::cout << ' ';
			}
		}
		std::cout << '\n';
	}
}


//----------------------------------------------------------------------------------------------

void S_Generate::process(long previousFrameTime)
// Generate the map
// WARNING : entity_manager must be empty
{
	// create all the walls
	vector<vector<unsigned int>> labyrinth (em->SIZE_MAP, vector<unsigned int>(em->SIZE_MAP));
	for (unsigned int i=0; i < em->SIZE_MAP; ++i)
	{
		for (unsigned int j=0; j < em->SIZE_MAP; ++j)
		{
			unsigned int wall = em->create_entity(Walll());
			labyrinth[i][j] = wall;
			Position* position = em->get_component<Position*>(wall, C_Position);
			position->x = i;
			position->y = j;
		}
	}

	// Destroy some walls to form a dungeon
	int nRoom=1;
	int nEmptyRoom=1;
	int x = randint(0,em->SIZE_MAP);
	int y = randint(0,em->SIZE_MAP);
	em->kill_entity(labyrinth[x][y]);

	unsigned int emptyRoom = em->create_entity(Room());

	Position* position = em->get_component<Position*>(emptyRoom, C_Position);
	position->x = x;
	position->y = y;

	Numbered* numbered = em->get_component<Numbered*>(emptyRoom, C_Numbered);
	numbered->number = nEmptyRoom;

	labyrinth[x][y] = 0;

	while (nRoom < em->N_ROOM)
	{
		int rx = randint(-1,2);
		int ry = randint(-1,2);

		if (rx+ry==1 || rx+ry==-1)
		{
			x+=rx;
			x = (x==-1) ? em->SIZE_MAP-1 : x;
			x = (x==em->SIZE_MAP) ? 0 : x;

			y+=ry;
			y = (y==-1) ? em->SIZE_MAP-1 : y;
			y = (y==em->SIZE_MAP) ? 0 : y;


			if (labyrinth[x][y] != 0)
			{

				em->kill_entity(labyrinth[x][y]);
				labyrinth[x][y] = 0;
				++nRoom;

				int rt = randint(0,100);
				if (rt < em->PROB_PIT)
				{
					unsigned int pit = em->create_entity(Pit());
					Position* position = em->get_component<Position*>(pit, C_Position);
					position->x = x;
					position->y = y;
					labyrinth[x][y]=pit;
				}
				else
				{
					++nEmptyRoom;

					unsigned int eRoom = em->create_entity(Room());

					Position* position = em->get_component<Position*>(eRoom, C_Position);
					position->x = x;
					position->y = y;

					Numbered* numbered = em->get_component<Numbered*> (eRoom, C_Numbered);
					numbered->number = nEmptyRoom;
				}
			}
		}
	}


	// Create the monsters:
	//	- bat
	int nBats=0;
	while (nBats < em->N_BAT)
	{
		x = randint(0,em->SIZE_MAP);
		y = randint(0,em->SIZE_MAP);

		if (labyrinth[x][y] == 0)
		{
			unsigned int bat = em->create_entity(Bat());
			Position* position = em->get_component<Position*>(bat, C_Position);
			position->x = x;
			position->y = y;
			labyrinth[x][y]=bat;
			++nBats;
		}
	}

	//	- wumpus
	int nWumpus=0;
	while (nWumpus < em->N_WUMPUS)
	{
		x = randint(0,em->SIZE_MAP);
		y = randint(0,em->SIZE_MAP);

		if (labyrinth[x][y] == 0)
		{
			unsigned int wumpus = em->create_entity(Wumpus());
			Position* position = em->get_component<Position*>(wumpus, C_Position);
			position->x = x;
			position->y = y;
			labyrinth[x][y]=wumpus;
			++nWumpus;
		}
	}


	// create the player:
	bool playerCreated=false;
	while (!playerCreated)
	{
		x = randint(0,em->SIZE_MAP);
		y = randint(0,em->SIZE_MAP);
		if (labyrinth[x][y] == 0)
		{
			unsigned int player = em->create_entity(Player());
			Position* position = em->get_component<Position*>(player, C_Position);
			position->x = x;
			position->y = y;
			labyrinth[x][y]=player;

			playerCreated=true;
		}
	}
}


//----------------------------------------------------------------------------------------------

bool S_EndOfGame::special_process(long previousFrameTime)
{
	vector<unsigned int> vitalEntities;
	em->fill_entities_componentType(vitalEntities, C_Vital);
	vector<Vital*> vVitalVit;
	em->fill_components_of_entities<Vital*>(vVitalVit, vitalEntities, C_Vital);

	bool allPlayerDead=true;
	bool allWumpusDead=true;
	for (auto it=vVitalVit.begin(); it!=vVitalVit.end(); ++it)
	{
		if ((*it)->player)
		{
			allPlayerDead = false;
		}
		if (!((*it)->player))
		{
			allWumpusDead = false;
		}
	}

	if (allPlayerDead)
	{
		std::cout << "You're dead ! You lost.\n";
		std::cin.get();
		return true;
	}
	if (allWumpusDead)
	{
		std::cout << "You killed the Wumpus ! You winned.\n";
		std::cin.get();
		return true;
	}

	return false;
}
