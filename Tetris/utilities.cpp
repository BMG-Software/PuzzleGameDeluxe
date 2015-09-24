

#include "utilities.h"


Line::Line(){};


Line::Line(int ax, int ay, int bx, int by)
{

	a.x = ax;

	a.y = ay;

	b.x = bx;

	b.y = by;

}


Square::Square(int pos_x, int pos_y, SDL_Texture *tex,
	Line t, Line d, Line l, Line r)
{

	x = pos_x;

	y = pos_y;

	this->tex = tex;

	top = t;

	down = d;

	left = l;

	right = r;

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



Block::Block(){}


Block::Block(SDL_Renderer *ren, std::string colour_filename,
	std::string block_filename, std::string rotate_one,
	std::string rotate_two, std::string rotate_three)
{

	colour = IMG_LoadTexture(ren, colour_filename.c_str());

	block_squares = ParseBlockFile(ren, block_filename);

	block_squares_r = ParseBlockFile(ren, rotate_one);

	block_squares_r2 = ParseBlockFile(ren, rotate_two);

	block_squares_r3 = ParseBlockFile(ren, rotate_three);

}



void Block::SetCurrentSquares(std::vector<Square> squares)
{

	current_squares = squares;

}



std::vector<Square> Block::ParseBlockFile(SDL_Renderer *ren, std::string block_filename)
{

	std::vector<Square> temp;

	int block[4][4];
	
	std::ifstream my_stream(block_filename);

	if (!my_stream.is_open())
	{
		std::cout << "Error opening " << block_filename << "\n";
	}

	std::string block_string;

	while (!my_stream.eof())
	{
		std::string temp;
		my_stream >> temp;
		block_string += temp;
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (block_string[x + (i * 4)] == '1')
			{
				SDL_Rect dest;
				dest.x = x * 32 + 192;
				dest.y = i * 32 - 128;

				SDL_QueryTexture(colour, NULL, NULL, &dest.w, &dest.h);

				temp.push_back(Square(dest.x, dest.y, colour, 
					Line(dest.x, dest.y, dest.x + dest.w, dest.y),
					Line(dest.x, dest.y + dest.h, dest.x + dest.w, dest.y + dest.h),
					Line(dest.x, dest.y, dest.x, dest.y + dest.h),
					Line(dest.x + dest.w, dest.y, dest.x + dest.w, dest.y + dest.h)));
				
			}

		}

	}

	return temp;

}

void Block::UpdateSquares(int x, int y)
{
	for (int i = 0; i < current_squares.size(); ++i)
	{

		current_squares[i].Update(x, y);

	}

	for (int i = 0; i < block_squares.size(); ++i)
	{

		block_squares[i].Update(x, y);

	}

	for (int i = 0; i < block_squares_r.size(); ++i)
	{

		block_squares_r[i].Update(x, y);

	}

	for (int i = 0; i < block_squares_r2.size(); ++i)
	{

		block_squares_r2[i].Update(x, y);

	}

	for (int i = 0; i < block_squares_r3.size(); ++i)
	{

		block_squares_r3[i].Update(x, y);

	}

}



std::vector<Square> Block::ReturnFlip(std::vector<Square> squares, bool hv)
{
	
	for (int i = 0; i < squares.size(); ++i)
	{
		if (!hv)
		{
			if (squares[i].x == x)
			{
				squares[i].x = x + 96;
			}
			if (squares[i].x == x + 32)
			{
				squares[i].x = x + 64;
			}
			if (squares[i].x ==  x + 64)
			{
				squares[i].x =  x + 32;
			}
			if (squares[i].x == x + 96)
			{
				squares[i].x = x;
			}
		}
		else
		{
			if (squares[i].y == y)
			{
				squares[i].y = y + 96;
			}
			if (squares[i].y == y + 32)
			{
				squares[i].y = y + 64;
			}
			if (squares[i].y == y + 64)
			{
				squares[i].y = y + 32;
			}
			if (squares[i].y == y + 96)
			{
				squares[i].y = y;
			}

		}


	}
	return squares;

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

	for (int i = 0; i < lines.size(); ++i)
	{

		SDL_RenderDrawLine(ren, lines[i].a.x, lines[i].a.y,
			lines[i].b.x, lines[i].b.y);

	}

}
