

#include "block.h"


std::array < std::array<int, 4>, 4 > BlockControl::i_block = 
{
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::j_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 1, 1, 0, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::l_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 1, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::s_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 0, 1, 1 } },
	std::array<int, 4> { { 0, 1, 1, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::t_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 1, 1, 1 } },
	std::array<int, 4> { { 0, 0, 1, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::z_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 1, 1, 0, 0 } },
	std::array<int, 4> { { 0, 1, 1, 0 } }
};


std::array < std::array<int, 4>, 4 > BlockControl::o_block =
{
	std::array<int, 4> { { 0, 0, 0, 0 } },
	std::array<int, 4> { { 0, 1, 1, 0 } },
	std::array<int, 4> { { 0, 1, 1, 0 } },
	std::array<int, 4> { { 0, 0, 0, 0 } }
};


BlockControl::BlockControl(){}


BlockControl::BlockControl(SDL_Renderer *ren)
{

	blocks.push_back(Block(ren, "Resources\\green.png", i_block));
	blocks.push_back(Block(ren, "Resources\\red.png", j_block));
	blocks.push_back(Block(ren, "Resources\\blue.png", l_block));
	blocks.push_back(Block(ren, "Resources\\orange.png", o_block));
	blocks.push_back(Block(ren, "Resources\\purple.png", s_block));
	blocks.push_back(Block(ren, "Resources\\cyan.png", t_block));
	blocks.push_back(Block(ren, "Resources\\yellow.png", z_block));

	speed = 0;
	
	velocity = 60;

	game_timer.StartTimer();

}


BlockControl::BlockControl(const BlockControl &b) 
{

	speed = b.speed;

	current_block = b.current_block;

	blocks = b.blocks;

	velocity = b.velocity;

}


bool PointInLineRange(Point p, Line line)
{

	if (p.x == line.a.x &&
		p.y > line.a.y &&
		p.y < line.b.y)
	{
		return true;
	}

	else return false;

}


bool BlockControl::CheckCollision(std::vector<Square> block_squares, int direction,
	std::vector<Square> board_squares)
{

	for (unsigned int i = 0; i < block_squares.size(); ++i)
	{

		for (unsigned int x = 0; x < board_squares.size(); ++x)
		{
			
			if (direction == Block::UP)
			{

				if (Utilities::CompareLines(block_squares[i].top,
					board_squares[x].down))
				{

					return true;

				}

			}

			if (direction == Block::DOWN)
			{

				if (Utilities::CompareLines(block_squares[i].down,
					board_squares[x].top))
				{

					return true;

				}

			}

			if (direction == Block::LEFT)
			{

				if (Utilities::CompareLines(block_squares[i].left,
					board_squares[x].right))
				{

					return true;

				}
				else if (CheckAdvancedCollision(block_squares[i].left, board_squares[x].right, 0))
				{

					return true;

				}

			}

			if (direction == Block::RIGHT)
			{

				if (Utilities::CompareLines(block_squares[i].right,
					board_squares[x].left))
				{

					return true;

				}
				else if (CheckAdvancedCollision(block_squares[i].right, board_squares[x].left, 480))
				{

					return true;

				}

			}

		}

	}

	return false;

}


bool BlockControl::CheckAdvancedCollision(Line block_line, Line board_line, int limit)
{

	if (PointInLineRange(block_line.a, board_line)
		|| PointInLineRange(block_line.b, board_line)
		|| block_line.a.x == limit)
	{

		return true;

	}
	return false;

}


void BlockControl::GenerateRandomBlock()
{
	int i = rand() % 7;
	current_block = blocks[i];
	

}


bool BlockControl::DrawBlock(SDL_Renderer *ren, 
	std::vector<Square> board_squares, float frame_time)
{

	if (UpdatePosition(board_squares, frame_time))
	{

		return true;

	}

	RenderBlock(ren);

	return false;

}


bool BlockControl::UpdatePosition(std::vector<Square> board_squares, float frame_time)
{

	if (!CheckCollision(current_block.block_squares, Block::DOWN, board_squares))
	{

		current_block.UpdateSquares(0, 1/*velocity * frame_time*/);
		return false;

	}
	else
	{

		return true;
		
	}
	
}


void BlockControl::RenderBlock(SDL_Renderer *ren)
{

	for (unsigned int i = 0; i < current_block.block_squares.size(); ++i)
	{

		Utilities::RenderTexture(ren, current_block.block_squares[i].tex/*.get()*/,
			current_block.block_squares[i].x, current_block.block_squares[i].y);
		
		std::vector<Line> my_lines;

		my_lines.push_back(current_block.block_squares[i].top);
		my_lines.push_back(current_block.block_squares[i].down);
		my_lines.push_back(current_block.block_squares[i].left);
		my_lines.push_back(current_block.block_squares[i].right);

		// current_block.block_squares[i].PrintSquareInfo();

		Utilities::DrawLines(ren, my_lines);
	}

}


void BlockControl::MoveBlock(SDL_Renderer* ren, 
	std::vector<Square> board_squares, float frame_time)
{

	const Uint8 *current_state = SDL_GetKeyboardState(NULL);

	// Limit the speed at which the block can move left and
	// right as well as rotation.
	if (game_timer.GetTimeSeconds() > time_updated + 0.1)
	{
		
		HandleLeftAndRight(current_state, board_squares);
				
		HandleUp(ren, current_state, board_squares);

	}

	HandleDown(current_state, frame_time, board_squares);
	
}


void BlockControl::HandleUp(SDL_Renderer* ren, 
	const Uint8* state, std::vector<Square> board_squares)
{

	// Before attempting to rotate, we check for collision in any direction 
	// for the block in it's current state.
	if (state[SDL_SCANCODE_UP]
		&& !CheckCollision(current_block.block_squares, Block::UP, board_squares)
		&& !CheckCollision(current_block.block_squares, Block::DOWN, board_squares)
		&& !CheckCollision(current_block.block_squares, Block::LEFT, board_squares)
		&& !CheckCollision(current_block.block_squares, Block::RIGHT, board_squares))
	{

		Rotate(ren, board_squares);
		
		time_updated = game_timer.GetTimeSeconds();
	}

}


void BlockControl::HandleLeftAndRight(const Uint8 *state, std::vector<Square> board_squares)
{

	if (state[SDL_SCANCODE_RIGHT]
		&& !CheckCollision(current_block.block_squares, Block::RIGHT, board_squares))
	{
		current_block.UpdateSquares(32, 0);

		time_updated = game_timer.GetTimeSeconds();
	}

	if (state[SDL_SCANCODE_LEFT]
		&& !CheckCollision(current_block.block_squares, Block::LEFT, board_squares))
	{
		current_block.UpdateSquares(-32, 0);

		time_updated = game_timer.GetTimeSeconds();
	}

}


void BlockControl::HandleDown(const Uint8 *state, float frame_time, std::vector<Square> board_squares)
{
	
	if (!CheckCollision(current_block.block_squares, Block::DOWN, board_squares))
	{

		if (state[SDL_SCANCODE_DOWN])
		{
			
			velocity = 120;

		}

		if (!state[SDL_SCANCODE_DOWN])
		{

			if (velocity == 120)
			{

				velocity = 60;

			}

		}

	}

}


Block BlockControl::GetCurrentBlock()
{

	return current_block;

}


void BlockControl::Rotate(SDL_Renderer* ren, std::vector<Square> board_squares)
{

	current_block.Rotate(ren);
	
}

