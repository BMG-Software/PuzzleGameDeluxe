

#ifdef _WIN32
#include <SDL.h>
// #include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif // _WIN32

#include <iostream>

#include "core.h"


/*

Whole program would likely benefit from having a 
proper exception system integrated into it.

*/

const int RC_SUCCESS = 0;
const int RC_FAILURE = 1;

struct GameSetup
{
	bool m_fullscreen = false;
	int m_width = 0;
	int m_height = 0;
};


void ParseCommandLineArgs(int argc, char** argv, GameSetup &gameSetup)
{
	if (argc >= 2)
	{
		if (strncmp(argv[1], "-f", 2) == 0 || strncmp(argv[1], "-F", 2) == 0)
		{
			gameSetup.m_fullscreen = true;
		}
	}

	// Set to default width and height for now
	// TODO: More argument parsing
	gameSetup.m_width = 1280;
	gameSetup.m_height = 720;
}


bool InitialiseSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{

		std::cout << "Error initialising SDL.\n";
		return false;

	}
	return true;
}


int main(int argc, char** argv)
{
	GameSetup gameSetup;
	ParseCommandLineArgs(argc, argv, gameSetup);

	if (!InitialiseSDL())
		return RC_FAILURE;

	Game my_game(gameSetup.m_fullscreen, gameSetup.m_width, gameSetup.m_height);
	my_game.Run();
    SDL_Quit();

	return RC_SUCCESS;

}



