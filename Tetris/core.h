

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
#include "resourcestore.h"


class Game
{
private:
	int score;
	static int m_windowWidth, m_windowHeight;

	ResourceStore& m_resourceStore;

    SDL_GameController *game_controller;
    SDL_Joystick *joystick;

    Uint8 m_p1ControlDirection, m_p2ControlDirection;

    // Window and renderer could be refactored out into their own class.
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> ren;
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win;
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> numbers;

    std::shared_ptr<SDL_Texture> background;
    SDL_Rect background_src, background_dest;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> board_background;
    SDL_Rect m_p1BoardDest, m_p2BoardDest;


	static std::vector<SDL_Rect> number_clips;


	// Utilised to allow for frame independant movement.
	Timer frame_timer;


	// Checks for quit events.
	bool EventLoop();


	void InitWinAndRen(bool fullscreen);

	// TODO: rewrite to utilise true type fonts

	// Converts score so that it can be represented on screen.
	void DrawScore();


	// Prints converted score to screen
	void PrintScore(char number, SDL_Rect location);


	// Checks if a new block needs to be generated.
	void CheckForGenBlock(BlockControl &controller, bool &inOutGenBlock);


	// Draw the block in it's current position and add to board if necessary.
	bool DrawAndCheckBoardAddition(float frame_time, BlockControl &controller, Board &gameBoard);
	

public:
	Game(bool fullscreen, int windowWidth, int windowHeight);

    virtual ~Game();

	// Main game loop.
	void Run();


};


#endif // CORE_H_
