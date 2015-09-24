

#include "constants.h"
#include "board.h"
#include "utilities.h"

#ifndef BLOCKCONTROL_H_
#define BLOCKCONTROL_H_


class BlockControl
{


private:

	enum direction { UP, DOWN, LEFT, RIGHT };


	SDL_Texture* tex;


	int speed;


	Block current_block;


	std::vector<Block> blocks;


public:


	BlockControl(SDL_Renderer *ren);


	bool CheckCollision(std::vector<Square> block_squares, int direction, 
		std::vector<Square> board_squares);


	bool DrawBlock(SDL_Renderer *ren, std::vector<Square> board_squares);


	void MoveBlock(std::vector<Square> board_squares);


	Block GetCurrentBlock();


	void GenerateRandomBlock(); 


	int frame_num;


	void Rotate(std::vector<Square> board_squares);

};


#endif // BLOCKCONTROL_H_