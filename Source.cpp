#include "Game.h"
#include <ctime>
#include <iostream>

int main()
{

	srand(time(NULL));

	Game game;

	try
	{
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
		system("pause");
	}
	return 1;
}