
#include "utilities.h"
#include "constants.h"

#ifndef BOARD_H_
#define BOARD_H_


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


	Board(SDL_Renderer *ren);


	bool DrawBoardBlocks(SDL_Renderer *ren);


	void AddToBoard(Block block, int &score);


};



#endif // BOARD_H_