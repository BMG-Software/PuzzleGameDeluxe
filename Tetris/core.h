

#ifndef CORE_H_
#define CORE_H_


#ifdef _WIN32
#include <SDL.h>
// #include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif // _WIN32


#include <memory>
#include <sstream>
#include <vector>


#include "block.h"
#include "board.h"
#include "timer.h"


class Game
{


private:


	int score;


	bool gen_block;


    SDL_GameController *game_controller;
    SDL_Joystick *joystick;
    Uint8 control_direction;


    // Window and renderer could be refactored out into their own class.
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> ren;


	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win;


	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> numbers;


    SDL_Texture *background;
    SDL_Rect background_src, background_dest;

    SDL_Texture *board_background;
    SDL_Rect board_background_dest;


	BlockControl controller;


	Board game_board;


	static std::vector<SDL_Rect> number_clips;


	// Utilised to allow for frame independant movement.
	Timer frame_timer;


	// Checks for quit events.
	bool EventLoop();


	void InitWinAndRen();

	// TODO: rewrite to utilise true type fonts

	// Converts score so that it can be represented on screen.
	void DrawScore();


	// Prints converted score to screen
	void PrintScore(char number, SDL_Rect location);


	// Checks if a new block needs to be generated.
	void CheckForGenBlock();


	// Draw the block in it's current position and add to board if necessary.
	void DrawAndCheckBoardAddition(float frame_time);
	

public:


    static const int WINDOW_WIDTH = 1920;
    static const int WINDOW_HEIGHT = 1080;


	Game();

    virtual ~Game();


	// Main game loop.
	void Run();


};


#endif // CORE_H_
