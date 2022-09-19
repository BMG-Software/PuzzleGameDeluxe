

#include "board.h"
#include "core.h"

const int BLOCK_SIZE = 32;

Board::Board()
{
	m_boardBoundary = { 0, 0, 0, 0 };
}


Board::Board(SDL_Renderer *ren, const SDL_Rect &boardBoundary)
{
	m_boardBoundary = boardBoundary;

	SDL_Rect rect;
	rect.w = BLOCK_SIZE;
	rect.h = BLOCK_SIZE;

	// Create a base line
    int baseline_width = m_boardBoundary.x + m_boardBoundary.w;
	for (int i = m_boardBoundary.x; i < baseline_width; i += BLOCK_SIZE)
	{
		rect.x = i;
		rect.y = m_boardBoundary.h;

		board_squares.push_back(Square(rect, nullptr));   
	}
}


bool Board::DrawBoardBlocks(SDL_Renderer *ren)
{
	for (size_t i = 0; i < board_squares.size(); ++i)
	{
		if (board_squares[i].tex)
		{
			Utilities::RenderTexture(ren, board_squares[i].tex,
				board_squares[i].x, board_squares[i].y);
		}

		std::vector<Line> my_lines = {
			board_squares[i].top,
			board_squares[i].down,
			board_squares[i].left,
			board_squares[i].right
		};
		Utilities::DrawLines(ren, my_lines);
	}
	return CheckForFail();
}


void Board::AddToBoard(Block block, int &score)
{
	for (auto &blockSquare : block.block_squares)
	{
		board_squares.push_back(blockSquare);
	}
	CheckForLine(score);
}


void Board::CheckForLine(int &score)
{
	int cascade = 0;
	int amnt_destroyed = 0;

	for (int i = m_boardBoundary.h; i > 0; i -= BLOCK_SIZE)
	{
        double count = m_boardBoundary.x;
		std::vector<Square> squares_to_delete;

		for (size_t x = 0; x < board_squares.size(); ++x)
		{
			if (board_squares[x].y == i && board_squares[x].y + BLOCK_SIZE <= m_boardBoundary.h)
			{
				if (cascade > 0)
				{
					board_squares[x].Update(0, cascade);
				}

				count += static_cast<double>(BLOCK_SIZE);
				squares_to_delete.push_back(board_squares[x]);

				if (count == m_boardBoundary.x + m_boardBoundary.w)
				{
					EraseDuplicates(squares_to_delete, board_squares);
					++amnt_destroyed;
					score += 20 * amnt_destroyed;
					cascade += BLOCK_SIZE;
				}
			}
		}
	}
}


bool Board::CheckForFail()
{
	for (size_t i = 0; i < board_squares.size(); ++i)
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
		for (size_t i = 0; i < full_vector.size(); ++i)
		{
			for (size_t x = 0; x < to_remove.size(); ++x)
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

