#include <iostream>
#include <SDL.h>
#include <cmath>
#include <SDL_image.h>
#include <ctime>
#include <vector>
#include "mine.h"
#include <SDL_mixer.h>
#include "animation.h"
using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Mine Sweeper!!!";

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
void refreshScreen(SDL_Renderer* renderer, vector< vector<int> >& matrix, vector< vector<int> >& matrix1, int r, int c, SDL_Texture* image_texture, SDL_Texture* surface_texture, const bool leftMouse);
int checkWin(const vector< vector<int> >& matrix, int cnt);
void logSDLError(std::ostream& os,
	const std::string& msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();


int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	initSDL(window, renderer);
	IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);
	SDL_Texture* surface_texture = IMG_LoadTexture(renderer, "whitescreen.jpg");
	SDL_Texture* image_texture = IMG_LoadTexture(renderer, "bomb.png");
	SDL_Texture* game = IMG_LoadTexture(renderer, "game.png");
	SDL_Texture* start = IMG_LoadTexture(renderer, "start.png");
	SDL_Texture* helpp = IMG_LoadTexture(renderer, "help.png");
	SDL_Texture* creditss = IMG_LoadTexture(renderer, "credits.png");
	SDL_Texture* loser = IMG_LoadTexture(renderer, "loser.png");
	SDL_Texture* winner = IMG_LoadTexture(renderer, "winner.png");
	SDL_RenderClear(renderer);

	Mix_Music* gMusic = NULL;
	Mix_Chunk* gClick = NULL;
	Mix_Chunk* gFlag = NULL;
	Mix_Chunk* gUnFlag = NULL;
	Mix_Chunk* gOver = NULL;
	Mix_Chunk* gWin = NULL;
	gMusic = Mix_LoadMUS("music.ogg");
	if (gMusic == NULL) cout << "Music error: %s\n" << Mix_GetError();
	gClick = Mix_LoadWAV("click.wav");
	if (gClick == NULL) cout << "Click sound error: %s\n" << Mix_GetError();
	gFlag = Mix_LoadWAV("flag.wav");
	if (gFlag == NULL) cout << "Flag sound error: %s\n" << Mix_GetError();
	gOver = Mix_LoadWAV("gameOver.wav");
	if (gOver == NULL) cout << "Lost sound error: %s\n" << Mix_GetError();
	gWin = Mix_LoadWAV("gameWin.wav");
	if (gWin == NULL) cout << "Win sound error: %s\n" << Mix_GetError();

	int r, c, rows = SCREEN_HEIGHT / 20, columns = SCREEN_WIDTH / 20;
	int status = nothing;
	int cnt, opened;
	vector< vector<int> > matrix(rows, vector<int>(columns));
	vector< vector<int> > matrix1(rows, vector<int>(columns));
	SDL_Rect rect;
	rect.h = 20;
	rect.w = 20;
	srand(time(0));
	do
	{
		int i = 0;
		Mix_PlayMusic(gMusic, 0);
		do
		{
			if (status == nothing)
			{
				SDL_RenderCopy(renderer, game, NULL, NULL);
				SDL_RenderPresent(renderer);
			}	
			SDL_Delay(1);
			if (SDL_WaitEvent(&e) == 0) continue;
			if (e.type == SDL_QUIT) return 0;
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = e.button.x;
				int y = e.button.y;
				if (status == nothing)//nothing has been done
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{   //Button
						if (x <= 340 && x >= 261)
						{
							if (y >= 397 && y <= 439) return 0;//quit
							else if (y >= 286 && y <= 327)
							{
								SDL_RenderCopy(renderer, helpp, NULL, NULL);
								SDL_RenderPresent(renderer);
								status = help;
							}
							else if (y >= 342 && y <= 382)
							{
								SDL_RenderCopy(renderer, creditss, NULL, NULL);
								SDL_RenderPresent(renderer);
								status = credits;
							}
							else if (y >= 227 && y <= 270)
							{
								SDL_RenderCopy(renderer, start, NULL, NULL);
								SDL_RenderPresent(renderer);
								status = choose;
							}
						}
					}
				}
				else if (status == choose)//enter choose mode
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						if (x >= 214 && x <= 377)
						{
							if (y >= 237 && y <= 280)
							{
								status = easy;
							}
							else if (y >= 304 && y <= 347)
							{
								status = medium;
							}
							else if (y >= 374 && y <= 415)
							{
								status = hard;
							}
							else if (y >= 441 && y <= 482)
							{
								status = superUltraHard;
							}
							if (status != choose)
							{
								SDL_RenderCopy(renderer, surface_texture, NULL, NULL);
								for (int i = 0; i < 30; i++)
									for (int j = 0; j < 30; j++)
									{
										rect.x = i * 20;
										rect.y = j * 20;
										SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
										SDL_RenderDrawRect(renderer, &rect);
									}
								SDL_RenderPresent(renderer);
								setMines(matrix, status);
								cnt = countMinesInMatrix(matrix);
								for (unsigned int i = 0; i < matrix.size(); i++)
								{
									for (unsigned int j = 0; j < matrix[0].size(); j++)
									{
										matrix1[i][j] = matrix[i][j];
									}
								}
							}
						}
					}
				}
				else if (status <= easy && status >= superUltraHard)//when game is on
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						c = x / 20;
						r = y / 20;
						if (matrix[r][c] == mine)
						{
							refreshScreen(renderer, matrix, matrix1, r, c, image_texture, surface_texture, true);
							for (unsigned int i = 0; i < matrix.size(); i++)
							{
								for (unsigned int j = 0; j < matrix[0].size(); j++)
								{
									matrix[i][j] = lose;
								}
							}
							status = lose;
							Mix_PauseMusic();
							Mix_PlayChannel(-1, gOver, 0);
							SDL_Delay(1000);
							explosion(renderer);
						}
						else if (matrix[r][c] != lose && matrix[r][c] != win)
						{
							Mix_PlayChannel(-1, gClick, 0);
							refreshScreen(renderer, matrix, matrix1, r, c, image_texture, surface_texture, true);
							opened = checkWin(matrix, cnt);
							SDL_RenderPresent(renderer);
							if (opened + cnt == 30 * 30)
							{
								for (unsigned int i = 0; i < matrix.size(); i++)
								{
									for (unsigned int j = 0; j < matrix[0].size(); j++)
									{
										matrix[i][j] = win;
									}
								}
								status = win;
								Mix_PauseMusic();
								Mix_PlayChannel(-1, gWin, 0);
							}
						}
					}
					else
					{
						Mix_PlayChannel(-1, gFlag, 0);
						c = x / 20;
						r = y / 20;
						refreshScreen(renderer, matrix, matrix1, r, c, image_texture, surface_texture, false);
					}
				}
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (((status <= easy && status >= superUltraHard) || status >= credits && status <= choose) && e.key.keysym.sym == SDLK_ESCAPE)
					break;
				if (!i)
				{
					if (status == win)
					{
						SDL_RenderCopy(renderer, winner, NULL, NULL);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1, gWin, 0);
						i++;
					}
					else if (status == lose)
					{
						SDL_RenderCopy(renderer, loser, NULL, NULL);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1, gOver, 0);
						i++;
					}
				}
				else  status = playAgain;
			}
		} while (status != playAgain);
		status = nothing;
	} while (status);

	SDL_DestroyTexture(image_texture);
	SDL_DestroyTexture(surface_texture);
	SDL_DestroyTexture(game);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gFlag);
	Mix_FreeChunk(gOver);
	Mix_FreeChunk(gWin);
	Mix_FreeMusic(gMusic);
	gClick = NULL;
	gFlag = NULL;
	gOver = NULL;
	gWin = NULL;
	gMusic = NULL;
	IMG_Quit();
	Mix_Quit();
	quitSDL(window, renderer);
	return 0;
}

void refreshScreen(SDL_Renderer* renderer, vector< vector<int> >& matrix, vector< vector<int> >& matrix1, int r, int c, SDL_Texture* image_texture, SDL_Texture* surface_texture, const bool leftMouse)
{
	SDL_Rect srcrect;
	srcrect.y = 0;
	srcrect.w = 64;
	srcrect.h = 64;
	SDL_Rect rect;
	rect.x = c * 20;
	rect.y = r * 20;
	rect.h = 20;
	rect.w = 20;
	if (leftMouse)
	{
		int mines = countMines(matrix, r, c);
		srcrect.x = mines * 64;
		if (matrix[r][c] == mine)//if a mine was clicked, draw mine.
		{
			for (unsigned int i = 0; i < matrix.size(); i++)
			{
				for (unsigned int j = 0; j < matrix[0].size(); j++)
				{
					if (matrix[i][j] == mine)
					{
						rect.x = j * 20;
						rect.y = i * 20;
						SDL_RenderCopy(renderer, image_texture, &srcrect, &rect);
					}
				}
			}
			SDL_RenderPresent(renderer);
		}
		else if (matrix[r][c] == flag || matrix[r][c] == clicked || matrix[r][c] == lose) return;
		else
		{
			SDL_RenderCopy(renderer, image_texture, &srcrect, &rect);
			matrix[r][c] = clicked;
			if (mines == 0)
			{
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (r + i >= 0 && r + i < matrix.size() && c + j >= 0 && c + j < matrix[0].size())
						{
							refreshScreen(renderer, matrix, matrix1, r + i, c + j, image_texture, surface_texture, true);
						}
					}
				}
			}
		}
	}
	else
	{
		srcrect.x = 11 * 64; //flag
		if (matrix[r][c] >= 0)
		{
			matrix[r][c] = -1;
			SDL_RenderCopy(renderer, image_texture, &srcrect, &rect);
		}
		else if (matrix[r][c] == -1)//if there is a flag
		{
			matrix[r][c] = matrix1[r][c];
			SDL_RenderCopy(renderer, surface_texture, NULL, &rect);
			SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
			SDL_RenderDrawRect(renderer, &rect);
		}
		SDL_RenderPresent(renderer);
	}
}

int checkWin(const vector< vector<int> >& matrix, int cnt)
{
	int opened = 0;
	int r = matrix.size();
	if (r <= 0) return 0;
	int c = matrix[0].size();
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (matrix[i][j] == -2)
			{
				opened++;
			}
		}
	}
	return opened;
}


void initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		logSDLError(std::cout, "SDL_Init", true);
	window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) logSDLError(std::cout, "CreateWindow", true);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) logSDLError(std::cout, "CreateRenderer", true);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void logSDLError(std::ostream& os,
	const std::string& msg, bool fatal)
{
	os << msg << " Error: " << SDL_GetError() << std::endl;
	if (fatal) {
		SDL_Quit();
		exit(1);
	}
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()

{
	SDL_Event e;
	while (true) {
		if (SDL_WaitEvent(&e) != 0 &&
			(e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
			return;
		SDL_Delay(100);
	}
}
