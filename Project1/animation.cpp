#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "animation.h"
using namespace std;

SDL_Rect gSpriteClips[ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


void explosion(SDL_Renderer* renderer)
{
	int frame = 0;
	int j = 0;
	gSpriteSheetTexture.loadFromFile(renderer, "explosion.png");
	for (int i = 0; i < 15; i++)
	{
		gSpriteClips[i].w = 600;
		gSpriteClips[i].h = 600;
		if (i < 5)
		{
			gSpriteClips[i].x = i*600;
			gSpriteClips[i].y = 0;
		}
		else if (i >= 5 && i < 10)
		{
			gSpriteClips[i].x = (i-5) * 600;
			gSpriteClips[i].y = 600;
		}
		else
		{
			gSpriteClips[i].x = (i-10) * 600;
			gSpriteClips[i].y = 1200;
		}
	}
	while (j < 3)
	{
		SDL_Rect* current_clip = &gSpriteClips[frame / 15];
		gSpriteSheetTexture.render(renderer, 0, 0, current_clip);
		SDL_RenderPresent(renderer);
		frame++;
		if (frame / 15 >= ANIMATION_FRAMES)
		{
			frame = 0;
			j++;
		}
	}
}
