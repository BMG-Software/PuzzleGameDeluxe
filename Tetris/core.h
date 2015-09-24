
// These should be in the header guard...

#include "block.h"
#include "board.h"
#include "constants.h"

#ifndef CORE_H_
#define CORE_H_


class Game
{


private:


	int score;


	bool gen_block;


	SDL_Renderer *ren;


	SDL_Window *win;


	SDL_Texture *numbers;


	static std::vector<SDL_Rect> number_clips;


	bool EventLoop();


	void InitWinAndRen();


	void DrawScore();


	void PrintScore(char number, SDL_Rect location);


public:


	Game();


	void Run();


};


#endif // CORE_H_