

#include "utilities.h"
#include "core.h"


Line::Line()
{
	a.x = 0;
	a.y = 0;
	b.x = 0;
	b.y = 0;
};


Line::Line(int ax, int ay, int bx, int by)
{
	a.x = ax;
	a.y = ay;
	b.x = bx;
	b.y = by;
}


void Line::PrintLineInfo()
{
	std::cout << a.x << ", " << a.y << 
		", " << b.x << ", " << b.y << "\n";
}


Square::Square(SDL_Rect rect, SDL_Texture *tex) 
{
	this->tex = tex; // TODO: Fixme - At a glance I think this texture leaks

	x = rect.x;
	y = rect.y;
	
	top = Line(rect.x, rect.y, rect.x + rect.w, rect.y);
	down = Line(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
	left = Line(rect.x, rect.y, rect.x, rect.y + rect.h);
	right = Line(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
}


Square::Square(const Square &s) 
{
	tex = s.tex;
	x = s.x;
	y = s.y;
	top = s.top;
	down = s.down;
	left = s.left;
	right = s.right;
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


void Square::PrintLocation()
{
	std::cout << "Current square location " 
		<< x << ", " << y << "\n";
}


void Square::PrintSquareInfo()
{
	PrintLocation();
	
	std::cout << "Top: ";
	top.PrintLineInfo();

	std::cout << "Down: ";
	down.PrintLineInfo();

	std::cout << "Right: ";
	right.PrintLineInfo();

	std::cout << "Left: ";
	left.PrintLineInfo();
}


Block::Block() //: colour(nullptr, SDL_DestroyTexture)
{
	block_arr.fill({ 0, 0, 0, 0 });

	x = 0;
	y = 0;
	colour = nullptr;
	current_dir = 0;
}


void Print2DArray(std::array<std::array<int, 4>, 4> arr)
{

	for (int i = 0; i < 4; ++i)
	{

		for (int z = 0; z < 4; ++z)
		{

			std::cout << arr[i][z] << " ";

		}

		std::cout << "\n";

	}

}


/*
* Quick and dirty function to avoid needing colour files loaded
*/
std::unique_ptr<SDL_Surface, void(*)(SDL_Surface *)> ColourFromFilename(const std::string& name)
{
    auto ret_surface = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface *)>(SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0), SDL_FreeSurface);

    Uint32 colour_pixel = 0;

    if (name == "green")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 0, 255, 0);
    }
    else if (name == "red")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 255, 0, 0);
    }
    else if (name == "blue")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 0, 0, 255);
    }
    else if (name == "orange")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 165, 0, 255);
    }
    else if (name == "purple")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 128, 128, 0);
    }
    else if (name == "cyan")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 127, 127, 255);
    }
    else if (name == "yellow")
    {
        colour_pixel = SDL_MapRGB(ret_surface->format, 255, 0, 255);
    }

    SDL_FillRect(ret_surface.get(), NULL, colour_pixel);
    return ret_surface;

}


Block::Block(SDL_Renderer *ren, std::string colour_filename,
	std::array<std::array<int, 4>, 4> block_array, int startPoint) 
	// : colour(IMG_LoadTexture(ren, colour_filename.c_str()), SDL_DestroyTexture)
{
    auto surface = ColourFromFilename(colour_filename);
    if (surface == nullptr)
    {
        const char *err = SDL_GetError();
    }

    colour = SDL_CreateTextureFromSurface(ren, surface.get());

	// 192 and 128 are the offsets used to centre the block off screen.
	// They are only set if a particular location isn't entered into the
	// function

	this->x = startPoint;
	this->y = -128;

	// Block is constructed at default location.
	ParseBlockArray(ren, block_array);

	current_dir = UP; // UP is set as the default direction for each block spawned.
}


Block::Block(const Block &b) //: colour(b.colour)
{

	this->x = b.x;

	this->y = b.y;

	Copy2DArray(b.block_arr);

	block_squares = b.block_squares;

	current_dir = b.current_dir;

    this->colour = b.colour;
	/*
	if (b.colour == nullptr)
	{

		std::cout << "Error copying block colour texture.\n";

	}*/

}


void Block::Copy2DArray(std::array<std::array<int, 4>, 4> input)
{

	for (int i = 0; i < 4; ++i)
	{

		for (int x = 0; x < 4; ++x)
		{

			block_arr[i][x] = input[i][x];

		}

	}

}


void Block::ParseBlockArray(SDL_Renderer *ren,
	std::array<std::array<int, 4>, 4> block_array)
{

	Copy2DArray(block_array); 
	// back up the array so blocks can be reverted 
	// if collision is detected.

	block_squares = std::vector<Square>();

	SDL_Rect dest;

	dest.w = 32;
	dest.h = 32;

	for (int i = 0; i < 4; ++i)
	{

		for (int x = 0; x < 4; ++x)
		{

			if (block_array[i][x] == 1)
			{
			
				dest.x = x * 32 + this->x;
				dest.y = i * 32 + this->y;
				
				block_squares.push_back(Square(dest, /*colour.get()*/colour));
				
			}

		}

	}
	
}


void Block::UpdateSquares(int x, int y)
{

	std::cout << "Moved down by: " << y << "\n";

	for (unsigned int i = 0; i < block_squares.size(); ++i)
	{

		block_squares[i].Update(x, y);
	
	}
	
	this->x += x;
	this->y += y;

}


void Block::Rotate(SDL_Renderer* ren)
{

	// TODO: Must figure out a rotation algorithm.

	// Should cache block_arr locally so that after
	// collision is checked the block can be reverted back
	// if necessary.

	std::array<std::array<int, 4>, 4> rotated_arr;

	rotated_arr[0][3] = block_arr[0][0];
	rotated_arr[1][3] = block_arr[0][1];
	rotated_arr[2][3] = block_arr[0][2];
	rotated_arr[3][3] = block_arr[0][3];

	rotated_arr[0][2] = block_arr[1][0];
	rotated_arr[1][2] = block_arr[1][1];
	rotated_arr[2][2] = block_arr[1][2];
	rotated_arr[3][2] = block_arr[1][3];

	rotated_arr[0][1] = block_arr[2][0];
	rotated_arr[1][1] = block_arr[2][1];
	rotated_arr[2][1] = block_arr[2][2];
	rotated_arr[3][1] = block_arr[2][3];

	rotated_arr[0][0] = block_arr[3][0];
	rotated_arr[1][0] = block_arr[3][1];
	rotated_arr[2][0] = block_arr[3][2];
	rotated_arr[3][0] = block_arr[3][3];

	ParseBlockArray(ren, rotated_arr);
	/*
	for (int i = 0; i < block_squares.size(); ++i)
	{

		block_squares[i].PrintLocation();

	}*/
	
}


int Block::GetLocX()
{

	return this->x;

}


int Block::GetLocY()
{

	return this->y;

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

	if (tex == nullptr)
	{

		std::cout << "A null texture has been passed to be rendered.\n";

	}
	else
	{

		SDL_Rect dest;

		dest.x = x;

		dest.y = y;

        dest.w = 32;

        dest.h = 32;

		//SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

		SDL_RenderCopy(ren, tex, NULL, &dest);

	}

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

