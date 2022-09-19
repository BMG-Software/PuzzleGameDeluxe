

#ifndef BOARD_H_
#define BOARD_H_

#ifdef _WIN32
#include <SDL.h>
// #include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif // _WIN32


#include <vector>


#include "utilities.h"


// Board could also be refactored into components e.g. 
// it could use the same graphics component as the blocks will.
class Board
{


private:

	void CheckForLine(int &score);


	bool CheckForFail();


	void EraseDuplicates(std::vector<Square> to_remove, 
		std::vector<Square> &full_vector);


	bool CompareSquares(Square one, Square two);


public:


	std::vector<Square> board_squares;


	Board();


	Board(SDL_Renderer *ren);


	bool DrawBoardBlocks(SDL_Renderer *ren);


	void AddToBoard(Block block, int &score);


};



#endif // BOARD_H_
