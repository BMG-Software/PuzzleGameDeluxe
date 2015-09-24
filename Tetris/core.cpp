

#include "core.h"


std::vector<SDL_Rect> Game::number_clips;


void Game::InitWinAndRen()
{

	win = SDL_CreateWindow("Tetris", 
		SDL_WINDOWPOS_CENTERED, 50,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);


	ren = SDL_CreateRenderer(win, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (win == nullptr || ren == nullptr)
	{

		std::cout << "Problem initialising window and/or renderer.\n";
		
	}

}


Game::Game()
{

	InitWinAndRen();

	numbers = IMG_LoadTexture(ren, "Resources\\numbers.png");
	
	for (int i = 0; i < 320; i += 32)
	{
		SDL_Rect temp;
		temp.x = i;
		temp.y = 0;
		temp.w = 32;
		temp.h = 32;

		number_clips.push_back(temp);
	}

	score = 00000000;

	gen_block = true;

}


bool Game::EventLoop()
{

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{

		if (e.key.keysym.sym == SDLK_ESCAPE)
		{

			return true;

		}

	}

	return false;

}


void Game::DrawScore()
{

	std::string score_string;

	std::stringstream my_stream;

	my_stream << score;
	my_stream >> score_string;

	for (int i = 0; i < score_string.size(); ++i)
	{

		PrintScore(score_string[i], number_clips[i]);

	}



}


void Game::PrintScore(char number, SDL_Rect location)
{

	// Just convert the char into an int...

	if (number == '0') SDL_RenderCopy(ren, numbers,
		&number_clips[0], &location);
	else if (number == '1')	SDL_RenderCopy(ren, numbers, 
		&number_clips[1], &location);
	else if (number == '2')	SDL_RenderCopy(ren, numbers, 
		&number_clips[2], &location);
	else if (number == '3')	SDL_RenderCopy(ren, numbers,
		&number_clips[3], &location);
	else if (number == '4')	SDL_RenderCopy(ren, numbers,
		&number_clips[4], &location);
	else if (number == '5')	SDL_RenderCopy(ren, numbers, 
		&number_clips[5], &location);
	else if (number == '6')	SDL_RenderCopy(ren, numbers,
		&number_clips[6], &location);
	else if (number == '7')	SDL_RenderCopy(ren, numbers, 
		&number_clips[7], &location);
	else if (number == '8') SDL_RenderCopy(ren, numbers, 
		&number_clips[8], &location);
	else if (number == '9')	SDL_RenderCopy(ren, numbers, 
		&number_clips[9], &location);

}


void Game::Run()
{

	BlockControl controller(ren);

	Board game_board(ren);

	while (!EventLoop())
	{

		SDL_SetRenderDrawColor(ren, 127, 127, 127, 255);
		
		SDL_RenderClear(ren);
		
		controller.MoveBlock(game_board.board_squares);
		
		SDL_Delay(0100);
		
		if (gen_block)
		{

			controller.GenerateRandomBlock();
			// Centre the block.
			gen_block = false;
		
		}


		if (controller.DrawBlock(ren, game_board.board_squares))
		{

			game_board.AddToBoard(controller.GetCurrentBlock(), score);

			gen_block = true;

		}

		if (game_board.DrawBoardBlocks(ren))
		{

			// Display with true type font

			std::cout << " GAME OVER ! \n"; 

			// remove for linux support

			system("pause");

			break;

		}
		
		DrawScore();
		
		SDL_RenderPresent(ren);

	}

}