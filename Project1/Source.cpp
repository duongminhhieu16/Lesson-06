#include <iostream>
#include <SDL.h>
#include <cmath>
#include <SDL_image.h>
#include <ctime>
#include <vector>
#include "mine.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
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
	SDL_Texture* loser = IMG_LoadTexture(renderer, "loser.png");
	SDL_Texture* winner = IMG_LoadTexture(renderer, "winner.png");
	SDL_RenderClear(renderer);


	int r, c, rows = SCREEN_HEIGHT / 25, columns = SCREEN_WIDTH / 25;
	int status = 0;
	int cnt, opened;
	bool playAgain = true;
	vector< vector<int> > matrix(rows, vector<int>(columns));
	vector< vector<int> > matrix1(rows, vector<int>(columns));
	SDL_Rect rect;
	rect.h = 25;
	rect.w = 25;
	srand(time(0));
	do
	{
		if (status == 0)
		{
			SDL_RenderCopy(renderer, game, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		SDL_Delay(10);
		if (SDL_WaitEvent(&e) == 0) continue;
		if (e.type == SDL_QUIT) break;
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x = e.button.x;
			int y = e.button.y;
			if (status == 0)//nothing has been done
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{   //Button
					if (x <= 450 && x >= 350)
					{
						if (y >= 480 && y <= 530) break;//quit
						else if (y >= 320 && y <= 370)
						{
							SDL_RenderCopy(renderer, start, NULL, NULL);
							SDL_RenderPresent(renderer);
							status = choose;
						}
					}
				}

			}
			else if (status == choose)//chose mode
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (x >= 290 && x <= 500)
					{
						if (y >= 320 && y <= 370)
						{
							status = easy;
						}
						else if (y >= 410 && y <= 460)
						{
							status = medium;
						}
						else if (y >= 500 && y <= 550)
						{
							status = hard;
						}
						else if (y >= 590 && y <= 640)
						{
							status = superUltraHard;
						}
					}
				}
				SDL_RenderCopy(renderer, surface_texture, NULL, NULL);
				for (int i = 0; i < 32; i++)
					for (int j = 0; j < 32; j++)
					{
						rect.x = i * 25;
						rect.y = j * 25;
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
			else//when game is on
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					c = x / 25;
					r = y / 25;
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
					}
					else if (matrix[r][c] != lose && matrix[r][c] != win)
					{
						refreshScreen(renderer, matrix, matrix1, r, c, image_texture, surface_texture, true);
						opened = checkWin(matrix, cnt);
						SDL_RenderPresent(renderer);
						if (opened + cnt == 32 * 32)
						{
							for (unsigned int i = 0; i < matrix.size(); i++)
							{
								for (unsigned int j = 0; j < matrix[0].size(); j++)
								{
									matrix[i][j] = win;
								}
							}
							status = win;
						}
					}
				}
				else
				{
					c = x / 25;
					r = y / 25;
					refreshScreen(renderer, matrix, matrix1, r, c, image_texture, surface_texture, false);
				}
			}
		}
		if (e.type == SDL_KEYDOWN)
		{
			if (status == win)
			{
				SDL_RenderCopy(renderer, winner, NULL, NULL);
				SDL_RenderPresent(renderer);
				cout << status;
				status += 100;
			}
			else if (status == lose)
			{
				SDL_RenderCopy(renderer, loser, NULL, NULL);
				SDL_RenderPresent(renderer);
				cout << status;
				status += 100;//wait until key pressed
			}
			else status = 0;
		}
	} while (playAgain);

	SDL_DestroyTexture(image_texture);
	SDL_DestroyTexture(surface_texture);
	SDL_DestroyTexture(game);
	IMG_Quit();
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
	rect.x = c * 25;
	rect.y = r * 25;
	rect.h = 25;
	rect.w = 25;
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
						rect.x = j * 25;
						rect.y = i * 25;
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
