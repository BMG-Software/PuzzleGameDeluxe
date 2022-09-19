

#include "core.h"

// Constants
const char * const BACKGROUND_FILENAME = ".\\Resources\\background.bmp";
const int BOARD_WIDTH = 448;

// Static members
std::vector<SDL_Rect> Game::number_clips;

int Game::m_windowWidth = 0;
int Game::m_windowHeight = 0;

// Member functions
void Game::InitWinAndRen(bool fullscreen)
{
	win.reset(SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, 50, m_windowWidth, m_windowHeight, 
        (fullscreen) ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN));
    ren.reset(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED));

	if (win == nullptr || ren == nullptr)
	{
		std::cout << "Problem initialising window and/or renderer.\n";	
	}
}

Game::Game(bool fullscreen, int windowWidth, int windowHeight) : 
    win(nullptr, SDL_DestroyWindow),
	ren(nullptr, SDL_DestroyRenderer),
	numbers(nullptr, SDL_DestroyTexture),
    background(nullptr, SDL_DestroyTexture),
    board_background(nullptr, SDL_DestroyTexture)
{
    m_windowWidth   = windowWidth;
    m_windowHeight  = windowHeight;

	InitWinAndRen(fullscreen);

	// numbers.reset(IMG_LoadTexture(ren.get(), "Resources\\numbers.png"));
	
	for (int i = 0; i < 320; i += 32)
	{
        number_clips.push_back({ i, 0, 32, 32 });
	}

	score               = 00000000; // Ha why so many zeroes :D
    game_controller     = nullptr;
    joystick            = nullptr;
    control_direction   = BlockControl::block_direction_down;

    auto bg_surface = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(SDL_LoadBMP(BACKGROUND_FILENAME), SDL_FreeSurface);
    background      = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(SDL_CreateTextureFromSurface(ren.get(), bg_surface.get()), SDL_DestroyTexture);

    SDL_QueryTexture(background.get(), NULL, NULL, &background_src.w, &background_src.h);

    background_src.x = 0;
    background_src.y = 0;

    background_dest = { 
        0, 
        0, 
        windowWidth, 
        windowHeight 
    };

    board_background    = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)>(SDL_CreateTexture(ren.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth / 2, windowHeight), SDL_DestroyTexture);
    int boardOffset = static_cast<int>(windowWidth * 0.05);
    m_p1BoardDest       = { 
        boardOffset, 
        0, 
        BOARD_WIDTH, 
        windowHeight 
    };

    m_p2BoardDest = {
        windowWidth - BOARD_WIDTH - boardOffset,
        0,
        BOARD_WIDTH,
        windowHeight
    };

    SDL_SetTextureBlendMode(board_background.get(), SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderTarget(ren.get(), board_background.get());
    SDL_SetRenderDrawBlendMode(ren.get(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren.get(), 127, 127, 127, 127);
    SDL_RenderFillRect(ren.get(), NULL);

    SDL_SetRenderTarget(ren.get(), NULL);
}

Game::~Game() = default;


bool Game::EventLoop()
{

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE || e.type == SDL_QUIT)
		{
			return true;
		}
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_UP)
            {
                control_direction = BlockControl::block_direction_up; // UP
            }
            else if (e.key.keysym.sym == SDLK_DOWN)
            {
                control_direction = BlockControl::block_direction_down;
            }
            else if (e.key.keysym.sym == SDLK_LEFT)
            {
                control_direction = BlockControl::block_direction_left;
            }
            else if (e.key.keysym.sym == SDLK_RIGHT)
            {
                control_direction = BlockControl::block_direction_right;
            }
        }
        else if (e.type == SDL_CONTROLLERDEVICEADDED) // Sets up the xbox controller
        {
            for (int i = 0; i < SDL_NumJoysticks(); i++)
            {
                game_controller = SDL_GameControllerOpen(i);
                if (game_controller)
                {
                    joystick = SDL_JoystickOpen(i);
                    if (joystick)
                    {
                        return false;
                    }

                }
            }

        }
#if 0
        // Analogue stick doesn't move blocks when pushed fully in any direction
        else if (e.type == SDL_JOYAXISMOTION)
        {

            int value = e.jaxis.value; //SDL_JoystickGetAxis(joystick, e.jaxis.which);

            if (e.jaxis.axis == 0)
            {
                if (value < -3200)
                {
                    control_direction = BlockControl::block_direction_left; // LEFT
                }
                else if (value > 3200)
                {
                    control_direction = BlockControl::block_direction_right; // RIGHT
                }
            }
            else if (e.jaxis.axis == 1)
            {
                if (value < -3200)
                {
                    control_direction = BlockControl::block_direction_up; // UP
                }
                else if (value > 3200)
                {
                    control_direction = BlockControl::block_direction_down; // DOWN
                }
            }
        }
#else
        else if (e.type == SDL_JOYHATMOTION) // DPad works like a dream
        {
            if (e.jhat.value & SDL_HAT_UP)
            {
                control_direction = BlockControl::block_direction_up; // UP
            }
            else if (e.jhat.value & SDL_HAT_DOWN)
            {
                control_direction = BlockControl::block_direction_superdown; // DOWN
            }
            else if (e.jhat.value & SDL_HAT_LEFT)
            {
                control_direction = BlockControl::block_direction_left; // LEFT
            }
            else if (e.jhat.value & SDL_HAT_RIGHT)
            {
                control_direction = BlockControl::block_direction_right; // RIGHT
            }
        }

#endif

	}

	return false;

}


void Game::DrawScore()
{
	std::string score_string;

	std::stringstream my_stream;

	my_stream << score;
	my_stream >> score_string;

	for (size_t i = 0; i < score_string.size(); ++i)
	{
		PrintScore(score_string[i], number_clips[i]);
	}
}


void Game::PrintScore(char number, SDL_Rect location)
{
	std::stringstream convertor;
	convertor << number;
	int my_num = 0;
	convertor >> my_num;

	// SDL_RenderCopy(ren.get(), numbers.get(), &number_clips[my_num], &location);
}


void Game::CheckForGenBlock(BlockControl &controller, bool &inOutGenBlock)
{
	// checks if a new block should be generated
	if (inOutGenBlock)
	{
		controller.GenerateRandomBlock();
		inOutGenBlock = false;
	}
}


bool Game::DrawAndCheckBoardAddition(float frame_time, BlockControl &controller, Board &gameBoard)
{
	// Draws the block at it's current location - 
	// returns true if the block has collided with another.
	if (controller.DrawBlock(ren.get(), gameBoard.board_squares, frame_time))
	{
		gameBoard.AddToBoard(controller.GetCurrentBlock(), score);
		return true;
	}
    return false;
}

void Game::Run()
{
    bool genBlock = true;
	float frame_time = 0.0;

    BlockControl p1Controller(ren.get(), m_p1BoardDest);
    BlockControl p2Controller(ren.get(), m_p2BoardDest);

    Board p1GameBoard(ren.get(), m_p1BoardDest);
    Board p2GameBoard(ren.get(), m_p2BoardDest);
    
	while (!EventLoop())
	{
		frame_timer.StartTimer();

		SDL_SetRenderDrawColor(ren.get(), 127, 127, 127, 255);	
		SDL_RenderClear(ren.get());

        SDL_RenderCopy(ren.get(), background.get(), &background_src, &background_dest);
        SDL_RenderCopy(ren.get(), board_background.get(), &m_p1BoardDest, &m_p1BoardDest);
        SDL_RenderCopy(ren.get(), board_background.get(), &m_p1BoardDest, &m_p2BoardDest);
	
		p1Controller.MoveBlock(ren.get(), p1GameBoard.board_squares, control_direction, frame_time);
		
		SDL_Delay(0017); // aim for 60 fps
		
		CheckForGenBlock(p1Controller, genBlock);
		
		genBlock = DrawAndCheckBoardAddition(frame_time, p1Controller, p1GameBoard);

		if (p1GameBoard.DrawBoardBlocks(ren.get())) 
            break;
		
        DrawScore();

		SDL_RenderPresent(ren.get());

		frame_timer.StopTimer();
		frame_time = frame_timer.GetTimeSeconds();

        control_direction = BlockControl::block_direction_down;	
	}

}