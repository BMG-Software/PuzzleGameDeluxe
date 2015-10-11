

#ifndef CORE_H_
#define CORE_H_


#include <SDL.h>
#include <SDL_image.h>


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

    // Window and renderer could be refactored out into their own class.
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> ren;


	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win;


	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> numbers;


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


	Game();


	// Main game loop.
	void Run();


};


#endif // CORE_H_