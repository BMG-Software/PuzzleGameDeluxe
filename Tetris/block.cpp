

#include "block.h"


BlockControl::BlockControl(SDL_Renderer *ren)
	: tex(nullptr, SDL_DestroyTexture)
{

	// There is definitly a better solution than loading tons of different text files.

	// blocks should be small, hardcoded 2D arrays and should be rotated by switching the arrays around

	blocks.push_back(Block(ren, "Resources\\green.png", "Resources\\i-block.txt", "Resources\\i-block-r.txt",
		"Resources\\i-block.txt", "Resources\\i-block-r.txt"));
	blocks.push_back(Block(ren, "Resources\\red.png", "Resources\\j-block.txt", "Resources\\j-block-r.txt",
		"Resources\\j-block-r2.txt", "Resources\\j-block-r3.txt"));
	blocks.push_back(Block(ren, "Resources\\blue.png", "Resources\\l-block.txt", "Resources\\l-block-r.txt",
		"Resources\\l-block-r2.txt", "Resources\\l-block-r3.txt"));
	blocks.push_back(Block(ren, "Resources\\orange.png", "Resources\\o-block.txt", "Resources\\o-block.txt",
		"Resources\\o-block.txt", "Resources\\o-block.txt"));
	blocks.push_back(Block(ren, "Resources\\purple.png", "Resources\\s-block.txt", "Resources\\s-block-r.txt",
		"Resources\\s-block.txt", "Resources\\s-block-r.txt"));
	blocks.push_back(Block(ren, "Resources\\cyan.png", "Resources\\t-block.txt", "Resources\\t-block-r.txt",
		"Resources\\t-block-r2.txt", "Resources\\t-block-r3.txt"));
	blocks.push_back(Block(ren, "Resources\\yellow.png", "Resources\\z-block.txt", "Resources\\z-block-r.txt",
		"Resources\\z-block.txt", "Resources\\z-block-r.txt"));

	speed = 0;

	velocity = 60;

	game_timer.StartTimer();

}


BlockControl::BlockControl(const BlockControl &b) :
    tex(b.tex)
{

	frame_num = b.frame_num;

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
			
			if (direction == UP)
			{

				if (Utilities::CompareLines(block_squares[i].top,
					board_squares[x].down))
				{

					return true;

				}

			}

			if (direction == DOWN)
			{

				if (Utilities::CompareLines(block_squares[i].down,
					board_squares[x].top))
				{

					std::cout << block_squares[i].down.a.y << "\n" << block_squares[i].down.b.y
						<< "\n" << board_squares[x].top.a.y << "\n" << board_squares[x].top.b.y << "\n.";
					system("pause");
					return true;

				}

			}

			if (direction == LEFT)
			{

				if (Utilities::CompareLines(block_squares[i].left,
					board_squares[x].right))
				{

					return true;

				}
				else if (CheckAdvancedCollision(block_squares[i].left, board_squares[x].left, 0))
				{

					return true;

				}

			}

			if (direction == RIGHT)
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
	current_block.SetCurrentSquares(current_block.block_squares);

	frame_num = 1;

}


bool BlockControl::DrawBlock(SDL_Renderer *ren, std::vector<Square> board_squares)
{

	if (UpdatePosition(board_squares))
	{

		return true;

	}

	RenderBlock(ren);

	return false;

}


bool BlockControl::UpdatePosition(std::vector<Square> board_squares)
{

	for (int i = 0; i < speed; ++i)
	{

		if (!CheckCollision(current_block.current_squares, DOWN, board_squares))
		{

			current_block.UpdateSquares(0, 1);
			return false;

		}
		else
		{

			return true;
		
		}

	}

}


void BlockControl::RenderBlock(SDL_Renderer *ren)
{
	
	for (unsigned int i = 0; i < current_block.current_squares.size(); ++i)
	{

		Utilities::RenderTexture(ren, current_block.current_squares[i].tex.get(),
			current_block.current_squares[i].x, current_block.current_squares[i].y);

		std::vector<Line> my_lines;

		my_lines.push_back(current_block.current_squares[i].top);
		my_lines.push_back(current_block.current_squares[i].down);
		my_lines.push_back(current_block.current_squares[i].left);
		my_lines.push_back(current_block.current_squares[i].right);

		Utilities::DrawLines(ren, my_lines);
	}

}


void BlockControl::MoveBlock(std::vector<Square> board_squares, float frame_time)
{

	// Refactor

	const Uint8 *current_state = SDL_GetKeyboardState(NULL);

	if (game_timer.GetTimeSeconds() > time_updated + 0.1)
	{
		
		HandleLeftAndRight(current_state, board_squares);

		HandleUp(current_state, board_squares);

	}

	HandleDown(current_state, frame_time);

}


void BlockControl::HandleUp(const Uint8* state, std::vector<Square> board_squares)
{

	// Check collision based on the direction the block will be rotating to

	if (state[SDL_SCANCODE_UP]
		&& !CheckCollision(current_block.current_squares, UP, board_squares)
		&& !CheckCollision(current_block.current_squares, DOWN, board_squares)
		&& !CheckCollision(current_block.current_squares, LEFT, board_squares)
		&& !CheckCollision(current_block.current_squares, RIGHT, board_squares))
	{
		Rotate(board_squares);

		time_updated = game_timer.GetTimeSeconds();
	}

}


void BlockControl::HandleLeftAndRight(const Uint8 *state, std::vector<Square> board_squares)
{

	if (state[SDL_SCANCODE_RIGHT]
		&& !CheckCollision(current_block.current_squares, RIGHT, board_squares))
	{
		current_block.UpdateSquares(32, 0);

		time_updated = game_timer.GetTimeSeconds();
	}

	if (state[SDL_SCANCODE_LEFT]
		&& !CheckCollision(current_block.current_squares, LEFT, board_squares))
	{
		current_block.UpdateSquares(-32, 0);

		time_updated = game_timer.GetTimeSeconds();
	}

}


void BlockControl::HandleDown(const Uint8 *state, float frame_time)
{

	if (state[SDL_SCANCODE_DOWN])
	{

		if (speed < 10)
		{
			++speed;
		}

	}

	if (!state[SDL_SCANCODE_DOWN])
	{

		speed = velocity * frame_time;

		std::cout << "Moving down at: " << speed << ".\n";

	}

}


Block BlockControl::GetCurrentBlock()
{

	return current_block;

}


void BlockControl::Rotate(std::vector<Square> board_squares)
{

	// AHH! My eyes!!

	++frame_num;

	if (frame_num == 1 && !CheckCollision(current_block.block_squares, UP, board_squares)
		&& !CheckCollision(current_block.block_squares, DOWN, board_squares) &&
		!CheckCollision(current_block.block_squares, RIGHT, board_squares) &&
		!CheckCollision(current_block.block_squares, LEFT, board_squares))
	{

		current_block.SetCurrentSquares(current_block.block_squares);

	}
	else if (frame_num == 2 && !CheckCollision(current_block.block_squares_r, UP, board_squares)
		&& !CheckCollision(current_block.block_squares_r, DOWN, board_squares) &&
		!CheckCollision(current_block.block_squares_r, RIGHT, board_squares) &&
		!CheckCollision(current_block.block_squares_r, LEFT, board_squares))
	{

		current_block.SetCurrentSquares(current_block.block_squares_r);

	}
	else if (frame_num == 3 && !CheckCollision(current_block.block_squares_r2, UP, board_squares)
		&& !CheckCollision(current_block.block_squares_r2, DOWN, board_squares) &&
		!CheckCollision(current_block.block_squares_r2, RIGHT, board_squares) &&
		!CheckCollision(current_block.block_squares_r2, LEFT, board_squares))
	{

		current_block.SetCurrentSquares(current_block.block_squares_r2);

	}
	else if (frame_num == 4 && !CheckCollision(current_block.block_squares_r3, UP, board_squares)
		&& !CheckCollision(current_block.block_squares_r3, DOWN, board_squares) &&
		!CheckCollision(current_block.block_squares_r3, RIGHT, board_squares) &&
		!CheckCollision(current_block.block_squares_r3, LEFT, board_squares))
	{

		current_block.SetCurrentSquares(current_block.block_squares_r3);

	}
	else
	{

		frame_num = 0;

	}


}



