#include<ctime>
#include<iostream>
#include<string>
#include"Entity_manager.h"
#include"System_all.h"
#include"Assemblage.h"
#include"menu.h"

int main()
{
	srand(time(nullptr));

	bool endOfPlay = menu();

	while (!endOfPlay) {

		Entity_manager* world = new Entity_manager();

		S_Generate generate (world);
		S_Display display (world);
		S_Feedback feedback (world);
		S_Damage damage (world);
		S_Trap trap (world);
		S_IA ai (world);
		S_Player player (world);
		S_Movement movement (world);
		S_EndOfGame endOfGame (world);

		bool endOfMatch = false;

		generate.process();

		while (!endOfMatch) {
//			display.process();

			player.process();

			damage.process();
			endOfMatch = endOfGame.special_process();
			if (endOfMatch) break;

			ai.process();

			movement.process();
			feedback.process();

			trap.process();
			feedback.process();
			damage.process();
			endOfMatch = endOfGame.special_process();
		}

		delete world;

		endOfPlay = menu();
	}

	return 0;
}
