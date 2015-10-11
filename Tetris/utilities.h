

#ifndef UTILITIES_H_
#define UTILITIES_H_


#include <SDL.h>
#include <SDL_image.h>


#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <array>


// Points and lines could be removed if rects
// were utilised instead.

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


	void PrintLineInfo();

};


class Square
{


public:


	// std::shared_ptr<SDL_Texture> tex;


	// SDL_Texture* tex;

	
	int x;


	int y;


	// Each square has lines that are used to check collision.
	// TODO: change lines and use rectangles for collision.
	Line top;

	
	Line down;


	Line left;


	Line right;


	Square(SDL_Rect rect, SDL_Texture *tex);


	Square(const Square &s);


	void Update(int x, int y);


	void PrintLocation();


	void PrintSquareInfo();


};


// TODO: Seperate out into a component architecture.
class Block
{


public:
	

	// Stores the x and y locations of the top 
	// left corner of the current block.
	int x;


	int y;


	// std::shared_ptr<SDL_Texture> colour;


	SDL_Texture* colour;


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


	void Copy2DArray(std::array<std::array<int, 4>, 4> input);


	// Updates block squares based on the given array.
	// Sets block to default position if loc_x and loc_y are
	// set to NULL.
	void ParseBlockArray(SDL_Renderer *ren,
		std::array<std::array<int, 4>, 4> block_array);


	// Updates the location of each of the squares 
	// by the x and y amounts passed in.
	void UpdateSquares(int x, int y);


	void Rotate(SDL_Renderer* ren);


	int GetLocX();


	int GetLocY();


};


// TODO: Remove utilities. Rendering should be moved to a graphics component
// created for blocks.
class Utilities
{


public:


	static bool CompareLines(Line one, Line two);


	static void RenderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);


	static void DrawLines(SDL_Renderer *ren, std::vector<Line> lines);


};



#endif // UTILITIES_H_