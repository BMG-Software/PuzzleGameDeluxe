

#include "board.h"
#include "core.h"


Board::Board() = default;


Board::Board(SDL_Renderer *ren)
{
	SDL_Rect rect;
	rect.w = 32;
	rect.h = 32;

	// Create a base line

    int baseline_width = Game::WindowWidth() * 0.75;
	for (int i = Game::WindowWidth() * 0.25; i < baseline_width; i += 32)
	{
		rect.y = Game::WindowHeight();
		rect.x = i;

		board_squares.push_back(Square(rect, nullptr));   
	}
}


bool Board::DrawBoardBlocks(SDL_Renderer *ren)
{

	for (unsigned int i = 0; i < board_squares.size(); ++i)
	{
		
		if (board_squares[i].tex != nullptr)
		{

			Utilities::RenderTexture(ren, board_squares[i].tex,
				board_squares[i].x, board_squares[i].y);
		
		}

		std::vector<Line> my_lines;

		my_lines.push_back(board_squares[i].top);
		my_lines.push_back(board_squares[i].down);
		my_lines.push_back(board_squares[i].left);
		my_lines.push_back(board_squares[i].right);

		Utilities::DrawLines(ren, my_lines);

	}

	return CheckForFail();
	
}


void Board::AddToBoard(Block block, int &score)
{

	for (unsigned int i = 0; i < block.block_squares.size(); ++i)
	{

		board_squares.push_back(block.block_squares[i]);

	}

	CheckForLine(score);

}


void Board::CheckForLine(int &score)
{

	// Refactor out into functions

	int cascade = 0;
	int amnt_destroyed = 0;

	for (int i = Game::WindowHeight(); i > 0; i -= 32)
	{
        double count = Game::WindowWidth() * 0.25;

		std::vector<Square> squares_to_delete;

		for (unsigned int x = 0; x < board_squares.size(); ++x)
		{

			if (board_squares[x].y == i && board_squares[x].y + 32 <= Game::WindowHeight())
			{
				if (cascade > 0)
				{
					board_squares[x].Update(0, cascade);
				}

				count += 32.0;

				squares_to_delete.push_back(board_squares[x]);

				if (count == Game::WindowWidth() * 0.75)
				{
					EraseDuplicates(squares_to_delete, board_squares);
					++amnt_destroyed;
					score += 20 * amnt_destroyed;
					cascade += 32;

				}

			}

		}

	}

}


bool Board::CheckForFail()
{
	for (unsigned int i = 0; i < board_squares.size(); ++i)
	{
		if (board_squares[i].top.a.y <= 0)
		{
			return true;
		}
	}
	return false;
}



void Board::EraseDuplicates(std::vector<Square> to_remove,
	std::vector<Square> &full_vector)
{

	int deletions = 1;

	while (deletions != 0)
	{

		deletions = 0;

		// TODO: Fix this. Code is not safe, works on a wing and a prayer
		for (unsigned int i = 0; i < full_vector.size(); ++i)
		{

			for (unsigned int x = 0; x < to_remove.size(); ++x)
			{

				if (CompareSquares(full_vector[i], to_remove[x]))
				{

					full_vector.erase(full_vector.begin() + i);
					++deletions;
				}

			}

		}

	}

}


bool Board::CompareSquares(Square one, Square two)
{
	return (one.x == two.x && one.y == two.y);
}

