

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "core.h"


/*

Whole program would likely benefit from having a 
proper exception system integrated into it.

*/


int InitialiseSDL()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{

		std::cout << "Error initialising SDL.\n";
		return -1;

	}

	return 0;

}


int main(int argc, char* argv[])
{

	if (InitialiseSDL() < 0) return -1;

	Game my_game;

	my_game.Run();

	return 0;

}



