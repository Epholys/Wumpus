#include<iostream>
#include<fstream>
#include<limits>
#include<string>
#include<stdexcept>
#include"menu.h"


bool menu()
{
	bool endOfMenu = false;
	while (!endOfMenu)
	{
		std::cout << "\t\t*** Welcome to Hunt the Wumpus! ***\n\n";
		std::cout << "\t1- Start the game\n";
		std::cout << "\t2- Read the help\n";
		std::cout << "\t3- Quit\n";

		int input=0;
		input = get_input(1, 3);

		switch(input)
		{
		case 1:
			return false;
			break;
		case 2:
			help_screen();
			break;
		case 3:
			return true;
			break;
		default:
			throw std::runtime_error("menu(): Invalid Input");
			break;
		}
	}

	return true;	// Never happen
}


int get_input (int min, int max)
{
	int input=max;
	std::cout << "Your choice: ";
	std::cin >> input;
	do
	{
		if (!std::cin || input<min || max<input)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Please enter a valid choice.\n";
			std::cout << "Your choice: ";
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return input;
		}
	} while (!(std::cin >> input));

	return input;
}

void help_screen()
{
	std::ifstream ifs("Help.txt");
	if (!ifs)
	{
		throw std::ios_base::failure("Can't open Help.txt");
	}
	char buff = ' ';
	while (ifs) {
		ifs.get(buff);
		std::cout << buff;
	}
	std::cin.get();
}
