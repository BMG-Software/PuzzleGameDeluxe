

#ifndef UTILITIES_H_
#define UTILITIES_H_


#include <SDL.h>
#include <SDL_image.h>


#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <array>


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


class Square
{


public:


	std::shared_ptr<SDL_Texture> tex;


	int x;


	int y;


	// Each square has lines that are used to check collision.
	Line top;

	
	Line down;


	Line left;


	Line right;


	// Needs updating. Lines can be figured out from the x and y positions.
	Square(SDL_Rect rect, SDL_Texture *tex/*, 
		Line t, Line d, Line l, Line r*/);


	void Update(int x, int y);


};



class Block
{


public:



	// Stores the x and y locations of the top 
	// left corner of the current block.
	int x;


	int y;


	std::shared_ptr<SDL_Texture> colour;


	// Each block saves a copy of it's own array which
	// is later used for rotations.
	std::array<std::array<int, 4>, 4> block_arr;


	// Stores all of the squares that make up the current block.
	std::vector<Square> block_squares;


	static enum directions { UP, DOWN, LEFT, RIGHT };


	// Stores the current direction of the block.
	int current_dir;
	

	Block();


	Block(SDL_Renderer *ren, std::string colour_filename,
		std::array<std::array<int, 4>, 4> block_array);


	Block(const Block &b);


	// Updates block squares based on the given array.
	// Sets block to default position if loc_x and loc_y are
	// set to NULL.
	void ParseBlockArray(SDL_Renderer *ren, int loc_x, int loc_y,
		std::array<std::array<int, 4>, 4> block_array);


	// Updates the location of each of the squares 
	// by the x and y amounts passed in.
	void UpdateSquares(int x, int y);


	void Rotate();


};



class Utilities
{


public:


	static bool CompareLines(Line one, Line two);


	static void RenderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);


	static void DrawLines(SDL_Renderer *ren, std::vector<Line> lines);


};



#endif // UTILITIES_H_