



#ifndef BLOCKCONTROL_H_
#define BLOCKCONTROL_H_


#include <SDL.h>
#include <SDL_image.h>


#include <memory>


#include "board.h"
#include "utilities.h"
#include "timer.h"


class BlockControl
{


private:

	enum direction { UP, DOWN, LEFT, RIGHT };


	std::shared_ptr<SDL_Texture> tex;


	int speed;


	int velocity;


	float time_updated;


	Block current_block;


	Timer game_timer;


	std::vector<Block> blocks;


public:


	BlockControl(SDL_Renderer *ren);


	BlockControl(const BlockControl &b);


	bool CheckCollision(std::vector<Square> block_squares, int direction, 
		std::vector<Square> board_squares);


	bool CheckAdvancedCollision(Line block_line, Line board_line, int limit);


	bool DrawBlock(SDL_Renderer *ren, std::vector<Square> board_squares);


	bool UpdatePosition(std::vector<Square> board_squares);


	void RenderBlock(SDL_Renderer *ren);


	void MoveBlock(std::vector<Square> board_squares, float frame_time);


	void HandleUp(const Uint8 *state, std::vector<Square> board_squares);


	void HandleLeftAndRight(const Uint8 *state, std::vector<Square> board_squares);


	void HandleDown(const Uint8 *state, float frame_time);


	Block GetCurrentBlock();


	void GenerateRandomBlock(); 


	int frame_num;


	void Rotate(std::vector<Square> board_squares);

};


#endif // BLOCKCONTROL_H_