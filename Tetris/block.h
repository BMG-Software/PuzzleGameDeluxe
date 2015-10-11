



#ifndef BLOCKCONTROL_H_
#define BLOCKCONTROL_H_


#include <SDL.h>
#include <SDL_image.h>


//#include <memory>
#include <array>


#include "board.h"
#include "utilities.h"
#include "timer.h"


class BlockControl
{


private:


	// Each array stores the pattern for loading a block
	static std::array<std::array<int, 4>, 4> i_block;


	static std::array<std::array<int, 4>, 4> j_block;


	static std::array<std::array<int, 4>, 4> l_block;


	static std::array<std::array<int, 4>, 4> s_block;


	static std::array<std::array<int, 4>, 4> t_block;


	static std::array<std::array<int, 4>, 4> z_block;


	static std::array<std::array<int, 4>, 4> o_block;


	int speed;


	int velocity;
	

	float time_updated;


	Block current_block;


	Timer game_timer;


	std::vector<Block> blocks;


public:


	BlockControl();


	BlockControl(SDL_Renderer *ren);


	BlockControl(const BlockControl &b);


	bool CheckCollision(std::vector<Square> block_squares, int direction, 
		std::vector<Square> board_squares);


	bool CheckAdvancedCollision(Line block_line, Line board_line, int limit);


	bool DrawBlock(SDL_Renderer *ren, 
		std::vector<Square> board_squares, float frame_time);


	bool UpdatePosition(std::vector<Square> board_squares, float frame_time);


	void RenderBlock(SDL_Renderer *ren);


	void MoveBlock(SDL_Renderer* ren, 
		std::vector<Square> board_squares, float frame_time);


	void HandleUp(SDL_Renderer* ren, const Uint8 *state, 
		std::vector<Square> board_squares);


	void HandleLeftAndRight(const Uint8 *state, 
		std::vector<Square> board_squares);


	void HandleDown(const Uint8 *state, float frame_time, std::vector<Square> board_squares);


	Block GetCurrentBlock();


	void GenerateRandomBlock(); 


	void Rotate(SDL_Renderer* ren, std::vector<Square> board_squares);




};


#endif // BLOCKCONTROL_H_