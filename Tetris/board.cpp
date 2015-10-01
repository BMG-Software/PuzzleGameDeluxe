

#include "board.h"


Board::Board(SDL_Renderer *ren)
{

	// Create a base line
	for (int i = 0; i < 480; i += 32)
	{

		board_squares.push_back(Square(i, 800, nullptr,
			Line(i, 800, i + 32, 800),
			Line(i, 800 + 32, i + 32, 800 + 32),
			Line(i, 800, i, 800 + 32),
			Line(i + 32, 800, i + 32, 800 + 32)));
	
	}

}


bool Board::DrawBoardBlocks(SDL_Renderer *ren)
{

	for (unsigned int i = 0; i < board_squares.size(); ++i)
	{

		Utilities::RenderTexture(ren, board_squares[i].tex.get(),
			board_squares[i].x, board_squares[i].y);

	}

	return CheckForFail();
	

}


void Board::AddToBoard(Block block, int &score)
{

	for (unsigned int i = 0; i < block.current_squares.size(); ++i)
	{

		board_squares.push_back(block.current_squares[i]);

	}

	CheckForLine(score);

}


void Board::CheckForLine(int &score)
{

	// Refactor out into functions

	int cascade = 0;
	int amnt_destroyed = 0;

	for (int i = 800; i > 0; i -= 32)
	{
		int count = 0;

		std::vector<Square> squares_to_delete;

		for (unsigned int x = 0; x < board_squares.size(); ++x)
		{

			if (board_squares[x].y == i && board_squares[x].y + 32 <= 800)
			{
				if (cascade > 0)
				{
					board_squares[x].Update(0, cascade);
				}

				count += 32;

				squares_to_delete.push_back(board_squares[x]);

				if (count == 480)
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
		/*
		if (board_squares[i].top.a.y <= 0)
		{

			return true;

		}*/

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

	if (one.x == two.x
		&& one.y == two.y)
	{

		return true;

	}

	else return false;
}

