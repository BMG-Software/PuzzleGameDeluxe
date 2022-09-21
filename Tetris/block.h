

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
	int speed;
	int velocity;
	float time_updated;

	SDL_Rect m_boardBoundary;

	Block current_block;
	Timer game_timer;

	std::vector<Block> blocks;

public:
    static Uint8 block_direction_up, block_direction_down, block_direction_left, block_direction_right, block_direction_superdown;

	BlockControl();
	BlockControl(SDL_Renderer *ren, SDL_Rect boardBoundary);
	BlockControl(const BlockControl &b);

	bool CheckCollision(const std::vector<Square> &block_squares, int direction, const std::vector<Square> &board_squares);
	bool CheckAdvancedCollision(const Line &block_line, const Line &board_line, int limit);

	bool DrawBlock(SDL_Renderer *ren, const std::vector<Square> &board_squares, float frame_time);
	bool UpdatePosition(const std::vector<Square> &board_squares, float frame_time);
	void RenderBlock(SDL_Renderer *ren);

	void MoveBlock(SDL_Renderer* ren, const std::vector<Square> &board_squares, Uint8 direction, float frame_time);
	void HandleUp(SDL_Renderer* ren, Uint8 direction, const std::vector<Square> &board_squares);
	void HandleLeftAndRight(Uint8 direction, const std::vector<Square> &board_squares);
	void HandleDown(Uint8 direction, float frame_time, const std::vector<Square> &board_squares);

	Block GetCurrentBlock();
	void GenerateRandomBlock(); 
	void Rotate(SDL_Renderer* ren);
};


#endif // BLOCKCONTROL_H_
