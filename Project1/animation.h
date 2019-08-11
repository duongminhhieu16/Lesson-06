#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int ANIMATION_FRAMES = 15;

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(SDL_Renderer* renderer, std::string path);
	void free();
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);
	int getWidth();
	int getHeight();
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};
void explosion(SDL_Renderer* renderer);
