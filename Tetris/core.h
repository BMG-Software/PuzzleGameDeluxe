

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


	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> ren;


	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win;


	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> numbers;


	static std::vector<SDL_Rect> number_clips;


	Timer frame_timer;


	bool EventLoop();


	void InitWinAndRen();


	void DrawScore();


	void PrintScore(char number, SDL_Rect location);


public:


	Game();


	void Run();


};


#endif // CORE_H_