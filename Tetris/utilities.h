

#include "constants.h"

#ifndef UTILITIES_H_
#define UTILITIES_H_



struct Point
{

	int x;


	int y;

};


struct Line
{

	Point a;


	Point b;


	Line();


	Line(int ax, int ay, int bx, int by);

};


struct Square
{

	SDL_Texture *tex;


	int x;


	int y;


	Line top;

	
	Line down;


	Line left;


	Line right;


	Square(int pos_x, int pos_y, SDL_Texture *tex, 
		Line t, Line d, Line l, Line r);


	void Update(int x, int y);


};



struct Block
{

	int x;


	int y;


	SDL_Texture *colour;


	std::vector<Square> block_squares;


	std::vector<Square> block_squares_r;


	std::vector<Square> block_squares_r2;


	std::vector<Square> block_squares_r3;


	std::vector<Square> current_squares;


	Block();


	Block(SDL_Renderer *ren, std::string colour_filename,
		std::string block_filename, std::string rotate_one,
		std::string rotate_two, std::string rotate_three);


	void SetCurrentSquares(std::vector<Square> squares);


	std::vector<Square> ParseBlockFile(SDL_Renderer *ren, std::string block_filename);


	void UpdateSquares(int x, int y);


	std::vector<Square> ReturnFlip(std::vector<Square> squares, bool hv);

};



class Utilities
{


public:


	static bool CompareLines(Line one, Line two);


	static void RenderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);


	static void DrawLines(SDL_Renderer *ren, std::vector<Line> lines);


};



#endif // UTILITIES_H_