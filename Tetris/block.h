

#ifndef BLOCKCONTROL_H_
#define BLOCKCONTROL_H_


#ifdef _WIN32
#include <SDL.h>
// #include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif // _WIN32


#include <array>


#include "board.h"
#include "utilities.h"
#include "timer.h"


// TODO: Modify and turn into a control component that makes up part of a block.
class BlockControl
{


private:


	// TODO: Move these arrays into the block class.
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

    static Uint8 block_direction_up, block_direction_down, block_direction_left, block_direction_right, block_direction_superdown;

	BlockControl();


	BlockControl(SDL_Renderer *ren);


	BlockControl(const BlockControl &b);



	// TODO: Refactor collision out into a block physics component.
	bool CheckCollision(std::vector<Square> block_squares, int direction, 
		std::vector<Square> board_squares);


	bool CheckAdvancedCollision(Line block_line, Line board_line, int limit);

	// TODO: Move to graphics component.
	bool DrawBlock(SDL_Renderer *ren, 
		std::vector<Square> board_squares, float frame_time);


	bool UpdatePosition(std::vector<Square> board_squares, float frame_time);


	void RenderBlock(SDL_Renderer *ren);


	void MoveBlock(SDL_Renderer* ren, std::vector<Square> board_squares, Uint8 direction, float frame_time);


	void HandleUp(SDL_Renderer* ren, Uint8 direction, std::vector<Square> board_squares);


	void HandleLeftAndRight(Uint8 direction, std::vector<Square> board_squares);


	void HandleDown(Uint8 direction, float frame_time, std::vector<Square> board_squares);


	Block GetCurrentBlock();


	void GenerateRandomBlock(); 


	void Rotate(SDL_Renderer* ren);
	

};


#endif // BLOCKCONTROL_H_
