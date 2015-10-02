

#include "utilities.h"


Line::Line(){};


Line::Line(int ax, int ay, int bx, int by)
{

	a.x = ax;

	a.y = ay;

	b.x = bx;

	b.y = by;

}


Square::Square(SDL_Rect rect, SDL_Texture *tex) : 
    tex(tex, SDL_DestroyTexture)
{

	x = rect.x;

	y = rect.y;
	
	top = Line(rect.x, rect.y, rect.x + rect.w, rect.y);

	down = Line(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);

	left = Line(rect.x, rect.y, rect.x, rect.y + rect.h);

	right = Line(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);

}


void Square::Update(int x, int y)
{

	this->x += x;
	this->y += y;


	top.a.x += x;
	top.a.y += y;
	top.b.x += x;
	top.b.y += y;


	down.a.x += x;
	down.a.y += y;
	down.b.x += x;
	down.b.y += y;


	right.a.x += x;
	right.a.y += y;
	right.b.x += x;
	right.b.y += y;


	left.a.x += x;
	left.a.y += y;
	left.b.x += x;
	left.b.y += y;

}



Block::Block() : colour(nullptr, SDL_DestroyTexture) {}


Block::Block(SDL_Renderer *ren, std::string colour_filename,
	std::array<std::array<int, 4>, 4> block_array) :
	colour(IMG_LoadTexture(ren, colour_filename.c_str()), SDL_DestroyTexture)
{

	// Block is constructed at default location.
	ParseBlockArray(ren, NULL, NULL, block_array);

	current_dir = UP; // UP is set as the default direction for each block spawned.

}


Block::Block(const Block &b) : colour(b.colour)
{

	x = b.x;

	y = b.y;

	block_squares = b.block_squares;

	current_dir = b.current_dir;

}


void Block::ParseBlockArray(SDL_Renderer *ren, int loc_x, int loc_y,
	std::array<std::array<int, 4>, 4> block_array)
{
		
	block_arr = block_array;

	for (int i = 0; i < 4; ++i)
	{

		for (int x = 0; x < 4; ++x)
		{

			if (block_array[i][x] == 1)
			{
				
				SDL_Rect dest;

				if (loc_x == NULL && loc_y == NULL)
				{
					// 192 and 128 are the offsets used to centre the block off screen.
					// They are only set if a particular location isn't entered into the
					// function.
					dest.x = x * 32 + 192;
					dest.y = i * 32 - 128;

				}
				else
				{

					dest.x = loc_x;
					dest.y = loc_y;

				}

				SDL_QueryTexture(colour.get(), NULL, NULL, &dest.w, &dest.h);
				
				block_squares.push_back(Square(dest, colour.get()));
				
			}

		}

	}
	
}

void Block::UpdateSquares(int x, int y)
{

	for (unsigned int i = 0; i < block_squares.size(); ++i)
	{

		block_squares[i].Update(x, y);

	}

}


void Block::Rotate()
{

	// TODO: Must figure out a rotation algorithm.

	std::array<std::array<int, 4>, 4> rotated_arr;

	for (int i = 0; i < 4; ++i)
	{

		for (int x = 0; x < 4; ++x)
		{



		}

	}

}


bool Utilities::CompareLines(Line one, Line two)
{

	if (one.a.x == two.a.x
		&& one.a.y == two.a.y
		&& one.b.y == two.b.y
		&& one.b.x == two.b.x)
	{
		return true;
	}
	else return false;

}


void Utilities::RenderTexture(SDL_Renderer *ren, 
	SDL_Texture *tex, int x, int y)
{

	SDL_Rect dest;

	dest.x = x;

	dest.y = y;

	SDL_QueryTexture(tex,
		NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(ren, tex, NULL, &dest);

}


void Utilities::DrawLines(SDL_Renderer *ren, std::vector<Line> lines)
{

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	for (unsigned int i = 0; i < lines.size(); ++i)
	{

		SDL_RenderDrawLine(ren, lines[i].a.x, lines[i].a.y,
			lines[i].b.x, lines[i].b.y);

	}

}

